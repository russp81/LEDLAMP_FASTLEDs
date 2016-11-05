//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 3
#include "FastLED.h"
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define HOSTNAME "LEDLAMP"

#define DBG_OUTPUT_PORT Serial

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    240
CRGB leds[NUM_LEDS];
 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


enum MODE { HOLD, OFF, ALL, MIXEDSHOW, RAINBOW, CONFETTI,
            SINELON, JUGGLE, BPM, PALETTE_ANIMS, RIPPLE, COMET, THEATERCHASE };

MODE mode = OFF;   // Standard mode that is active when software starts

uint8_t FPS = 50;               // Global variable for storing the frames per second
uint8_t brightness = 255;       // Global variable for storing the brightness (255 == 100%)
uint8_t show_length = 15;       // Global variable for storing the show_time (in seconds)
uint8_t ftb_speed = 50;         // Global variable for fade to black speed
uint8_t glitter_density = 50;   // Global variable for glitter density      
long lastMillis = 0;            // Global variable for timechecking last show cycle time
long theaterMillis = 0; 
long paletteMillis = 0;         // Global variable for timechecking color palette shifts
bool exit_func = false;         // Global helper variable to get out of the color modes when mode changes
bool GLITTER_ON = false;        // Global to add / remove glitter to any animation

//******Palette Animation Globals*******************************************
uint8_t targetpicker;
uint8_t colorIndex;
String anim_direction = "forward";
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;
//***************************************************************************

//***************RIPPLE******************************************************
int color;
int center = 0;
int step = -1;
int maxSteps = 32;
float fadeRate = 0.8;
int diff;
 
//background color
uint32_t currentBg = random(256);
uint32_t nextBg = currentBg;
//******************************************************************************

byte dothue = 0;
int lead_dot = 0;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

struct ledstate             // Data structure to store a state of a single led
{
   uint8_t red;
   uint8_t green;
   uint8_t blue;
};

typedef struct ledstate LEDState;   // Define the datatype LEDState
LEDState ledstates[NUM_LEDS];       // Get an array of led states to store the state of the whole strip
LEDState main_color;                // Store the "main color" of the strip used in single color modes 
LEDState glitter_color;             // Store the "glitter color" of the strip for glitter mode

