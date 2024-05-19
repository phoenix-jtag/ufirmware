#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define FASTLED_ALLOW_INTERRUPTS 0

// DEFINITIONS and CONSTANTS
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    13
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255

enum class matrix_state {
  BLACK,
  PRIDE,
};

struct matrix_config {
  uint8_t brightness;
};

class matrix_interface {
private:
    matrix_state  state;
    matrix_config config;

    CRGB leds[NUM_LEDS];

    // methods for matrix state
    void black();
    void pride();

public:

    matrix_interface();
    ~matrix_interface();

    void         set_state(matrix_state state);
    matrix_state get_state();

    void display(matrix_state state);
};