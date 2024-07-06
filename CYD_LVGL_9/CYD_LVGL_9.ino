#include <lvgl.h>

#include <TFT_eSPI.h>

#include <examples/lv_examples.h>
#include <demos/lv_demos.h>

#include <XPT2046_Touchscreen.h>

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240,touchScreenMaximumY = 3800;

/*Set to your screen resolution*/
#define TFT_HOR_RES   320
#define TFT_VER_RES   240

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

TFT_eSPI tft = TFT_eSPI();

/* LVGL calls it when a rendered image needs to copied to the display*/

void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  lv_draw_sw_rgb565_swap(px_map, w * h);
  tft.pushImage(area->x1, area->y1, w, h, (uint16_t *)px_map);

  lv_disp_flush_ready(disp);
}


/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
  if(touchscreen.touched())
  {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if(p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if(p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if(p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if(p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x,touchScreenMinimumX,touchScreenMaximumX,1,TFT_HOR_RES); /* Touchscreen X calibration */
    data->point.y = map(p.y,touchScreenMinimumY,touchScreenMaximumY,1,TFT_VER_RES); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

lv_indev_t * indev; //Touchscreen input device
uint8_t* draw_buf;  //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer

void setup()
{
  //Some basic info on the Serial console
  Serial.begin(115200);
   
  //Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi); /* Touchscreen init */
  touchscreen.setRotation(3); /* Inverted landscape orientation to match screen */

  //Initialise LVGL
  lv_init();
  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t * disp;

  //disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

  disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

  tft.init();
  tft.setRotation(3); //This is the display in landscape
  // For the Micro USB board, this needs to be
  // tft.invertDisplay(0);
  // For the Micro USB & USB C board, we need to invert the display
  // tft.invertDisplay(1);
  tft.invertDisplay(0); 

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
