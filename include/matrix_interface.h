#include <FastLED.h>
FASTLED_USING_NAMESPACE


#define DATA_PIN    13
//#define CLK_PIN   4
#define LED_TYPE    WS2812B

#define COLOR_ORDER GRB
#define BRIGHTNESS  255
#define FPS_RATE    60

#define NUM_COLS    4
#define NUM_ROWS    32
#define NUM_LEDS    NUM_COLS * NUM_ROWS

extern CRGB leds[NUM_LEDS];


enum class matrix_state {

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
};


struct matrix_config {

  uint8_t brightness;
};


class matrix_interface {

private:

    matrix_state  state;
    matrix_config config;

public:

    matrix_interface();
    ~matrix_interface();

    void         set_state(matrix_state state);
    matrix_state get_state();

    void display();

    void set_all(CRGB color);
    void set_row(uint8_t row, CRGB color);
    void set_col(uint8_t col, CRGB color);
    
    void pride();

    void animate_light_rings(bool uprising,  CRGB color); // 0 - falling, 1 - rising
    void animate_light_lines(bool clockwise, CRGB color); // 0 - left,    1 - right

    

 
};