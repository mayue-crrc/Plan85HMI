#include "traction_condition.h"
#include "ui_traction_condition.h"

Traction_Condition::Traction_Condition(QWidget *parent) :
    MyBase(parent),
    ui(new Ui::Traction_Condition)
{
    ui->setupUi(this);
    oc = new Operating_Condition(this);
    oc->hide();
    this->ui->BTN_Page->hide();
    curPage = 0;
}

Traction_Condition::~Traction_Condition()
{
    delete ui;
}

void Traction_Condition::updatePage()
{
    faultString.clear();
/*
    faultString<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"
                  << "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"
                     << "z"<< "x"<< "xq"<< "xa"<< "xq"<< "xa"<< "xq"<< "da"<< "dq"<< "da"<< "dq"<< "da"<< "qd"<< "a"<< "q"<< "a"
                        << "dq"<< "da"<< "dq"<< "da"<< "q"<< "a"<< "q"<< "sa"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a"<< "q"<< "a";
*/
faultString<< "q"<< "a"<< "q"<< "a"<< "q"<< "a";
    int remainder = faultString.size() % 16;
    totalPage = faultString.size()/16;
    if (remainder > 0)
    {
        totalPage = totalPage + 1;
    }

    if (curPage == totalPage -1)
    {
        oc->updateLineText(faultString, curPage, curPage*16 , faultString.size());
    }
    else
    {
        if (totalPage == 0)
        {
            oc->updateLineText(faultString, curPage, 0 , 0);
        }else
        {
            oc->updateLineText(faultString, curPage, curPage*16 , (curPage+1)*16);
        }
    }
    if (curPage == 0)
    {
        this->ui->BTN_Page->hide();
    }
    else if (curPage = totalPage - 1)
    {
        this->ui->BTN_Page->setText("上一页");
        this->ui->BTN_Page->show();
    }
    else
    {
        this->ui->BTN_Page->setText("上一页");
        this->ui->BTN_Page->show();
    }
    oc->show();
}

void Traction_Condition::on_BTN_Page_clicked()
{

    if (curPage == totalPage - 1)
    {
        curPage = 0;
    }
    else
    {
        curPage++;
    }
    this->updatePage();
}

