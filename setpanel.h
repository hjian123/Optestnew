#ifndef SETPANEL_H
#define SETPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>

class Setpanel : public QWidget
{
    Q_OBJECT
public:
    explicit Setpanel(QWidget *parent = 0);
    ~Setpanel();
   unsigned int upValue;
   unsigned int dwValue;
private:
    QLineEdit *upedit;
    QLineEdit *dwedit;
   QPushButton *upaddbt;
   QPushButton *upsubbt;
   QPushButton *dwaddbt;
   QPushButton *dwsubbt;
   QSlider *upslider;
   QSlider *dwslider;
signals:

private slots:
   void addUpvalue(void);
   void subUpvalue(void);
   void addDwvalue(void);
   void subDwvalue(void);
   void chageUpvalue(int value);
   void chageDwvalue(int value);
};

#endif // SETPANEL_H
