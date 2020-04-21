// Target device:  Arduino UNO, LCD Keypad Shield


const String VERSION = "1.0";

#include <LiquidCrystal.h>
#include "LcdChars.h"

#include <ArduinoJson.h>
const size_t JSON_DOC_CAPACITY = JSON_OBJECT_SIZE(1);


const uint32_t SERIAL_DATA_RATE = 115200;
const int RX = 2;
const int TX = 3;

const int NUMBER_OF_CHANNELS = 9;


//LCD pin to Arduino
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



int activeChannel = 0;

const int IN_HIDE = 0;
const int IN_HIDDEN = 1;
const int IN_SHOW = 2;
const int IN_VISIBLE = 3;
const int IN_REFRESH = 4;


int state[] = {};
const int ST_MENU = 100;
const int ST_SEPARATORS = 101;
const int ST_CHANNELS = 102;
const int ST_ACTIVE_CHANNEL = 103;
const int ST_CURRENT_TIME = 104;
const int ST_ELAPSED_TIME = 105;
const int ST_REMAINING_TIME = 106;



#include <SoftwareSerial.h>
SoftwareSerial mySerial(RX, TX); // RX, TX


#include <SimpleTimer.h>
SimpleTimer serialTimer;
SimpleTimer displayTimer;

String menuItems[] = {"ITEM 1", "ITEM 2", "ITEM 3", "ITEM 4"};
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;

void initStates() {
  state[ST_MENU] = IN_REFRESH;
  state[ST_SEPARATORS] = IN_REFRESH;
  state[ST_CHANNELS] = IN_REFRESH;
  state[ST_ACTIVE_CHANNEL] = IN_REFRESH;
  state[ST_CURRENT_TIME] = IN_REFRESH;
  state[ST_ELAPSED_TIME] = IN_HIDDEN;
  state[ST_REMAINING_TIME] = IN_HIDDEN;

}

void setup() {
  Serial.begin(SERIAL_DATA_RATE);

  delay(10);
  while (!Serial) {}

  mySerial.begin(SERIAL_DATA_RATE);
  serialTimer.setInterval(500, Sent_serial);

  log("Initializing LCD...");
  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.clear();

  // Custom caracters
  lcd.createChar(DIVIDER_POS, DIVIDER_RIGHT);

  displayTimer.setInterval(1000, refreshDisplay);
  initStates();
  log("LCD initialized...");
}

void loop() {
  //serialTimer.run();
  displayTimer.run();
}

void refreshDisplay() {
  drawChannels();
  drawSeparators();
  drawCurrentTime();
  drawActiveChannel();  //must be the last
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
  return state[ST];
}

int setState(int ST, int targetState) {
  switch (state[ST]) {
    case IN_SHOW:
      if (targetState == IN_VISIBLE) state[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From IN_SHOW to " + stateToStr(targetState));
      break;
    case IN_VISIBLE:
      if (targetState == IN_REFRESH || targetState == IN_HIDE) state[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + " : " + "From IN_VISIBLE to " + stateToStr(targetState));
      break;
    case IN_HIDE:
      if (targetState == IN_HIDDEN) state[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From IN_HIDE to " + stateToStr(targetState));
      break;
    case IN_HIDDEN:
      if (targetState == IN_SHOW) state[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + " : " + "From IN_HIDDEN to " + stateToStr(targetState));
      break;
    case IN_REFRESH:
      if (targetState == IN_VISIBLE) state[ST] = targetState;
      else log("Invalid state transition fired for " + String(ST) + ": " + "From IN_REFRESH to " + stateToStr(targetState));
      break;
  }
}

String stateToStr(int state) {
  if (state == IN_SHOW) return "IN_SHOW";
  if (state == IN_VISIBLE) return "IN_VISIBLE";
  if (state == IN_REFRESH) return "IN_REFRESH";
  if (state == IN_HIDE) return "IN_HIDE";
  if (state == IN_HIDDEN) return "IN_HIDDEN";
}

void drawSeparators() {
  if (getState(ST_SEPARATORS) == IN_SHOW || getState(ST_SEPARATORS) == IN_REFRESH) {
    lcd.setCursor(10, 0);
    lcd.write(DIVIDER_POS);
    lcd.setCursor(10, 1);
    lcd.write(DIVIDER_POS);
    setState(ST_SEPARATORS, IN_VISIBLE);
  }
  else if (getState(ST_SEPARATORS) == IN_HIDE) {
    lcd.setCursor(10, 0);
    lcd.print(CHR_SPACE);
    lcd.setCursor(10, 1);
    lcd.print(CHR_SPACE);
    setState(ST_SEPARATORS, IN_HIDDEN);
  }
}

void drawChannels() {
  if (getState(ST_CHANNELS) == IN_SHOW || getState(ST_CHANNELS) == IN_REFRESH) {
    lcd.setCursor(0, 0);
    lcd.print("-");
    for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
      lcd.print(i + 1);
    }
    setState(ST_CHANNELS, IN_VISIBLE);
  }
  else if (getState(ST_CHANNELS) == IN_HIDE) {
    lcd.setCursor(0, 0);
    for (int i = 0; i <= NUMBER_OF_CHANNELS; i++) {
      lcd.print(CHR_SPACE);
    }
    setState(ST_CHANNELS, IN_HIDDEN);
  }
}

void drawActiveChannel() {
  if (getState(ST_ACTIVE_CHANNEL) == IN_SHOW || getState(ST_ACTIVE_CHANNEL) == IN_REFRESH) {
    lcd.setCursor(activeChannel, 0);
    lcd.cursor();
    setState(ST_ACTIVE_CHANNEL, IN_VISIBLE);
  }
  else if (getState(ST_ACTIVE_CHANNEL) == IN_HIDE || getState(ST_ACTIVE_CHANNEL) == IN_REFRESH) {
    lcd.setCursor(activeChannel, 0);
    lcd.noCursor();
    setState(ST_ACTIVE_CHANNEL, IN_HIDDEN);
  }
}

void drawCurrentTime() {
  if (getState(ST_CURRENT_TIME) == IN_SHOW || getState(ST_CURRENT_TIME) == IN_REFRESH) {
    lcd.setCursor(11, 0);
    lcd.print("01:23");
    setState(ST_CURRENT_TIME, IN_VISIBLE);
  }
  else if (getState(ST_CURRENT_TIME) == IN_HIDE || getState(ST_CURRENT_TIME) == IN_REFRESH) {
    lcd.setCursor(12, 0);
    lcd.print("     ");
    setState(ST_CURRENT_TIME, IN_HIDDEN);
  }
}

// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void mainMenuDraw() {
  log(menuPage);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);

  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}


// When called, this function will erase the current cursor and redraw it based on the cursorPosition and menuPage variables.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    else {                          // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    else {                          // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}

void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button = readButtons();
    /*if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
      }*/
    switch (button) {
      case btnNONE: // When button returns as 0 there is no action taken
        break;
      case btnRIGHT:  // This case will execute if the "forward" button is pressed
        button = btnNONE;
        switch (cursorPosition) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case btnUP:
        button = btnNONE;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case btnDOWN:
        button = btnNONE;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}

void menuItem1() { // Function executes when you select the 1st item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 1");

  while (activeButton == 0) {
    int button = readButtons();
    switch (button) {
      case btnLEFT:  // This case will execute if the "back" button is pressed
        button = btnNONE;
        activeButton = 1;
        break;
    }
  }
}

void menuItem2() { // Function executes when you select the 2nd item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 2");

  while (activeButton == 0) {
    int button = readButtons();
    switch (button) {
      case btnLEFT:  // This case will execute if the "back" button is pressed
        button = btnNONE;
        activeButton = 1;
        break;
    }
  }
}

void menuItem3() { // Function executes when you select the 3rd item from main menu
  int activeButton = btnNONE;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sub Menu 3");

  while (activeButton == btnNONE) {
    int button = readButtons();
    switch (button) {
      case btnLEFT:  // This case will execute if the "back" button is pressed
        button = btnNONE;
        activeButton = 1;
        break;
    }
  }
}


String messageJson(String msg) {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.as<JsonObject>();
  object["message"] = msg;
  String response;
  serializeJsonPretty(doc, response);
  return response;
}


void Sent_serial() {
  String content = "";  //null string constant ( an empty string )
  char character;
  while (mySerial.available()) {
    character = mySerial.read();
    content.concat(character);
  }
  if (content != "") {
    log(content);
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
