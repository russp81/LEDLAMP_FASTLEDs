# doctormord's FASTLEDs
I mixed the work of McLighting and Russell with FastLED (FastLED library 3.1.3 as of this writing), the colorjs colorpicker, color spectrums created via FastLED Palette Knife, and some additional strip animations (included in the Arduino Sketch above).

FastLED 3.1.3 library:
https://github.com/FastLED/FastLED

McLighting library:
https://github.com/toblum/McLighting

Russel's implementation:
https://github.com/russp81/LEDLAMP_FASTLEDs

jscolor Color Picker:
http://jscolor.com/

FastLED Palette Knife:
http://fastled.io/tools/paletteknife/

If you aren't familiar with how to setup your ESP8266, see the readme on McLighting's git.  It's well written and should get you up and running.

In short you will:

1.  Configure the Arduino IDE to communicate with the ESP8266
2.  Upload the sketch (from this repo) The sketch is setup for a 240 pixel WS2812B GRB LED Strip.   
    (change the applicable options in "definitions.h" to your desire)
3.  On first launch, the ESP8266 will advertise it's own WiFi network for you to connect to, once you connect to it, launch your browser
    and the web interface is self explanatory.  (If the interface doesn't load, type in "192.168.4.1" into your browser and hit go)
4.  Once the ESP is on your wifi network, you can then upload the required files for the web interface by typing the in IP address
    of the ESP followed by "/edit" (i.e. 192.168.1.20/edit).  Then upload the files from the folder labeled "upload these" from this         repo. 
5.  Once you have finished uploading, type in the IP of the ESP into your browser and you should be up and running!

Forked from Russel,

i removed Adafruit Neopixel references and library calls.

Added/changed so far:

* new effect: Fire (from WS2812FX)
* new effect: Fireworks [single color, rainbow, random] (from McLightning, ported to used FastLED instead off Adafruit Neopixel)
* new settings for effects in webinterface *.htm 
* integrated Arduino OTA 


-I edited clockless_esp8266.h (in the FastLED platforms folder) and 
kept getting flickering until I incremented the WAIT_TIME up to 18us. 
(also I did "#define FASTLED_INTERRUPT_RETRY_COUNT 3" inside my sketch).-

For reference, interrupts issue:  https://github.com/FastLED/FastLED/issues/306
