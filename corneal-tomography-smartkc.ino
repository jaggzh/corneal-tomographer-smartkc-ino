/* by jaggz.h {who is at} gmail dot com
   GNU 3.0 Copyright.
   Use with SmartKC project from Microsoft or my FDM-3d-printable model
   MS's: https://github.com/microsoft/SmartKC-A-Smartphone-based-Corneal-Topographer

   I use 11x ws2812b LEDs from a strip.
   The "pro micro 16" has a 500ma fuse between vusb and the vcc pin,
   (I'm not sure what my blue clone version has).
   Nevertheless, it's powerful enough for the brightnesses I used
   below in the led_brightness array.
   Notes:
     I disabled the last 3 pixels (0,0,0 brightness) because they
    were at the center, and it turns out they made spot that was
    too bright (so 9 pixels would have been sufficient).
    SmartKC docs point out they found 10 was a good count.
*/
#include <Adafruit_NeoPixel.h>

#define PIN         9
//#define ACTPIXELS   10
#define TOTPIXELS	13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(TOTPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayms = 20;

int led_brightness[] =
	{200,160,200,160,200,160,200,160,200,160,120,0}; // cornea
	//{160,130,160,130,160,130,160,130,160,130,100,0}; // for cornea

// Actual actively-used LEDs in pattern
// (Tests might use TOTPIXELS)
#define ACT_LED_CNT ((sizeof(led_brightness) / sizeof(*led_brightness)))
#define ACT_LED_MAX (((sizeof(led_brightness) / sizeof(*led_brightness)))-1)

void clearall() {
	for (int i = 0; i < TOTPIXELS; i++) {
		pixels.setPixelColor(i, pixels.Color(0, 0, 0));
	}
	pixels.show();
}

void setup() {
	pixels.begin();
	//randomSeed(analogRead(0));
	clearall();
	for (int i=0; i<20; i++) {
		set_random_pattern();
		delay(7);
	}
	delay(500);
	show_sequential();
	delay(delayms);
	set_active_on();
	Serial.begin(115200);
}

void led_all_off() {
	for (unsigned int i = 0; i<TOTPIXELS; i++) {
		pixels.setPixelColor(i, pixels.Color(0, 0, 0));
	}
	pixels.show();
}

void show_sequential() {
	led_all_off();
	delay(500);
	for (unsigned int i = 0; i<TOTPIXELS; i++) {
		// 40-100 (13)
		// 40 + ((100-40) / ACT_LED_CNT)*i
		pixels.setPixelColor(i, pixels.Color(
			map(i, 0, TOTPIXELS-1, 0, 60),
			0,
			map(i, 0, TOTPIXELS-1, 20, 60)
			));
		pixels.show();
		delay(200);
		/* if (i>0) pixels.setPixelColor(i-1, pixels.Color(0, 0, 0)); */
	}
	delay(500);
}

void set_active_on() {
	Serial.println("Setting active blue LEDs");
	led_all_off();
	for (unsigned int i = 0; i<(sizeof(led_brightness) / sizeof(*led_brightness)); i++) {
		int b = led_brightness[i];
		int r = b;
		int g = b;
		pixels.setPixelColor(i, pixels.Color(r, g, b));
		pixels.show();
		delay(delayms);
	}
}

void set_random_pattern() {
	Serial.println("Setting random pattern");
	int maxbright=25;
	for (int i = 0; i<TOTPIXELS; i++) {
		int b = random(maxbright);
		int r = random(maxbright-b);
		int g = random(maxbright-b-r);
		pixels.setPixelColor(i, pixels.Color(r, g, b));
		pixels.show();
		delay(delayms);
	}
}

void loop() {
	//static int r=0, g=0, b=20;
	Serial.println("Loop");
}
