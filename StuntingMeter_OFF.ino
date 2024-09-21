//BENERIN Ultrasonik
#include <FS.h>
#include "Free_Fonts.h"
#include "SD.h"
#include "SPI.h"
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "SRF05.h"
#include <BluetoothSerial.h>
#include <Adafruit_Thermal.h>
#include <Wire.h>
#include <RTClib.h>

//trig 32 echo 33
#define SS_PIN 32 // Change this to your desired SS pin
#define SCK_PIN 25 // Change this to your desired SCK pin
#define MOSI_PIN 27 // Change this to your desired MOSI pin
#define MISO_PIN 26 // Change this to your desired MISO pin

SPIClass spiSD(HSPI);
TFT_eSPI tft = TFT_eSPI();
// TFT_eWidget widget = TFT_eWidget(tft);

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

// Keyboard start position, key sizes and spacing
#define KEY_X1 67 // Centre of key
#define KEY_Y1 118 
#define KEY_W1 65 // Width and height
#define KEY_H1 35
#define KEY_SPACING_X1 5 // X and Y
#define KEY_SPACING_Y1 5
#define KEY_TEXTSIZE1 1

// Keyboard start position, key sizes and spacing
#define KEY_X2 39 // Centre of key
#define KEY_Y2 118 
#define KEY_W2 48 // Width and height
#define KEY_H2 38
#define KEY_SPACING_X2 2 // X and Y gap
#define KEY_SPACING_Y2 2
// #define KEY_TEXTSIZE1 1

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
ButtonWidget btnRESC = ButtonWidget(&tft);
ButtonWidget btnRES2 = ButtonWidget(&tft);
ButtonWidget btnRESC2 = ButtonWidget(&tft);
ButtonWidget btnBackCAL = ButtonWidget(&tft);
ButtonWidget btnSAVE = ButtonWidget(&tft);
ButtonWidget btnPRINT = ButtonWidget(&tft);
ButtonWidget btnPRINT2 = ButtonWidget(&tft);
ButtonWidget btnDELETE = ButtonWidget(&tft);
ButtonWidget btnDELETE2 = ButtonWidget(&tft);
ButtonWidget btnFILL = ButtonWidget(&tft);
ButtonWidget btnDUMMY3 = ButtonWidget(&tft);
ButtonWidget btnASI0Y = ButtonWidget(&tft);
ButtonWidget btnASI0T = ButtonWidget(&tft);
ButtonWidget btnASI1Y = ButtonWidget(&tft);
ButtonWidget btnASI1T = ButtonWidget(&tft);
ButtonWidget btnASI2Y = ButtonWidget(&tft);
ButtonWidget btnASI2T = ButtonWidget(&tft);
ButtonWidget btnASI3Y = ButtonWidget(&tft);
ButtonWidget btnASI3T = ButtonWidget(&tft);
ButtonWidget btnASI4Y = ButtonWidget(&tft);
ButtonWidget btnASI4T = ButtonWidget(&tft);
ButtonWidget btnASI5Y = ButtonWidget(&tft);
ButtonWidget btnASI5T = ButtonWidget(&tft);
ButtonWidget btnASI6Y = ButtonWidget(&tft);
ButtonWidget btnASI6T = ButtonWidget(&tft);
ButtonWidget btnSumberPUSAT = ButtonWidget(&tft);
ButtonWidget btnSumberDAERAH = ButtonWidget(&tft);
ButtonWidget btnPUSATY = ButtonWidget(&tft);
ButtonWidget btnPUSATT = ButtonWidget(&tft);
ButtonWidget btnDAERAHY = ButtonWidget(&tft);
ButtonWidget btnDAERAHT = ButtonWidget(&tft);
ButtonWidget btnVITAY = ButtonWidget(&tft);
ButtonWidget btnVITAT = ButtonWidget(&tft);
ButtonWidget btnSETTINGS = ButtonWidget(&tft);
ButtonWidget btnSetWIFI = ButtonWidget(&tft);
ButtonWidget btnSetPLACE = ButtonWidget(&tft);
ButtonWidget btnUBAH = ButtonWidget(&tft);
ButtonWidget btnBackUBAH = ButtonWidget(&tft);
ButtonWidget btnUbahWIFI = ButtonWidget(&tft);
ButtonWidget btnBackUbahWIFI = ButtonWidget(&tft);

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
#define NUM_LEN_2 2
#define NUM_LEN_4 4
#define NUM_LEN_5 5
#define NUM_LEN_10 10
#define NUM_LEN_13 13
#define NUM_LEN_19 19
#define NUM_LEN_16 16
char bodyWeight[NUM_LEN_5 + 1]="", nik[NUM_LEN_16 + 1]="", todaysDate[NUM_LEN_10 + 1]="", todaysDatee[NUM_LEN_10 + 1], tanggalLahir[NUM_LEN_10 + 1]="", 
     tanggalLahirr[NUM_LEN_10 + 1], calibrate[NUM_LEN_5 + 1]="", name[NUM_LEN_19 + 1]="", ortu[NUM_LEN_19 + 1]="", nikORTU[NUM_LEN_16 + 1]="", phone[NUM_LEN_13 + 1]="",
     pkm[NUM_LEN_19 + 1]="", kelurahan[NUM_LEN_19 + 1]="", posyandu[NUM_LEN_19 + 1]="", address[NUM_LEN_19 + 1]="", lila[NUM_LEN_4 + 1]="", vita[NUM_LEN_5 + 1]="",
     like[NUM_LEN_5 + 1]="", rt[NUM_LEN_2 + 1]="", rw[NUM_LEN_2 + 1]="", tahunProduksi[NUM_LEN_4 + 1]="", beriKE[NUM_LEN_4 + 1]="", sumberPMT[NUM_LEN_19 + 1]="", 
     beriPUSAT[NUM_LEN_19 + 1]="", beriDAERAH[NUM_LEN_19 + 1]="", wifi[NUM_LEN_19 + 1]="", password[NUM_LEN_19 + 1]="";

// We have a status line for messages
#define STATUS_X 240 // Centred on this
#define STATUS_Y 90 //95

#define n_data 50 //4
float jarak, data[n_data], newDistance, ave, newAve, newAver, newAverage, filteredDistance;
int i_data = 0, waktu_sebelum;  // 10 detik
char data_sd_char[150];
int displayState = 1, VSensor = 13, usia = 40, days = 0, usiaHari, usiaBulan, usiaBulanTemp, tahun, sisaBulan, currentDate, currentMonth, currentYear,
    tempTime = 0, birthDate, birthMonth, birthYear, inputValue = 0, ref_voltage = 5.0, noKolom;
const int buzzer = 12, trigger = 14, echo = 33;
// echo 33, trigger 14

long waktu;
unsigned long previousMillis = 0, currentMillis;
const unsigned long interval = 2000;

uint8_t indexWeight=0, indexLahir=0, indexDate=0, indexCalibrate=0, indexName=0, indexNik=0, indexOrtu=0, indexNikORTU=0, indexPhone=0, indexPKM=0, indexKELURAHAN=0, indexPOSYANDU=0, indexADDRESS=0, indexLILA=0, indexVITA=0,
        indexLIKE=0, indexRT=0, indexRW=0, indexBeriKE=0, indexBeriPUSAT=0, indexBeriDAERAH=0, indexTAHUN=0, indexSumberPMT=0,  indexWIFI=0, indexPASSWORD=0;

boolean method;
bool connected, ulangState, found;
float tinggiBadan, simpangan = 0, akhirCAL;

float adcValue = 0.0, voltage = 0.0, R1 = 30000.0, R2 = 7500.0, length, beratBadan, sd_TBU, m_TBU, zscore_TBU, l_BBU, m_BBU;
float s_BBU, zscore_BBU, l_BBTB, m_BBTB, s_BBTB, zscore_BBTB, imt, l_IMTU, m_IMTU, s_IMTU, zscore_IMTU;
String apa, gender, genderr, vitaa, nikString, kategori_TBU, kategori_BBU, kategori_BBTB, kategori_IMTU, methodd, pesanPrinter, pesanSD, pesanCloud, usiaTahun, ASI0, ASI1, ASI2, ASI3, ASI4, ASI5, ASI6, sumberPMTstr, beriPUSATstr, beriDAERAHstr;
String noKolomStr, nikStr, nameStr, tanggalLahirStr, usiaTahunStr, usiaBulanStr, ortuStr, nikOrtuStr, phoneStr, pkmStr, kelurahanStr, posyanduStr, rtStr, rwStr,
       addressStr, todaysDateStr, lengthStr, beratBadanStr, lilaStr, vitaStr, likeStr, beriKeStr, tahunProduksiStr;
DateTime now;
File myFile;

#include "TBU.h"
#include "BBU.h"
#include "BBTB.h"
#include "IMTU.h"
#include "function.h"

// Create 15 keys for the keypad
char keyLabel[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "0", "Back" },
keyLabel1[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", "/", "0", "Back" },
keyLabel2[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "Back" },
keyLabel3[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "-" },
keyboardLabel[30][5] PROGMEM = {"Q", "W", "E", "R", "T", "Y",
                          "U", "I", "O", "P",  "A", "S",
                          "D", "F", "G", "H", "J", "K",
                          "L", "Z", "X", "C", "V", "Del",
                          "B", "N", "M", " ", "New", "Send"},
keyboardFullLabel[45][5] PROGMEM = {"1", "2", "3", "4", "5", "6", "7", "8", "9", 
                                    "O", "Q", "W", "E", "R", "T", "Y", "U", "New",
                                    "I", "O", "P", "A", "S", "D", "F", "G", "Del", 
                                    "H", "J", "K", "L", "Z", "X", "C", "V", "Back", 
                                    "B", "N", "M", ".", "/", "-", ":",  " ", "Send"};
uint16_t keyColor[15] = {TFT_RED, TFT_BROWN, TFT_DARKGREEN,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_DARKGREY
                        },                        
keyColor3[15] = {TFT_RED, TFT_BROWN, TFT_DARKGREEN,
                TFT_BLUE, TFT_BLUE, TFT_BLUE,
                TFT_BLUE, TFT_BLUE, TFT_BLUE,
                TFT_BLUE, TFT_BLUE, TFT_BLUE,
                TFT_BLUE, TFT_BLUE, TFT_BLUE
                },
keyboardColor[30] PROGMEM = {TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                             TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                             TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                             TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BROWN,
                             TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_GREY, TFT_RED, TFT_DARKGREEN
                            },
keyboardFullColor[45] PROGMEM = {TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN,
                                 TFT_DARKGREEN, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_RED,
                                 TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BROWN,
                                 TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLACK,
                                 TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_GREY, TFT_GREY, TFT_GREY, TFT_GREY, TFT_GREY, TFT_DARKGREEN
                                };

unsigned long lastFrame = millis();

uint8_t addressBT[6]  = {0x86, 0x67, 0x7A, 0xF9, 0x5F, 0xA7};

RTC_DS3231 rtc;
WiFiClientSecure client;
SRF05 SRF(trigger, echo);
BluetoothSerial SerialBT;
Adafruit_Thermal printer(&SerialBT);
String slaveName = "RPP02N", myName = "ESP32-BT-Master", pkmm, kelurahann, posyanduu, addresss, rtt, rww, wifii, passwordd;
String GOOGLE_SCRIPT_ID = "AKfycbwNIzSs6aL7UcZFO7v_jTFWwBoDf3ZVJ3r6BddEwoJcZZaZoMpEm54YofQ0h_37sBKvSQ";

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button keyWEIGHT[15],keyBIRTH[15], keyDATE[15],keyCalibrate[15],keyNAME[30],keyNIK[15],keyORTU[30],keyNikORTU[15],keyPhone[15],keyPKM[30],keyKELURAHAN[30],keyPOSYANDU[30],keyADDRESS[45],keyLILA[15],keyVITA[15],
                keyLIKE[15],keyRT[15],keyRW[15],keyBeriKE[15],keySumberPMT[30],keyBeriPUSAT[30],keyTAHUN[15],keyBeriDAERAH[30],keyWIFI[45],keyPASSWORD[45];

// Create an array of button instances to use in for() loops
ButtonWidget* btn[] = {&btnSTART,&btnCAL,&btnSETTINGS,&btnBOY,&btnGIRL,&btnRCM,&btnSTD,&btnCALCULATE,&btnFILL,&btnRES,&btnRESC,&btnPRINT,&btnBackCAL,&btnSAVE,&btnDUMMY3,
                       &btnASI0Y,&btnASI0T,&btnASI1Y,&btnASI1T,&btnASI2Y,&btnASI2T,&btnASI3Y,&btnASI3T,&btnASI4Y,&btnASI4T,&btnASI5Y,&btnASI5T,&btnASI6Y,&btnASI6T,
                       &btnRES2,&btnRESC2,&btnDELETE2,&btnPRINT2,&btnSumberPUSAT,&btnSumberDAERAH,&btnPUSATY,&btnPUSATT,&btnDAERAHY,&btnDAERAHT,&btnVITAY,&btnVITAT,
                       &btnBackUBAH,&btnUBAH,&btnSetPLACE,&btnSetWIFI,&btnBackUbahWIFI,&btnUbahWIFI};
uint8_t buttonCount = sizeof(btn) / sizeof(btn[0]);

//-------------BUTTON PressAction----------
void btnSTART_pressAction(void){
  if(btnSTART.justPressed()){
    drawKeypadNIK();
    displayState = 15;
  }
}

void btnSETTINGS_pressAction(void){
  if(btnSETTINGS.justPressed()){
    displayState = 42;
    initButtonSetPLACE();
    initButtonSetWIFI();
  }
}

void btnSetWIFI_pressAction(void){
  if(btnSetWIFI.justPressed()){
    EEPROM.get(250,wifii);
    EEPROM.get(270,passwordd);

    displayState = 43;
    initButtonUbahWIFI();
  }
}

void btnSetPLACE_pressAction(void){
  if(btnSetPLACE.justPressed()){
    //POSYANDU DLL
    //TAMPILAN Current Place

    EEPROM.get(1,pkm);
    EEPROM.get(22,kelurahan);
    EEPROM.get(53,posyandu);
    EEPROM.get(94,address);
    EEPROM.get(115,rt);
    EEPROM.get(136,rw);

    tft.fillScreen(TFT_CYAN);
    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(100, 40);
    tft.print("CURRENT PLACE INFORMATION");
    tft.setCursor(35, 80);
    tft.print("PKM");
    tft.setCursor(tft.width()/2-100, 80);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 80);
    tft.print(pkm);
    tft.setCursor(35, 110);
    tft.print("Kelurahan");
    tft.setCursor(tft.width()/2-100, 110);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 110);
    tft.print(kelurahan);
    tft.setCursor(35, 140);
    tft.print("Posyandu");
    tft.setCursor(tft.width()/2-100, 140);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 140);
    tft.print(posyandu);
    tft.setCursor(35, 170);
    tft.print("Alamat");
    tft.setCursor(tft.width()/2-100, 170);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 170);
    tft.print(address);
    tft.setCursor(35, 200);
    tft.print("RT");
    tft.setCursor(tft.width()/2-100, 200);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 200);
    tft.print(rt);
    tft.setCursor(35, 230);
    tft.print("RW");
    tft.setCursor(tft.width()/2-100, 230);
    tft.print(":");
    tft.setCursor(tft.width()/2-90, 230);
    tft.print(rw);
    // delay(3000);

    initButtonUBAH();
    displayState = 41;
  }
}
void btnBOY_pressAction(void){
  if(btnBOY.justPressed()){
    gender = "L";
    displayState = 5;
    drawKeypadBIRTH();
  }
}

void btnGIRL_pressAction(void){
  if(btnGIRL.justPressed()){
    gender = "P";
    displayState = 5;
    drawKeypadBIRTH();
  }
}

void btnRCM_pressAction(void) {
  if (btnRCM.justPressed()) {
    Serial.println("RECUMBENT");
    method = RECUMBENT; //recumbent
    tft.fillScreen(TFT_CYAN);
    tft.setCursor(30,40);
    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK);
    tft.print("Make sure the ground is clear !");
    tft.setCursor(tft.width()/2-135,tft.height()/2+80);
    tft.setTextSize(1);
    tft.print("CLICK TO START CALCULATE");
    initButtonCALCU();
    displayState = 9;
    movingAverage_Clear();
  }
}

void btnSTD_pressAction(void) {
  if (btnSTD.justPressed()) {
    Serial.println("STANDING");
    method = STANDING; //standing
    tft.fillScreen(TFT_CYAN);
    tft.setCursor(30,40);
    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK);
    tft.print("Make sure the ground is clear !");
    tft.setCursor(tft.width()/2-135,tft.height()/2+80);
    tft.setTextSize(1);
    tft.print("CLICK TO START CALCULATE");
    initButtonCALCU();
    displayState = 9;
    movingAverage_Clear();
  }
}

void btnSumberPUSAT_pressAction(void){
  if(btnSumberPUSAT.justPressed()){
    sumberPMTstr = "Pusat";
    displayState = 37;
    initButtonBeriPUSAT();
  }
}

void btnSumberDAERAH_pressAction(void){
  if(btnSumberDAERAH.justPressed()){
    sumberPMTstr = "Daerah";
    displayState = 37;
    initButtonBeriPUSAT();
  }
}

void btnPUSATY_pressAction(void){
  if(btnPUSATY.justPressed()){
    beriPUSATstr = "Ya";
    displayState = 38;
    drawKeypadTAHUN();
  }
}

void btnPUSATT_pressAction(void){
  if(btnPUSATT.justPressed()){
    beriPUSATstr = "Tidak";
    displayState = 38;
    drawKeypadTAHUN();
  }
}

void btnDAERAHY_pressAction(void){
  if(btnDAERAHY.justPressed()){
    beriDAERAHstr = "Ya";
    displayState = 40;
    initButtonPRINT();
  }
}

void btnDAERAHT_pressAction(void){
  if(btnDAERAHT.justPressed()){
    beriDAERAHstr = "Tidak";
    displayState = 40;
    initButtonPRINT();
  }
}

void btnASI0T_pressAction(void){
  if(btnASI0T.justPressed()){
    ASI0="Tidak";
    displayState = 29;
    initButtonASI1();
  }
}

void btnASI0Y_pressAction(void){
  if(btnASI0Y.justPressed()){
    ASI0="Ya";
    displayState = 29;
    initButtonASI1();
  }
}

void btnASI1T_pressAction(void){
  if(btnASI1T.justPressed()){
    ASI1="Tidak";
    displayState = 30;
    initButtonASI2();
  }
}

void btnASI1Y_pressAction(void){
  if(btnASI1Y.justPressed()){
    ASI1="Ya";
    displayState = 30;
    initButtonASI2();
  }
}

void btnASI2T_pressAction(void){
  if(btnASI2T.justPressed()){
    ASI2="Tidak";
    displayState = 31;
    initButtonASI3();
  }
}

void btnASI2Y_pressAction(void){
  if(btnASI2Y.justPressed()){
    ASI2="Ya";
    displayState = 31;
    initButtonASI3();
  }
}

void btnASI3T_pressAction(void){
  if(btnASI3T.justPressed()){
    ASI3="Tidak";
    displayState = 32;
    initButtonASI4();
  }
}

void btnASI3Y_pressAction(void){
  if(btnASI3Y.justPressed()){
    ASI3="Ya";
    displayState = 32;
    initButtonASI4();
  }
}

void btnASI4T_pressAction(void){
  if(btnASI4T.justPressed()){
    ASI4="Tidak";
    displayState = 33;
    initButtonASI5();
  }
}

void btnASI4Y_pressAction(void){
  if(btnASI4Y.justPressed()){
    ASI4="Ya";
    displayState = 33;
    initButtonASI5();
  }
}

void btnASI5T_pressAction(void){
  if(btnASI5T.justPressed()){
    ASI5="Tidak";
    displayState = 34;
    initButtonASI6();
  }
}

void btnASI5Y_pressAction(void){
  if(btnASI5Y.justPressed()){
    ASI5="Ya";
    displayState = 34;
    initButtonASI6();
  }
}

void btnASI6T_pressAction(void){
  if(btnASI6T.justPressed()){
    ASI6="Tidak";
    displayState = 35;
    drawKeypadBeriKE();
  }
}

void btnASI6Y_pressAction(void){
  if(btnASI6Y.justPressed()){
    ASI6="Ya";
    displayState = 35;
    drawKeypadBeriKE();
  }
}

void btnVITAY_pressAction(void){
  if(btnVITAY.justPressed()){
    vitaa="Ya";
    displayState = 27;
    drawKeypadLIKE();
  }
}

void btnVITAT_pressAction(void){
  if(btnVITAT.justPressed()){
    vitaa="Tidak";
    displayState = 27;
    drawKeypadLIKE();
  }
}

void btnCALCULATE_pressAction(void){
  if(btnCALCULATE.justPressed()){
    tft.fillScreen(TFT_CYAN);
    tft.setTextColor(TFT_BLACK);
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
    tft.print(tinggiBadan, 1); 
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(100);
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(100);
    delay(1000);

    displayState = 10;
  }
}

void btnRES2_pressAction(void){
  if(btnRES2.justPressed()){
    
    if(method == RECUMBENT){
      methodd = "Recumbent";
    }else if(method == STANDING){
      methodd = "Standing";
    }

    EEPROM.get(1,pkm);
    EEPROM.get(22,kelurahan);
    EEPROM.get(53,posyandu);
    EEPROM.get(94,address);
    EEPROM.get(115,rt);
    EEPROM.get(136,rw);
    EEPROM.get(77,noKolom);
    if(noKolom <= 0){
      noKolom = 0;
    }
    noKolom += 1;
    pesanSD = String(noKolom) + "," + String(nik) + "," + String(name) + "," + String(tanggalLahir) + "," + String(usiaTahun) + ","
        + String(usiaBulan) + "," + gender + "," + String(ortu) + "," + String(nikORTU) + "," + String(phone) + "," + String(pkm) + "," 
        + String(kelurahan) + "," + String(posyandu) + "," + String(rt) + "," + String(rw) + "," + String(address) + ","  
        + String(todaysDate) + "," + String(length,1) + "," + methodd + "," + String(beratBadan,1) + "," + String(lila) + ","
        + String(vitaa) + "," + String(like) + ","
        + ASI0 + "," + ASI1 + "," + ASI2 + "," + ASI3 + "," + ASI4 + "," + ASI5 + "," + ASI6 + ","
        + String(beriKE) + "," + String(sumberPMTstr) + "," + String(beriPUSATstr) + "," + String(tahunProduksi) + "," 
        + String(beriDAERAHstr) + "\n";
      
    pesanSD.toCharArray(data_sd_char, pesanSD.length()+1);
    appendFile(SD, "/data.csv", data_sd_char);
    
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillScreen(TFT_CYAN);
    tft.setCursor(tft.width()/2-70, tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.print("SAVED");
    EEPROM.put(77,noKolom);
    EEPROM.commit();
    delay(2000);
    displayState = 40;
    initButtonPRINT();
  }
}

void btnRESC2_pressAction(void){
  if(btnRESC2.justPressed()){
    Serial.print("Pesan Printer: "); Serial.println(pesanPrinter);
    Serial.print("Pesan Cloud: "); Serial.println(pesanCloud);

    pesanSD.toCharArray(data_sd_char, pesanSD.length()+1);
    appendFile(SD, "/data.csv", data_sd_char);
    // tft.fillScreen(TFT_CYAN);
    // tft.setFreeFont(&FreeSansBold9pt7b);
    // tft.setCursor(tft.width()/2-70, tft.height()/2-20);
    // tft.setTextSize(2);
    // tft.setTextColor(TFT_BLACK);
    // tft.print("SAVED");
    // tft.setCursor(tft.width()/2-170, tft.height()/2+20);
    // tft.print("SD Card & Cloud");
    Serial.print("Pesan Printer: "); Serial.println(pesanPrinter);
    Serial.print("Pesan Cloud: "); Serial.println(pesanCloud);
    delay(1000);
    displayState = 47;
  }
}

void btnRESC_pressAction(void){
  if(btnRESC.justPressed()){
    Serial.print("Pesan Printer: "); Serial.println(pesanPrinter);
    Serial.print("Pesan Cloud: "); Serial.println(pesanCloud);
    if(method == RECUMBENT){
      methodd = "Recumbent";
    }else if(method == STANDING){
      methodd = "Standing";
    }

    EEPROM.get(1,pkm);
    EEPROM.get(22,kelurahan);
    EEPROM.get(53,posyandu);
    EEPROM.get(94,address);
    EEPROM.get(115,rt);
    EEPROM.get(136,rw);
    EEPROM.get(77,noKolom);
    EEPROM.get(250,wifii);
    EEPROM.get(270,passwordd);

    if(noKolom <= 0){
      noKolom = 0;
    }
    noKolom += 1;
    Serial.print("Pesan SD: "); Serial.println(pesanSD);
    pesanSD = String(noKolom) + "," + String(nik) + "," + String(name) + "," + String(tanggalLahir) + "," + String(usiaTahun) + ","
        + String(usiaBulan) + "," + gender + "," + String(ortu) + "," + String(nikORTU) + "," + String(phone) + "," + String(pkm) + "," 
        + String(kelurahan) + "," + String(posyandu) + "," + String(rt) + "," + String(rw) + "," + String(address) + ","  
        + String(todaysDate) + "," + String(length,1) + "," + methodd + "," + String(beratBadan,1) + "," + String(lila) + ","
        + String(vitaa) + "," + String(like) + ","
        + ASI0 + "," + ASI1 + "," + ASI2 + "," + ASI3 + "," + ASI4 + "," + ASI5 + "," + ASI6 + ","
        + String(beriKE) + "," + String(sumberPMTstr) + "," + String(beriPUSATstr) + "," + String(tahunProduksi) + "," 
        + String(beriDAERAHstr) + "\n";

    noKolomStr = String(noKolom); nikStr = String(nik); nameStr = String(name); tanggalLahirStr = String(tanggalLahir); usiaTahunStr = String(usiaTahun); usiaBulanStr = String(usiaBulan);
    ortuStr = String(ortu); nikOrtuStr = String(nikORTU); phoneStr = String(phone); pkmStr = String(pkm); kelurahanStr = String(kelurahan); posyanduStr = String(posyandu); rtStr = String(rt); rwStr = String(rw);
    addressStr = String(address); todaysDateStr = String(todaysDate); lengthStr = String(length, 1); beratBadanStr = String(beratBadan, 1); lilaStr = String(lila); vitaStr = String(vitaa); 
    likeStr = String(like); beriKeStr = String(beriKE); tahunProduksiStr = String(tahunProduksi);
        
    pesanCloud = "NIK=" + urlencode(nikStr) + "&nama_anak=" + urlencode(nameStr) + "&tgl_lahir=" + urlencode(tanggalLahirStr) + "&umur_tahun=" + urlencode(usiaTahunStr) + "&umur_bulan="
        + urlencode(usiaBulanStr) + "&jenis_kelamin=" + urlencode(gender) + "&nama_ortu=" + urlencode(ortuStr) + "&nik_ortu=" + urlencode(nikOrtuStr) + "&hp_ortu=" + urlencode(phoneStr) + "&PKM=" + urlencode(pkmStr) + "&KEL=" 
        + urlencode(kelurahanStr) + "&POSY=" + urlencode(posyanduStr) + "&RT=" + urlencode(rtStr) + "&RW=" + urlencode(rwStr) + "&ALAMAT=" + urlencode(addressStr) + "&TANGGALUKUR="  
        + urlencode(todaysDateStr) + "&TINGGI=" + urlencode(lengthStr) + "&CARAUKUR=" + urlencode(methodd) + "&BERAT=" + urlencode(beratBadanStr) 
        + "&LILA=" + urlencode(lilaStr) + "&vita=" + urlencode(vitaStr) + "&lingkar_kepala=" + urlencode(likeStr) + "&asi_bulan_0="
        + urlencode(ASI0) + "&asi_bulan_1=" + urlencode(ASI1) + "&asi_bulan_2=" + urlencode(ASI2) + "&asi_bulan_3=" + urlencode(ASI3) + "&asi_bulan_4=" + urlencode(ASI4) + "&asi_bulan_5=" + urlencode(ASI5) + "&asi_bulan_6=" + urlencode(ASI6) + "&pemberian_ke="
        + urlencode(beriKeStr) + "&sumber_pmt=" + urlencode(sumberPMTstr) + "&pemberian_pusat=" + urlencode(beriPUSATstr) + "&tahun_produksi=" + urlencode(tahunProduksiStr) + "&pemberian_daerah=" 
        + urlencode(beriDAERAHstr);

    pesanSD.toCharArray(data_sd_char, pesanSD.length()+1);
    appendFile(SD, "/data.csv", data_sd_char);
    // tft.fillScreen(TFT_CYAN);
    // tft.setFreeFont(&FreeSansBold9pt7b);
    // tft.setCursor(tft.width()/2-70, tft.height()/2-20);
    // tft.setTextSize(2);
    // tft.setTextColor(TFT_BLACK);
    // tft.print("SAVED");
    // tft.setCursor(tft.width()/2-170, tft.height()/2+20);
    // tft.print("SD Card & Cloud");
    
    Serial.print("Pesan Printer: "); Serial.println(pesanPrinter);
    Serial.print("Pesan Cloud: "); Serial.println(pesanCloud);
    delay(1000);
    
    displayState = 46;
  }
}

void btnRES_pressAction(void){
  if(btnRES.justPressed()){

    if(method == RECUMBENT){
      methodd = "Recumbent";
    }else if(method == STANDING){
      methodd = "Standing";
    }

    EEPROM.get(1,pkm);
    EEPROM.get(22,kelurahan);
    EEPROM.get(53,posyandu);
    EEPROM.get(94,address);
    EEPROM.get(115,rt);
    EEPROM.get(136,rw);
    EEPROM.get(77,noKolom);
    if(noKolom <= 0){
      noKolom = 0;
    }
    noKolom += 1;
    pesanSD = String(noKolom) + "," + String(nik) + "," + String(name) + "," + String(tanggalLahir) + "," + String(usiaTahun) + ","
        + String(usiaBulan) + "," + gender + "," + String(ortu) + "," + String(nikORTU) + "," + String(phone) + "," + String(pkm) + "," 
        + String(kelurahan) + "," + String(posyandu) + "," + String(rt) + "," + String(rw) + "," + String(address) + ","  
        + String(todaysDate) + "," + String(length,1) + "," + methodd + "," + String(beratBadan,1) + "," + String(lila) + ","
        + String(vitaa) + "," + String(like) + ","
        + ASI0 + "," + ASI1 + "," + ASI2 + "," + ASI3 + "," + ASI4 + "," + ASI5 + "," + ASI6 + ","
        + String(beriKE) + "," + String(sumberPMTstr) + "," + String(beriPUSATstr) + "," + String(tahunProduksi) + "," 
        + String(beriDAERAHstr) + "\n";

    pesanSD.toCharArray(data_sd_char, pesanSD.length()+1);
    appendFile(SD, "/data.csv", data_sd_char);
    // tft.fillScreen(TFT_CYAN);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillScreen(TFT_CYAN);
    tft.setCursor(tft.width()/2-70, tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.print("SAVED");
    EEPROM.put(77,noKolom);
    EEPROM.commit();
    delay(2000);
    
    displayState = 10;
  }
}

void btnBackUbahWIFI_pressAction(void){
  if(btnBackUbahWIFI.justPressed()){
    displayState = 42;
    initButtonSetPLACE();
    initButtonSetWIFI();
  }
}

void btnUbahWIFI_pressAction(void){
  if(btnUbahWIFI.justPressed()){
    drawKeyboardWIFI();
    displayState = 44;
  }
}

void btnBackUBAH_pressAction(void){
  if(btnBackUBAH.justPressed()){
    displayState = 42;
    initButtonSetPLACE();
    initButtonSetWIFI();
  }
}
void btnUBAH_pressAction(void){
  if(btnUBAH.justPressed()){
    drawKeyboardPKM();
    displayState = 19;
  }
}

void btnFILL_pressAction(void){
  if(btnFILL.justPressed()){
    Serial.println(found);
    if(!found){
      displayState = 16;
      drawKeyboardORTU();
    }
    if(found){
      drawKeypadLILA();
      displayState = 25;
    }
  }
}

void btnDELETE_pressAction(void){
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(tft.width()/2-90, tft.height()/2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("DELETED");
  delay(2000);
  ESP.restart();
}

void btnPRINT_pressAction(void){
  
  // if(btnPRINT.justPressed()){
  if(btnRESC.justPressed()){
    if(gender == "L"){
    genderr = "Boy";
  }else if(gender == "P"){
    genderr = "Girl";
  }

  pesanPrinter = String(name) + "\nAge : " + String(usiaBulan) + " Month" + "\nGender : " + genderr 
    + "\nMethod : " + methodd + "\nHeight (cm) : " + String(length, 1) + "\nWeight (kg) : " + String(beratBadan, 1) 
    + "\nLAZ  : " + String(zscore_TBU,1) + "  (" + kategori_TBU + ")"
    + "\nWAZ  : " + String(zscore_BBU,1) + "  (" + kategori_BBU + ")"
    + "\nWLZ  : " + String(zscore_BBTB,1) + "  (" + kategori_BBTB + ")" 
    + "\nBMIZ : " + String(zscore_IMTU,1) + "  (" + kategori_IMTU + ")\n\n";

    Serial.print("Pesan Printer: "); Serial.println(pesanPrinter);
    Serial.print("Pesan Cloud: "); Serial.println(pesanCloud);
    delay(500);

    SerialBT.begin(myName, true);
    // uint8_t address[6]  = {0x86, 0x67, 0x7A, 0xC7, 0x70, 0x33};

    connected = SerialBT.connect(slaveName);
    // connected = SerialBT.connect(addressBT);

    if(connected) {
      Serial.println("Bluetooth CONNECTED!!!");
    } else {
      while(!SerialBT.connected(10000)) { 
        //Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
      }
    }
    
    // Serial.println(pesanPrinter);
    sendPrinter();
    
    // delay(2000);
    // delay(2000);
    // displayState = 10;
  }
}

void btnPRINT2_pressAction(void){
  // if(btnPRINT2.justPressed()){
  if(btnRESC2.justPressed()){
    delay(500);
    SerialBT.begin(myName, true);
    // uint8_t address[6]  = {0x86, 0x67, 0x7A, 0xC7, 0x70, 0x33};

    connected = SerialBT.connect(slaveName);
    // connected = SerialBT.connect(addressBT);

    if(connected) {
      Serial.println("Bluetooth CONNECTED!!!");
    } else {
      while(!SerialBT.connected(10000)) { 
        //Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
      }
    }

    // if(gender == "L"){
    //   genderr = "Boy";
    // }else if(gender == "P"){
    //   genderr = "Girl";
    // }

    // if(method == RECUMBENT){
    //   methodd = "Recumbent";
    // }else if(method == STANDING){
    //   methodd = "Standing";
    // }

    // pesanPrinter = String(name) + "\nAge : " + String(usiaBulan) + " Month" + "\nGender : " + genderr 
    //   + "\nMethod : " + methodd + "\nHeight (cm) : " + String(length, 1) + "\nWeight (kg) : " + String(beratBadan, 1) 
    //   + "\nLAZ  : " + String(zscore_TBU,1) + "  (" + kategori_TBU + ")"
    //   + "\nWAZ  : " + String(zscore_BBU,1) + "  (" + kategori_BBU + ")"
    //   + "\nWLZ  : " + String(zscore_BBTB,1) + "  (" + kategori_BBTB + ")" 
    //   + "\nBMIZ : " + String(zscore_IMTU,1) + "  (" + kategori_IMTU + ")\n\n";
    // Serial.println(pesanPrinter);
    sendPrinter2();
    // delay(3000);
    // ESP.restart();
    // delay(2000);
    // displayState = 40;
    // initButtonPRINT();
  }
}

void btnCAL_pressAction(void){
  if(btnCAL.justPressed()){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
  tft.print("Current Offset Value :");
  tft.setTextSize(1);
  tft.setCursor(tft.width()/2+90,tft.height()/2);
  tft.print(simpangan);
  delay(3000);

  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  drawKeypadCAL();
  displayState = 12;
  }
}

void btnBackCAL_pressAction(void){
  if(btnBackCAL.justPressed()){
    tft.fillRect(0, 0, 480, 320, TFT_CYAN);
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
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("CHOOSE GENDER !");
  
  uint16_t x1 = (tft.width() - BUTTON_W) / 2;
  uint16_t y1 = tft.height() / 2 - BUTTON_H - 10;
  btnBOY.initButtonUL(x1, y1+40, BUTTON_W, BUTTON_H1, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Boy", 1);
  btnBOY.setPressAction(btnBOY_pressAction);
  btnBOY.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonGIRL(){
  uint16_t x1 = (tft.width() - BUTTON_W) / 2;
  uint16_t y1 = tft.height() / 2 - BUTTON_H + 60;
  btnGIRL.initButtonUL(x1, y1+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Girl", 1);
  btnGIRL.setPressAction(btnGIRL_pressAction);
  btnGIRL.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonRCM(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("CHOOSE METHOD !");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnRCM.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Recumbent", 1);
  btnRCM.setPressAction(btnRCM_pressAction);
  btnRCM.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSTD(){
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H + 60;
  btnSTD.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Standing", 1);
  btnSTD.setPressAction(btnSTD_pressAction);
  btnSTD.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI0(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 0 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI0Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI0Y.setPressAction(btnASI0Y_pressAction);
  btnASI0Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI0T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI0T.setPressAction(btnASI0T_pressAction);
  btnASI0T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI1(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 1 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI1Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI1Y.setPressAction(btnASI1Y_pressAction);
  btnASI1Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI1T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI1T.setPressAction(btnASI1T_pressAction);
  btnASI1T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI2(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 2 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI2Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI2Y.setPressAction(btnASI2Y_pressAction);
  btnASI2Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI2T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI2T.setPressAction(btnASI2T_pressAction);
  btnASI2T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI3(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 3 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI3Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI3Y.setPressAction(btnASI3Y_pressAction);
  btnASI3Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI3T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI3T.setPressAction(btnASI3T_pressAction);
  btnASI3T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI4(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 4 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI4Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI4Y.setPressAction(btnASI4Y_pressAction);
  btnASI4Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI4T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI4T.setPressAction(btnASI4T_pressAction);
  btnASI4T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI5(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 5 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI5Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI5Y.setPressAction(btnASI5Y_pressAction);
  btnASI3Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI5T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI5T.setPressAction(btnASI5T_pressAction);
  btnASI5T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonASI6(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(100,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Mendapat ASI pada Usia 6 Bulan?");
  
  uint16_t x2 = (tft.width() - BUTTON_W) / 2;
  uint16_t y2 = tft.height() / 2 - BUTTON_H - 10;
  btnASI6Y.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN, "Ya", 1);
  btnASI6Y.setPressAction(btnASI6Y_pressAction);
  btnASI6Y.drawSmoothButton(false, 3, TFT_BLACK);

  y2 = tft.height() / 2 - BUTTON_H + 60;
  btnASI6T.initButtonUL(x2, y2+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnASI6T.setPressAction(btnASI6T_pressAction);
  btnASI6T.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSTART(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);
  if(millis()>50){
    Serial.print("WiFi: "); Serial.println(wifii);
    Serial.print("Password: "); Serial.println(passwordd);
    if (bacaTegangan() > 3.40){
      tft.fillScreen(TFT_GREEN);
      tft.setCursor(150,tft.height()/2-90);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(1);
      tft.print("Battery = ");
      tft.setCursor(240,tft.height()/2-90);
      tft.print(bacaTegangan());
      tft.setCursor(280,tft.height()/2-90);
      tft.print("V");
      Serial.println(bacaTegangan());
    }
    else if ( bacaTegangan() >= 3.00 && bacaTegangan() <= 3.40){
      tft.fillScreen(TFT_YELLOW);
      tft.setCursor(150,tft.height()/2-90);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(1);
      tft.print("Battery = ");
      tft.setCursor(240,tft.height()/2-90); //160
      tft.print(bacaTegangan());
      tft.setCursor(280,tft.height()/2-90); //220
      tft.print("V");
      Serial.println(bacaTegangan());
    }
    else if( bacaTegangan() < 3.00){
      tft.fillScreen(TFT_RED);
      tft.setCursor(150,tft.height()/2-90); //160
      tft.print("  LOW BATTERY !!");
      Serial.println(bacaTegangan());    
    }
  }

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 50; //10;
  btnSTART.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_BLUE, TFT_WHITE, TFT_BLUE, "Start", 1);
  btnSTART.setPressAction(btnSTART_pressAction);
  btnSTART.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonCAL(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H + 15; //60;
  btnCAL.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Calibrate", 1);
  btnCAL.setPressAction(btnCAL_pressAction);
  btnCAL.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSETTINGS(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H + 80; //60;
  btnSETTINGS.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Settings", 1);
  btnSETTINGS.setPressAction(btnSETTINGS_pressAction);
  btnSETTINGS.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSetPLACE(){
  tft.fillScreen(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 35; //60;
  btnSetPLACE.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Set Place", 1);
  btnSetPLACE.setPressAction(btnSetPLACE_pressAction);
  btnSetPLACE.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSetWIFI(){
  tft.setFreeFont(&FreeSansBold9pt7b);
  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H + 30; //60;
  btnSetWIFI.initButtonUL(x, y+40, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Set WiFi", 1);
  btnSetWIFI.setPressAction(btnSetWIFI_pressAction);
  btnSetWIFI.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonCALCU(){
  uint16_t x = (tft.width() - BUTTON_W3) / 2;
  uint16_t y = tft.height() / 2;
  btnCALCULATE.initButtonUL(x, y-20, BUTTON_W3, BUTTON_H3, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Calculate", 1);
  btnCALCULATE.setPressAction(btnCALCULATE_pressAction);
  btnCALCULATE.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonUBAH(){
  uint16_t x = tft.width()/2-160;
  uint16_t y = tft.height() - 70; //80; //110;
  btnBackUBAH.initButtonUL(x, y, BUTTON_W3-10, BUTTON_H-15, TFT_BLUE, TFT_BLUE, TFT_WHITE, "BACK", 1);
  btnBackUBAH.setPressAction(btnBackUBAH_pressAction);
  btnBackUBAH.drawSmoothButton(false, 3, TFT_BLACK);

  x = tft.width()/2+20; //tft.width()/2+25;
  btnUBAH.initButtonUL(x, y, BUTTON_W3-10, BUTTON_H-15, TFT_RED, TFT_RED, TFT_WHITE, "CHANGE", 1);
  btnUBAH.setPressAction(btnUBAH_pressAction);
  btnUBAH.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonUbahWIFI(){
  tft.fillScreen(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(100, 80);
  tft.print("CURRENT WIFI INFORMATION");
  tft.setCursor(35, 140);
  tft.print("WiFi");
  tft.setCursor(tft.width()/2-100, 140);
  tft.print(":");
  tft.setCursor(tft.width()/2-90, 140);
  tft.print(wifii);
  tft.setCursor(35, 170);
  tft.print("Password");
  tft.setCursor(tft.width()/2-100, 170);
  tft.print(":");
  tft.setCursor(tft.width()/2-90, 170);
  tft.print(passwordd);


  uint16_t x = tft.width()/2-160;
  uint16_t y = tft.height() - 70; //80; //110;
  btnBackUbahWIFI.initButtonUL(x, y, BUTTON_W3-10, BUTTON_H-15, TFT_BLUE, TFT_BLUE, TFT_WHITE, "BACK", 1);
  btnBackUbahWIFI.setPressAction(btnBackUbahWIFI_pressAction);
  btnBackUbahWIFI.drawSmoothButton(false, 3, TFT_BLACK);

  x = tft.width()/2+20; //tft.width()/2+25;
  // y = tft.height() - 80; //30;
  btnUbahWIFI.initButtonUL(x, y, BUTTON_W3-10, BUTTON_H-15, TFT_RED, TFT_RED, TFT_WHITE, "CHANGE", 1);
  btnUbahWIFI.setPressAction(btnUbahWIFI_pressAction);
  btnUbahWIFI.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonFILL(){
  uint16_t x = tft.width()/2+25;
  uint16_t y = 20; //30; //30;
  btnFILL.initButtonUL(x, y, BUTTON_W3, BUTTON_H-20, TFT_BLUE, TFT_BLUE, TFT_WHITE, "FILL DATA", 1);
  btnFILL.setPressAction(btnFILL_pressAction);
  btnFILL.drawSmoothButton(false, 3, TFT_BLACK);

  x = tft.width()/2+25;
  y = 63; //80; //80; //110;
  btnRES.initButtonUL(x, y, BUTTON_W3, BUTTON_H-20, TFT_RED, TFT_RED, TFT_WHITE, "SAVE", 1);
  btnRES.setPressAction(btnRES_pressAction);
  btnRES.drawSmoothButton(false, 3, TFT_BLACK);

  // x = tft.width()/2+25;
  // y = 106; //80; //80; //110;
  // btnRESC.initButtonUL(x, y, BUTTON_W3, BUTTON_H-20, TFT_RED, TFT_RED, TFT_WHITE, "SAVE ALL", 1);
  // btnRESC.setPressAction(btnRESC_pressAction);
  // btnRESC.drawSmoothButton(false, 3, TFT_BLACK);

  x = tft.width()/2+25;
  y = 106; //80; //110;
  btnRESC.initButtonUL(x, y, BUTTON_W3, BUTTON_H-20, TFT_DARKGREEN, TFT_DARKGREEN, TFT_WHITE, "PRINT", 1);
  btnRESC.setPressAction(btnPRINT_pressAction);
  btnRESC.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonPRINT(){
  if(method == RECUMBENT){
    methodd = "Recumbent";
  }else if(method == STANDING){
    methodd = "Standing";
  }

  EEPROM.get(1,pkm);
  EEPROM.get(22,kelurahan);
  EEPROM.get(53,posyandu);
  EEPROM.get(94,address);
  EEPROM.get(115,rt);
  EEPROM.get(136,rw);
  EEPROM.get(77,noKolom);
  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);

  if(noKolom <= 0){
    noKolom = 0;
  }
  noKolom += 1;
  // Serial.print("Pesan SD: "); Serial.println(pesanSD);
  pesanSD = String(noKolom) + "," + String(nik) + "," + String(name) + "," + String(tanggalLahir) + "," + String(usiaTahun) + ","
      + String(usiaBulan) + "," + gender + "," + String(ortu) + "," + String(nikORTU) + "," + String(phone) + "," + String(pkm) + "," 
      + String(kelurahan) + "," + String(posyandu) + "," + String(rt) + "," + String(rw) + "," + String(address) + ","  
      + String(todaysDate) + "," + String(length,1) + "," + methodd + "," + String(beratBadan,1) + "," + String(lila) + ","
      + String(vitaa) + "," + String(like) + ","
      + ASI0 + "," + ASI1 + "," + ASI2 + "," + ASI3 + "," + ASI4 + "," + ASI5 + "," + ASI6 + ","
      + String(beriKE) + "," + String(sumberPMTstr) + "," + String(beriPUSATstr) + "," + String(tahunProduksi) + "," 
      + String(beriDAERAHstr) + "\n";

  nikStr = String(nik); nameStr = String(name); tanggalLahirStr = String(tanggalLahir); usiaTahunStr = String(usiaTahun); usiaBulanStr = String(usiaBulan);
  ortuStr = String(ortu); nikOrtuStr = String(nikORTU); phoneStr = String(phone); pkmStr = String(pkm); kelurahanStr = String(kelurahan); posyanduStr = String(posyandu); rtStr = String(rt); rwStr = String(rw);
  addressStr = String(address); todaysDateStr = String(todaysDate); lengthStr = String(length, 1); beratBadanStr = String(beratBadan, 1); lilaStr = String(lila); vitaStr = String(vitaa); 
  likeStr = String(like); beriKeStr = String(beriKE); tahunProduksiStr = String(tahunProduksi);
      
  pesanCloud = "NIK=" + urlencode(nikStr) + "&nama_anak=" + urlencode(nameStr) + "&tgl_lahir=" + urlencode(tanggalLahirStr) + "&umur_tahun=" + urlencode(usiaTahunStr) + "&umur_bulan="
      + urlencode(usiaBulanStr) + "&jenis_kelamin=" + urlencode(gender) + "&nama_ortu=" + urlencode(ortuStr) + "&nik_ortu=" + urlencode(nikOrtuStr) + "&hp_ortu=" + urlencode(phoneStr) + "&PKM=" + urlencode(pkmStr) + "&KEL=" 
      + urlencode(kelurahanStr) + "&POSY=" + urlencode(posyanduStr) + "&RT=" + urlencode(rtStr) + "&RW=" + urlencode(rwStr) + "&ALAMAT=" + urlencode(addressStr) + "&TANGGALUKUR="  
      + urlencode(todaysDateStr) + "&TINGGI=" + urlencode(lengthStr) + "&CARAUKUR=" + urlencode(methodd) + "&BERAT=" + urlencode(beratBadanStr) 
      + "&LILA=" + urlencode(lilaStr) + "&vita=" + urlencode(vitaStr) + "&lingkar_kepala=" + urlencode(likeStr) + "&asi_bulan_0="
      + urlencode(ASI0) + "&asi_bulan_1=" + urlencode(ASI1) + "&asi_bulan_2=" + urlencode(ASI2) + "&asi_bulan_3=" + urlencode(ASI3) + "&asi_bulan_4=" + urlencode(ASI4) + "&asi_bulan_5=" + urlencode(ASI5) + "&asi_bulan_6=" + urlencode(ASI6) + "&pemberian_ke="
      + urlencode(beriKeStr) + "&sumber_pmt=" + urlencode(sumberPMTstr) + "&pemberian_pusat=" + urlencode(beriPUSATstr) + "&tahun_produksi=" + urlencode(tahunProduksiStr) + "&pemberian_daerah=" 
      + urlencode(beriDAERAHstr);

  if(gender == "L"){
    genderr = "Boy";
  }else if(gender == "P"){
    genderr = "Girl";
  }

  pesanPrinter = String(name) + "\nAge : " + String(usiaBulan) + " Month" + "\nGender : " + genderr 
    + "\nMethod : " + methodd + "\nHeight (cm) : " + String(length, 1) + "\nWeight (kg) : " + String(beratBadan, 1) 
    + "\nLAZ  : " + String(zscore_TBU,1) + "  (" + kategori_TBU + ")"
    + "\nWAZ  : " + String(zscore_BBU,1) + "  (" + kategori_BBU + ")"
    + "\nWLZ  : " + String(zscore_BBTB,1) + "  (" + kategori_BBTB + ")" 
    + "\nBMIZ : " + String(zscore_IMTU,1) + "  (" + kategori_IMTU + ")\n\n";

  EEPROM.put(77,noKolom);
  EEPROM.commit();

  tft.fillScreen(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 85; //120; //30;
  btnRES2.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "SAVE", 1);
  btnRES2.setPressAction(btnRES2_pressAction);
  btnRES2.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H - 15; //50; //30;
  btnRESC2.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "PRINT", 1);
  btnRESC2.setPressAction(btnPRINT2_pressAction);
  btnRESC2.drawSmoothButton(false, 3, TFT_BLACK);

  // y = tft.height() / 2 - BUTTON_H - 15; //50; //30;
  // btnRESC2.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "SAVE ALL", 1);
  // btnRESC2.setPressAction(btnRESC2_pressAction);
  // btnRESC2.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H + 55; //80; //110;
  btnDELETE2.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_RED, TFT_WHITE, "DELETE", 1);
  btnDELETE2.setPressAction(btnDELETE_pressAction);
  btnDELETE2.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonSumberPMT(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(150, tft.height()/2-70);
  tft.print("Sumber PMT");

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 10; //30;
  btnSumberPUSAT.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Pusat", 1);
  btnSumberPUSAT.setPressAction(btnSumberPUSAT_pressAction);
  btnSumberPUSAT.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H + 60; //130; //110;
  btnSumberDAERAH.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Daerah", 1);
  btnSumberDAERAH.setPressAction(btnSumberDAERAH_pressAction);
  btnSumberDAERAH.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonBeriPUSAT(){
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(150,tft.height()/2-70); //60
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(150, tft.height()/2-70);
  tft.print("Pemberian Pusat");

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 10; //30;
  btnPUSATY.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Ya", 1);
  btnPUSATY.setPressAction(btnPUSATY_pressAction);
  btnPUSATY.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H + 60; //130; //110;
  btnPUSATT.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnPUSATT.setPressAction(btnPUSATT_pressAction);
  btnPUSATT.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonVITA(){
  tft.fillScreen(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(200, tft.height()/2-70);
  tft.print("VITA");

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 10; //30;
  btnVITAY.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Ya", 1);
  btnVITAY.setPressAction(btnVITAY_pressAction);
  btnVITAY.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H + 60; //130; //110;
  btnVITAT.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnVITAT.setPressAction(btnVITAT_pressAction);
  btnVITAT.drawSmoothButton(false, 3, TFT_BLACK);
}

void initButtonBeriDAERAH(){
  tft.fillScreen(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(150, tft.height()/2-70);
  tft.print("Pemberian Daerah");

  uint16_t x = (tft.width() - BUTTON_W) / 2;
  uint16_t y = tft.height() / 2 - BUTTON_H - 10; //30;
  btnDAERAHY.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Ya", 1);
  btnDAERAHY.setPressAction(btnDAERAHY_pressAction);
  btnDAERAHY.drawSmoothButton(false, 3, TFT_BLACK);

  y = tft.height() / 2 - BUTTON_H + 60; //130; //110;
  btnDAERAHT.initButtonUL(x, y+40, BUTTON_W, BUTTON_H1, TFT_WHITE, TFT_DARKGREEN, TFT_WHITE, "Tidak", 1);
  btnDAERAHT.setPressAction(btnDAERAHT_pressAction);
  btnDAERAHT.drawSmoothButton(false, 3, TFT_BLACK);
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
  btnSAVE.drawSmoothButton(false, 3, TFT_BLACK);
}
//-------------END of Init BUTTON------------

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  EEPROM.get(69,simpangan);
  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);
  ulangState = false;
  spiSD.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); // Initialize the SPI bus
  if(!SD.begin(SS_PIN, spiSD)){ // Begin the SD card with the new SPI instance
    tft.setTextColor(TFT_BLACK);
    // tft.println("ERROR");
    return;
  }
  // SerialBT.begin(myName, true);
  // // uint8_t address[6]  = {0x86, 0x67, 0x7A, 0xC7, 0x70, 0x33};

  // // connected = SerialBT.connect(slaveName);
  // connected = SerialBT.connect(slaveName);

  // if(connected) {
  //   Serial.println("Bluetooth CONNECTED!!!");
  // } else {
  //   while(!SerialBT.connected(10000)) { 
  //     //Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
  //   }
  // }

  tft.begin();
  tft.setRotation(3); //(3); //1
  // tft.setTouchRotation(TFT_MIRROR);
  // widget.setTouchRotation(3);
  tft.setFreeFont(&FreeSansBold9pt7b); //FF18
  pinMode(buzzer, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
  initButtonSTART();
  initButtonCAL();
  initButtonSETTINGS();
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
  const char delimiter[] = "/";

//-------------Display STATE------------
  switch(displayState){
  case 1:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 0; b <= 2; b++) {
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
  for (uint8_t b = 0; b < 30; b++) {
    if (pressed && keyNAME[b].contains(t_x, t_y)) {
      keyNAME[b].press(true);  // tell the button it is pressed
    } else {
      keyNAME[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 30; b++) {

    if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyNAME[b].justReleased()) keyNAME[b].drawButton();     // draw normal

    if (keyNAME[b].justPressed()) {
      keyNAME[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b < 28 && b != 23) {
        if (indexName < NUM_LEN_19) {
          name[indexName] = keyboardLabel[b][0];
          indexName++;
          name[indexName] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 23) {
        name[indexName] = 0;
        if (indexName > 0) {
          indexName--;
          name[indexName] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 29) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 3;
        initButtonBOY();
        initButtonGIRL();
        break;
      }
      
      if (b == 28) {
        status("Value cleared", TFT_CYAN);
        indexName = 0; // Reset index to 0
        name[indexName] = 0; // Place null in buffer
      }

      if (b != 29){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(name, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 3:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 3; b <= 4; b++) {
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
  
  case 4:
  // tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
  //     tft.setFreeFont(&FreeSans18pt7b);
  tft.setFreeFont(&FreeSansBold9pt7b); //tft.setFreeFont(&FreeSansBold9pt7b);
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(tft.width()/2-100, tft.height()/2-20);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.print("Searching...");
  readFile(SD, "/data.csv");
  myFile = SD.open("/data.csv", FILE_READ);
  found = false;
  if (myFile) {
    while (myFile.available()) {
      Serial.println("csv available");
      String line = myFile.readStringUntil('\n');
      Serial.print("NIK: ");
      Serial.println(String(nik));
      if (line.indexOf(String(nik)) != -1) {
        // Data ditemukan, lakukan sesuatu dengan baris ini
        Serial.println(line);
        int pos = 0;
        for (int i = 0; i < 2; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeTiga = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-3 (Nama Balita): " + dataKeTiga);
        strcpy(name, dataKeTiga.c_str());
        Serial.println(name);

        for (int i = 0; i < 1; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeEmpat = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-4 (Tanggal Lahir): " + dataKeEmpat);
        strcpy(tanggalLahir, dataKeEmpat.c_str());
        Serial.println(tanggalLahir);
        // break;

        for (int i = 0; i < 3; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeTujuh = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-7 (Gender): " + dataKeTujuh);
        gender = dataKeTujuh;
        Serial.println(gender);

        for (int i = 0; i < 1; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeDelapan = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-8 (Nama Ortu): " + dataKeDelapan);
        strcpy(ortu, dataKeDelapan.c_str());
        Serial.println(ortu);

        for (int i = 0; i < 1; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeSembilan = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-9 (NIK Ortu): " + dataKeSembilan);
        strcpy(nikORTU, dataKeSembilan.c_str());
        Serial.println(nikORTU);

        for (int i = 0; i < 1; i++) {
          pos = line.indexOf(',', pos) + 1;
        }
        String dataKeSepuluh = line.substring(pos, line.indexOf(',', pos));
        Serial.println("Data Ke-10 (HP Ortu): " + dataKeSepuluh);
        strcpy(phone, dataKeSepuluh.c_str());
        Serial.println(phone);

        Serial.println("FOUND!!!");
        found = true;

        // delay(2000);
        break;
      }
    }
    myFile.close();
    if(!found) {
      Serial.print("no data "); // Print if "VERONIKA" is not found
      Serial.println(name);
      tft.setCursor(tft.width()/2-7*12, tft.height()/2+30);
      tft.print("No Data");
      delay(2000);
      // displayState = 5;
      // drawKeypadBIRTH();
      drawKeyboardNAME();
      displayState = 2;
      break;
    }
    if(found){
      int nameDigit = strlen(name);
      tft.setCursor(tft.width()/2-nameDigit*12, tft.height()/2+30);
      tft.print(name);
      delay(2000);
      displayState = 6;
      Serial.println("FOUNDDDD!!!!!!!");
      break;
    }
  }else{
    drawKeyboardNAME();
    displayState = 2;
    break;
  }
  break; 

  case 5: 
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed4 && keyBIRTH[b].contains(t_x4, t_y4)) {
      keyBIRTH[b].press(true);  // tell the button it is pressed
    } else {
      keyBIRTH[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyBIRTH[b].justReleased()) keyBIRTH[b].drawButton();     // draw normal

    if (keyBIRTH[b].justPressed()) {
      keyBIRTH[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggalLahir
      if (b >= 3 && b != 14) {
        if (indexLahir < NUM_LEN_10) {
          tanggalLahir[indexLahir] = keyLabel1[b][0];
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
        status("Sent value to serial port", TFT_CYAN);
        Serial.println(tanggalLahir);
        displayState = 6;
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexLahir = 0; // Reset index to 0
        tanggalLahir[indexLahir] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 3; 
        initButtonBOY();
        initButtonGIRL();
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
  
  case 6: // Calculate umur
  Wire.begin();
  while (!rtc.begin()) {
    Wire.begin();
    Serial.println("Couldn't find RTC");
    tft.fillScreen(TFT_CYAN);
    tft.setCursor(tft.width()/2-160, tft.height()/2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK);
    tft.print("Couldn't find RTC");
    // while (1);
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));

  now = rtc.now();
  sprintf(todaysDate, "%02d/%02d/%04d", now.day(), now.month(), now.year());

  char parsedStrings[5][20];
  
  for (int i = 0; i < NUM_LEN_10; ++i) {
    todaysDatee[i] = todaysDate[i];
  }
  // indexLahir = 0; // Reset index to 0

  token = strtok(todaysDatee,delimiter);
  strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));
  for (int i = 1 ; i< 3; i++){
    token = strtok(NULL,delimiter);
    strncpy(parsedStrings[i],token, sizeof(parsedStrings[i]));
  }

  currentDate = atoi(parsedStrings[0]);
  currentMonth = atoi(parsedStrings[1]);
  currentYear = atoi(parsedStrings[2]);
  
  for (int i = 0; i < NUM_LEN_10; ++i) {
    tanggalLahirr[i] = tanggalLahir[i];
  }
  char parsedStrings2[5][20];
  token2 = strtok(tanggalLahirr,delimiter);
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
  tahun = usiaBulan / 12;
  sisaBulan = usiaBulan - (tahun * 12); // Menghitung bulan

  // Menyimpan hasil dalam format "X Tahun Y bulan"
  if(sisaBulan != 0){
    usiaTahun = String(tahun) + " Tahun " + String(sisaBulan) + " Bulan";
  }else if(sisaBulan == 0){
    usiaTahun = String(tahun) + " Tahun";
  }
  
  // todaysDatee[0] = 0;
  days = 0;
  tft.fillScreen(TFT_CYAN);
  // tft.setTextDatum(TC_DATUM);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(tft.width()/2 -150, tft.height()/2-20 );
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("Umur: ");
  tft.setCursor(tft.width()/2 - 20, tft.height()/2-20 );
  tft.print(usiaBulan);
  tft.setCursor(tft.width()/2 + 40, tft.height()/2-20 );
  tft.print(" Bulan");
  delay(1000);
  if(sisaBulan != 0 && sisaBulan <= 9){
    // usiaTahun = String(tahun) + " Tahun " + String(sisaBulan) + " bulan";
    tft.setCursor(tft.width()/2-130,tft.height()/2+30);
    tft.print(usiaTahun);
  }else if(sisaBulan != 0 && sisaBulan >= 10){
    // usiaTahun = String(tahun) + " Tahun " + String(sisaBulan) + " bulan";
    tft.setCursor(tft.width()/2-150,tft.height()/2+30);
    tft.print(usiaTahun);
  }else if(sisaBulan == 0){
    // usiaTahun = String(tahun) + " Tahun";
    tft.setCursor(tft.width()/2-75,tft.height()/2+30);
    tft.print(usiaTahun);
  }
  
  delay(3000);

  SRF.setCorrectionFactor(1.035);
  Wire.end();
  displayState = 7;
  drawKeypadWEIGHT();
  break;
  
  case 7: 
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyWEIGHT[b].contains(t_x, t_y)) {
      keyWEIGHT[b].press(true);  // tell the button it is pressed
    } else {
      keyWEIGHT[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyWEIGHT[b].justReleased()) keyWEIGHT[b].drawButton();     // draw normal

    if (keyWEIGHT[b].justPressed()) {
      keyWEIGHT[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexWeight < NUM_LEN_5) {
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
        status("Sent value to serial port", TFT_CYAN);
        displayState = 8;
        initButtonRCM();
        initButtonSTD();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexWeight = 0; // Reset index to 0
        bodyWeight[indexWeight] = 0; // Place null in buffer
      }

      if (b == 14) {
        if(found){
          drawKeypadNIK();
          displayState = 15;
        }
        if(!found){
          displayState = 5; 
          drawKeypadBIRTH();
        }
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

  case 8:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 5; b <= 6; b++) {
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
  newAverage = movingAverage1(newAver+simpangan, 0);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Serial.print(newDistance); Serial.print(" ");
    // Serial.println(newAverage);
    tft.fillRect((tft.width()-120)/2-12, tft.height()/2-75, 140, 40, TFT_BLACK);
    if (newAverage+simpangan >= 100){
      tft.setCursor((tft.width()-120)/2 + 10, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage, 1);
    }else if (newAverage+simpangan < 10){
      tft.setCursor((tft.width()-120)/2 + 30, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage, 1);
    }else{
      tft.setCursor((tft.width()-120)/2 + 20, tft.height()/2-43);
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BROWN);
      tft.print(newAverage, 1);
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
  //Output
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
  
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.fillScreen(TFT_CYAN);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);

  tft.setCursor(35, 30);
  tft.print(name);

  tft.setCursor(35, 60);
  tft.print("Age :");
  tft.setCursor(tft.width()/2-157, 60);
  tft.print(usiaBulan);
  if (usiaBulan < 10){
    tft.setCursor(tft.width()/2-137, 60);
    tft.print("month");
  }else{
    tft.setCursor(tft.width()/2-127, 60);
    tft.print("month");
  }

  tft.setCursor(35, 90);
  tft.print("Gender :");
  tft.setCursor(tft.width()/2-130, 90);
  if(gender == "L"){
    tft.print("Boy");
  }
  else if (gender == "P"){
    tft.print("Girl");
  }

  tft.setCursor(35, 120);
  tft.print("Method :");
  tft.setCursor(tft.width()/2-127, 120);
  if(method == RECUMBENT){
    tft.print("Recumbent");
  }
  else if (method == STANDING){
    tft.print("Standing");
  }

  tft.setCursor(35, 150); //130
  tft.print("Height (cm) :");
  tft.setCursor(tft.width()/2-90, 150);
  tft.print(tinggiBadan, 1);

  tft.setCursor(35, 180); //160
  tft.print("Weight (kg) :");
  tft.setCursor(tft.width()/2-90, 180);
  tft.print(beratBadan, 1);

  tft.setCursor(35, 210); //190
  if(usiaBulan < 24){
    tft.print("Z-Score LAZ");
  }else if(usiaBulan >= 24){
    tft.print("Z-Score HAZ");
  }
  tft.setCursor(tft.width()/2-80, 210);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 210);
  tft.print(zscore_TBU, 1);
  tft.setCursor(tft.width()/2-13, 210);
  tft.print(kategori_TBU);

  tft.setCursor(35, 240); //220
  tft.print("Z-Score WAZ");
  tft.setCursor(tft.width()/2-80, 240);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 240);
  tft.print(zscore_BBU, 1);
  tft.setCursor(tft.width()/2-13, 240);
  tft.print(kategori_BBU);

  tft.setCursor(35, 270);
  if(usiaBulan < 24){
    tft.print("Z-Score WLZ");
  }else if(usiaBulan >= 24){
    tft.print("Z-Score WHZ");
  }
  tft.setCursor(tft.width()/2-80, 270);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 270);
  tft.print(zscore_BBTB, 1);
  tft.setCursor(tft.width()/2-13, 270);
  tft.print(kategori_BBTB);

  tft.setCursor(35, 300);
  tft.print("Z-Score BMIZ");
  tft.setCursor(tft.width()/2-80, 300);
  tft.print(":");
  tft.setCursor(tft.width()/2-70, 300);
  tft.print(zscore_IMTU, 1);
  tft.setCursor(tft.width()/2-13, 300); //+25
  tft.print(kategori_IMTU);

  if(method == RECUMBENT){
    methodd = "Recumbent";
  }else if(method == STANDING){
    methodd = "Standing";
  }

  EEPROM.get(1,pkm);
  EEPROM.get(22,kelurahan);
  EEPROM.get(53,posyandu);
  EEPROM.get(94,address);
  EEPROM.get(115,rt);
  EEPROM.get(136,rw);
  EEPROM.get(77,noKolom);
  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);

  if(noKolom <= 0){
    noKolom = 0;
  }
  noKolom += 1;
  // Serial.print("Pesan SD: "); Serial.println(pesanSD);
  pesanSD = String(noKolom) + "," + String(nik) + "," + String(name) + "," + String(tanggalLahir) + "," + String(usiaTahun) + ","
      + String(usiaBulan) + "," + gender + "," + String(ortu) + "," + String(nikORTU) + "," + String(phone) + "," + String(pkm) + "," 
      + String(kelurahan) + "," + String(posyandu) + "," + String(rt) + "," + String(rw) + "," + String(address) + ","  
      + String(todaysDate) + "," + String(length,1) + "," + methodd + "," + String(beratBadan,1) + "," + String(lila) + ","
      + String(vitaa) + "," + String(like) + ","
      + ASI0 + "," + ASI1 + "," + ASI2 + "," + ASI3 + "," + ASI4 + "," + ASI5 + "," + ASI6 + ","
      + String(beriKE) + "," + String(sumberPMTstr) + "," + String(beriPUSATstr) + "," + String(tahunProduksi) + "," 
      + String(beriDAERAHstr) + "\n";
  
  EEPROM.put(77,noKolom);
  EEPROM.commit();
  
  initButtonFILL();
  displayState = 11;
  break;
    
  case 11:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 8; b <= 10; b++) {
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

  case 12:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyCalibrate[b].contains(t_x, t_y)) {
      keyCalibrate[b].press(true);  // tell the button it is pressed
    } else {
      keyCalibrate[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyCalibrate[b].justReleased()) keyCalibrate[b].drawButton();

    if (keyCalibrate[b].justPressed()) {
      keyCalibrate[b].drawButton(true);
      if (b >= 3) {
        if (indexCalibrate < NUM_LEN_5) {
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
        status("Sent value to serial port", TFT_CYAN);
        Serial.println(calibrate);
        simpangan = atof(calibrate);
        EEPROM.put(69,simpangan);
        EEPROM.commit();       
        displayState = 13;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
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
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(tft.width()/2-90, 50);
  tft.setFreeFont(LABEL1_FONT);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
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
  newAverage = movingAverage1(newAver+simpangan, 0);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    tft.fillRect(tft.width()/2+20, tft.height()/2-60, 120, 40, TFT_BLUE);
    tft.setCursor(tft.width()/2+20, tft.height()/2-30);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    // akhirCAL = round(newAverage + simpangan, 1);
    tft.print(newAverage, 1);
  }

  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed4 = tft.getTouch(&t_x4, &t_y4);
    scanTime = millis();
    for (uint8_t b = 12; b <= 13; b++) {
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
  
  case 15:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed2 && keyNIK[b].contains(t_x2, t_y2)) {
      keyNIK[b].press(true);  // tell the button it is pressed
    } else {
      keyNIK[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyNIK[b].justReleased()) keyNIK[b].drawButton();     // draw normal

    if (keyNIK[b].justPressed()) {
      keyNIK[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexNik < NUM_LEN_16) {
          nik[indexNik] = keyLabel2[b][0];
          indexNik++;
          nik[indexNik] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        nik[indexNik] = 0;
        if (indexNik > 0) {
          indexNik--;
          nik[indexNik] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 4;
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexNik = 0; // Reset index to 0
        nik[indexNik] = 0; // Place null in buffer
      }

      if (b == 14) {
        displayState = 1;
        initButtonSTART();
        initButtonCAL();
        initButtonSETTINGS();
      }

      if (b != 2 && b != 14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(nik, DISP_X + 4, DISP_Y + 12);
      nikString = nik;
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 16:
  for (uint8_t b = 0; b < 30; b++) {
    if (pressed && keyORTU[b].contains(t_x, t_y)) {
      keyORTU[b].press(true);  // tell the button it is pressed
    } else {
      keyORTU[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 30; b++) {

    if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyORTU[b].justReleased()) keyORTU[b].drawButton();     // draw normal

    if (keyORTU[b].justPressed()) {
      keyORTU[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b < 28 && b != 23) {
        if (indexOrtu < NUM_LEN_19) {
          ortu[indexOrtu] = keyboardLabel[b][0];
          indexOrtu++;
          ortu[indexOrtu] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 23) {
        ortu[indexOrtu] = 0;
        if (indexOrtu > 0) {
          indexOrtu--;
          ortu[indexOrtu] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 29) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 17;
        drawKeypadNikORTU();
        // }
        break;
      }
      
      if (b == 28) {
        status("Value cleared", TFT_CYAN);
        indexOrtu = 0; // Reset index to 0
        ortu[indexOrtu] = 0; // Place null in buffer
      }

      if (b != 29){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(ortu, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 17:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyNikORTU[b].contains(t_x, t_y)) {
      keyNikORTU[b].press(true);  // tell the button it is pressed
    } else {
      keyNikORTU[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyNikORTU[b].justReleased()) keyNikORTU[b].drawButton();

    if (keyNikORTU[b].justPressed()) {
      keyNikORTU[b].drawButton(true);
      if (b >= 3 && b != 14) {
        if (indexNikORTU < NUM_LEN_16) {
          nikORTU[indexNikORTU] = keyLabel2[b][0];
          indexNikORTU++;
          nikORTU[indexNikORTU] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }
      if (b == 1) {
        nikORTU[indexNikORTU] = 0;
        if (indexNikORTU > 0) {
          indexNikORTU--;
          nikORTU[indexNikORTU] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 18;
        drawKeypadPhone();
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexNikORTU = 0; // Reset index to 0
        nikORTU[indexNikORTU] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 16;
        drawKeyboardORTU();
      }

      if(b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(nikORTU, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      
      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 18:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyPhone[b].contains(t_x, t_y)) {
      keyPhone[b].press(true);  // tell the button it is pressed
    } else {
      keyPhone[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyPhone[b].justReleased()) keyPhone[b].drawButton();

    if (keyPhone[b].justPressed()) {
      keyPhone[b].drawButton(true);
      if (b >= 3 && b != 14) {
        if (indexPhone < NUM_LEN_13) {
          phone[indexPhone] = keyLabel2[b][0];
          indexPhone++;
          phone[indexPhone] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }
      if (b == 1) {
        phone[indexPhone] = 0;
        if (indexPhone > 0) {
          indexPhone--;
          phone[indexPhone] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        drawKeypadLILA();
        displayState = 25;
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexPhone = 0; // Reset index to 0
        phone[indexPhone] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 17;
        drawKeypadNikORTU();
      }

      if(b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(phone, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      
      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 19:
  for (uint8_t b = 0; b < 30; b++) {
    if (pressed && keyPKM[b].contains(t_x, t_y)) {
      keyPKM[b].press(true);  // tell the button it is pressed
    } else {
      keyPKM[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 30; b++) {

    if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyPKM[b].justReleased()) keyPKM[b].drawButton();     // draw normal

    if (keyPKM[b].justPressed()) {
      keyPKM[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b < 28 && b != 23) {
        if (indexPKM < NUM_LEN_19) {
          pkm[indexPKM] = keyboardLabel[b][0];
          indexPKM++;
          pkm[indexPKM] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 23) {
        pkm[indexPKM] = 0;
        if (indexPKM > 0) {
          indexPKM--;
          pkm[indexPKM] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 29) {
        status("Sent value to serial port", TFT_CYAN);
        pkmm = String(pkm);
        EEPROM.put(1,pkm);
        EEPROM.commit();
        displayState = 20;
        drawKeyboardKELURAHAN();
        // }
        break;
      }
      
      if (b == 28) {
        status("Value cleared", TFT_CYAN);
        indexPKM = 0; // Reset index to 0
        pkm[indexPKM] = 0; // Place null in buffer
      }

      if (b != 29){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(pkm, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 20:
  for (uint8_t b = 0; b < 30; b++) {
    if (pressed && keyKELURAHAN[b].contains(t_x, t_y)) {
      keyKELURAHAN[b].press(true);  // tell the button it is pressed
    } else {
      keyKELURAHAN[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 30; b++) {

    if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyKELURAHAN[b].justReleased()) keyKELURAHAN[b].drawButton();     // draw normal

    if (keyKELURAHAN[b].justPressed()) {
      keyKELURAHAN[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b < 28 && b != 23) {
        if (indexKELURAHAN < NUM_LEN_19) {
          kelurahan[indexKELURAHAN] = keyboardLabel[b][0];
          indexKELURAHAN++;
          kelurahan[indexKELURAHAN] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 23) {
        kelurahan[indexKELURAHAN] = 0;
        if (indexKELURAHAN > 0) {
          indexKELURAHAN--;
          kelurahan[indexKELURAHAN] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 29) {
        status("Sent value to serial port", TFT_CYAN);
        kelurahann = String(kelurahan);
        EEPROM.put(22,kelurahan);
        EEPROM.commit();
        displayState = 21;
        drawKeyboardPOSYANDU();
        // }
        break;
      }
      
      if (b == 28) {
        status("Value cleared", TFT_CYAN);
        indexKELURAHAN = 0; // Reset index to 0
        kelurahan[indexKELURAHAN] = 0; // Place null in buffer
      }

      if (b != 29){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(kelurahan, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 21:
  for (uint8_t b = 0; b < 30; b++) {
    if (pressed && keyPOSYANDU[b].contains(t_x, t_y)) {
      keyPOSYANDU[b].press(true);  // tell the button it is pressed
    } else {
      keyPOSYANDU[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 30; b++) {

    if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyPOSYANDU[b].justReleased()) keyPOSYANDU[b].drawButton();     // draw normal

    if (keyPOSYANDU[b].justPressed()) {
      keyPOSYANDU[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b < 28 && b != 23) {
        if (indexPOSYANDU < NUM_LEN_19) {
          posyandu[indexPOSYANDU] = keyboardLabel[b][0];
          indexPOSYANDU++;
          posyandu[indexPOSYANDU] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 23) {
        posyandu[indexPOSYANDU] = 0;
        if (indexPOSYANDU > 0) {
          indexPOSYANDU--;
          posyandu[indexPOSYANDU] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 29) {
        status("Sent value to serial port", TFT_CYAN);
        posyanduu = String(posyandu);
        EEPROM.put(53,posyandu);
        EEPROM.commit();
        displayState = 22;
        drawKeyboardADDRESS();
        // }
        break;
      }
      
      if (b == 28) {
        status("Value cleared", TFT_CYAN);
        indexPOSYANDU = 0; // Reset index to 0
        posyandu[indexPOSYANDU] = 0; // Place null in buffer
      }

      if (b != 29){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(posyandu, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 22:
  for (uint8_t b = 0; b < 45; b++) {
    if (pressed && keyADDRESS[b].contains(t_x, t_y)) {
      keyADDRESS[b].press(true);  // tell the button it is pressed
    } else {
      keyADDRESS[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 45; b++) {

    if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyADDRESS[b].justReleased()) keyADDRESS[b].drawButton();     // draw normal

    if (keyADDRESS[b].justPressed()) {
      keyADDRESS[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b != 17 && b != 26 && b != 35 && b != 44) {
        if (indexADDRESS < NUM_LEN_19) {
          address[indexADDRESS] = keyboardFullLabel[b][0];
          indexADDRESS++;
          address[indexADDRESS] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 26) { //DELETE
        address[indexADDRESS] = 0;
        Serial.println("Delete Pressed");
        if (indexADDRESS > 0) {
          indexADDRESS--;
          address[indexADDRESS] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 35){ //BACK
        displayState = 21;
        drawKeyboardPOSYANDU();
      }

      if (b == 44) { //SEND
        status("Sent value to serial port", TFT_CYAN);
        addresss = String(address);
        EEPROM.put(94,address);
        EEPROM.commit();
        displayState = 23;
        drawKeypadRT();
        break;
      }
      
      if (b == 17) { //NEW
        status("Value cleared", TFT_CYAN);
        indexADDRESS = 0; // Reset index to 0
        address[indexADDRESS] = 0; // Place null in buffer
      }

      if (b != 44 && b != 35){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(address, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 23:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyRT[b].contains(t_x, t_y)) {
      keyRT[b].press(true);  // tell the button it is pressed
    } else {
      keyRT[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyRT[b].justReleased()) keyRT[b].drawButton();

    if (keyRT[b].justPressed()) {
      keyRT[b].drawButton(true);
      if (b >= 3 && b != 14) {
        if (indexRT < NUM_LEN_2) {
          rt[indexRT] = keyLabel2[b][0];
          indexRT++;
          rt[indexRT] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }
      if (b == 1) {
        rt[indexRT] = 0;
        if (indexRT > 0) {
          indexRT--;
          rt[indexRT] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        rtt = String(rt);
        EEPROM.put(115,rt);
        EEPROM.commit();
        drawKeypadRW();
        displayState = 24;
        // }
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexRT = 0; // Reset index to 0
        rt[indexRT] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 22;
        drawKeyboardADDRESS();
      }

      if(b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(rt, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      
      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 24:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyRW[b].contains(t_x, t_y)) {
      keyRW[b].press(true);  // tell the button it is pressed
    } else {
      keyRW[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyRW[b].justReleased()) keyRW[b].drawButton();

    if (keyRW[b].justPressed()) {
      keyRW[b].drawButton(true);
      if (b >= 3 && b != 14) {
        if (indexRW < NUM_LEN_2) {
          rw[indexRW] = keyLabel2[b][0];
          indexRW++;
          rw[indexRW] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }
      if (b == 1) {
        rw[indexRW] = 0;
        if (indexRW > 0) {
          indexRW--;
          rw[indexRW] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }
      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.fillScreen(TFT_CYAN);
        tft.setCursor(tft.width()/2-130, tft.height()/2);
        tft.setTextSize(2);
        tft.setTextColor(TFT_BLACK);
        tft.print("PLACE SAVED");
        rww = String(rw);
        EEPROM.put(136,rw);
        EEPROM.commit();
        delay(2000);
        ESP.restart();
        // }
        break;
      }
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexRW = 0; // Reset index to 0
        rw[indexRW] = 0; // Place null in buffer
      }
      if (b == 14) {
        displayState = 23;
        drawKeypadRT();
      }

      if(b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(rw, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
      
      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 25:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyLILA[b].contains(t_x, t_y)) {
      keyLILA[b].press(true);  // tell the button it is pressed
    } else {
      keyLILA[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyLILA[b].justReleased()) keyLILA[b].drawButton();     // draw normal

    if (keyLILA[b].justPressed()) {
      keyLILA[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexLILA < NUM_LEN_4) {
          lila[indexLILA] = keyLabel2[b][0];
          indexLILA++;
          lila[indexLILA] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        lila[indexLILA] = 0;
        if (indexLILA > 0) {
          indexLILA--;
          lila[indexLILA] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 26;
        initButtonVITA();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexLILA = 0; // Reset index to 0
        lila[indexLILA] = 0; // Place null in buffer
      }

      if (b == 14) {
        if(found){
          displayState = 10;
          // initButtonFILL();
        }
        if(!found){
          drawKeypadPhone();
          displayState = 18;
        }
      }

      if (b != 2 && b !=14 ){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(lila, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 26:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 39; b <= 40; b++) {
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
  
  case 27:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyLIKE[b].contains(t_x, t_y)) {
      keyLIKE[b].press(true);  // tell the button it is pressed
    } else {
      keyLIKE[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyLIKE[b].justReleased()) keyLIKE[b].drawButton();     // draw normal

    if (keyLIKE[b].justPressed()) {
      keyLIKE[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexLIKE < NUM_LEN_5) {
          like[indexLIKE] = keyLabel2[b][0];
          indexLIKE++;
          like[indexLIKE] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        like[indexLIKE] = 0;
        if (indexLIKE > 0) {
          indexLIKE--;
          like[indexLIKE] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        if (usiaBulan < 7){
          displayState = 28;
          initButtonASI0();
          break;
        }else{
          displayState = 35;
          drawKeypadBeriKE();
          break;
        }
        
        // }
        // break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexLIKE = 0; // Reset index to 0
        like[indexLIKE] = 0; // Place null in buffer
      }

      if (b == 14) {
        displayState = 26; 
        // drawKeypadVITA();
        initButtonVITA();
      }

      if (b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(like, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 28:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 15; b <= 16; b++) {
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

  case 29:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 17; b <= 18; b++) {
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

  case 30:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 19; b <= 20; b++) {
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

  case 31:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 21; b <= 22; b++) {
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

  case 32:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 23; b <= 24; b++) {
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

  case 33:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 25; b <= 26; b++) {
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

  case 34:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 27; b <= 28; b++) {
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

  case 35:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyBeriKE[b].contains(t_x, t_y)) {
      keyBeriKE[b].press(true);  // tell the button it is pressed
    } else {
      keyBeriKE[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyBeriKE[b].justReleased()) keyBeriKE[b].drawButton();     // draw normal

    if (keyBeriKE[b].justPressed()) {
      keyBeriKE[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexBeriKE < NUM_LEN_4) {
          beriKE[indexBeriKE] = keyLabel2[b][0];
          indexBeriKE++;
          beriKE[indexBeriKE] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        beriKE[indexBeriKE] = 0;
        if (indexBeriKE > 0) {
          indexBeriKE--;
          beriKE[indexBeriKE] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 36;
        initButtonSumberPMT();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexBeriKE = 0; // Reset index to 0
        beriKE[indexBeriKE] = 0; // Place null in buffer
      }

      if (b == 14) {
        if (usiaBulan < 7){
          displayState = 34; 
          initButtonASI6();
          break;
        }else{
          // displayState = 35;
          displayState = 27;
          drawKeypadLIKE();
          break;
        }
      }

      if (b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(beriKE, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 36:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 33; b <= 34; b++) {
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
  
  case 37:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 35; b <= 36; b++) {
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
  
  case 38:
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && keyTAHUN[b].contains(t_x, t_y)) {
      keyTAHUN[b].press(true);  // tell the button it is pressed
    } else {
      keyTAHUN[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyTAHUN[b].justReleased()) keyTAHUN[b].drawButton();     // draw normal

    if (keyTAHUN[b].justPressed()) {
      keyTAHUN[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the tanggal
      if (b >= 3 && b != 14) {
        if (indexTAHUN < NUM_LEN_4) {
          tahunProduksi[indexTAHUN] = keyLabel2[b][0];
          indexTAHUN++;
          tahunProduksi[indexTAHUN] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        tahunProduksi[indexTAHUN] = 0;
        if (indexTAHUN > 0) {
          indexTAHUN--;
          tahunProduksi[indexTAHUN] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 2) {
        status("Sent value to serial port", TFT_CYAN);
        displayState = 39;
        initButtonBeriDAERAH();
        break;
      }
      
      if (b == 0) {
        status("Value cleared", TFT_CYAN);
        indexTAHUN = 0; // Reset index to 0
        tahunProduksi[indexTAHUN] = 0; // Place null in buffer
      }

      if (b == 14) {
        displayState = 37; 
        // drawKeyboardBeriPUSAT();
        initButtonBeriPUSAT();
      }

      if (b != 2 && b !=14){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(tahunProduksi, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 39:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 37; b <= 38; b++) {
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
  
  case 40:
  
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 29; b <= 31; b++) {
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

  case 41:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 41; b <= 42; b++) {
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

  case 42:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 43; b <= 44; b++) {
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

  case 43:
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    // Serial.println(pressed);
    scanTime = millis();
    for (uint8_t b = 45; b <= 46; b++) {
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

  case 44:
  for (uint8_t b = 0; b < 45; b++) {
    if (pressed && keyWIFI[b].contains(t_x, t_y)) {
      keyWIFI[b].press(true);  // tell the button it is pressed
    } else {
      keyWIFI[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 45; b++) {

    if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyWIFI[b].justReleased()) keyWIFI[b].drawButton();     // draw normal

    if (keyWIFI[b].justPressed()) {
      keyWIFI[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b != 17 && b != 26 && b != 35 && b != 44) {
        if (indexWIFI < NUM_LEN_19) {
          wifi[indexWIFI] = keyboardFullLabel[b][0];
          indexWIFI++;
          wifi[indexWIFI] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 26) { //DELETE
        wifi[indexWIFI] = 0;
        Serial.println("Delete Pressed");
        if (indexWIFI > 0) {
          indexWIFI--;
          wifi[indexWIFI] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 35){ //BACK
        displayState = 43;
        initButtonUbahWIFI();
        // displayState = 21;
        // drawKeyboardPOSYANDU();
      }

      if (b == 44) { //SEND
        status("Sent value to serial port", TFT_CYAN);
        wifii = wifi;
        EEPROM.put(250,wifii);
        EEPROM.commit();
        displayState = 45;
        drawKeyboardPASSWORD();
        // }
        break;
      }
      
      if (b == 17) { //NEW
        status("Value cleared", TFT_CYAN);
        indexWIFI = 0; // Reset index to 0
        wifi[indexWIFI] = 0; // Place null in buffer
      }

      if (b != 44 && b != 35){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(wifi, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 45:
  for (uint8_t b = 0; b < 45; b++) {
    if (pressed && keyPASSWORD[b].contains(t_x, t_y)) {
      keyPASSWORD[b].press(true);  // tell the button it is pressed
    } else {
      keyPASSWORD[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 45; b++) {

    if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (keyPASSWORD[b].justReleased()) keyPASSWORD[b].drawButton();     // draw normal

    if (keyPASSWORD[b].justPressed()) {
      keyPASSWORD[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the 
      if (b != 17 && b != 26 && b != 35 && b != 44) {
        if (indexPASSWORD < NUM_LEN_19) {
          password[indexPASSWORD] = keyboardFullLabel[b][0];
          indexPASSWORD++;
          password[indexPASSWORD] = 0; // zero terminate
        }
        status("", 0); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 26) { //DELETE
        password[indexPASSWORD] = 0;
        Serial.println("Delete Pressed");
        if (indexPASSWORD > 0) {
          indexPASSWORD--;
          password[indexPASSWORD] = 0;//' ';
        }
        status("", 0); // Clear the old status
      }

      if (b == 35){ //BACK
        displayState = 44;
        drawKeyboardWIFI();
      }

      if (b == 44) { //SEND
        status("Sent value to serial port", TFT_CYAN);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.fillScreen(TFT_CYAN);
        tft.setCursor(tft.width()/2-130, tft.height()/2);
        tft.setTextSize(2);
        tft.setTextColor(TFT_BLACK);
        tft.print("WIFI SAVED");
        passwordd = String(password);
        EEPROM.put(270,passwordd);
        EEPROM.commit();
        delay(2000);
        ESP.restart();

        break;
      }
      
      if (b == 17) { //NEW
        status("Value cleared", TFT_CYAN);
        indexPASSWORD = 0; // Reset index to 0
        password[indexPASSWORD] = 0; // Place null in buffer
      }

      if (b != 44 && b != 35){
      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour
      tft.setTextSize(1);

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(password, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
      }
    }
  }
  break;

  case 46:
  // wifi_deinit();
  // WiFi.mode(WIFI_OFF);
  delay(1000);
  tft.fillScreen(TFT_CYAN);
  tft.setTextSize(2);
  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifii, passwordd);
  tft.setTextSize(1);

  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    tft.setCursor(tft.width()/2 - 140, tft.height()/2);
    tft.setTextColor(TFT_RED);
    tft.print("Please Turn On Wi-Fi");
    // Serial.print("WiFi: "); Serial.println(wifii);
    // Serial.print("PASS: "); Serial.println(passwordd);
    // WiFi.begin(wifi, password);
  }
  Serial.println("CONNECTED!!!!");
  apa = "HAIii HAII";
  sendSpreadsheets(pesanCloud);
  // sendSpreadsheets("info1=" + urlencode(apa));
  delay(1000);
  // displayState = 10;
  break;

  case 47:
  // wifi_deinit();
  // WiFi.mode(WIFI_OFF);
  delay(1000);
  EEPROM.get(250,wifii);
  EEPROM.get(270,passwordd);
  tft.fillScreen(TFT_CYAN);
  tft.setTextSize(2);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifii, passwordd);
  tft.setTextSize(1);

  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    tft.setCursor(tft.width()/2 - 140, tft.height()/2);
    tft.setTextColor(TFT_RED);
    tft.print("Please Turn On Wi-Fi");
    // Serial.print("WiFi: "); Serial.println(wifi);
    // Serial.print("PASS: "); Serial.println(password);
  }
  Serial.println("CONNECTED!!!!");
  // String apa = "HAIii HAII";
  sendSpreadsheets2(pesanCloud);
  // sendSpreadsheets("info1=" + urlencode(apa));
  delay(1000);
  // displayState = 40;
  // initButtonPRINT();
  break;

  }
}
//-------------END of Display State------------
//-----------------END of LOOP-----------------

//-------------Calculate Variable--------------
void cekMicroSD(){
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    tft.setCursor(110,tft.height()/2-120);
    tft.print("No Micro SD Card");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }
  
  // menghitung ukuran micro sd    
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void sendPrinter(){
  String str1 = "      POLKESYO KIDS CARE\n";
  String str2 = "         " + String(todaysDate) + "\n\n";
  String stripe = "--------------------------------\n\n";
  String space = "\n";
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < str1.length(); i++) {
    SerialBT.write(str1[i]);
  }
  for (int i = 0; i < str2.length(); i++) {
    printer.write(str2[i]);
  }
  for (int i = 0; i < stripe.length(); i++) {
    SerialBT.write(stripe[i]);
  }
  for (int i = 0; i < pesanPrinter.length(); i++) {
    SerialBT.write(pesanPrinter[i]);
  }
  for (int i = 0; i < stripe.length(); i++) {
    SerialBT.write(stripe[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  delay(3000);
  ESP.restart();
}

void sendPrinter2(){
  String str1 = "      POLKESYO KIDS CARE\n";
  String str2 = "         " + String(todaysDate) + "\n\n"; //todaysDateStr 
  String stripe = "--------------------------------\n\n";
  String space = "\n";
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < str1.length(); i++) {
    SerialBT.write(str1[i]);
  }
  for (int i = 0; i < str2.length(); i++) {
    printer.write(str2[i]);
  }
  for (int i = 0; i < stripe.length(); i++) {
    SerialBT.write(stripe[i]);
  }
  for (int i = 0; i < pesanPrinter.length(); i++) {
    SerialBT.write(pesanPrinter[i]);
  }
  for (int i = 0; i < stripe.length(); i++) {
    SerialBT.write(stripe[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  for (int i = 0; i < space.length(); i++) {
    SerialBT.write(space[i]);
  }
  delay(3000);
  ESP.restart();
}

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

  // Serial.print("Average: "); Serial.println(average);
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

  // Serial.print("Average: "); Serial.println(average1);
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
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Offset Value");
  tft.setCursor(280, 17);
  tft.setTextColor(TFT_BLACK);
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

      keyCalibrate[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor3[b], TFT_WHITE,
                        keyLabel3[b], KEY_TEXTSIZE);
      keyCalibrate[b].drawButton();
    }
  }
}

void drawKeypadBIRTH(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(47, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Birth Date");
  tft.setCursor(242, 17);
  tft.setTextColor(TFT_BLACK);
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

      keyBIRTH[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel1[b], KEY_TEXTSIZE);
      keyBIRTH[b].drawButton();
    }
  }
}
void drawKeypadDATE(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(47, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Today's Date");
  tft.setCursor(242, 17);
  tft.setTextColor(TFT_BLACK);
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

      keyDATE[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel1[b], KEY_TEXTSIZE);
      keyDATE[b].drawButton();
    }
  }
}
void drawKeyboardNAME(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(35, 30);
  tft.print("Toddler's Name");
  tft.setCursor(225, 17);
  tft.setTextColor(TFT_BLACK);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(2);
  tft.print("(ex: ANANDA AZRIEL)");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyNAME[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyNAME[b].drawButton();
    }
  }
}

void drawKeyboardORTU(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(180, 30);
  tft.print("Nama Ortu");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyORTU[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyORTU[b].drawButton();
    }
  }
}

void drawKeyboardKELURAHAN(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(180, 30);
  tft.print("Kelurahan");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyKELURAHAN[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyKELURAHAN[b].drawButton();
    }
  }
}

void drawKeyboardPKM(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(200, 30);
  tft.print("PKM");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyPKM[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyPKM[b].drawButton();
    }
  }
}

void drawKeyboardPOSYANDU(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(135, 30);
  tft.print("Nama Posyandu");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyPOSYANDU[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyPOSYANDU[b].drawButton();
    }
  }
}

void drawKeyboardADDRESS(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(125, 30);
  tft.print("Alamat Posyandu");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyADDRESS[b].initButton(&tft, KEY_X2 + col * (KEY_W2 + KEY_SPACING_X2),
                        KEY_Y2 + row * (KEY_H2 + KEY_SPACING_Y2), // x, y, w, h, outline, fill, text
                        KEY_W2, KEY_H2, TFT_WHITE, keyboardFullColor[b], TFT_WHITE,
                        keyboardFullLabel[b], KEY_TEXTSIZE1);
      keyADDRESS[b].drawButton();
    }
  }
}

void drawKeyboardWIFI(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(170, 30);
  tft.print("WiFi Name");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyWIFI[b].initButton(&tft, KEY_X2 + col * (KEY_W2 + KEY_SPACING_X2),
                        KEY_Y2 + row * (KEY_H2 + KEY_SPACING_Y2), // x, y, w, h, outline, fill, text
                        KEY_W2, KEY_H2, TFT_WHITE, keyboardFullColor[b], TFT_WHITE,
                        keyboardFullLabel[b], KEY_TEXTSIZE1);
      keyWIFI[b].drawButton();
    }
  }
}

void drawKeyboardPASSWORD(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(140, 30);
  tft.print("WIFI PASSWORD");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b == 17 || b == 26 || b == 35 || b == 44) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyPASSWORD[b].initButton(&tft, KEY_X2 + col * (KEY_W2 + KEY_SPACING_X2),
                        KEY_Y2 + row * (KEY_H2 + KEY_SPACING_Y2), // x, y, w, h, outline, fill, text
                        KEY_W2, KEY_H2, TFT_WHITE, keyboardFullColor[b], TFT_WHITE,
                        keyboardFullLabel[b], KEY_TEXTSIZE1);
      keyPASSWORD[b].drawButton();
    }
  }
}

void drawKeyboardSumberPMT(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  // tft.setCursor(150, 30);
  // tft.setCursor(tft.width()/2-10*12, tft.height()/2-20);
  tft.print("Sumber PMT");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keySumberPMT[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keySumberPMT[b].drawButton();
    }
  }
}

void drawKeyboardBeriPUSAT(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(130, 30);
  tft.print("Pemberian Pusat");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyBeriPUSAT[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyBeriPUSAT[b].drawButton();
    }
  }
}

void drawKeyboardBeriDAERAH(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setCursor(130, 30);
  tft.print("Pemberian Daerah");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 6; col++) {
      uint8_t b = col + row * 6;

      if (b > 26 || b == 23) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyBeriDAERAH[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1), // x, y, w, h, outline, fill, text
                        KEY_W1, KEY_H1, TFT_WHITE, keyboardColor[b], TFT_WHITE,
                        keyboardLabel[b], KEY_TEXTSIZE1);
      keyBeriDAERAH[b].drawButton();
    }
  }
}

void drawKeypadNikORTU(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(180, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("NIK Ortu");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyNikORTU[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyNikORTU[b].drawButton();
    }
  }
}

void drawKeypadPhone(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(135, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Nomor HP Ortu");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyPhone[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyPhone[b].drawButton();
    }
  }
}

void drawKeypadRT(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(205, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("RT");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyRT[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyRT[b].drawButton();
    }
  }
}

void drawKeypadRW(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(205, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("RW");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyRW[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyRW[b].drawButton();
    }
  }
}

void drawKeypadLILA(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(200, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("LILA");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyLILA[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyLILA[b].drawButton();
    }
  }
}

void drawKeypadVITA(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(200, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("VITA");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyVITA[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyVITA[b].drawButton();
    }
  }
}

void drawKeypadLIKE(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(150, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Lingkar Kepala");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyLIKE[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyLIKE[b].drawButton();
    }
  }
}

void drawKeypadBeriKE(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(150, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Pemberian Ke-");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyBeriKE[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyBeriKE[b].drawButton();
    }
  }
}

void drawKeypadTAHUN(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(140, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Tahun Produksi");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyTAHUN[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyTAHUN[b].drawButton();
    }
  }
}

void drawKeypadWEIGHT(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  tft.setCursor(28, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("Enter Body Weight in kg");
  tft.setCursor(318, 17);
  tft.setTextColor(TFT_BLACK);
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

      keyWEIGHT[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyWEIGHT[b].drawButton();
    }
  }
}

void drawKeypadNIK(){
  tft.fillRect(0, 0, 480, 320, TFT_CYAN);
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  
  tft.setCursor(170, 30);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.print("NIK Balita");

  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b < 3 || b == 14) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      keyNIK[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      keyNIK[b].drawButton();
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
    tft.setTextColor(TFT_BLUE, color);
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

void sendSpreadsheets(String params) {
  HTTPClient http;
  String urlSpreadsheets="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
  http.begin(urlSpreadsheets);
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(120, tft.height()/2-20);
  tft.setTextColor(TFT_BLACK);
  tft.print("Sending Data to Cloud...");

  int httpCode1 = http.GET();  
  http.end();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(1000); 
  tft.setCursor(190, tft.height()/2+20); tft.print("Data Sent"); delay(2000); 
  
  delay(1000);
  displayState = 10;
  // initButtonFILL();
}

void sendSpreadsheets2(String params) {
  HTTPClient http;
  String urlSpreadsheets="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
  http.begin(urlSpreadsheets);
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(120, tft.height()/2-20);
  tft.setTextColor(TFT_BLACK);
  tft.print("Sending Data to Cloud...");

  int httpCode2 = http.GET();  
  http.end();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(1000); 
  tft.setCursor(190, tft.height()/2+20); tft.print("Data Sent"); delay(2000); 
  
  delay(1000);
  displayState = 40;
  initButtonPRINT();
}

String urlencode(String &str) {
  String tempStr = str;
  tempStr.replace(" ", "%20");
  tempStr.replace("&", "%26");
  tempStr.replace("#", "%23");
  tempStr.replace("=", "%3D");
  tempStr.replace("+", "%2B");
  tempStr.replace(",", "%2C");
  tempStr.replace("/", "%2F");
  tempStr.replace("?", "%3F");
  tempStr.replace(":", "%3A");
  tempStr.replace(";", "%3B");
  tempStr.replace("<", "%3C");
  tempStr.replace(">", "%3E");
  tempStr.replace("@", "%40");
  tempStr.replace("[", "%5B");
  tempStr.replace("]", "%5D");
  tempStr.replace("\\", "%5C");
  tempStr.replace("^", "%5E");
  tempStr.replace("_", "%5F");
  tempStr.replace("`", "%60");
  tempStr.replace("{", "%7B");
  tempStr.replace("|", "%7C");
  tempStr.replace("}", "%7D");
  tempStr.replace("~", "%7E");
  return tempStr;
}

float getFakeTemperature() {
return micros()%20;
}

String fakeFunc1(String dataa)
{
  // String dataa = "HAIIII IIII";
  urlencode(dataa);
  return dataa;
}
float fakeFunc2()
{
  return millis()%100;
}