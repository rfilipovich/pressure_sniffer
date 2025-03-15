#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}

class rtl_433_supported_protocols;

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();

public Q_SLOTS:
    void show(const QList<rtl_433_supported_protocols> &proto_list, const QList<quint32> & freq_list);

private Q_SLOTS:
    void on_pushButtonStartDialogApply_clicked();
    void on_pushButtonStartDialogBack_clicked();

Q_SIGNALS:
    void signal_apply(const rtl_433_supported_protocols &proto, const quint32 & freq);
    void signal_break(void);

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
