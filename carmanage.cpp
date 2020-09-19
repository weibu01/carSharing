#include "carmanage.h"
#include "ui_carmanage.h" //添加carmanage.h对应的ui文件,不添加会报错
#include "applicantmanage.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QDebug>

CarManage::CarManage(QWidget *parent) : QWidget(parent), ui(new Ui::CarManage)
{
    ui->setupUi(this);//tableWidget(表格)是拖进ui的
    ui->tableWidget->setColumnCount(7);//列数要确定的
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<"车牌号" << "编号"<< "管理人电话"
                                               <<"车辆管理人" <<"是否在用车" <<"用车人姓名" <<"用车人电话");
    //设置隔行变色
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设定列之间平分剩余水平空间
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CarManage::~CarManage()
{
    delete ui;
}

//将用户数据逐一添加到每一行
void CarManage::appendRowData(QStringList rowData)
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

void CarManage::clearTableData()
{
    //清除表格
    int rowCount = ui->tableWidget->rowCount();
    for(int r = rowCount-1 ; r >=0 ; r--)
    {
        ui->tableWidget->removeRow(r);
    }
}

QStringList CarManage::getCurrentRowData()
{
    QStringList rowData;
    int row = ui->tableWidget->currentRow();
    if(row != -1)
    {
        rowData << ui->tableWidget->item(row , 0)->text();
        rowData << ui->tableWidget->item(row , 1)->text();
        rowData << ui->tableWidget->item(row , 2)->text();
        rowData << ui->tableWidget->item(row , 3)->text();
        rowData << ui->tableWidget->item(row , 4)->text();
        rowData << ui->tableWidget->item(row , 5)->text();
        rowData << ui->tableWidget->item(row , 6)->text();
    }
    return rowData;
}

//获取当前行id
QString CarManage::getCurrentNumber()
{
    QString number ;
    int row = ui->tableWidget->currentRow();
    if(row != -1)
    {
        number =  ui->tableWidget->item(row , 1)->text();
        qDebug()<<number;
    }
    return number;
}
