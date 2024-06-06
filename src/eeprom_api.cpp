#include "eeprom_api.h"

// PAGES DEFINITIONS
#define SERVICE     pages_list::SERVICE
#define TOUCH       pages_list::TOUCH
#define MATRIX      pages_list::MATRIX

// STATES DEFINITIONS
#define FAILURE   	eeprom_states::FAILURE
#define INITED   	eeprom_states::INITED

// STATIC VARIABLES
eeprom_states eeprom_api::eeprom_state;
eeprom_config eeprom_api::eeprom_conf;


eeprom_api::eeprom_api() {

	// EEPROM INITIALIZATION
	EEPROM.begin(EEPROM_SIZE) ? eeprom_state = INITED : eeprom_state = FAILURE;

	if (eeprom_state == FAILURE) { Serial.println("> eeprom: failure"); } // <- check eeprom state

	// GET DINAMIC CONFIGURATION
	get_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf);
				
	// CHK DINAMIC CONFIGURATION			
	if (eeprom_conf.state != MAGIC) { // VALIDATION FAILED

		// APPLY DEFAULT CONFIG
		eeprom_conf.state = MAGIC;
		strcpy(eeprom_conf.device_id, "default_id");

		// SAVE DEFAULT CONFIG
		set_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf);

	} // <- eeprom_conf.state != MAGIC


	Serial.println("> device_id: " + String(eeprom_conf.device_id));
}; // <- eeprom_api ctor



eeprom_api::~eeprom_api() {

	// SAVE SERVICE PAGE
	set_page(SERVICE, sizeof(eeprom_conf), &eeprom_conf);
	EEPROM.end(); 
}



bool eeprom_api::set_page(pages_list page, size_t size, void* data) {

	if (eeprom_state == FAILURE)   { return false; } // <- check eeprom state
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

	if (eeprom_state == FAILURE)   { return false; } // <- check eeprom state
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



bool eeprom_api::clr_page(pages_list page) {

	if (eeprom_state == FAILURE)   { return false; } // <- check eeprom state
	if (page >= pages_list::SCOPE) { return false; } // <- check page (max 4 pages)

	int start_address = static_cast<int>(page) * EEPROM_PAGE_SIZE;

	for (int i = 0; i < EEPROM_PAGE_SIZE; i++) { EEPROM.put(start_address + i, '\0'); }

	return EEPROM.commit();
}



bool eeprom_api::clr_eeprom() {

	if (eeprom_state == FAILURE) { return false; } // <- check eeprom state

	for (int i = 0; i < EEPROM_SIZE; i++) { EEPROM.put(i, '\0'); }

	return EEPROM.commit();
}