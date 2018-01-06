from value_holder import ValueHolder
from messageHandler import *
import time


ZERO_CELCIUS = 273.15
TEMP_ONE_STEP = 4
RESOLUTION_LED_STRIP = 60/100 #60 leds/meter
LENGTH_ONE_LED = 100/60 #cm
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11
MAX_LED_BRIGHTNESS = 100

data=[]
#spi= None

modes={'MOVE':1,'UP':2,'DOWN':3,'INIT':4,'ANI':5,'LIGHT':6,'LEVEL':7}
speeds={ 'FULL':1,'HALF':2,'QUARTER':3,'EIGTHTH':4}
initShortcut={'TOP':1,'BOTTOM':2,'CALIBRATE':3}
animations={'BLINK':1,'GLOW':2}
sides={'A':1,'B':2,'C':3,'D':4}
lightOperations={'ADD':1,'REMOVE':2,'CLEARSIDE':3}

weatherModes={'TEMPERATURE':1,'RAIN':2}
displayMode={'CURRENT':1, 'FORECAST':2}

colors = {	'black':"0x000000", 
			'white':"0xFFFFFF", 
			'black':"0x000000", 
			'red':"0xFF0000", 
			'green':"0x00FF00", 
			'blue':"0x0000FF" };
			
sides = { 	'front':'A',
			'right':'B',
			'back':'C',
			'left':'D'
		}
		
displays=[None]*4			#saves value objects for the sides of bar
addSubValue=0

def main():
	#init_spi()
	#parse_message("TEMPERATURE:Hannover//Germany//10//100")
	#parse_message("RAINFORECAST:Sydney//Germany//10//100")
	#parse_message("RAINFORECAST:Berlin//Germany//10//100")
	
	#while(True):
	#	for i in range(0,4):
	#		if displays[i] is not None:
	#			displays[i].displayValue()
	#	
	#	time.sleep(600)
	
	
	requestURL='http://api.openweathermap.org/data/2.5/weather?q=Hannover&APPID=d1e9d70bdb58b701b0366495d128403d'
	pathJson=['main','humidity']
	referenceValue=0 
	stepSize=2
	side="A"
	color="FFFFFF"
	brightness=100
	
	#temperatureDiplay= ValueHolder(requestURL,None,pathJson,referenceValue,None,stepSize,side,color,brightness)
	#temperatureDiplay.setValueJson()
	
	#print(temperatureDiplay.value)
	
	#requestURL='http://api.openweathermap.org/data/2.5/weather?q=Hannover&APPID=d1e9d70bdb58b701b0366495d128403d&mode=xml'
	pathXML=['temperature','value']
	referenceValue=0 
	stepSize=1
	side="A"
	color="FFFFFF"
	brightness=100
	
	#temperatureDiplay= ValueHolder(requestURL,pathXML,None,referenceValue,scaleFunctionTemperature,stepSize,side,color,brightness)
	#temperatureDiplay.setValueXML()
	
	#print(temperatureDiplay.value)
	
	#temperatureDiplay.displayFromBottomToTop()
	
	pathXML="temperature,value"
	
	requestURL='api.openweathermap.org/data/2.5/weather?q=Hannover&APPID=d1e9d70bdb58b701b0366495d128403d&mode=xml'
	parse_message("DISPLAY:A//FFFFFF//100//20//-273//5//"+requestURL+"//"+pathXML+"//None")
	testValues=[1,30,7,8,17,20,25,30,40]
	index=0
	while(True):
		for i in range(0,4):
			if displays[i] is not None:
				displays[i].value=testValues[index]
				index=index+1
				displays[i].displayRelative()
				print("value=",displays[i].value)
		
		time.sleep(10)


#DISPLAY:SIDE//COLOR//BRIGHTNESS//REFERENCE//ADDSUBVALUE//STEPSIZE//URL//PATHXML//PATHJSON	#TODO: RANGEMAPPING

#TEMPERATURE:TIME//CITY//COUNTRY//BARSIDE//BRIGHTNESS
#RAIN:TIME//CITY//COUNTRY//BARSIDE//BRIGHTNESS


def parse_message(message):
	messageLength= len(message)
	seperatorIndex=message.index(':')
	
	mode= message[0:seperatorIndex]
	
	values= message[seperatorIndex+1:messageLength]
	values = values.split("//")
	
	
	side=values[0]
	color=values[1]
	brightness=int(values[2])
	referenceValue=float(values[3])
	addSubValue=float(values[4])
	stepSize=float(values[5])
	requestURL=values[6]

	
	pathXML=None
	pathJson=None
	
	if(values[7] != "None"):
		pathXML=values[7].split(",")
	if(values[8] != "None"):
		pathJson=values[8].split(",")
		
	print(pathJson,pathXML)	
	
	if(pathJson is None):
		addNewValueObject(requestURL,pathXML,None,referenceValue,scaleValue,addSubValue,stepSize,side,color,brightness)
	elif(pathXML is None):
		addNewValueObject(requestURL,None,path,referenceValue,scaleValue,addSubValue,stepSize,side,color,brightness)
	else:
		print("No SearchPath.") #error log
	
	print(mode,values)

def scaleValue(value): #scale value, e.g. distribution,intervalls
	
	return value
	

def scaleFunctionRain(millimeterPerHour):
	if(millimeterPerHour<2.5):
		return 1
	elif(millimeterPerHour<7.6):
		return 2
	elif(millimeterPerHour<30):
		return 3
	else:
		return 4

		
def addNewValueObject(requestURL,pathOfValueXML,pathOfValueJSON,referenceValue,scaleFunction,addSubValue,stepSize,side,color,brigthness):
	newValueObject= ValueHolder(requestURL,pathOfValueXML,pathOfValueJSON,referenceValue,scaleFunction,addSubValue,stepSize,side,color,brigthness)
	if(len(displays)<4):
		displays.append(newValueObject)
	else:
		displays[len(displays)%4]=newValueObject

if __name__== '__main__':
	main()