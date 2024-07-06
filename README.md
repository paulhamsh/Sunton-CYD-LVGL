# Sunton-CYD-LVGL
Sunton  ESP32 2432S028 (Cheap Yellow Display) LVGL

## Useful information

Manufacturer links (not in English):   
http://www.jczn1688.com/zlxz    
http://www.jczn1688.com/filedownload/536930    

This on has downloads for useful documntation and programs:    
http://pan.jczn1688.com/1/SPI%20%20display%20module  
     
Nice wiki:     https://wiki.makerfabs.com/Sunton_ESP32_2.8_inch_240x320_TFT_with_Touch.html   
Information:   https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display    
Tutorial:      https://randomnerdtutorials.com/cheap-yellow-display-esp32-2432s028r/    
LovyanGFX config here: https://github.com/lovyan03/LovyanGFX/issues/519   


<p align="center">
  <img src="https://github.com/paulhamsh/Sunton-CYD-LVGL/blob/main/sunton_esp32_2432S028.jpg" width="400" title="Sunton 2432S028 (Cheap Yellow Display)">
</p>

These boards come in different types   
I have tested with:   
### MicroUSB
```
**Board**
    cfg.memory_width    =   240;
    cfg.memory_height   =   320;
    cfg.panel_width     =   240;
    cfg.panel_height    =   320;

    cfg.offset_x        =     0;
    cfg.offset_y        =     0;
    cfg.offset_rotation =     6;
    cfg.dummy_read_pixel=     8;
    cfg.dummy_read_bits =     2;
    cfg.readable        =  true;
    cfg.invert          = false;
    cfg.rgb_order       = true;
    cfg.dlen_16bit      = false;
    cfg.bus_shared      = false;
**Touch**
    cfg.offset_rotation = 1;
```

### MicroUSB + USB C
```
**Board**
    cfg.memory_width    =   240;
    cfg.memory_height   =   320;
    cfg.panel_width     =   240;
    cfg.panel_height    =   320;

    cfg.offset_x        =     0;
    cfg.offset_y        =     0;
    cfg.offset_rotation =     6;
    cfg.dummy_read_pixel=     8;
    cfg.dummy_read_bits =     2;
    cfg.readable        =  true;
    cfg.invert          = false;
    cfg.rgb_order       = true;
    cfg.dlen_16bit      = false;
    cfg.bus_shared      = false;
**Touch**
    cfg.offset_rotation = 1;
```
This is a useful site for explaining the differences:

https://github.com/rzeldent/platformio-espressif32-sunton    
https://github.com/rzeldent/esp32-smartdisplay    

