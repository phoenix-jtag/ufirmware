#include "eeprom_interface.h"

// <- init memory, load default or custom config
eeprom_interface::eeprom_interface() {

    EEPROM.begin(EEPROM_SIZE); // <- init eeprom
 
    if (load() == false) { // config verification

        // set default config
        strcpy(config.device_id, "default_id");
        config.preset_id = DEFAULT_MODE;

        save(); // <- save config to eeprom
    }
    // now we have valid config: custom or default 
    Serial.println("> device_id: " + String(config.device_id));
};

eeprom_interface::~eeprom_interface() {

    save(); // <- save config to eeprom
    EEPROM.end(); 
}

bool eeprom_interface::load() {

    EEPROM.get(EEPROM_ADDR, config);
    return (config.state == INITED) ? true : false;
}

bool eeprom_interface::save() {

    if (sizeof(config) > EEPROM_SIZE) { return false; } // <- check eeprom size

    config.state = INITED; // <- config INITED

    EEPROM.put(EEPROM_ADDR, config);
    return EEPROM.commit();
}

bool eeprom_interface::erase() {

    for (int i = 0; i < EEPROM_SIZE; i++) { EEPROM.put(i, '\0'); }
    return EEPROM.commit();
}