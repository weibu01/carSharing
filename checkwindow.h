#ifndef CHECKWINDOW_H
#define CHECKWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QString>

#include <QtNetwork/QTcpSocket>


namespace Ui {
class CheckWindow;
}

class CheckWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CheckWindow(QWidget *parent = 0);
    void gainSocket(QTcpSocket *socket);
    void gainAccount(QString str);
    void showImage();
    ~CheckWindow();

private:
    Ui::CheckWindow *ui;

    //MainWindow *m_pMainWindow;

private slots:
    void execAddImages(QString);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


public:
    QTcpSocket *tcpSocket;
    QString logIn_account;

};

#endif // CHECKWINDOW_H
