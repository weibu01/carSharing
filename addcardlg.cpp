#include "addcardlg.h"
#include "ui_addcardlg.h"
//#include "ui_addcardlg.h"
#include <QDateTime>
#include <QStringList>
#include <QString>
#include <QDebug>

AddCarDlg::AddCarDlg(QWidget *parent) :QDialog(parent),ui(new Ui::AddCarDlg)
{
    ui->setupUi(this);
    //clearData();
    //显示编辑信息界面
    setFixedSize(sizeHint());// 显示添加用户界面
//  setAttribute(Qt::WA_DeleteOnClose);
}

AddCarDlg::~AddCarDlg()
{
    delete ui;
}

void AddCarDlg::setEditData(QStringList rowData)
{
    //将要编辑的信息在  编辑界面上显示出来,这些信息已经有了
    ui->carNumberEdit->setText(rowData.at(0));
    ui->numberEdit->setText(rowData.at(1));
    ui->managerPhoneEdit->setText(rowData.at(2));
    ui->carManagerEdit->setText(rowData.at(3));
    ui->isUseComboBox->setCurrentText(rowData.at(4));
    ui->userNameEdit->setText(rowData.at(5));
    ui->userPhoneEdit->setText(rowData.at(6));
}

void AddCarDlg::clearData()
{
    ui->carNumberEdit->clear();
    ui->numberEdit->clear();
    ui->managerPhoneEdit->clear();
    ui->carManagerEdit->clear();
    //ui->isUseComboBox->clear();//这一行不要删除，每次打开用户编辑界面都默认显示的
    ui->userNameEdit->clear();
    ui->userPhoneEdit->clear();
}

//用来在关闭界面时清楚界面数据、、、第一次好像无效
void AddCarDlg::closeEvent(QCloseEvent *event)
{
    clearData();
}


//添加用户界面 OK 按钮
void AddCarDlg::on_buttonBox_accepted()
{
    QVariantMap carInfo;

    //获取填写的用户信息
    QString carNumber = ui->carNumberEdit->text();
    QString number = ui->numberEdit->text();
    QString managerPhone = ui->managerPhoneEdit->text();
    QString carManager = ui->carManagerEdit->text();
    QString isUse = ui->isUseComboBox->currentText();
    QString userName = ui->userNameEdit->text();
    QString userPhone = ui->userPhoneEdit->text();

    //获取系统当前时间
    QString datetime = QDateTime::currentDateTime().toString();

    carInfo.insert("datetime" ,datetime );
    carInfo.insert("carNumber" ,carNumber );
    carInfo.insert("number" ,number );
    carInfo.insert("managerPhone" ,managerPhone );
    carInfo.insert("carManager" ,carManager );
    carInfo.insert("isUse" ,isUse );
    carInfo.insert("userName" ,userName );
    carInfo.insert("userPhone" ,userPhone );

    emit signalCarInfo(carInfo);

    clearData();//关闭界面编辑界面时清楚界面数据，防止下次打开还有数据
}

//添加用户界面Cancel按钮
void AddCarDlg::on_buttonBox_rejected()
{
    close();
}
