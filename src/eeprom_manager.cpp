#include "eeprom_manager.h"

eeprom_manager::eeprom_manager() {
    // init eeprom memory
    EEPROM.begin(EEPROM_SIZE);

    // check if eeprom is inited and init if not
    if (!is_inited()) {
        // configure eeprom page data
        eeprom_page.magic_number = EEPROM_MAGIC_DATA;

        // set magic data to eeprom memory
        EEPROM.put(EEPROM_ADDR, eeprom_page);
        EEPROM.commit();
    }
};

eeprom_manager::~eeprom_manager() {

    EEPROM.end(); 
}

bool eeprom_manager::is_inited() {
    // check magic data from eeprom memory
    EEPROM.get(EEPROM_ADDR, eeprom_page);
    if (eeprom_page.magic_number == EEPROM_MAGIC_DATA) {
        return true;
    } else {
        return false;
    }
}

bool eeprom_manager::clr_eeprom() {

    for (int i = 0; i < EEPROM_SIZE; i++) { EEPROM.put(i, '\0'); }
    return EEPROM.commit();
}

int eeprom_manager::get_page_number(eeprom_pager page) {
    int ipage = static_cast<int>(page);
    if (ipage == 0 || ipage >= EEPROM_PAGE_COUNT) {
        return -1;
    } else {
        return ipage;
    }
}

bool eeprom_manager::set_page(void *data, size_t size, eeprom_pager page) {

    if (data == NULL) { return false; }
    if (size > EEPROM_PAGE_SIZE) { return false; }
    
    int ipage = get_page_number(page);
    if (ipage == -1) { return false; }

    EEPROM.put(ipage * EEPROM_PAGE_SIZE, data);

    return EEPROM.commit();
}

bool eeprom_manager::get_page(void *data, size_t size, eeprom_pager page) {
    
    if (data == NULL) { return false; }
    if (size > EEPROM_PAGE_SIZE) { return false; }
    
    int ipage = get_page_number(page);
    if (ipage == -1) { return false; }

    EEPROM.get(page * EEPROM_PAGE_SIZE, data);

    return true;
}

bool eeprom_manager::clr_page(eeprom_pager page) {

    int ipage = get_page_number(page);
    if (ipage == -1) { return false; }

    int start_addr = ipage * EEPROM_PAGE_SIZE;

    for (int i = start_addr; i < start_addr + EEPROM_PAGE_SIZE; i++) {
        EEPROM.put(i, '\0');
    }

    return EEPROM.commit();
}