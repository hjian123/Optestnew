#ifndef CANPROTCL_H
#define CANPROTCL_H

/***********************************************************
������CAN֡�ṹ����׽��ֵ�ַ�ṹ�嶨����include/linux/can.h��


* ��չ��ʽʶ����� 29 λ��ɡ����ʽ�����������֣�11 λ���� ID��18 λ��չ ID��
* Controller Area Network Identifier structure
*
* bit 0-28     : CANʶ��� (11/29 bit)
* bit 29     : ����֡��־ (0 = data frame, 1 = error frame)
* bit 30     : Զ�̷��������־ (1 = rtr frame)
* bit 31     :֡��ʽ��־ (0 = standard 11 bit, 1 = extended 29 bit)

typedef __u32 canid_t;

struct can_frame {
     canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags
      __u8    can_dlc; / /���ݳ���: 0 .. 8 /
      __u8    data[8] __attribute__((aligned(8)));
 };

************************************************************/
#include "cantest.h"
// ͨ������
#define HEART_BEAT 0x30//
#define START_TEST 0x31	//
#define STOP_TEST  0x32	//
#define SET_DETAILS   0x33	//
#define SET_AUTO_MAN 0x34	//

#define CFG_ID          0x33	// �����豸ID
#define GET_DEVINFO_1   0x34	// �豸��Ϣ��ѯ


#define ID_FIRST  1<<27     //ID27
#define ID_LAST   1<<26     //ID26
#define ID_RQ_RP  1<<25     //ID25
#define ID_RE_WR  1<<24     //ID24
#define EFF   1<<31     //IDE 0:��׼��ʶ�� 1: ��չ��ʶ��
#define RTR  1<<30    // 1:Զ�� 0: ����
#define ERR  1<<29    //(0 = data frame, 1 = error frame)

#define M_ID_CMD     (ID_FIRST | ID_LAST | ID_RQ_RP)
#define M_FIRST_CMD  (ID_FIRST | ID_LAST) //��������������Ӧ�ı�ʶ
#define M_NEXT_CMD   0x00                          //�����������������Ӧ�ı�ʶ
#define S_FIRST_RPLY  (ID_FIRST | ID_RQ_RP) //�ӻ��״���Ӧ��Ӧ�ı�ʶ
#define S_LAST_RPLY  (ID_LAST  | ID_RQ_RP) //�ӻ����һ����Ӧ��Ӧ�ı�ʶ
#define S_NEXT_RPLY  ID_RQ_RP //�ӻ�������Ӧ��Ӧ�ı�ʶ


#define ID_SRC_ADDR  0xff<<16 //ID13-ID20
#define ID_DST_ADDR  0xff<<8  //ID5-ID12

#define SRC_ADDR  0xee
#define DST_ADDR  0x01

#define FIRST_RPLY_ID  M_FIRST_CMD|(SRC_ADDR<<13)|(DST_ADDR<<5)|EFF
#define LAST_RPLY_ID   S_LAST_RPLY|(SRC_ADDR<<13)|(DST_ADDR<<5)|EFF
#define NEXT_RPLY_ID   M_NEXT_CMD|(SRC_ADDR<<13)|(DST_ADDR<<5)|EFF

#define CRMBUFFSIZE 20
#define REVBUFFERSIZE 20

typedef struct
{
    unsigned char ucBuf[CRMBUFFSIZE];	// ���ݻ�����
    unsigned char ucLen;				// ���ݳ���
}tCanData;
class CanProtcl : public QObject
{
    Q_OBJECT
public:
   explicit CanProtcl(QObject *parent = 0);
    ~CanProtcl();

    tCanData SendData;
    tCanData RevData;
    void vStartTest(void);
    void vStopTest(void);
    void vSet(void);
    void vSetAutoMan(bool a);
    void vHeartBeat(void);
 private:
    typedef enum
    {
        frame_rq = 0x00,  //����֡
        frame_rp = 0x01   //��Ӧ֡
    }eFRAME_TYPE;

    typedef struct
    {
        eFRAME_TYPE frame_type;
        unsigned char ucnode_addr;
        unsigned char ucdata_type;
        unsigned char uclen;
    }tFrameHead;

    typedef struct
    {
        unsigned char ucFrameBuf[CRMBUFFSIZE];	// ��������z֡
        unsigned int uiFrameLen;			    // ��������֡����
        unsigned int  uiCanID;
        unsigned char  uiCmdType;
    }tRevFrame;

    unsigned char ucXOR(unsigned char *p_pcData, unsigned int p_uiLength);
    void vSendPacket(unsigned int ID,unsigned char cmd);
//    void vCheckPacket(tRevFrame *frame);//֡����
    bool bCheckErrorRply(tRevFrame *frame);

    CanTest  *canTest;
    bool  autoManual;
signals:
    void msgChanged(); //���յ�������һ֡���ݺ��͸��ź�
protected slots:
   void onRevChanged();//���յ�һ��CAN����֡
};

#endif // CanProtcl_H
