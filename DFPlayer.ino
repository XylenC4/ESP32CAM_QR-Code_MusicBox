/***************************************************
  DFPlayer - A Mini MP3 Player For Arduino
  <https://www.dfrobot.com/product-1121.html>

 ***************************************************
  This example shows the all the function of library for DFPlayer.

  Created 2016-12-07
  By [Angelo qiao](Angelo.qiao@dfrobot.com)

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
  All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
  1.Connection and Diagram can be found here
  <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
  2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;

void setup_DFPlayer()
{
  Serial.begin(9600);
  myDFPlayer.begin(Serial);
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.reset();     //Reset the module
  
  //----Set volume----
  myDFPlayer.volume(2);  //Set volume value (0~30).

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);

  //----Set device we use SD as default----
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
  //  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);

  //----Mp3 control----
  //  myDFPlayer.sleep();     //sleep
  //  myDFPlayer.reset();     //Reset the module
  //  myDFPlayer.enableDAC();  //Enable On-chip DAC
  //  myDFPlayer.disableDAC();  //Disable On-chip DAC
  //myDFPlayer.outputSetting(true, 5); //output setting, enable the output and set the gain to 15
}

void DFPlayerPlay(int Nr) {
  if (Nr >= 0 && Nr <= 65000) {
    TelnetStream.println(String("play: SD\\mp3\\") + Nr + String(".mp3"));
    myDFPlayer.playMp3Folder(Nr);
  }
}
