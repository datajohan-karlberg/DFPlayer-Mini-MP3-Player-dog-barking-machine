/***************************************************
  DFPlayer - A Mini MP3 Player For Arduino
  <https://www.dfrobot.com/product-1121.html>

 ***************************************************
  This example shows the basic function of library for DFPlayer.

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
// sample changed to be a dog brking machine

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int dogtime = 0;
int plays = 0;
boolean x1 = 0;
int x2 = 0;
//******************************************
void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  //myDFPlayer.enableLoopAll();
}
//******************************************



void loop()
{
  //******************************************
  // count down dogtime
  static unsigned long timer = millis();
  if (millis() - timer > 1000) {
    timer = millis();
    if (dogtime > 0) {
      dogtime = dogtime - 1 ;
      Serial.print(F("Dogimer:"));
      Serial.println(dogtime);
    }
  }

  //******************************************
  // input reading with filter. makes no false alarm
  if (digitalRead(2) == x1) {
    x2 = 0;
  }
  else {
    x2 = x2 + 1;
  }
  if (x2 > 100) {
    x2 = 0;
    x1 = x1 ^ 1;
  }


  //******************************************
  if (x1 != 0) {
    dogtime = 30;// how many seconds dog barking you want
    if (plays == 0) {
      plays = 1;
      int del = random(1, 4); // first 4 files is bark starting
      myDFPlayer.play(del);  //Play the first mp3
      Serial.println(F("Start"));
    }

  }
  //******************************************


  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  //******************************************
  delay(1);
}


//******************************************
void printDetail(uint8_t type, int value) {

  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case 11://DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      plays = 0;
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  // if still have dogtime and MP# is finish - play another one
  if ( type == 11 && dogtime > 0) {
    int del = random(20, 500);
    delay(random(20, 500));
    del = random(1, 40);
    myDFPlayer.play(del);
    Serial.print(F("Number:"));
    Serial.print(del);
    Serial.println(F(" Play Start!"));
    plays = 1;
  }

}
