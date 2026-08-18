void displayWrite() {
  // set font (approx 15*4 chars available)
  myOled.setFont(u8g_font_unifont);
  myOled.setPrintPos(0, 10);
  myOled.print("Last NFC code:");
  if (lastUidLength == 0) return;
  myOled.setPrintPos(0, 25);
  myOled.print("UID ");
  myOled.print(formatCode(lastUid, lastUidLength, 1));
  myOled.setPrintPos(0, 40);
  myOled.print("EVCH ");
  myOled.print(formatCode(lastUid, lastUidLength, 2));
  myOled.setPrintPos(0, 55);
  myOled.print("ADAM ");
  myOled.print(formatCode(lastUid, lastUidLength, 3));
}

void displayWriteInit() {
  // set font (approx 15*4 chars available)
  myOled.setFont(u8g_font_unifont);
  // set init position
  myOled.setPrintPos(20, 10);
  // write text to the display
  myOled.print("NFC reader");
  myOled.setPrintPos(0, 25);
  myOled.print("v");
  myOled.print(firmwareVersion);
  myOled.setPrintPos(0, 40);
  myOled.print("petr.simunek@");
  myOled.setPrintPos(0, 55);
  myOled.print("gmail.com");
}