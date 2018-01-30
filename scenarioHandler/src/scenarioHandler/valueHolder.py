import sys
sys.path.append('..')

from xml.etree import ElementTree
import requests
from messageHandler import *
from utility import *

MAX_LED_BRIGHTNESS = 100
NUMBER_LEDS_WITH_HEIGHT = 19
NUMBER_LEDS = 11
UPPER_BUFFER = 1  #to recognize highest led, led range[1,10]
REFERENCE_RELATIVE = 6
REFERENCE_LEDS = 1
STEP_MODE_HEIGHT_MOVEMENT = "HALF"


colors = {  'black':"000000", 
            'white':"FFFFFF",
            'red':"900000", 
            'green':"00FF00", 
            'blue':"0000FF",
            'yellow':"EEEE00",
            'orange':'FFA500' };

class ValueHolder:
    previousValue=0
    value=0
    referenceLedInit=False
    overfilledWarning=False
    currentHeight=0
    
    def __init__(self,side,displayColor,referenceColor,referenceValue,stepSize,mode,requestURL,pathOfValueXML,pathOfValueJSON):
        self.requestURL="http://"+requestURL
        self.pathOfValueXML=pathOfValueXML
        self.pathOfValueJSON=pathOfValueJSON
        self.referenceValue=referenceValue
        #self.scaleFunction=scaleFunction
        self.stepSize=stepSize        #TODO: better intervall with [(min,min+1),....,(max-1,max)]
        self.side=side
        self.color=displayColor
        self.referenceColor=referenceColor
        self.mode=mode
        
        self.value=referenceValue
        self.previousValue=referenceValue
        
    #def scale(self):        #scale value, e.g. unit conversion
     #   if self.scaleFunction is not None:
      #      self.value=self.scaleFunction(self.value)

    
    def requestJSON(self):
        responseData=None
        try:
            r = requests.get(self.requestURL)
            responseData = r.json
            print responseData
        except requests.exceptions.RequestException as e:
            printError(e)   #error log
            print "request excpetion"
            setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, colors['yellow'])  #show warning, if url unreachable
        except ValueError as  e:
            printError(e)   #error log
            print "value error"
            setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, colors['orange'])   #show warning, if parse error
        
        return responseData
        
    def requestXML(self):
        responseData=None
        try:
            r = requests.get(self.requestURL)
            print r.content
            responseData= ElementTree.fromstring(r.content)
        except requests.exceptions.RequestException as e:
            printError(e)  #error log
            setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, colors['yellow'])  #show warning, if url unreachable
        except ElementTree.ParseError as e:
            setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, colors['orange'])  #show warning, if parse error
            printError(e)  #error log

        return responseData
            
    def setValueJson(self):
        self.previousValue=self.value
        
        valueField=self.requestJSON()
        
        if(valueField is None):
            printError("No message received.")
            return
            
        for item in self.pathOfValueJSON:
            if item in valueField:
                valueField= valueField[item]
            else:
                printError("JSON entry "+valueField[item] +" not found.")
                return                    #error log
        
        if isinstance(valueField,int):
            self.value=valueField
        
        if isinstance(valueField,float):
            self.value=valueField
        
        if isinstance(valueField,str):
            self.value=float(valueField)
        
        print "new value=",valueField
    
    def setValueXML(self):
        self.previousValue=self.value
        
        valueField=self.requestXML()
        print valueField
   
        if(valueField is None):
            printError("No message received.")
            return
            
        numberItems= len(self.pathOfValueXML)
        node=valueField
        
        for i in range(0,numberItems-1):
           if node is not None:
              print "parse field:",self.pathOfValueXML[i]
              node=node.find(self.pathOfValueXML[i])
	   else:
              printError("XML parsing error: entry "+ self.pathOfValueXML[i]+ " not found")
              return
        
        temp = node.get(self.pathOfValueXML[numberItems-1])
        
        if(temp is None):
            printError("XML parsing error: entry "+ self.pathOfValueXML[numberItems-1]+ " not found.")
            return                                #log error
        
        if isinstance(temp,int):
            self.value=temp
        if isinstance(temp,float):
            self.value=temp
        if isinstance(temp,str):
            self.value=float(temp)
        
        print "new value=",temp
        
        
    def setValue(self):
        if self.pathOfValueJSON is None and self.pathOfValueXML is not None:
            self.setValueXML()
        elif self.pathOfValueXML is None and self.pathOfValueJSON is not None:
            self.setValueJson()
        else:
            printError("No path value set.")        #error log
    
        
    def display(self):
       
        if(self.mode=="From bottom to top"):
            self.displayFromBottomToTop()
        elif(self.mode=="Relative"):
            self.displayRelative()
        elif(self.mode=="Moving height"):
            self.displayFromBottomToTopMovingHeight()
        else:
            printError("No valid display mode")
            
            
        
    def displayRelative(self):
        previousNumberLeds=int(round((self.previousValue-self.referenceValue)/self.stepSize))
        numberLeds=int(round((self.value-self.referenceValue)/self.stepSize))
        #print(numberLeds,previousNumberLeds)
        #print(self.previousValue,self.value)
        #print(self.referenceValue)
        
        if(self.referenceLedInit is False):
            setLed(self.side,"ADD",6,self.referenceColor)    #set reference white led
            self.referenceLedInit=True
        
        if((6+previousNumberLeds)>NUMBER_LEDS):
            previousNumberLeds= int((NUMBER_LEDS-REFERENCE_LEDS)/2)  #number of leds for upper and lower half
        elif((6+previousNumberLeds)<1):
            previousNumberLeds= -int((NUMBER_LEDS-REFERENCE_LEDS)/2)  #number of leds for upper and lower half
       
        if((6+numberLeds)>NUMBER_LEDS):
            setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, self.color)  #show warning, if overfilled  
            numberLeds=int((NUMBER_LEDS-REFERENCE_LEDS)/2)
            self.overfilledWarning=True
        elif((6+numberLeds)<1):
            setSingleAnimation("BEACON", self.side, 1, 10000, self.color)  #show warning, if overfilled
            numberLeds=-int((NUMBER_LEDS-REFERENCE_LEDS)/2)
            self.overfilledWarning=True
        else:
            if(self.overfilledWarning):
                setSingleAnimation("OFF", self.side, 1, 10000, self.color)  #stop animation
                setLeds(self.side,"REMOVE",list([1,NUMBER_LEDS]),0)     #clear warning leds,if stopped in on state
                self.overfilledWarning=False
               
        if(numberLeds>=0):
            if(previousNumberLeds<=0):
                setLeds(self.side,"REMOVE",range(6+previousNumberLeds,6)[0:],0)        #delete part below reference led
                setLeds(self.side,"ADD",range(6,6+numberLeds+1)[1:],self.color)                #add part above
            elif(previousNumberLeds>0):
                if(numberLeds<previousNumberLeds):
                    difference=previousNumberLeds-numberLeds
                    setLeds(self.side,"REMOVE",reversed(range(6+numberLeds,6+previousNumberLeds)[1:]),0)    #delete part above new led number
                elif(numberLeds>previousNumberLeds):
                    setLeds(self.side,"ADD",range(6+previousNumberLeds,6+numberLeds+1)[1:],self.color)        #add part above
        elif(numberLeds<0):
            if(previousNumberLeds>=0):
                setLeds(self.side,"REMOVE",reversed(range(6,6+previousNumberLeds+1)[1:]),0)        #delete part above reference led
                setLeds(self.side,"ADD",reversed(range(6+numberLeds,6)[0:]),self.color)                    #add part below
            elif(previousNumberLeds<0):
                if(numberLeds>previousNumberLeds):
                    difference=numberLeds-previousNumberLeds
                    setLeds(self.side,"REMOVE",reversed(range(6+previousNumberLeds,6+numberLeds)[0:]),0)    #delete part below new led number
                elif(numberLeds<previousNumberLeds):
                    setLeds(self.side,"ADD",reversed(range(6+numberLeds,6+previousNumberLeds)[0:]),self.color)        #add part below
                    
       
            
        self.previousValue=self.value
    
    
    def displayFromBottomToTop(self):
        
        previousNumberLeds=int(round((self.previousValue-self.referenceValue)/self.stepSize))
        numberLeds=int(round((self.value-self.referenceValue)/self.stepSize))  
        print("current value=", self.value)
        print("previous value=",self.previousValue)          
        
        if(numberLeds<0): #set to zero leds
            numberLeds=0
        if(previousNumberLeds<0):
            previousNumberLeds=0   
        
        print(numberLeds,previousNumberLeds)
           
        maxQuantity=NUMBER_LEDS-UPPER_BUFFER-REFERENCE_LEDS       #led number range [1,10]
                   
        if(self.referenceLedInit is False):  #Not initialized
                setLed(self.side,"ADD",1,self.referenceColor)    #set reference white led
                self.referenceLedInit=True
              
        if(numberLeds>previousNumberLeds):  #led display range [REFERENCE_LEDS+1,10] 
            setLeds(self.side,"ADD",range(previousNumberLeds+REFERENCE_LEDS,numberLeds+REFERENCE_LEDS+1)[1:],self.color) 
        elif(numberLeds<previousNumberLeds):
            setLeds(self.side,"REMOVE",reversed(range(numberLeds+REFERENCE_LEDS,previousNumberLeds+REFERENCE_LEDS+1)[1:]),0)
        

        if(numberLeds>maxQuantity):          
    	   numberLeds=maxQuantity
           if(self.overfilledWarning is False):
              setSingleAnimation("BEACON", self.side, NUMBER_LEDS, 10000, self.color)    
              self.overfilledWarning=True
        else:
           if(self.overfilledWarning):
	      setSingleAnimation("OFF", self.side, NUMBER_LEDS, 10000, self.color)  #stop animation
              setLed(self.side,"REMOVE",NUMBER_LEDS,0)     #clear warning leds,if stopped in on state
	      previousNumberLeds=maxQuantity
              self.overfilledWarning=False
 
        self.previousValue=self.value
        
        
    
    def displayFromBottomToTopMovingHeight(self):
        
        previousNumberLeds=int(round((self.previousValue-self.referenceValue)/self.stepSize))
        numberLeds=int(round((self.value-self.referenceValue)/self.stepSize))
        #print(numberLeds,previousNumberLeds)
        #print(self.previousValue,self.referenceValue)
        
        if(numberLeds<0): #set to zero leds
            numberLeds=0
        if(previousNumberLeds<0):
            previousNumberLeds=0 
            
        if(self.referenceLedInit is False):  #Not initialized
                setLed(self.side,"ADD",1,self.referenceColor)    #set reference white led
                self.referenceLedInit=True
                
        
        maxQuantity=NUMBER_LEDS-UPPER_BUFFER-REFERENCE_LEDS       #led number range [1,8]       
        
        if(numberLeds<=maxQuantity):
            if(previousNumberLeds>maxQuantity):
                moveToPercentage(0,STEP_MODE_HEIGHT_MOVEMENT)
            self.displayFromBottomToTop()
        
        elif(numberLeds>maxQuantity):
            newNumberLeds= numberLeds - maxQuantity -1   #number Leds for height adjusted bar, -1 -> don't show reference led
            self.referenceLedInit=False
            
            if(previousNumberLeds<=maxQuantity):
                setLed(self.side,"ADD",1,self.color) #e.g. 9 new, 8 old -> remove bottom reference led
                moveUpNumberLeds(newNumberLeds)#move up, no led change necessary->highest led at index 9
            elif(previousNumberLeds>maxQuantity):
                difference= abs(numberLeds-previousNumberLeds)
                
                if(previousNumberLeds<numberLeds):
                    moveUpNumberLeds(difference);
                elif(previousNumberLeds>numberLeds):
                    moveDownNumberLeds(difference)
            
        self.previousValue=self.value  
         
      
    def printValueHolder(self):
       print("side: ", self.side)
       print("display color: ", self.color)
       print("reference color: ", self.referenceColor)
       print("mode: ", self.mode)
       print("stepSize: ", self.stepSize)
       print("requestURL= ", self.requestURL)
       print("XML path: ",self.pathOfValueXML)
       print("JSON path: ", self.pathOfValueJSON)
