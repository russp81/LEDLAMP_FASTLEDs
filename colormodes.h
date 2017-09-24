// ***************************************************************************
// Color modes
// ***************************************************************************
//#include "definitions.h"

#include "Arduino.h"

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB(glitter_color.red, glitter_color.green, glitter_color.blue);
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  if (GLITTER_ON == true){addGlitter(glitter_density);}
  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(500/FPS)));  
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, ftb_speed);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, brightness);
  if (GLITTER_ON == true){addGlitter(glitter_density);}  
  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));  
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, ftb_speed);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, brightness);
  if (GLITTER_ON == true){addGlitter(glitter_density);}
  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));    
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = FPS;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, brightness);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  if (GLITTER_ON == true){addGlitter(glitter_density);}

  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));
  
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, ftb_speed);
  int dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, brightness);
    dothue += 32;
  }
  if (GLITTER_ON == true){addGlitter(glitter_density);}

  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));  
}
//******************************************************************************************
//                     PALETTE ANIMATION FUNCTIONS
//******************************************************************************************
void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
 // uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = ColorFromPalette( currentPalette, colorIndex + sin8(i*16), brightness);
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness);
    if (anim_direction == "forward") {colorIndex += 3;}
    if (anim_direction == "back") {colorIndex -= 3;}
    
  }
}

void ChangePalettePeriodically(){
  
  if (millis() - paletteMillis > (show_length*1000)){
    paletteMillis = millis();

    randomSeed(analogRead(0));
    
    targetpicker = random(0,22);

    currentPalette = targetPalette;

    if (anim_direction == "forward"){
       anim_direction = "back";
       } else {
        anim_direction = "forward";
       }

        if(targetpicker == 0){ targetPalette = bhw1_29_gp;}

        if(targetpicker == 1){ targetPalette = bhw1_purplered_gp;}

        if(targetpicker == 2){ targetPalette = bhw1_sunconure_gp;}

        if(targetpicker == 3){ targetPalette = bhw1_purpgreen_gp;}

        if(targetpicker == 4){ targetPalette = prism_gp;}

        if(targetpicker == 5){ targetPalette = blue_waves_gp;}

        if(targetpicker == 6){ targetPalette = twilight_gp;}

        if(targetpicker == 7){ targetPalette = plasma_gp;}

        if(targetpicker == 8){ targetPalette = sls_gp;}

        if(targetpicker == 9){ targetPalette = lkmtch00_gp;}

        if(targetpicker == 10){ targetPalette = Lindaa07_gp;}

        if(targetpicker == 11){ targetPalette = bud2_gp;}

        if(targetpicker == 12){ targetPalette = patriot_gp;}

        if(targetpicker == 13){ targetPalette = brightsong2_gp;}

        if(targetpicker == 14){ targetPalette = goddess_moon_gp;}

        if(targetpicker == 15){ targetPalette = beading_gp;}

        if(targetpicker == 16){ targetPalette = sorcery_2_gp;}

        if(targetpicker == 17){ targetPalette = wintercolors_gp;}

        if(targetpicker == 18){ targetPalette = usa1_gp;}

        if(targetpicker == 19){ targetPalette = faewing3_gp;}

        if(targetpicker == 20){ targetPalette = starrynite_gp;}

        if(targetpicker == 21){ targetPalette = blueeyedgal_gp;}

        if(targetpicker == 22){ targetPalette = water1_gp;}
    
  }      
}
void palette_anims(){

  currentBlending = LINEARBLEND;
  
  ChangePalettePeriodically();
    
  uint8_t maxChanges = int(float(FPS/2)); 
  nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);

  static uint8_t startIndex = 0;
  
  /* motion speed */
  startIndex = startIndex + 3;
  
  FillLEDsFromPaletteColors(startIndex);
  
  if (GLITTER_ON == true){addGlitter(glitter_density);}

  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS))); 
}

//*****************LED RIPPLE*****************************************************

void one_color_allHSV(int ahue, int abright) {                // SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(ahue, 255, abright);
  }
}

int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
}

void ripple() {

    if (currentBg == nextBg) {
      nextBg = random(256);
    } 
    else if (nextBg > currentBg) {
      currentBg++;
    } else {
      currentBg--;
    }
    for(uint16_t l = 0; l < NUM_LEDS; l++) {
      leds[l] = CHSV(currentBg, 255, brightness);         // strip.setPixelColor(l, Wheel(currentBg, 0.1));
    }
 
  if (step == -1) {
    center = random(NUM_LEDS);
    color = random(256);
    step = 0;
  }
 
  if (step == 0) {
    leds[center] = CHSV(color, 255, brightness);         // strip.setPixelColor(center, Wheel(color, 1));
    step ++;
  } 
  else {
    if (step < maxSteps) {
      Serial.println(pow(fadeRate,step));

      leds[wrap(center + step)] = CHSV(color, 255, pow(fadeRate, step)*255);       //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
      leds[wrap(center - step)] = CHSV(color, 255, pow(fadeRate, step)*255);       //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
      if (step > 3) {
        leds[wrap(center + step - 3)] = CHSV(color, 255, pow(fadeRate, step - 2)*255);     //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
        leds[wrap(center - step + 3)] = CHSV(color, 255, pow(fadeRate, step - 2)*255);     //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
      }
      step ++;
    } 
    else {
      step = -1;
    }
  }
  if (GLITTER_ON == true){addGlitter(glitter_density);}  
  
  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));
}

//***************************END LED RIPPLE*****************************************************

void comet(){

  fadeToBlackBy(leds, NUM_LEDS, ftb_speed);
  lead_dot = beatsin16(int(float(FPS/3)), 0, NUM_LEDS);
  leds[lead_dot] = CHSV(dothue, 200, 255);  
  dothue += 8;
  if (GLITTER_ON == true){addGlitter(glitter_density);}
  FastLED.show();
}

// Theatre-style crawling lights.
void theaterChase() {
  for (int q = 0; q < 3; q++) {
    for (int i = 0; i < NUM_LEDS; i = i + 3) {
      leds[i+q] = CRGB(main_color.red,main_color.green,main_color.blue);  //turn every third pixel on
    }
    FastLED.show();

    FastLED.delay(int(float(1000/FPS)));
    
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        leds[i+q] = CRGB(0,0,0);      //turn every third pixel off
        if (GLITTER_ON == true){addGlitter(glitter_density);}      
      }
    }
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
// Default 55, suggested range 20-100 
#define COOLING  80

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 60

bool gReverseDirection = false;


void fire2012()
{
  
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      //heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2)); // original with COOLING
      heat[i] = qsub8( heat[i],  random8(0, ((ftb_speed * 20) / NUM_LEDS) + 2)); // modified with FTBspeed
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    // if( random8() < SPARKING ) { // Original with SPARKING
    if( random8() < show_length ) { // Modified with show_length
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
}

//frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));
    
}


// Fireworks from WS2812FX

boolean _singlecolor = false; // used for single color mode
boolean _rainbow = false; // used for rainbow mode

void fireworks() {



// fadeToBlackBy( leds, NUM_LEDS, ftb_speed);

 
  uint32_t px_rgb = 0;
  byte px_r = 0;
  byte px_g = 0;
  byte px_b = 0;
  byte px_boost = 200;

  for(uint16_t i=0; i < NUM_LEDS; i++) {

    

    //leds[i] /= 2; // fade out (divide by 2)
    leds[i].nscale8(130 - int(float(ftb_speed*0.5)));
    //leds[i].fadeToBlackBy(ftb_speed);

  }

  // first LED has only one neighbour
  leds[0].r = (leds[1].r >> 1) + leds[0].r;
  leds[0].g = (leds[1].g >> 1) + leds[0].g;
  leds[0].b = (leds[1].b >> 1) + leds[0].b;
  //leds[0].setRGB(px_r, px_g, px_b);

// set brightness(i) = ((brightness(i-1)/2 + brightness(i+1)) / 2) + brightness(i)
    for(uint16_t i=1; i < NUM_LEDS-1; i++) {
    leds[i].r = ((
            (leds[i-1].r  >> 1) +
            leds[i+1].r ) >> 1) +
            leds[i].r;

    leds[i].g = ((
            (leds[i-1].g >> 1) +
            leds[i+1].g ) >> 1) +
            leds[i].g;

    leds[i].b = ((
            (leds[i-1].b   >> 1) +
            leds[i+1].b ) >> 1) +
            leds[i].b;
  }

  // last LED has only one neighbour
  leds[NUM_LEDS-1].r = ((leds[NUM_LEDS-2].r >> 2) + leds[NUM_LEDS-1].r);
  leds[NUM_LEDS-1].g = ((leds[NUM_LEDS-2].g >> 2) + leds[NUM_LEDS-1].g);
  leds[NUM_LEDS-1].b = ((leds[NUM_LEDS-2].b >> 2) + leds[NUM_LEDS-1].b);

px_r = random8();
px_g = random8();
px_b = random8();


  
    for(uint16_t i=0; i<_max(1,NUM_LEDS/20); i++) {
      if(random8(show_length + 4) == 0) {
        //Adafruit_NeoPixel::setPixelColor(random(_led_count), _mode_color);
        byte pixel = random(NUM_LEDS);
        if(_singlecolor){
          leds[pixel] = CRGB(main_color.red,main_color.green,main_color.blue); // tails are in single color from set color on web interface
        } else if(_rainbow) {
          leds[pixel] = CHSV( gHue, 255, brightness); // Rainbow cycling color  
        } else if(!_singlecolor && !_rainbow) {
          leds[pixel].setRGB(px_r, px_g, px_b); // Multicolored tale
          
        }
        leds[pixel].maximizeBrightness();          
      }
    }
  
    _singlecolor = false;
    _rainbow = false;

  if (GLITTER_ON == true){addGlitter(glitter_density);}


  //frame has been created, now show it
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(int(float(1000/FPS)));
}

void fw_single() {
  _singlecolor = true;
  fireworks();
}

void fw_rainbow() {
  _rainbow = true;
  fireworks();
}

// END




//*******************************ARRAY OF SHOW ANIMATIONS FOR MIXED SHOW MODE***********************
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle, bpm,
                                palette_anims, ripple, comet };
//**************************************************************************************************                                
