// KEY AFTER THE CODE MAP
// 1 = KEY_TAB
// 2 = KEY_RETURN
// 3 = KEY_ESC

// KEYBOARD MODES
// 1 = EN
// 2 = CS
// 3 = NUMPAD

// general function
void sendToKeyboard() {
  // send to keyboard using methid based on configuration
  switch (configuration.keyboardMode) {
    case 1: stkEN(); break;
    case 2: stkCS(); break;
    case 3: stkNUMPAD(); break;
  }
  // send defined key after the code, if configured
  if (configuration.sendKeyAfter) {
    stkKEYAFTER();
  }
}


// send to keyboard EN
void stkEN() {
  for (int i = 0; i < stringDecimalValue.length(); i++) {
    // get current number
      int digit = stringDecimalValue[i] - '0';
      // normal 48 is shifting to numbers in ASCII table
      Keyboard.write(digit+48);
  }
}

// send to keyboard CS
void stkCS() {
  Keyboard.press(KEY_LEFT_SHIFT); // with shift - for czech keyboard
  stkEN();
  Keyboard.releaseAll();
}

// send to keyboard NUMPAD
void stkNUMPAD() {
  for (int i = 0; i < stringDecimalValue.length(); i++) {
      // get current number
      int digit = stringDecimalValue[i] - '0';
      switch (digit) {
        case 0: Keyboard.write(KEY_KP_0); break;
        case 1: Keyboard.write(KEY_KP_1); break;
        case 2: Keyboard.write(KEY_KP_2); break;
        case 3: Keyboard.write(KEY_KP_3); break;
        case 4: Keyboard.write(KEY_KP_4); break;
        case 5: Keyboard.write(KEY_KP_5); break;
        case 6: Keyboard.write(KEY_KP_6); break;
        case 7: Keyboard.write(KEY_KP_7); break;
        case 8: Keyboard.write(KEY_KP_8); break;
        case 9: Keyboard.write(KEY_KP_9); break;
      }
  }
}


// send key after the code
void stkKEYAFTER() {
  // send appropriate key after the code
  switch (configuration.keyAfterSend) {
    case 1: Keyboard.write(KEY_TAB); break;
    case 2: Keyboard.write(KEY_RETURN); break;
    case 3: Keyboard.write(KEY_ESC); break;
  }
}

