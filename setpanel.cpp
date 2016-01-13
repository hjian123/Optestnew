#include "setpanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QValidator>
#include <QIntValidator>

Setpanel::Setpanel(QWidget *parent) : QWidget(parent)
{
    upValue = 700000;
    dwValue = 600000;
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();

    upsubbt = new QPushButton();
    upsubbt->setIcon(QIcon(":/img/minus-sign.png"));
    upsubbt->setStyleSheet("QPushButton{background-color:transparent;}");
    upsubbt->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    QValidator * validator = new QIntValidator(0,8388607,this);
    upedit = new QLineEdit(QString::number(upValue,10));
    upedit->setValidator(validator);
    upedit->setStyleSheet("QLineEdit{background-color:transparent;font-size:24px;color:white}");
    upedit->setAlignment(Qt::AlignCenter);
    upedit->setCursor(Qt::BlankCursor);
    upedit->setFocusPolicy(Qt::NoFocus);
    upedit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    upedit->setEnabled(false);
    upaddbt = new QPushButton();
    upaddbt->setIcon(QIcon(":/img/plus-sign.png"));
    upaddbt->setStyleSheet("QPushButton{background-color:transparent;}");
    upaddbt->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    hlayout1->addWidget(upsubbt,1);
    hlayout1->addWidget(upedit,3);
    hlayout1->addWidget(upaddbt,1);
    vlayout->addLayout(hlayout1);

    upslider = new QSlider(Qt::Horizontal);
    upslider->setMinimum(600000);
    upslider->setMaximum(1000000);
    vlayout->addWidget(upslider);

    dwsubbt = new QPushButton();
    dwsubbt->setIcon(QIcon(":/img/minus-sign.png"));
    dwsubbt->setStyleSheet("QPushButton{background-color:transparent;}");
    dwsubbt->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    dwedit = new QLineEdit(QString::number(dwValue,10));
    dwedit->setAlignment(Qt::AlignCenter);
    dwedit->setFocusPolicy(Qt::NoFocus);
    dwedit->setValidator(validator);
    dwedit->setCursor(Qt::BlankCursor);
    dwedit->setStyleSheet("QLineEdit{background-color:transparent;font-size:24px;color:white}");
    dwedit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dwedit->setEnabled(false);  //只读
    dwaddbt = new QPushButton();
    dwaddbt->setIcon(QIcon(":/img/plus-sign.png"));
    dwaddbt->setStyleSheet("QPushButton{background-color:transparent;}");
    dwaddbt->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    hlayout2->addWidget(dwsubbt,1);
    hlayout2->addWidget(dwedit,3);
    hlayout2->addWidget(dwaddbt,1);
    vlayout->addLayout(hlayout2);

    dwslider = new QSlider(Qt::Horizontal);
    dwslider->setMinimum(600000);
    dwslider->setMaximum(1000000);
    vlayout->addWidget(dwslider);
    connect(upaddbt,SIGNAL(clicked()),this,SLOT(addUpvalue()));
    connect(upsubbt,SIGNAL(clicked()),this,SLOT(subUpvalue()));
    connect(dwaddbt,SIGNAL(clicked()),this,SLOT(addDwvalue()));
    connect(dwsubbt,SIGNAL(clicked()),this,SLOT(subDwvalue()));
    connect(upslider,SIGNAL(valueChanged(int)),this,SLOT(chageUpvalue(int)));
    connect(dwslider,SIGNAL(valueChanged(int)),this,SLOT(chageDwvalue(int)));
}

Setpanel::~Setpanel()
{

}

void Setpanel::addUpvalue()
{
    upValue ++;
    upedit->setText(QString::number(upValue,10));
}

void Setpanel::subUpvalue()
{
    upValue --;
    upedit->setText(QString::number(upValue,10));
}

void Setpanel::addDwvalue()
{
    dwValue ++;
    dwedit->setText(QString::number(dwValue,10));
}

void Setpanel::subDwvalue()
{
    dwValue --;
    dwedit->setText(QString::number(dwValue,10));
}

void Setpanel::chageUpvalue(int value)
{
    upValue = value;
    upedit->setText(QString::number(upValue,10));
}

void Setpanel::chageDwvalue(int value)
{
    dwValue = value;
    dwedit->setText(QString::number(dwValue,10));
}

