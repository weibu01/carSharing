#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void delay(int msec);

private slots:
    void on_pushButton_clicked();
    void newConnect();
    void readMessage();
    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *m_ptcpServer;
    QTcpSocket *m_ptcpSocket;
};

#endif // WIDGET_H
