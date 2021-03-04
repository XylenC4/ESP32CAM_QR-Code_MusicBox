# ESP32CAM QR-Code MusicBox
This is an Kids-Toy which plays music by an QR-Code

# Informations
## Hardware
Used Hardware: ESP32CAM
- Pins: https://github.com/ldab/ESP32-CAM-MQTT/blob/master/pics/esp32cam_pinout.png
- Buy: https://www.aliexpress.com/wholesale?SearchText=ina226
- OTA-Mod: When you plan to use the OTA you have to change the SPI-Flash from 4MB to 16MB!
         This requires knowledge and Hardware to decap the Shield of the ESP32 and solder a new one on!
          I'll used 16MB SPI Flash W25Q128FVSG (other manufacturers will work also)
          Buy: https://www.aliexpress.com/wholesale?SearchText=W25Q128FVSG


# Libaries
INA226Lib by Peter Buchegger/Peter Buchegger https://www.arduino.cc/reference/en/libraries/ina226lib/
- Buy: https://www.aliexpress.com/wholesale?SearchText=ina226
DFPlayer by DFRobot/Angelo  https://www.arduino.cc/reference/en/libraries/dfrobotdfplayermini/
- Buy: https://www.aliexpress.com/wholesale?SearchText=DFPlayer
ESPino32CAM by Github User ThaiEasyElec https://github.com/ThaiEasyElec/ESPIno32CAM
After downloading the Libary you have to edit the Pins-Declaration inside \Arduino\libraries\ESPIno32CAM\src\ESPino32CAM.h
If your LED on the board lights on, you have forgot to edit the pins!
CAMERA_MODEL_AI_THINKER
```bash
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
```

# OTA Implementation
OTA Implementation: (everything is already included)
Andreas Spiess https://github.com/SensorsIot/ESP32-OTA
Video: https://www.youtube.com/watch?v=1pwqS_NUG7Q
OTA-Mod: When you plan to use the OTA you have to change the SPI-Flash from 4MB to 16MB!
         This requires knowledge and Hardware to decap the Shield of the ESP32 and solder a new one on!
         I'll used 16MB SPI Flash W25Q128FVSG (other manufacturers will work also)
         Buy: https://www.aliexpress.com/wholesale?SearchText=W25Q128FVSG

         After solderig-in a new Flash Chip you have to add a new board by yourself.
         Add the following lines in \AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.X.X\boards.txt below "esp32cam.build.code_debug=0"
         After Restarting the Arduino IDE you have to select the Board "AI Thinker ESP32-CAM 16MB Flash"
         
```bash
esp32cam16mb.name=AI Thinker ESP32-CAM 16MB Flash

esp32cam16mb.upload.tool=esptool_py
esp32cam16mb.upload.maximum_size=3145728
esp32cam16mb.upload.maximum_data_size=327680
esp32cam16mb.upload.wait_for_upload_port=true
esp32cam16mb.upload.speed=460800

esp32cam16mb.serial.disableDTR=true
esp32cam16mb.serial.disableRTS=true

esp32cam16mb.build.mcu=esp32
esp32cam16mb.build.core=esp32
esp32cam16mb.build.variant=esp32
esp32cam16mb.build.board=ESP32_DEV
esp32cam16mb.build.flash_size=16MB
esp32cam16mb.build.partitions=default_16MB
esp32cam16mb.build.defines=-DBOARD_HAS_PSRAM -mfix-esp32-psram-cache-issue
esp32cam16mb.build.extra_libs=-lc-psram-workaround -lm-psram-workaround
esp32cam16mb.build.code_debug=0

esp32cam16mb.menu.CPUFreq.240=240MHz (WiFi/BT)
esp32cam16mb.menu.CPUFreq.240.build.f_cpu=240000000L
esp32cam16mb.menu.CPUFreq.160=160MHz (WiFi/BT)
esp32cam16mb.menu.CPUFreq.160.build.f_cpu=160000000L
esp32cam16mb.menu.CPUFreq.80=80MHz (WiFi/BT)
esp32cam16mb.menu.CPUFreq.80.build.f_cpu=80000000L
esp32cam16mb.menu.CPUFreq.40=40MHz (40MHz XTAL)
esp32cam16mb.menu.CPUFreq.40.build.f_cpu=40000000L
esp32cam16mb.menu.CPUFreq.26=26MHz (26MHz XTAL)
esp32cam16mb.menu.CPUFreq.26.build.f_cpu=26000000L
esp32cam16mb.menu.CPUFreq.20=20MHz (40MHz XTAL)
esp32cam16mb.menu.CPUFreq.20.build.f_cpu=20000000L
esp32cam16mb.menu.CPUFreq.13=13MHz (26MHz XTAL)
esp32cam16mb.menu.CPUFreq.13.build.f_cpu=13000000L
esp32cam16mb.menu.CPUFreq.10=10MHz (40MHz XTAL)
esp32cam16mb.menu.CPUFreq.10.build.f_cpu=10000000L

esp32cam16mb.menu.FlashMode.qio=QIO
esp32cam16mb.menu.FlashMode.qio.build.flash_mode=dio
esp32cam16mb.menu.FlashMode.qio.build.boot=qio
esp32cam16mb.menu.FlashMode.dio=DIO
esp32cam16mb.menu.FlashMode.dio.build.flash_mode=dio
esp32cam16mb.menu.FlashMode.dio.build.boot=dio
esp32cam16mb.menu.FlashMode.qout=QOUT
esp32cam16mb.menu.FlashMode.qout.build.flash_mode=dout
esp32cam16mb.menu.FlashMode.qout.build.boot=qout
esp32cam16mb.menu.FlashMode.dout=DOUT
esp32cam16mb.menu.FlashMode.dout.build.flash_mode=dout
esp32cam16mb.menu.FlashMode.dout.build.boot=dout

esp32cam16mb.menu.FlashFreq.80=80MHz
esp32cam16mb.menu.FlashFreq.80.build.flash_freq=80m
esp32cam16mb.menu.FlashFreq.40=40MHz
esp32cam16mb.menu.FlashFreq.40.build.flash_freq=40m
```
