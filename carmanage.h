#ifndef CARMANAGEWIDGET_H
#define CARMANAGEWIDGET_H

#include <QWidget>

namespace Ui {
class CarManage;
}

class CarManage : public QWidget
{
    Q_OBJECT

public:
    explicit CarManage(QWidget *parent = 0);
    ~CarManage();

    void appendRowData(QStringList rowData); //添加一行数据

    void clearTableData(); //清除表格数据

    QStringList getCurrentRowData();

    QString getCurrentNumber();

private:
    Ui::CarManage *ui;
};

#endif // CARMANAGEWIDGET_H
