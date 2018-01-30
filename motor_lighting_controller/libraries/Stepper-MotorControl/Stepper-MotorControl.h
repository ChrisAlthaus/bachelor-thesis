#ifndef Stepper_MotorControl_h
#define Stepper_MotorControl_h

#include "Arduino.h"
#include "Stepper-MotorControl.h"

/* -------------------------------------------------------------------------------------
 *
 */

#define FULL_STEP 			1
#define HALF_STEP			2
#define QUARTER_STEP		3
#define EIGTHTH_STEP		4
//#define SIXTEENTH_STEP	101

/* -------------------------------------------------------------------------------------
 *
 */


#define STEP_PIN        3 //OC2B, step for motor/A4988
#define DIRECTION_PIN   A5 //arbitrary
#define ENABLE_PIN      9 // OC1A
#define MS1_PIN         7 // arbitrary better 7 
#define MS2_PIN         6 // arbitrary better 6
#define MS3_PIN 0 // not used
#define PULSE_COUNTER_PIN 5 // T1 (clocksource for timer1),counts pulses of STEP_PIN

#define STEPS_PER_ROUND 200
#define STEP_DELAY      500

#define IR_PIN        A2

#define BUTTON1_PIN   A1
#define BUTTON2_PIN   A0

#define MIN_POSITION  0
#define MAX_POSITION  15600    // FULL: 2000, HALF: 4000, QUARTER: 8000, EIGTHTH: 16000
#define CHECKUP_STEPS 1560000   // calibrate each 1.560.000 steps  100 full movements

/* -------------------------------------------------------------------------------------
 *
 */

class MotorControl
{
	public:
		MotorControl();
		MotorControl(
			uint8_t stepPin,
			uint8_t directionPin,
			uint8_t enablePin,
			uint8_t ms1Pin,
			uint8_t ms2Pin,
			uint8_t ms3Pin,
			unsigned int stepsPerRound,
			unsigned int stepDelay,
			uint8_t _irPin,
			uint8_t _button1Pin,
			uint8_t _button2Pin,
			unsigned int _minPosition,
			unsigned int _maxPosition,
			unsigned int _checkupSteps
		);
		void setStepMode(unsigned int mode);
		void doStep();
		void doSteps(unsigned int number);
		void enable();
		void disable();
		boolean isEnabled();
		void up();
		void down();
		void changeDirection();
		void setStepsPerRound(double stepAngle);
		void setStepDelay(unsigned int delayMicroSeconds);
		void setMinPosition(unsigned int position);
		void setMaxPosition(unsigned int position);
		void setCheckUp(unsigned int steps);
		void calibrate();
		void moveTo(unsigned int position);
		void moveTo(unsigned int position, unsigned int stepMode);
		void moveUp(unsigned int steps);
		void moveDown(unsigned int steps);
		void setIRPin(unsigned int pin);
		void setButton1Pin(unsigned int pin);
		void setButton2Pin(unsigned int pin);
		boolean hasBottomReached();
		boolean isIRActive();
		unsigned int getButton1Value();
		unsigned int getButton2Value();
		unsigned int getIRValue();
		unsigned long getSteps();
		int getPosition();
		unsigned int getMinPosition();
		unsigned int getMaxPosition();
		unsigned int getStepMode();
		unsigned long getCheckUpSteps();
	private:
		unsigned int _stepsPerRound;
		uint8_t _stepPin;
		uint8_t _directionPin;
		uint8_t _enablePin;
		uint8_t _ms1Pin;
		uint8_t _ms2Pin;
		uint8_t _ms3Pin;
		uint8_t _button1Pin;
		uint8_t _button2Pin;
		uint8_t _irPin;
		unsigned int _stepDelay;
		unsigned int _initDelay;
		unsigned int _minDelay;
		unsigned int _directionChangeDelay;
		boolean _moveUp;
		boolean _isEnabled;
		boolean _isNewRequest;
		boolean _isCalibrated;
		int _position;
		unsigned long _steps;
		unsigned long _checkupSteps;
		unsigned int _minPosition;
		unsigned int _maxPosition;
		unsigned int _stepMode;
		void initPinModes();
		void setFullStepMode();
		void setHalfStepMode();
		void setQuarterStepMode();
		void setEigThthStepMode();
		//void setSixteenthStepMode();
		void doCheckUp(unsigned int targetPosition);
		unsigned long scaleSteps(unsigned long steps);
		unsigned long scaleSteps(unsigned long steps, unsigned int stepMode);
		unsigned int scaleStepsToPosition(unsigned long steps, unsigned int stepMode);
		unsigned long scalePositionToStep(unsigned int targetPosition, unsigned int stepMode );
		void setPosition(unsigned int percent);
		void changePosition(int positionIncrement);
		void addSteps(unsigned int steps);
		void addSteps(unsigned int steps, unsigned int stepMode);
		void resetSteps();

		boolean hasCheckup();

};

#endif
