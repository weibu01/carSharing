#include "applicantmanage.h"
#include "ui_applicantmanage.h"
#include "applicantmanage.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>

ApplicantManage::ApplicantManage(QWidget *parent) : QWidget(parent), ui(new Ui::ApplicantManage)
{
    ui->setupUi(this);//tableWidget(表格)是拖进ui的
    ui->tableWidget->setColumnCount(6);//列数要确定的
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<"姓名" << "编号"<< "手机号"
                                               <<"密码" <<"是否通过审核"<<"其它");
    //设置隔行变色
    ui->tableWidget->setAlternatingRowColors(true);
    //设置一次选择一行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设定列之间平分剩余水平空间
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ApplicantManage::~ApplicantManage()
{
    delete ui;
}

//将用户数据逐一添加到每一行
void ApplicantManage::appendRowData1(QStringList rowData)
{
    //返回所有行数  此行数也表示新添加一行对应的数字
    int row = ui->tableWidget->rowCount();

    //插入新行 行号-row
    ui->tableWidget->insertRow(row);

    for(int i  = 0 ; i < rowData.size() ; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(rowData.at(i));
        ui->tableWidget->setItem(row , i , item);
    }
    //设置表格里文本居中显示失败
//    for(int i=0; i<=row; i++)
//    {
//        QTableWidgetItem *item = new QTableWidgetItem;
//        for(int j=0; j<rowData.size(); j++)
//        {
//            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
//        }
//    }

}

void ApplicantManage::clearTableData1()
{
    //清除表格
    int rowCount = ui->tableWidget->rowCount();
    for(int r = rowCount-1 ; r >=0 ; r--)
    {
        ui->tableWidget->removeRow(r);
    }
}

int ApplicantManage::returnRowCount()//这个是自定义的
{
    return ui->tableWidget->rowCount();//系统函数
}

QStringList ApplicantManage::getCurrentRowData1()
{
    QStringList rowData;
    int row = ui->tableWidget->currentRow();
    if(row != -1)
    {
        rowData << ui->tableWidget->item(row , 0)->text();//注意这里是获取Tablewidget的数据，从0开始
        rowData << ui->tableWidget->item(row , 1)->text();
        rowData << ui->tableWidget->item(row , 2)->text();
        rowData << ui->tableWidget->item(row , 3)->text();
        rowData << ui->tableWidget->item(row , 4)->text();
        rowData << ui->tableWidget->item(row , 5)->text();
    }
    return rowData;
}

//获取当前行id
QString ApplicantManage::getCurrentNumber()
{
    QString number;//默认从编号1开始
    int row = ui->tableWidget->currentRow();
    if(row != -1)
    {
        number =  ui->tableWidget->item(row , 1)->text();
    }
    return number;
}
