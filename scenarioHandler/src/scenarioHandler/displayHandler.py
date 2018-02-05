import threading
import copy
import time

def singleton(cls):
    return cls()

@singleton            
class displayHandler():
    
    displays=[None]*4
    tempDisplays=[None]*4   
    delay=60
 

    def changeDelay(self,delay):
	self.delay= delay    
        
    def displayData(self):
        print("display data")
        self.applyTempDisplays()
 
        for i in range(0,4):
               	if(self.displays[i] is not None):
                   	self.displays[i].printValueHolder()
                   	self.displays[i].setValue()
                   
                 	self.displays[i].display()
                    	print("value=",self.displays[i].value)
	
	print "delay=",self.delay

        nextCall =threading.Timer(self.delay,self.displayData)
        nextCall.start()
        
     
    def addNewValueObject(self,newValueObject,index):
        self.tempDisplays[index]= newValueObject

    
    def applyTempDisplays(self):
        for i in range(0,4):
	   if(self.tempDisplays[i] is not None):
              self.displays[i]=self.tempDisplays[i]
              self.tempDisplays[i]=None
        

    def resetSide(self,index):
	self.displays[index]= None   
