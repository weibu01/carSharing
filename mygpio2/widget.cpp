#include "widget.h"
#include "ui_widget.h"
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QString>
#include <QTime>
#include <QDebug>

//add by myself
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>

static int fb;
static int fb2;
volatile int LED=0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //打开设备节点
    fb=open("/dev/mygpio",0);
    if(fb<0)
    {
        perror("open device mygpio failed!");
        exit(1);
    }
    fb2=open("/dev/leds",0);
    if(fb<0)
    {
        perror("open device led failed!");
        exit(1);
    }


    m_ptcpServer = new QTcpServer(this);
    m_ptcpServer->listen(QHostAddress::Any,8888);
    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(newConnect()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::delay(int msec)//ms
{
    QTime n=QTime::currentTime();
            QTime now;
            do{
                 now=QTime::currentTime();
               }while (n.msecsTo(now)<=msec);
}

void Widget::newConnect()
{
    //取出建立好连接的套接字 同时在这里初始化指针 tcpSocket
    m_ptcpSocket = m_ptcpServer->nextPendingConnection();
    connect(m_ptcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage())); //有可读的信息，触发读函数槽
}

void Widget::readMessage()
{
        QStringList allString;//保存用户数据registerInfoList
        qint64 len = m_ptcpSocket->bytesAvailable();   

        if(len>0)//len>0说明有数据发进来
        {
            QByteArray alldata = m_ptcpSocket->readAll();
            //开始转换编码
            //QTextCodec *utf8codec = QTextCodec::codecForName("UTF-8");
            //QString utf8str = utf8codec->toUnicode(alldata.mid(2));
            qDebug()<<alldata;
            if(alldata == "useCarFF00005")
            {
                ioctl(fb,1,0);   //gpio0   high level
                ioctl(fb,1,1);//gpio1 high
                ioctl(fb2,1,1);//led  high level
                delay(2000);
                ioctl(fb,0,0);   //gpio0 low level
                ioctl(fb,0,1); //gpio 1 low
                ioctl(fb2,0,1); //led low level
            }
            if(alldata == "returnFF00005")
            {
                ioctl(fb,1,0);   //gpio0   high level
                ioctl(fb,1,1);
                delay(2000);
                ioctl(fb,0,0);   //gpio0   low  level
                ioctl(fb,0,1);
            }
         }
}

//这部分是转到槽
void Widget::on_pushButton_clicked()
{
    qDebug() << ioctl(fb,1,0);
    qDebug() << ioctl(fb2,1,1);
    delay(5000);
    ioctl(fb,0,0);
    ioctl(fb2,0,1);
    //m_pTimer->start(2000);
    //connect(m_pTimer, SIGNAL(timeout()), this, SLOT(dealTimeout()) );
}

void Widget::on_pushButton_2_clicked()
{
    qDebug() << ioctl(fb,0,0);
    qDebug() << ioctl(fb2,0,1);
    //m_pTimer->start(2000);  这里不需要定时

}


//控制gpio1 输入输出
void Widget::on_pushButton_3_clicked()
{
     ioctl(fb,1,1);
     delay(5000);
     ioctl(fb,0,1);
}

void Widget::on_pushButton_4_clicked()
{
    ioctl(fb,0,1);
}
