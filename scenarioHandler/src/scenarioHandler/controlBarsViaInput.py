import sys
sys.path.append('..')

from messageHandler import *



def main():
	while(True):
		enteredText = raw_input("Type a command:")
		
		if(enteredText=='UP'):
			moveUp(400)
		elif(enteredText=='DOWN'):
			moveDown(400)
		elif(enteredText=='MOVE'):
			moveToPercentage(50,"HALF")
		elif(enteredText=="CALIBRATE"):
			calibrate()
		elif(enteredText=="LIGHT"):
			arguments= raw_input("Enter arguments:")
			values=arguments.split(",")
			
			side=values[0]
			operation=values[1]
			led=int(values[2])
			color=int(values[3],16)
	
			setLed(side,operation,led,color)
		elif(enteredText=='ANI'):
		        arguments= raw_input("Enter arguments:")
			values=arguments.split(",")

                       	type=values[0]

			if type=="SINGLE":
				animation=values[1]
				side=values[2]
				led=int(values[3])
				durationOneCycle=int(values[4])
                       		color=int(values[5],16)
                        	brightness=int(values[6])

				setSingleAnimation(animation,side,led,durationOneCycle,color,brightness)
			elif type=="OVERALL":
				animation=values[1]
                        	color=int(values[2],16)
                        	brightness=int(values[3])
                        	speed=int(values[4])

				setAnimation(animation,color,brightness,speed)

if __name__== '__main__':
    main()
