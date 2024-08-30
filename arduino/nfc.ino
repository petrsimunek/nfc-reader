void readNfc() {
  // init vars, where the results will be stored
  boolean success;                          // successfull reading
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // unique ID of the NFC tag
  uint8_t uidLength;                        // length of ID
  // start reading nearby tags, results will be stored to variables
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  // if the read is success, print results to serial
  if (success) {
    Serial.println("STATUS-NFC tag found!");
    Serial.print("STATUS-NFC UID length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("STATUS-NFC UID value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
      decimalValue = (decimalValue << 8) | uid[i];
    }
    stringDecimalValue = String(decimalValue);
    Serial.println();
    Serial.print("STATUS-NFC UID value decimal: ");
    Serial.println(stringDecimalValue);
    // avoid repeated sending
    if (configuration.doubleReadProtection && (stringDecimalValue == prevStringDecimalValue)) {
      Serial.println("STATUS-Double reading protection.");
      return;
    }
    //increase number of scanned NFCs
    scannedNFCCounter++;
    // store to history
    prevStringDecimalValue = stringDecimalValue;
    if (configuration.sendToKeyboard) {
      sendToKeyboard();
    }

  }
}