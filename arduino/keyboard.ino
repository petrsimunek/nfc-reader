// KEY AFTER THE CODE MAP
// 1 = KEY_TAB
// 2 = KEY_RETURN
// 3 = KEY_ESC

// KEYBOARD MODES
// 1 = EN
// 2 = CS
// 3 = NUMPAD

void sendCodeOnly(String code) {
  stringDecimalValue = code;
  switch (configuration.keyboardMode) {
    case 1: stkEN(); break;
    case 2: stkCS(); break;
    case 3: stkNUMPAD(); break;
  }
}

void sendCardModes() {
  // UID, KEYAFTER, EVCH, KEYAFTER, ADAM
  sendCodeOnly(formatCode(lastUid, lastUidLength, 1));
  stkKEYAFTER();
  sendCodeOnly(formatCode(lastUid, lastUidLength, 2));
  stkKEYAFTER();
  sendCodeOnly(formatCode(lastUid, lastUidLength, 3));
  if (configuration.sendKeyAfter) {
    stkKEYAFTER();
  }
}

// general function
void sendToKeyboard() {
  if (configuration.codeMode == 4) {
    sendCardModes();
    return;
  }
  sendCodeOnly(stringDecimalValue);
  // send defined key after the code, if configured
  if (configuration.sendKeyAfter) {
    stkKEYAFTER();
  }
}


// send to keyboard EN (digits and hex letters A-F)
void stkEN() {
  for (int i = 0; i < stringDecimalValue.length(); i++) {
    char c = stringDecimalValue[i];
    if (c >= '0' && c <= '9') {
      Keyboard.write(c);
    } else if (c >= 'A' && c <= 'F') {
      Keyboard.write(c);
    } else if (c >= 'a' && c <= 'f') {
      Keyboard.write(c - 32); // uppercase
    }
  }
}

// send to keyboard CS
void stkCS() {
  Keyboard.press(KEY_LEFT_SHIFT); // with shift - for czech keyboard
  stkEN();
  Keyboard.releaseAll();
}

// send to keyboard NUMPAD (hex letters fall back to top-row keys)
void stkNUMPAD() {
  for (int i = 0; i < stringDecimalValue.length(); i++) {
      char c = stringDecimalValue[i];
      if (c >= 'a' && c <= 'f') c = c - 32;
      switch (c) {
        case '0': Keyboard.write(KEY_KP_0); break;
        case '1': Keyboard.write(KEY_KP_1); break;
        case '2': Keyboard.write(KEY_KP_2); break;
        case '3': Keyboard.write(KEY_KP_3); break;
        case '4': Keyboard.write(KEY_KP_4); break;
        case '5': Keyboard.write(KEY_KP_5); break;
        case '6': Keyboard.write(KEY_KP_6); break;
        case '7': Keyboard.write(KEY_KP_7); break;
        case '8': Keyboard.write(KEY_KP_8); break;
        case '9': Keyboard.write(KEY_KP_9); break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          Keyboard.write(c);
          break;
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