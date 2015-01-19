#ifndef MODBUS_H
#define MODBUS_H
//#include "config.h"

// 功能码
#define MB_FUNC_READREGISTERS   0x03    // 读多个寄存器
#define MB_FUNC_WRITEREGISTER   0x06    // 写单个寄存器
#define MB_FUNC_WRITEREGISTERS  0x10    // 写单个寄存器


#define MB_BUF_MAXLENGTH    64  // 缓冲区大小

#define HLBYTE2U16(_buf, _ix)	((0x00ff & _buf[_ix])<<8 | (0x00ff & _buf[(_ix)+1]))
#define LHBYTE2U16(_buf, _ix)	((0x00ff & _buf[(_ix)+1])<<8 | (0x00ff & _buf[_ix]))

#define U162HLBYTE(_buf, _ix, _v)	{_buf[(_ix)] = (unsigned char)((_v)>>8); _buf[(_ix)+1] = (unsigned char)((_v)&0x00ff);}
#define U162LHBYTE(_buf, _ix, _v)	{_buf[(_ix)+1] = (unsigned char)((_v)>>8); _buf[(_ix)] = (unsigned char)((_v)&0x00ff);}

#ifndef u8
typedef unsigned char u8;
#endif // u8

#ifndef u16
typedef unsigned short u16;
#endif // u16


// 接收器状态
#define MB_RECEIVER_STATUS_NONE         0   // 没有开始接收
#define MB_RECEIVER_STATUS_RECEIVING    1   // 正在接收
#define MB_RECEIVER_STATUS_RECEIVED     2   // 接收到一帧数据

// Modbus接收器
struct mb_receiver{
    u8 status;  // 状态
    u8 datapos; // 指针
    u8 databuf[MB_BUF_MAXLENGTH];   // 缓冲区
};
typedef struct mb_receiver mb_receiver_t;


// modbus 接收器
extern mb_receiver_t mb_receiver;

// 初始化modbus
void mb_init(void);

// 计算CRC16校验值
u16 mb_calcu_crc(const u8 *dats, u16 len);

// 初始化Modbus数据帧接收器
void mb_reset_receiver(mb_receiver_t *recv);

// 处理收到的字节数据，在串口中断里面调用
void mb_receive_byte(mb_receiver_t *recv, u8 dat);

// 接收超时，在定时器终端里面调用
u16 mb_event_timeout(mb_receiver_t *recv);



#if !MB_SLAVE_ID

// 主机特有

#define MB_TIMEOUT		100	// Modbus超时（单位为时间间隔）

#define MB_MASTER_STATUS_NONE			0			// 空状态
#define MB_MASTER_STATUS_WAITTING	1			// 等待从机数据
#define MB_MASTRT_STATUS_TIMEOUT	2			// 等待超时
#define MB_MASTER_STATUS_RESOK		3			// 收到从机响应
#define MB_MASTER_STATUS_RESERROR	255		// 从机响应出错

/*
u8* mb_read_registers(u8 address, u16 reg_start, u16 reg_count);
void mb_write_register(u8 address, u16 reg_addr, u16 value);
void mb_write_registers(u8 address, u16 reg_start, u16 reg_count, const u16 *data);
*/
#endif

// 需实现函数
// 发送一个字节
void mb_send_byte(u8 dat);

#if MB_SLAVE_ID
// 从机事件
// 正常返回0，否则返回错误码
// 读多个寄存器事件
u8 mb_event_read_registers(u8 *outbuf, u16 reg_start, u16 reg_count);
// 写单个寄存器事件
u8 mb_event_write_register(u16 reg_addr, u16 value);
// 读多个寄存器事件
u8 mb_event_write_registers(u8 *inbuf, u16 reg_start, u16 reg_count);
#else
// 主机事件
// 读多个寄存器响应
void mb_response_read_registers(u8 address, u8 bytesize, u8 *inbuf);
// 写单个寄存器响应
void mb_response_write_register(u8 address, u16 reg_addr, u16 value);
// 写多个寄存器响应
void mb_response_write_registers(u8 address, u16 reg_start, u16 reg_count);
// 响应错误，错误码，异常码
void mb_response_error(u8 address, u8 err_code, u8 exp_code);

// 主机方法
// 写单个寄存器
void mb_write_register(u8 address, u16 reg_addr, u16 value);
// 读取多个寄存器
void mb_read_registers(u8 address, u16 reg_start, u16 reg_count);
// 写多个寄存器
void mb_write_registers(u8 address, u16 reg_start, u16 reg_count, u16 *values);

#endif // MB_SLAVE_ID



#endif // MODBUS_H
