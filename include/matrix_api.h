#define FASTLED_INTERNAL
#include <FastLED.h>
#include "eeprom_api.h"
FASTLED_USING_NAMESPACE

#define MAGIC					0x5A

#define DATA_PIN				13
//#define CLK_PIN	 			4
#define LED_TYPE				WS2812B

#define COLOR_ORDER				GRB
#define DEFAULT_BRIGHTNESS		255
#define DEFAULT_FPS_RATE		60

#define NUM_COLS				4
#define NUM_ROWS				32
#define NUM_LEDS				NUM_COLS * NUM_ROWS


enum class matrix_states {

	FAILURE,
	INITED,

	// static
	BLACK,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,

	// animated
	PRIDE,
	LIGHT_RING_RISSING,
	LIGHT_RING_FALLING,

	POWER_ON,
};


struct matrix_config {

	uint8_t		state;
	uint8_t		fps_rate;
	uint8_t	 	brightness;
};


class matrix_api {

private:
	// instances
	eeprom_api& eeprom = eeprom_api::getInstance();

	// matrix objects
	static matrix_states	matrix_state;
	static matrix_config 	matrix_conf;

	matrix_api();
	~matrix_api();

public:
	// singleton pattern
    static matrix_api& getInstance() {
        static matrix_api instance; // Guaranteed to be destroyed.
        return instance;            // Instantiated on first use.
    }
    matrix_api(matrix_api const&) = delete; // Deleting the copy constructor
    void operator=(matrix_api const&) = delete; // Deleting the assignment operator


	// class methods
	void			set_state(matrix_states state);
	matrix_states 	get_state();

	void display();

	void set_all(CRGB color);
	void set_row(uint8_t row, CRGB color);
	void set_col(uint8_t col, CRGB color);
		
	void pride();

	void animate_light_rings_with_acceleration(bool uprising, CRGB color, unsigned long duration_ms);
	void animate_light_lines_with_acceleration(bool clockwise,	CRGB color, unsigned long duration_ms);

	void animate_light_rings(bool uprising,	CRGB color, int fps); // 0 - falling, 1 - rising
	void animate_light_lines(bool clockwise, CRGB color, int fps); // 0 - left,		1 - right

};