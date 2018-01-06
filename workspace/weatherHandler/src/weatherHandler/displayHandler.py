import threading
from weatherHandler import addNewValueObject

class displayHandler():
    
    displays=[None]*4   
    
    def __init__(self,delay):
        threading.Thread.__init__(self)
        self.delay=delay
        
    def run(self):
        for i in range(0,4):
            if(self.displays[i] is not None):
                self.displays[i].displayFromBottomToTop()
        
        
    def addNewValueObject(self,newValueObject,index):
        self.displays[index]= newValueObject