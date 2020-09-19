#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QCoreApplication>
#include <QFont>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QTextCodec *pCodec = QTextCodec::codecForName("GBK");
    if(pCodec)
    {
        QTextCodec::setCodecForLocale(pCodec);
    }

    QCoreApplication::setApplicationName("BackManager");

    QApplication a(argc, argv);

    //样式表 不知道为啥——绝对路径才有效  ./Styles/white.qss ../Styles/white.qss后面两种都无效
    QFile file("F:/Qt code/CarSharingBackManager2/Styles/white.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString style = in.readAll();
        file.close();
        qApp->setStyleSheet(style);
    }

    QFont serifFont("Times", 12, QFont::Thin);
    a.setFont(serifFont);

    MainWindow w;
    w.setWindowTitle(QObject::tr("管理系统"));
    w.show();

    return a.exec();
}

