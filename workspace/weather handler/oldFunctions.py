def request(self):
		global requestData
		
		if(type==displayMode["CURRENT"]):
			r = requests.get('http://api.openweathermap.org/data/2.5/weather?q=' + city + '&APPID=d1e9d70bdb58b701b0366495d128403d')
			requestData = r.json()
		elif(type==displayMode["FORECAST"]):
			r = requests.get('http://api.openweathermap.org/data/2.5/forecast?q=' + city + '&APPID=d1e9d70bdb58b701b0366495d128403d')
			requestData = r.json()
			
			
		
	def displayTemperature(self):
		temp= data["main"]["temp"] - ZERO_CELCIUS
		print("temp=" + str(temp))
		
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
		
	
	def displayRain(self):
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
		
	
	def getRainIntensity(millimeter_per_hour):
		if(millimeter_per_hour<2.5):
			return 1
		elif(millimeter_per_hour<7.6):
			return 2
		elif(millimeter_per_hour<30):
			return 3
		else:
			return 4	
		
	def parse_message(message):
		message_length= len(message)
		seperator_index=message.index(':')
		
		mode= message[0:seperator_index]
		
		values= message[seperator_index+1:message_length]
		values = values.split("//")
		print(mode,values)
		
		if(mode=="TEMPERATURE"):
			addNewValueObject("TEMPERATURE","CURRENT",values[0],values[1],values[2],values[3])
		
		elif(mode=="TEMPFORECAST"):
			addNewValueObject("TEMPERATURE","FORECAST",values[0],values[1],values[2],values[3])	
		
		elif(mode=="RAIN"):
			addNewValueObject("RAIN","CURRENT",values[0],values[1],values[2],values[3])
			
		elif(mode=="RAINFORECAST"):
			addNewValueObject("RAIN","FORECAST",values[0],values[1],values[2],values[3])	
		
	def scaleFunctionTemperature(value):
		return value - ZERO_CELCIUS
	
	def displayRelative(self):
		previousNumberLeds=round((self.previousValue-self.referenceValue)/self.stepSize)
		numberLeds=round((self.value-self.referenceValue)/self.stepSize)
		print(numberLeds,previousNumberLeds)
		print(self.previousValue,self.referenceValue)
		print(self.value)
		if(numberLeds>previousNumberLeds and self.previousValue>=self.referenceValue):	#add led above reference value
			setLeds(self.side,"ADD",range(6+previousNumberLeds+1,6+numberLeds+1)[0:],self.color,self.brigthness)
		elif(numberLeds<previousNumberLeds and self.previousValue<=self.referenceValue):	#add led below reference value
			setLeds(self.side,"ADD",range(6+numberLeds,6+previousNumberLeds)[0:],self.color,self.brigthness) #reference led number=6
		elif(numberLeds>previousNumberLeds and numberLeds>6 and previousNumberLeds<6):
			setLeds(self.side,"REMOVE",range(6+previousNumberLeds,6)[1:],0,0)		#delete part below reference led
			setLeds(self.side,"ADD",range(6,6+numberLeds+1)[1:],0,0)				#add part above
		elif(numberLeds<previousNumberLeds and numberLeds<6 and previousNumberLeds>6):
			setLeds(self.side,"REMOVE",range(6,6+previousNumberLeds)[1:],0,0)		#delete part above reference led
			setLeds(self.side,"ADD",range(6+numberLeds,6)[1:],0,0)					#add part below
		elif(numberLeds>previousNumberLeds and numberLeds<6 and previousNumberLeds<6):
			setLeds(self.side,"REMOVE",range(6+previousNumberLeds,6+numberLeds)[0:],colors['white'],0)		
		setLed(self.side,"ADD",6,colors['white'],self.brigthness)	#set reference white led
		
		self.previousValue=self.value
		
	def quantityWithHeight:
		if(leds_quantity<= maxQuantity):	
			for ledNumber in range(1,leds_quantity):
				setLedWithEffect(sides[self.side],ledNumber,self.color,self.brigthness)
		else:
			difference = leds_quantity - maxQuantity
		
			for ledNumber in range(1,maxQuantity+1):
				setLed(sides[self.side],"ADD",ledNumber,self.color,self.brigthness)
			
			moveUpNumberLeds(difference)
			
			indexStart= (NUMBER_LEDS - UPPER_BUFFER)-difference
			for i in range(1,difference+1):
				setLedWithEffect(sides[self.side],indexStart+i,self.color,self.brigthness)