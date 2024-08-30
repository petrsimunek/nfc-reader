#include <EEPROM.h>

bool defaultSendToKeyboard = true;
bool defaultSendKeyAfter = true;
int defaultKeyAfterSend = 1;
int defaultKeyboardMode = 1;
bool defaultDoubleReadProtection = true;

// EEPROM is empty?
bool isFirstRun() {
  return EEPROM.read(0) == 0xFF;
}

// function for printing current configuration to serial console
void printCurrentConfig() {
  Serial.println("STATUS-Current configuration:");
  // get from EEPROM
  EEPROM.get(1, configuration);
  // serial print every config value
  Serial.print("CONFIG-SENDTOKEYBOARD:");
  Serial.println(configuration.sendToKeyboard);
  Serial.print("CONFIG-SENDKEYAFTER:");
  Serial.println(configuration.sendKeyAfter);
  Serial.print("CONFIG-KEYAFTERSEND:");
  Serial.println(configuration.keyAfterSend);
  Serial.print("CONFIG-KEYBOARDMODE:");
  Serial.println(configuration.keyboardMode);
  Serial.print("CONFIG-DOUBLEREADPROTECTION:");
  Serial.println(configuration.doubleReadProtection);

}

// set default values to global configuration variable
void setDefaultConfig() {
  configuration.sendToKeyboard = defaultSendToKeyboard;
  configuration.sendKeyAfter = defaultSendKeyAfter;
  configuration.keyAfterSend = defaultKeyAfterSend;
  configuration.keyboardMode = defaultKeyboardMode;
  configuration.doubleReadProtection = defaultDoubleReadProtection;
}

// save the current configuration to the EEPROM
void saveConfig() {
  Serial.println("STATUS-Saving current configuation to EEPROM.");
  EEPROM.write(0, 0x00);      // set first block of EEPROM to 0 to indicate, that the configuration is there
  EEPROM.put(1, configuration);
}

// reset to defaults
void reset() {
  Serial.println("STATUS-Reseting configuration");
  EEPROM.write(0, 0xFF);  // set memory indicator
  restart();
}


void newConfig(String command) {
  Serial.print("STATUS-New configuration value came: ");
  // get rest of the command
  String remaining = command.substring(7);
  remaining.trim();
  // find colon
  int colonIndex = remaining.indexOf(':');
  if (colonIndex != -1) {
    // split to parameter and value
    String parameter = remaining.substring(0, colonIndex);
    String value = remaining.substring(colonIndex + 1);
    parameter.trim();
    value.trim();
    // serial
    Serial.print("Parameter:");
    Serial.print(parameter);
    Serial.print(", Value=");
    Serial.println(value);
    // set parameter
    if (parameter == "SENDTOKEYBOARD"){configuration.sendToKeyboard = value.toInt();}
    else if (parameter == "SENDKEYAFTER"){configuration.sendKeyAfter = value.toInt();}
    else if (parameter == "KEYAFTERSEND"){configuration.keyAfterSend = value.toInt();}
    else if (parameter == "KEYBOARDMODE"){configuration.keyboardMode = value.toInt();}
    else if (parameter == "DOUBLEREADPROTECTION"){configuration.doubleReadProtection = value.toInt();}
    else {Serial.println("Unknown parameter.");}
    // save config to EEPROM
    saveConfig();
    Serial.println("STATUS-New config has been set.");
  }
  else {
    Serial.println("Colon not found, config command corrupted");
  }
}