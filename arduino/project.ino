// include libs
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <U8glib.h>
#include <Keyboard.h>

// init I2C communication with NFC reader
PN532_I2C pn532i2c(Wire);
// init instance of NFC reader
PN532 nfc(pn532i2c);

// init OLED display
U8GLIB_SSD1306_128X64 myOled(U8G_I2C_OPT_NONE);
long int rewrite = 0;               // timer for rewrite the display
int scannedNFCCounter = 0;          // counter of scanned NFC tags since start

// init var for decimal value of the NFS tag
unsigned long decimalValue = 0;       // ID in dec
String stringDecimalValue;            // ID in string
String prevStringDecimalValue;        // previous ID in string

void setup() {
  // keyboard HID start
  Keyboard.begin();
  // start serial communication
  Serial.begin(9600);
  // greetings on serial after a small delay
  delay(3000);
  Serial.println("Hello!");
  // start NFC communication
  nfc.begin();
  // set max no of NFC read attempts
  // set to approx. one second
  nfc.setPassiveActivationRetries(0xFF);
  // configure NFC for reading tags
  nfc.SAMConfig();
  // debug info for user
  Serial.println("Ready");
}

void loop() {
  // init vars, where the results will be stored
  boolean success;                          // successfull reading
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // unique ID of the NFC tag
  uint8_t uidLength;                        // length of ID

  //display refresh
  if (millis()-rewrite > 100) {
    // following set of commands refresh display
    // refresh display
    myOled.firstPage();
    do {
      displayWrite();
    } while( myOled.nextPage() );
    // save last time of rewrite
    rewrite = millis();
  }

  // start reading nearby tags, results will be stored to variables
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  // if the read is success, print results to serial
  if (success) {
    Serial.println("NFC tag found!");
    Serial.print("UID length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
      decimalValue = (decimalValue << 8) | uid[i];
    }
    stringDecimalValue = String(decimalValue);
    Serial.println();
    Serial.println("UID value complete: ");
    Serial.println(stringDecimalValue);
    // avoid repeated sending
    if (stringDecimalValue != prevStringDecimalValue) {
      // send to keyboard
      for (int i = 0; i < stringDecimalValue.length(); i++) {
        // get current number
        int digit = stringDecimalValue[i] - '0';
        // send it to keyboard
        typeNumber(digit);
      }
      Keyboard.write(KEY_TAB);
      //increase number of scanned NFCs
      scannedNFCCounter++;
      // store to history
      prevStringDecimalValue = stringDecimalValue;
    }
    // wait 1 second before next read
    delay(1000);
  }
  
}

void displayWrite() {
  // set font (approx 15*4 chars available)
  myOled.setFont(u8g_font_unifont);
  // set init position
  myOled.setPrintPos(0, 10);
  // write text to the display
  myOled.print("Last NFC code:");
  myOled.setPrintPos(0, 25);
  myOled.print(stringDecimalValue);
  myOled.setPrintPos(0, 40);
  myOled.print("Scanned:");
  myOled.setPrintPos(40, 55);
  myOled.print(scannedNFCCounter);
  myOled.print(" NFCs");
}

void typeNumber(int digit) {
  // simulate numpad
  // added this due the keyboard lang settings
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