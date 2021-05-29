#include "interface.h"
#include <QPalette>
#include <QPixmap>
#include <QFont>
#include <QDebug>
int msgNum=0;
QString msg[] = {"loading.","loading..","loading...","loading...."};
interface::interface(QWidget *parent) :
    QWidget(parent)
{
    msgLabel = new QLabel(this);//提示信息
    msgLabel->setGeometry(380,440,100,75);
    msgLabel->setStyleSheet("color:white");
    msgLabel->setFont(QFont("Microsoft YaHei",13));
    msgLabel->hide();

    probar = new QProgressBar(this);//进度条
    probar->setStyle(QStyleFactory::create("fusion"));
    probar->setGeometry(180,500,480,15);
    probar->setTextVisible(true);
    probar->hide();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(loading()));

    QFont font("Microsoft YaHei" ,16, 55);
    QString button_style="QPushButton{background-color:transparent;color:white;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";

    button_Dungeon=new QPushButton(this);
    button_Dungeon->setGeometry(280,325,320,50);
    button_Dungeon->setText("dungeon mode");
    button_Dungeon->setStyleSheet(button_style);
    button_Dungeon->setFont(font);
    connect(button_Dungeon,SIGNAL(clicked()),this,SLOT(DungeonStart()));

    button_Quit=new QPushButton(this);
    button_Quit->setGeometry(280,365,300,50);
    button_Quit->setText("exit");
    button_Quit->setStyleSheet(button_style);
    button_Quit->setFont(font);
    connect(button_Quit,SIGNAL(clicked()),this,SLOT(Quit()));

    button_About=new QPushButton(this);
    button_About->setGeometry(280,405,300,50);
    button_About->setText("about");
    button_About->setStyleSheet(button_style);
    button_About->setFont(font);
    connect(button_About,SIGNAL(clicked()),this,SLOT(AboutShow()));

    isok=false;
    //classicalisok=false;
    Dungeonisok=false;

    BGM=new QMediaPlayer(this);
    BGM->setMedia(QUrl("qrc:/music/music/maze_main_theme.mp3"));//相对路径
    BGM->setVolume(50);
    BGM->play();
    BGMTimer=new QTimer(this);
    connect(BGMTimer,SIGNAL(timeout()),this,SLOT(CheckBGMstate()));
    BGMTimer->start(10);
}

void interface::paintEvent(QPaintEvent*)
{

    painter = new QPainter(this);
    if(!isok)
    {
        QImage pixmap(":/interface/image/interface/StartPage.png");//开始页
        painter->drawImage(geometry(),pixmap);
    }
    else
    {
        QImage pixmap(":/interface/image/interface/SetNamePage.PNG");//设置角色名页
        painter->drawImage(geometry(),pixmap);
    }
    painter->end();
}
void interface::showMainMenu()
{
    button_Dungeon->show();
    button_Quit->show();
    button_About->show();
}

void interface::DungeonStart()
{
    button_Dungeon->hide();
    button_Quit->hide();
    button_About->hide();
    msgLabel->show();//提示消息显示
    timer->start(100);//启动计数器
    probar->show();//进度前显示
    Dungeonisok=true;
}

void interface::loading()
{
    msgLabel->setText(msg[(msgNum++)%4]);//提示信息的变化
    probar->setValue(probar->value()+20);
    if(probar->value() >= 99)
    {
        timer->stop();
        msgLabel->hide();
        probar->hide();
        isok=true;
        probar->setValue(0);
        this->update();//更新窗口，触发绘图事件
    }

}
void interface::Quit()
{
    QMessageBox message(QMessageBox::Information,"exit","ready to exit?",QMessageBox::Yes|QMessageBox::No,this);
    message.setIconPixmap(QPixmap(":/info/image/information/退出.png"));
    message.setWindowIcon(QIcon(":/info/image/information/关于.ico"));
    message.setButtonText(QMessageBox::Yes, QString("Yes"));
    message.setButtonText(QMessageBox::No, QString("Return"));
    if(message.exec()==QMessageBox::Yes)
    {
        qApp->quit();//退出当前应用程序，不需要 QApplication的头文件
    }
    else
    {
        return;
    }
}
void interface::CheckBGMstate()
{
   if(BGM->state()==0)//0代表停止 1代表播放 2代表暂停
   {
       BGM->play();
   }
}
void interface::AboutShow()
{
    QMessageBox message(QMessageBox::Information,"about",
                        "Written By Realself,in SWJTU,2019.08",QMessageBox::Yes,this);
    message.setIconPixmap(QPixmap(":/info/image/information/关于.ico"));
    message.button(QMessageBox::Yes)->hide();//去除按钮
    message.exec();
}
