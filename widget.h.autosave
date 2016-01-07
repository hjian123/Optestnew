#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "canprotcl.h"
#include <QTimer>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QState>
#include <QStateMachine>
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
    QPushButton  *startbt;
    QPushButton  *stopbt;
    QPushButton  *setbt;
    QTextEdit  *outPutMsg;
    QRadioButton *autorb;
    QRadioButton *manualrb;  
signals:
    void chgStater2w();
    void chgStatew2r();
protected slots:
    void onCkeckedAutorb(bool check);
    void onCkeckedManualrb(bool check);
    void onClickedStartbt(void);
    void onClickedStopbt(void);
    void onClickedSetbt(void);
    void checkMsg(void);
    void comtimerUpdate(void);
};

#endif // WIDGET_H
