#!/usr/bin/python


# used smbus to connect i2c bus
# if python-smbus not installed, use the follow comman to install it(for BeagleBone or BeagleBone Black).
# #opkg install python-smbus

import sys
import smbus
import time

i2cch = 1 # channel number, e.g 0,1,2,3 means i2c-0, i2c-1, i2c-2, i2c-3.
devaddr = 0x22 # i2c device address, you can use "i2cdete -r 1" to find it.
testcount = 256	# write or read count for test.
step = 1 # step

def help():
	name = 'i2ctest.py'
	print '%s [i2cch devaddr testcount]'%name
	print '\t i2cch: i2c bus channel number, default %d'%i2cch
	print '\t devaddr: i2c device address to test, default %d'%devaddr
	print '\t testcount: write or read count for test, default %d'%testcount
	print '\t step: write or read by one address, default %d'%step

def test_write():
	stm = time.time()
	if step>1:
		data = range(step)
	for i in range(0, testcount, step):
		n = i%256
		if step==1:
			bus.write_byte_data(devaddr, n, n)
		else:
			bus.write_i2c_block_data(devaddr, n, data)
	etm = time.time()
	tms = int((etm-stm)*1000)
	print 'write:\n\tcount:%d byte\n\ttime:%dms\n\tspeed:%d byte/s\n'%(testcount, tms, 
testcount*1000/tms)

def test_read():
	stm = time.time()
	errct = 0
	if step>1:
		data = range(step)
	for i in range(0, testcount, step):
		n = i%256
		if step==1:
			m = bus.read_byte_data(devaddr, n)
		else:
			m = bus.read_i2c_block_data(devaddr, n, step)[0]
		if n != m:
			errct = errct +1
	etm = time.time()
	tms = int((etm-stm)*1000)
	print 'read:\n\tcount:%d byte\n\ttime:%dms\n\tspeed:%d byte/s\n'%(testcount, tms,
testcount*1000/tms)
	print 'error count:%d byte'%errct

if __name__ == '__main__':
	if len(sys.argv) > 1 and (sys.argv[1] == '?' or sys.argv[1] == '-h'):
		help()
		exit(0)
	else:
		argv = sys.argv
		argc = len(argv)
		try:
			if argc > 1:
				i2cch = eval(argv[1])
			if argc > 2:
				devaddr = eval(argv[2])
			if argc > 3:
				testcount = eval(argv[3])
			if argc > 4:
				step = eval(argv[4])
					
		except:
			print 'errot args'
			help()
			exit(1)
		bus = smbus.SMBus(i2cch)
		test_write()
		test_read()

	

