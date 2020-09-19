#include "createdb.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

CreateDb::CreateDb()//CreateDb类的构造函数里没有操作
{

}

void CreateDb::initDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//添加SQLITE数据库

    db.setHostName("HostName.db");//主机名
    db.setDatabaseName("DatabaseName");//数据库名,
    bool ok = db.open();//打开数据库
    if(ok)//判断是否打开
    {
        qDebug()<<"Create DB";

        //SQL语句
        QSqlQuery query;
        query.exec("create table CarManage (create_date datetime, "
                   "number varchar(20) , carNumber varchar(20) ,"
                   " managerPhone varchar(20) ,carManager INTEGER ,"
                   "isUse varchar(20) , userName varchar(12) , userPhone varchar(50))");
    }
    else
    {
        qDebug()<<db.databaseName();//会打印数据库打开失败
    }
}

//查询返会数据库里所有数据  返回QList<QStringList>
QList<QStringList> CreateDb::selectDataFromBase()
{
    //sql语句
    QSqlQuery query("SELECT * FROM CarManage");//来自 表CarManage * 表示查询数据库里的所有数据

    QList<QStringList> carInfo;

    while (query.next())
    {
        QStringList rowData ;//数据库里 每一行数据由8个数据组成,用QStringList存放每一行数据

        rowData <<query.value(2).toString();//姓名放在表的最前面
        rowData <<query.value(1).toString();
        rowData <<query.value(3).toString();
        rowData <<query.value(4).toString();
        rowData <<query.value(5).toString();
        rowData <<query.value(6).toString();
        rowData <<query.value(7).toString();
        rowData <<query.value(8).toString();

        carInfo.append(rowData);//最后再用QList将表格里所有行数据存储起来
    }
    return carInfo;//将所有数据返回给调用此函数的调用者
}

