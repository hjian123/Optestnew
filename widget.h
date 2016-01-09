#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "canprotcl.h"
#include "setpanel.h"
#include <QTimer>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QState>
#include <QStateMachine>

#define  TEST_STATE_PRE        0x01    //等待上料状态
#define  TEST_STATE_HOM      0x02   //回原点状态
#define  TEST_STATE_GAUG     0x03   //压力测量状态
#define  TEST_STATE_CHECK   0x04   //判断结果状态
#define  TEST_STATE_WAIT      0x05    //等待命令
#define  TEST_SUCCESS           true
#define  TEST_FAIL                   false

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    CanProtcl  *canProtcl;
    QState *comStateRight;                  //表明CAN接口的通讯状态，正常或异常
    QState *comStateWrong;                  //表明CAN接口的通讯状态，正常或异常
    QStateMachine *machine;
    QTimer *comtimer;
    QTimer *outtimer;
    QPushButton  *startbt;
    QPushButton  *stopbt;
    QPushButton  *setbt;
    QTextEdit  *outPutMsg;
    QRadioButton *autorb;
    QRadioButton *manualrb;  
    Setpanel *setpanel;
    QString getCurrentTime(void);
    bool testMode;
    bool testMode_s;
    bool startTestFg;
signals:
    void chgStater2w();
    void chgStatew2r();
    void finishedOneTest();
protected slots:
    void onCkeckedAutorb(bool check);
    void onCkeckedManualrb(bool check);
    void onClickedStartbt(void);
    void onClickedStopbt(void);
    void onClickedSetbt(void);
    void checkMsg(void);
    void comtimerUpdate(void);
    void outtimerUpdate(void);
};

#endif // WIDGET_H
