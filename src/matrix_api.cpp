#include "matrix_api.h"

// STRUCT DEFINITIONS
matrix_states matrix_api::matrix_state;
matrix_config matrix_api::matrix_conf;

CRGB leds[NUM_LEDS];


matrix_api::matrix_api() {

	// GET DINAMIC CONFIGURATION
	eeprom.get_page(pages_list::MATRIX, sizeof(matrix_conf), &matrix_conf);

	// CHK DINAMIC CONFIGURATION
	if (matrix_conf.state != MAGIC) { // VALIDATION FAILED

		// APPLY DEFAULT CONFIG
		matrix_conf.state		= MAGIC;
		matrix_conf.fps_rate	= DEFAULT_FPS_RATE;
		matrix_conf.brightness 	= DEFAULT_BRIGHTNESS;

		// SAVE DEFAULT CONFIG
		eeprom.set_page(pages_list::MATRIX, sizeof(matrix_conf), &matrix_conf);
	}

	// INIT FUNCTIONALITY
	FastLED
			.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
			.setCorrection(TypicalLEDStrip);
		
	// set master brightness control
	FastLED.setBrightness(matrix_conf.brightness);

	matrix_state = matrix_states::INITED;

	// debug 
	Serial.println("> matrix: B[" + String(matrix_conf.brightness) + "] F[" + String(matrix_conf.fps_rate) + "]");
}



matrix_api::~matrix_api() {
	// SAVE CONFIG STRUCT to EEPROM_CONFIG
}



void matrix_api::animate_light_rings(bool uprising, CRGB color, int fps) {

    static uint8_t 		 current_row = 0;
    static unsigned long last_update_time = 0;

    // Calculate delay time based on the desired frame rate
    int frame_delay = 1000 / fps;

    // Non-blocking delay check
    unsigned long current_time = millis();

    if (current_time - last_update_time >= frame_delay) {
        set_row(current_row, CRGB::Black); // Turn off the previous row

        if (uprising) {
            current_row = (current_row + 1) % NUM_ROWS; // Move to the next row
        } else {
            current_row = (current_row == 0) ? NUM_ROWS - 1 : current_row - 1; // Move to the previous row
        }

        set_row(current_row, color); // Turn on the current row

        FastLED.show();
        last_update_time = current_time; // Update the last update time
    }
}


void matrix_api::animate_light_lines(bool clockwise, CRGB color, int fps) {
	
    static uint8_t current_row = 0;
    static unsigned long last_update_time = 0;

    // Calculate delay time based on the desired frame rate
    int frame_delay = 1000 / fps;

    // Non-blocking delay check
    unsigned long current_time = millis();
    if (current_time - last_update_time >= frame_delay) {
        set_row(current_row, CRGB::Black); // Turn off the previous row

        if (clockwise) {
            current_row = (current_row + 1) % NUM_ROWS; // Move to the next row
        } else {
            current_row = (current_row == 0) ? NUM_ROWS - 1 : current_row - 1; // Move to the previous row
        }

        set_row(current_row, color); // Turn on the current row

        FastLED.show();
        last_update_time = current_time; // Update the last update time
    }
}


void matrix_api::set_row(uint8_t row, CRGB color) {

		// Assuming NUM_ROWS is the total number of rows in your matrix
		if (row < NUM_ROWS) {

				// Assuming NUM_COLS is the total number of columns in your matrix
				for (uint8_t col = 0; col < NUM_COLS; ++col) {

						uint8_t led_index = row + col * NUM_ROWS;

						if (led_index < NUM_LEDS) {

								leds[led_index] = color;
						}
				} // end for
		} // end if
} 


void matrix_api::set_col(uint8_t col, CRGB color) {

		// Assuming NUM_COLS is the total number of columns in your matrix
		if (col < NUM_COLS) {

				// Assuming NUM_ROWS is the total number of rows in your matrix
				for (uint8_t row = 0; row < NUM_ROWS; ++row) {

						uint8_t led_index = col * NUM_ROWS + row;
						
						if (led_index < NUM_LEDS) {
								leds[led_index] = color;
						}
				}
		}
}






// ANIMATION - METHODS FOR MATRIX STATE ------------------------------
void matrix_api::black_task(void *pvParameters) {

	// init 

	// loop
	for (;;) {
		for (int i = 0; i < NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
		}

		FastLED.show();
		vTaskDelay(1000 / matrix_conf.fps_rate);
	}
}



void matrix_api::pride_task(void *pvParameters) {

	// init 

	// loop
	for (;;) {
		pride();
		
		FastLED.show();
		vTaskDelay(1000 / matrix_conf.fps_rate);
	}
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void matrix_api::pride() 
{
	static uint16_t sPseudotime = 0;
	static uint16_t sLastMillis = 0;
	static uint16_t sHue16 = 0;
 
	uint8_t sat8 = beatsin88( 87, 220, 250);
	uint8_t brightdepth = beatsin88( 341, 96, 224);
	uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
	uint8_t msmultiplier = beatsin88(147, 23, 60);

	uint16_t hue16 = sHue16;//gHue * 256;
	uint16_t hueinc16 = beatsin88(113, 1, 3000);
	
	uint16_t ms = millis();
	uint16_t deltams = ms - sLastMillis ;
	sLastMillis	= ms;
	sPseudotime += deltams * msmultiplier;
	sHue16 += deltams * beatsin88( 400, 5,9);
	uint16_t brightnesstheta16 = sPseudotime;
	
	for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
		hue16 += hueinc16;
		uint8_t hue8 = hue16 / 256;

		brightnesstheta16	+= brightnessthetainc16;
		uint16_t b16 = sin16( brightnesstheta16	) + 32768;

		uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
		uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
		bri8 += (255 - brightdepth);
		
		CRGB newcolor = CHSV( hue8, sat8, bri8);
		
		uint16_t pixelnumber = i;
		pixelnumber = (NUM_LEDS-1) - pixelnumber;
		
		nblend( leds[pixelnumber], newcolor, 64);
	}
}