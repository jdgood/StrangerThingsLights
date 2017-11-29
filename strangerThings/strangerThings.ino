#include <FastLED.h>
#include <SoftwareSerial.h>

#define DATA_PIN 7  //this is the data pin connected to the LED strip.  If using WS2801 you also need a clock pin
#define NUM_LEDS 100 //change this for the number of LEDs in the strip
#define COLOR_ORDER RGB

//The array of ledIndex->color used by the FastLED library
CRGB leds[NUM_LEDS];

//Array of colors to make them accessible via a number
CRGB colors[13];

//Map of char values a=0; z=25 used to create a range of lights per letter
int charToLedRange[26];

//Random messages to use when nothing is queued to be printed
String randomMessages[2];

//Pretty standard colors
CRGB white = CRGB(255,255,255);
CRGB red = CRGB (255, 0, 0);
CRGB green = CRGB(0, 255, 0);
CRGB blue = CRGB(0, 0, 255);

//Some random colors that seem to look pretty good with the theme
CRGB hotPink = CRGB (255,105,180);
CRGB darkGreen = CRGB(0,128,0);
CRGB darkPurple = CRGB(76, 0, 153);
CRGB royalBlue = CRGB(65,105,255);
CRGB orange = CRGB(255,165,0);
CRGB springGreen = CRGB(0,255,127);
CRGB yellow = CRGB(255,255,0);
CRGB darkOrchid = CRGB(153, 50, 204);
CRGB aqua = CRGB(0,255,255);

//Black aka light is off
CRGB off = CRGB(0, 0, 0);

//Sets up the initial values and libraries
void setup() {
	//Register our ledIndex->color array to FastLED
	FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

	//Create a seed for random number generator using a kind of random number
	randomSeed(analogRead(0));

	//Map for letters a=0, z=25 to max led value in range, i.e a=49 b=47 so range=[48,49]
	//There may be some customization to this depending on how many lights you'll have per a letter
	charToLedRange[0] = 49;//A
	charToLedRange[1] = 47;//B
	charToLedRange[2] = 45;//C
	charToLedRange[3] = 43;//D
	charToLedRange[4] = 41;//E
	charToLedRange[5] = 39;//F
	charToLedRange[6] = 37;//G
	charToLedRange[7] = 35;//H

	charToLedRange[8] = 18;//I
	charToLedRange[9] = 20;//J
	charToLedRange[10] = 22;//K
	charToLedRange[11] = 24;//L
	charToLedRange[12] = 26;//M
	charToLedRange[13] = 28;//N
	charToLedRange[14] = 30;//O
	charToLedRange[15] = 31;//P
	charToLedRange[16] = 32;//Q

	charToLedRange[17] = 15;//R
	charToLedRange[18] = 13;//S
	charToLedRange[19] = 11;//T
	charToLedRange[20] =  9;//U
	charToLedRange[21] =  7;//V
	charToLedRange[22] =  5;//W
	charToLedRange[23] =  3;//X
	charToLedRange[24] =  1;//Y
	charToLedRange[25] =  0;//Z

	//Some random colors that seem to look pretty good with the theme
	colors[0] = white;
	colors[1] = red;
	colors[2] = green;
	colors[3] = blue;
	colors[4] = darkGreen;
	colors[6] = darkPurple;
	colors[7] = royalBlue;
	colors[8] = orange;
	colors[9] = springGreen;
	colors[10] = yellow;
	colors[11] = darkOrchid;
	colors[12] = aqua;

	//Random messages for when nothing is queued to be printed
	randomMessages[0] = "upside down";
	randomMessages[1] = "happy friendsgiving";

	//Ensure we start clean
	setAll(off);

	//Demo modes to display are colors above via modulus and random algortihms
	ledsSequential();
	abcsRandom();

	//Set the data rate for the SoftwareSerial port to start listening for input
	Serial.begin(9600);

	//Send the initial prompt to the console
	Serial.print("Send a message from the upside down: ");
}

//Sets all leds to a certain color. This includes off, which is useful for resetting to a clean state
void setAll(CRGB color) {
	for (int x = 0; x < NUM_LEDS; x++) {
		leds[x] = color;
	}
	FastLED.show();
}

//Lights up all lights sequentially using a modulus to iterate through the colors
void ledsSequential() {
	for (int i = 0; i < 25; i++) {
		leds[i] = colors[i % 13];
		FastLED.show();
		delay(250);
	}
	setAll(off);
}

//Returns a random color from the array
CRGB randomColor() {
	int t = random(13);
	return colors[t];
}

//Print a-z sequentially using a random numbers to pick colors
void abcsRandom() {
	for (int i = 0; i < 25; i++) {
    displayLetter(i, 250, false);
	}
	setAll(off);
}

//The main program, which just infinitely loops
void loop() {
	//Print out the queued message, otherwise pring our random word and some random letters
	if(!typeQueued()) {
		wordRandomizer();
		letterRandomizer();
	}
}

//Type out if anyone has input anything, return false if nothing to print
boolean typeQueued() {
	if (Serial.available()) {
		String message = Serial.readString();
		Serial.println("Sending message");
		type(message);
		Serial.println("Message sent");
		Serial.print("Send a message from the upside down: ");
		return true;
	}
	return false;
}

//Type out a random message from our prepopulated array
void wordRandomizer() {
	int i = random(1);
	type(randomMessages[i]);
}

//Successively pick 6 random letters to illuminate, reset only after all 6 have been illuminated
void letterRandomizer() {
	for (int i = 0; i < 6; i++) {
		int id = random(25);
		displayLetter(id, 200, false);
	}
	setAll(off);
}

//Forces a range of 0-25 (a-z) for simple array mapping purposes
int charToInt(char c) {
	return (int)c - '0';
}

//Turns the lights on for a corresponding letter, can also set the delay between lights and if the lights should stay illuminated
void displayLetter(int charRepRangeStart, int timeDelay, boolean reset) {
	int ledStart = charToLedRange[charRepRangeStart];
	int ledEnd = (charRepRangeStart + 1) == 27 ? charToLedRange[charRepRangeStart + 1] : -1;

	for(int i = ledStart; i < ledEnd; i--) {
#pragma message(Reminder "TODO: Maybe set some lights that will always be off (mainly due to ones that are along the wrap around)"
		leds[i] = randomColor();
	}
	FastLED.show();
	delay(timeDelay);
	if(reset) {
		for(int i = ledStart; i < ledEnd; i--) {
			leds[i] = off;
		}
		FastLED.show();
	}
}

//Type out a message, while waiting until the previous letter's lights turns off after 2 seconds
void type(String message){
	message.toLowerCase();
	for(int i =0; i< message.length();i++){
		//Spaces will illuminate everything white (TODO: may want to use a dimmer white)
		if(isWhitespace(message[i])){
			setAll(white);
			delay(2000);
			setAll(off);
		} else {
			displayLetter(charToInt(message[i]), 2000, true);
		}
	}
#pragma message(Reminder "TODO: add a flicker effect here")
}
