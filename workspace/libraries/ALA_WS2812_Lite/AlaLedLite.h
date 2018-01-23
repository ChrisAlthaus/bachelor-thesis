#ifndef AlaLedLite_h
#define AlaLedLite_h

#include "AlaLite.h"



/**
 *  AlaLedRgb can be used to drive a single or multiple RGB leds to perform animations.
 *  Available drivers are PWM pin, TLC5940, WS2811.
 */
class AlaLedLite
{

public:

	AlaLedLite();

    /**
    * Initializes WS2812 LEDs. It be invoked in the setup() function of the main Arduino sketch.
    *
    * The type field can be used to set the RGB order and chipset frequency. Constants are Adafruit_NeoPixel.h file.
    * It is set by default to NEO_GRB + NEO_KHZ800.
    */
	void initStripe(int numLeds, byte pin, byte type=0x01+0x02);
	void initBarStripes(byte pin, byte type=0x01+0x02);
	
	void initLightPatternIntervalls();
	void printValues();
	/**
	* Sets the maximum brightness level.
	*/
	void setBrightness(AlaColor maxOut);

	/**
	* Sets the maximum refresh rate in Hz (default value is 50 Hz).
	* May be useful to reduce flickering in some cases.
	*/
	void setRefreshRate(int refreshRate);

	int getCurrentRefreshRate();


	void setAnimation(int animation, long speed, AlaColor color);
	void setAnimation(int animation, long speed, AlaPalette palette);
	void setAnimation(AlaSeq animSeq[]);
	void setAnimation(int animation, String side, int ledNumber, int durationOneCycle, AlaColor color); //animation for single led
	
	bool runAnimation();	//query if animation is chosen->_customLighting
	void nextAnimation();		//delete?

	void setLed(String side, int led, int animation, AlaColor color);		//set single led on/off (ALA_ON/ALA_OFF)
	void setSide(String side, int animation, AlaColor color);

	void setCustomLighting(bool customLighting);
	bool hasCustomLighting();
private:

	void setAnimationFunc(int animation);
    
	void blinkLed();
	void beaconLed();
	void staircaseLed();
	void pulseSlowLed();
	void transmissionFixedBrightnessLed();
	void ledAnimationOff();
	
	void setLedsOn(AlaColor colors[]);
	void setLedsOff();
	
	boolean isLedOn(int side,int numberLed);
	
	void on();
    void off();
	void blink();


	void pixelSmoothShiftRight();
    void pixelSmoothShiftLeft();
    void comet();

	void pixelSmoothBounce();
	void larsonScanner();


    void glow();

		int getSideNumber(String side); // 'A'->0, 'B'->1, 'C'->2, 'D'->3
		void run();

	byte driver;    // type of led driver: ALA_PWM, ALA_TLC5940
	byte *pins;     // pins where the leds are attached to
	AlaColor *leds; // array to store leds brightness values
	int numLeds;    // number of leds
	int numLedsPerSide; // number of leds per bar side e.g. 4x 11 LEDs
  int sides;			// number of bars

	int animation;
	int currAnim;
    long speed;
	AlaPalette palette;
    AlaSeq *animSeq;
    int animSeqLen;
	long animSeqDuration;

    void (AlaLedLite::*animFunc)();
    AlaColor maxOut;
	int refreshMillis;
	int refreshRate;   // current refresh rate
	unsigned long animStartTime;
	unsigned long lastRefreshTime;

	float *pxPos;
	float *pxSpeed;

	boolean _customLighting;
	
	//int singleAnimationLed;
	//String singleAnimationSide;
	//AlaColor singleAnimationColor;
	int timeDelayOn; //in milliseconds
	int timeDelayOff; //in milliseconds
	int durationOneCycle;
	
	boolean singleAnimationSides[4];
	int singleAnimationLedNumbers[4];
	AlaColor singleAnimationColors[4];
	boolean ledOn;	
};


#endif
