#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#define VOLUME 30

const int rxPin = D3;
const int txPin = D4;

const int btnPrev = D6;
const int btnPlay = D7;
const int btnNext = D8;

class Mp3Notify;

SoftwareSerial swSerial(rxPin, txPin); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(swSerial);

void setup() {
  Serial.begin(115200);

  pinMode(btnPrev, INPUT);
  pinMode(btnPlay, INPUT);
  pinMode(btnNext, INPUT);

  Serial.println("Initializing...");

  mp3.begin();

  Serial.print("Setting volume to: ");
  Serial.println(VOLUME);
  mp3.setVolume(VOLUME);

  uint16_t count = mp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("Files: ");
  Serial.println(count);
  delay(500);

  Serial.println("Starting...");
  mp3.playMp3FolderTrack(1);

  delay(10);
}

void loop() {
  if (digitalRead(btnPlay) == HIGH) {
    uint16_t status = mp3.getStatus();
    if (status == 513) {
      mp3.pause();
      Serial.println("PAUSED");
    } else if (status == 514) {
      mp3.start();
      Serial.println("RESUMED");
    }
  } else if (digitalRead(btnPrev) == HIGH) {
    mp3.prevTrack();
    Serial.println("PREVIOUS");
  } else if (digitalRead(btnNext) == HIGH) {
    mp3.nextTrack();
    Serial.println("NEXT");
  }

  mp3.loop();
  delay(500);
}

class Mp3Notify {
  public:
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
      if (source & DfMp3_PlaySources_Sd) {
        Serial.print("SD Card, ");
      }

      if (source & DfMp3_PlaySources_Usb) {
        Serial.print("USB Disk, ");
      }

      if (source & DfMp3_PlaySources_Flash) {
        Serial.print("Flash, ");
      }

      Serial.println(action);
    }

    static void OnError(uint16_t errorCode) {
      // see DfMp3_Error for code meaning
      Serial.println();
      Serial.print("Com Error ");
      Serial.println(errorCode);
    }

    static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {
      Serial.print("Play finished for #");
      Serial.println(track);
    }

    static void OnPlaySourceOnline(DfMp3_PlaySources source) {
      PrintlnSourceAction(source, "online");
    }

    static void OnPlaySourceInserted(DfMp3_PlaySources source) {
      PrintlnSourceAction(source, "inserted");
    }

    static void OnPlaySourceRemoved(DfMp3_PlaySources source) {
      PrintlnSourceAction(source, "removed");
    }
};
