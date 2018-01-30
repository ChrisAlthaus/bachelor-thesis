#include "Arduino.h"
#include "Stepper-MotorControl.h"
#include "StepperControlZeroCpu.h"

MotorControl::MotorControl() {
	_stepPin = STEP_PIN;
	_directionPin = DIRECTION_PIN;
	_enablePin = ENABLE_PIN;

	_ms1Pin = MS1_PIN;
	_ms2Pin = MS2_PIN;
	_ms3Pin = MS3_PIN;

	_stepsPerRound = STEPS_PER_ROUND;
	_stepDelay = STEP_DELAY;

	_button1Pin = BUTTON1_PIN;
	_button2Pin = BUTTON2_PIN;
	_irPin = IR_PIN;

	_minPosition = MIN_POSITION;
	_maxPosition = MAX_POSITION;
	_checkupSteps = CHECKUP_STEPS;

	_directionChangeDelay = 10; // ms

	_steps = 0L;
	_position = 0;

	initPinModes();

	setStepMode(HALF_STEP);

	_isCalibrated = false;

	disable();
}

MotorControl::MotorControl(
	uint8_t stepPin,
	uint8_t directionPin,
	uint8_t enablePin,
	uint8_t ms1Pin,
	uint8_t ms2Pin,
	uint8_t ms3Pin,
	unsigned int stepsPerRound,
	unsigned int stepDelay,
	uint8_t irPin,
	uint8_t button1Pin,
	uint8_t button2Pin,
	unsigned int minPosition,
	unsigned int maxPosition,
	unsigned int checkupSteps
)
{
	_stepPin = stepPin;
	_directionPin = directionPin;
	_enablePin = enablePin;

	_ms1Pin = ms1Pin;
	_ms2Pin = ms2Pin;
	_ms3Pin = ms3Pin;

	_irPin = irPin;
	_button1Pin = button1Pin;
	_button2Pin = button2Pin;

	_steps = 0L;
	_position = 0;
	_minPosition = minPosition;
	_maxPosition = maxPosition;
	_checkupSteps = checkupSteps;

	_stepsPerRound = stepsPerRound;
	_stepDelay = stepDelay;
	_directionChangeDelay = 10; // ms  

	initPinModes();

	setStepMode(HALF_STEP);

	_isCalibrated = false;

	disable();
}

/*
 * MS PINs Methods
 * Step resolution
 */
void MotorControl::setStepMode(unsigned int mode)
{
	switch(mode) {
      case FULL_STEP: setFullStepMode(); break;
      case HALF_STEP: setHalfStepMode(); break;
      case QUARTER_STEP: setQuarterStepMode(); break;
      case EIGTHTH_STEP: setEigThthStepMode(); break;
      //case SIXTEENTH_STEP: setSixteenthStepMode(); break;

      default:
        setFullStepMode();
    }
}

void MotorControl::doStep()
{
	digitalWrite(_stepPin,HIGH);
	delayMicroseconds(_stepDelay);
	digitalWrite(_stepPin,LOW);
	delayMicroseconds(_stepDelay);
}

void MotorControl::doSteps(unsigned int number)
{
	if( number != 0 ){
		startPulses(number);
	}
	
}

void MotorControl::enable()
{
	digitalWrite(_enablePin,LOW);
	_isEnabled = true;
}

void MotorControl::disable()
{
	digitalWrite(_enablePin,HIGH);
	_isEnabled = false;
}

void MotorControl::up()
{
	_moveUp = true;
	//disable();
	digitalWrite(_directionPin,LOW);
	delay(_directionChangeDelay);
	//enable();
	//delay(_directionChangeDelay);
}

void MotorControl::down()
{
	_moveUp = false;
	//disable();
	digitalWrite(_directionPin,HIGH);
	delay(_directionChangeDelay);
	//enable();
	//delay(_directionChangeDelay);
}

void MotorControl::changeDirection() {
	if (_moveUp) {
		down();
	} else {
		up();
	}
}

void MotorControl::calibrate() {

	// calibrated
  if (_isCalibrated && getPosition() == 0 && getSteps() == 0) return;

  setHalfStepMode(); delay(20);

  // move up
  int stepsUpwards = 400;
  moveUp(stepsUpwards); delay(1000);

  // move down and calibrate
  int stepResolutionDownwardsSlow = 20;
  int stepResolutionDownwardsFast = 400;
  int tempMAX_POSITION = _maxPosition; //TODO

	// 1) Fast down
  for (int i = 0; i < tempMAX_POSITION/stepResolutionDownwardsFast; i++) {

    if (isIRActive()) {
      break; // -> 2)
    }
    moveDown(stepResolutionDownwardsFast);

    if (hasBottomReached()) {
      setPosition( scaleStepsToPosition(_minPosition, EIGTHTH_STEP) );
      resetSteps();
      return; // fast calibrated

    }
  }

	// 2) Slow down
  for (int i = 0; i < tempMAX_POSITION/stepResolutionDownwardsSlow; i++) {

    moveDown(stepResolutionDownwardsSlow);

    if (hasBottomReached()) { // check position after "stepResolutionDownwards" steps
      setPosition( scaleStepsToPosition(_minPosition, EIGTHTH_STEP) );
      resetSteps();
      break; // calibration complete
    }
  }

}






void MotorControl::moveTo(unsigned int position) {
	moveTo(position, HALF_STEP);
}

void MotorControl::moveTo(unsigned int position, unsigned int stepMode) {

	setStepMode(stepMode);

	unsigned int currentPosition = getPosition();
	unsigned int targetPosition = position;

  if (targetPosition > currentPosition) {
    int difference = targetPosition - currentPosition;
    moveUp( scalePositionToStep(difference, stepMode) );

  } else if (targetPosition < currentPosition) {
    int difference = currentPosition - targetPosition;
  	moveDown( scalePositionToStep(difference, stepMode) );

  }

}

void MotorControl::moveUp(unsigned int steps) {

	//enable();

  if (! hasCheckup()) {

    up();
    //delay(100);
		int temp=getPosition() + scaleStepsToPosition(steps, getStepMode());

		if ( getPosition() + scaleStepsToPosition(steps, getStepMode()) <= 100 ) {

		doSteps(steps);
		
	    changePosition( + scaleStepsToPosition(steps, getStepMode() ) );
	    addSteps(steps);

		} else {
			steps = scalePositionToStep(100 - getPosition(), getStepMode());
		
			doSteps(steps);
			Serial.println("Move up");
			
			setPosition(100);
			addSteps(steps);

		}

  } else { // CHECKUP
    int targetPosition = getPosition() + scaleStepsToPosition(steps, getStepMode() ); // up

    doCheckUp(targetPosition); // checkup + move to initial target position

  }

	//disable();
}

void MotorControl::moveDown(unsigned int steps) {

	//enable();

	if (! hasCheckup()) {

    down();
    //delay(100);

		if (isIRActive() & hasBottomReached()) {

			setPosition(0);
			resetSteps();

		} else {

			doSteps(steps);
			
			/*for (int i = 0; i < steps; i++) {

			  doStep();

					if (i % 50 == 0) { // check every 50 steps
							if (isIRActive() & hasBottomReached()) {
								setPosition(0);
								resetSteps();
								break;
							}
					}
			}*/

			changePosition( - scaleStepsToPosition(steps, getStepMode() ) );
			addSteps(steps, getStepMode());

		}

	} else { // CHECKUP

		int targetPosition = getPosition() - scaleStepsToPosition(steps, getStepMode() ); // down

		doCheckUp(targetPosition); // checkup + move to initial target position

	}

	//disable();
}


unsigned long MotorControl::scalePositionToStep(unsigned int targetPosition, unsigned int stepMode ) {

  switch(stepMode) {
    case FULL_STEP: return map(targetPosition, 0, 100, getMinPosition()/8, getMaxPosition()/8 );
    case HALF_STEP: return map(targetPosition, 0, 100, getMinPosition()/4, getMaxPosition()/4 );
    case QUARTER_STEP: return map(targetPosition, 0, 100, getMinPosition()/2, getMaxPosition()/2 );
    case EIGTHTH_STEP: return map(targetPosition, 0, 100, getMinPosition(), getMaxPosition() );
  }

	return 0L;

}


void MotorControl::doCheckUp(unsigned int targetPosition) {
	Serial.println("DO CHECKUP");
  resetSteps();
  calibrate();

  moveTo(targetPosition);
}




boolean MotorControl::hasBottomReached() {

  int minValue = 100; // button inactive = ~0
  int maxValue = 700; // button active = ~1023

  return ( getButton1Value()  >= maxValue); // button active

}

unsigned int MotorControl::getButton1Value() {
  return analogRead(_button1Pin);
}

unsigned int MotorControl::getButton2Value() {
  return analogRead(_button2Pin);
}

unsigned int MotorControl::getIRValue() {
  return analogRead(_irPin);
}

boolean MotorControl::isIRActive() {

  int minValue = 200; // button inactive = ~180
  int maxValue = 800; // button active = ~1023

  return (getIRValue() <= minValue); // ir active

}




void MotorControl::setPosition(unsigned int percent) {
  _position = percent;
}

unsigned long MotorControl::scaleSteps(unsigned long steps) {
	return scaleSteps( steps, getStepMode() );
}

unsigned long MotorControl::scaleSteps(unsigned long steps, unsigned int stepMode) {

	switch (stepMode) {
		case FULL_STEP: return steps * 8;
		case HALF_STEP: return steps * 4;
		case QUARTER_STEP: return steps * 2;
		case EIGTHTH_STEP: return steps * 1;

		default: return steps * 4;
	}

}


unsigned int MotorControl::scaleStepsToPosition(unsigned long steps, unsigned int stepMode) {

	switch (stepMode) {
		case FULL_STEP: return map(steps * 8, getMinPosition(), getMaxPosition(), 0, 100);
		case HALF_STEP: return map(steps * 4, getMinPosition(), getMaxPosition(), 0, 100);
		case QUARTER_STEP: return map(steps * 2, getMinPosition(), getMaxPosition(), 0, 100);
		case EIGTHTH_STEP: return map(steps * 1, getMinPosition(), getMaxPosition(), 0, 100);
	}

	return 0;
}

void MotorControl::changePosition(int positionIncrement) {
		_position = _position + positionIncrement;
}
int MotorControl::getPosition() {
  return _position;
}
void MotorControl::addSteps(unsigned int steps) {
	addSteps( steps, getStepMode() );
}
void MotorControl::addSteps(unsigned int steps, unsigned int stepMode) {

	_steps = _steps + scaleSteps(steps, stepMode);

}
void MotorControl::resetSteps() {
  _steps = 0;
}
unsigned long MotorControl::getSteps() {
  return _steps;
}
boolean MotorControl::hasCheckup() {

  return (getSteps() >= _checkupSteps); // calibrate after each CHECKUP_STEPS
}
/* -------------------------------------------------------------------------------------
 * Setter & Getter
 */

unsigned long MotorControl::getCheckUpSteps() {
		return _checkupSteps;
}

unsigned int MotorControl::getStepMode() {
	return _stepMode;
}

boolean MotorControl::isEnabled()
{
	return _isEnabled;
}

void MotorControl::setStepsPerRound(double stepAngle)
{
	_stepsPerRound = (360 / stepAngle);
}

void MotorControl::setStepDelay(unsigned int delayMicroSeconds)
{
	_stepDelay = delayMicroSeconds;
}

void MotorControl::setMinPosition(unsigned int position) {
	_minPosition = position;
}

void MotorControl::setMaxPosition(unsigned int position) {
	_maxPosition = position;
}

void MotorControl::setIRPin(unsigned int pin) {
	_irPin = pin;
}

void MotorControl::setButton1Pin(unsigned int pin) {
	_button1Pin = pin;
}

void MotorControl::setButton2Pin(unsigned int pin) {
	_button2Pin = pin;
}

unsigned int MotorControl::getMinPosition() {
	return _minPosition;
}

unsigned int MotorControl::getMaxPosition() {
	return _maxPosition;
}

/* -------------------------------------------------------------------------------------
 *
 */

void MotorControl::initPinModes()
{
	/*
	// Digital Pins
	if (_stepPin >= 2 && _stepPin <= 13) { pinMode(_stepPin,OUTPUT); digitalWrite(_stepPin,LOW); }
	if (_directionPin >= 2 && _directionPin <= 13) { pinMode(_directionPin,OUTPUT); digitalWrite(_directionPin,LOW); }
	if (_enablePin >= 2 && _enablePin <= 13) { pinMode(_enablePin,OUTPUT); digitalWrite(_enablePin,LOW); }
	if (_ms1Pin >= 2 && _ms1Pin <= 13) { pinMode(_ms1Pin,OUTPUT); digitalWrite(_ms1Pin,LOW); }
	if (_ms2Pin >= 2 && _ms2Pin <= 13) { pinMode(_ms2Pin,OUTPUT); digitalWrite(_ms2Pin,LOW); }
	if (_ms3Pin >= 2 && _ms3Pin <= 13) { pinMode(_ms3Pin,OUTPUT); digitalWrite(_ms3Pin,LOW); }
	//TODO OneWire Pin
	
	*/
	// Analog Pins
	if (_button1Pin >= 0 && _button1Pin <= 7) { pinMode(_button1Pin,INPUT); }
	if (_button2Pin >= 0 && _button2Pin <= 7) { pinMode(_button2Pin,INPUT); }
	if (_irPin >= 0 && _irPin <= 7) { pinMode(_irPin,INPUT); }
	
}

void MotorControl::setFullStepMode()
{
	digitalWrite(_ms1Pin,LOW);
	digitalWrite(_ms2Pin,LOW);
	digitalWrite(_ms3Pin,LOW);

	_stepMode = FULL_STEP;
}

void MotorControl::setHalfStepMode()
{
	digitalWrite(_ms1Pin,HIGH);
	digitalWrite(_ms2Pin,LOW);
	digitalWrite(_ms3Pin,LOW);

	_stepMode = HALF_STEP;
}

void MotorControl::setQuarterStepMode()
{
	digitalWrite(_ms1Pin,LOW);
	digitalWrite(_ms2Pin,HIGH);
	digitalWrite(_ms3Pin,LOW);
	
	Serial.println("Quarter Step Mode");

	_stepMode = QUARTER_STEP;
}

void MotorControl::setEigThthStepMode()
{
	digitalWrite(_ms1Pin,HIGH);
	digitalWrite(_ms2Pin,HIGH);
	digitalWrite(_ms3Pin,LOW);
	
	Serial.println("Eigthth Step Mode");

	_stepMode = EIGTHTH_STEP;
}

// void MotorControl::setSixteenthStepMode()
// {
// 	digitalWrite(_ms1Pin,HIGH);
// 	digitalWrite(_ms2Pin,HIGH);
// 	digitalWrite(_ms3Pin,HIGH);
// }
