import spi-connection

modes={'MOVE':1,'UP':2,'DOWN':3,'INIT':4,'ANI':5,'LIGHT':6,'LEVEL':7}
speeds={ 'FULL':1,'HALF':2,'QUARTER':3,'EIGTHTH':4}
initShortcut={'TOP':1,'BOTTOM':2,'CALIBRATE':3}
animations={'BLINK':1,'GLOW':2}
sides={'A':1,'B':2,'C':3,'D':4}
lightOperations={'ADD':1,'REMOVE':2,'CLEARSIDE':3}


def main():
	
	
def init_spi():
	spi = SPI(0,0)
	spi.open()

def send_arduino(message):
	spi.send_message(message)
	
def moveUp(stepNumber):
	message=[]
	message.append(mode["UP"])
	
	message.extend(getByteArrayFromNumber(400,2))
	
	send_arduino(message)
	
def moveDown(stepNumber):
	message=[]
	message.append(mode["DOWN"])
	
	message.extend(getByteArrayFromNumber(400,2))
	
	send_arduino(message)
	
def setLed(side,operation,led,color,brightness):
	message=[]
	message.append(mode["LIGHT"])
	message.append(getSideOperationLedByte(sides["A"],lightOperations["ADD"],led)
	
	message.extend(getByteArrayFromNumber(color,3))
	
	send_arduino(message)
	
def getByteArrayFromNumber(number,numberBytes):
	byteArray=[]
	for i in range(1,numberBytes+1):
		byteArray.append((number>>((numberBytes-i)*8))&255)
		
	return byteArray
	
def getSideOperationLedByte(side,operation,led):
	number=led
	number+= operation<<4
	number+= operation<<6
	
	return number
	
	
if __name__== '__main__':
	main()