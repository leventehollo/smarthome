
#ifndef LcdChars_h
#define LcdChars_h

// *****************************
// ************ 0 **************
// *****************************

// Number 0, black on white
byte NUM_0[8] = {
  B00110,
  B01001,
  B01001,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000
};

// Number 0, inverted (white on black)
byte NUM_0_INV[8] = {
  B11001,
  B10110,
  B10110,
  B10110,
  B10110,
  B11001,
  B11111,
  B11111
};

// Number 0, black on white, with cursor (underlined)
byte NUM_0_CUR[8] = {
  B00110,
  B01001,
  B01001,
  B01001,
  B01001,
  B00110,
  B00000,
  B11111
};

// Number 0, inverted (white on black), with cursor (underlined)
byte NUM_0_CUR_INV[8] = {
  B11001,
  B10110,
  B10110,
  B10110,
  B10110,
  B11001,
  B11111,
  B00000
};




// *****************************
// ************ 1 **************
// *****************************


// Number 1, black on white
byte NUM_1[8] = {
  B00010,
  B00110,
  B00010,
  B00010,
  B00010,
  B00111,
  B00000,
  B00000
};

// Number 1, inverted (white on black)
byte NUM_1_INV[8] = {
  B11101,
  B11001,
  B11101,
  B11101,
  B11101,
  B11000,
  B11111,
  B11111
};

// Number 1, black on white, with cursor (underlined)
byte NUM_1_CUR[8] = {
  B00010,
  B00110,
  B00010,
  B00010,
  B00010,
  B00111,
  B00000,
  B11111
};

// Number 1, inverted (white on black), with cursor (underlined)
byte NUM_1_CUR_INV[8] = {
  B11101,
  B11001,
  B11101,
  B11101,
  B11101,
  B11000,
  B11111,
  B00000
};


// *****************************
// ************ 2 **************
// *****************************

// Number 2, black on white
byte NUM_2[8] = {
  B00110,
  B01001,
  B00001,
  B00010,
  B00100,
  B01111,
  B00000,
  B00000
};

// Number 2, inverted (white on black)
byte NUM_2_INV[8] = {
  B11001,
  B10110,
  B11110,
  B11101,
  B11011,
  B10000,
  B11111,
  B11111
};

// Number 2, black on white, with cursor (underlined)
byte NUM_2_CUR[8] = {
  B00110,
  B01001,
  B00001,
  B00010,
  B00100,
  B01111,
  B00000,
  B11111
};

// Number 2, inverted (white on black), with cursor (underlined)
byte NUM_2_CUR_INV[8] = {
  B11001,
  B10110,
  B11110,
  B11101,
  B11011,
  B10000,
  B11111,
  B00000
};


// *****************************
// ************ 3 **************
// *****************************

// Number 3, black on white
byte NUM_3[8] = {
  B00111,
  B00001,
  B00010,
  B00010,
  B00001,
  B00111,
  B00000,
  B00000
};

// Number 3, inverted (white on black)
byte NUM_3_INV[8] = {
  B11000,
  B11110,
  B11101,
  B11101,
  B11110,
  B11000,
  B11111,
  B11111
};

// Number 3, black on white, with cursor (underlined)
byte NUM_3_CUR[8] = {
  B00111,
  B00001,
  B00010,
  B00010,
  B00001,
  B00111,
  B00000,
  B11111
};

// Number 3, inverted (white on black), with cursor (underlined)
byte NUM_3_CUR_INV[8] = {
  B11000,
  B11110,
  B11101,
  B11101,
  B11110,
  B11000,
  B11111,
  B00000
};


// *****************************
// ************ 4 **************
// *****************************


// Number 4, black on white
byte NUM_4[8] = {
  B00010,
  B00110,
  B01010,
  B01111,
  B00010,
  B00010,
  B00000,
  B00000
};

// Number 4, inverted (white on black)
byte NUM_4_INV[8] = {
  B11101,
  B11001,
  B10101,
  B10000,
  B11101,
  B11101,
  B11111,
  B11111
};

// Number 4, black on white, with cursor (underlined)
byte NUM_4_CUR[8] = {
  B00010,
  B00110,
  B01010,
  B01111,
  B00010,
  B00010,
  B00000,
  B11111
};

// Number 4, inverted (white on black), with cursor (underlined)
byte NUM_4_CUR_INV[8] = {
  B11101,
  B11001,
  B10101,
  B10000,
  B11101,
  B11101,
  B11111,
  B00000
};


// *****************************
// ************ 5 **************
// *****************************

// Number 5, black on white
byte NUM_5[8] = {
  B01111,
  B01000,
  B01110,
  B00001,
  B01001,
  B00110,
  B00000,
  B00000
};

// Number 5, inverted (white on black)
byte NUM_5_INV[8] = {
  B10000,
  B10111,
  B10001,
  B11110,
  B10110,
  B11001,
  B11111,
  B11111
};

// Number 5, black on white, with cursor (underlined)
byte NUM_5_CUR[8] = {
  B01111,
  B01000,
  B01110,
  B00001,
  B01001,
  B00110,
  B00000,
  B11111
};

// Number 5, inverted (white on black), with cursor (underlined)
byte NUM_5_CUR_INV[8] = {
  B10000,
  B10111,
  B10001,
  B11110,
  B10110,
  B11001,
  B11111,
  B00000
};


// *****************************
// ************ 6 **************
// *****************************

// Number 6, black on white
byte NUM_6[8] = {
  B00111,
  B01000,
  B01110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000
};

// Number 6, inverted (white on black)
byte NUM_6_INV[8] = {
  B11000,
  B10111,
  B10001,
  B10110,
  B10110,
  B11001,
  B11111,
  B11111
};

// Number 6, black on white, with cursor (underlined)
byte NUM_6_CUR[8] = {
  B00111,
  B01000,
  B01110,
  B01001,
  B01001,
  B00110,
  B00000,
  B11111
};

// Number 6, inverted (white on black), with cursor (underlined)
byte NUM_6_CUR_INV[8] = {
  B11000,
  B10111,
  B10001,
  B10110,
  B10110,
  B11001,
  B11111,
  B00000
};


// *****************************
// ************ 7 **************
// *****************************

// Number 7, black on white
byte NUM_7[8] = {
  B01111,
  B00001,
  B00010,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000
};

// Number 7, inverted (white on black)
byte NUM_7_INV[8] = {
  B10000,
  B11110,
  B11101,
  B11011,
  B11011,
  B11011,
  B11111,
  B11111
};

// Number 7, black on white, with cursor (underlined)
byte NUM_7_CUR[8] = {
  B01111,
  B00001,
  B00010,
  B00100,
  B00100,
  B00100,
  B00000,
  B11111
};

// Number 7, inverted (white on black), with cursor (underlined)
byte NUM_7_CUR_INV[8] = {
  B10000,
  B11110,
  B11101,
  B11011,
  B11011,
  B11011,
  B11111,
  B00000
};


// *****************************
// ************ 8 **************
// *****************************

// Number 8, black on white
byte NUM_8[8] = {
  B00110,
  B01001,
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000
};

// Number 8, inverted (white on black)
byte NUM_8_INV[8] = {
  B11001,
  B10110,
  B11001,
  B10110,
  B10110,
  B11001,
  B11111,
  B11111
};

// Number 8, black on white, with cursor (underlined)
byte NUM_8_CUR[8] = {
  B00110,
  B01001,
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B11111
};

// Number 8, inverted (white on black), with cursor (underlined)
byte NUM_8_CUR_INV[8] = {
  B11001,
  B10110,
  B11001,
  B10110,
  B10110,
  B11001,
  B11111,
  B00000
};


// *****************************
// ************ 9 **************
// *****************************

// Number 9, black on white
byte NUM_9[8] = {
  B00110,
  B01001,
  B01001,
  B00111,
  B00001,
  B01110,
  B00000,
  B00000
};

// Number 9, inverted (white on black)
byte NUM_9_INV[8] = {
  B11001,
  B10110,
  B10110,
  B11000,
  B11110,
  B10001,
  B11111,
  B11111
};


// Number 9, black on white, with cursor (underlined)
byte NUM_9_CUR[8] = {
  B00110,
  B01001,
  B01001,
  B00111,
  B00001,
  B01110,
  B00000,
  B11111
};

// Number 9, inverted (white on black), with cursor (underlined)
byte NUM_9_CUR_INV[8] = {
  B11001,
  B10110,
  B10110,
  B11000,
  B11110,
  B10001,
  B11111,
  B00000
};


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
