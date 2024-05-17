#pragma once

#include <EEPROM.h>

#define EEPROM_ADDR  0
#define EEPROM_SIZE  1024

#define DEFAULT_ID   0
#define DEFAULT_MODE 0
#define INITED       1

class eeprom_interface {
public:
    // eeprom config partition 
    struct Config {
        bool                state;      // <- 0 - not inited, 1 - inited
        char  device_id[64];      // <- device id
        uint8_t         preset_id;      // <- number of preset
    }; 
    
    Config config;

    // eeprom memory ctor and dtor
    eeprom_interface();           // <- init memory, load default or custom config
    ~eeprom_interface();          // <- save current config to eeprom 

    // eeprom memory methods
    bool load();                // <- load config from eeprom, return true if valid
    bool save();                // <- save config to eeprom, mark as INITED
    bool erase();               // <- erase all eeprom memory, set config state to 0
};

// TODO: add set, get, clr methods for config fields