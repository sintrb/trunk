/*
 * @brief I2C相关操作封装
 *
 *  Created on: Dec 17, 2013
 *      Author: trb
 */

#ifndef I2CACCESS_H_
#define I2CACCESS_H_

/**
 * @brief 打开i2c设备
 *
 * @param i2cbus i2c总线
 *
 *
 * @return 返回文件句柄，失败返回负值
 */
int i2c_open_dev(int i2cbus);

/**
 * @brief 强制修改需要通信的从设备地址，如需要非强制方式请使用ioctl
 *
 * @param i2cfd i2c设备文件
 * @param address 从设备地址
 *
 * @return 设置成功返回非负
 */
int i2c_change_slave(int i2cfd, int address);

/**
 * @brief 读取指定的寄存器值
 *
 * @param i2cfd i2c设备文件
 * @param offset 寄存器位置
 * @param value 返回
 *
 * @return 成功返回1，其它为失败
 */
size_t i2c_read(int i2cfd, int offset, unsigned char *value);

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
size_t i2c_read_block(int i2cfd, int offset, void *buf, size_t size);

/**
 * @brief 向取指定寄存器写值
 *
 * @param i2cfd i2c设备文件
 * @param offset 寄存器位置
 * @param value 需要写入的值
 *
 * @return 写入的数据长度,成功返回1
 */
size_t i2c_write(int i2cfd, int offset, unsigned char value);


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
size_t i2c_write_block(int i2cfd, int offset, void *buf, size_t size);

#endif /* I2CACCESS_H_ */
