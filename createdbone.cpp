#include "createdbone.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

CreateDbOne::CreateDbOne()
{

}//CreateDbOne类的构造函数里没有操作


void CreateDbOne::initDB1()
{
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE","applicanttable");//添加SQLITE数据库
    if(QSqlDatabase::contains("applicanttable"))
      db1 = QSqlDatabase::database("applicanttable");
    else
      db1 = QSqlDatabase::addDatabase("QSQLITE","applicanttable");

    db1.setDatabaseName("DatabaseName1");//数据库名,为啥新建的表放不进去，而是放到了之前的数据库文件里了。。。
    db1.setHostName("HostName1.db");//主机名

    bool ok = db1.open();//打开数据库
    if(ok)//判断是否打开
    {
        qDebug()<<"Create DB1";

        //SQL语句
        QSqlQuery query;
        query.exec("create table ApplicantManage (create_date datetime, "
                   "name varchar(20) , number varchar(20) ,"
                   "phone varchar(20) ,password varchar(20) ,ischeck varchar(20),"
                   "other varchar(40))");
    }
    else
    {
        qDebug()<<db1.databaseName();//会打印数据库打开失败
    }
}

//查询返会数据库里所有列数据  返回QList<QStringList>
QList<QStringList> CreateDbOne::selectDataFromBase1()
{
    //sql语句
    QSqlQuery query("SELECT * FROM ApplicantManage");//来自 表ApplicantManage * 表示查询数据库里的所有数据

    QList<QStringList> applicantInfo;

    while (query.next())
    {
        QStringList rowData ;//数据库里 每一行数据由8个数据组成,用QStringList存放每一行数据

        rowData <<query.value(1).toString();
        rowData <<query.value(2).toString();
        rowData <<query.value(3).toString();
        rowData <<query.value(4).toString();
        rowData <<query.value(5).toString();
        rowData <<query.value(6).toString();


        applicantInfo.append(rowData);//最后再用QList将表格里所有行数据存储起来
    }
    return applicantInfo;//将所有数据返回给调用此函数的调用者
}


