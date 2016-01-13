#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSize>
#include <QDebug>
#include <QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setCursor(Qt::BlankCursor);
    resize(1024,600);
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("lawngreen" ));
  //  palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/cubemap_negx.jpg")));
    setPalette(palette);

    QVBoxLayout  *mainLayoutV = new QVBoxLayout(this);
    QHBoxLayout  *mainLayoutH = new QHBoxLayout();
    outPutMsg = new QTextEdit();
    outPutMsg->setFocusPolicy(Qt::NoFocus);
    outPutMsg->setCursor(Qt::BlankCursor);
    outPutMsg->setTextInteractionFlags(Qt::NoTextInteraction);
  //  outPutMsg->setEnabled(false);  //设置位只读
    outPutMsg->append(getCurrentTime());
    outPutMsg->append("    欢迎使用，请稍后...\n");
    mainLayoutH->addWidget(outPutMsg,4);

    QGroupBox  *gb = new QGroupBox();
    gb->setStyleSheet("QGroupBox{border:none}");
    QVBoxLayout  *gbLayout = new QVBoxLayout;
    autorb = new QRadioButton("自动");
    manualrb = new QRadioButton("手动");
    manualrb->setChecked(true);
    autorb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    manualrb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    gbLayout->addWidget(autorb);
    gbLayout->addSpacing(10);
    gbLayout->addWidget(manualrb);

    setpanel =new Setpanel();
    gbLayout->addWidget(setpanel,2);
    gbLayout->addSpacing(30);
    startbt = new QPushButton("启动测试");
    stopbt = new QPushButton("停止测试");
    setbt = new QPushButton("设    置");
    QString btstyle = "QPushButton{background-color:slateblue;font-size:45px;color:white;}"
                                  "QPushButton:pressed{background-color:darkgray;}";
    startbt->setStyleSheet(btstyle);
    stopbt->setStyleSheet(btstyle);
    setbt->setStyleSheet(btstyle);
    startbt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    stopbt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setbt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    gbLayout->addWidget(setbt,1);
    gbLayout->addWidget(startbt,1);
//    gbLayout->addSpacing(20);
    gbLayout->addWidget(stopbt,1);
//    gbLayout->addSpacing(20);
 //   gbLayout->addSpacing(20);
    gb->setLayout(gbLayout);
    mainLayoutH->addWidget(gb,1);
    mainLayoutV->addLayout(mainLayoutH);
    mainLayoutV->addSpacing(20);
    connect(autorb,SIGNAL(toggled(bool)),this,SLOT(onCkeckedAutorb(bool)));
    connect(manualrb,SIGNAL(toggled(bool)),this,SLOT(onCkeckedManualrb(bool)));
    connect(startbt,SIGNAL(clicked()),this,SLOT(onClickedStartbt()));
    connect(stopbt,SIGNAL(clicked()),this,SLOT(onClickedStopbt()));
    connect(setbt,SIGNAL(clicked()),this,SLOT(onClickedSetbt()));

    comStateRight = new QState();             //添加状态right
    comStateRight->setObjectName("right");
    comStateRight->assignProperty(startbt,"enabled",true);
    comStateRight->assignProperty(stopbt,"enabled",true);
    comStateRight->assignProperty(setbt,"enabled",true);
//    comStateRight->assignProperty(manualrb,"checkable",true);
 //   comStateRight->assignProperty(autorb,"checkable",true);

    comStateWrong = new QState();       //添加状态wrong
    comStateWrong->setObjectName("wrong");
    comStateWrong->assignProperty(startbt,"enabled",false);
    comStateWrong->assignProperty(stopbt,"enabled",false);
    comStateWrong->assignProperty(setbt,"enabled",false);
//    comStateWrong->assignProperty(manualrb,"checkable",false);
//   comStateWrong->assignProperty(autorb,"checkable",false);

    comStateRight->addTransition(this,SIGNAL(chgStater2w()),comStateWrong);
    comStateWrong->addTransition(this,SIGNAL(chgStatew2r()),comStateRight);

    machine = new QStateMachine(this);
    machine->addState(comStateRight);
    machine->addState(comStateWrong);
    machine->setInitialState(comStateWrong);
    machine->start();

    canProtcl = new CanProtcl(this);
    connect(canProtcl,SIGNAL(msgChanged()),this,SLOT(checkMsg()));
//    canProtcl->enableRev = true;

    outtimer = new QTimer(this);
    connect(outtimer,SIGNAL(timeout()),this,SLOT(outtimerUpdate()));
    comtimer = new QTimer(this);
    connect(comtimer,SIGNAL(timeout()),this,SLOT(comtimerUpdate()));
    comtimer->start(1000);
    startTestFg = false;
    testMode = 1;
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::getCurrentTime()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    return current_date_time.toString("yyyy-MM-dd hh:mm:ss ");
}

void Widget::onCkeckedAutorb(bool checked)
{
    if(checked){
        comtimer->stop();
        testMode_s = false;
        canProtcl->vSetAutoMan(false);
    }
}

void Widget::onCkeckedManualrb(bool checked)
{
    if(checked){
        comtimer->stop();
        testMode_s = true;
        canProtcl->vSetAutoMan(true);
    }
}

void Widget::onClickedStartbt()
{
       comtimer->stop();
       canProtcl->vStartTest();
}

void Widget::onClickedStopbt()
{
    comtimer->stop();
    canProtcl->vStopTest();
}

void Widget::onClickedSetbt()
{
   comtimer->stop();
   canProtcl->vSetMax(setpanel->upValue);
}

void Widget::checkMsg(void)
{
    unsigned char buf[20];
    unsigned char cmd;
    static bool firstRply = true;

    memcpy(buf,canProtcl->RevData.ucBuf,canProtcl->RevData.ucLen);
    cmd = (buf[2] & 0x7F);
    qDebug()<<"checkMsg";
    switch(cmd){
        case HEART_BEAT:{
          //      qDebug()<<"checkMsg HEART_BEAT";
           //     qDebug()<<comtimer->remainingTime();
                outtimer->stop(); 
                if(firstRply) {
                    outPutMsg->append(getCurrentTime());
                    outPutMsg->append("    初始化成功，请开始测试\n");
                    firstRply = false;
                }
                emit chgStatew2r();
                testMode = (bool)(buf[4]>>3 &0x03);
                autorb->setChecked(!testMode);
                manualrb->setChecked(testMode);
                if(startTestFg && (0x01 == (buf[4]>>6 &0x03))) {   //完成一次测试
                       if(!testMode) {                //自动模式下重新开始一次测试
                           comtimer->stop();
                           canProtcl->vStartTest();
                       }
                       if (TEST_SUCCESS == ((buf[4]>>4) &0x03)){
                           outPutMsg->append(getCurrentTime());
                           outPutMsg->append("    本次测试成功\n");
                       }
                       else if(TEST_FAIL == ((buf[4]>>4) &0x03)){
                           outPutMsg->append(getCurrentTime());
                           outPutMsg->append("    本次测试失败\n");
                       }
                    startTestFg = false;
                }
                 break;
        }
        case START_TEST:{
          //       qDebug()<<"checkMsg START_TEST";
                 setbt->setEnabled(false);
                 outPutMsg->append(getCurrentTime());
                 if(0== testMode)
                         outPutMsg->append("    开始自动测试\n");
                 else outPutMsg->append("    开始手动测试\n");
                 comtimer->start(1000);
                 startTestFg = true;
                 setbt->setEnabled(false);
                 break;
        }
        case STOP_TEST:{
          //       qDebug()<<"checkMsg STOP_TEST";
                 setbt->setEnabled(true);
                 outPutMsg->append(getCurrentTime());
                 outPutMsg->append("    停止测试\n");
                 comtimer->start(1000);
                 break;
        }
        case SET_MAXVALUE:{
                 qDebug()<<"checkMsg SET_MAXVALUE";
             //    outPutMsg->append(getCurrentTime());
             //    outPutMsg->append("    设置成功\n");
             //    comtimer->start(1000);
                    canProtcl->vSetMin(setpanel->dwValue);
                 break;
        }
        case SET_MINVALUE:{
         //    qDebug()<<"checkMsg SET_MAXVALUE";
                   outPutMsg->append(getCurrentTime());
                   outPutMsg->append("    设置成功\n");
                   comtimer->start(1000);
             break;
         }
        case SET_AUTO_MAN:{
            //     qDebug()<<"checkMsg SET_AUTO_MAN";
                 comtimer->start(1000);
                 testMode = testMode_s; //收到从机确认后才算设置成功
                 break;
        }
        default : break;
    }
}

void Widget::comtimerUpdate()
{
    canProtcl->vHeartBeat();
    outtimer->start(500);
}

void Widget::outtimerUpdate()
{
    emit chgStater2w();
    outPutMsg->append(getCurrentTime());
    outPutMsg->append("    通信超时，请确认原因\n");
}
