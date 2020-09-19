#include "checkwindow.h"
#include "ui_checkwindow.h"
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

#include "mainwindow.h"

CheckWindow::CheckWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckWindow)
{
    ui->setupUi(this);
}

void CheckWindow::showImage()
{
    //QString path="F:\Qt code\build-Application-Desktop_Qt_5_6_3_MinGW_32bit-Debug\用户信息\刘鹏888888";
    QPixmap pix0("id1.jpg");
    QPixmap pix1("id2.jpg");
    QPixmap pix2("drivingLicence.jpg");
    ui->id1->setPixmap(pix0);
    ui->id2->setPixmap(pix1);
    ui->drivingLicence->setPixmap(pix2);
    //connect(parent , SIGNAL(signlImagePath(QString)) , this ,SLOT(execAddImages(QString)));
}

void CheckWindow::gainSocket(QTcpSocket *socket)
{
    this->tcpSocket=socket;
}

void CheckWindow::gainAccount(QString str)
{
    this->logIn_account = str;
}

CheckWindow::~CheckWindow()
{
    delete ui;
}

void CheckWindow::execAddImages(QString path)
{
    qDebug()<<"path is -------->"<< path;
    QPixmap pix0(path+"/id1.jpg");
    QPixmap pix1(path+"/id2.jpg");
    QPixmap pix2(path+"/drivingLicence.jpg");
    ui->id1->setPixmap(pix0);
    ui->id2->setPixmap(pix1);
    ui->drivingLicence->setPixmap(pix2);
}



//转到槽函数，审核通过按钮
void CheckWindow::on_pushButton_clicked()
{   
    QString str0="审核通过";
    tcpSocket->write(str0.toStdString().c_str(),strlen(str0.toStdString().c_str()));
    this->close();

    //通过审核后修改数据库，将数据库里对应用户修改为通过审核
    QString isCheck = "是";
    QString sql;
    sql = QString("UPDATE ApplicantManage "
                  "SET isCheck = '%1' "
                  " where phone ='%2'")
            .arg(isCheck).arg(logIn_account);

    QSqlQuery query;
    bool ok = query.exec(sql);//用户编辑的数据->加载到数据库
    if(ok)
    {
        //onBtnRefresh1(); //用户编辑的数据->显示在界面上
        QMessageBox::information(this ,tr("提示") , tr("编辑成功!"));
    }
    else
    {
        QMessageBox::information(this ,tr("提示") , tr("编辑失败!"));
    }
}

void CheckWindow::on_pushButton_2_clicked()
{
    QString str1="审核未通过，请您重新发送清晰的审核照片";
    tcpSocket->write(str1.toStdString().c_str(),strlen(str1.toStdString().c_str()));
    this->close();
}
