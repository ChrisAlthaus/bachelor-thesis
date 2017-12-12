from xml.etree import ElementTree
import requests
from message_handler import *


MAX_LED_BRIGHTNESS = 100
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11
UPPER_BUFFER = 2  #to recognize highest led, led range[0,9]
REFERENCE_RELATIVE = 6

colors = {	'black':"0x000000", 
			'white':"0xFFFFFF", 
			'black':"0x000000", 
			'red':"0xFF0000", 
			'green':"0x00FF00", 
			'blue':"0x0000FF" };

class ValueHolder:
	previousValue=0
	value=0
	
	def __init__(self,requestURL,pathOfValueXML,pathOfValueJSON,referenceValue,scaleFunction,addSubValue,stepSize,side,color,brigthness):
		self.requestURL="http://"+requestURL
		self.pathOfValueXML=pathOfValueXML
		self.pathOfValueJSON=pathOfValueJSON
		self.referenceValue=referenceValue
		self.scaleFunction=scaleFunction
		self.addSubValue=addSubValue
		self.stepSize=stepSize		#TODO: better intervall with [(min,min+1),....,(max-1,max)]
		self.side=side
		self.color=color
		self.brigthness=brigthness
		
		self.value=referenceValue
		self.previousValue=referenceValue
		
	def scale(self):		#scale value, e.g. unit conversion
		if self.scaleFunction is not None:
			self.value=self.scaleFunction(self.value)
		
	def setAddSubValue(self):		#shift value to other range
		self.value=self.value+self.addSubValue
		
	
	def requestJSON(self):
		responseData=None
		try:
			r = requests.get(self.requestURL)
			responseData = r.json()
		except requests.exceptions.ConnectionError:
			print("Connection refused")
		
		return responseData
		
	def requestXML(self):
		responseData=None
		try:
			r = requests.get(self.requestURL)
			responseData= ElementTree.fromstring(r.content)
		except requests.exceptions.ConnectionError:
			print("Connection refused")
		
		return responseData
			
	def setValueJson(self):
		self.previousValue=self.value
		
		valueField=self.requestJSON()
		
		if(valueField is None):
			print("No message received.")
			return
			
		for item in self.pathOfValueJSON:
			if item in valueField:
				valueField= valueField[item]
			else:
				print("JSON value not found.")
				return					#error log
		
		if isinstance(valueField,int):
			self.value=valueField
		
		if isinstance(valueField,float):
			self.value=valueField
		
		if isinstance(valueField,str):
			self.value=float(valueField)
		
		self.setAddSubValue()	
		self.scale()
	
	def setValueXML(self):
		self.previousValue=self.value
		
		valueField=self.requestXML()
		
		if(valueField is None):
			print("No message received.")
			return
			
		numberItems= len(self.pathOfValueXML)
		node=valueField
		
		for i in range(0,numberItems-1):
			node=node.find(self.pathOfValueXML[i])
		
		temp = node.get(self.pathOfValueXML[numberItems-1])
		
		if(temp is None or node is None):
			print("XML value not found.")
			return								#log error
		
		if isinstance(temp,int):
			self.value=temp
		if isinstance(temp,float):
			self.value=temp
		if isinstance(temp,str):
			self.value=float(temp)
		
		
		self.setAddSubValue()
		self.scale()
		
		
	def setValue(self):
		#print(self.requestURL)
		if self.pathOfValueJSON is None and self.pathOfValueXML is not None:
			self.setValueXML()
		elif self.pathOfValueXML is None and self.pathOfValueJSON is not None:
			self.setValueJson()
		else:
			print("No path value set.")		#error log
		
	def displayRelative(self):
		previousNumberLeds=round((self.previousValue-self.referenceValue)/self.stepSize)
		numberLeds=round((self.value-self.referenceValue)/self.stepSize)
		print(numberLeds,previousNumberLeds)
		print(self.previousValue,self.referenceValue)
		print(self.value)
		
		if(numberLeds>0):
			if(previousNumberLeds<=0):
				setLeds(self.side,"REMOVE",range(6+previousNumberLeds,6)[0:],0,0)		#delete part below reference led
				setLeds(self.side,"ADD",range(6,6+numberLeds+1)[1:],self.color,0)				#add part above
			elif(previousNumberLeds>0):
				if(numberLeds<previousNumberLeds):
					difference=previousNumberLeds-numberLeds
					setLeds(self.side,"REMOVE",range(6+numberLeds,6+previousNumberLeds)[1:],0,0)	#delete part above new led number
				elif(numberLeds>previousNumberLeds):
					setLeds(self.side,"ADD",range(6+previousNumberLeds,6+numberLeds+1)[1:],self.color,0)		#add part above
		elif(numberLeds<0):
			if(previousNumberLeds>=0):
				setLeds(self.side,"REMOVE",range(6,6+previousNumberLeds+1)[1:],0,0)		#delete part above reference led
				setLeds(self.side,"ADD",range(6+numberLeds,6)[0:],self.color,0)					#add part below
			elif(previousNumberLeds<0):
				if(numberLeds>previousNumberLeds):
					difference=numberLeds-previousNumberLeds
					setLeds(self.side,"REMOVE",range(6+numberLeds,6+previousNumberLeds+1)[1:],0,0)	#delete part below new led number
				elif(numberLeds<previousNumberLeds):
					setLeds(self.side,"ADD",range(6+previousNumberLeds,6+numberLeds+1)[1:],self.color,0)		#add part below
					
		setLed(self.side,"ADD",6,colors['white'],self.brigthness)	#set reference white led
		
		self.previousValue=self.value
	
	
	def displayFromBottomToTop(self):
		
		previousNumberLeds=round((self.previousValue-self.referenceValue)/self.stepSize)
		numberLeds=round((self.value-self.referenceValue)/self.stepSize)			
		
		if(previousNumberLeds is 0 ):  #Not initialized
				setLed(self.side,"ADD",1,colors['white'],self.brigthness)	#set reference white led
				
		if(numberLeds>previousNumberLeds):
			setLeds(self.side,"ADD",range(previousNumberLeds+1,numberLeds+1)[0:],self.color,self.brigthness)
		elif(numberLeds<previousNumberLeds):
			setLeds(self.side,"REMOVE",range(numberLeds,previousNumberLeds+1)[1:],0,0)
			
		#TODO: blink if side is overfilled
		
	
	def displayFromBottomToTopMovingHeight(self):
	
		setLed(sides[self.side],0,colors['white'],self.brigthness)
		maxQuantity = ( NUMBER_LEDS-1 ) - UPPER_BUFFER;		#one led for reference + two for upper buffer
		
		previousNumberLeds=round((self.previousValue-self.referenceValue)/self.stepSize)
		numberLeds=round((self.value-self.referenceValue)/self.stepSize)
		
		difference= abs(numberLeds-previousNumberLeds)
		
		if(numberLeds<=maxQuantity):
			moveDownNumberLeds(difference)
			displayFromBottomToTop()
		
		elif(numberLeds>maxQuantity):
			maxQuantity=maxQuantity +1 #don't show white led
			newNumberLeds= numberLeds - maxQuantity			#number Leds for height adjusted bar
			
			difference=difference-1 	#no white led 
			setLed(sides[self.side],0,self.color,self.brigthness) #e.g. 10 new, 9 old -> remove bottom reference led
			
			if(previousNumberLeds<newNumberLeds):
				setLeds(self.side,"ADD",range(previousNumberLeds+1,numberLeds+1)[0:],self.color,self.brigthness)
				moveUpNumberLeds(difference)
			elif(previousNumberLeds>newNumberLeds):
				setLeds(self.side,"REMOVE",range(numberLeds,previousNumberLeds+1)[1:],0,0)	
				moveDownNumberLeds(difference)
			
			
		
