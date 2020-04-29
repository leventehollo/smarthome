
#ifndef UI_h
#define UI_h

const int ST_HIDE = 0;
const int ST_HIDDEN = 1;
const int ST_SHOW = 2;
const int ST_VISIBLE = 3;
const int ST_REFRESH = 4;


int uiStates[] = {};
const int UI_MESSAGE = 100;
const int UI_SEPARATORS = 101;
const int UI_CHANNELS = 102;
const int UI_ACTIVE_CHANNEL = 103;
const int UI_CURRENT_TIME = 104;
const int UI_ELAPSED_TIME = 105;
const int UI_REMAINING_TIME = 106;

void initUiStates() {
  uiStates[UI_MESSAGE] = ST_REFRESH;
  uiStates[UI_SEPARATORS] = ST_REFRESH;
  uiStates[UI_CHANNELS] = ST_REFRESH;
  uiStates[UI_ACTIVE_CHANNEL] = ST_REFRESH;
  uiStates[UI_CURRENT_TIME] = ST_REFRESH;
  uiStates[UI_ELAPSED_TIME] = ST_HIDDEN;
  uiStates[UI_REMAINING_TIME] = ST_HIDDEN;
}

String uiValues[] = {};

void initUiValues() {
  uiValues[UI_MESSAGE] = "Booting";
  uiValues[UI_ACTIVE_CHANNEL] = "00";
  uiValues[UI_CURRENT_TIME] = "----";
  uiValues[UI_ELAPSED_TIME] = "----";
  uiValues[UI_REMAINING_TIME] = "----";
}


// *****************************
// ********* Symbols ***********
// *****************************

byte DIVIDER_RIGHT[8] = {
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110
};

uint8_t DOWN_ARROW[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

uint8_t UP_ARROW[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

uint8_t RIGHT_ARROW[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};




#endif
