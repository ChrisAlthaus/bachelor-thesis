import threading
import copy
import time

def singleton(cls):
    return cls()

@singleton            
class displayHandler():
    
    displays=[None]*4
    tempDisplays=[None]*4   
    #testValues=[40,45,50,55,60,70,20,30,40]
    #index=0
    delay=60
   # def __init__(self,delay):
    #    self.delay=delay    #delay in seconds
    #TODO: copy display list to avoid NoneType error
    def changeDelay(self,delay):
	self.delay= delay    
        
    def displayData(self):
        print("display data")
        self.applyTempDisplays()
 
	#threadDisplays = copy.deepcopy(self.displays)
        for i in range(0,4):
                if(self.displays[i] is not None):
                    print "side=", self.displays[i].side
                    self.displays[i].printValueHolder()
                    self.displays[i].setValue()
                    #self.displays[i].value=self.testValues[self.index]
                    #self.index= self.index + 1
                    #self.displays[i].displayFromBottomToTopMovingHeight()
                    self.displays[i].display()
                    print("value=",self.displays[i].value)
        print "delay=",self.delay
        #self.displays= copy.deepcopy(threadDisplays)

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
        #setSingleAnimation("OFF",
	self.displays[index]= None   
