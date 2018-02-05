	import threading
	
	            
	class displayHandler():
	    
	    displays=[None]*4   
	    
	    def __init__(self,delay):
	        threading.Thread.__init__(self)
	        self.delay=delay    #delay in seconds
	        
	        
	    def displayData(self):
	        print("display data")
	        for i in range(0,4):
	                if(self.displays[i] is not None):
	                    self.displays[i].display()
	        
	        nextCall =threading.Timer(self.delay,self.displayData)
	        nextCall.start()
	        
	        
	        
	     
	    def addNewValueObject(self,newValueObject,index):
	        self.displays[index]= newValueObject



