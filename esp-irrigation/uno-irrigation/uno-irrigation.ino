// Target device:  Arduino UNO, LCD Keypad Shield


const String VERSION = "1.0";

#include "MyDevice.h"
#include "IrrigationSerialProtocol.h"

#include <LiquidCrystal.h>
#include "UI.h"

const int pin_RS = 8; const int pin_EN = 9; const int pin_d4 = 4; const int pin_d5 = 5; const int pin_d6 = 6; const int pin_d7 = 7; const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

#define btnNONE   0
#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5

const int DIVIDER_POS = 0;

const char* CHR_SPACE = " ";
const char* CHR_SEPARATOR = "|";

#include <SoftwareSerial.h>
SoftwareSerial mySerial(RX, TX); // RX, TX


#include <SimpleTimer.h>
SimpleTimer serialTimer;
SimpleTimer displayTimer;

void setup() {
  Serial.begin(SERIAL_DATA_RATE);

  delay(10);
  while (!Serial) {}

  mySerial.begin(SERIAL_DATA_RATE);
  serialTimer.setInterval(500, readSerial);

  log("Initializing LCD...");
  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.clear();

  // Custom caracters
  lcd.createChar(DIVIDER_POS, DIVIDER_RIGHT);  
  initUiStates();
  initUiValues();
  log("LCD initialized.");
  displayTimer.setInterval(2000, refreshDisplay);
}


void loop() {
  serialTimer.run();
  displayTimer.run();
}


bool validatePayload(String payload) {
  log("validatig payload...:"+payload+" - "+payload.length());
  if (payload.length() != PAYLOAD_LENGTH)
    return false;
  log("Payload length is ok");

  int from = 0;
  int found = 0;
  while (true) {
    int separatorIndex = payload.indexOf(SEPARATOR, from + 1);
    if (separatorIndex > 0) {
      from = separatorIndex;
      found++;
    }
    else {
      break;
    }
  }
    log("found:"+String(found));

  return found + 1 == NUMBER_OF_SEGMENTS;
}


void handlePayload(String payload) {
  log("Valid Payload received");
  
  uiValues[UI_MESSAGE] = getState(payload);
  setState(UI_MESSAGE, ST_REFRESH);
  
  uiValues[UI_ACTIVE_CHANNEL] = getChannel(payload);
  setState(UI_ACTIVE_CHANNEL, ST_REFRESH);
  
  uiValues[UI_CURRENT_TIME] = getCurrentTime(payload);
  setState(UI_CURRENT_TIME, ST_REFRESH);
  
  uiValues[UI_ELAPSED_TIME] = getElapsedTime(payload);
  setState(UI_ELAPSED_TIME, ST_REFRESH);
  
  uiValues[UI_REMAINING_TIME] = getRemainingTime(payload);
  setState(UI_REMAINING_TIME, ST_REFRESH);
}

int getState(String payload) {
  return payload.substring(2, 4).toInt();
}

String getChannel(String payload) {
  return payload.substring(7, 9);
}

int channelToInt(String chStr) {
  if (chStr.charAt(0) == "0")
    return chStr.substring(1, 2).toInt();
  else return chStr.toInt();
}

String getCurrentTime(String payload) {
  return payload.substring(12, 16);
}

String getElapsedTime(String payload) {
  return payload.substring(19, 23);
}

String getRemainingTime(String payload) {
  return payload.substring(26, 30);
}


void refreshDisplay() {
  drawChannels();
  drawSeparators();
  drawCurrentTime();
  drawActiveChannel();  //must be the last to see the cursor on the right place
}


// For V1.0 comment the other threshold and use the one below
int readButtons() {
  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnNONE;
}

int getState(int ST) {
  return uiStates[ST];
}

int setState(int ST, int targetState) {
  switch (uiStates[ST]) {
    case ST_SHOW:
      if (targetState == ST_VISIBLE) uiStates[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From ST_SHOW to " + stateToStr(targetState));
      break;
    case ST_VISIBLE:
      if (targetState == ST_REFRESH || targetState == ST_HIDE) uiStates[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + " : " + "From ST_VISIBLE to " + stateToStr(targetState));
      break;
    case ST_HIDE:
      if (targetState == ST_HIDDEN) uiStates[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From ST_HIDE to " + stateToStr(targetState));
      break;
    case ST_HIDDEN:
      if (targetState == ST_SHOW) uiStates[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + " : " + "From ST_HIDDEN to " + stateToStr(targetState));
      break;
    case ST_REFRESH:
      if (targetState == ST_VISIBLE) uiStates[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From ST_REFRESH to " + stateToStr(targetState));
      break;
  }
}

String stateToStr(int state) {
  if (state == ST_SHOW) return "ST_SHOW";
  if (state == ST_VISIBLE) return "ST_VISIBLE";
  if (state == ST_REFRESH) return "ST_REFRESH";
  if (state == ST_HIDE) return "ST_HIDE";
  if (state == ST_HIDDEN) return "ST_HIDDEN";
}

void drawSeparators() {
  if (getState(UI_SEPARATORS) == ST_SHOW || getState(UI_SEPARATORS) == ST_REFRESH) {
    lcd.setCursor(10, 0);
    lcd.write(DIVIDER_POS);
    lcd.setCursor(10, 1);
    lcd.write(DIVIDER_POS);
    setState(UI_SEPARATORS, ST_VISIBLE);
  }
  else if (getState(UI_SEPARATORS) == ST_HIDE) {
    lcd.setCursor(10, 0);
    lcd.print(CHR_SPACE);
    lcd.setCursor(10, 1);
    lcd.print(CHR_SPACE);
    setState(UI_SEPARATORS, ST_HIDDEN);
  }
}

void drawChannels() {
  if (getState(UI_CHANNELS) == ST_SHOW || getState(UI_CHANNELS) == ST_REFRESH) {
    lcd.setCursor(0, 0);
    lcd.print("-");
    for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
      lcd.print(i + 1);
    }
    setState(UI_CHANNELS, ST_VISIBLE);
  }
  else if (getState(UI_CHANNELS) == ST_HIDE) {
    lcd.setCursor(0, 0);
    for (int i = 0; i <= NUMBER_OF_CHANNELS; i++) {
      lcd.print(CHR_SPACE);
    }
    setState(UI_CHANNELS, ST_HIDDEN);
  }
}

void drawActiveChannel() {
  if (getState(UI_ACTIVE_CHANNEL) == ST_SHOW || getState(UI_ACTIVE_CHANNEL) == ST_REFRESH) {
    int activeChannel = channelToInt(uiValues[UI_ACTIVE_CHANNEL]);
    lcd.setCursor(activeChannel, 0);
    lcd.cursor();
    //setState(UI_ACTIVE_CHANNEL, ST_VISIBLE);
  }
  else if (getState(UI_ACTIVE_CHANNEL) == ST_HIDE || getState(UI_ACTIVE_CHANNEL) == ST_REFRESH) {
    lcd.setCursor(0, 0);
    lcd.noCursor();
    setState(UI_ACTIVE_CHANNEL, ST_HIDDEN);
  }
}

String formatTime(String unformattedTime) {
  return unformattedTime.substring(0, 2) + ":" + unformattedTime.substring(2, 4);
}

void drawCurrentTime() {
  if (getState(UI_CURRENT_TIME) == ST_SHOW || getState(UI_CURRENT_TIME) == ST_REFRESH) {
    String currentTime = formatTime(uiValues[UI_CURRENT_TIME]);
    lcd.setCursor(11, 0);
    lcd.print(currentTime);
    setState(UI_CURRENT_TIME, ST_VISIBLE);
  }
  else if (getState(UI_CURRENT_TIME) == ST_HIDE || getState(UI_CURRENT_TIME) == ST_REFRESH) {
    lcd.setCursor(12, 0);
    lcd.print("     ");
    setState(UI_CURRENT_TIME, ST_HIDDEN);
  }
}

void drawMessage() {
  if (getState(UI_MESSAGE) == ST_SHOW || getState(UI_MESSAGE) == ST_REFRESH) {
    String message = uiValues[UI_MESSAGE];
    lcd.setCursor(1, 1);
    lcd.print(message);
    setState(UI_MESSAGE, ST_VISIBLE);
  }
  else if (getState(UI_MESSAGE) == ST_HIDE || getState(UI_MESSAGE) == ST_REFRESH) {
    lcd.setCursor(1, 1);
    lcd.print("         ");
    setState(UI_MESSAGE, ST_HIDDEN);
  }
}


void readSerial() {
  String payload = "";
  char character;
  log("Read serial");
  while (mySerial.available()) {
    character = mySerial.read();
    payload.concat(character);
  }
  if (payload != "") {
    log(payload);
    if (validatePayload(payload)) {     // valid message received
      handlePayload(payload);
    }
  }
}


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

boolean isNumber(String str) {
  for (byte i = 0; i < str.length(); i++)
  {
    if (isDigit(str.charAt(i))) return true;
  }
  return false;
}

void log(String msg) {
  Serial.println(msg);
  mySerial.println(msg);
}


void log(String msg, LiquidCrystal lcd) {
  log(msg);
  lcd.setCursor(0, 1);
  lcd.print(msg);
}
