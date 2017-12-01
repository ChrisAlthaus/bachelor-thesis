#!/usr/bin/python

import spidev
import time

class SPI:
	def __init__(self,bus,device):
		self.spi = spidev.SpiDev()
		spi.max_speed_hz=1000000
		
	def open(self):
		self.spi.open(bus,device)
		
	def close():
		self.spi.close(self)
		
	def send_message(self,message):
		data=[]
		
		for c in message:
			data.append(ord(c))
		data.append(ord('\n'))
		
		try:
			resp = spi.xfer2(list(data))
			print "Send via SPI:" + "".join([chr(c) for c in resp])
			time.sleep(0.01)
		except KeyboardInterrupt:
			spi.close()
			