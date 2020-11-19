#include "ESPino32CAM.h"
#include "ESPino32CAM_QRCode.h"
ESPino32CAM cam;
ESPino32QRCode qr;
#define BUTTON_QR 15
void setup_QRCode() {
  Serial.println("\r\nESPino32CAM");
  if (cam.init() != ESP_OK)
  {
    Serial.println(F("Fail"));
    while (1);
  }
  qr.init(&cam);
  sensor_t *s = cam.sensor();
  s->set_framesize(s, FRAMESIZE_VGA);
  s->set_whitebal(s, true);
  s->set_lenc(s, false);
  s->set_raw_gma(s, false);
  s->set_saturation(s, 2);
  s->set_contrast(s, 1);
  s->set_exposure_ctrl(s, false);
  s->set_aec2(s, false);
  s->set_gain_ctrl(s, false);
  s->set_agc_gain(s, 3);
  s->set_aec_value(s, 600);
  s->set_whitebal(s, false);
  s->set_special_effect(s, 2);
}
void loop_QRCode()
{
  static bool isAqquired = false;
  if (!digitalRead(BUTTON_QR) && !isAqquired)
  {
    FastLEDWhite();
    camera_fb_t *fb = cam.capture();
    if (!fb)
    {
      //    Serial.println("Camera capture failed");
      TelnetStream.println("Camera capture failed");
      return;
    }
    //Serial.write(fb->buf, fb->len);
    dl_matrix3du_t *image_rgb;
    if (cam.jpg2rgb(fb, &image_rgb))
    {
      cam.clearMemory(fb);
      //      cam.printDebug("\r\nQR Read:");
      //      TelnetStream.println("QR Read:");
      qrResoult res = qr.recognition(image_rgb);
      if (res.status)
      {
        //        TelnetStream.println(String("Version: ") + res.version);
        //        TelnetStream.println(String("ECC level: ") + res.eccLevel);
        //        TelnetStream.println(String("Mask: ") + res.mask);
        //        TelnetStream.println(String("Data type: ") + String(qr.dataType(res.dataType)));
        //        TelnetStream.println(String("Length: ") + res.length);
        TelnetStream.println(String("Payload: ") + res.payload);
        //        cam.printfDebug("Version: %d", res.version);
        //        cam.printfDebug("ECC level: %c", res.eccLevel);
        //        cam.printfDebug("Mask: %d", res.mask);
        //        cam.printDebug("Data type: " + qr.dataType(res.dataType));
        //        cam.printfDebug("Length: %d", res.length);
        //        cam.printDebug("Payload: " + res.payload);
        isAqquired = true;
        if (res.payload.length() == 9){
          DFPlayerPlay(res.payload.substring(0,4).toInt());
        }
        
        }
      else
        TelnetStream.println("FAIL");
      //        cam.printDebug("FAIL");
    }
    cam.clearMemory(image_rgb);
  }

  if (digitalRead(BUTTON_QR)) {
    isAqquired = false;
  }
}
