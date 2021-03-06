#include "widget.h"
#include "ui_widget.h"
#include "mybase.h"
#include "header.h"
#include "qtimer.h"
#include "database.h"
#include "navigator.h"
#include "vehiclerunstatepage.h"
#include "crrcmvb.h"
#include "crrcfault.h"
#include "simulation.h"
#include "widgets/ctrlnumbertable.h"
#ifdef QT_VERSION_5_6
#include "qdesktopwidget.h"
#endif

#include "maindata_trainoutline.h"
#include "maindata_driveroutline.h"
#include "settng_bypass.h"
#include "settng_panto.h"
#include "settng_distance.h"
#include "settng_traincode.h"
#include "settng_lubrication.h"
#include "settng_separation.h"
#include "settng_hangmode.h"
#include "settng_others.h"
#include "main_simulate.h"
#include "main_assistantdevice.h"
#include "main_380.h"
#include "main_light.h"
#include "main_lubrication.h"
#include "main_panto.h"
#include "main_separation.h"
#include "main_wheel.h"
#include "main_datetime.h"
#include "fault_current.h"
#include "fault_history.h"
#include "fault_download.h"
#include "condition_pantoup.h"
#include "condition_breaker.h"
#include "condition_brake.h"
#include "condition_traction.h"
#include "maindata_tracbrakeoutline.h"
#include "unity.h"
#include "unity_brake.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QDesktopWidget *desktop = QApplication::desktop();

    if (desktop->width() == 800 && desktop->height() == 600)
    {
        this->showFullScreen();
    }
    else
    {
        this->move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
    }

    this->database = new Database();
    MyBase::database = this->database;

    this->timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePage()));

    if(CrrcFault::initCrrcFault("fault_type_Plan85.db","fault_DB_Plan85.db"))
    {
        crrcFault = CrrcFault::getCrrcFault();
    }else
    {
        logger()->error("故障文件初始化错误");
    }




    this->navigator = new Navigator(this);
    this->navigator->setMyBase(uBottom,QString("导航栏"));
    this->navigator->show();
    connect(navigator,SIGNAL(translateLanguage()),this,SLOT(translateLanguage()));

    this->vehicleRunStatePage = new VehicleRunStatePage(this);
    this->vehicleRunStatePage->setMyBase(uMiddleTraction,QString("牵引模式"));
    this->vehicleRunStatePage->show();

    this->header = new Header(this);
    this->header->setMyBase(uTop,QString("标题栏"));
    this->header->show();

    //add maindata pages

    this->mainData_TrainOutline = new MainData_TrainOutline(this);
    this->mainData_TrainOutline->setMyBase(uMiddleMainData,QString("机车概况"));
    this->mainData_TrainOutline->hide();

    this->mainData_DriverOutline = new MainData_DriverOutline(this);
    this->mainData_DriverOutline->setMyBase(uMiddleMainData,QString("驱动概况"));
    this->mainData_DriverOutline->hide();

    this->mainData_TracBrakeOutline = new MainData_TracBrakeOutline(this);
    this->mainData_TracBrakeOutline->setMyBase(uMiddleMainData,QString("牵引制动"));
    this->mainData_TracBrakeOutline->hide();

    //add setting pages
    this->settng_Bypass = new Settng_Bypass(this);
    this->settng_Bypass->setMyBase(uMiddleSetting,QString("隔离"));
    this->settng_Bypass->hide();

    this->settng_Panto = new Settng_Panto(this);
    this->settng_Panto->setMyBase(uMiddleSetting,QString("受电弓"));
    this->settng_Panto->hide();

    this->settng_Distance = new Settng_Distance(this);
    this->settng_Distance->setMyBase(uMiddleSetting,QString("距离计数"));
    this->settng_Distance->hide();

    this->settng_TrainCode = new Settng_TrainCode(this);
    this->settng_TrainCode->setMyBase(uMiddleSetting,QString("车号设置"));
    this->settng_TrainCode->hide();

    this->settng_Lubrication = new Settng_Lubrication(this);
    this->settng_Lubrication->setMyBase(uMiddleSetting,QString("轮缘润滑"));
    this->settng_Lubrication->hide();

    this->settng_Separation = new Settng_Separation(this);
    this->settng_Separation->setMyBase(uMiddleSetting,QString("分相线路"));
    this->settng_Separation->hide();

    this->settng_HangMode = new Settng_HangMode(this);
    this->settng_HangMode->setMyBase(uMiddleSetting,QString("挂车模式"));
    this->settng_HangMode->hide();

    this->settng_Others = new Settng_Others(this);
    this->settng_Others->setMyBase(uMiddleSetting,QString("其他设置"));
    this->settng_Others->hide();

    //maintaince
    this->main_Simulate=new Main_Simulate(this);
    this->main_Simulate->setMyBase(uMiddleMain,QString("仿真测试"));
    this->main_Simulate->hide();


    this->main_AssistantDevice=new Main_AssistantDevice(this);
    this->main_AssistantDevice->setMyBase(uMiddleMain,QString("辅机测试"));
    this->main_AssistantDevice->hide();

    this->main_380=new Main_380(this);
    this->main_380->setMyBase(uMiddleMain,QString("库内380测试"));
    this->main_380->hide();

    this->main_Light=new Main_Light(this);
    this->main_Light->setMyBase(uMiddleMain,QString("指示灯测试"));
    this->main_Light->hide();

    this->main_Lubrication=new Main_Lubrication(this);
    this->main_Lubrication->setMyBase(uMiddleMain,QString("轮缘润滑"));
    this->main_Lubrication->hide();

    this->main_Panto=new Main_Panto(this);
    this->main_Panto->setMyBase(uMiddleMain,QString("双弓测试"));
    this->main_Panto->hide();

    this->main_Separation=new Main_Separation(this);
    this->main_Separation->setMyBase(uMiddleMain,QString("过分相测试"));
    this->main_Separation->hide();

    this->main_Wheel=new Main_Wheel(this);
    this->main_Wheel->setMyBase(uMiddleMain,QString("轮径设置"));
    this->main_Wheel->hide();

    this->main_Datetime=new Main_Datetime(this);
    this->main_Datetime->setMyBase(uMiddleMain,QString("日期时间"));
    this->main_Datetime->hide();

    this->fault_Current=new Fault_Current(this);
    this->fault_Current->setMyBase(uMiddleFault,QString("实时故障"));
    this->fault_Current->hide();

    this->fault_History=new Fault_History(this);
    this->fault_History->setMyBase(uMiddleFault,QString("历史故障"));
    this->fault_History->hide();

    this->fault_Download=new Fault_Download(this);
    this->fault_Download->setMyBase(uMiddleFault,QString("故障下载"));
    this->fault_Download->hide();

    this->simulation = new Simulation();
    this->simulation->hide();

    //运行条件
    this->condition_PantoUp = new Condition_PantoUp(this);
    this->condition_PantoUp->setMyBase(uMiddleCondition,QString("升弓条件"));
    this->condition_PantoUp->hide();

    this->condition_Breaker = new Condition_Breaker(this);
    this->condition_Breaker->setMyBase(uMiddleCondition,QString("主断条件"));
    this->condition_Breaker->hide();

    this->condition_Brake = new Condition_Brake(this);
    this->condition_Brake->setMyBase(uMiddleCondition,QString("制动条件"));
    this->condition_Brake->hide();

    this->condition_Traction = new Condition_Traction(this);
    this->condition_Traction->setMyBase(uMiddleCondition,QString("牵引条件"));
    this->condition_Traction->hide();


    this->unity = new Unity(this);
    this->unity->setMyBase(uMiddleUnite,QString("合屏模式"));
    this->unity->hide();

    this->unity_Brake = new Unity_Brake(this);
    this->unity_Brake->setMyBase(uMiddleUnite,QString("电空制动"));
    this->unity_Brake->hide();

    this->widgets.insert(uVehicleRunStatePage,this->vehicleRunStatePage);
    this->widgets.insert(uMainData_TrainOutline,this->mainData_TrainOutline);
    this->widgets.insert(uMainData_DriverOutline,this->mainData_DriverOutline);
    this->widgets.insert(uMainData_TracBrakeOutline,this->mainData_TracBrakeOutline);
    this->widgets.insert(uSettng_Bypass,this->settng_Bypass);
    this->widgets.insert(uSettng_Panto,this->settng_Panto);
    this->widgets.insert(uSettng_Distance,this->settng_Distance);
    this->widgets.insert(uSettng_TrainCode,this->settng_TrainCode);
    this->widgets.insert(uSettng_Lubrication,this->settng_Lubrication);
    this->widgets.insert(uSettng_Separation,this->settng_Separation);
    this->widgets.insert(uSettng_HangMode,this->settng_HangMode);
    this->widgets.insert(uSettng_Others,this->settng_Others);
    this->widgets.insert(uMain_Simulate,this->main_Simulate);
    this->widgets.insert(uMain_AssistantDevice,this->main_AssistantDevice);
    this->widgets.insert(uMain_380,this->main_380);
    this->widgets.insert(uMain_Light,this->main_Light);
    this->widgets.insert(uMain_Lubrication,this->main_Lubrication);
    this->widgets.insert(uMain_Panto,this->main_Panto);
    this->widgets.insert(uMain_Separation,this->main_Separation);
    this->widgets.insert(uMain_Wheel,this->main_Wheel);
    this->widgets.insert(uMain_Datetime,this->main_Datetime);
    this->widgets.insert(uFault_Current,this->fault_Current);
    this->widgets.insert(uFault_History,this->fault_History);
    this->widgets.insert(uFault_Download,this->fault_Download);
    this->widgets.insert(uUnityMode,this->unity);
    this->widgets.insert(uCondition_PantoUp,this->condition_PantoUp);
    this->widgets.insert(uCondition_Breaker,this->condition_Breaker);
    this->widgets.insert(uCondition_Traction,this->condition_Traction);
    this->widgets.insert(uCondition_Brake,this->condition_Brake);
    this->widgets.insert(uBrakeMode,this->unity_Brake);

    this->header->setPageName(this->widgets[uVehicleRunStatePage]->name);
    crrcMvb = CrrcMvb::getCrrcMvb();
}

Widget::~Widget()
{
    delete ui;
}
void Widget::updatePage()
{
    static int counter = 1;


    // update comm data,some base logic
    //if(counter%2 == 0)
    {
        crrcMvb->synchronizeMvbData();
        this->simulation->installMvb(CrrcMvb::getCrrcMvb());
        this->database->updateData();
    }
    this->header->updatePage();
    this->widgets[MyBase::currentPage]->updatePage();

    // start fault scanning thread
    static int faultdelaycnt = 0;
    if ((faultdelaycnt++ > 45) && !crrcFault->isRunning())
    {
        crrcFault->start();
    }
    counter >= 100 ? (counter = 1) : (counter ++);



}
void Widget::changePage(int page)
{
    foreach (int key, this->widgets.keys())
    {
        if (key == page)
        {
            MyBase::currentPage = page;
            this->widgets[page]->show();

            this->header->setPageName(this->widgets[page]->name);

            _LOG << "change page to" << this->widgets[page]->name;
            CtrlNumberTable::numbers="";
        }
        else
        {
            this->widgets[key]->hide();
        }
    }
    navigator->updateButtonsNamesByPositon(this->widgets[MyBase::currentPage]->Position);

}
void Widget::showEvent(QShowEvent *)
{
    if(MainGetDefaultPara::configureValid())
    {
#ifndef WINDOWS_MODE
//        if(database->HMIPosition == 1)//config  sourcePORT and IP
//        {
//            system("ifconfig eth0 192.168.2.3");

//            if(!crrcMvb->initializeMvb())
//            {
//                logger()->error("MVB板卡初始化失败");
//            }
//            this->database->HMIPosition = 1;
//            QList<unsigned short int> tmp_list;
//            tmp_list<<0x310<<0x311<<0x312<<0x313<<0x320<<0x321<<0x322<<0x323;
//            this->database->getHMISourceSinkPost(tmp_list);
//            //HMI-CCU

//        }else if(database->HMIPosition == 2)
//        {
//            system("ifconfig eth0 192.168.2.4");
//            if(!crrcMvb->initializeMvb())
//            {
//                logger()->error("MVB板卡初始化失败");
//            }
//            //HMI-CCU
//            this->database->HMIPosition = 2;
//            QList<unsigned short int> tmp_list;
//            tmp_list<<0x320<<0x321<<0x322<<0x323<<0x310<<0x311<<0x312<<0x313;
//            this->database->getHMISourceSinkPost(tmp_list);
//        }

        if(!crrcMvb->initializeMvb())
        {
            logger()->error("MVB板卡初始化失败");
        }
        //add ports
        {
            CrrcMvb::getCrrcMvb()->addSourcePort(0xfff,MVB_FCode4,32);
            for(int i = 0;i < 32;i++)
            {
                CrrcMvb::getCrrcMvb()->setUnsignedChar(0xfff,i,0);
            }

            //source
            CrrcMvb::getCrrcMvb()->addSourcePort(0x710,MVB_FCode4,32);
            CrrcMvb::getCrrcMvb()->addSourcePort(0x711,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSourcePort(0x712,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSourcePort(0x713,MVB_FCode4,128);

            //sink
            //acu
            CrrcMvb::getCrrcMvb()->addSinkPort(0x103,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x104,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x105,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x113,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x114,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x115,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x100,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x110,MVB_FCode4,128);

            //bcu
            CrrcMvb::getCrrcMvb()->addSinkPort(0x136,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x1f,MVB_FCode4,128);

            //riom
            CrrcMvb::getCrrcMvb()->addSinkPort(0x210,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x211,MVB_FCode4,32);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x212,MVB_FCode4,512);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x220,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x221,MVB_FCode4,32);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x222,MVB_FCode4,512);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x218,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x228,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x229,MVB_FCode4,512);

            //ddu
            CrrcMvb::getCrrcMvb()->addSinkPort(0x300,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x301,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x302,MVB_FCode4,128);

            //cmd
            CrrcMvb::getCrrcMvb()->addSinkPort(0x310,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x320,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x321,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x322,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x323,MVB_FCode4,128);

            //tcu
            CrrcMvb::getCrrcMvb()->addSinkPort(0x400,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x401,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x402,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x410,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x411,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x412,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x420,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x421,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x422,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x430,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x431,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x432,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x500,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x501,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x502,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x503,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x530,MVB_FCode4,32);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x531,MVB_FCode4,64);

            //erm
            CrrcMvb::getCrrcMvb()->addSinkPort(0x610,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x611,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x800,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x801,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x802,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x803,MVB_FCode4,128);

            //tcn
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe10,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe11,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe12,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe13,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe14,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe20,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe21,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe22,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe23,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe24,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe30,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe31,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe32,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe33,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe34,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe40,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe41,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe42,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe43,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xe44,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xea0,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf10,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf11,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf12,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf13,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf14,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf20,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf21,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf22,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf23,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf24,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf30,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf31,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf32,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf33,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf34,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf40,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf41,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf42,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf43,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xf44,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfa0,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x700,MVB_FCode4,64);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x701,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x702,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x703,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0x704,MVB_FCode4,128);
            //virtual ports 702  6分时  0x1702~0x6702
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x1702,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x2702,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x3702,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x4702,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x5702,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x6702,MVB_FCode4);
            //virtual ports 704  4分时  0x1704~0x4704
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x1704,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x2704,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x3704,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0x4704,MVB_FCode4);

            //virtual ports TCN1/2主从
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe10,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe11,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe12,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe13,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe14,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe20,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe21,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe22,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe23,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe24,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe30,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe31,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe32,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe33,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe34,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe40,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe41,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe42,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe43,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xfe44,MVB_FCode4);
            // 0xE/F*2 端口6分时 虚拟端口命名为0xf1*2 0xf2*2 0xf3*2 0xf4*2 0xf5*2 0xf6*2
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf112,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf212,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf312,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf412,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf512,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf612,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf122,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf222,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf322,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf422,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf522,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf622,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf132,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf232,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf332,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf432,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf532,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf632,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf142,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf242,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf342,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf442,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf542,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf642,MVB_FCode4);
            // 0xE/F*4 端口4分时 虚拟端口命名为0xf1*4 0xf2*4 0xf3*4 0xf4*4
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf114,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf214,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf314,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf414,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf124,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf224,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf324,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf424,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf134,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf234,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf334,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf434,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf144,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf244,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf344,MVB_FCode4);
            CrrcMvb::getCrrcMvb()->addVirtualPort(0xf444,MVB_FCode4);

            //


            //ato
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfb0,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfc0,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfc1,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfc2,MVB_FCode4,128);
            CrrcMvb::getCrrcMvb()->addSinkPort(0xfc3,MVB_FCode4,128);




        }
#endif

        if(crrcMvb->InitReadPortData())
        {
            logger()->error("MVB板卡设置操作模式失败");
        }

        timer->start(333);
    }else
    {
        logger()->error("configure.ini文件错误");
        _LOG << "fail to read configure file.";
    }

}


void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }else if (event->key() == Qt::Key_S)
    {
        QDesktopWidget *desktop = QApplication::desktop();

        // show a window uesd to manipulate the mvb ports and change page

        simulation->move((desktop->width() - simulation->width()) / 2, (desktop->height() - simulation->height()) / 2);
        simulation->show();
    }
}
void Widget::translateLanguage()
{
    QTranslator *translator = new QTranslator(0);
    static bool t_istranslate = false;
    if(t_istranslate)
    {
        qApp->removeTranslator(translator);
    }else
    {
        translator->load("language_en.qm");
        qApp->installTranslator(translator);
    }
    t_istranslate = !t_istranslate;
    foreach(int i,this->widgets.keys())
    {
        this->widgets[i]->updateLanguage();
    }

    delete translator;
}
