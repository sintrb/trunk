trunk
=====

only a code trunk

#### i2c_code_v3
##### A demo of BeagleBone I2C demo
###### see [http://www.element14.com/community/message/77591](http://www.element14.com/community/message/77591)

### BeageleBone/BeagleBone Black

#### Python I2C Test
##### Using smbus to implement.
##### see [/beaglebone/i2ctest.py](https://github.com/sintrb/trunk/blob/master/beaglebone/i2ctest.py)
###### use usage:
> if you didn't know witch I2C bus or witch I2C address.
> you can user 'i2cdete -r ?' to scan x I2C bus.
> after you know the I2C bus and I2C address.
> you can test it like follow:
> >python i2ctest.py 1 0x22 1024 32


#### C Language Can Bus Test
##### see [/beaglebone/cantraf.c](https://github.com/sintrb/trunk/blob/master/beaglebone/cantraf.c)
###### use usage:
> before test, you may ready the can bus.
> the follow is set can0 bus to bitrate 250kbps
> >bash readycan.sh 250000
> then test can0 receive performance:
> >./cantraf
> after BeagleBone Black is ready to test, you can device( a nother can bus device)
> can send data to BeagleBone Black( wire link is necessary).
> 
> of course, you can test send ( or transmition) performance:
> >./cantraf 1024
> the, BeagleBone Black will send 1024 bytes data to you another can bus device.
>