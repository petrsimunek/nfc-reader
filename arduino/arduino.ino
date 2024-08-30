// include libs
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <Keyboard.h>
#include <U8glib.h>
#include <avr/wdt.h>


// config structure
struct config
{
  bool sendToKeyboard;
  bool sendKeyAfter;
  int keyAfterSend;
  int keyboardMode;
  bool doubleReadProtection;
};
// global variable for configuration
config configuration;


// init I2C communication with NFC reader
PN532_I2C pn532i2c(Wire);
// init instance of NFC reader
PN532 nfc(pn532i2c);
// init OLED display
U8GLIB_SSD1306_128X64 myOled(U8G_I2C_OPT_NONE);
unsigned long previousMillis = 0;   // timer for loop
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
  // greetings on display
  myOled.firstPage();
  do {
    displayWriteInit();
  } while( myOled.nextPage() );
  // greetings on serial after a small delay
  delay(3000);
  Serial.println("STATUS-Hello!");
  // init configuration
  // check first run, then set the dafults and save it to the EEPROM
  if (isFirstRun()) {
    Serial.println("STATUS-First run, writing the defaults to EEPROM.");
    setDefaultConfig();
    saveConfig();
  }
  printCurrentConfig();
  // start NFC communication
  nfc.begin();
  // set max no of NFC read attempts
  nfc.setPassiveActivationRetries(0xFF);
  // configure NFC for reading tags
  nfc.SAMConfig();
  // debug info for user
  Serial.println("STATUS-Ready");
}







void loop() {
  // current time for timer
  unsigned long currentMillis = millis();
  //read NFC in interval
  if (currentMillis - previousMillis >= (2000)) {
    // update prev. timer
    previousMillis = currentMillis;
    // read NFC
    readNfc();
    // rewrite display
    // refresh display
    myOled.firstPage();
    do {
      displayWrite();
    } while( myOled.nextPage() );
  }
  //check new commands
  checkSerialCommands();
}







// checks serial commands
void checkSerialCommands() {
    // check
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        // run function based on command prefix
        if (command.indexOf("CONFIG-") == 0) newConfig(command);
        else if (command == "SHOWCONFIG") printCurrentConfig();
        else if (command == "SHOWNFCLAST") Serial.println(prevStringDecimalValue);
        else if (command == "RESTART") restart();
        else if (command == "RESET") reset();
        else Serial.println("Unknown command prefix");
    }
}

// function for restarting arduino
void restart() {
  Serial.println("STATUS-Restarting device");
  wdt_enable(WDTO_15MS);
  while (1) {
    // infinite loop for restarting the arduino
  }
}




