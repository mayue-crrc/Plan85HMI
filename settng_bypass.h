#ifndef SETTNG_BYPASS_H
#define SETTNG_BYPASS_H

#include <mybase.h>
#include "QLabel"
#include "QPushButton"

namespace Ui {
class Settng_Bypass;
}

class Settng_Bypass : public MyBase
{
    Q_OBJECT

public:
    explicit Settng_Bypass(QWidget *parent = 0);
    ~Settng_Bypass();
    void updatePage();
    void timerEvent(QTimerEvent *e);
private slots:
    void SelectTrainEvent();
    void TCUListEvent();
    void on_BTN_Close_pressed();
    void on_BTN_HandISO_pressed();
    void on_BTN_ISOCancel_pressed();
    void on_BTN_AutoSandCutout_pressed();

    void on_BTN_CombCutout_pressed();

    void on_BTN_ClearFault_pressed();

private:
    Ui::Settng_Bypass *ui;
    QList<bool> TrainBrkISOstate[4],TrainISOstate[4],TrainHISOstate[4];

    QList<bool*> Train1CutSignal,Train2CutSignal,Train3CutSignal,Train4CutSignal,
                 Train1CancelSignal,Train2CancelSignal,Train3CancelSignal,Train4CancelSignal,FaultReset;
    QList<QLabel*> BrkList,TCUlabList;
    QList<QPushButton*> TrainSelect,TCUList;
    QList<QString> handISO,notISO;
    int TrainIndex,SelectIndex,timerISO,timerREL,timerResetFault;
    void setLabelState(QList<QLabel*> lblList,QList<bool> stateList);
    void setLabelState(QList<QLabel*> lblList,QList<bool> handISOList,QList<bool> ISOList);
    void setButtonState(QPushButton* btn,bool iso,QString str1,QString str2);
      void hideAll();
};

#endif // SETTNG_BYPASS_H
