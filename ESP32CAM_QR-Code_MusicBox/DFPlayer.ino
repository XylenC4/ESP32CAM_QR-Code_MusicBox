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

#define VOLUME 10 //Set volume value (0~30).

int oldFolderNr = -1;
bool bRegister[255]; //Played files register
int16_t iRegister[99]; //Folder count register

DFRobotDFPlayerMini myDFPlayer;

void setup_DFPlayer()
{
  Serial.begin(9600);
  myDFPlayer.begin(Serial);
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.reset();     //Reset the module
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  myDFPlayer.volume(VOLUME);  //Set volume value (0~30).
  //Init folder count register
  for ( int i = 0; i < 1000; i++) {
    iRegister[i] = -1;
  }
}

//Play a file
void DFPlayerPlay(int Nr) {
  myDFPlayer.volume(VOLUME);  //Set volume value (0~30).
  if (Nr >= 0 && Nr <= 65000) {
    TelnetStream.println(String("play: SD\\mp3\\") + Nr + String(".mp3"));
    myDFPlayer.playMp3Folder(Nr);
  }
}

//Play a random file in a defined folder
void DFPlayerPlayFolderRandom(int FolderNr) {
  myDFPlayer.volume(VOLUME);  //Set volume value (0~30).
  int FilesInFoler = DFPlayerGetFolderCount(FolderNr);

  if (FilesInFoler < 0) {
    return;
  }

  // Reset the register if the folder has changed
  if (FolderNr != oldFolderNr) {
    DFPlayerResetRegister();
  }
  if (DFPlayerIsAllPlayed(FilesInFoler)) { // Reset the register if all songs are already played
    DFPlayerResetRegister();
  }

  int TrackNr = DFPlayerRandomTrackFromRegister(FilesInFoler) + 1;

  myDFPlayer.playFolder(FolderNr, TrackNr); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  oldFolderNr = FolderNr;
  DFPlayerShowRegister(FilesInFoler);
}

// My DFPlayer has quite often issues to read the file count and returns a -1
// This function will aqquire the folder number and stored the result.
int DFPlayerGetFolderCount(int FolderNr) {
  if (iRegister[FolderNr] == -1) {
    for ( int i = 0; i < 10; i++) {
      // Workarount because the DFPlayer returns at least one time the told value again.
      myDFPlayer.readFileCountsInFolder(FolderNr);
      delay(100);
      myDFPlayer.readFileCountsInFolder(FolderNr);
      delay(100);
      int FilesInFoler = myDFPlayer.readFileCountsInFolder(FolderNr);
      TelnetStream.println(String("Get file count of folder:") + FolderNr + String("-") + FilesInFoler);
      if (FilesInFoler == -1) {
        delay(100);
      }
      else {
        iRegister[FolderNr] = FilesInFoler;
        break;
      }
    }
  }
  TelnetStream.println(String("FileCount:") + iRegister[FolderNr]);
  return iRegister[FolderNr];
}

// Reset
void DFPlayerResetRegister() {
  for ( int i = 0; i < 255; i++) {
    bRegister[i] = false;
  }
}

// Check if all tracks are already played
bool DFPlayerIsAllPlayed(int MaxTrackNr) {
  for ( int i = 0; i < MaxTrackNr; i++) {
    TelnetStream.println("IsPlayed [i]:" + String(i) + "-" + String(bRegister[i]));
    if (bRegister[i] == false) {
      return false;
    }
  }
  TelnetStream.println("true");
  return true;
}

// Play random file
int DFPlayerRandomTrackFromRegister(int MaxTrackNr) {
  // Only try randomize MaxTrackNr*2 times to reduce the changse of a lockup
  for ( int i = 0; i < MaxTrackNr * 2; i++) {
    int rnd = random(0, MaxTrackNr);
    if (!bRegister[rnd]) {
      TelnetStream.println("Play Random [i]:" + String(i) + "[rnd]:" + String(rnd));
      bRegister[rnd] = true;
      return rnd;
    }
  }
  return DFPlayerNextNotPlayedFromRegister(MaxTrackNr);
}

// Wen radomization has failed play the next one
int DFPlayerNextNotPlayedFromRegister(int MaxTrackNr) {
  for ( int i = 0; i < MaxTrackNr; i++) {
    if (!bRegister[i]) {
      TelnetStream.println("NextFromRegister [i]:" + String(i));
      return i;
    }
  }
  return 0;
}

void DFPlayerShowRegister(int MaxTrackNr) {
  for ( int i = 0; i < MaxTrackNr; i++) {
    TelnetStream.println("[" + String(i) + "]:" + String(bRegister[i]));
  }
}
