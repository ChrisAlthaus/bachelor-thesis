import time
from spi.spiConnection import SPI
from modes import *

MAX_LED_BRIGHTNESS = 100
STEPS_PER_REVOLUTION = 200 #eigthth step mode
ONE_REVOLUTION = 15.0    #mm
RESOLUTION_LED_STRIP = 60/100 #60 leds/meter
LENGTH_ONE_LED = 100/60.0 *10.0 #mm
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11
LED_CHANGE_TIME_DELAY = 0.4


spi=None
    
currentStepMode=speeds["HALF"]

#initialize spi

def init_spi():
    global spi    
    spi = SPI(0,0)
    spi.open()

def send_arduino(message):
    spi.send_message(message)
    print(message)
    
    
#motor functions

def moveUp(stepNumber):
    message=[]
    message.append(modes["UP"])
    
    message.extend(getByteArrayFromNumber(int(stepNumber),2))
    
    print("UP:"+str(stepNumber))
    send_arduino(message)
    
def moveDown(stepNumber):
    message=[]
    message.append(modes["DOWN"])
    
    message.extend(getByteArrayFromNumber(int(stepNumber),2))
    
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
    
def setLed(side,operation,led,color):
    message=[]
    message.append(modes["LIGHT"])
    
    if(led<1 or led>11):
        return
    
    message.append(getSideOperationLedByte(sides[side],lightOperations[operation],led))
    
    if isinstance(color,int):
        message.extend(getByteArrayFromNumber(color,3))
    else:
        try:
            message.extend(getByteArrayFromNumber(int(color,16),3))
        except TypeError:
            print("Color must be a string or integer")          #TODO: log file
            return
    
    message.append(50)

    print("LIGHT:"+str(side)+"//"+str(operation)+"//"+str(led)+"//"+str(color))
    send_arduino(message)

def setBrightness(brightness):
    message=[]
    message.append(modes["BRIGHTNESS"])
    
    message.append(brightness)
    
    send_arduino(message)
    print("BRIGHTNESS:"+str(brightness))    
    
def clearSide(side):
    message=[]
    message.append(modes["LIGHT"])
    
    message.append(getSideOperationLedByte(sides[side],lightOperations["CLEARSIDE"],0))
    
    print("CLEARSIDE:"+str(side))
    
    send_arduino(message)
    
    
def setAnimation(animation,color,brightness,speed):
    message=[]
    message.append(modes["ANI"])
    message.append(getTypeAnimationByte(animationTypes["OVERALL"],animations[animation]))
    
    if isinstance(color,int):
        message.extend(getByteArrayFromNumber(color,3))
    else:
        try:
            message.extend(getByteArrayFromNumber(int(color,16),3))
        except TypeError:
            print("Color must be a string or integer")
            
    
    
    message.extend(getByteArrayFromNumber(speed,2))
    
    message.append(getSideLedByte(0,0))
    message.append(brightness)
    
    send_arduino(message)
    
  
def setSingleAnimation(animation, side, ledNumber, durationOneCycle, color):
    message=[]
    message.append(modes["ANI"])
    message.append(getTypeAnimationByte(animationTypes["SINGLE"],animations[animation]))
    
    
    if isinstance(color,int):
        message.extend(getByteArrayFromNumber(color,3))
    else:
        try:
            message.extend(getByteArrayFromNumber(int(color,16),3))
        except TypeError:
            print("Color must be a string or integer")
            
    
    message.extend(getByteArrayFromNumber(durationOneCycle,2))
    
    message.append(getSideLedByte(sides[side],ledNumber))
    
    print("ANIMATION:SINGLE"+str(animation)+"//"+str(color)+"//"+
          str(durationOneCycle)+"//"+str(side)+"//"+str(ledNumber))
    send_arduino(message)  
    
    
def setLevel(level,color,brightness):
    message=[]
    message.append(modes["LEVEL"])
    message.extend(getByteArrayFromNumber(int(color,16),3))
    
    message.append(brightness)
    
    send_arduino(message)
    
    
    
#special functions

def moveUpNumberLeds(numberLeds):
    print("step mode=",currentStepMode)
    if(numberLeds > 0):
        print("move up number leds=",numberLeds)
	moveUp(getSteps(numberLeds,currentStepMode))
        print("steps=" + str(getSteps(numberLeds,currentStepMode)))

def moveDownNumberLeds(numberLeds):
    if(numberLeds > 0):
	 moveDown(getSteps(numberLeds,currentStepMode))
    print("steps=" + str(getSteps(numberLeds,currentStepMode)))

def setLedWithEffect(side,led,color,brightness):
    setLed("A","ADD",led,color,MAX_LED_BRIGHTNESS)
    time.sleep(0.5)
    setLed("A","ADD",led,color,brightness)
    
def setLeds(side,operation,leds,color):
    for ledNumber in leds:
        setLed(side,operation,ledNumber,color)
        time.sleep(LED_CHANGE_TIME_DELAY)

       
def showWifiConnectionError(self):
    setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, colors['yellow'])  #show warning, if url unreachable  
    
    
    
#helper functions

def getSteps(numberLeds,step_mode):
    step_number=0
    length= LENGTH_ONE_LED * numberLeds
    print("length=",length)
    if(step_mode==speeds["FULL"]):
        step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION
    elif(step_mode==speeds["HALF"]):
        step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION *2
	print("step_number=",step_number)
    elif(step_mode==speeds["QUARTER"]):
        step_number= (length/ONE_REVOLUTION)*STEPS_PER_REVOLUTION *4
    elif(step_mode==speeds["EIGTHTH"]):
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
    number+= side<<6
    
    return number
    
def getTypeAnimationByte(type,animation):
    number=animation
    number+= type<<4
    
    return number

def getSideLedByte(side,led):
    number=led
    number+= side<<4
    
    return number
