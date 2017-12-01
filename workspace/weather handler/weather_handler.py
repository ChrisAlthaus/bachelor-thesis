import requests
import spi_connection


ZERO_CELCIUS = 273.15
TEMP_ONE_STEP = 4
RESOLUTION_LED_STRIP = 60/100 #60 leds/meter
LENGTH_ONE_LED = 100/60 #cm
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11

data=[]
spi= None

modes={'MOVE':1,'UP':2,'DOWN':3,'INIT':4,'ANI':5,'LIGHT':6,'LEVEL':7}
speeds={ 'FULL':1,'HALF':2,'QUARTER':3,'EIGTHTH':4}
initShortcut={'TOP':1,'BOTTOM':2,'CALIBRATE':3}
animations={'BLINK':1,'GLOW':2}
sides={'A':1,'B':2,'C':3,'D':4}
lightOperations={'ADD':1,'REMOVE':2,'CLEARSIDE':3}

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

def main():
	parse_message("TEMPERATURE:Hannover//Germany//10//100")
	parse_message("RAINFORECAST:Sydney//Germany//10//100")
	parse_message("RAINFORECAST:Berlin//Germany//10//100")
	

def init_spi():
	spi = SPI(0,0)
	spi.open()

def send_arduino(message):
	spi.send_message(message)
	
def request(type,city):
	global data
	
	if(type=="CURRENT"):
		r = requests.get('http://api.openweathermap.org/data/2.5/weather?q=' + city + '&APPID=d1e9d70bdb58b701b0366495d128403d')
		data = r.json()
	elif(type=="FORECAST"):
		r = requests.get('http://api.openweathermap.org/data/2.5/forecast?q=' + city + '&APPID=d1e9d70bdb58b701b0366495d128403d')
		data = r.json()

		
#TEMPERATURE:CITY//COUNTRY//REFERENCETEMP//BRIGHTNESS
#RAINFORECAST:CITY//COUNTRY//BRIGHTNESS

def parse_message(message):
	message_length= len(message)
	seperator_index=message.index(':')
	
	mode= message[0:seperator_index]
	
	values= message[seperator_index+1:message_length]
	values = values.split("//")
	print(mode,values)
	
	if(mode=="TEMPERATURE"):
		request("CURRENT",values[0])
		#display_temperature(values[2],values[3])
		display_quantity_with_height(90,5,2,"front",colors["green"],75)
	elif(mode=="RAINFORECAST"):
		request("FORECAST",values[0])
		display_rain("front",values[2])
	
def display_temperature(reference_temp,brigthness):
	temp= data["main"]["temp"] - ZERO_CELCIUS
	print("temp=" + str(temp))
	
	number_leds=11 #11 led's
	
	min_temp = -10
	max_temp = 30
	
	
	led_number= int(map(temp,min_temp,max_temp,1,11))
	
	led_color=colors['white']
	if(led_number<6):
		led_color=colors['red']
	else:
		led_color=color['blue']
	
	setLed(sides[side],6,colors['white'],brigthness)
	setLeds(sides[side],range(6,led_number+1)[1:],led_color,brigthness)


def display_quantity_with_height(value,step_resolution,upper_buffer,side,quantity_color,brigthness):
	moveToPercentage(0,"half")
	print("value=" + str(value))
	
	setLed(sides[side],0,colors['white'],brigthness)
	
	leds_quantity = int(value/step_resolution)
	
	if(leds_quantity<= (NUMBER_LEDS - 1) - upper_buffer):
		for led_number in range(1,leds_quantity):
			setLedWithEffect(sides[side],led_number,quantity_color,brigthness)
	else:
		for led_number in range(1,NUMBER_LEDS-1):
			setLed(sides[side],led_number,quantity_color,brigthness)
		
		difference = leds_quantity - (( NUMBER_LEDS-1 ) - upper_buffer)
	
		
		for i in range(1,difference+1):
			moveUpOneLed()
		
		index_start= (NUMBER_LEDS - upper_buffer)-difference
		for i in range(1,difference+1):
			setLedWithEffect(sides[side],index_start+i,quantity_color,brigthness)
			
		#setLeds(sides[side],range(1,NUMBER_LEDS-upper_buffer),quantity_color,brigthness)
		#setLedWithEffect(sides[side],NUMBER_LEDS - upper_buffer,quantity_color,brigthness)
		
	

def moveToPercentage(percentage,step_mode):
	print("MOVE:"+ str(percentage) + "/" + step_mode)
	#send_arduino("MOVE:"+ str(percentage) + "/" + step_mode)

def moveUp(number_leds):
	print("UP:"+ str(getSteps(number_leds,"quarter")))
	#send_arduino("UP:"+ getSteps(number_leds,"quarter"))

def moveUpOneLed():
	print("UP:"+ str(getSteps(1,"quarter")))
	#send_arduino("UP:"+ getSteps(number_leds,"quarter"))

def setLed(side,led_number,color,brigthness):
	print("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + str(brigthness))
	#send_arduino("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + str(brigthness))

def setLedWithEffect(side,led_number,color,brigthness):
	print("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + "100")
	#send_arduino("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + "100")
	#delay
	print("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + str(brigthness))
	#send_arduino("LIGHT:"+ str(side) +"/+/" + str(led_number) +"/"+ str(color) + "/" + str(brigthness))

def display_rain(side,brigthness):
	rainfall=0
	if("list" in data):
		if("rain" in data["list"][1]):
			if("3h" in data["list"][1]["rain"] ):
				rainfall=data["list"][1]["rain"]["3h"]
				print("rainfall",rainfall)
	
	if(rainfall==0):
		led_count=0
	else:
		rain_intensity= getRainIntensity(rainfall)
		if(rain_intensity==4):
			show_warning()
		led_count=1 + int(map(rain_intensity,1,4,1,9))
		
	print("rain led's=" + str(led_count))

	setLeds(sides[side],range(2,led_count+1), colors["blue"],50)
	
def getSteps(length,step_mode):
	step_number=0
	if(step_mode=="full"):
		step_number= (length/15)*200
	elif(step_mode=="half"):
		step_number= (length/15)*200 *2
	elif(step_mode=="quarter"):
		step_number= (length/15)*200 *4
	elif(step_mode=="eigthth"):
		step_number= (length/15)*200 *8
	return step_number
	
def map(x,in_min,in_max,out_min,out_max):
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	

def getRainIntensity(millimeter_per_hour):
	if(millimeter_per_hour<2.5):
		return 1
	elif(millimeter_per_hour<7.6):
		return 2
	elif(millimeter_per_hour<30):
		return 3
	else:
		return 4

def setLeds(side,leds,color,brigthness):
	for led_number in leds:
		print("LIGHT:"+str(side)+"/+/" + str(led_number) + "/" + str(color) + "/" + str(brigthness))
		#send_arduino("LIGHT:"+str(side)+"/+/" + str(led_number) + "/" + str(color) + "/100")

def show_warning():
	#send_arduino("LIGHT:A/+/11/"+ color["red"] + "/100")
	print("LIGHT:A/+/11/"+ color["red"] + "/100")
	
if __name__== '__main__':
	main()