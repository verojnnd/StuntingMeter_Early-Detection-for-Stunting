//BENERIN Ultrasonik
#include <FS.h>
#include "Free_Fonts.h"

#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <EEPROM.h>

#include "SRF05.h"
#include <SimpleKalmanFilter.h>

TFT_eSPI tft = TFT_eSPI();

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

// Keypad start position, key sizes and spacing
#define KEY_X 105 // Centre of key
#define KEY_Y 118 
#define KEY_W 110 // Width and height
#define KEY_H 35
#define KEY_SPACING_X 25 // X and Y gap
#define KEY_SPACING_Y 5
#define KEY_TEXTSIZE 1

// Numeric display box size and location
#define DISP_X 5
#define DISP_Y 40
#define DISP_W 469 //320
#define DISP_H 47 //50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

#define RefreshTime 200

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

ButtonWidget btnSTART = ButtonWidget(&tft);
ButtonWidget btnCAL = ButtonWidget(&tft);
ButtonWidget btnBOY = ButtonWidget(&tft);
ButtonWidget btnGIRL = ButtonWidget(&tft);
ButtonWidget btnL = ButtonWidget(&tft);
ButtonWidget btnR = ButtonWidget(&tft);
ButtonWidget btnL2 = ButtonWidget(&tft);
ButtonWidget btnR2 = ButtonWidget(&tft);
ButtonWidget btn2 = ButtonWidget(&tft);
ButtonWidget btnRCM = ButtonWidget(&tft);
ButtonWidget btnSTD = ButtonWidget(&tft);
ButtonWidget btnCALCULATE = ButtonWidget(&tft);
ButtonWidget btnRES = ButtonWidget(&tft);
ButtonWidget btnBackCAL = ButtonWidget(&tft);
ButtonWidget btnSAVE = ButtonWidget(&tft);

#define BUTTON_W 200
#define BUTTON_H 60

#define BUTTON_W1 80
#define BUTTON_H1 60

#define BUTTON_W2 120
#define BUTTON_H2 60

#define BUTTON_W3 160
#define BUTTON_H3 60

#define RECUMBENT 0
#define STANDING 1

#define BOY 0
#define GIRL 1

#define INDONESIA 0
#define ENGLISH 1

// Number length, buffer for storing it and character index
#define NUM_LEN 10
#define NUM_LEN2 5
#define NUM_LEN3 4
char bodyWeight[NUM_LEN2 + 1] = "";
char numberBuffer[NUM_LEN + 1] = "";
char tanggalLahir[NUM_LEN + 1] = "";
char calibrate[NUM_LEN2 + 1] = "";

// We have a status line for messages
#define STATUS_X 240 // Centred on this
#define STATUS_Y 90 //95

int displayState = 1;
int VSensor = 25;
const int buzzer = 26;
const int trigger = 32;
const int echo    = 33;

long waktu;
float jarak;

float newDistance, ave, newAve, newAver, newAverage;
#define n_data 50 //4
float data[n_data];
int i_data = 0;
unsigned long previousMillis = 0, currentMillis;
const unsigned long interval = 2000;
float alpha = 0.2, filteredDistance;

uint8_t numberIndex = 0;
uint8_t indexWeight = 0;
uint8_t indexLahir = 0;
uint8_t indexCalibrate = 0;

boolean gender;
boolean method;
int usia = 40;
int days = 0;
int usiaHari;
int usiaBulan;
int usiaBulanTemp;
float tinggiBadan;
float regresi;
float simpangan = 0;
float akhirCAL;

int currentDate ;
int currentMonth; 
int currentYear; 

int tempTime = 0;

int birthDate ;
int birthMonth;
int birthYear ;

int inputValue = 0;
int ref_voltage = 5.0;
float adcValue = 0.0;
float voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;

float length, beratBadan;
float sd_TBU, m_TBU, zscore_TBU;
float l_BBU, m_BBU, s_BBU, zscore_BBU;
float l_BBTB, m_BBTB, s_BBTB, zscore_BBTB;
float imt, l_IMTU, m_IMTU, s_IMTU, zscore_IMTU;
String kategori_TBU, kategori_BBU, kategori_BBTB, kategori_IMTU;

#include "TBU.h"
#include "BBU.h"
#include "BBTB.h"
#include "IMTU.h"

// Create 15 keys for the keypad
char keyLabel[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "0", "Back" };
char keyLabel2[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "Back" };
char keyLabel3[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "-" };
uint16_t keyColor[15] = {TFT_RED, TFT_BROWN, TFT_DARKGREEN,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_DARKGREY
                        };
uint16_t keyColor3[15] = {TFT_RED, TFT_BROWN, TFT_DARKGREEN,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE
                        };

unsigned long lastFrame = millis();

SRF05 SRF(trigger, echo);
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[15], key1[15],key2[15],key3[15];

// Create an array of button instances to use in for() loops
ButtonWidget* btn[] = {&btnSTART,&btnCAL,&btnBOY,&btnGIRL,&btnRCM,&btnSTD,&btnCALCULATE,&btnRES,&btnBackCAL,&btnSAVE};;
uint8_t buttonCount = sizeof(btn) / sizeof(btn[0]);

//-------------BUTTON PressAction----------
void btnSTART_pressAction(void){
  if(btnSTART.justPressed()){
    tft.fillScreen(TFT_BLUE);
    tft.setCursor(tft.width()/2-80,tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Starting.");
    delay(1000);
    tft.setCursor(tft.width()/2-80,tft.height()/2);
    tft.print("Starting..");
    delay(1000);
    tft.setCursor(tft.width()/2-80,tft.height()/2);
    tft.print("Starting...");
    delay(1000);

    initButtonBOY();
    initButtonGIRL();
    displayState = 2;
  }
}

void btnBOY_pressAction(void){
  if(btnBOY.justPressed()){
    Serial.println("BOY");
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillScreen(TFT_BLUE);
    tft.setCursor(tft.width()/2-35, tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Boy");
    delay(2000);

    gender = BOY;
    displayState = 3;
    drawKeypadWEIGHT();
  }
}

void btnGIRL_pressAction(void){
  if(btnGIRL.justPressed()){
    Serial.println("GIRL");
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillScreen(TFT_BLUE);
    tft.setCursor(tft.width()/2-35, tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Girl");
    delay(2000);

    gender = GIRL;
    displayState = 3;
    drawKeypadWEIGHT();
  }
}

void btnRCM_pressAction(void) {
  if (btnRCM.justPressed()) {
    tft.fillScreen(TFT_RED);
    tft.setCursor(140,tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Recumbent");
    delay(2000);

    Serial.println("RECUMBENT");
    method = RECUMBENT; //recumbent
    tft.fillScreen(TFT_BROWN);
    tft.setCursor(30,40);
    tft.setTextSize(1);
    tft.print("Make sure the ground is clear !");
    tft.setCursor(tft.width()/2-135,tft.height()/2+80);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print("CLICK TO START CALCULATE");
    initButtonCALCU();
    displayState = 9;
    movingAverage_Clear();
  }
}

void btnSTD_pressAction(void) {
  if (btnSTD.justPressed()) {
    tft.fillScreen(TFT_RED);
    tft.setCursor(155,tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Standing");
    delay(2000);

    Serial.println("STANDING");
    method = STANDING; //standing
    tft.fillScreen(TFT_BROWN);
    tft.setCursor(30,40);
    tft.setTextSize(1);
    tft.print("Make sure the ground is clear !");
    tft.setCursor(tft.width()/2-135,tft.height()/2+80);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print("CLICK TO START CALCULATE");
    initButtonCALCU();
    displayState = 9;
  }
}

void btnCALCULATE_pressAction(void){
  if(btnCALCULATE.justPressed()){
    tft.fillScreen(TFT_BROWN);
    tft.setTextColor(TFT_WHITE);
    // tft.setFreeFont(LABEL2_FONT);
    // tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setFreeFont(LABEL2_FONT);
    tft.setTextSize(1);
    tft.setCursor(60,tft.height()/2);
    tft.print("Height (Cm) : ");
    tft.setCursor(tft.width()/2,tft.height()/2);
    tft.setTextSize(2);
 
    if(usiaBulan>24 && method == RECUMBENT){
      tinggiBadan = newAverage + 0.7;
    }
    else if(usiaBulan<24 && method == STANDING){
      tinggiBadan = newAverage - 0.7;
    }
    else{
      tinggiBadan = newAverage;
    }

    Serial.println(tinggiBadan);
    // tinggiBadan = round(tinggiBadan, 1);
    tft.print(tinggiBadan, 1); 
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(100);
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(100);
    displayState = 10;
    delay(2000);
  }
}

void btnRES_pressAction(void){
  if(btnRES.justPressed()){
    ESP.restart();
  }
}

void btnCAL_pressAction(void){
  if(btnCAL.justPressed()){
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(100,tft.height()/2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.print("Nilai Offset Saat ini :");
  tft.setTextSize(1);
  tft.setCursor(tft.width()/2+90,tft.height()/2);
  tft.print(simpangan);
  delay(3000);

  tft.fillRect(0, 0, 480, 320, TFT_BLUE);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  drawKeypadCAL();
  displayState = 12;
  }
}

void btnBackCAL_pressAction(void){
  if(btnBackCAL.justPressed()){
    tft.fillRect(0, 0, 480, 320, TFT_BLUE);
    tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
    tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
    drawKeypadCAL();
    displayState = 12;
  }
}

void btnSave_pressAction(void){
  if(btnSAVE.justPressed()){
    delay(3000);
    ESP.restart();
  }
}
//------------END of BUTTON PressAction----------

//-------------Init BUTTON------------
void initButtonBOY(){
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.print("CHOOSE GENDER !");
  
  uint16_t x1 = (tft.width() - BUTTON_W) / 2;
  uint16_t y1 = tft.height() / 2 - BUTTON_H - 10;
  btnBOY.initButtonUL(x1, y1+40, BUTTON_W, BUTTON_H1, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Boy", 1);
  btnBOY.setPressAction(btnBOY_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnBOY.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonGIRL(){
  uint16_t x1 = (tft.width() - BUTTON_W) / 2;
  uint16_t y1 = tft.height() / 2 - BUTTON_H + 60;
  btnGIRL.initButtonUL(x1, y1+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Girl", 1);
  btnGIRL.setPressAction(btnGIRL_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnGIRL.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonRCM(){
  tft.fillScreen(TFT_RED);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.print("CHOOSE METHOD !");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnRCM.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_BROWN, TFT_WHITE, TFT_BROWN, "Recumbent", 1);
  btnRCM.setPressAction(btnRCM_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnRCM.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSTD(){
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H + 60;
  btnSTD.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_BROWN, TFT_WHITE, "Standing", 1);
  btnSTD.setPressAction(btnSTD_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnSTD.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSTART(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();
  if(millis()>50){
    if (bacaTegangan() > 3.40){
      tft.fillScreen(TFT_GREEN);
      tft.setCursor(150,tft.height()/2-70);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(1);
      tft.print("Battery = ");
      tft.setCursor(240,tft.height()/2-70);
      tft.print(bacaTegangan());
      tft.setCursor(280,tft.height()/2-70);
      tft.print("V");
      Serial.println(bacaTegangan());
    }
    else if ( bacaTegangan() >= 3.00 && bacaTegangan() <= 3.40){
      tft.fillScreen(TFT_YELLOW);
      tft.setCursor(150,tft.height()/2-70);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(1);
      tft.print("Battery = ");
      tft.setCursor(240,tft.height()/2-70); //160
      tft.print(bacaTegangan());
      tft.setCursor(280,tft.height()/2-70); //220
      tft.print("V");
      Serial.println(bacaTegangan());
    }
    else if( bacaTegangan() < 3.00){
      tft.fillScreen(TFT_RED);
      tft.setCursor(110,tft.height()/2-70); //60
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(1);
      tft.print("Battery = ");
      tft.setCursor(210,tft.height()/2-70); //160
      tft.print("LOW BATTERY !!");
      Serial.println(bacaTegangan());    
    }
  }

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 10;
  btnSTART.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_BLUE, TFT_WHITE, TFT_BLUE, "Start", 1);
  btnSTART.setPressAction(btnSTART_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnSTART.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonCAL(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H + 60;
  btnCAL.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Calibrate", 1);
  btnCAL.setPressAction(btnCAL_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnCAL.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonCALCU(){
  uint16_t x = (tft.width() - BUTTON_W3) / 2;
  uint16_t y = tft.height() / 2;
  btnCALCULATE.initButtonUL(x, y-20, BUTTON_W3, BUTTON_H3, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Calculate", 1);
  btnCALCULATE.setPressAction(btnCALCULATE_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnCALCULATE.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonsRES(){
  uint16_t x = tft.width()/2+25;
  uint16_t y = 40;
  btnRES.initButtonUL(x, y, BUTTON_W3, BUTTON_H+30, TFT_RED, TFT_RED, TFT_WHITE, "RESTART", 1);
  btnRES.setPressAction(btnRES_pressAction);
  //btnSTART.setReleaseAction(btnSTART_releaseAction);
  btnRES.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSAVE(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  
  uint16_t x = (tft.width() - BUTTON_W2) / 2 - 80;
  uint16_t y = tft.height() / 2 - BUTTON_H2 + 80;
  btnBackCAL.initButtonUL(x, y, BUTTON_W2, BUTTON_H2, TFT_WHITE, TFT_RED, TFT_WHITE, "Back", 1);
  btnBackCAL.setPressAction(btnBackCAL_pressAction);
  btnBackCAL.drawSmoothButton(false, 3, TFT_BLACK);

  x = (tft.width() - BUTTON_W2) / 2 + 80;
  y = tft.height() / 2 - BUTTON_H2 + 80;
  btnSAVE.initButtonUL(x, y, BUTTON_W2, BUTTON_H2, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Save", 1);
  btnSAVE.setPressAction(btnSave_pressAction);
  //btnR.setReleaseAction(btnR_releaseAction);
  btnSAVE.drawSmoothButton(false, 3, TFT_BLACK);
}
//-------------END of Init BUTTON------------

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  EEPROM.get(69,simpangan);
  tft.begin();
  tft.setRotation(1);
  tft.setFreeFont(&FreeSansBold9pt7b); //FF18
  pinMode(buzzer, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  initButtonSTART();
  initButtonCAL();
}

void loop() {
  while((millis()-lastFrame )<20);
  lastFrame = millis();
  
  static uint32_t scanTime = millis();
  static uint32_t scanTime1 = millis();
  static uint32_t scanTime2 = millis();
  uint16_t t_x = 9999, t_y = 9999;    // To store the touch coordinates
  uint16_t t_x2 = 9999, t_y2 = 9999;
  uint16_t t_x4 = 9999, t_y4 = 9999;
  bool pressed = tft.getTouch(&t_x, &t_y);
  bool pressed2 = tft.getTouch(&t_x2, &t_y2);
  bool pressed4 =tft.getTouch(&t_x4, &t_y4);
  char *token ;
  char *token2;
  const char delimiter[] = "-";

//-------------Display STATE------------
  switch(displayState){
  case 1:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 0; b <= 1; b++) {
      if (pressed) {
        if (btn[b]->contains(t_x, t_y)) {
          btn[b]->press(true);
          btn[b]->pressAction();
        }
      }
      else {
        btn[b]->press(false);
        btn[b]->releaseAction();
      }
    }
  }
  break; 
  break;

  case 2:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 2; b <= 3; b++) {
      if (pressed) {
        if (btn[b]->contains(t_x, t_y)) {
          btn[b]->press(true);
          btn[b]->pressAction();
        }
      }
      else {
        btn[b]->press(false);
        btn[b]->releaseAction();
      }
    }
  }
  break; 
  break;

  case 3:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key[b].justReleased()) key[b].drawButton();     // draw normal

    if (key[b].justPressed()) {
      key[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the numberBuffer
      if (b >= 3 && b != 14) {
        if (indexWeight < NUM_LEN3) {
          bodyWeight[indexWeight] = keyLabel2[b][0];
          indexWeight++;
          bodyWeight[indexWeight] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        bodyWeight[indexWeight] = 0;
        if (indexWeight > 0) {
          indexWeight--;
          bodyWeight[indexWeight] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_DARKGREY);
        Serial.println(bodyWeight);
        
        tft.fillScreen(TFT_DARKGREY);
        tft.setCursor(tft.width()/2-100, tft.height()/2-20);
        tft.setTextSize(4);
        tft.setTextColor(TFT_WHITE);
        tft.print(bodyWeight);
        tft.setCursor(tft.width()/2+40, tft.height()/2-20);
        tft.print("kg");
        delay(3000);
        displayState = 4;
        drawKeypadDATE();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_DARKGREY);
        indexWeight = 0; // Reset index to 0
        bodyWeight[indexWeight] = 0; // Place null in buffer
      }

      if (b == 14) {
        displayState = 2; 
        initButtonBOY();
        initButtonGIRL();
      }

      if (b != 14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(bodyWeight, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 4: 
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed2 && key1[b].contains(t_x2, t_y2)) {
      key1[b].press(true);  // tell the button it is pressed
    } else {
      key1[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key1[b].justReleased()) key1[b].drawButton();     // draw normal

    if (key1[b].justPressed()) {
      key1[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the numberBuffer
      if (b >= 3 && b != 14) {
        if (numberIndex < NUM_LEN) {
          numberBuffer[numberIndex] = keyLabel[b][0];
          numberIndex++;
          numberBuffer[numberIndex] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        numberBuffer[numberIndex] = 0;
        if (numberIndex > 0) {
          numberIndex--;
          numberBuffer[numberIndex] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_BROWN);
        Serial.println(numberBuffer);
        tft.fillScreen(TFT_BROWN);
        tft.setCursor(tft.width()/2-120, tft.height()/2-20);
        tft.setTextSize(4);
        tft.setTextColor(TFT_WHITE);
        tft.print(numberBuffer);
        delay(3000);
        displayState = 5;
        drawkeypadBIRTH();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_BROWN);
        numberIndex = 0; // Reset index to 0
        numberBuffer[numberIndex] = 0; // Place null in buffer
      }

      if (b == 14) {
        displayState = 3; 
        drawKeypadWEIGHT();
      }

      if (b != 14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 5:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed4 && key2[b].contains(t_x4, t_y4)) {
      key2[b].press(true);  // tell the button it is pressed
    } else {
      key2[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key2[b].justReleased()) key2[b].drawButton();     // draw normal

    if (key2[b].justPressed()) {
      key2[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggalLahir
      if (b >= 3 && b != 14) {
        if (indexLahir < NUM_LEN) {
          tanggalLahir[indexLahir] = keyLabel[b][0];
          indexLahir++;
          tanggalLahir[indexLahir] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        tanggalLahir[indexLahir] = 0;
        if (indexLahir > 0) {
          indexLahir--;
          tanggalLahir[indexLahir] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_DARKGREEN);
        Serial.println(tanggalLahir);
        displayState = 7;
        // displayState = 3;
        delay(1000);
        tft.fillScreen(TFT_DARKGREEN);
        tft.setCursor(tft.width()/2-120, tft.height()/2-20);
        tft.setTextSize(4);
        tft.setTextColor(TFT_WHITE);
        tft.print(tanggalLahir);
        //initButtons4();
        delay(3000);
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_DARKGREEN);
        indexLahir = 0; // Reset index to 0
        tanggalLahir[indexLahir] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 4; 
        drawKeypadDATE();
      }
      if (b != 14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(tanggalLahir, DISP_X + 4, DISP_Y + 12);

      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 7: // Calculate umur
  char parsedStrings[5][20];
  token = strtok(numberBuffer,delimiter);
  strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));
  for (int i = 1 ; i< 3; i++){
    token = strtok(NULL,delimiter);
    strncpy(parsedStrings[i],token, sizeof(parsedStrings[i]));
  }

  currentDate = atoi(parsedStrings[0]);
  currentMonth = atoi(parsedStrings[1]);
  currentYear = atoi(parsedStrings[2]);

  char parsedStrings2[5][20];
  token2 = strtok(tanggalLahir,delimiter);
  strncpy(parsedStrings2[0], token2, sizeof(parsedStrings2[0]));
  for (int i = 1 ; i< 3; i++){
  token2 = strtok(NULL,delimiter);
    strncpy(parsedStrings2[i],token2, sizeof(parsedStrings2[i]));
  }

  birthDate = atoi(parsedStrings2[0]);
  birthMonth = atoi(parsedStrings2[1]);
  birthYear = atoi(parsedStrings2[2]);

  for( int year = birthYear; year < currentYear ; year++){
    if(isLeapYear(year)){
      days += 366;
    }else{
      days += 365;
    }
  }

  for(int month = 1 ; month < birthMonth ; month++){
    days -= getDaysInMonth(month, birthYear);
  }
  days -= birthDate;

  for(int month = 1 ; month < currentMonth ; month++){
    days += getDaysInMonth(month, currentYear);
  }

  days += currentDate;
  usiaHari = days;
  usiaBulan = usiaHari/30.4375;
  usiaBulanTemp = usiaBulan;
  
  tft.fillScreen(TFT_RED);
  tft.setCursor(tft.width()/2 -110, tft.height()/2-40 );
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.print("Umur: ");
  tft.setCursor(tft.width()/2 - 20, tft.height()/2-40 );
  tft.print(usiaBulanTemp);
  tft.setCursor(tft.width()/2, tft.height()/2-40 );
  tft.print(" Bulan");
  delay(1000);
  tft.setCursor(tft.width()/2-90,tft.height()/2);
  tft.print("Loading...");
  delay(1000);

  SRF.setCorrectionFactor(1.035);
  displayState = 8;
  initButtonRCM();
  initButtonSTD();
  break;

  case 8:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 4; b <= 5; b++) {
      if (pressed) {
        if (btn[b]->contains(t_x, t_y)) {
          btn[b]->press(true);
          btn[b]->pressAction();
        }
      }
      else {
        btn[b]->press(false);
        btn[b]->releaseAction();
      }
    }
  }
  break;
  break;

  case 9: //ukur tinggi
  newDistance = read_US(trigger, echo);
  ave = 0.0404*newDistance - 0.077;
  newAve = movingAverage(newDistance, 0);
  newAver = newAve + ave;
  newAverage = movingAverage1(newAver, 0);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print(newDistance); Serial.print(" ");
    Serial.println(newAverage);
    tft.fillRect((tft.width()-120)/2-12, tft.height()/2-75, 140, 40, TFT_BLACK);
    if (newAverage+simpangan >= 100){
      tft.setCursor((tft.width()-120)/2 + 10, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage + simpangan, 1);
    }else if (newAverage+simpangan < 10){
      tft.setCursor((tft.width()-120)/2 + 30, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage + simpangan, 1);
    }else{
      tft.setCursor((tft.width()-120)/2 + 20, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage + simpangan, 1);
    }
  }

  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis(); 
    if (pressed) {
      if (btnCALCULATE.contains(t_x, t_y)) {
        btnCALCULATE.press(true);
        btnCALCULATE.pressAction();
      }
      else {
      btnCALCULATE.press(false);
      btnCALCULATE.releaseAction();
      }
    }
  }
  break;

  //calculate Zscore
  case 10:
  usiaBulan = usiaBulanTemp;
  length = tinggiBadan;
  beratBadan = atof(bodyWeight);

  calculate_TBU();
  calculate_BBU();
  calculate_BBTB();
  calculate_IMTU();

  Serial.print(zscore_TBU);
  Serial.println(kategori_TBU);

  tft.fillScreen(TFT_BROWN);
  tft.setCursor(75, tft.height()/2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Calculating..");
  delay(2000);
  displayState = 11;
  
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.fillScreen(TFT_BROWN);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);

  tft.setCursor(35, 40);
  tft.print("Age :");
  // tft.setCursor(tft.width()/2-70, 160);
  // tft.print(":");
  tft.setCursor(tft.width()/2-157, 40);
  tft.print(usiaBulan);
  if (usiaBulan < 10){
    tft.setCursor(tft.width()/2-137, 40);
    tft.print("month");
  }else{
    tft.setCursor(tft.width()/2-127, 40);
    tft.print("month");
  }

  tft.setCursor(35, 70);
  tft.print("Gender :");
  tft.setCursor(tft.width()/2-130, 70);
  if(gender == BOY){
    tft.print("Boy");
  }
  else if (gender == GIRL){
    tft.print("Girl");
  }

  tft.setCursor(35, 100);
  tft.print("Method :");
  tft.setCursor(tft.width()/2-127, 100);
  if(method == RECUMBENT){
    tft.print("Recumbent");
  }
  else if (method == STANDING){
    tft.print("Standing");
  }

  tft.setCursor(35, 160); //160
  tft.print("Weight (kg) :");
  tft.setCursor(tft.width()/2-90, 160);
  tft.print(beratBadan, 1);

  tft.setCursor(35, 130); //130
  tft.print("Height (cm) :");
  tft.setCursor(tft.width()/2-90, 130);
  tft.print(tinggiBadan, 1);

  tft.setCursor(35, 220); //220
  tft.print("Z-Score WAZ");
  tft.setCursor(tft.width()/2-80, 220);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 220);
  tft.print(zscore_BBU, 1);
  tft.setCursor(tft.width()/2-13, 220);
  tft.print(kategori_BBU);

  tft.setCursor(35, 190); //190
  if(usiaBulan < 24){
    tft.print("Z-Score LAZ");
  }else if(usiaBulan >= 24){
    tft.print("Z-Score HAZ");
  }
  tft.setCursor(tft.width()/2-80, 190);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 190);
  tft.print(zscore_TBU, 1);
  tft.setCursor(tft.width()/2-13, 190);
  tft.print(kategori_TBU);

  tft.setCursor(35, 250);
  if(usiaBulan < 24){
    tft.print("Z-Score WLZ");
  }else if(usiaBulan >= 24){
    tft.print("Z-Score WHZ");
  }
  tft.setCursor(tft.width()/2-80, 250);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 250);
  tft.print(zscore_BBTB, 1);
  tft.setCursor(tft.width()/2-13, 250);
  tft.print(kategori_BBTB);

  tft.setCursor(35, 280);
  tft.print("Z-Score BMIZ");
  tft.setCursor(tft.width()/2-80, 280);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 280);
  tft.print(zscore_IMTU, 1);
  tft.setCursor(tft.width()/2-13, 280); //+25
  tft.print(kategori_IMTU);

  initButtonsRES();
  break;
    
  //Output
  case 11:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis(); 
    if (pressed) {
      if (btnRES.contains(t_x, t_y)) {
        btnRES.press(true);
        btnRES.pressAction();
      }
      else {
        btnRES.press(false);
        btnRES.releaseAction();
      }
    }  
  }
  break;

  case 12:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && key3[b].contains(t_x, t_y)) {
      key3[b].press(true);  // tell the button it is pressed
    } else {
      key3[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key3[b].justReleased()) key3[b].drawButton();

    if (key3[b].justPressed()) {
      key3[b].drawButton(true);
      if (b >= 3) {
        if (indexCalibrate < NUM_LEN2) {
          calibrate[indexCalibrate] = keyLabel3[b][0];
          indexCalibrate++;
          calibrate[indexCalibrate] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }
      if (b == 1) {
        calibrate[indexCalibrate] = 0;
        if (indexCalibrate > 0) {
          indexCalibrate--;
          calibrate[indexCalibrate] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_BLUE);
        Serial.println(calibrate);
        simpangan = atof(calibrate);
        EEPROM.put(69,simpangan);
        EEPROM.commit();        
        displayState = 13;
      }
      if (b == 0) {
        status("Value cleared", TFT_BLUE);
        indexCalibrate = 0; // Reset index to 0
        calibrate[indexCalibrate] = 0; // Place null in buffer
      }
    
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(calibrate, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      
      delay(10); // UI debouncing
    }
  }
  break;

  case 13:
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(tft.width()/2-90, 50);
  tft.setFreeFont(LABEL1_FONT);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.print("Offset : ");
  tft.setCursor(tft.width()/2+10, 50);
  tft.print(simpangan);
  
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setCursor(80,tft.height()/2-30);
  tft.print("Height (cm) : ");

  initButtonSAVE();
  displayState = 14;
  movingAverage_Clear();
  break;

  case 14:
  newDistance = read_US(trigger, echo);
  ave = 0.0404*newDistance - 0.077;
  newAve = movingAverage(newDistance, 0);
  newAver = newAve + ave;
  newAverage = movingAverage1(newAver, 0);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print(newDistance); Serial.print(" ");
    Serial.println(newAverage);
    tft.fillRect(tft.width()/2+20, tft.height()/2-60, 120, 40, TFT_BLUE);
    tft.setCursor(tft.width()/2+20, tft.height()/2-30);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    // akhirCAL = round(newAverage + simpangan, 1);
    tft.print(newAverage + simpangan, 1);
  }

  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed4 = tft.getTouch(&t_x4, &t_y4);
    scanTime = millis();
    for (uint8_t b = 8; b <= 9; b++) {
      if (pressed4) {
        if (btn[b]->contains(t_x4, t_y4)) {
          btn[b]->press(true);
          btn[b]->pressAction();
        }
      }
      else {
        btn[b]->press(false);
        btn[b]->releaseAction();
      }
    }
  }
  break;
  }
}
//-------------END of Display State------------
//-----------------END of LOOP-----------------

//-------------Calculate Variable--------------
float read_US(int trigger, int echo){
  long duration;
  float distance;
  int i;

  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH); //uS
  distance = duration * 0.034 / 2;
  return distance;
}

float movingAverage(float in_data, int debug){
  float average;
  float sum;

  data[i_data] = in_data;

  if (data[n_data - 1] == 0){
    sum = 0;

    for (int i = 0; i <= i_data; i++){
      sum += data[i];
    }
    average = sum / (i_data + 1);
  }
  else{
    sum = 0;
    for (int i = 0; i < n_data; i++){
      sum += data[i];
    }
    average = sum / n_data;
  }


  //#Debug
  if (debug){
    for (int i = 0; i < n_data; i++){
      Serial.print(i); Serial.print(" "); Serial.println(data[i]);
    }
    Serial.print("i"); Serial.print(" "); Serial.println(i_data);
    Serial.print("Last"); Serial.print(" "); Serial.println(data[n_data - 1]);

    Serial.print("Sum"); Serial.print(" "); Serial.println(sum);
    Serial.print("Ave"); Serial.print(" "); Serial.println(average);
    Serial.println();
  }

  i_data++;
  if (i_data >= n_data)
    i_data = 0;

  Serial.print("Average: "); Serial.println(average);
  return average;
}

float movingAverage1(float in_data, int debug){
  float average1;
  float sum;

  data[i_data] = in_data;

  if (data[n_data - 1] == 0){
    sum = 0;

    for (int i = 0; i <= i_data; i++){
      sum += data[i];
    }
    average1 = sum / (i_data + 1);
  }
  else{
    sum = 0;
    for (int i = 0; i < n_data; i++){
      sum += data[i];
    }
    average1 = sum / n_data;
  }


  //#Debug
  if (debug){
    for (int i = 0; i < n_data; i++){
      Serial.print(i); Serial.print(" "); Serial.println(data[i]);
    }
    Serial.print("i"); Serial.print(" "); Serial.println(i_data);
    Serial.print("Last"); Serial.print(" "); Serial.println(data[n_data - 1]);

    Serial.print("Sum"); Serial.print(" "); Serial.println(sum);
    Serial.print("Ave"); Serial.print(" "); Serial.println(average1);
    Serial.println();
  }

  i_data++;
  if (i_data >= n_data)
    i_data = 0;

  Serial.print("Average: "); Serial.println(average1);
  return average1;
}

void movingAverage_Clear(){
  //Make sure All Array is 0
  for (int i = 0; i < n_data; i++) {
    data[i] = 0;
  }
}

float bacaTegangan(){
  inputValue = analogRead(VSensor);
  adcValue = (inputValue * ref_voltage)/1024.0;
  Serial.print("Input Voltage = ");
  Serial.println(voltage,2);

  return adcValue;
}

bool isLeapYear(int year){
  if(year % 4 != 0){
    return false;
  } else if (year % 100 != 0 ){
    return true;
  } else if (year % 400 != 0) {
    return false;
  } else {
    return true;
  }
}

int getDaysInMonth (int month, int year){
  if (month == 2){
    return isLeapYear(year) ? 29 :28;
  } else if (month ==  4 || month == 6 || month == 9 || month == 11){
    return 30;
  } else{
    return 31;
  }
}
//-----------END of Calculate Variable---------

//-----------------Draw Keypad-----------------
void drawKeypadCAL(){
  tft.setCursor(55, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Offset Value");
  tft.setCursor(280, 17);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(2);
  tft.print("(ex: 0.75)");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key3[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor3[b], TFT_WHITE,
                        keyLabel3[b], KEY_TEXTSIZE);
      key3[b].drawButton();
    }
  }
}

void drawkeypadBIRTH(){
  tft.fillRect(0, 0, 480, 320, TFT_DARKGREEN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(47, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Birth Date");
  tft.setCursor(242, 17);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(2);
  tft.print("(ex: 07-04-2024)");
  
  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key2[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key2[b].drawButton();
    }
  }
}

void drawKeypadWEIGHT(){
  tft.fillRect(0, 0, 480, 320, TFT_DARKGREY);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(28, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Body Weight in kg");
  tft.setCursor(318, 17);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(2);
  tft.print("(ex: 10.87)");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      key[b].drawButton();
    }
  }
}

void drawKeypadDATE(){
  tft.fillRect(0, 0, 480, 320, TFT_BROWN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  
  tft.setCursor(25, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Today's Date");
  tft.setCursor(255, 17);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(2);
  tft.print("(ex: 07-04-2024)");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key1[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key1[b].drawButton();
    }
  }
}
//---------------END of Draw Keypad------------

void touch_calibrate(){
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!LittleFS.begin()) {
    Serial.println("Formating file system");
    LittleFS.format();
    LittleFS.begin();
  }

  // check if calibration file exists and size is correct
  if (LittleFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      LittleFS.remove(CALIBRATION_FILE);
    } else {
      File f = LittleFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = LittleFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void status(const char *msg, uint16_t color){
  tft.setTextPadding(240);
  if(color != 0){
    tft.setTextColor(TFT_WHITE, color);
  }else{
    tft.setTextColor(color, color);
  }
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
}

void drawBitmap(const uint16_t *bitmap){
  int h=320, w=480, row, col, buffidx=0;
  for (row=0; row<h; row++){
    for (col=0; col<w; col++){
      tft.drawPixel(col, row, pgm_read_word(bitmap + buffidx));
      buffidx++;
    }
  }
}