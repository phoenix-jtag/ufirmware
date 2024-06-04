#pragma once
#include <Arduino.h>
#include <Button2.h>
#include "eeprom_api.h"

#define MAGIC 0x5A

#define TOUCH_PIN T0 
#define DEFAULT_THRESHOLD_LVL	70
#define DEFAULT_DEBOUNCE_TIME	65


enum class touch_states {

	IDLE,
	TOUCH1,
	TOUCH2,
	TOUCH3,
	PRESS,
	HOLD,
	RESET,
};


struct touch_config {

	uint8_t     state;
	uint8_t     threshold_lvl;
	uint8_t     debounce_time;
};


class touch_api {

private:
	// instances
	eeprom_api& eeprom = eeprom_api::getInstance();

	static touch_states touch_state;
	static touch_config touch_conf;

	Button2 button;

	static void got_touch_event();
	static byte cap_state_handler();

	static void touch1_handler(Button2 &btn) {
		Serial.println("> 1 touch");
		touch_state = touch_states::TOUCH1;
	}

	static void touch2_handler(Button2 &btn) {
		Serial.println("> 2 touch");
		touch_state = touch_states::TOUCH2;
	}

	static void touch3_handler(Button2 &btn) {
		Serial.println("> 3 touch");
		touch_state = touch_states::TOUCH3;
	}

	static void press_handler(Button2 &btn) {
		Serial.println("> press");
		touch_state = touch_states::PRESS;
	}

	static void hold_handler(Button2 &btn) {
		Serial.println("> hold");
		touch_state = touch_states::HOLD;
	}

	touch_api();
	~touch_api();

public:
	// Singleton pattern
    static touch_api& getInstance() {
        static touch_api instance; // Guaranteed to be destroyed.
        return instance;            // Instantiated on first use.
    }
    touch_api(touch_api const&) = delete; // Deleting the copy constructor
    void operator=(touch_api const&) = delete; // Deleting the assignment operator


	touch_states get_state();

	bool touch_calibration();
};