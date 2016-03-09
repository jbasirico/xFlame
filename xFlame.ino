
//  02-08-2016 Using FastLED library based on Flame template. Added parameters for 3 LED strands. Tested on one strand/


#include "FastLED.h"

#define NUM_LEDS1_APA102 125
#define NUM_LEDS2_APA102 116
#define NUM_LEDS_WS2801 25

#define LED1_PIN    13
#define CLOCK1_PIN  A0
#define LED2_PIN    9
#define CLOCK2_PIN  11
#define LED3_PIN    A4
#define CLOCK3_PIN  0


#define APA102_COLOR_ORDER BGR
#define WS2801_COLOR_ORDER GRB
#define CHIPSET1     APA102
#define CHIPSET2     WS2801



CRGB leds1[NUM_LEDS1_APA102];
CRGB leds2[NUM_LEDS2_APA102];
CRGB leds3[NUM_LEDS_WS2801];

#define BRIGHTNESS  150
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB color;
float hue = 10; // for rainbow

void setup() {
  delay(3000); // sanity delay
  
  FastLED.addLeds<CHIPSET1, LED1_PIN, CLOCK1_PIN, APA102_COLOR_ORDER>(leds1, NUM_LEDS1_APA102).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET1, LED2_PIN, CLOCK2_PIN, APA102_COLOR_ORDER>(leds2, NUM_LEDS2_APA102).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET2, LED3_PIN, CLOCK3_PIN, WS2801_COLOR_ORDER>(leds3, NUM_LEDS_WS2801).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
   random16_add_entropy( random());

  Fire2012a(); // run simulation frame
  Fire2012b(); // run simulation frame
  rainbow(0); // run WS2901 program
  
  FastLED.show(); // display this frame
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012a()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS1_APA102];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS1_APA102; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS1_APA102) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS1_APA102 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS1_APA102; j++) {
       color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS1_APA102-1) - j;
      } else {
        pixelnumber = j;
      }
      leds1[pixelnumber] = color;
    }
}

void Fire2012b()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS2_APA102];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS2_APA102; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS2_APA102) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS2_APA102 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS1_APA102; j++) {
       color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS2_APA102-1) - j;
      } else {
        pixelnumber = j;
      }
      leds2[pixelnumber] = color;
    }
}

void Fire2012c()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS_WS2801];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS_WS2801; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_WS2801) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS_WS2801 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS_WS2801; j++) {
       color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS_WS2801-1) - j;
      } else {
        pixelnumber = j;
      }
      leds3[pixelnumber] = color;
    }
}
void rainbow(uint8_t wait) 
{

 for( int i = 0; i < NUM_LEDS_WS2801; i++)
 {
    fill_solid( &(leds3[0]), NUM_LEDS_WS2801 /*led count*/, color /*starting hue*/);
     delay(wait); 
 }
   return;  
}
