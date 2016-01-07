#ifndef CANPROTCL_H
#define CANPROTCL_H

/***********************************************************
基本的CAN帧结构体和套接字地址结构体定义在include/linux/can.h：


* 扩展格式识别符由 29 位组成。其格式包含两个部分：11 位基本 ID、18 位扩展 ID。
* Controller Area Network Identifier structure
*
* bit 0-28     : CAN识别符 (11/29 bit)
* bit 29     : 错误帧标志 (0 = data frame, 1 = error frame)
* bit 30     : 远程发送请求标志 (1 = rtr frame)
* bit 31     :帧格式标志 (0 = standard 11 bit, 1 = extended 29 bit)

typedef __u32 canid_t;

struct can_frame {
     canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags
      __u8    can_dlc; / /数据长度: 0 .. 8 /
      __u8    data[8] __attribute__((aligned(8)));
 };

************************************************************/
#include "cantest.h"
// 通信命令
#define HEART_BEAT 0x30//
#define START_TEST 0x31	//
#define STOP_TEST  0x32	//
#define SET_DETAILS   0x33	//
#define SET_AUTO_MAN 0x34	//

#define CFG_ID          0x33	// 配置设备ID
#define GET_DEVINFO_1   0x34	// 设备信息查询


#define ID_FIRST  1<<27     //ID27
#define ID_LAST   1<<26     //ID26
#define ID_RQ_RP  1<<25     //ID25
#define ID_RE_WR  1<<24     //ID24
#define EFF   1<<31     //IDE 0:标准标识符 1: 扩展标识符
#define RTR  1<<30    // 1:远程 0: 数据
#define ERR  1<<29    //(0 = data frame, 1 = error frame)

#define M_ID_CMD     (ID_FIRST | ID_LAST | ID_RQ_RP)
#define M_FIRST_CMD  (ID_FIRST | ID_LAST) //主机发起的命令对应的标识
#define M_NEXT_CMD   0x00                          //主机继续请求命令对应的标识
#define S_FIRST_RPLY  (ID_FIRST | ID_RQ_RP) //从机首次响应对应的标识
#define S_LAST_RPLY  (ID_LAST  | ID_RQ_RP) //从机最后一次响应对应的标识
#define S_NEXT_RPLY  ID_RQ_RP //从机继续响应对应的标识


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
    unsigned char ucBuf[CRMBUFFSIZE];	// 数据缓冲区
    unsigned char ucLen;				// 数据长度
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
        frame_rq = 0x00,  //请求帧
        frame_rp = 0x01   //响应帧
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
        unsigned char ucFrameBuf[CRMBUFFSIZE];	// 接收数据z帧
        unsigned int uiFrameLen;			    // 接收数据帧长度
        unsigned int  uiCanID;
        unsigned char  uiCmdType;
    }tRevFrame;

    unsigned char ucXOR(unsigned char *p_pcData, unsigned int p_uiLength);
    void vSendPacket(unsigned int ID,unsigned char cmd);
//    void vCheckPacket(tRevFrame *frame);//帧解析
    bool bCheckErrorRply(tRevFrame *frame);

    CanTest  *canTest;
    bool  autoManual;
signals:
    void msgChanged(); //接收到完整的一帧数据后发送该信号
protected slots:
   void onRevChanged();//接收到一个CAN数据帧
};

#endif // CanProtcl_H
