#include <lvgl.h>

//#include <TFT_eSPI.h>

#include <examples/lv_examples.h>
#include <demos/lv_demos.h>

/*
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240,touchScreenMaximumY = 3800;
*/

/*Set to your screen resolution*/
#define TFT_HOR_RES   320
#define TFT_VER_RES   240

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))


#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9341 _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;
//----------------------------------------------------------------------  
public:LGFX(void){
  {
    auto cfg = _bus_instance.config();
    cfg.spi_host   = SPI2_HOST;
    cfg.spi_mode   = 0;
    cfg.freq_write = 40000000;
    cfg.freq_read  = 16000000; 
    cfg.spi_3wire  = false; 
    cfg.use_lock   = true; 
    cfg.dma_channel=  1; 
    cfg.pin_sclk   = 14;
    cfg.pin_mosi   = 13; 
    cfg.pin_miso   = 12;
    cfg.pin_dc     =  2; 
    _bus_instance.config(cfg); 
    _panel_instance.setBus(&_bus_instance);
  }
  { 
    auto cfg = _panel_instance.config();
    cfg.pin_cs          =    15;
    cfg.pin_rst         =    -1;
    cfg.pin_busy        =    -1;
      
    cfg.memory_width    =   320;
    cfg.memory_height   =   240;
    cfg.panel_width     =   320;
    cfg.panel_height    =   240;
    cfg.rgb_order       =  true;
    cfg.offset_rotation =     6;
    
    cfg.offset_x        =     0;
    cfg.offset_y        =     0;
    cfg.dummy_read_pixel=     8;
    cfg.dummy_read_bits =     2;
    cfg.readable        =  true;
    cfg.invert          = false;
    cfg.dlen_16bit      = false;
    cfg.bus_shared      = false;
    _panel_instance.config(cfg);
  }
  {
    auto cfg = _light_instance.config();
    cfg.pin_bl             = 21; 
    cfg.invert          = false;  
    cfg.freq            = 44100;
    cfg.pwm_channel         = 7; 
    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
  }
  { 
    auto cfg = _touch_instance.config();
    cfg.x_min             = 300; 
    cfg.x_max            = 3900; 
    cfg.y_min             = 200;
    cfg.y_max            = 3700; 
    cfg.pin_int            = 36; 
    cfg.bus_shared      = false; 
    cfg.offset_rotation     = 1; 
    cfg.spi_host    = VSPI_HOST;
    cfg.freq          = 1000000;
    cfg.pin_sclk           = 25; 
    cfg.pin_mosi           = 32; 
    cfg.pin_miso           = 39;
    cfg.pin_cs             = 33; 
    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
  }
  setPanel(&_panel_instance);
  }
};
LGFX tft;


/* LVGL calls it when a rendered image needs to copied to the display*/

void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lv_draw_sw_rgb565_swap(px_map, w * h);
  tft.pushImage(area->x1, area->y1, w, h, (uint16_t *)px_map);

  lv_disp_flush_ready(disp);
}


/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
  uint16_t touchX, touchY;

  data->state = LV_INDEV_STATE_REL;

  if(tft.getTouch(&touchX, &touchY))
  {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
  else
    data->state = LV_INDEV_STATE_RELEASED;
}


lv_indev_t * indev; //Touchscreen input device
uint8_t* draw_buf;  //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer

void setup()
{
  //Some basic info on the Serial console
  Serial.begin(115200);

  tft.init();

  //Initialise LVGL
  lv_init();
  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t * disp;

  disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

  //Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);  
  lv_indev_set_read_cb(indev, my_touchpad_read);
 
  lv_demo_widgets();

  Serial.println( "Setup done" );
}

void loop()
{   
    lv_tick_inc(millis() - lastTick); //Update the tick timer. Tick is new for LVGL 9
    lastTick = millis();
    lv_timer_handler();               //Update the UI
    delay(5);
}
