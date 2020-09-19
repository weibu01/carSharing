#include "obj.h"
#include <QDebug>
#include <QHostAddress>
#include <QThread>
Obj::Obj(QObject *parent) :
    QObject(parent)
{

}

void Obj::gainCarMessage(QString carState,QString armIP)
{
    m_carState = carState;
    m_armIP = armIP;
}

void Obj::work()
{
    // 处理自己想处理的事
    qDebug()<< "thread:" << m_carState;
    //连接服务端
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(m_armIP),8888);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(dealConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(dealdisConnected()));
}

void Obj::dealConnected()
{
    qDebug()<< "连接arm成功...";
    tcpSocket->write(m_carState.toUtf8().data());

    //发送数据完以后关闭tcpSocket
    //tcpSocket->disconnectFromHost();
    //tcpSocket->close();
}

void Obj::dealdisConnected()
{
    qDebug()<<"连接arm控制器失败";
}






