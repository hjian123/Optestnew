#include "canprotcl.h"
#include <string.h>

CanProtcl::CanProtcl(QObject *parent): QObject(parent)
{
    canTest = new CanTest(this);
    connect(canTest,SIGNAL(revChanged()),this,SLOT(onRevChanged()));
}

CanProtcl::~CanProtcl()
{
    delete canTest;
}

void CanProtcl::vStartTest()
{
//     finished = a;
    vSendPacket(FIRST_RPLY_ID,START_TEST);
}

void CanProtcl::vStopTest()
{
    vSendPacket(FIRST_RPLY_ID,STOP_TEST);
}

void CanProtcl::vSetMax(int a)
{
    maxValue = a;
    vSendPacket(FIRST_RPLY_ID,SET_MAXVALUE);
}

void CanProtcl::vSetMin(int a)
{
    minValue = a;
    vSendPacket(FIRST_RPLY_ID,SET_MINVALUE);
}

void CanProtcl::vSetAutoMan(bool a) //0:auto 1:man
{
    autoManual = a;
    vSendPacket(FIRST_RPLY_ID,SET_AUTO_MAN);
}

void CanProtcl::vHeartBeat()
{
    vSendPacket(FIRST_RPLY_ID,HEART_BEAT);
}

unsigned char CanProtcl::ucXOR(unsigned char *p_pcData, unsigned int p_uiLength)
{
    unsigned int i;
    unsigned char kh;
    kh=*p_pcData;
    for(i=1;i<p_uiLength;i++)
    {
        kh^=*(p_pcData+i);
    }
    return((unsigned char)(kh));
}

void CanProtcl::vSendPacket(unsigned int ID,unsigned char cmd)
{
    memset(&SendData,0,sizeof(SendData));
    switch(cmd){
        case HEART_BEAT:{
            SendData.ucLen = 5;
            break;
         }
        case START_TEST:{
            SendData.ucLen = 5;
            break;
        }
        case STOP_TEST:{
            SendData.ucLen = 6;
     //       SendData.ucBuf[4] = finished;
            break;
     }
        case SET_MAXVALUE:{
            SendData.ucLen = 8;
            SendData.ucBuf[4] = maxValue &0xff;
            SendData.ucBuf[5] = maxValue>>8 &0xff;
            SendData.ucBuf[6] = maxValue>>16 &0xff;
            break;
        }
        case SET_MINVALUE:{
            SendData.ucLen = 8;
            SendData.ucBuf[4] = minValue &0xff;
            SendData.ucBuf[5] = minValue>>8 &0xff;
            SendData.ucBuf[6] = minValue>>16 &0xff;
            break;
        }
        case SET_AUTO_MAN:{
            SendData.ucLen = 6;
            SendData.ucBuf[4] = autoManual;
            break;
        }
        default: break;
    }
    SendData.ucBuf[0] = frame_rq;
    SendData.ucBuf[1] = DST_ADDR;
    SendData.ucBuf[2] = cmd;
    SendData.ucBuf[3] = SendData.ucLen;

    SendData.ucBuf[SendData.ucLen-1] = ucXOR(SendData.ucBuf,SendData.ucLen-1);
    canTest->send(ID,SendData.ucLen,SendData.ucBuf);
}

bool CanProtcl::bCheckErrorRply(CanProtcl::tRevFrame *frame)
{
    static int len = 0;
    static int lencount = 0;
    if((frame->uiCanID & M_ID_CMD) == S_FIRST_RPLY) {    //收到从机第一条回复
 //       printf("S_FIRST_RPLY\n");
        lencount = 0;
        memset(&RevData,0,sizeof(RevData));  //清除接收缓存
        if(frame_rp != frame->ucFrameBuf[0]) {
            printf("Frame type error: not a reply frame !\n");
            lencount = 0;
            return false;
        }
        else if(SRC_ADDR != frame->ucFrameBuf[1]){
            printf("Frame Address error: not a host address !\n");
            lencount = 0;
            return false;
        }
    /*    else if((SendData.ucBuf[2] | 0x80)!= frame->ucFrameBuf[2]){
            printf("first Cmd error !\n");
            lencount = 0;
            return false;
        }*/

        lencount += frame->uiFrameLen; //此次CAN数据长度
        RevData.ucLen = lencount;
        memcpy(RevData.ucBuf,frame->ucFrameBuf,frame->uiFrameLen); //
        len = frame->ucFrameBuf[3]; //保存帧长度
        vSendPacket(NEXT_RPLY_ID,SendData.ucBuf[2]);
        return true;
    }
    else if((frame->uiCanID & M_ID_CMD) == S_LAST_RPLY) { //收到从机最后一条回复
         if(lencount == 0){
            if(frame_rp != frame->ucFrameBuf[0]) {
                printf("Frame type error: not a reply frame !\n");
                return false;
            }
            else if(SRC_ADDR != frame->ucFrameBuf[1]){
                printf("Frame Address error: not a host address !\n");
                return false;
            }
      /*      else if((SendData.ucBuf[2] | 0x80)!= frame->ucFrameBuf[2]){
                printf("last Cmd error !\n");
                return false;
            }*/
            else if(frame->uiFrameLen  != frame->ucFrameBuf[3]) {     //检查长度
                 printf("Frame length error !\n");
                 return false;
             }
            else if(frame->ucFrameBuf[frame->uiFrameLen-1] !=
                    ucXOR(frame->ucFrameBuf,frame->uiFrameLen-1)) {  //检查CRC
                 printf("Frame CRC error !\n");
                 return false;
             }
            RevData.ucLen = frame->uiFrameLen; //此次CAN数据长度
            memcpy(RevData.ucBuf,frame->ucFrameBuf,frame->uiFrameLen); //
         }
         else {     //检查长度
            memcpy(RevData.ucBuf+RevData.ucLen,frame->ucFrameBuf,frame->uiFrameLen); //
            lencount += frame->uiFrameLen; //此次CAN数据长度
            RevData.ucLen = lencount;
            if(lencount != len) {
                printf("Frame length error ! lencount =%d len =%d\n",lencount,len);
                memset(&RevData,0,sizeof(RevData));  //清除接收缓存
                lencount = 0;
                return false;
            }
            else if(frame->ucFrameBuf[frame->uiFrameLen-1] !=
                    ucXOR(RevData.ucBuf,RevData.ucLen-1)) {  //检查CRC
                printf("Frame CRC error !\n");
                memset(&RevData,0,sizeof(RevData));  //清除接收缓存
                lencount = 0;
                return false;
            }
        }
         lencount = 0;
         emit msgChanged();
         return true;
    }
    else if((frame->uiCanID & M_ID_CMD) == S_NEXT_RPLY) {  //收到从机继续回复
        memcpy(RevData.ucBuf+RevData.ucLen,frame->ucFrameBuf,frame->uiFrameLen); //
        lencount += frame->uiFrameLen; //此次CAN数据长度 += frame->uiFrameLen; //此次CAN数据长度
        RevData.ucLen = lencount;
        vSendPacket(NEXT_RPLY_ID,SendData.ucBuf[2]);
        return true;
    }
    else return true;
}

 void CanProtcl::onRevChanged()
{
        tRevFrame revFrame;
        memcpy(revFrame.ucFrameBuf,canTest->recv_frame.data,canTest->recv_frame.can_dlc);
        revFrame.uiFrameLen = canTest->recv_frame.can_dlc;
        revFrame.uiCanID = canTest->recv_frame.can_id;

        bCheckErrorRply(&revFrame);
}

