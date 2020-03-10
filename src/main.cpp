/*
  EZ-Set Test001
  Feb2020

  Description: Set DS3232 time in UTC or else you won't
             generate the correct numbers.

  Set Baudrate: stty -F /dev/ttyUSB0 115200
  Check Baudrate: stty -a -F /dev/ttyUSB0
  Send time set command: echo -e "`date --utc +%Y,%-m,%-d,%-H,%-M,%-S,`" > /dev/ttyUSB0
  Example of time string: 2020,2,24,8,41,49,
  Watch Serial: cu -l /dev/ttyUSB0 -s 115200

  EZSet Hardware

  +----------------------------------+
  | +---+ Battery Compartment +---+  |                              |
  | |I2C| +--+ +--++--+ +--+ / CR  \ |
  | |RTC| |  | |  ||  | |  | \ 2032/ |
  | +---+ |AA| |AA||AA| |AA|  +---+  |
  |       |  | |  ||  | |  |         |
  |       +--+ +--++--+ +--+         |
  |       +----------------+         |
  |       | Arduino ProMini|         |
  |       +----------------+         |
  |     +--------------------+       |
  |     | EZSet {o} Mainboard|       |
  |     +--------^-----------+       |
  |            Button  +-----------+ |
  |          |\\\\\\|--| DC Motor  |-|
  |            /SML\   +-----------+ |
  |           | Gear|                |
  |       +----\___/---------+       |
  |      /                    \      |
  |     |     Large Gear       |     |
  |     |         (+)          |     |
  |  __ | \                    | __  |
  |  \ \ \|\_____            |/ / /  |
  |   \ \ |+----------------+| / /   |
  |    \_\| Limit2    Limit1 |/_/    |
  |  Contact Left     Right Contact  |
  +----------------------------------+

  Lock \ Left Limit 2
  Lock / Right Limit 1
 
  
  Arduino Pro Mini

                   3V3  D0   D1   RST
         GND  GND  VCC  RX   TX   /DTR
      +--------------------------------+
      |  [ ]  [ ]  [ ]  [ ]  [ ]  [ ]  |
      |              FTDI              |
  D1  | [ ]1/TX                 RAW[ ] |  3.4V to 12V
  D0  | [ ]0/RX    SCL/19/A5[ ] GND[ ] |  GND
  C6  | [ ]RST     SDA/18/A4[ ] RST[ ] |  C6
  GND | [ ]GND       +-+        VCC[ ] |  3.3V
  D2  | [ ]2/INT0   /   \     17/A3[ ] |  C3
  D3  |~[ ]3/INT1  /PRO  \    16/A2[ ] |  C2
  D4  | [ ]4       \ MINI/    15/A1[ ] |  C1
  D5  |~[ ]5        \___/     14/A0[ ] |  C0
  D6  |~[ ]6         +-+     SCK/13[ ] |  B5
  D7  | [ ]7          A7[ ] MISO/12[ ] |  B4
  B0  | [ ]8          A6[ ] MOSI/11[ ]~|  B3
  B1  |~[ ]9                  SS/10[ ]~|  B2
      |           [RST-BTN]            |
      +--------------------------------+

  PWM ~3, ~5, ~6, ~9, ~10 and ~11

  00  RX
  01  TX
  02  Limit Switch 1
  03~ Buzzer
  04  Limit Switch 2
  05~ Motor Right
  06~ Motor Left
  07  1 2 3 Horizontal Buttons
  08  4 5 6 Horizontal Buttons
  09~ 7 8 9 Horizontal Buttons
  10~ EZSET Button LED Green -
  11~ EZSET Button LED Red -
  12  0 EZS Horizontal Buttons
  13  Arduino LED + NumPad LED +
  A0  1 4 7 0   Vertical Buttons
  A1  2 4 8 EZS Vertical Buttons
  A2  3 6 9     Vertical Buttons
  A3  Low Battery or Current Detection or Door Contact?
  A4  SDA I2C Bus Extender
  A5  SCL I2C Bus Extender
  A6
  A7

  EZSet Mainboard

  +----------------------------------------------------------+
  |==  20 19 18 17 16 15 14 13 12 11  {O} Left HBridge       |
  | []  |  |  |  |  |  |  |  |  |  |  SW3    +-+ Motor       |
  |( ) +----------------------------+  +-+   | |  [:]        |
  |PSU |                            |  | |   +-+ +-+   ( ~ ) |
  |    +----------------------------+  +-+       | |   Buzzer|
  |BAT  |  |  |  |  |  |  |  |  |  |  EEPROM     +-+         |
  |[:] 01 02 03 04 05 06 07 08 09 10       Right HBridge     |
  +----------------------------------------------------------+

  01 TX
  02 RX
  03 Contact 1
  04 Contact 2
  05 GND Resistor?
  06 GND
  07 Header 1 Pin 3
  08 Header 1 Pin 2
  09 Motor L
  10 Motor R
  11 SDL EEPROM N/C
  12 SCL EEPROM N/C
  13 Buzzer
  14 Low Battery or Current Detection?
  15 VCC
  16 N/C
  17 Header 2 Pin 2
  18 Switch 3
  19 Switch 2
  20 Switch 1

  EZSet Keypad

              +---------+
             /  {EZSet}  \
   +--------+             +-------+
  / {1}{2}{3}{4}{5}{6}{7}{8}{9}{0} \
  | 20 19 18 17 16 15 14 13 12 11  |
  |  |  |  |  |  |  |  |  |  |  |  |
  | +----------------------------+ |
  | |                            | |
  | +----------------------------+ |
  |  |  |  |  |  |  |  |  |  |  |  |
  | 01 02 03 04 05 06 07 08 09 10  |
  +--------------------------------+

  01 N/C
  02 RX
  03 TX
  04 RFModule
  05 RFModule
  06 GND
  07 N/C
  08 NumPad LED +
  09 EZSET Button LED Green -
  10 EZSET Button LED Red -
  11 1 2 3 Horizontal Buttons
  12 4 5 6 Horizontal Buttons
  13 7 8 9 Horizontal Buttons
  14 0 EZS Horizontal Buttons
  15 VCC
  16 SDL EEPROM N/C
  17 SDC EEPROM N/C
  18 1 4 7 0   Vertical Buttons
  19 2 5 8 EZS Vertical Buttons
  20 3 6 9     Vertical Buttons

  DS3232 RTC

  GND VCC SDA SCL SQW 32K
          ||||||
         +------+
         |123456|
        /|      |\
        ||[ ]+-+||
        ||   | |||
        \|   +-+|/
         | 1234 |
         +------+
      GND VCC SDA SCL
  
  01 32K 
  02 SQW
  03 SCL 04
  04 SDA 03
  05 VCC 02
  06 GND 01

  Pin Header

  4 Pin Header

  01 Red 3.3v
  02 Brown EZSET Button LED Red (-) -> Arduino 11
  03 Orange EZSET Button LED Green (-) -> Arduino 10
  04 Black Ground

  8 Pin Header

  Pink Side -> Black Side

  01 3 6 9 Vertical Buttons -> Arduino A2
  02 2 5 8 EZS Vertical Buttons -> Arduino A1
  03 1 4 7 0   Vertical Buttons -> Arduino A0
  04 NumPad LED + -> Arduino 13
  05 0 EZS Horizontal Buttons Arduino 12
  06 7 8 9 Horizontal Buttons Arduino 09
  07 4 5 6 Horizontal Buttons Arduino 08
  08 1 2 3 Horizontal Buttons Arduino 07

rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

*/

// Comment this out to disable serial debug print.
#define DEBUG 

#ifdef DEBUG
  //#include "DebugUtils.h" // Debug Serial Print.
  long baud = 115200; //74880,115200;
#endif

#include <Arduino.h>
// Keypad Mark Stanley, Alexander Brevig V3.1.1 : https://github.com/Chris--A/Keypad
#include <Keypad.h>
//#include <Servo.h>
// DS3232RTC Jack Christensen V1.2.10 : https://github.com/JChristensen/DS3232RTC
#include <DS3232RTC.h>
// Adafruit_PN532 Adafruit V1.2.0 : https://github.com/adafruit/Adafruit-PN532/
#include <Adafruit_PN532.h>
// swRTC Leonardo Miliani V1.2.8 : https://github.com/leomil72/swRTC
//#include <swRTC.h>
// SimpleHOTP Jonathan Lusky V1.0.1 : https://github.com/jlusPrivat/SimpleHOTP
#include <sha1.h>
// TOTP Luca Dentella V1.0.1 : https://github.com/lucadentella/TOTP-Arduino
#include <TOTP.h>
// Streaming Mikal Hart V5 : https://github.com/janelia-arduino/Streaming
#include <Streaming.h> 

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
// Use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Music Notes
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0
  /*,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
  
  */
};

// Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

// Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0, 0
  /*,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
  */
};

// Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

// Button press melody
int buttonpress_melody[] = {
  NOTE_C4
};

// Button press tempo
int buttonpress_tempo[] = {
  12, 12, 12, 12
};

// Constants won't change. They're used here to set pin numbers:
const int limit1Pin = 2;     // The number of the limit switch pin.
const int melodyPin = 3;     // The number of the buzzer pin.
const int limit2Pin = 4;     // The number of the limit switch pin.
const int motorRPin = 5;     // The number of the motor left pin.
const int motorLPin = 6;     // The number of the motor right pin.
const int led1Pin   = 10;
const int led2Pin   = 11;
const int led3Pin   = 13;    // The number of the LED pin.
const int auxPin    = 17;    // The number of the AUX pin.

// Variables will change:
int limit1State = 0;         // Variable for reading the pushbutton status.
int limit2State = 0;         // Variable for reading the pushbutton status.
int auxState    = 0;         // Variable for reading the AUX status.

tmElements_t tm;

bool limit1ran = false;
bool limit2ran = false;
bool turnoff = false;
// 0 Null; 1 Right; 2 Left
int direction = 0;

// https://arduino.stackexchange.com/questions/21619/detect-when-rfid-card-is-absent
// 
// http://www.lucadentella.it/OTP/
// http://www.lucadentella.it/2013/09/14/serratura-otp/
// https://zxing.org/w/decode.jspx
// http://asciiqr.com/
// https://chart.googleapis.com/chart?chs=200x200&chld=M|0&cht=qr&chl=' + urlencoded
// https://emn178.github.io/online-tools/base32_encode.html
// https://www.barcodefaq.com/ascii-chart-char-set/
// https://github.com/lucadentella/TOTP-Arduino
// Shared Secret (hmacKey hex): NSBeepBoop
// Google Authenticator code (BASE32 of shared secret above): JZJUEZLFOBBG633Q
// Raw QR Text: otpauth://totp/Front Door Lock?secret=JZJUEZLFOBBG633Q
// ASCII QR Creation: http://asciiqr.com/index.php?i=&t=otpauth%3A%2F%2Ftotp%2FNonaSuomy%3Fsecret%3DJZJUEZLFOBBG633Q
/*
   ▄▄▄▄▄▄▄  ▄  ▄ ▄  ▄▄▄  ▄▄▄▄▄▄▄  
   █ ▄▄▄ █ █▄▄██ █▀ █▀▄█ █ ▄▄▄ █  
   █ ███ █ ▄▀▀▀ ▀▀▀█▄▀   █ ███ █  
   █▄▄▄▄▄█ █▀▄▀▄ █▀▄ ▄▀▄ █▄▄▄▄▄█  
   ▄▄ ▄  ▄▄▀▀▄ █ ▄▄█▀█ █ ▄▄▄ ▄▄   
   ▀███▀▀▄█▄▄█▀ ▄ ▀▀▄▀▄▀▄▀▄ ▀▄█▀  
   ▀██ ▀▀▄█▀    ▀▀▄ ▄▀ ▀█▄▀▀▄▀▀▄  
   ▀▀   ▄▄ ██▀█▀▄▄▀▄▀█ █ ▄▄▄█ ▄▄  
   █ ██▄▀▄▀█ ▄  █ ▄ █▀█▄▀ ▄   █   
   ▄ ▀ █ ▄ ▀▄▄▀ ▄▀▀▀   ▄█▀█  █▄█  
   ▄ ▄▄▀ ▄██▄▄▄  ██ ▀▀██▄▄▄█▀▄▄   
   ▄▄▄▄▄▄▄ █▄  ▄▀▄  █▄▀█ ▄ ████▀  
   █ ▄▄▄ █  ▀▄▀▀▀▀▀▀▄▀ █▄▄▄██▀▀▄  
   █ ███ █ ▀  ▄ ▄   ▀███ ███ █ ▄  
   █▄▄▄▄▄█ █▄▄ ██ ▀▀█ ▀█ █▄▀█ █   


*/


// ASCII Number:     78=N  83=S  66=B  101=e 101=e 112=p 66=B  111=o 111=o 112=p
uint8_t hmacKey[] = {0x4e, 0x53, 0x42, 0x65, 0x65, 0x70, 0x42, 0x6f, 0x6f, 0x70};
TOTP totp = TOTP(hmacKey, 10);

// Keypad configuration
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'0','E'}
};
byte rowPins[rows] = {7, 8, 9, 12};
byte colPins[cols] = {14, 15, 16};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//Servo doorServo;

//swRTC rtc;
char* totpCode;
char inputCode[7];
int inputCode_idx;
boolean lockOpen;

void sing(int s);
void buzz(int targetPin, long frequency, long length);
void displayRTC_Time();
void print2digits(int number);
void printDateTime(time_t t);
void printTime(time_t t);
void printDate(time_t t);
void printI00(int val, char delim);

// The setup function runs once when you press reset or power the board
void setup() {
  //RTC.set(DateTime(F(__DATE__), F(__TIME__)));
  #ifdef DEBUG
  // Initialize serial communications at 9600 bps:
    Serial.begin(115200);
  #endif
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(limit1Pin, INPUT);
  pinMode(limit2Pin, INPUT);
  pinMode(melodyPin, OUTPUT);
  pinMode(motorRPin, OUTPUT);
  pinMode(motorLPin, OUTPUT);
  pinMode(auxPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  #ifdef DEBUG
    Serial.println(F("EZ-Set Test Started..."));
  #endif
  /*
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    #ifdef DEBUG
    Serial.print(F("Didn't find PN53x board"));
    #endif
    //while (1); // halt
  }
  // Got ok data, print it out!
  #ifdef DEBUG
  Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print(F('.')); Serial.println((versiondata>>8) & 0xFF, DEC);
  #endif
  // configure board to read RFID tags
  nfc.SAMConfig();
  #ifdef DEBUG
  Serial.println(F("Waiting for an ISO14443A Card ..."));
  #endif
  */
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus() != timeSet) {
  ;
  #ifdef DEBUG
    Serial.println(F("Unable to sync with the RTC"));
  #endif
  } 
  else
  {
  ;
  #ifdef DEBUG
    Serial.println(F("RTC has set the system time"));
  #endif
  }
  // Init software RTC with the current time
  //rtcs.stopRTC();
  //rtcs.setDate(10, 3, 2020);
  //rtcs.setTime(1, 29, 00);
  //rtcs.startRTC();
  #ifdef DEBUG
  Serial.println(F("RTC initialized and started"));
  #endif
  // reset input buffer index
  inputCode_idx = 0;
  //displayRTC_Time();
}

int song = 0;

void sing(int s) {
  // Iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println(F(" 'Underworld Theme'"));
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(melodyPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);
    }
  } else if (song == 3) {
    #ifdef DEBUG
    Serial.println(F(" 'Button Press'"));
    #endif
    int size = sizeof(buttonpress_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / buttonpress_tempo[thisNote];

      buzz(melodyPin, buttonpress_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);
    }
  } else {
    #ifdef DEBUG
    Serial.println(F(" 'Mario Theme'"));
    #endif
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(led3Pin, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// Multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW);  // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(led3Pin, LOW);
}

void displayRTC_Time()
{
  //read time from RTC
  int status = RTC.read(tm); //TimeElements variable
  #ifdef DEBUG
  Serial.print(F("Ok, RTC Time = "));
  print2digits(tm.Hour);
  Serial.write(':');
  print2digits(tm.Minute);
  Serial.write(':');
  print2digits(tm.Second);
  Serial.print(F(", Date (D/M/Y) = "));
  Serial.print(tm.Day);
  Serial.write('/');
  Serial.print(tm.Month);
  Serial.write('/');
  Serial.print(tmYearToCalendar(tm.Year));
  Serial.println();
  //unix time
  Serial.print(F("Unix Time "));
  Serial.println(RTC.get());
  #endif
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  #ifdef DEBUG
  Serial.print(number);
  #endif
}
/*
// print date and time to Serial
void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    printTime(t);
}


// print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}

// print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << monthShortStr(month(t)) << _DEC(year(t));
}

// Print an integer in "00" format (with leading zero),
// followed by a delimiter character to Serial.
// Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}
*/
char rx_byte = 0;
String rx_str = "";
boolean not_number = false;
int result;
char code[7];
// The loop function runs over and over again forever.
void loop() {
  
    if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n') {
      // end of string
      if (not_number) {
        #ifdef DEBUG
        Serial.println(F("Not a number"));
        #endif
      }
      else {
        // multiply the number by 2
        result = rx_str.toInt() * 2;
        #ifdef DEBUG
        // print the result
        Serial.print(rx_str);
        Serial.print(F(" x 2 = "));
        Serial.print(result);
        Serial.println(F(""));
        Serial.println(F("Enter a number to multiply by 2."));
        #endif
      }
      not_number = false;         // reset flag
      rx_str = "";                // clear the string for reuse
    }
    else {
      // non-number character received
      not_number = true;    // flag a non-number
    }
  } // end: if (Serial.available() > 0)
  long UNIX = RTC.get();
  char* newCode = totp.getCode(UNIX);
  if(strcmp(code, newCode) != 0) {
    strcpy(code, newCode);
    #ifdef DEBUG
    displayRTC_Time();
    Serial.println(code);
    #endif
  }
  static time_t tLast;
  time_t t;
  //tmElements_t tm;
  t = now();
  if (t != tLast) {
    tLast = t;
    
    if (second(t) == 0) {
      //printDateTime(t);
      float c = RTC.temperature() / 4.;
      //float f = c * 9. / 5. + 32.;
      //Serial.print << F("  ") << c << (F(" C  ")) << f << (F(" F"));
      Serial.print(c);
      Serial.print(F(" C"));
      Serial.println(F(""));
    }
    
    //Serial.print << endl;
  }
  char key = keypad.getKey();
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  // a key was pressed
  if (key != NO_KEY) {
    digitalWrite(led3Pin, HIGH);
    sing(3);
    // # resets the input buffer    
    if(key == 'E') {
      #ifdef DEBUG
      Serial.println(F("E pressed, resetting the input buffer..."));
      #endif
      inputCode_idx = 0;      
    //}
    
    // * closes the door
    //else if(key == 'E') {

      if(lockOpen == false) {
      ;
      #ifdef DEBUG
        Serial.println(F("E pressed but the door is already closed"));
      #endif
      }
      else {
        #ifdef DEBUG
        Serial.println(F("E pressed, closing the door..."));
        #endif
        if (direction == 2) {
           limit2ran = true;
        }
        //for(int i = 0; i < SERVO_OPENED - SERVO_CLOSED; i++) {
          //doorServo.write(SERVO_OPENED - i);
          //delay(SERVO_DELAY);
        //}
        lockOpen = false;
      }
    } else {      
      
      // Save key value in input buffer
      inputCode[inputCode_idx++] = key;
      
      // If the buffer is full, add string terminator, reset the index
      // get the actual TOTP code and compare to the buffer's content
      if(inputCode_idx == 6) {
        
        inputCode[inputCode_idx] = '\0';
        inputCode_idx = 0;
        #ifdef DEBUG
        Serial.print(F("New code inserted: "));
        Serial.println(inputCode);
        #endif
        //long GMT = rtc.getTimestamp();
        //displayRTC_Time();
        #ifdef DEBUG
        Serial.println(F(""));
        #endif
        totpCode = totp.getCode(UNIX);
        
        // Code is ok :)
        if(strcmp(inputCode, totpCode) == 0) {
          
          if(lockOpen == true) Serial.println(F("Code ok but the door is already open"));
          
          else {
            #ifdef DEBUG
            Serial.println(F("Code ok, opening the door..."));
            #endif
            digitalWrite(led1Pin, HIGH); 
            digitalWrite(led2Pin, LOW);
            delay(1000);
            digitalWrite(led2Pin, HIGH);
            sing(1);
            //for(int i = 0; i < SERVO_OPENED - SERVO_CLOSED; i++) {
              //doorServo.write(SERVO_CLOSED + i);
              //delay(SERVO_DELAY);
            //}
            if (direction == 1) {
              limit1ran = true;
            }
            lockOpen = true;
          }
          
        // Code is wrong :(  
        } else {
          #ifdef DEBUG
          Serial.print(F("Wrong code... the correct was: "));
          #endif
          digitalWrite(led2Pin, HIGH);
          digitalWrite(led1Pin, LOW);
          delay(1000);
          digitalWrite(led1Pin, HIGH);
          #ifdef DEBUG
          Serial.println(totpCode);
          #endif
          sing(2);        
        }
      }      
    }
  }
  /*
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    #ifdef DEBUG
    Serial.println(F("Found an ISO14443A card"));
    Serial.print(F("  UID Length: "));Serial.print(uidLength, DEC);Serial.println(F(" bytes"));
    Serial.print(F("  UID Value: "));
    #endif
    nfc.PrintHex(uid, uidLength);
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];  
      cardid <<= 8;
      cardid |= uid[3];
      #ifdef DEBUG
      Serial.print(F("Seems to be a Mifare Classic card #"));
      Serial.println(cardid);
      #endif
    }
    #ifdef DEBUG
    Serial.println(F(""));
    #endif
  }
  */

  // Read the state of the limit switches.
  limit1State = digitalRead(limit1Pin);
  limit2State = digitalRead(limit2Pin);
  auxState = digitalRead(auxPin);
  
  //int sensorValue = analogRead(A3);
  //Serial.println(sensorValue);
  //delay(1);        // delay in between reads for stability
  if (auxState == HIGH) {
    //#ifdef DEBUG
    //Serial.println(F("AUX High"));
    //#endif
  } else {
    //#ifdef DEBUG
    //Serial.println(F("AUX Low"));
    //#endif
    turnoff = true;
    /*
    if (limit1ran == true) {
      limit1ran = false;
      limit2ran = true;
    }
    else if (limit2ran == true {
      limit1ran = true;
      limit2ran = false;
    }
  }
  if (limit1ran == true) {
    limit2ran = false;
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, HIGH);
  }
  if (limit2ran == true) {
    limit1ran = false;
    digitalWrite(motorRPin, LOW);
    digitalWrite(motorLPin, HIGH);
  }*/
  }
  if (turnoff == false) {
    // Check if the limit switch is pressed. If it is, the limit1State is HIGH:
    if (limit1State == HIGH) {
      // Limit switch knob to the right.
      Serial.println(F("limit1State==HIGH"));
      direction = 2;
      // Turns lock back and forth test (turn left).
      //limit2ran = false;
      //limit1ran = true;
      // Turn off both motor directions.
      limit2ran = false;
      limit1ran = false;
      // turn LED/Motor on:
      //digitalWrite(led3Pin, HIGH);
      //digitalWrite(motorRPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      if (limit1ran == true) {
        //sing(2);
        //#ifdef DEBUG
        //Serial.println(F("Limit Switch 1 High."));
        //#endif
        //limit1ran = false;
      }
    } 
    else if (limit1State == LOW) {
      direction = 0;
      if (limit1ran == true) {
        // turn LED off:
        //digitalWrite(led3Pin, LOW);
        //digitalWrite(motorRPin, LOW);    // turn the LED off by making the voltage LOW
        //#ifdef DEBUG
        //Serial.println(F("Limit Switch 1 Low."));
        //#endif
        //limit1ran = false;
      }
    }
    // Check if the limit switch is pressed. If it is, the limit1State is HIGH:
    if (limit2State == HIGH) {
      // Limit switch knob to the left.
      Serial.println(F("limit2State==HIGH"));
      direction = 1;
      // Turns lock back and forth test (turn right).
      //limit1ran = false;
      //limit2ran = true;
      limit1ran = false;
      limit2ran = false;
      if (limit2ran == true) {
        // turn LED on:
        //digitalWrite(led3Pin, HIGH);
        //digitalWrite(motorLPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        //sing(1);
        //#ifdef DEBUG
        //Serial.println(F("Limit Switch 2 High."));
        //#endif
        //limit2ran = false;
      }
    } 
    else if (limit2State == LOW) {
      direction = 0;
      if (limit2ran == true) {
        // turn LED off:
        //digitalWrite(led3Pin, LOW);
        //digitalWrite(motorLPin, LOW);    // turn the LED off by making the voltage LOW
        //#ifdef DEBUG
        //Serial.println(F("Limit Switch 2 Low."));
        //#endif
        //limit2ran = false;
      }
    }
  }
  /*
  if (limit1State == HIGH && limit2State == HIGH) {
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, LOW);
    limit1ran = false;
    limit2ran = false;
    if (turnoff == true){
      turnoff = false;
      #ifdef DEBUG
      Serial.println(F("turnoff = false."));
      #endif
    }
    else if (turnoff == false){
      turnoff = true;
      #ifdef DEBUG
      Serial.println(F("turnoff = true."));
      #endif
    }
  }
  */
  if (limit1ran == true) {
    Serial.println(F("limit1ran==true Turning left"));
    limit2ran = false;
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, HIGH);
  }
  if (limit2ran == true) {
    Serial.println(F("limit2ran==true Turning right"));
    limit1ran = false;
    digitalWrite(motorRPin, LOW);
    digitalWrite(motorLPin, HIGH);
  }
}