#ifndef OBJ_H
#define OBJ_H

#include <QString>
#include <QObject>
#include <QtNetwork/QTcpSocket>

class Obj : public QObject
{
    Q_OBJECT
public:
    explicit Obj(QObject *parent = 0);

public slots:
    void work();//线程处理函数

signals:
    void Done();

private slots:
    void dealConnected();
    void dealdisConnected();

private:
    QString m_carState;
    QString m_armIP;
    QTcpSocket *tcpSocket;

public:
    void gainCarMessage(QString carState,QString armIP);

};

#endif // OBJ_H
