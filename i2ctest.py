#!/usr/bin/python


# used smbus to connect i2c bus
# if python-smbus not installed, use the follow comman to install it.
# #opkg install python-smbus

import sys
import smbus
import time

i2cch = 1 # channel number, e.g 0,1,2,3 means i2c-0, i2c-1, i2c-2, i2c-3.
devaddr = 0x22 # i2c device address, you can use "i2cdete -r 1" to find it.
testcount = 256	# write or read count for test.

bus = smbus.SMBus(i2cch)

def help():
	name = 'i2ctest.py'
	print '%s [i2cch devaddr testcount]'%name
	print '\t i2cch: i2c bus channel number, default %d'%i2cch
	print '\t devaddr: i2c device address to test, default %d'%devaddr
	print '\t testcount: write or read count for test, default %d'%testcount
	

def test_write():
	stm = time.time()
	for i in range(testcount):
		n = i%256
		bus.write_byte_data(devaddr, n, n)
	etm = time.time()
	tms = int((etm-stm)*1000)
	print 'write:\n\tcount:%d byte\n\ttime:%dms\n\tspeed:%d byte/s\n'%(testcount, tms, 
testcount*1000/tms)

def test_read():
	stm = time.time()
	errct = 0
	for i in range(testcount):
		n = i%256
		m = bus.read_byte_data(devaddr, n)
		if n != m:
			# read fail
			errct = errct + 1
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
		if len(sys.argv) == 4:
			argv = sys.argv
			try:
				i2cch = eval(argv[1])
				devaddr = eval(argv[2])
				testcount = eval(argv[3])
				print 'i2cch:%s devaddr:%s testcount:%s'%tuple(argv[1:])
			except:
				help()
				exit(1)
		test_write()
		test_read()

	

