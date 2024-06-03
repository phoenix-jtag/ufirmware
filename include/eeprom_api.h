#pragma once

#include <EEPROM.h>

//#define DEBUG 1

#define EEPROM_ADDR  0
#define EEPROM_SIZE  1024

#define EEPROM_PAGE_SIZE    256
#define EEPROM_PAGE_COUNT   4


#define DEFAULT_ID   0
#define DEFAULT_MODE 0
#define INITED       1


enum class pages_list {

    SERVICE,
    TOUCH,
    MATRIX,
    DATA,
    SCOPE
};


enum class eeprom_states {

    IS_INITED,
    FAIL_INIT,
    FAIL_SET,
    FAIL_GET,
};


struct eeprom_config {

    bool    state;              // <- 0 - undef, 1 - default, 2 - custom
    char    device_id[32];      // <- device id
}; 


class eeprom_api {

private:
    // singleton pattern
    eeprom_api(){}

public:
    // singleton pattern
    static eeprom_api& getInstance() {
        static eeprom_api instance; // Guaranteed to be destroyed.
        return instance;            // Instantiated on first use.
    }
    eeprom_api(eeprom_api const&) = delete;
    void operator=(eeprom_api const&) = delete;

    // eeprom objects
    eeprom_states eeprom_state;
    eeprom_config eeprom_conf;

    // eeprom memory ctor and dtor
    eeprom_api();           // <- init memory, load default or custom config
    ~eeprom_api();          // <- save current config to eeprom 

    // memory methods
    bool set_page(pages_list page, size_t size, void* data); // <- RAM ---> EEPROM
    bool get_page(pages_list page, size_t size, void* data); // <- EEPROM ---> RAM
    
    bool clr_page(pages_list page);                          // <- ERASE PAGE
    bool clr_eeprom();                                       // <- ERASE ALL EEPROM
};