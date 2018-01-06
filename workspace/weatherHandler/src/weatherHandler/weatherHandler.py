from valueHolder import *
from messageHandler import *
import time
import threading


spi= None

#weatherModes={'TEMPERATURE':1,'RAIN':2}
#displayMode={'CURRENT':1, 'FORECAST':2}

colors = {    'black':"0x000000", 
            'white':"0xFFFFFF", 
            'black':"0x000000", 
            'red':"0xFF0000", 
            'green':"0x00FF00", 
            'blue':"0x0000FF" };
            
displays=[None]*4            #saves value objects for the sides of bar
#display=displayHandler(10)

addSubValue=0

def main():
    #init_spi()
    
    #while(True):
    #    for i in range(0,4):
    #        if displays[i] is not None:
    #            displays[i].displayValue()
    #    
    #    time.sleep(600)
    
    
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
    parse_message("DISPLAY:B//FFFFFF//100//0//0//5//"+requestURL+"//"+pathXML+"//None")
    
    #print(displays.count(None))
    #print(displays[0].side)
    #print(displays[1].side)
     
    #testValues=[1,30,7,8,17,20,25,30,40]
    testValues2=[40,50,60,70,20,30]
    #index=0
    index2=0
    
    #print(moveUpNumberLeds(1))

    while(True):
        for i in range(0,4):
            if displays[i] is not None:
                #if(i==0):
                 #   displays[i].value=testValues[index]
                 #   displays[i].displayRelative()
                 #   index=index+1
                #elif(i==1):
                if(i==1):
                    displays[i].value=testValues2[index2]
                    displays[i].displayFromBottomToTopMovingHeight()
                    index2=index2+1
               
                    print("value=",displays[i].value)
                    print("\n")
        
        time.sleep(10)
    #displayData()
    
    #while True:
     #   print("doing something...")
     #   time.sleep(1) 
    
def displayData():
    print("display data")
    for i in range(0,4):
            if(displays[i] is not None):
                displays[i].displayFromBottomToTop()
                print("value=",displays[i].value)
    
    nextCall =threading.Timer(10,displayData)
    nextCall.daemon=True
    nextCall.start()

#DISPLAY:SIDE//COLOR//BRIGHTNESS//REFERENCE//ADDSUBVALUE//STEPSIZE//URL//PATHXML//PATHJSON    #TODO: RANGEMAPPING

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
        addNewValueObject(requestURL,None,pathJson,referenceValue,scaleValue,addSubValue,stepSize,side,color,brightness)
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
    
    if(side=='A'):
        displays[0]=newValueObject
        #display.addNewValueObject(newValueObject,0)
    elif (side=='B'):
        displays[1]=newValueObject
       #display.addNewValueObject(newValueObject,1)
    elif(side=='C'):
        displays[2]=newValueObject
        #display.addNewValueObject(newValueObject,2)
    elif(side=='D'):
        displays[3]=newValueObject
        #display.addNewValueObject(newValueObject,3)
        
    #index=len(displays)-displays.count(None)
    #if(index<4):     
    #    displays[index]=newValueObject
    #else:       #override entry in displays list from begin to end
    #    displays[len(displays)%4]=newValueObject
    #print(newValueObject)


if __name__== '__main__':
    main()