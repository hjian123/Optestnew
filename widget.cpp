#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSize>
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(1024,600);
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(32,32,32));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/cubemap_negx.jpg")));
    setPalette(palette);

    QVBoxLayout  *mainLayoutV = new QVBoxLayout(this);
    QHBoxLayout  *mainLayoutH = new QHBoxLayout(this);
    outPutMsg = new QTextEdit();
    mainLayoutH->addWidget(outPutMsg,4);

    QGroupBox  *gb = new QGroupBox();
    QVBoxLayout  *gbLayout = new QVBoxLayout;
    autorb = new QRadioButton("自动");
    manualrb = new QRadioButton("手动");
    gbLayout->addWidget(autorb);
    gbLayout->addSpacing(10);
    gbLayout->addWidget(manualrb);
    gbLayout->addStretch();
    startbt = new QPushButton("启动测试");
    stopbt = new QPushButton("停止测试");
    setbt = new QPushButton("设    置");
    gbLayout->addWidget(startbt);
    gbLayout->addSpacing(20);
    gbLayout->addWidget(stopbt);
    gbLayout->addSpacing(20);
    gbLayout->addWidget(setbt);
    gbLayout->addSpacing(20);
    gb->setLayout(gbLayout);
    mainLayoutH->addWidget(gb,1);
    mainLayoutV->addLayout(mainLayoutH);
    mainLayoutV->addSpacing(20);
    connect(autorb,SIGNAL(toggled(bool)),this,SLOT(onCkeckedAutorb(bool)));
    connect(manualrb,SIGNAL(toggled(bool)),this,SLOT(onCkeckedManualrb(bool)));
    connect(startbt,SIGNAL(clicked()),this,SLOT(onClickedStartbt()));
    connect(stopbt,SIGNAL(clicked()),this,SLOT(onClickedStopbt()));
    connect(setbt,SIGNAL(clicked()),this,SLOT(onClickedSetbt()));

    canProtcl = new CanProtcl(this);
    connect(canProtcl,SIGNAL(msgChanged()),this,SLOT(checkMsg()));

    comtimer = new QTimer(this);
    connect(comtimer,SIGNAL(timeout()),this,SLOT(comtimerUpdate()));
    comtimer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onCkeckedAutorb(bool checked)
{
    comtimer->stop();
    if(checked){
        canProtcl->vSetAutoMan(false);
    }
}

void Widget::onCkeckedManualrb(bool checked)
{
    comtimer->stop();
    if(checked){
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
    canProtcl->vSet();
}

void Widget::checkMsg(void)
{
    unsigned char buf[20];
    unsigned char cmd;
    memcpy(buf,canProtcl->RevData.ucBuf,canProtcl->RevData.ucLen);
    cmd = (buf[2] & 0x7F);
    qDebug()<<"checkMsg";
    switch(cmd){
        case HEART_BEAT:{
                qDebug()<<"checkMsg HEART_BEAT";
                comState = true;
                 break;
        }
        case START_TEST:{
                 qDebug()<<"checkMsg START_TEST";
                 comtimer->start(1000);
                 break;
        }
        case STOP_TEST:{
                 qDebug()<<"checkMsg STOP_TEST";
                 comtimer->start(1000);
                 break;
        }
        case SET_DETAILS:{
                 qDebug()<<"checkMsg SET_DETAILS";
                 comtimer->start(1000);
                 break;
        }
        case SET_AUTO_MAN:{
                 qDebug()<<"checkMsg SET_AUTO_MAN";
                 comtimer->start(1000);
                 break;
        }
        default : break;
    }
}

void Widget::comtimerUpdate()
{
     comState = false;
     canProtcl->vHeartBeat();
}


