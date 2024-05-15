#pragma once

#include <EEPROM.h>

#define EEPROM_ADDR 0
#define EEPROM_SIZE 1024

#define EEPROM_PAGE_SIZE 256
#define EEPROM_PAGE_COUNT (EEPROM_SIZE / EEPROM_PAGE_SIZE)

#define EEPROM_MAGIC_DATA 0x55

enum eeprom_pager {
    EPAGE = 0,  // 0 - eeprom system page 
    CONFIG, // 1 - device config page
    STATES, // 2 - device states page
    DATA,   // 3 - device data page
    PAGES   // 4 - upper limit
};


class eeprom_manager {
public:
    // eeprom system partition 
    struct eeprom_page {
        uint8_t magic_number;
    };

    // eeprom ctor and dtor (init eeprom, checking pages)
    eeprom_manager();
    ~eeprom_manager();

    // eeprom memory methods
    bool is_inited();
    bool clr_eeprom();

    // help methods
    int get_page_number(eeprom_pager page);

    // page memory methods
    bool set_page(void *data, size_t size, eeprom_pager page);
    bool get_page(void *data, size_t size, eeprom_pager page);
    bool clr_page(eeprom_pager page);

private:
    eeprom_page eeprom_page;
};