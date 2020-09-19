#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createdbone.h"
#include "obj.h"
#include <QObject>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextCodec>
#include <QThread>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QLayout>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUi();//显示学生管理、教师管理界面

    setTabCornerBtn();//显示右上角按钮、添加信号和槽      默认打开显示的

    //此信号和槽用来显示tab 对应的 右上角的功能按钮
    connect(m_pTabWidget,SIGNAL(currentChanged(int)),this,SLOT(showTabCornerBtn()));

    initAddDlg();//
    initAddDlg1();

    m_pCreateDb = new CreateDb;
    m_pCreateDb1 = new CreateDbOne;
    m_pCreateDb->initDB();  //创建、打开数据库0
    m_pCreateDb1->initDB1();  //创建、打开数据库1

    onBtnRefresh(); //程序启动时，刷新显示所有数据0
    onBtnRefresh1();//程序启动时，刷新显示所有数据1

    startTcpserver();//tcp通信用来获取注册信息：账号+密码

    showMaximized();//最大化显示窗口

    m_operateType = Add;//默认操作类型？？？
}

MainWindow::~MainWindow()
{
    delete ui;

}




//显示学生管理、教师管理界面 代码方式添加
void MainWindow::initUi()
{
    m_pCarManage=new CarManage;//管理器里有表格TableWidget
    m_pApplicantManage = new ApplicantManage;

    //左上角标签
    m_pTabWidget = new QTabWidget(this);
    //手动添加将m_pCarManage m_pApplicantManage添加到TabWidget控件中
    m_pTabWidget->addTab(m_pCarManage , tr("车辆管理")); //tr()???
    m_pTabWidget->addTab(m_pApplicantManage ,tr("用车人信息管理"));

    //设置样式表
    setStyleSheet("QTabBar::tab{min-width:30ex; min-height:10ex;}");

    setCentralWidget(m_pTabWidget);
    setContentsMargins(5 , 10, 5 ,5);
}


void MainWindow::showTabCornerBtn()
{

    int tabIndex=m_pTabWidget->currentIndex();
    //qDebug()<<tabIndex;打印哪个tab
    switch(tabIndex)
    {
        case 0: setTabCornerBtn();
            break;
        case 1: setTabCornerBtn1();
            break;
    }
}
//(1)在右上角显示tab0对应的功能按钮
void MainWindow::setTabCornerBtn()
{
    if(isDefined1==0)
    {
        //*widget = new QWidget; 在全局.h文件里实例化了 局部实例化会报错
        widget->setMinimumSize(20 ,55);//设置大小
        widget->setContentsMargins(0,0,0,10);

        QPushButton *btnRefresh = new QPushButton;
        btnRefresh->setText(tr("刷新"));

        QPushButton *btnAdd = new QPushButton;
        btnAdd->setText(tr("添加"));

        QPushButton *btnEdit = new QPushButton;
        btnEdit->setText(tr("编辑"));

        QPushButton *btnDel = new QPushButton;
        btnDel->setText(tr("删除"));

        //*hLayout = new QHBoxLayout(widget);//将四个按钮添加到水平布局管理器中
        hLayout->addWidget(btnRefresh);
        hLayout->addWidget(btnAdd);
        hLayout->addWidget(btnEdit);
        hLayout->addWidget(btnDel);

        widget->setLayout(hLayout);//往widget里装啥显示啥应该
        m_pTabWidget->setCornerWidget(widget ,Qt::TopRightCorner);//

        //四个按钮对应四个信号和槽
        connect(btnRefresh , SIGNAL(clicked(bool)) , this ,SLOT(onBtnRefresh()));
        connect(btnAdd , SIGNAL(clicked(bool)) , this ,SLOT(onBtnAdd()));
        connect(btnEdit , SIGNAL(clicked(bool)), this ,SLOT(onBtnEdit()));
        connect(btnDel , SIGNAL(clicked(bool)) ,this , SLOT(onBtnDel()));

        isDefined1=1;
    }
    widget1->setVisible(false);//隐藏tab1对应的功能按钮
    widget->setVisible(true);//显示tab0对应的功能按钮
}
//(1 刷新按钮——槽函数
void MainWindow::onBtnRefresh()
{
    qDebug()<<"onBtnRefresh";
    QList<QStringList> tableData;

    //m_pCreateDb——数据库对象
    tableData = m_pCreateDb->selectDataFromBase();
    m_pCarManage->clearTableData();//清除显示管理界面内容
    if(!tableData.isEmpty())
    {
        QListIterator<QStringList> itr(tableData);
        while(itr.hasNext())
        {
            m_pCarManage->appendRowData(itr.next());
        }
    }
}

//(2 添加按钮——槽函数
void MainWindow::onBtnAdd()
{
    qDebug()<<"onBtnAdd";
    m_operateType = Add;
    if(m_pAddCarDlg)
    {
        m_pAddCarDlg->activateWindow();//激活添加用户窗口，中间显示
        m_pAddCarDlg->setWindowTitle(tr("添加:车辆信息"));
        m_pAddCarDlg->exec();
    }
}
//(3 编辑按钮——槽函数
void MainWindow::onBtnEdit()
{
    qDebug()<<"onBtnEdit";
    m_operateType = Edit;

    QStringList rowData ;
    //m_pCreateDb——数据库对象
    rowData = m_pCarManage->getCurrentRowData();
    if(rowData.isEmpty())
    {
        QMessageBox::information(this, tr("提示") , tr("请选中需要编辑的数据!"));
        return ;
    }
    else
    {
        m_pAddCarDlg->setEditData(rowData);
    }

    m_pAddCarDlg->activateWindow();
    m_pAddCarDlg->setWindowTitle(tr("修改:车辆信息"));
    m_pAddCarDlg->exec();
}
//(4 删除按钮——槽函数
void MainWindow::onBtnDel()
{
    qDebug()<<"onBtnDel";
    QString number = m_pCarManage->getCurrentNumber();
    if(number.isEmpty())
    {
        QMessageBox::information(this , tr("提示") , tr("请选中一条记录！"));
        return ;
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this , tr("提示") ,tr("确定删除这一条记录？"));
        if(button == QMessageBox::Yes)
        {
            //删除操作
            ExecDelSql(number);
        }
    }
}
//a 删除数据库内容
void MainWindow::ExecDelSql(QString number)
{
    QString sql;
    sql = QString("DELETE FROM CarManage "
                  "where number = '%1' ").arg(number);//" where id = '%9' ").arg(number);

    QSqlQuery query;
    bool ok = query.exec(sql);//数据库执行删除语句
    if(ok)
    {
        onBtnRefresh(); //重新加载数据
        QMessageBox::information(this ,tr("提示") , tr("删除成功!"));
    }
    else
    {
        QMessageBox::information(this ,tr("提示") , tr("删除失败!"));
    }
}



//(2)在右上角显示tab1对应的功能按钮
void MainWindow::setTabCornerBtn1()
{
    if(isDefined==0)
    {
        //*widget1 = new QWidget; 在全局.h文件里实例化了 局部实例化会报错
        widget1->setMinimumSize(20 ,55);//设置大小
        widget1->setContentsMargins(0,0,0,10);

        QPushButton *btnEdit1 = new QPushButton;
        btnEdit1->setText(tr("编辑"));
        QPushButton *btnAdd1 = new QPushButton;
        btnAdd1->setText(tr("添加"));
        QPushButton *btnRefresh1 = new QPushButton;
        btnRefresh1->setText(tr("刷新"));

        //*hLayout1 = new QHBoxLayout(widget1);//将四个按钮添加到水平布局管理器中
        hLayout1->addWidget(btnEdit1);
        hLayout1->addWidget(btnAdd1);
        hLayout1->addWidget(btnRefresh1);

        widget1->setLayout(hLayout1);//往widget里装啥显示啥应该
        m_pTabWidget->setCornerWidget(widget1 ,Qt::TopRightCorner);//

        connect(btnAdd1 , SIGNAL(clicked(bool)) , this ,SLOT(onBtnAdd1()));
        connect(btnEdit1 , SIGNAL(clicked(bool)), this ,SLOT(onBtnEdit1()));
        connect(btnRefresh1, SIGNAL(clicked(bool)), this ,SLOT(onBtnRefresh1()));

        isDefined=1;//指定上面这些组将已经初始化过了，二次触发此槽函数时，不会重新定义
    }
    widget->setVisible(false);//隐藏tab0对应的功能按钮
    widget1->setVisible(true);//显示tab1对应的功能按钮
}

//(1 刷新按钮——槽函数
void MainWindow::onBtnRefresh1()
{
    qDebug()<<"onBtnRefresh1";
    QList<QStringList> tableData;

    //m_pCreateDb——数据库对象(包含数据库所有数据)
    tableData = m_pCreateDb1->selectDataFromBase1();
    m_pApplicantManage->clearTableData1();//清除显示管理界面内容
    if(!tableData.isEmpty())
    {
        QListIterator<QStringList> itr(tableData);
        while(itr.hasNext())
        {
            m_pApplicantManage->appendRowData1(itr.next());
        }
    }
}

//(2 添加按钮——槽函数
void MainWindow::onBtnAdd1()
{
    qDebug()<<"onBtnAdd1";
    m_operateType = Add;
    if(m_pAddApplicantDlg)
    {
        m_pAddApplicantDlg->activateWindow();//激活添加用户窗口，中间显示
        m_pAddApplicantDlg->setWindowTitle(tr("添加: 用户资料"));
        m_pAddApplicantDlg->exec();
    }
}

//(3 编辑按钮——槽函数
void MainWindow::onBtnEdit1()
{
    qDebug()<<"onBtnEdit1";
    m_operateType = Edit;

    QStringList rowData ;
    //m_pCreateDb——数据库对象
    rowData = m_pApplicantManage->getCurrentRowData1();
    if(rowData.isEmpty())
    {
        QMessageBox::information(this, tr("提示") , tr("请选中需要编辑的数据!"));
        return ;
    }
    else
    {
        m_pAddApplicantDlg->setEditData1(rowData);
    }

    m_pAddApplicantDlg->activateWindow();
    m_pAddApplicantDlg->setWindowTitle(tr("编辑: 用户资料"));
    m_pAddApplicantDlg->exec();
}



void MainWindow::initAddDlg()
{
    m_pAddCarDlg = new AddCarDlg;
    connect(m_pAddCarDlg , SIGNAL(signalCarInfo(QVariantMap)) , this ,SLOT(ExecAddSql(QVariantMap)));
    connect(m_pAddCarDlg , SIGNAL(signalCarInfo(QVariantMap)) , this ,SLOT(ExecEditSql(QVariantMap)));
}
//(1)槽函数——将从AddCarDlg.cpp里通过信号传来新用户数据添加到数据库 并显示
void MainWindow::ExecAddSql(QVariantMap carInfo)
{
    if(m_operateType == Add)
    {
        QString create_date = carInfo.value("datetime").toString();
        QString number = carInfo.value("number").toString();
        QString carNumber = carInfo.value("carNumber").toString();
        QString managerPhone = carInfo.value("managerPhone").toString();
        QString carManager = carInfo.value("carManager").toString();
        QString isUse = carInfo.value("isUse").toString();
        QString userName = carInfo.value("userName").toString();
        QString userPhone = carInfo.value("userPhone").toString();

        QString sql;//打包用户所有数据
        sql = QString("insert into CarManage (create_date,number ,carNumber ,managerPhone ,carManager ,isUse , userName ,userPhone)"
                      " VALUES ('%1', '%2' , '%3' , '%4' , '%5' , '%6' , '%7' , '%8')")
                .arg(create_date).arg(number).arg(carNumber).arg(managerPhone).arg(carManager).arg(isUse).arg(userName).arg(userPhone);

        QSqlQuery query;
        bool ok = query.exec(sql);//添加用户所有数据到数据库
        if(ok)
        {
            QStringList rowData;
            rowData << carNumber <<number <<managerPhone <<carManager << isUse <<userName << userPhone ;
            m_pCarManage->appendRowData(rowData);

            QMessageBox::information(this ,tr("提示") , tr("添加成功!"));
        }
        else
        {
            QMessageBox::information(this ,tr("提示") , tr("添加失败!"));
        }
    }
}
//(2)槽函数——将从AddCarDlg.cpp里通过信号传的 重新编辑过的用户数据 添加到数据库 并显示
void MainWindow::ExecEditSql(QVariantMap carInfo)
{
    if(m_operateType == Edit)
    {
        QString create_date = carInfo.value("datetime").toString();
        QString number = carInfo.value("number").toString();
        QString carNumber = carInfo.value("carNumber").toString();
        QString managerPhone = carInfo.value("managerPhone").toString();
        QString carManager = carInfo.value("carManager").toString();
        QString isUse = carInfo.value("isUse").toString();
        QString userName = carInfo.value("userName").toString();
        QString userPhone = carInfo.value("userPhone").toString();

        QString sql;
        sql = QString("UPDATE CarManage "
                      "SET create_date = '%0' ,number = '%1' ,carNumber = '%2',"
                      "managerPhone = '%3',carManager = '%4',isUse = '%5', userName = '%6',userPhone = '%7'"
                      "WHERE number = '%8'")
                .arg(create_date).arg(number).arg(carNumber).arg(managerPhone).arg(carManager).arg(isUse).arg(userName).arg(userPhone).arg(number);

        QSqlQuery query;
        bool ok = query.exec(sql);
        if(ok)
        {
            onBtnRefresh(); //重新加载数据
            QMessageBox::information(this ,tr("提示") , tr("修改成功!"));
        }
        else
        {
            QMessageBox::information(this ,tr("提示") , tr("修改失败!"));
        }
    }
}



void MainWindow::initAddDlg1()
{
    m_pAddApplicantDlg = new AddApplicantDlg;
    connect(m_pAddApplicantDlg , SIGNAL(signalApplicantInfo1(QVariantMap)) , this ,SLOT(ExecAddSql1(QVariantMap)));
    connect(m_pAddApplicantDlg , SIGNAL(signalApplicantInfo1(QVariantMap)) , this ,SLOT(ExecEditSql1(QVariantMap)));
}
//(1)槽函数——将从AddCarDlg.cpp里通过信号传来新用户(编辑界面)数据添加到数据库 并显示
void MainWindow::ExecAddSql1(QVariantMap applicantInfo)
{
    if(m_operateType == Add)
    {
        QString create_date = applicantInfo.value("datetime").toString();
        QString name = applicantInfo.value("name").toString();
        QString number = applicantInfo.value("number").toString();
        QString phone = applicantInfo.value("phone").toString();
        QString password = applicantInfo.value("password").toString();
        QString isCheck = applicantInfo.value("isCheck").toString();
        QString other = applicantInfo.value("other").toString();

        QString sql;//打包用户所有数据,此处数据从编辑用户资料界面传来
        sql = QString("insert into ApplicantManage (create_date ,name ,number ,phone ,password ,isCheck ,other)"
                      " VALUES ('%1', '%2' , '%3' , '%4' , '%5' , '%6' , '%7')")
                .arg(create_date).arg(name).arg(number).arg(phone).arg(password).arg(isCheck).arg(other);

        QSqlQuery query;
        bool ok = query.exec(sql);//添加用户所有数据到数据库,另一个单独文件 insert into CarManage -- sql语句
        if(ok)
        {
            QStringList rowData;
            rowData << name <<number <<phone <<password <<isCheck << other;
            m_pApplicantManage->appendRowData1(rowData);//添加用户数据到m_pCarManage对象中，显示的表格界面

            QMessageBox::information(this ,tr("提示") , tr("添加成功!"));
        }
        else
        {
            QMessageBox::information(this ,tr("提示") , tr("添加失败!"));
        }
    }
}
//(2)槽函数——将从AddCarDlg.cpp里通过信号传的 重新编辑过的用户数据 添加到数据库并显示
void MainWindow::ExecEditSql1(QVariantMap applicantInfo)
{
    if(m_operateType == Edit)
    {
        QString create_date = applicantInfo.value("datetime").toString();
        QString name = applicantInfo.value("name").toString();
        QString number = applicantInfo.value("number").toString();
        QString phone = applicantInfo.value("phone").toString();
        QString password = applicantInfo.value("password").toString();
        QString isCheck = applicantInfo.value("isCheck").toString();
        QString other = applicantInfo.value("other").toString();

        QString sql;
        sql = QString("UPDATE ApplicantManage "
                      "SET create_date = '%1' ,name = '%2' ,number = '%3',"
                      "phone = '%4',password = '%5',isCheck = '%6',other = '%7'"
                      " where number ='%8'")
                .arg(create_date).arg(name).arg(number).arg(phone).arg(password).arg(isCheck).arg(other).arg(number);

        QSqlQuery query;
        bool ok = query.exec(sql);//用户编辑的数据->加载到数据库
        if(ok)
        {
            onBtnRefresh1(); //用户编辑的数据->显示在界面上
            QMessageBox::information(this ,tr("提示") , tr("编辑成功!"));
        }
        else
        {
            QMessageBox::information(this ,tr("提示") , tr("编辑失败!"));
        }
    }
}




void MainWindow::startTcpserver()
{
    m_tcpServer = new QTcpServer(this);
    m_tcpServer->listen(QHostAddress::Any,8100); //监听任何连上9999端口的ip
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnect())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样，其实你可以随便取。
}
//(1)获取socket 准备读取、发送
void MainWindow::newConnect()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection(); //得到每个连进来的socket
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽
}
//(1 读取信息
void MainWindow::readMessage() //读取信息
{
    QStringList allString;//保存用户数据registerInfoList
    qint64 len = m_tcpSocket->bytesAvailable();
    qDebug()<<"socket data len:"<< len;
    if(len>0)//len>0说明读取用户注册数据进来,才执行后面的操作，不然可能会越界，引起程序崩溃
    {
        QByteArray alldata = m_tcpSocket->readAll();
        //开始转换编码
        QTextCodec *utf8codec = QTextCodec::codecForName("UTF-8");
        QString utf8str = utf8codec->toUnicode(alldata.mid(2));
        qDebug()<<"111-------"<<utf8str;

        if(utf8str.left(5) != "flag_")//读进来的是文件
        {
            applicant_check(alldata);
        }
        else //读进来的是文字
        {
            allString=utf8str.split('|');//分开用户注册信息 账号+密码
            QString m_flag=allString[0];
            switch(flag_convert(m_flag))//数据分流处理
            {
            case flag_registerInfo:
                applicant_register(allString);break;
            case flag_logInInfo:
                applicant_logIn(allString);break;
            //case flag_imageInfo:
            //    applicant_check(alldata);break;
            case flag_usingCarInfo:
                open_car(allString);break;
            }
        }
    }
}
//<0>字符串转整型（枚举）
int MainWindow::flag_convert(QString flag)
{
    if(flag=="flag_registerInfo")
        return 0;
    else if(flag=="flag_logInInfo")
        return 1;
    else
        return 2;//此时flag=="flag_imageInfo"
}

//<1>用户注册处理,添加新用户资料到数据库
void MainWindow::applicant_register(QStringList allString)
{
    QStringList registerInfoList=allString;
    qDebug()<<"有用户注册中... 账号:"<<registerInfoList[1]<<"密码:"<<registerInfoList[2];
    //if(!registerInfoList.isEmpty())//判断是否读取用户注册的数据进来，有时用户点太快了，数据传不进来，运行程序会越界,程序会崩溃


    //判断数据库中是否注册过此账号（手机号）
    QList<QStringList> applicantPhoneData=selectColumnDataFromBase1();
    if(!applicantPhoneData.isEmpty())//检查是否从数据库中取出数据
    {
        QListIterator<QStringList> phoneColumn(applicantPhoneData);//类型转换便于调用next()函数
        while(phoneColumn.hasNext())//itr是QStringList类型
        {
            QStringList phone=phoneColumn.next();
            if(registerInfoList[1]==phone.at(0))
            {
                qDebug()<<phone.at(0)<<"  此账号已注册过";
                sendMessageToClient(0);
                return;//注册过此账号，之后的程序不运行
            }
        }
    }


    //添加新用户数据到数据库
    //UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing' WHERE LastName = 'Wilson'
    //获取当前系统时间
    QString create_date = QDateTime::currentDateTime().toString();
    //获取行数，第几位用户
    int rowCount=m_pApplicantManage->returnRowCount();
    QString number=tr("%1").arg(rowCount+1);//整型转字符串  +1 新添加一行
    QString sql;
    sql = QString("insert into ApplicantManage (create_date ,number ,phone ,password ,isCheck)"
                  " VALUES ('%1', '%2' , '%3' , '%4' , '%5')")
            .arg(create_date).arg(number).arg(registerInfoList[1]).arg(registerInfoList[2]).arg("否");

    QSqlQuery query;
    bool ok = query.exec(sql);//用户编辑的数据->加载到数据库
    if(ok)
    {
        onBtnRefresh1(); //用户编辑的数据->显示在界面上
        QMessageBox::information(this ,tr("提示") , tr("注册成功!"));
    }
    else
    {
        QMessageBox::information(this ,tr("提示") , tr("注册失败!"));
    }
}
//<1 查询返会数据库里账号列数据  返回QList<QStringList>
QList<QStringList> MainWindow::selectColumnDataFromBase1()
{
    //sql语句
    QSqlQuery query("SELECT phone FROM ApplicantManage");//查询数据库里的phone这一列所有数据

    QList<QStringList> applicantPhoneData;

    while (query.next())
    {
        QStringList columnData ;//数据库里 每一行数据由8个数据组成,用QStringList存放每一行数据

        columnData <<query.value(0).toString();

        applicantPhoneData.append(columnData);//最后再用QList将表格里所有列数据存储起来
    }
    return applicantPhoneData;//将所有数据返回给调用此函数的调用者
}

//<2>用户登录处理：检查数据库中是否有此账号，密码是否对，核对后允许登录
void MainWindow::applicant_logIn(QStringList allString)
{
    //用户登录时的账号+密码
    QString logIn_account=allString[1];
    QString logIn_password=allString[2];
    //判断数据库中是否注册过此账号（手机号）
    QList<QStringList> applicant3ColumnData=select3ColumnDataFromBase1();
    if(!applicant3ColumnData.isEmpty())//检查是否从数据库中取出数据
    {
        QListIterator<QStringList> phone_Password_IscheckData(applicant3ColumnData);//类型转换便于调用next()函数
        while(phone_Password_IscheckData.hasNext())//itr是QStringList类型
        {
            QStringList phone_Password_Ischeck=phone_Password_IscheckData.next();
            if(logIn_account==phone_Password_Ischeck.at(0) && logIn_password==phone_Password_Ischeck.at(1))
            {
                m_pcheckWindow = new CheckWindow;
                m_pcheckWindow->gainAccount(phone_Password_Ischeck.at(0));//m_pcheckWindow在此初始化，并将登录的用户账号发到此窗口，后面会用到此账号（用户通过审核，修改数据库用户通过审核）

                if(phone_Password_Ischeck.at(2) == "是")
                {
                    qDebug()<<"登录成功,通过审核";
                    m_logInfo = "登录成功,通过审核";
                }else
                {
                    qDebug()<<"登录成功,未通过审核";
                    m_logInfo = "登录成功,未通过审核";
                }
                sendMessageToClient(1);
                return;//使得不执行后面的语句
            }
        }
        //遍历完数据库没找到对应的账号、密码，说明此账号或密码出错
        qDebug()<<"账号或密码错误mmm";
        sendMessageToClient(2);
    }

}
//<1 查询返会数据库里账号+密码列数据  返回QList<QStringList>
QList<QStringList> MainWindow::select3ColumnDataFromBase1()
{
    //sql语句
    QSqlQuery query("SELECT phone,password,ischeck FROM ApplicantManage");//查询数据库里的phone这一列所有数据

    QList<QStringList> phone_Password_IscheckData;

    while (query.next())
    {
        QStringList columnData ;//数据库里 每一行数据由8个数据组成,用QStringList存放每一行数据

        columnData <<query.value(0).toString();
        columnData <<query.value(1).toString();
        columnData <<query.value(2).toString();

        phone_Password_IscheckData.append(columnData);//最后再用QList将表格里所有列数据存储起来
    }
    return phone_Password_IscheckData;//将所有数据返回给调用此函数的调用者
}
//<2 发送信息
void MainWindow::sendMessageToClient(int sendWhat)
{
    QString strMesg0;
    QString strMesg1;
    QString strMesg2;
    switch(sendWhat)
    {
    //用户界面提示账号注册过
    case 0:
        strMesg0="此账号已注册过";
        m_tcpSocket->write(strMesg0.toStdString().c_str(),strlen(strMesg0.toStdString().c_str()));
        qDebug()<<strMesg0.toStdString().c_str();
        break;
    //用户界面提示登录成功
    case 1:
        strMesg1=m_logInfo;
        m_tcpSocket->write(strMesg1.toStdString().c_str(),strlen(strMesg1.toStdString().c_str()));
        m_logInfo="";//全局变量，用过一次后清空
        break;
    //用户界面提示（登录时） 账号或密码错误
    case 2:
        strMesg2="账号或密码错误";
        m_tcpSocket->write(strMesg2.toStdString().c_str(),strlen(strMesg2.toStdString().c_str()));
        break;
    }
}


//<3> 用户审核处理：获取用户相关证件图片,保存图片
void MainWindow::applicant_check(QByteArray allData)
{
    if(false == isStartReadFile)//接收文件头部信息
    {
        isStartReadFile = true;

        QString StringData = allData;
        QStringList strList = StringData.split('|');//strList[0]-name+account
                                                    //strList[1]-imageSize
        fileSize = strList[1].toInt();
        qDebug()<<"filesize is"<<fileSize;
        qDebug()<<"debuf----------"<<StringData;

        //QDir tempDir;
        currentDir=tempDir.currentPath();//后面要设置回来？？
        qDebug() << currentDir;


        QString path = currentDir + "/用户信息/"+strList[0]; //目录路径
        qDebug() << "path is :" <<path;
        if(!tempDir.exists(path))
        {
            tempDir.mkpath(path);
        }
        tempDir.setCurrent(path);
        qDebug() << tempDir.currentPath();
        QString fileName[] = {"id1.jpg","id2.jpg","drivingLicence.jpg"};              
        if(file.exists(fileName[0]) && file.exists(fileName[1]) && file.exists(fileName[2]))
        {
            //如果三个文件同时存在，说明用户之前发过三张图片，删除之前发的，重新接收新发的这组
            tempDir.remove(fileName[0]);
            tempDir.remove(fileName[1]);
            tempDir.remove(fileName[2]);
        }
        //打开文件
        for(int i=0; i<3 ;i++)
        {
            QFileInfo fileInfo(fileName[i]);
            if(!fileInfo.exists())
            {
                file.setFileName(fileName[i]);//在客户端创建文件
                break;
            }
        }
        bool isOk = file.open(QIODevice::WriteOnly);
        if(isOk == false)
        {
            qDebug() <<"WriteOnly error 34";
        }
    }
    else //isStartReadFile==true
    {
        //QByteArray buf=utf8str.toUtf8();
        qint64 len = file.write(allData);
        qDebug()<<"len2 == " <<len;
        if(len>0)
        {
            recvSize+=len;
            if(recvSize>50000)
                qDebug()<<"recvsize is"<<recvSize;
        }
        //file.size()
        if(file.size() == fileSize)//不能用recvSize == fileSize奇怪
        {
            qDebug()<<"总大小为:"<<recvSize << "接收完一个图片";
            file.close();//关闭打开的文件

            isStartReadFile = false;       //设置为false 下次来时先执行if{}语句
            recvSize = 0;//重新置0

            imageNumber++;
            if(imageNumber == 3)//说明接收了3张图片
            {
                QString str="图片上传完成,人工审核中,请稍后...";
                m_tcpSocket->write(str.toStdString().c_str(),strlen(str.toStdString().c_str()));

                //m_pcheckWindow = new CheckWindow;  在其他地方初始化了
                m_pcheckWindow->gainSocket(m_tcpSocket);//通过public方法将socket传到另一个窗口
                m_pcheckWindow->showImage();//显示审核的图片到窗口上
                m_pcheckWindow->activateWindow();
                m_pcheckWindow->setWindowTitle(tr("新用户信息审核\n%1").arg(tempDir.currentPath()));
                m_pcheckWindow->exec();

                onBtnRefresh1();//退出checkWindow界面后重新刷新用户界面，可能有用户通过审核

                imageNumber=0;//重新设置为0        
            }
            tempDir.setCurrent(currentDir);//设置为原来的路径
        }

    }
}

void MainWindow::open_car(QStringList allString)
{
    qDebug()<<"---------"<<allString[1]<<allString[2];
    QString armIP;
    QStringList carNumber_and_armIP;
    for(int i=0; i<carNumber_IP.count(); i++)
    {
        carNumber_and_armIP = carNumber_IP[i].split("|");
        if(allString[2] == carNumber_and_armIP[0])//java中是用equal()函数  这步主要是根据车牌号查arm IP，
        {                                          //ARM IP和车牌号是一一对应的
            armIP = carNumber_and_armIP[1];
            break;
        }
    }
    //QThread *mythread = new QThread(this);
    Obj *obj = new Obj();
    QThread *mythread = new QThread(this);
    obj->gainCarMessage(allString[1]+allString[2],armIP);//传参

    obj->moveToThread(mythread);//将object类移动到线程中执行
    mythread->start();//线程开始,触发statted信号，转去执行Obj中的槽函数work() 在线程里要做的事放到这里去做
    connect(mythread,SIGNAL(started()),obj,SLOT(work()));
    connect(mythread,SIGNAL(finished()),mythread,SLOT(quit()));
}







