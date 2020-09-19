#ifndef ADDAPPLICANTDLG_H
#define ADDAPPLICANTDLG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class AddApplicantDlg;
}

class AddApplicantDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddApplicantDlg(QWidget *parent = 0);
    ~AddApplicantDlg();

    void setEditData1(QStringList rowData);

private:
    void clearData1();

    void closeEvent1(QCloseEvent *event);

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void signalApplicantInfo1(QVariantMap stuInfo);

private:
    Ui::AddApplicantDlg *ui;

};

#endif // ADDAPPLICANTDLG_H

