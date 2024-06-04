#pragma once

#include <EEPROM.h>

//#define DEBUG 1
#define MAGIC 0x5A

#define EEPROM_ADDR  0
#define EEPROM_SIZE  2048

#define EEPROM_PAGE_SIZE    512
#define EEPROM_PAGE_COUNT   4


enum class pages_list {

	SERVICE,
	KERNEL,
	TOUCH,
	MATRIX,
	SCOPE
};


enum class eeprom_states {

	FAILURE,
	INITED,
};


struct eeprom_config {

	uint8_t    	state;              // <- 
	char    	device_id[32];      // <- device id
}; 


class eeprom_api {

private:

	// eeprom objects
	static eeprom_states eeprom_state;
	static eeprom_config eeprom_conf;

	// eeprom memory ctor and dtor
	eeprom_api();           // <- init memory, load default or custom config
	~eeprom_api();          // <- save current config to eeprom 

public:

	// singleton pattern
	static eeprom_api& getInstance() {
		static eeprom_api instance; // Guaranteed to be destroyed.
		return instance;            // Instantiated on first use.
	}
	eeprom_api(eeprom_api const&) = delete;
	void operator=(eeprom_api const&) = delete;


	// class methods
	eeprom_states get_state() { return eeprom_state; }; 	 // <- get eeprom state

	// memory methods
	bool set_page(pages_list page, size_t size, void* data); // <- RAM ---> EEPROM
	bool get_page(pages_list page, size_t size, void* data); // <- EEPROM ---> RAM
	
	bool clr_page(pages_list page);                          // <- ERASE PAGE
	bool clr_eeprom();                                       // <- ERASE ALL EEPROM
};