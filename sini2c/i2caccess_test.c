/**
 * BeagleBone Black I2C Bus测试
 * trb
 * 2013-12-17
 * ©ynsoft.cn
 */

// 相关头文件：
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "i2caccess.h"

int main(int argc, char *argv[]) {
	int fd, i;
	unsigned char value;
	unsigned char wbuf[] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
	unsigned char rbuf[sizeof(wbuf)];

	// 打开i2c总线1，需要根据具体情况来的打开
	fd = i2c_open_dev(1);

	if (fd < 0) {
		// 打开失败，可能是没有该i2c总线
		printf("open i2c fail %d\n", fd);
		exit(1);
	}

	// 设置i2c从设备地址为0x22
	if(i2c_change_slave(fd, 0x22)<0){
		printf("set slave failed\n");
		close(fd);
		exit(2);
	}

	printf("byte test...\n");
	// 单字节写入
	printf("write..\n");
	for(i=0; i<256; ++i){
		value = i;
		if(i2c_write(fd, i, value)==1){
			// 写入成功
			printf("[%02x]=%02x ", i, value);
		}
		else{
			// 写入失败
			printf("[%02x]=XX ", i);
		}
		if(i%8==7)
			printf("\n");
	}
	// 读取
	printf("read..\n");
	for(i=0; i<256; ++i){
		if(i2c_read(fd, i, &value)==1){
			// 读取成功
			printf("[%02x]=%02x ", i, value);
		}
		else{
			// 读取失败
			printf("[%02x]=XX ", i);
		}
		if(i%8==7)
			printf("\n");
	}

	printf("block test...\n");
	// 块写入
	if(i2c_write_block(fd, 0, wbuf, sizeof(wbuf))!=sizeof(wbuf)){
		printf("i2c_write_block failed\n");
	}
	// 块读出
	if(i2c_read_block(fd, 0, rbuf, sizeof(rbuf))!=sizeof(rbuf)){
		printf("i2c_read_block failed\n");
	}
	else{
		for(i=0; i<sizeof(rbuf); ++i){
			printf("r[%02x]=%02x\n", i, rbuf[i]);
		}
	}
	// 最后关闭文件
	close(fd);
	return 0;
}
