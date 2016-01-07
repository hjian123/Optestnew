#ifndef CANTEST_H
#define CANTEST_H

#include <QObject>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <QTimer>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

#define CAN_PORT   "can0"
class CanTest : public QObject
{
    Q_OBJECT
public:
    explicit CanTest(QObject *parent = 0);
    ~CanTest();

    struct can_frame recv_frame;
    int send(unsigned int ID,unsigned char DLC,const unsigned char *buf);
 private:
    int send_socket_fd;                  //发送套接字
    int recv_socket_fd;                   //接收套接字
    int socket_listen(const char *port);
     int recv(void);
//    int socket_can_filter(void);     //设置过滤规则
    QTimer *timer;
signals:
    void revChanged();

public slots:
    void timerUpdate();

};

#endif // CANTEST_H
