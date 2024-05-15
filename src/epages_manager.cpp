#include "epages_manager.h"

epages_manager::epages_manager() {
    // check if the pages are inited and init if not
    if (!is_inited()) {
        // configure config page data
        config_page.magic_number = PAGE_MAGIC_NUMBER;
        config_page.device_id = 0;

        // set magic data to config page memory
        eeprom_interface.set_page(&config_page, sizeof(config_page), CONFIG);


        // configure states page data
        states_page.magic_number = PAGE_MAGIC_NUMBER;
        states_page.device_state = 0;

        // set magic data to states page memory
        eeprom_interface.set_page(&states_page, sizeof(states_page), STATES);

        // configure data page data
        data_page.magic_number = PAGE_MAGIC_NUMBER;
        data_page.device_data = 0;

        // set magic data to data page memory
        eeprom_interface.set_page(&data_page, sizeof(data_page), DATA);
    }

     // Assuming eeprom_interface has a get_page method that reads an EEPROM page
    eeprom_interface.get_page(&config_page, sizeof(config_page), CONFIG);
    eeprom_interface.get_page(&states_page, sizeof(states_page), STATES);
    eeprom_interface.get_page(&data_page, sizeof(data_page), DATA);
};

epages_manager::~epages_manager() {
    
};

bool epages_manager::is_inited() {

};

bool epages_manager::clr_epages() {
    return true;
};

bool epages_manager::set_device_id(uint8_t device_id) {
    return true;
};

bool epages_manager::get_device_id(uint8_t *device_id) {
    return true;
};

bool epages_manager::clr_device_id() {
    return true;
};

bool epages_manager::set_device_state(uint8_t device_state) {
    return true;
};

bool epages_manager::get_device_state(uint8_t *device_state) {
    return true;
};

bool epages_manager::clr_device_state() {
    return true;
};

bool epages_manager::set_device_data(uint8_t device_data) {
    return true;
};

bool epages_manager::get_device_data(uint8_t *device_data) {
    return true;
};

bool epages_manager::clr_device_data() {
    return true;
};

