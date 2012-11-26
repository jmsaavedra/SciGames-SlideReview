#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>

boolean RFID_GO = false;
boolean haveInfoToSend = false;
byte infoToSend[5];
int numBytesToSend = 5;
boolean currLedStatus = false; //true==on

AndroidAccessory acc("Joe Saavedra",
"SG SlideGame Review",
"SciGames Slide Review Board",
"1.0",
"http://jos.ph",
"0000000012345678");

void setup() {
  // connect to the serial port
  Serial.begin(9600);  
  Serial.println("SETUP BEGIN...");
  initLeds();
  
  delay(500);   
  acc.powerOn();
  Serial.println("...SETUP DONE");
}

void loop () {

  checkForAndroidComm();

  if(RFID_GO){ //received the on RFID page message
    RFID_reader();
  }
}

void checkForAndroidComm(){

  byte msgIn[3];
  if (acc.isConnected()) {
    int len = acc.read(msgIn, sizeof(msgIn), 1);

    //    Serial.println( "len: " + len );
    if (len > 0) {
      Serial.println("-------msg------");
      Serial.print((char)msgIn[0]);
      Serial.println((char)msgIn[1]);

      Serial.println("-------  -------");
      char command = (char)msgIn[1];

      Serial.println("command: ");
      Serial.println(command);
      Serial.println();
      ledsOff();

      switch(command) {
      case 'Y':
        Serial.println("received Y"); //prepare for RFID Scan
        Serial3.begin(9600);
        delay(50); //let serial initialize
        ledsOff();
        RFID_GO = true;
        break;

      case 'Z':
        Serial.println("received Z"); //turn LEDs on/off = standby
        RFID_GO = false;
        currLedStatus = !currLedStatus;
        allLeds(currLedStatus);
        break;
      }
    } 
    else {
      if(haveInfoToSend){
        //redOn(); // turn on light
        acc.write(infoToSend, numBytesToSend);
        delay(1000);
        haveInfoToSend = false;
        ledsOff();
      }
    }
  } 
  else {
    //solid green when not connected to board
    RFID_GO = false;
    haveInfoToSend = false;
    greenOn();
  }
}

void reset(){
  haveInfoToSend = false;
  RFID_GO = false;
  ledsOff();
}







