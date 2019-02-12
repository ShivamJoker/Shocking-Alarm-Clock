#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// include button library
// #include <Bounce2.h>
#include <JC_Button.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//Set the time of now
unsigned int hour = 8;
unsigned int minute = 2;
unsigned int second = 50;

String AMorPM = "PM";

//Set the alaram variables
unsigned int Al_hour = 8;
unsigned int Al_minute = 1;


String Al_AMorPM = "PM";

// Editing mode
bool editHH;
bool editMM;
bool editAmPm;



bool settingsMode;
bool alarmMode;
bool shockMode;
bool editShockP;
bool editShockT;
bool oledOn = true;


unsigned int modeBtnCount = 1;
bool modeBtnPressed = false;

// Declare for how much time u want shock in seconds
unsigned int shockTime = 20;



float batteryVoltage;
float ledVoltage;

//use millis to increase second
unsigned long TpreviousMillis = 0;
unsigned long DpreviousMillis = 0;

const long Tinterval = 983;

const long Dinterval = 10000;
// adjust according to your hardware


Button DecreaseBtn(5);
Button ModeBtn(6);
Button IncreaseBtn(7);

#define buzzerPin 11
#define shockPin 10

const unsigned char BatteryCharging [] PROGMEM = {
        0x7f, 0xff, 0xf8, 0x80, 0x00, 0x06, 0x80, 0x00, 0x02, 0x80, 0x00, 0x06, 0x80, 0x30, 0x03, 0x86,
        0x3c, 0x03, 0x83, 0xff, 0x03, 0x80, 0xff, 0x83, 0x80, 0x30, 0x03, 0x80, 0x00, 0x06, 0x80, 0x00,
        0x02, 0x80, 0x00, 0x06, 0x7f, 0xff, 0xfc
};

// 'battery full', 24x13px
const unsigned char BatteryFull [] PROGMEM = {
        0x7f, 0xff, 0xf8, 0xf9, 0xef, 0x3c, 0xf9, 0xef, 0x3c, 0xf9, 0xef, 0x3c, 0xf9, 0xef, 0x3f, 0xf9,
        0xef, 0x3f, 0xf9, 0xef, 0x3f, 0xf9, 0xef, 0x3f, 0xf9, 0xef, 0x3f, 0xf9, 0xef, 0x3c, 0xf9, 0xef,
        0x3c, 0xf9, 0xef, 0x3c, 0x7f, 0xff, 0xf8
};

// 'battery down', 24x13px
const unsigned char BatteryDown [] PROGMEM = {
        0x7f, 0xff, 0xf8, 0x80, 0x00, 0x06, 0x80, 0x00, 0x02, 0x80, 0x00, 0x06, 0x80, 0x00, 0x03, 0x80,
        0x00, 0x03, 0x80, 0x00, 0x03, 0x80, 0x00, 0x03, 0x80, 0x00, 0x03, 0x80, 0x00, 0x06, 0x80, 0x00,
        0x02, 0x80, 0x00, 0x06, 0x7f, 0xff, 0xfc
};


void setup() {

        Serial.begin(115200);
        // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
                Serial.println(F("SSD1306 allocation failed"));
                for (;;); // Don't proceed, loop forever
        }

        // Initialize the buttons

        DecreaseBtn.begin();
        ModeBtn.begin();
        IncreaseBtn.begin();

// Define buzzer prin
        pinMode(buzzerPin, OUTPUT);
        pinMode(shockPin, OUTPUT);


}

void setAlaram(){
        if (Al_minute > 60) {
                Al_minute = 59;
        }
        if (Al_minute > 59) {
                Al_minute = 0;

        }

        if (Al_hour > 12) {
                Al_hour = 1;
        }


        if (Al_hour < 1) {
                hour = 12;
        }

        String Al_time = String(Al_hour) + ":" + String(Al_minute) + " " + String(Al_AMorPM);

        if (Al_minute < 10) {
                Al_time = String(Al_hour) + ":" + "0" + String(Al_minute) + " " + String(Al_AMorPM);
        }

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(10, 0);
        display.println(Al_time);

        display.setTextSize(1);

        display.setCursor(25, 20);
        display.println("Set Alaram");
        if (editHH) {
                display.setCursor(10, 10);

        } else if (editMM) {
                display.setCursor(50, 10);
        }
        else if (editAmPm) {
                display.setCursor(80, 10);
        }
        display.println("__");

        display.display();



}

void openSettings(){
        display.setTextSize(1);
        display.setCursor(35, 20);
        display.println("Set Time");
}


void showTime(){
        if (minute > 60) {
                minute = 59;
        }
        if (minute > 59) {
                minute = 0;

        }

        if (hour > 12) {
                hour = 1;
        }


        if (hour < 1) {
                hour = 12;
        }

        String time = String(hour) + ":" + String(minute) + ":" + String(second);


        if (minute < 10) {
                time = String(hour) + ":" + "0" + String(minute) + ":" + String(second);
        }

        if (second < 10) {
                time = String(hour) + ":" + String(minute) + ":" + "0" + String(second);
        }

        if (second < 10 && minute < 10) {
                time = String(hour) + ":" + "0" + String(minute) + ":" + "0" + String(second);
        }




        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        if (hour < 10) {
                display.setCursor(5, 0);
        } else{
                display.setCursor(0, 0);
        }
        display.println(time);
        display.setCursor(100, 0);
        display.println(AMorPM);
        if (settingsMode) {
                openSettings();
        }
        if (settingsMode) {
                if (editHH) {
                        display.setCursor(1, 10);

                } else if (editMM) {
                        display.setCursor(40, 10);
                }
                else if (editAmPm) {
                        display.setCursor(100, 10);
                }
                display.println("__");
        }
        if (!settingsMode) {
                if (ledVoltage > 3.5) {
                        display.drawBitmap(5,19, BatteryCharging, 24,13, WHITE);
                }
                else if (batteryVoltage > 4) {
                        display.drawBitmap(5,19, BatteryFull, 24,13, WHITE);
                }   else if (batteryVoltage < 3.7) {
                        display.drawBitmap(5,19, BatteryDown, 24,13, WHITE);
                }
        }

        display.display();
}




void increaseTime(){
        //  Store the currentMillis
        unsigned long TcurrentMillis = millis();

        if (TcurrentMillis - TpreviousMillis >= Tinterval) {
                // save the last time you blinked the LED
                TpreviousMillis = TcurrentMillis;
                second++;
                if (second > 59) {
                        second = 0;
                        minute++;
                        if (minute > 59) {
                                minute = 0;
                                hour++;
                                if (hour == 12) {
                                        if (AMorPM == "PM") {
                                                AMorPM = "AM";
                                        } else if (AMorPM == "AM") {
                                                AMorPM = "PM";
                                        }
                                }
                        }
                }

        }

}

// define function to show shock settings
void shockSettings(){

        if (editShockT) {
                if (DecreaseBtn.wasPressed()) {
                        shockTime-=10;
                }
                if (IncreaseBtn.wasPressed()) {
                        shockTime+=10;
                }
        }

        if (shockTime > 50) {
                shockTime = 0;
        }


        // make shock strings
        String shockTimeStr = "Time  " + String(shockTime) + "s";

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println(shockTimeStr);
        display.display();
}



void loop() {


        increaseTime();

        DecreaseBtn.read();
        ModeBtn.read();
        IncreaseBtn.read();

        if (settingsMode) {
                if (editHH) {

                        if (DecreaseBtn.wasPressed()) {
                                hour--;
                        }
                        if (IncreaseBtn.wasPressed()) {
                                hour++;
                        }

                }
                else if (editMM) {

                        if (DecreaseBtn.wasPressed()) {
                                minute--;
                        }
                        if (IncreaseBtn.wasPressed()) {
                                minute++;
                        }

                }
                else if (editAmPm) {

                        if (DecreaseBtn.wasPressed()) {
                          if (AMorPM == "PM") {
                            AMorPM = "AM";
                          } else if (AMorPM = "AM") {
                            AMorPM = "PM";
                        }}
                        if (IncreaseBtn.wasPressed()) {
                          if (AMorPM == "PM") {
                            AMorPM = "AM";
                          } else if (AMorPM = "AM") {
                            AMorPM = "PM";
                        }

                }

      }
    }

        if (alarmMode) {
                if (editHH) {

                        if (DecreaseBtn.wasPressed()) {
                                Al_hour--;
                        }
                        if (IncreaseBtn.wasPressed()) {
                                Al_hour++;
                        }

                }
                else if (editMM) {

                        if (DecreaseBtn.wasPressed()) {
                                Al_minute--;
                        }
                        if (IncreaseBtn.wasPressed()) {
                                Al_minute++;
                        }

                }  else if (editAmPm) {

                        if (DecreaseBtn.wasPressed()) {
                          if (Al_AMorPM == "PM") {
                            Al_AMorPM = "AM";
                          } else if (Al_AMorPM = "AM") {
                            Al_AMorPM = "PM";
                          }

                        }
                        if (IncreaseBtn.wasPressed()) {
                          if (Al_AMorPM == "PM") {
                            Al_AMorPM = "AM";
                          } else if (Al_AMorPM == "AM") {
                            Al_AMorPM = "PM";
                          }
                        }

                }
        }

        if ( ModeBtn.pressedFor(1000) && !settingsMode ) {
                modeBtnPressed = true;
                settingsMode = true;

        }



        if ( ModeBtn.wasPressed() && settingsMode) {
                modeBtnCount++;
        }




        if (settingsMode) {
                switch (modeBtnCount) {
                case 1:
                        editHH = true;
                        break;
                case 2:
                        editHH = false;
                        editMM = true;
                        break;
                case 3:
                        editMM = false;
                        editAmPm = true;
                        break;
                case 4:
                        editAmPm = false;
                        settingsMode = false;
                        alarmMode = true;
                        modeBtnCount = 0;

                }
        }

        if (ModeBtn.wasPressed() && alarmMode) {
                modeBtnCount++;

        }



        if (!alarmMode && !shockMode && oledOn) {
                showTime();
        }

        unsigned long DcurrentMillis = millis();


        if ( ModeBtn.wasPressed() || IncreaseBtn.wasPressed() || DecreaseBtn.wasPressed() && !oledOn) {
                oledOn = true;
                DpreviousMillis = DcurrentMillis;
        }


        // turn off oled after 5 seconds
        if (DcurrentMillis - DpreviousMillis >= Dinterval && oledOn && !settingsMode) {
                DpreviousMillis = DcurrentMillis;
                oledOn = false;
                display.clearDisplay();
                display.display();
        };


        if (alarmMode) {
                switch (modeBtnCount) {
                case 1:
                        editHH = true;
                        break;
                case 2:
                        editHH = false;
                        editMM = true;
                        break;
                case 3:
                        editMM = false;
                        editAmPm = true;
                        break;
                case 4:
                        editAmPm = false;
                        settingsMode = false;
                        alarmMode = false;
                        shockMode = true;
                        modeBtnCount = 0;

                }

                setAlaram();
        }
        if (ModeBtn.wasPressed() && shockMode) {
                modeBtnCount++;

        }

        if(shockMode) {
                switch (modeBtnCount) {
                case 1:
                        editShockT = true;
                        break;

                case 2:
                        editShockT = false;
                        shockMode = false;
                        modeBtnCount = 1;
                        break;
                }
        }


        if (shockMode) {
                shockSettings();
        }

        // ring the alaram if it matches with the Time
        if (Al_hour == hour && Al_minute == minute && second < 10) {
                // divide shock power with 100 and multiply to 255 to generate that amount of pwm
                digitalWrite(buzzerPin, HIGH);
        } else{
                digitalWrite(buzzerPin, LOW);

        }

        if (Al_hour == hour && Al_minute == minute && second < shockTime+10 && second > 10 && second%2 == 0) {
                // divide shock power with 100 and multiply to 255 to generate that amount of pwm
                digitalWrite(shockPin, HIGH);
        } else{
                digitalWrite(shockPin, LOW);

        }

        // read the input on analog pin 0:
        int batteryValue = analogRead(A0);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        batteryVoltage = ((batteryValue * 5.0) / 1023.0 ) - 0.3;
        // read the input on analog pin 0:

        // print out the value you read
        Serial.println(batteryVoltage);

        int ledValue = analogRead(A1);
        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        ledVoltage = ((ledValue * 5.0) / 1023.0 ) - 0.3;
        // read the input on analog pin 0:

        Serial.println("LED voltage here");
        // print out the value you read
        Serial.println(ledVoltage);

        delay(50);

      }
