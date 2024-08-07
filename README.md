# Sunton-CYD-LVGL (ESP32 2432S028)
Sunton  ESP32 2432S028 (Cheap Yellow Display) LVGL

## Useful information

Manufacturer links (not in English):   
http://www.jczn1688.com/zlxz    
http://www.jczn1688.com/filedownload/536930    

This one has downloads for useful documntation and programs:    
http://pan.jczn1688.com/1/SPI%20%20display%20module  
     
Nice wiki:     https://wiki.makerfabs.com/Sunton_ESP32_2.8_inch_240x320_TFT_with_Touch.html   
Information:   https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display    
Tutorial:      https://randomnerdtutorials.com/cheap-yellow-display-esp32-2432s028r/    
LovyanGFX config here: https://github.com/lovyan03/LovyanGFX/issues/519   

These boards come in different types   
These are useful sites for explaining the differences:   

https://github.com/rzeldent/platformio-espressif32-sunton    
https://github.com/rzeldent/esp32-smartdisplay    

<p align="center">
  <img src="https://github.com/paulhamsh/Sunton-CYD-LVGL/blob/main/sunton_esp32_2432S028.jpg" width="400" title="Sunton 2432S028 (Cheap Yellow Display)">
</p>

These boards come in different types   
I have tested with two boards below, and their LovyanGFX settings are different for rotation, RGB order and width and height.    The seem to need different drivers.   

| Board                    | Driver  |    
|--------------------------|---------|   
| MicroUSB                 | ILI9341 |   
| MicroUSB + USB C (2 USB) | ST7789  |   
| USB C                    |         |   

The examples in this report use either TFT_eSPI and XPT2046_Touchsrceen OR Lovyan GFX    

```
LVGL                   9.0.0
TFT_eSPI               2.5.34
XPT2046_Touchscreen    1.4
Lovyan GFX             1.1.16
```

The Arduino board setup is:
```
ESP32 Dev Module
FLash Mode DIO
```

As usual for LVGL, move the ```demos``` and ```examples``` libraries to ```src```    

Copy this ```lv_conf_.h``` to ```src\lv_conf.h```    
If using ```TFT_eSPI``` then copy this file:
```
User_Setup.h     Arduino\libraries\TFT_eSPI\User_Setup.h
```

## Lovyan GFX settings

### MicroUSB
```
Device
    lgfx::Panel_ILI9341 _panel_instance;
Board
    cfg.memory_width    =   240;
    cfg.memory_height   =   320;
    cfg.panel_width     =   240;
    cfg.panel_height    =   320;
    cfg.rgb_order       = false;
    cfg.offset_rotation =     1;
    cfg.invert          = false;

    cfg.offset_x        =     0;
    cfg.offset_y        =     0;
    cfg.dummy_read_pixel=     8;
    cfg.dummy_read_bits =     2;
    cfg.readable        =  true;
    cfg.dlen_16bit      = false;
    cfg.bus_shared      = false;
Touch
    cfg.offset_rotation =     6;
```

### MicroUSB + USB C (aka 2 USB)
```
Device
    lgfx::Panel_ST7789 _panel_instance;
Board
    cfg.memory_width    =   240;
    cfg.memory_height   =   320;
    cfg.panel_width     =   240;
    cfg.panel_height    =   320;
    cfg.rgb_order       = false;
    cfg.offset_rotation =     3; 
    cfg.invert          = false; 

    cfg.offset_x        =     0;
    cfg.offset_y        =     0;
    cfg.dummy_read_pixel=     8;
    cfg.dummy_read_bits =     2;
    cfg.readable        =  true;
    cfg.dlen_16bit      = false;
    cfg.bus_shared      = false;
Touch
    cfg.offset_rotation =     4;
```
## TFT_eSPI settings

### MicroUSB   

Copy the ```User_Setup.h``` provided   

### MicroUSB + USB C (aka 2 USB)

Copy the ```User_Setup.h``` provided      







