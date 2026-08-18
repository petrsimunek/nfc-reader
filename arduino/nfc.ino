// CODE MODES
// 1 = UID  - full UID as hex string
// 2 = EVCH - full UID as decimal
// 3 = ADAM - first 3 bytes reversed, as decimal
// 4 = CARD - UID, EVCH and ADAM in sequence, KEYAFTERSEND between them

String formatCode(uint8_t *uid, uint8_t uidLength, int mode) {
  switch (mode) {
    case 1: { // UID
      String hexValue = "";
      for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] < 0x10) hexValue += "0";
        hexValue += String(uid[i], HEX);
      }
      hexValue.toUpperCase();
      return hexValue;
    }
    case 3: { // ADAM - first 3 bytes, reversed order, as decimal
      if (uidLength < 3) return "0";
      unsigned long adamValue = ((unsigned long)uid[2] << 16) | ((unsigned long)uid[1] << 8) | uid[0];
      return String(adamValue);
    }
    case 2: // EVCH
    default: {
      decimalValue = 0;
      for (uint8_t i = 0; i < uidLength; i++) {
        decimalValue = (decimalValue << 8) | uid[i];
      }
      return String(decimalValue);
    }
  }
}

String formatCode(uint8_t *uid, uint8_t uidLength) {
  int mode = configuration.codeMode;
  if (mode == 4) mode = 1; // CARD uses UID on display / history
  return formatCode(uid, uidLength, mode);
}

void readNfc() {
  static uint8_t acceptedUid[10] = { 0 };
  static uint8_t acceptedUidLength = 0;
  static unsigned long acceptedMillis = 0;

  boolean success;
  uint8_t uid[10] = { 0 };
  uint8_t uidLength = 0;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (!success) return;
  if (uidLength > 10) uidLength = 10;
  if (uidLength == 0) return;

  Serial.println("STATUS-NFC tag found!");
  Serial.print("STATUS-NFC UID length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
  Serial.print("STATUS-NFC UID value: ");
  for (uint8_t i = 0; i < uidLength; i++) {
    Serial.print(" 0x"); Serial.print(uid[i], HEX);
  }
  Serial.println();

  lastUidLength = uidLength;
  for (uint8_t i = 0; i < uidLength; i++) {
    lastUid[i] = uid[i];
  }
  stringDecimalValue = formatCode(uid, uidLength);
  Serial.print("STATUS-NFC code (mode ");
  Serial.print(configuration.codeMode);
  Serial.print("): ");
  if (configuration.codeMode == 4) {
    Serial.print(formatCode(uid, uidLength, 1));
    Serial.print(" / ");
    Serial.print(formatCode(uid, uidLength, 2));
    Serial.print(" / ");
    Serial.println(formatCode(uid, uidLength, 3));
  } else {
    Serial.println(stringDecimalValue);
  }

  bool sameCard = (uidLength == acceptedUidLength);
  if (sameCard) {
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] != acceptedUid[i]) {
        sameCard = false;
        break;
      }
    }
  }

  unsigned long now = millis();
  unsigned long elapsed = now - acceptedMillis;
  Serial.print("STATUS-DRP enabled=");
  Serial.print(configuration.doubleReadProtection);
  Serial.print(" same=");
  Serial.print(sameCard);
  Serial.print(" ms=");
  Serial.println(elapsed);

  // same card cannot be sent again for 10 seconds
  if (configuration.doubleReadProtection && sameCard && (elapsed < 10000UL)) {
    Serial.println("STATUS-Double reading protection.");
    return;
  }

  prevStringDecimalValue = stringDecimalValue;
  acceptedUidLength = uidLength;
  for (uint8_t i = 0; i < uidLength; i++) {
    acceptedUid[i] = uid[i];
  }
  acceptedMillis = now;
  if (configuration.sendToKeyboard) {
    sendToKeyboard();
  }
}
