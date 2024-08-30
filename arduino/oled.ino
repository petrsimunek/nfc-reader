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

void displayWriteInit() {
  // set font (approx 15*4 chars available)
  myOled.setFont(u8g_font_unifont);
  // set init position
  myOled.setPrintPos(20, 10);
  // write text to the display
  myOled.print("NFC reader");
  myOled.setPrintPos(0, 25);
  myOled.print("by Petr Simunek");
  myOled.setPrintPos(0, 40);
  myOled.print("petr.simunek@");
  myOled.setPrintPos(0, 55);
    myOled.print("gmail.com");
}