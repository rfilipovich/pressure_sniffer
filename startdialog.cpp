#include "startdialog.h"
#include "ui_startdialog.h"
#include "rtl_433.h"

//// addons
#define ARRAY_SIZE(a)                               \
  ((sizeof(a) / sizeof(*(a))) /                     \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

static const char *_groups_array[] = {
    "TPMS",
};

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    const QList<QByteArray> _list;
    for(size_t i = 0; i < ARRAY_SIZE(_groups_array); i++) {
        QString _str = QString::fromLatin1(_groups_array[i]);
        groups_list.append(_str);
    };
}

StartDialog::~StartDialog()
{
    delete ui;
}


void StartDialog::show(const QList<rtl_433_supported_protocols> &protos_list, const QList<quint32> & freq_list)
{
    this->protos_list = protos_list;

    /* fill this form */
    ui->comboBoxFreqs->clear();
    foreach(quint32 item, freq_list) {
        QString atext = QString::number((static_cast<float>(item))/1000000, /**/ 'f', /*prec*/3);
        atext.append(QString(" Mhz"));
        ui->comboBoxFreqs->addItem(atext, item);
    };

    /* full init for the groups */
    ui->comboBoxGroups->clear();
    ui->comboBoxGroups->addItem(QString("all"), 0);
    ui->comboBoxGroups->addItems(groups_list);

    /* show this form */
    qobject_cast<QDialog *>(this)->show();
}


void StartDialog::on_pushButtonStartDialogApply_clicked()
{
    QList<rtl_433_supported_protocols> protos_list;
    quint32 freq;

    if((ui->comboBoxModels->currentIndex() == 0) && (ui->comboBoxGroups->currentIndex() == 0)) {
    /* enable all protocols */
        const rtl_433_supported_protocols _all_proto = rtl_433_supported_protocols(this, 0, QString("all"), false);
        protos_list.append(_all_proto);
    };

    freq = ui->comboBoxFreqs->currentData().toInt();

    Q_EMIT signal_apply(protos_list, freq);

    this->hide();
}

void StartDialog::on_pushButtonStartDialogBack_clicked()
{
    Q_EMIT signal_break();

    this->hide();
}


void StartDialog::on_comboBoxGroups_currentIndexChanged(const QString &arg1)
{
    qDebug() << "on_comboBoxGroups_currentIndexChanged" << arg1;

    /* clean all */
    ui->comboBoxModels->clear();
    ui->comboBoxModels->addItem(QString("all"), 0);

    /* setting new */
    if(arg1 == QString("all")) {
        /* fill all from all */
        foreach(rtl_433_supported_protocols item, this->protos_list) {
            ui->comboBoxModels->addItem(item.get_proto_name(), item.get_proto_id());
        };
    } else {
        const QString _group_str = arg1;

        /* special group! */
        foreach(rtl_433_supported_protocols item, this->protos_list) {
            if(true == item.get_proto_name().contains(_group_str, Qt::CaseInsensitive)) {
                ui->comboBoxModels->addItem(item.get_proto_name(), item.get_proto_id());
            };
        };

    };
}
