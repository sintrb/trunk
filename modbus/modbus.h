#ifndef MODBUS_H
#define MODBUS_H
//#include "config.h"

// ������
#define MB_FUNC_READREGISTERS   0x03    // ������Ĵ���
#define MB_FUNC_WRITEREGISTER   0x06    // д�����Ĵ���
#define MB_FUNC_WRITEREGISTERS  0x10    // д�����Ĵ���


#define MB_BUF_MAXLENGTH    64  // ��������С

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


// ������״̬
#define MB_RECEIVER_STATUS_NONE         0   // û�п�ʼ����
#define MB_RECEIVER_STATUS_RECEIVING    1   // ���ڽ���
#define MB_RECEIVER_STATUS_RECEIVED     2   // ���յ�һ֡����

// Modbus������
struct mb_receiver{
    u8 status;  // ״̬
    u8 datapos; // ָ��
    u8 databuf[MB_BUF_MAXLENGTH];   // ������
};
typedef struct mb_receiver mb_receiver_t;


// modbus ������
extern mb_receiver_t mb_receiver;

// ��ʼ��modbus
void mb_init(void);

// ����CRC16У��ֵ
u16 mb_calcu_crc(const u8 *dats, u16 len);

// ��ʼ��Modbus����֡������
void mb_reset_receiver(mb_receiver_t *recv);

// �����յ����ֽ����ݣ��ڴ����ж��������
void mb_receive_byte(mb_receiver_t *recv, u8 dat);

// ���ճ�ʱ���ڶ�ʱ���ն��������
u16 mb_event_timeout(mb_receiver_t *recv);



#if !MB_SLAVE_ID

// ��������

#define MB_TIMEOUT		100	// Modbus��ʱ����λΪʱ������

#define MB_MASTER_STATUS_NONE			0			// ��״̬
#define MB_MASTER_STATUS_WAITTING	1			// �ȴ��ӻ�����
#define MB_MASTRT_STATUS_TIMEOUT	2			// �ȴ���ʱ
#define MB_MASTER_STATUS_RESOK		3			// �յ��ӻ���Ӧ
#define MB_MASTER_STATUS_RESERROR	255		// �ӻ���Ӧ����

/*
u8* mb_read_registers(u8 address, u16 reg_start, u16 reg_count);
void mb_write_register(u8 address, u16 reg_addr, u16 value);
void mb_write_registers(u8 address, u16 reg_start, u16 reg_count, const u16 *data);
*/
#endif

// ��ʵ�ֺ���
// ����һ���ֽ�
void mb_send_byte(u8 dat);

#if MB_SLAVE_ID
// �ӻ��¼�
// ��������0�����򷵻ش�����
// ������Ĵ����¼�
u8 mb_event_read_registers(u8 *outbuf, u16 reg_start, u16 reg_count);
// д�����Ĵ����¼�
u8 mb_event_write_register(u16 reg_addr, u16 value);
// ������Ĵ����¼�
u8 mb_event_write_registers(u8 *inbuf, u16 reg_start, u16 reg_count);
#else
// �����¼�
// ������Ĵ�����Ӧ
void mb_response_read_registers(u8 address, u8 bytesize, u8 *inbuf);
// д�����Ĵ�����Ӧ
void mb_response_write_register(u8 address, u16 reg_addr, u16 value);
// д����Ĵ�����Ӧ
void mb_response_write_registers(u8 address, u16 reg_start, u16 reg_count);
// ��Ӧ���󣬴����룬�쳣��
void mb_response_error(u8 address, u8 err_code, u8 exp_code);

// ��������
// д�����Ĵ���
void mb_write_register(u8 address, u16 reg_addr, u16 value);
// ��ȡ����Ĵ���
void mb_read_registers(u8 address, u16 reg_start, u16 reg_count);
// д����Ĵ���
void mb_write_registers(u8 address, u16 reg_start, u16 reg_count, u16 *values);

#endif // MB_SLAVE_ID



#endif // MODBUS_H
