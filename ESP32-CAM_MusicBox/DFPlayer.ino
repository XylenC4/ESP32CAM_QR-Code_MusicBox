#include <DFPlayerMini_Fast.h>

void printError();

#define VOLUME 15 //Set volume value (0~30).
int oldFolderNr = -1;
bool bRegister[255]; //Played files register
int16_t iRegister[99]; //Folder count register


DFPlayerMini_Fast myMP3;

void setup_DFPlayer()
{

  TelnetStream.println("DFPlayer: Initialisation");
  Serial.begin(9600);
  delay(500); // Initialisation delay
  myMP3.begin(Serial);

  while (myMP3.currentMode() < 0) {
    TelnetStream.println("DFPlayer: No response -> SDCard inseret?");
    FastLEDColor(CRGB::Red);
  }

  if (myMP3.numSdTracks() <= 0) {
    TelnetStream.println("DFPlayer: No files on the SDCard");
    FastLEDColor(CRGB::Red);
    delay(10000);
    ESP.restart();
  }

  TelnetStream.println("DFPlayer: Online");

  myMP3.volume(VOLUME);

  //Init folder count register
  for ( int i = 0; i < 1000; i++) {
    iRegister[i] = -1;
  }
}

void loop_DFPlayer()
{
  printError();
}

//Play a file
void DFPlayerPlay(int Nr) {
  myMP3.volume(VOLUME);  //Set volume value (0~30).
  if (Nr >= 0 && Nr <= 65000) {
    TelnetStream.println(String("DFPlayer: play: SD\\mp3\\") + Nr + String(".mp3"));
    myMP3.playFromMP3Folder(Nr);
  }
}

//Play a random file in a defined folder
void DFPlayerPlayFolderRandom(int FolderNr) {
  if (FolderNr!=50) {
    myMP3.volume(VOLUME);  //Set volume value (0~30).
  }
  else {
    myMP3.volume(30);  //Set volume value (0~30).
  }
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

  myMP3.playFolder(FolderNr, TrackNr); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  oldFolderNr = FolderNr;
  DFPlayerShowRegister(FilesInFoler);
}

// My DFPlayer has quite often issues to read the file count and returns a -1
// This function will aqquire the folder number and stored the result.
int DFPlayerGetFolderCount(int FolderNr) {
  if (iRegister[FolderNr] == -1) {
    for ( int i = 0; i < 10; i++) {
      // Workarount because the DFPlayer returns at least one time the told value again.
      myMP3.numTracksInFolder(FolderNr);
      delay(100);
      myMP3.numTracksInFolder(FolderNr);
      delay(100);
      int FilesInFoler = myMP3.numTracksInFolder(FolderNr);
      TelnetStream.println(String("DFPlayer: Get file count of folder:") + FolderNr + String("-") + FilesInFoler);
      if (FilesInFoler == -1) {
        delay(100);
      }
      else {
        iRegister[FolderNr] = FilesInFoler;
        break;
      }
    }
  }
  TelnetStream.println(String("DFPlayer: FileCount:") + iRegister[FolderNr]);
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
    TelnetStream.println("DFPlayer: IsPlayed [i]:" + String(i) + "-" + String(bRegister[i]));
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
      TelnetStream.println("DFPlayer: Play Random [i]:" + String(i) + "[rnd]:" + String(rnd));
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
      TelnetStream.println("DFPlayer: NextFromRegister [i]:" + String(i));
      return i;
    }
  }
  return 0;
}

void DFPlayerShowRegister(int MaxTrackNr) {
  for ( int i = 0; i < MaxTrackNr; i++) {
    TelnetStream.println("DFPlayer: [" + String(i) + "]:" + String(bRegister[i]));
  }
}


void printError()
{
  if (myMP3.recStack.commandValue == 0x40)
  {
    switch (myMP3.recStack.paramLSB)
    {
      case 0x1:
        {
          TelnetStream.println("DFPlayer: Module busy (this info is returned when the initialization is not done)");
          break;
        }
      case 0x2:
        {
          TelnetStream.println("DFPlayer: Currently sleep mode(supports only specified device in sleep mode)");
          break;
        }
      case 0x3:
        {
          TelnetStream.println("DFPlayer: Serial receiving error(a frame has not been received completely yet)");
          break;
        }
      case 0x4:
        {
          TelnetStream.println("DFPlayer: Checksum incorrect");
          break;
        }
      case 0x5:
        {
          TelnetStream.println("DFPlayer: Specified track is out of current track scope");
          break;
        }
      case 0x6:
        {
          TelnetStream.println("DFPlayer: Specified track is not found");
          break;
        }
      case 0x7:
        {
          TelnetStream.println("DFPlayer: Insertion error(an inserting operation only can be done when a track is being played)");
          break;
        }
      case 0x8:
        {
          TelnetStream.println("DFPlayer: SD card reading failed(SD card pulled out or damaged)");
          break;
        }
      case 0xA:
        {
          TelnetStream.println("DFPlayer: Entered into sleep mode");
          break;
        }
      default:
        {
          TelnetStream.print("DFPlayer: Unknown error: ");
          TelnetStream.println(myMP3.recStack.paramLSB);
          break;
        }
    }
  }
}
