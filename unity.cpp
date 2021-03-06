#include "unity.h"
#include "ui_unity.h"

Unity::Unity(QWidget *parent) :
    MyBase(parent),
    ui(new Ui::Unity)
{
    ui->setupUi(this);

    ctrControlVoltage_Unity = new CtrControlVoltage_Unity(this);
    ctrControlVoltage_Unity->setGeometry(200,10,this->ctrControlVoltage_Unity->width(),this->ctrControlVoltage_Unity->height());

    ctrlNetCurrent_Unity = new CtrlNetCurrent_Unity(this);
    ctrlNetCurrent_Unity->setGeometry(105,10,this->ctrlNetCurrent_Unity->width(),this->ctrlNetCurrent_Unity->height());

    ctrlNetVoltage_Unity = new CtrlNetVoltage_Unity(this);
    ctrlNetVoltage_Unity->setGeometry(10,10,this->ctrlNetVoltage_Unity->width(),this->ctrlNetVoltage_Unity->height());

    ctrlTracBrake_Unity = new CtrlTracBrake_Unity(this);
    ctrlTracBrake_Unity->setGeometry(365,10,this->ctrlTracBrake_Unity->width(),this->ctrlTracBrake_Unity->height());

    ctrlSpeedometer1 = new CtrlSpeedometer(this);
    ctrlSpeedometer1 ->setGeometry(470,20,ctrlSpeedometer1->width(),ctrlSpeedometer1->height());

    ctrlSpeedometer2 = new CtrlSpeedometer(this);
    ctrlSpeedometer2 ->setGeometry(470,250,ctrlSpeedometer2->width(),ctrlSpeedometer2->height());

    ctrlFlowDiagram = new CtrlFlowDiagram(this);
    ctrlFlowDiagram ->setGeometry(690,160,ctrlFlowDiagram->width(),ctrlFlowDiagram->height());

}

Unity::~Unity()
{
    delete ui;
}
void Unity::updatePage()
{
    static int i   = 0;
    i +=10;
    ctrControlVoltage_Unity->setCtrlValueRect(i,i);
    ctrlNetCurrent_Unity->setCtrlValueRect(i);
    ctrlNetVoltage_Unity->setCtrlValueRect(i/10);
    ctrlTracBrake_Unity->setCtrlValueRect(i/10,i/10%2,i/10%3);

    ctrlSpeedometer1->setSpeenometerValue(i);
    ctrlSpeedometer2->setSpeenometerValue(i);
    ctrlFlowDiagram->setCtrlValueRect(i);
}

void Unity::showEvent(QShowEvent *)
{

}
