// thanks to dfrobot
// https://www.dfrobot.com/wiki/index.php/Gravity:_I2C_OLED-2864_Display
// https://www.dfrobot.com/wiki/index.php/Light_Disc_(SKU:DFR0106)
// https://www.dfrobot.com/product-1121.html

#include <DFRobotDFPlayerMini.h>
#include "SoftwareSerial.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

// pin setup
int B = 4; //Connect Blue led to Digital pin 3
int R = 2;//Connect Red led to Digital pin 5
int G = 3;//Connect Green led to Digital pin 6
int Bu = 9; //Connect Blue led to Digital pin 3
int Ru = 5;//Connect Red led to Digital pin 5
int Gu = 6;//Connect Green led to Digital pin 6

// brightness level for leds
int brightLevel = 255;

// set to normal mode
bool instagibMode = false;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 12;     // the number of the pushbutton pin
const int buttonPin1 = 13;     // the number of the pushbutton1 pin

// button state variables
int buttonState = 0;         // variable for reading the pushbutton status
int buttonState1 = 0; 

// variables for button timing
long buttonHeld;        // will store time button held down
const long waitTime = 100000;           // wait time for trigger

// setup serial
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX

// setup mp3 device
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  // begin serial (for debugging etc.)
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  // mp3 module initialisation  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  // decent volume that doesnt push the speaker/batteries too far  
  myDFPlayer.volume(20);  // Set volume value. From 0 to 30

  // set pins to outputs
  pinMode(10, OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);

  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

}

void loop()
{
  // picture loop
  u8g.firstPage();  
  do {

    // read the state of the pushbutton values:
    buttonState = digitalRead(buttonPin);
    buttonState1 = digitalRead(buttonPin1);

    // play standard beam sound if trigger is pulled, but if held down for a bit will play shock core sound
    if (buttonState == LOW) {
          while (buttonState == LOW) {buttonState = digitalRead(buttonPin); buttonHeld = ++buttonHeld;}
          Serial.println(buttonHeld);
          // check if the button held down time is over the configured waiting time 
          if(buttonHeld > waitTime) { 
             myDFPlayer.play(2);  // play the shock core soundfx
             delay(1000);
          }
         else {
             myDFPlayer.play(1);  // play the shock beam soundfx
             delay(1000);
          }
          // reset button timer
          buttonHeld = 0;
       }

    // if the other button is pressed then invert the instagib boolean to change modes
    if (buttonState1 == HIGH) {
      instagibMode = !instagibMode;
      delay(100);
      Serial.println("button2");
    }
    // check the instagib boolean and draw oled/set led colours as necessary
    if (instagibMode == false){  
      normal();
      drawNormal();
    }
    else if (instagibMode == true) {
      instagib();
      drawInstagib();
    }
  } while( u8g.nextPage() );
  
  delay(2);  
}

// set beam colour to normal (purple)
void normal()
{
  analogWrite(B,brightLevel);  
  analogWrite(R,brightLevel);  
  analogWrite(G,0);
  analogWrite(Bu,brightLevel);  
  analogWrite(Ru,brightLevel);  
  analogWrite(Gu,0);
  delay(80);  
}

// set beam colour to instagib (red)
void instagib()
{
  analogWrite(B,0);  
  analogWrite(R,brightLevel);  
  analogWrite(G,0);
  analogWrite(Bu,0);  
  analogWrite(Ru,brightLevel);  
  analogWrite(Gu,0);
  delay(80);  
}

// draw normal mode onto the oled
void drawNormal(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 10, 40, "Mode: Normal");
}

// draw instagib mode onto the oled
void drawInstagib(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 10, 40, "Mode: Instagib");
}