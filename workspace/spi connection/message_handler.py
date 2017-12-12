#import spi-connection
import time

MAX_LED_BRIGHTNESS = 100
STEPS_PER_REVOLUTION = 200 #eigthth step mode
ONE_REVOLUTION = 15	#mm
RESOLUTION_LED_STRIP = 60/100 #60 leds/meter
LENGTH_ONE_LED = 100/60 #cm
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11

modes={'MOVE':1,'UP':2,'DOWN':3,'INIT':4,'ANI':5,'LIGHT':6,'LEVEL':7}
speeds={ 'FULL':1,'HALF':2,'QUARTER':3,'EIGTHTH':4}
initShortcut={'TOP':1,'BOTTOM':2,'CALIBRATE':3}
animations={'BLINK':1,'GLOW':2}
sides={'A':1,'B':2,'C':3,'D':4}
lightOperations={'ADD':1,'REMOVE':2,'CLEARSIDE':3}


#def main():
#	calibrate()
#	
#	move(100,"HALF")
#	moveDown(400)
#	
#	setAnimation("BLINK","FFFFFF",64,50)
#	
#	setMinPosition(15600)
	
	
	
currentStepMode=speeds["HALF"]

#initialize spi

#def init_spi():
#	spi = SPI(0,0)
#	spi.open()

def send_arduino(message):
	#spi.send_message(message)
	print(message)
	
	
#motor functions

def moveUp(stepNumber):
	message=[]
	message.append(modes["UP"])
	
	message.extend(getByteArrayFromNumber(400,2))
	
	print("UP:"+str(stepNumber))
	send_arduino(message)
	
def moveDown(stepNumber):
	message=[]
	message.append(modes["DOWN"])
	
	message.extend(getByteArrayFromNumber(400,2))
	
	print("DOWN:"+str(stepNumber))
	send_arduino(message)
	
	
def moveToPercentage(position,stepMode):
	global currentStepMode
	currentStepMode=stepMode
	
	message=[]
	message.append(modes["MOVE"])
	
	message.append(position)
	message.append(speeds[stepMode])
	
	send_arduino(message)
	

def calibrate():
	message=[]
	message.append(modes["INIT"])
	message.append(initShortcut["CALIBRATE"])
	
	send_arduino(message)
	
	
def setMinPosition(stepPosition):
	message=[]
	message.append(modes["INIT"])
	
	message.append(initShortcut["BOTTOM"])
	message.extend(getByteArrayFromNumber(stepPosition,2))
	
	send_arduino(message)
	
	
def setMaxPosition(stepPosition):
	message=[]
	message.append(modes["INIT"])
	
	message.append(initShortcut["TOP"])
	message.extend(getByteArrayFromNumber(stepPosition,2))
	
	send_arduino(message)
	
	
	
#lighting functions
	
def setLed(side,operation,led,color,brightness):
	message=[]
	message.append(modes["LIGHT"])
	message.append(getSideOperationLedByte(sides[side],lightOperations[operation],led))
	
	message.extend(getByteArrayFromNumber(int(color,16),3))
	message.append(brightness)
	
	print("LIGHT:"+str(side)+"//"+str(operation)+"//"+str(led)+"//"+str(color)+"//"+str(brightness))
	send_arduino(message)
	
	
def clearSide(side):
	message=[]
	message.append(modes["LIGHT"])
	
	message.append(sides[side])
	message.append(lightOperations["CLEARSIDE"])
	
	send_arduino(message)
	
	
def setAnimation(animation,color,brightness,speed):
	message=[]
	message.append(modes["ANI"])
	message.extend(getByteArrayFromNumber(int(color,16),3))
	
	message.append(brightness)
	message.append(speed)
	
	send_arduino(message)
	
	
def setLevel(level,color,brightness):
	message=[]
	message.append(modes["LEVEL"])
	message.extend(getByteArrayFromNumber(int(color,16),3))
	
	message.append(brightness)
	
	send_arduino(message)
	
	
	
#special functions

def moveUpNumberLeds(numberLeds):
	moveUp(getSteps(numberLeds,currentStepMode))

def moveDownNumberLeds(numberLeds):
	moveDown(getSteps(numberLeds,currentStepMode))
	

def setLedWithEffect(side,led,color,brigthness):
	setLed("A","ADD",led,color,MAX_LED_BRIGHTNESS)
	time.sleep(0.5)
	setLed("A","ADD",led,color,brightness)
	
def setLeds(side,operation,leds,color,brigthness):
	for ledNumber in leds:
		setLed(side,operation,ledNumber,color,brightness)

def showWarning(side):
	setLed(side,"ADD",11,color["red"],MAX_LED_BRIGHTNESS)
	
	
	
#helper functions

def getSteps(numberLeds,step_mode):
	step_number=0
	length= LENGTH_ONE_LED * numberLeds
	
	if(step_mode=="FULL"):
		step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION
	elif(step_mode=="HALF"):
		step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION *2
	elif(step_mode=="QUARTER"):
		step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION *4
	elif(step_mode=="EIGTHTH"):
		step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION *8
	return step_number
	

def map(x,in_min,in_max,out_min,out_max):
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	

def getByteArrayFromNumber(number,numberBytes):
	byteArray=[]
	for i in range(1,numberBytes+1):
		byteArray.append((number>>((numberBytes-i)*8))&255)
		
	return byteArray
	
	
def getSideOperationLedByte(side,operation,led):
	number=led
	number+= operation<<4
	number+= operation<<6
	
	return number
