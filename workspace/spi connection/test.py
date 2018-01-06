


def main():
	
	number=400
	byte1=number & 255
	byte2=(number>>8)&255
	
	print(byte1,byte2)
	
	print(getByteArrayFromNumber(400,2))
	
	print(int("FFFFFF",16))
	print(int("0xFFFFFF",16))
	
	#print(int(0,16))
	
	print(round(3.6))
	print(round(3.5))
	
	print(2<<4)
	
	
def getByteArrayFromNumber(number,numberBytes):
	byteArray=[]
	for i in range(1,numberBytes+1):
		byteArray.append((number>>((numberBytes-i)*8))&255)
		
	return byteArray
	
if __name__== '__main__':
	main()