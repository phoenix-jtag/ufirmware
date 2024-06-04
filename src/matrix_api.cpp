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
		matrix_conf.brightness 	= DEFAULT_BRIGHTNESS;
		matrix_conf.fps_rate	= DEFAULT_FPS_RATE;

		// SAVE DEFAULT CONFIG
		eeprom.set_page(pages_list::MATRIX, sizeof(matrix_conf), &matrix_conf);
	}

	// SET STATIC CONFIGURATION
	matrix_state = matrix_states::BLACK;

	// INIT FUNCTIONALITY
	FastLED
			.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
			.setCorrection(TypicalLEDStrip);
		
	// set master brightness control
	FastLED.setBrightness(matrix_conf.brightness);
}



matrix_api::~matrix_api() {
	// SAVE CONFIG STRUCT to EEPROM_CONFIG
}



void matrix_api::set_state(matrix_states state) {

	this->matrix_state = state;
}



matrix_states matrix_api::get_state() {

	return matrix_state;
}



// MATRIC CONFIG - METHODS FOR CONFIG STRUCT -------------------------
void matrix_api::display() {

	switch (matrix_state) {

		// static
		case matrix_states::BLACK:		fill_solid(leds, NUM_LEDS, CRGB::Black);	FastLED.show(); break;
		case matrix_states::RED:		fill_solid(leds, NUM_LEDS, CRGB::Red);		FastLED.show(); break;
		case matrix_states::GREEN:		fill_solid(leds, NUM_LEDS, CRGB::Green);	FastLED.show(); break;
		case matrix_states::BLUE:		fill_solid(leds, NUM_LEDS, CRGB::Blue);		FastLED.show(); break;
		case matrix_states::YELLOW:		fill_solid(leds, NUM_LEDS, CRGB::Yellow);	FastLED.show(); break;
		case matrix_states::CYAN:		fill_solid(leds, NUM_LEDS, CRGB::Cyan);		FastLED.show(); break;
		case matrix_states::MAGENTA:	fill_solid(leds, NUM_LEDS, CRGB::Magenta); 	FastLED.show(); break;

		// animated
		case matrix_states::PRIDE: pride(); break;
		case matrix_states::LIGHT_RING_RISSING: animate_light_rings(1, CRGB::Cyan); break;
		case matrix_states::LIGHT_RING_FALLING: animate_light_rings(0, CRGB::Cyan); break;

		default: break;
	}
}


// SYSTEM - METHODS FOR MATRIX STATE ---------------------------------
void matrix_api::animate_light_rings(bool uprising, CRGB color) {

		static uint8_t current_row = 0;
		static unsigned long last_update_time = 0;

		// Map the speed level to a delay time
		int delay_time = 10;

		if (millis() - last_update_time >= delay_time) {

				set_row(current_row, CRGB::Black);												// Turn off the previous row
				
				if (uprising) {
						current_row = (current_row + 1) % NUM_ROWS;							 	// Move to the next row
				} else {
						current_row = (current_row == 0) ? NUM_ROWS - 1 : current_row - 1; 		// Move to the previous row
				}
													// Move to the next row
				set_row(current_row, color);																	// Turn on the current row

				FastLED.show();
				last_update_time = millis();																	// Update the last update time
		}
}


void matrix_api::animate_light_lines(bool clockwise, CRGB color) {

		static uint8_t current_col = 0;
		static unsigned long last_update_time = 0;

		// Map the speed level to a delay time
		int delay_time = 50;

		if (millis() - last_update_time >= delay_time) {

				set_col(current_col, CRGB::Black);														// Turn off the previous column
				
				if (clockwise) {
						current_col = (current_col + 1) % NUM_COLS;							 // Move to the next column
				} else {
						current_col = (current_col == 0) ? NUM_COLS - 1 : current_col - 1; // Move to the previous column
				}
													// Move to the next column
				set_col(current_col, color);																	// Turn on the current column

				FastLED.show();
				last_update_time = millis();																	// Update the last update time
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
	FastLED.show();
}