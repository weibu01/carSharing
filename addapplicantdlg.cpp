#include "addapplicantdlg.h"
//#include "ui_addapplicantdlg.h"
#include "ui_addapplicantdlg.h"
#include <QDateTime>
#include <QStringList>
#include <QString>
#include <QDebug>

AddApplicantDlg::AddApplicantDlg(QWidget *parent) :QDialog(parent),ui(new Ui::AddApplicantDlg)
{
    ui->setupUi(this);
    setFixedSize(sizeHint());// 显示添加用户界面
//  setAttribute(Qt::WA_DeleteOnClose);
}

AddApplicantDlg::~AddApplicantDlg()
{
    delete ui;
}

void AddApplicantDlg::setEditData1(QStringList rowData)
{
    //显示要添加的信息——对应表格表头信息
    ui->nameEdit->setText(rowData.at(0));
    ui->numberEdit->setText(rowData.at(1));
    ui->phoneEdit->setText(rowData.at(2));
    ui->passwordEdit->setText(rowData.at(3));
    ui->isCheckEdit->setText(rowData.at(4));
    ui->otherEdit->setText(rowData.at(5));
}

void AddApplicantDlg::clearData1()
{
    ui->nameEdit->clear();
    ui->numberEdit->clear();
    ui->phoneEdit->clear();
    ui->passwordEdit->clear();
    ui->isCheckEdit->clear();
    ui->otherEdit->clear();
}

//用来在关闭界面时清楚界面数据、、、第一次好像无效
void AddApplicantDlg::closeEvent1(QCloseEvent *event)
{
    clearData1();
}


//此函数通过转到槽调用的
//添加用户界面 OK 按钮  将用户所有信息保存到QVariantMap applicantInfo里
void AddApplicantDlg::on_buttonBox_accepted()
{
    QVariantMap applicantInfo;

    //获取填写的用户信息
    QString name = ui->nameEdit->text();
    QString number = ui->numberEdit->text();
    QString phone = ui->phoneEdit->text();
    QString password = ui->passwordEdit->text();
    QString isCheck = ui->isCheckEdit->text();
    QString other = ui->otherEdit->text();

    //获取系统当前时间
    QString datetime = QDateTime::currentDateTime().toString();

    applicantInfo.insert("datetime" ,datetime );
    applicantInfo.insert("name" ,name );
    applicantInfo.insert("number" ,number );
    applicantInfo.insert("phone" ,phone );
    applicantInfo.insert("password" ,password );
    applicantInfo.insert("isCheck" ,isCheck );
    applicantInfo.insert("other" ,other );

    emit signalApplicantInfo1(applicantInfo);//返回用户所有信息

    clearData1();//关闭界面编辑界面时清楚界面数据，防止下次打开还有数据
}


//此函数通过转到槽调用的
//添加用户界面Cancel按钮
void AddApplicantDlg::on_buttonBox_rejected()
{
    close();
}
