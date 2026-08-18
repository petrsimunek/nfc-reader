#include <EEPROM.h>

bool defaultSendToKeyboard = true;
bool defaultSendKeyAfter = true;
int defaultKeyAfterSend = 1;
int defaultKeyboardMode = 1;
bool defaultDoubleReadProtection = true;
int defaultCodeMode = 2; // EVCH

// EEPROM is empty?
bool isFirstRun() {
  return EEPROM.read(0) == 0xFF;
}

// keep config values in valid ranges (e.g. after firmware upgrade)
void sanitizeConfig() {
  if (configuration.codeMode < 1 || configuration.codeMode > 4) {
    configuration.codeMode = defaultCodeMode;
  }
  if (configuration.keyboardMode < 1 || configuration.keyboardMode > 3) {
    configuration.keyboardMode = defaultKeyboardMode;
  }
  if (configuration.keyAfterSend < 1 || configuration.keyAfterSend > 3) {
    configuration.keyAfterSend = defaultKeyAfterSend;
  }
  // EEPROM bools can be 0xFF after struct changes; treat garbage as default
  if (configuration.doubleReadProtection != 0 && configuration.doubleReadProtection != 1) {
    configuration.doubleReadProtection = defaultDoubleReadProtection;
  }
  if (configuration.sendToKeyboard != 0 && configuration.sendToKeyboard != 1) {
    configuration.sendToKeyboard = defaultSendToKeyboard;
  }
  if (configuration.sendKeyAfter != 0 && configuration.sendKeyAfter != 1) {
    configuration.sendKeyAfter = defaultSendKeyAfter;
  }
}

void loadConfig() {
  EEPROM.get(1, configuration);
  sanitizeConfig();
}

// function for printing current configuration to serial console
void printCurrentConfig() {
  Serial.println("STATUS-Current configuration:");
  Serial.print("STATUS-VERSION:");
  Serial.println(firmwareVersion);
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
  Serial.print("CONFIG-CODEMODE:");
  Serial.println(configuration.codeMode);
}

// set default values to global configuration variable
void setDefaultConfig() {
  configuration.sendToKeyboard = defaultSendToKeyboard;
  configuration.sendKeyAfter = defaultSendKeyAfter;
  configuration.keyAfterSend = defaultKeyAfterSend;
  configuration.keyboardMode = defaultKeyboardMode;
  configuration.doubleReadProtection = defaultDoubleReadProtection;
  configuration.codeMode = defaultCodeMode;
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
    else if (parameter == "CODEMODE"){configuration.codeMode = value.toInt();}
    else {Serial.println("Unknown parameter.");}
    sanitizeConfig();
    // save config to EEPROM
    saveConfig();
    Serial.println("STATUS-New config has been set.");
  }
  else {
    Serial.println("Colon not found, config command corrupted");
  }
}