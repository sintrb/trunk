/*
 * @brief I2C相关操作封装
 *
 *  Created on: Dec 17, 2013
 *      Author: trb
 */

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


/**
 * @brief 打开i2c设备
 *
 * @param i2cbus i2c总线
 *
 *
 * @return 返回文件句柄，失败返回负值
 */
int i2c_open_dev(int i2cbus) {
	int file;
	char filename[NAME_MAX];
	sprintf(filename, "/dev/i2c/%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file < 0) {
		sprintf(filename, "/dev/i2c-%d", i2cbus);
		file = open(filename, O_RDWR);
	}

	return file;
}

/**
 * @brief 强制修改需要通信的从设备地址，如需要非强制方式请使用ioctl
 *
 * @param i2cfd i2c设备文件
 * @param address 从设备地址
 *
 * @return 设置成功返回非负
 */
int i2c_change_slave(int i2cfd, int address)
{
	return ioctl(i2cfd, I2C_SLAVE_FORCE, address);
}

/**
 * @brief 读取指定的寄存器值
 *
 * @param i2cfd i2c设备文件
 * @param offset 寄存器位置
 * @param value 返回
 *
 * @return 成功返回1，其它为失败
 */
size_t i2c_read(int i2cfd, int offset, unsigned char *value)
{
	unsigned char addr = (unsigned char)offset;
	// 设置寄存器地址
	write(i2cfd, &addr, 1);

	// 读取
	return read(i2cfd, value, 1);
}

/**
 * @brief 从i2c设备中读取指定大小的数据
 *
 * @param i2cfd i2c设备文件
 * @param offset 起始寄存器位置
 * @param buf 读出缓冲区
 * @param size 需要读出的字节数
 *
 * @return 实际读出的数据长度
 */
size_t i2c_read_block(int i2cfd, int offset, void *buf, size_t size)
{
	unsigned char val = (unsigned char)offset;
	// 设置寄存器地址
	write(i2cfd, &val, 1);

	// 读取
	return read(i2cfd, buf, size);
}

/**
 * @brief 向取指定寄存器写值
 *
 * @param i2cfd i2c设备文件
 * @param offset 寄存器位置
 * @param value 需要写入的值
 *
 * @return 成功返回1，其它为失败
 */
size_t i2c_write(int i2cfd, int offset, unsigned char value)
{
	unsigned char buf[2];
	buf[0] = (unsigned char)offset;
	buf[1] = value;
	return write(i2cfd, buf, 2)-1;
}

/**
 * @brief 向i2c设备中写入指定大小的数据
 *
 * @param i2cfd i2c设备文件
 * @param offset 起始寄存器位置
 * @param buf 需要写入的数据缓冲区
 * @param size 需要写入的字节数
 *
 * @return 实际写入的数据长度, 非负表示成功
 */
size_t i2c_write_block(int i2cfd, int offset, void *buf, size_t size)
{
	unsigned char *ibuf = NULL;
	int ret = 0;
	ibuf = (unsigned char*)malloc(size+1);
	if(ibuf){
		ibuf[0] = (unsigned char)offset;
		memcpy(ibuf+1, buf, size);
		ret = write(i2cfd, ibuf, size+1) - 1;
		free(ibuf);
	}
	else{
		ret = -1;
	}
	return ret;
}

