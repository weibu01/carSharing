#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "carmanage.h"
#include "applicantmanage.h"
#include "createdb.h"
#include "addcardlg.h"
#include "addapplicantdlg.h"
#include <QHBoxLayout>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "createdbone.h"
#include <QFile>
#include <QDir>
#include <QString>
#include "checkwindow.h"
#include <QStringList>


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //枚举变量
    enum OperateType
    {
        Add ,
        Edit
    };

    typedef enum
    {
        flag_registerInfo,
        flag_logInInfo,
        flag_usingCarInfo
        //flag_imageInfo
    }OperateType1;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onBtnAdd();

    void onBtnEdit();

    void onBtnDel();

    void onBtnRefresh();

    void ExecAddSql(QVariantMap stuInfo);

    void ExecEditSql(QVariantMap stuInfo);

    void ExecDelSql(QString id);

    void showTabCornerBtn();



    void onBtnAdd1();

    void onBtnEdit1();

//    void onBtnDel1();

    void onBtnRefresh1();

    void ExecAddSql1(QVariantMap stuInfo);

    void ExecEditSql1(QVariantMap stuInfo);

//    void ExecDelSql1(QString id);

    void newConnect();
    void readMessage();
    void sendMessageToClient(int);

signals:
    void signlImagePath(QString path);

private:
    void initUi();

    void setTabCornerBtn();
    void setTabCornerBtn1();

    void initAddDlg();
    void initAddDlg1();


    void startTcpserver();
    int flag_convert(QString flag);

    QList<QStringList> selectColumnDataFromBase1();
    QList<QStringList> select3ColumnDataFromBase1();

    void applicant_register(QStringList allString);
    void applicant_logIn(QStringList allString);
    void applicant_check(QByteArray alldata);
    void open_car(QStringList allString);



private:
    Ui::MainWindow *ui;
    QTabWidget *m_pTabWidget;

    CarManage *m_pCarManage;
    ApplicantManage *m_pApplicantManage;

    CreateDb *m_pCreateDb;
    CreateDbOne *m_pCreateDb1;
    AddCarDlg* m_pAddCarDlg;
    AddApplicantDlg *m_pAddApplicantDlg;

    OperateType m_operateType;

    //用来显示右上角的功能按钮
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);//构建水平布局管理器指定父对象-widget（在widget里显示）
    QWidget *widget1 = new QWidget;
    QHBoxLayout *hLayout1 = new QHBoxLayout(widget1);//构建水平布局管理器指定父对象-widget（在widget里显示）
    int isDefined1=0;
    int isDefined=0;

    QDir tempDir;
    qint64 fileSize=0;
    qint64 recvSize=0;
    qint64 imageNumber=0;
    QFile file;
    bool isStartReadFile = false;
    QString currentDir;
    CheckWindow *m_pcheckWindow;
    QString m_logInfo;

public:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;

    QStringList carNumber_IP={"FF00005|192.168.43.121","AA12345|192.168.59.129","BB12345|192.168.59.129"};
};

#endif // MAINWINDOW_H
