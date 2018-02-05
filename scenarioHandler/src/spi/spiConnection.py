#!/usr/bin/python

import spidev
import time


class SPI:
        def __init__(self,bus,device):
                self.spi = spidev.SpiDev()
                self.device=device
                self.bus=bus

        def open(self):
                self.spi.open(self.bus,self.device)
                self.spi.max_speed_hz=100000

        def close(self):
                self.spi.close()

        def send_message(self,message):
                data=[]

                for c in message:
                        data.append(c)
                data.append(ord('\n'))

                try:
                        resp = self.spi.xfer2(list(data))
                        print "Send via SPI:" + "".join([str(c) for c in resp])
                        time.sleep(0.01)
                except KeyboardInterrupt:
                        self.spi.close()

            
