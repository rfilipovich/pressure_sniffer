#include "startdialog.h"
#include "ui_startdialog.h"
#include "rtl_433.h"

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);
}

StartDialog::~StartDialog()
{
    delete ui;
}


void StartDialog::show(const QList<rtl_433_supported_protocols> &proto_list, const QList<quint32> & freq_list)
{
    /* fill this form */
    foreach(quint32 item, freq_list) {
        QString atext = QString::number((static_cast<float>(item))/1000000, /**/ 'f', /*prec*/3);
        atext.append(QString(" Mhz"));
        ui->comboBoxFreq->addItem(atext, item);
    };

    /* show this form */
    qobject_cast<QDialog *>(this)->show();
}


void StartDialog::on_pushButtonStartDialogApply_clicked()
{
    rtl_433_supported_protocols proto;
    quint32 freq;

    Q_EMIT signal_apply(proto, freq);

    this->hide();
}

void StartDialog::on_pushButtonStartDialogBack_clicked()
{
    Q_EMIT signal_break();

    this->hide();
}

