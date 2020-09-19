#ifndef ADDCARDLG_H
#define ADDCARDLG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class AddCarDlg;
}

class AddCarDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddCarDlg(QWidget *parent = 0);
    ~AddCarDlg();

    void setEditData(QStringList rowData);

private:
    void clearData();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void signalCarInfo(QVariantMap carInfo);

private:
    Ui::AddCarDlg *ui;

};

#endif // ADDCARDLG_H
