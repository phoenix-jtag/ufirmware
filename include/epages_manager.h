#pragma once

#include "eeprom_manager.h"

class epages_manager {
public: 
    // pages structures 
    struct config_page {
        uint8_t magic_number;
        uint8_t device_id;
    };

    struct states_page {
        uint8_t magic_number;
        uint8_t device_state;
    };

    struct data_page {
        uint8_t magic_number;
        uint8_t device_data;
    };

    // ctor and dtor
    epages_manager();
    ~epages_manager();

    // pages memory methods
    bool is_inited();
    bool clr_epages();

    // config memory methods
    bool set_device_id(uint8_t device_id);
    bool get_device_id(uint8_t *device_id);
    bool clr_device_id();

    // states memory methods
    bool set_device_state(uint8_t device_state);
    bool get_device_state(uint8_t *device_state);
    bool clr_device_state();

    // data memory methods
    bool set_device_data(uint8_t device_data); 
    bool get_device_data(uint8_t *device_data); 
    bool clr_device_data();
};