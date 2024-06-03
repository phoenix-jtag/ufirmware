#include "eeprom_api.h"


// PAGES DEFINITIONS
#define SERVICE     pages_list::SERVICE
#define TOUCH       pages_list::TOUCH
#define MATRIX      pages_list::MATRIX

// STATES DEFINITIONS
#define IS_INITED   eeprom_states::IS_INITED
#define IS_CUSTOM   eeprom_states::IS_CUSTOM
#define IS_DEFAULT  eeprom_states::IS_DEFAULT
#define FAIL_INIT   eeprom_states::FAIL_INIT
#define FAIL_SET    eeprom_states::FAIL_SET
#define FAIL_GET    eeprom_states::FAIL_GET



// <- init memory, load default or custom config
eeprom_api::eeprom_api() {

    // EEPROM INITIALIZATION
    EEPROM.begin(EEPROM_SIZE) ? eeprom_state = IS_INITED : eeprom_state = FAIL_INIT;
 
    if (eeprom_state == IS_INITED) { // EEPROM IS AVAILABLE

        // READ SERVICE PAGE
        if (get_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf)) {
                
                // SUCCESS READING
                if (eeprom_conf.device_id[0] == '\0') {
                    
                    // DEFAULT CONFIG
                    strcpy(eeprom_conf.device_id, "default_id");

                    set_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf) ? eeprom_state = IS_DEFAULT : eeprom_state = FAIL_SET;

                    Serial.println("> device_id: " + String(eeprom_conf.device_id));
    
                } else { 

                    // CUSTOM CONFIG
                    eeprom_state = IS_CUSTOM;
                    Serial.println("> device_id: " + String(eeprom_conf.device_id));
                }
    
            } else { 
    
                // FAILURE READING
                eeprom_state = FAIL_GET;
                Serial.println("> eeprom: fail_get");
        }

    } else { 
        
        // EEPROM IS NOT AVAILABLE
        Serial.println("> eeprom: fail_init"); 
    }

};



eeprom_api::~eeprom_api() {

    // SAVE SERVICE PAGE
    set_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf);
    EEPROM.end(); 
}



bool eeprom_api::set_page(pages_list page, size_t size, void* data) {

    if (size > EEPROM_PAGE_SIZE)   { return false; } // <- check size (max 256 bytes
    if (page >= pages_list::SCOPE) { return false; } // <- check page (max 4 pages)
    if (data == nullptr)           { return false; } // <- check data (not null

    int start_address = static_cast<int>(page) * EEPROM_PAGE_SIZE;
    const uint8_t* data_ptr = static_cast<const uint8_t*>(data);

    for (size_t i = 0; i < size; ++i) {

        EEPROM.write(start_address + i, data_ptr[i]);
    }

    return EEPROM.commit();
}



bool eeprom_api::get_page(pages_list page, size_t size, void* data) {

    if (size > EEPROM_PAGE_SIZE)   { return false; } // <- check size (max 256 bytes
    if (page >= pages_list::SCOPE) { return false; } // <- check page (max 4 pages)
    if (data == nullptr)           { return false; } // <- check data (not null

    int start_address = static_cast<int>(page) * EEPROM_PAGE_SIZE;
    uint8_t* data_ptr = static_cast<uint8_t*>(data);

    for (size_t i = 0; i < size; ++i) {

        data_ptr[i] = EEPROM.read(start_address + i);
    }

    return true;
}



bool eeprom_api::erase_page(pages_list page) {

    if (page >= pages_list::SCOPE) { return false; } // <- check page (max 4 pages)

    int start_address = static_cast<int>(page) * EEPROM_PAGE_SIZE;

    for (int i = 0; i < EEPROM_PAGE_SIZE; i++) { EEPROM.put(start_address + i, '\0'); }

    return EEPROM.commit();
}



bool eeprom_api::erase_full() {

    for (int i = 0; i < EEPROM_SIZE; i++) { EEPROM.put(i, '\0'); }

    return EEPROM.commit();
}