#include "cantest.h"

CanTest::CanTest(QObject *parent) : QObject(parent)
{
    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
//    connect(this,SIGNAL(revChanged()),this,SLOT(onRevChanged()));
    send_socket_fd = socket_listen("can0");
    recv_socket_fd = socket_listen("can0");
  //  socket_can_filter();
    timer->start(100);
}

CanTest::~CanTest()
{
    close(send_socket_fd);
    close(recv_socket_fd);
    delete timer;
}

int CanTest::recv()
{
    struct timeval timeout;
    fd_set fs_read;
    int ret;

    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    FD_ZERO(&fs_read);
    FD_SET(recv_socket_fd,&fs_read);

    ret = select(recv_socket_fd+1,&fs_read,NULL,NULL,&timeout);
 //   ret = read(recv_socket_fd,&recv_frame,sizeof(can_frame)); //该函数为阻塞模式，直到读到数据
    if (ret == -1)
         perror("select()");
   else if (ret){
  //       printf("Data is available now.\n");
                   /* FD_ISSET(0, &rfds) will be true. */
          ret = read(recv_socket_fd,&recv_frame,sizeof(can_frame));
    }
   else{
   //     printf("No data within 100 microseconds.\n");
  }
    return ret;
}

int CanTest::send(unsigned int ID,unsigned char DLC,const unsigned char *buf)
{
        unsigned int ret;

        struct can_frame send_frame;
        send_frame.can_id = ID;

        memcpy((unsigned char *)send_frame.data,buf,DLC);

        send_frame.can_dlc = DLC;

        ret = write(send_socket_fd,&send_frame,sizeof(send_frame));
//        nbytes = sendto(send_socket_fd,&send_frame,sizeof(struct can_frame),
 //                       0,(struct sockaddr*)&send_addr,sizeof(send_addr));
  //      printf("Send a CAN frame from interface %s %d bytes\n",send_ifr.ifr_name,nbytes);
        return ret;
}

int CanTest::socket_listen(const char *port)
{
    int socket_fd = -1;
    struct sockaddr_can addr;
    struct ifreq ifr;
    int ret = 0;
    int loopback;

    socket_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);
    if(socket_fd < 0){
        printf("Openning socket %s failed \n",port);
        return -1;
    }

    strcpy((char *)ifr.ifr_name,port);
    ret = ioctl(socket_fd,SIOCGIFINDEX,&ifr);
    if(ret < 0){
        printf("Matching socket %s failed \n",port);
        return -1;
    }
    else printf("can_ifindex = %x\n",ifr.ifr_ifindex);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0){
        close(socket_fd);
        printf("Binding socket %s failed \n",port);
        return -1;
    }
    loopback = 0;
    setsockopt(socket_fd,SOL_CAN_RAW,CAN_RAW_LOOPBACK,&loopback,sizeof(loopback));
    return socket_fd;
}
/*
int CanTest::socket_can_filter()
{
    const int n = 1;
    struct can_filter rfilter[n];

    rfilter[0].can_id = 0x00;
    rfilter[0].can_mask = CAN_EFF_MASK;
    //发送套接字禁止接收
    setsockopt(send_socket_fd,SOL_CAN_RAW,CAN_RAW_FILTER,&rfilter,sizeof(rfilter));
    setsockopt(recv_socket_fd,SOL_CAN_RAW,CAN_RAW_FILTER,NULL,0);
    return 0;
}*/

void CanTest::timerUpdate()
{
 //   printf(" time updata!!! :\n");
    int i;
    timer->stop();
    if(recv() > 0){
        printf("Receive %d bytes:\n", recv_frame.can_dlc);
           for(i=0;i<recv_frame.can_dlc;i++) {
               printf(" 0x%x", recv_frame.data[i]);
           }
       printf(" \n");
        emit revChanged();
    }
    timer->start(100);
}

//void CanTest::onRevChanged()
//{

//}

