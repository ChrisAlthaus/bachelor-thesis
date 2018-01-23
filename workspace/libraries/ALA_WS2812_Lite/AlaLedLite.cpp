#include "AlaLite.h"
#include "AlaLedLite.h"

#include "ExtNeoPixel.h"
#include "LightPatterns.h"

Adafruit_NeoPixel *neopixels;




AlaLedLite::AlaLedLite()
{
	// set default values

	maxOut = 0xFFFFFF;
	speed = 1000;
	animSeqLen = 0;
	lastRefreshTime = 0;
	refreshMillis = 10000/50;
	pxPos = NULL;
	pxSpeed = NULL;
}

void AlaLedLite::initStripe(int numLeds, byte pin, byte type) // CHANGE: SINGLE STRIPE
{
	this->driver = ALA_WS2812;
	this->numLeds = numLeds;
	this->pins = 0;
	this->sides = 1;
	this->numLedsPerSide = numLeds;

	// allocate and clear leds array
	leds = (AlaColor *)malloc(3*numLeds);
	memset(leds, 0, 3*numLeds);

	neopixels = new Adafruit_NeoPixel(numLeds, pin, type);

	neopixels->begin();
}

void AlaLedLite::initBarStripes(byte pin, byte type) // CHANGE: 4 sides
{
	this->driver = ALA_WS2812;
	this->numLedsPerSide = 11;
	this->sides = 4;

	this->numLeds = numLedsPerSide * sides; // CHANGE
	this->pins = 0;

	// allocate and clear leds array
	leds = (AlaColor *)malloc(3*numLeds);		//4 sides?
	memset(leds, 0, 3*numLeds);

	neopixels = new Adafruit_NeoPixel(numLeds, pin, type);

	neopixels->begin();
}

void AlaLedLite::initLightPatternIntervalls(){
	initIntervalls();
	
	
}

void AlaLedLite::printValues(){
	printArray(beaconSampleValues,NUMBER_SAMPLE_VALUES);
}


void AlaLedLite::setBrightness(AlaColor maxOut)
{
	this->maxOut = maxOut;
	run();
}

void AlaLedLite::setRefreshRate(int refreshRate)
{
	this->refreshMillis = 1000/refreshRate;
}

int AlaLedLite::getCurrentRefreshRate()
{
	return refreshRate;
}

//set animation, which run on all sides
void AlaLedLite::setAnimation(int animation, long speed, AlaColor color)
{
	// is there any change?
	if (this->animation == animation && this->speed == speed && this->palette.numColors == 1 && this->palette.colors[0] == color)
		return;

	// delete any previously allocated array
	if (pxPos!=NULL)
	{ delete[] pxPos; pxPos=NULL; }
	if (pxSpeed!=NULL)
	{ delete[] pxSpeed; pxSpeed=NULL; }

	this->animation = animation;  
	this->speed = speed;

	this->palette.numColors = 1;
	this->palette.colors = (AlaColor*)malloc(3);
	this->palette.colors[0] = color;

	setAnimationFunc(animation);
	animStartTime = millis();
}

//set animation, which run on all sides
void AlaLedLite::setAnimation(int animation, long speed, AlaPalette palette)
{
	// is there any change?
	if (this->animation == animation && this->speed == speed && this->palette == palette)
		return;

	// delete any previously allocated array
	if (pxPos!=NULL)
	{ delete[] pxPos; pxPos=NULL; }
	if (pxSpeed!=NULL)
	{ delete[] pxSpeed; pxSpeed=NULL; }

	this->animation = animation;
	this->speed = speed;
	this->palette = palette;

	setAnimationFunc(animation);
	animStartTime = millis();
}

//set animation, which run on single side with single led
void AlaLedLite::setAnimation(int animation, String side, int ledNumber, int durationOneCycle, AlaColor color)
{
	int sideNumber=getSideNumber(side);
	// is there any change?
	/*if (this->animation == animation && this->speed == speed && this->singleAnimationLedNumbers[sideNumber] == ledNumber 
		&& this->singleAnimationColors[sideNumber] == color && this->singleAnimationSides[sideNumber] == true
		&& this->durationOneCycle == durationOneCycle)return;
	*/	

	// delete any previously allocated array
	if (pxPos!=NULL)
	{ delete[] pxPos; pxPos=NULL; }
	if (pxSpeed!=NULL)
	{ delete[] pxSpeed; pxSpeed=NULL; }

	this->animation = animation;   //delete from animation array
	this->singleAnimationLedNumbers[getSideNumber(side)]= ledNumber;
	
	if(animation != ALA_SINGLEANIMATIONOFF){
		this->singleAnimationSides[getSideNumber(side)]=1;
        _customLighting=false;		
	}else{
		this->singleAnimationSides[getSideNumber(side)]=0;
		/*if(isLedOn(sideNumber,ledNumber)){
			setLed(side, ledNumber, ALA_OFF, AlaColor(0,0,0));
		}*/	
		bool animationsOff=true;
		for(int i=0; i<4;i++){
			if(singleAnimationSides[i]==1){
				animationsOff=false;
			}
		}
		if(animationsOff)_customLighting=true;
	}
	
	this->durationOneCycle=durationOneCycle;
	this->singleAnimationColors[getSideNumber(side)]=color;

	setAnimationFunc(animation);
	animStartTime = millis();
}


void AlaLedLite::setAnimation(AlaSeq animSeq[])
{
	this->animSeq = animSeq;

	// initialize animSeqDuration and animSeqLen variables
    animSeqDuration = 0;
    for(animSeqLen=0; animSeq[animSeqLen].animation!=ALA_ENDSEQ; animSeqLen++)
    {
		animSeqDuration = animSeqDuration + animSeq[animSeqLen].duration;
    }
}

void AlaLedLite::nextAnimation()
{
	currAnim = (currAnim+1)%animSeqLen;
}

bool AlaLedLite::runAnimation()
{
	if ( ! _customLighting) {

	    // skip the refresh if not enough time has passed since last update
		unsigned long cTime = millis();
		if (cTime < lastRefreshTime + refreshMillis)
			return false;

		// calculate real refresh rate
		refreshRate = 1000/(cTime - lastRefreshTime);

		lastRefreshTime = cTime;
		Serial.println("run animation");

		if (animSeqLen != 0)
	    {
			if(animSeq[currAnim].duration == 0)
			{
				setAnimation(animSeq[currAnim].animation, animSeq[currAnim].speed, animSeq[currAnim].palette);
			}
			else
			{
				long c = 0;
				long t = cTime % animSeqDuration;
				for(int i=0; i<animSeqLen; i++)
				{
					if (t>=c && t<(c+animSeq[i].duration))
					{
						setAnimation(animSeq[i].animation, animSeq[i].speed, animSeq[i].palette);
						break;
					}
					c = c + animSeq[i].duration;
				}
			}
	    }

	    if (animFunc != NULL)
	    {
			//Serial.println("run animation");
			(this->*animFunc)();

			// use an 8 bit shift to divide by 256

			if(driver==ALA_WS2812)
			{
				// this is not really so smart...
				for(int i=0; i<numLeds; i++)
					neopixels->setPixelColor(i, neopixels->Color((leds[i].r*maxOut.r)>>8, (leds[i].g*maxOut.g)>>8, (leds[i].b*maxOut.b)>>8));

				neopixels->show();
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

void AlaLedLite::setLed(String side, int led, int animation, AlaColor color) {

	int s = getSideNumber(side);

	int x= led-1;

	switch (animation) {
		case ALA_ON:
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = color; 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = color; 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = color; 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = color; 					break; 	// Side D
				default: break;
			}
			break;
		case ALA_OFF:
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = 0x000000; 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = 0x000000; 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = 0x000000; 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = 0x000000; 					break; 	// Side D
				default: break;
			}
			break;
		default: break;
	}

	run();
}



void AlaLedLite::setSide(String side, int animation, AlaColor color) {

	int s = getSideNumber(side);

	switch (animation) {
		case ALA_ON:
			for(int x=0; x < numLedsPerSide; x++) {
				switch (s) {
					case 0: leds[numLedsPerSide - (x+1)] = color; 				break;	// Side A
					case 1: leds[numLedsPerSide + x] = color; 						break; 	// Side B
					case 2: leds[3*numLedsPerSide - (x+1)] = color; 			break;	// Side C
					case 3: leds[3*numLedsPerSide + x] = color; 					break; 	// Side D
					default: break;
				}
			}
		break;
		case ALA_OFF:
			for(int x=0; x < numLedsPerSide; x++) {
				switch (s) {
					case 0: leds[numLedsPerSide - (x+1)] = 0x000000; 				break;	// Side A
					case 1: leds[numLedsPerSide + x] = 0x000000; 						break; 	// Side B
					case 2: leds[3*numLedsPerSide - (x+1)] = 0x000000; 			break;	// Side C
					case 3: leds[3*numLedsPerSide + x] = 0x000000; 					break; 	// Side D
					default: break;
				}
			}
			break;
		default: break;
	}


	run();
}


void AlaLedLite::run() {
	for(int i=0; i<numLeds; i++)
		neopixels->setPixelColor(i, neopixels->Color((leds[i].r*maxOut.r)>>8, (leds[i].g*maxOut.g)>>8, (leds[i].b*maxOut.b)>>8));

	neopixels->show();
}

int AlaLedLite::getSideNumber(String side) {

	if (side == "a" || side == "A") {
		return 0;
	} else if (side == "b" || side == "B") {
		return 1;
	} else if (side == "c" || side == "C") {
		return 2;
	} else if (side == "d" || side == "D") {
		return 3;
	}
}


///////////////////////////////////////////////////////////////////////////////

void AlaLedLite::setAnimationFunc(int animation)
{
	
    switch(animation)
	{
		case ALA_ON:                    animFunc = &AlaLedLite::on;                    break;
		case ALA_OFF:                   animFunc = &AlaLedLite::off;                   break;
		
		case ALA_SINGLEANIMATIONOFF:	animFunc = &AlaLedLite::ledAnimationOff;       break;
		
		case ALA_BLINK:					animFunc = &AlaLedLite::blinkLed;              break;
		case ALA_BEACON:				animFunc = &AlaLedLite::beaconLed;             break;
		case ALA_STAIRCASE:				animFunc = &AlaLedLite::staircaseLed;          break;
		case ALA_PULSESLOW:				animFunc = &AlaLedLite::pulseSlowLed;          break;
		case ALA_TRANSMISSONFB:			animFunc = &AlaLedLite::transmissionFixedBrightnessLed;          break;
		
		case ALA_PIXELSMOOTHSHIFTRIGHT: animFunc = &AlaLedLite::pixelSmoothShiftRight; break;
		case ALA_PIXELSMOOTHSHIFTLEFT:  animFunc = &AlaLedLite::pixelSmoothShiftLeft;  break;
		case ALA_PIXELSMOOTHBOUNCE:     animFunc = &AlaLedLite::pixelSmoothBounce;     break;
		case ALA_COMET:                 animFunc = &AlaLedLite::comet;                 break;

		case ALA_LARSONSCANNER:         animFunc = &AlaLedLite::larsonScanner;         break;

		case ALA_GLOW:                  animFunc = &AlaLedLite::glow;                  break;

		default:                        animFunc = &AlaLedLite::off;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
// Single Led Effects
////////////////////////////////////////////////////////////////////////////////////////////
void AlaLedLite::blinkLed()
{
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	if(blinkSampleValues[index]==1){
		setLedsOn(singleAnimationColors);
	}else if(blinkSampleValues[index]==0){
		setLedsOff();
	}
	
}

void AlaLedLite::beaconLed(){
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	if(beaconSampleValues[index]==1){
		setLedsOn(singleAnimationColors);
	}else if(beaconSampleValues[index]==0){
		setLedsOff();
	}

}

/*void AlaLedLite::staircaseLed(){		
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	if(beaconSampleValues[index]==0){
		setLed(singleAnimationSide, singleAnimationLed, ALA_OFF, new AlaColor(0,0,0));
	}else{
		int scaleFactor=beaconSampleValues[index];  //intervall [1,4]
		AlaColor color = singleAnimationColor.scale(scaleFactor);
	
	    setLed(singleAnimationSide, singleAnimationLed, ALA_ON, color);
	}
}*/

void AlaLedLite::staircaseLed(){		
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	if(staircaseSampleValues[index]==0){
		setLedsOff();
	}else{
		int scaleFactor=staircaseSampleValues[index];  //intervall [1,4]
		int brightness = map(scaleFactor,1,4,63,255);
		
		
		
		AlaColor colors[4];
		
									
	    for(int i=0;i<4;i++){
			if(singleAnimationSides[i]==true){
				colors[i]= AlaColor((singleAnimationColors[i].r*brightness)>>8,(singleAnimationColors[i].g*brightness)>>8,
										(singleAnimationColors[i].b*brightness)>>8);
				
				}
			}
	
		
		setLedsOn(colors);
	}
}


void AlaLedLite::pulseSlowLed(){
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	//float scaleFactor=map(pulseSlowSampleValues[index]+1, 0, 1, 0.1, 0.9);  //intervall [0.1,0.9]
	float scaleFactor=pulseSlowSampleValues[index]+1.1;  //intervall [0.1,0.9]
	//AlaColor color = singleAnimationColor.scale(scaleFactor);	
	
	//setLedsOn(singleAnimationColors);
}

/*void AlaLedLite::pulseSlowLed(){
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	int scaleBrightness=(pulseSlowSampleValues[index]+1)/2;
	int brightness= map(scaleBrightness,0,1,0,100);
	
    setLedOn(singleAnimationSide,singleAnimationLed);
	setBrightness(AlaColor(brightness, brightness, brightness));	
	
	
}*/

void AlaLedLite::transmissionFixedBrightnessLed(){
	unsigned long time= millis()-animStartTime;
	long duration = time%durationOneCycle;
	
	int index = map(duration, 0, durationOneCycle, 0, NUMBER_SAMPLE_VALUES);
	
	if(transmissionFBSampleValues[index]==1){
		setLedsOn(singleAnimationColors);
	}else if(transmissionFBSampleValues[index]==0){
		setLedsOff();
	}
}

void AlaLedLite::ledAnimationOff(){
	
}

void AlaLedLite::setLedsOn(AlaColor colors [4]){
	//AlaColor color = singleAnimationColor;
	//int side = getSideNumber(singleAnimationSide);
	
	for(int i=0;i<4;i++){
		if(singleAnimationSides[i]==true){
			int side = i;
			int numberLed = singleAnimationLedNumbers[i];
			switch (side) {
				case 0: leds[numLedsPerSide - numberLed] = colors[side]; 		break;	
				case 1: leds[numLedsPerSide + (numberLed -1)] = colors[side]; 			break; 	
				case 2: leds[3*numLedsPerSide - numberLed] = colors[side]; 			break;	
				case 3: leds[3*numLedsPerSide + (numberLed -1)] = colors[side]; 		break; 	
				default: break;
			}	
		}
	}
	
}

void AlaLedLite::setLedsOff(){
	for(int i=0;i<4;i++){
		if(singleAnimationSides[i]==true){
			int side = i;
			int numberLed = singleAnimationLedNumbers[i];
			switch (side) {
				case 0: leds[numLedsPerSide - numberLed] = 0x000000; 				break;
				case 1: leds[numLedsPerSide + (numberLed -1)] = 0x000000; 			break; 	
				case 2: leds[3*numLedsPerSide - numberLed] = 0x000000; 			break;	
				case 3: leds[3*numLedsPerSide + (numberLed -1)] = 0x000000; 		break; 	
				default: break;
			}
		}
	}
}	

boolean AlaLedLite::isLedOn(int side,int numberLed){
	AlaColor colorValue=0;
	
	switch (side) {
			case 0: colorValue=leds[numLedsPerSide - numberLed];  				break;
			case 1: colorValue=leds[numLedsPerSide + (numberLed -1)]; 			break; 	
			case 2: colorValue=leds[3*numLedsPerSide - numberLed]; 				break;	
			case 3: colorValue=leds[3*numLedsPerSide + (numberLed -1)]; 		break; 	
			default: break;
		}
	
	if(colorValue == 0x000000){			// color white->led off
		return false;
	}else{
		return true;
	}
}
	

////////////////////////////////////////////////////////////////////////////////////////////
// Animation for all bars
////////////////////////////////////////////////////////////////////////////////////////////

void AlaLedLite::on()
{
	for(int i=0; i<numLeds; i++)
	{
		leds[i] = palette.colors[0];
	}
}

void AlaLedLite::off()
{
	for(int i=0; i<numLeds; i++)
	{
		leds[i] = 0x000000;
	}
}


void AlaLedLite::blink()
{
	int t = getStep(animStartTime, speed, 2);
	int k = (t+1)%2;
	for(int x=0; x < numLeds; x++) // <-- CHANGE
	{
			leds[x] = palette.colors[0].scale(k);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Shifting effects
////////////////////////////////////////////////////////////////////////////////////////////


void AlaLedLite::pixelSmoothShiftRight() // CHANGE
{
	float t = getStepFloat(animStartTime, speed, numLeds+1);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x = 0; x < numLedsPerSide; x++)
	{
			float k = max(0, (-abs(t-1-x)+1));
			for (int s = 0; s < sides; s++) {
				switch (s) {
					case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
					case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
					case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
					case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
					default: break;
				}
			}
	}
}

void AlaLedLite::pixelSmoothShiftLeft()
{
	float t = getStepFloat(animStartTime, speed, numLeds+1);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = max(0, (-abs(numLeds-t-x)+1));
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}

void AlaLedLite::comet()
{
	float l = numLeds/2;  // length of the tail
	float t = getStepFloat(animStartTime, speed, 2*numLeds-l);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain( (((x-t)/l+1.2f))*(((x-t)<0)? 1:0), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}
void AlaLedLite::pixelSmoothBounce()
{
	// see larsonScanner
	float t = getStepFloat(animStartTime, speed, 2*numLeds-2);
	AlaColor c = palette.getPalColor(getStepFloat(animStartTime, speed, palette.numColors));

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain((-abs(abs(t-numLeds+1)-x)+1), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}


void AlaLedLite::larsonScanner()
{
	float l = numLeds/4;
	float t = getStepFloat(animStartTime, speed, 2*numLeds-2);
	AlaColor c = palette.getPalColor(getStepFloat(animStartTime, speed, palette.numColors));

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain((-abs(abs(t-numLeds+1)-x)+l), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}




////////////////////////////////////////////////////////////////////////////////////////////
// Fading effects
////////////////////////////////////////////////////////////////////////////////////////////

void AlaLedLite::glow()
{
	float s = getStepFloat(animStartTime, speed, TWO_PI);
	float k = (-cos(s)+1)/2;

	for(int x=0; x<numLedsPerSide; x++)
	{
		//leds[x] = palette.colors[0].scale(k);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = palette.colors[0].scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = palette.colors[0].scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = palette.colors[0].scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = palette.colors[0].scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}



void AlaLedLite::setCustomLighting(bool customLighting) {
	_customLighting = customLighting;
}
bool AlaLedLite::hasCustomLighting() {
	return _customLighting;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////////////////////

long map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
