#ifndef APPLICANTMANAGEWIDGET_H
#define APPLICANTMANAGEWIDGET_H

#include <QWidget>

namespace Ui {
class ApplicantManage;
}

class ApplicantManage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicantManage(QWidget *parent = 0);
    ~ApplicantManage();

    void appendRowData1(QStringList rowData); //添加一行数据

    void clearTableData1(); //清除表格数据

    QStringList getCurrentRowData1();

    QString getCurrentNumber();

    int returnRowCount();

private:
    Ui::ApplicantManage *ui;
};

#endif // APPLICANTMANAGEWIDGET_H
