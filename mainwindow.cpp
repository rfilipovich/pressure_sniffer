#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QTextBlock>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QListWidgetItem>
#include <QBrush>
#include <QJsonValue>
#include <QCollator>
#include "SysConfig.h"


//// addons
#define ARRAY_SIZE(a)                               \
  ((sizeof(a) / sizeof(*(a))) /                     \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


/* main class */
MainWindow::MainWindow(SysConfig *sysCfg, QWidget *parent) :
    QMainWindow(parent), p_rtl433(NULL), p_linux(NULL),
    ui(new Ui::MainWindow)
{
    Q_ASSERT(sysCfg);
    this->sysCfg = sysCfg;

    ui->setupUi(this);

   /* setting default tabwidget */
    ui->tabWidgetMain->setCurrentIndex(MAIN_TAB_CURENT_INDEX);

    initAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initAllSubForms(QWidget *parent)
{
/// disable debug tab for the default
    ui->tabWidgetMain->setTabVisible(EN_DEBUG_TAB_INDEX, false);

/// prepering freq changing form
    p_freq_form = new freq_change_form(parent);
    p_freq_form->hide();

    p_StartDialog = new StartDialog(parent);

    return true;
}

bool MainWindow::initAll()
{
/**** register meta types from data structs */
    data_structs_register_meta_types();

    //!    connect(client, &ClientSideEmulation::onFinishRead, this, &MainWindow::slot_fillDbgLog);

    initAllSubForms(this);

/* preinit forms if need */
    ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: blue");
    ui->pushButtonMainStart->setStyleSheet("background-color: blue");

/* init classes */
// RTL433
    p_rtl433 = new rtl_433(this);

    /* sometimes need stop firstly the rtl_433 */
    Q_EMIT p_rtl433->stop_rtl433();

/* connetion slots for the rtl_433 */
    connect(p_rtl433, &rtl_433::signal_rtl433ProcessRawOutput, this, &MainWindow::slot_fillRTL433RawLog);
 //....
// linux shell commands
    p_linux = new linux_shell(this);

/* main tab */
    {
        QScrollBar *_scroll_bar_V = new QScrollBar();
        _scroll_bar_V->setStyleSheet("QScrollBar:vertical { width: 25px; } ");
        ui->listWidgetMainTable->setVerticalScrollBar(_scroll_bar_V);

        QScrollBar *_scroll_bar_H = new QScrollBar();
        _scroll_bar_H->setStyleSheet("QScrollBar:horizontal { height: 25px; } ");
        ui->listWidgetMainTable->setHorizontalScrollBar(_scroll_bar_H);
    };

/* setting the rtl433 form */
    QScrollBar *textEditRTL433RawLogBar = new QScrollBar();
    textEditRTL433RawLogBar->setStyleSheet("QScrollBar:vertical { width: 30px; } ");

    //
    ui->textEditRTL433RawLog->setVerticalScrollBar(textEditRTL433RawLogBar);

/* settings tab */
    ui->settingsTabSliderBacklight->setStyleSheet("QSlider::handle:horizontal { \
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f); \
        border: 2px solid #5c5c5c; \
        width: 28px; \
        margin: -2px 0; \
        border-radius: 5px; \
        }");


#if 0
    ///////////////// main list DBG
    ///
    for(int i = 0; i < 20; i++) {
        QString str;
            str.append(i);
            str.append("sjfajiaj'fjaifnha[ghaiognainganga :lkalflanfkjanfanlfknalkfnakfnfn");
        QListWidgetItem *_aitem = new QListWidgetItem(str);
        ui->listWidgetMainTable->addItem(_aitem);
    };
    //////////////////////////////////////////
#endif

/* preinit forms if need */
    on_tabWidgetMain_currentChanged(ui->tabWidgetMain->currentIndex());

    return true;
}


////////////// main TAB ///////////////////

void MainWindow::on_pushButtonMainStart_clicked()
{
    if (false == p_rtl433->is_started()) {
        const quint32 _freq_array_hz[] =
        {   433920000,
            868000000,
            315000000,
            345000000,
            915000000,
        };

        const std::list<quint32> freq_std_list(_freq_array_hz + 0, _freq_array_hz + ARRAY_SIZE(_freq_array_hz));
        QList<quint32> freq_list = QList<quint32>(freq_std_list.begin(), freq_std_list.end());

        /* get supported protocols */
        QList<rtl_433_supported_protocols> proto_list;
        p_rtl433->get_supported_protocols_rtl433(proto_list);

        connect(p_StartDialog, &StartDialog::signal_apply, this, &MainWindow::slot_tmain_start_dialog_apply);
        connect(p_StartDialog, &StartDialog::signal_break, this, &MainWindow::slot_tmain_start_dialog_break);

        /* show start form */
        p_StartDialog->show(proto_list, freq_list);
    } else {
        ui->pushButtonMainStart->setStyleSheet("background-color: blue");
        ui->pushButtonMainStart->setText("Start");

        /* disconnect slots */
        disconnect(p_rtl433, &rtl_433::signal_rtl433ProcessOutput, this, &MainWindow:: slot_main_tab_rtl433ProcessOutput);
        disconnect(p_rtl433, &rtl_433::signal_rtl433Finished, this, &MainWindow:: slot_main_tab_rtl433Finished);

        Q_EMIT p_rtl433->stop_rtl433();
    };
}

void MainWindow::slot_tmain_start_dialog_apply(const QList<quint16> &proto_id_list, const quint32 & freq)
{
    disconnect(p_StartDialog, &StartDialog::signal_apply, this, &MainWindow::slot_tmain_start_dialog_apply);
    disconnect(p_StartDialog, &StartDialog::signal_break, this, &MainWindow::slot_tmain_start_dialog_break);

    qDebug() << "slot_tmain_start_dialog_apply proto_list:" << proto_id_list << "freq:" << freq;

    if (false == p_rtl433->is_started()) {
        ui->pushButtonMainStart->setStyleSheet("background-color: red");
        ui->pushButtonMainStart->setText("Stop");

        ui->listWidgetMainTable->clear();

    //    QList<quint16> protocols;
    //    foreach (rtl_433_supported_protocols var, protos_list) {
    //        protocols.append(var.get_proto_id());
    //    };

        connect(p_rtl433, &rtl_433::signal_rtl433ProcessOutput, this, &MainWindow:: slot_main_tab_rtl433ProcessOutput);
        connect(p_rtl433, &rtl_433::signal_rtl433Finished, this, &MainWindow:: slot_main_tab_rtl433Finished);

        //// init statistics
        recs_statistics.reset();
        ui->labelStatistics->setText(recs_statistics.get_statiscs_string());

        Q_EMIT p_rtl433->start_rtl433(freq, proto_id_list);
    };
}

void MainWindow::slot_tmain_start_dialog_break(void)
{
    disconnect(p_StartDialog, &StartDialog::signal_apply, this, &MainWindow::slot_tmain_start_dialog_apply);
    disconnect(p_StartDialog, &StartDialog::signal_break, this, &MainWindow::slot_tmain_start_dialog_break);
}

void MainWindow::slot_main_tab_rtl433Finished()
{
    disconnect(p_rtl433, &rtl_433::signal_rtl433ProcessOutput, this, &MainWindow:: slot_main_tab_rtl433ProcessOutput);
    disconnect(p_rtl433, &rtl_433::signal_rtl433Finished, this, &MainWindow:: slot_main_tab_rtl433Finished);

    /* imitate pressing the button */
    on_pushButtonMainStart_clicked();
}

static void sort_qstring_list(QStringList &list)
{
    QCollator collator;
    collator.setNumericMode(true);

    std::sort(
        list.begin(),
        list.end(),
        [&](const QString &key1, const QString &key2)
        {
            return collator.compare(key1, key2) < 0;
        });
}


QString MainWindow::fill_item_from_json(const quint32 &index, const QJsonObject& json_object)
{
    QString output;
    QString type;

    QString model = json_object.value("model").toString();
    if(model.length() > 10) {
    /* delete midle for mte string, len must be maximum 10 chars */
        model = model.left(5) + QString('.') + model.right(5);
    };

    if(json_object.value("type").toString() == QString("TPMS"))
    {/* pressure sensors! */
        output = QString("TPMS[%1](%2)>ID:%3,P:%4 Bar,T:%5 C,F:%6")
                .arg(index, 3, 10, QChar('0'))
                .arg(model)
                .arg(json_object.value("id").toVariant().toString())
                .arg(QString::number((json_object.value("pressure_kPa").toDouble()/100), 'f', 2))
                .arg(QString::number(json_object.value("temperature_C").toDouble(), 'f', 2))
                .arg(json_object.value("flags").toVariant().toString());
    } else {
    /* all other devise is there */
        const QStringList exclude_keys = { "id", "model",  "pressure_kPa", "temperature_C", "time" };
        QStringList keys = json_object.keys();

        /* sort this list */
        sort_qstring_list(keys);

        /* add constant part */
        output = QString("TPMS[%1](%2)>ID:%3")
                .arg(index, 3, 10, QChar('0'))
                .arg(model)
                .arg(json_object.value("id").toVariant().toString());

        /* pressure */
        if(0 <= keys.indexOf(QLatin1String("pressure_kPa"), 0)) {
            output.append(QString(",P:%1 Bar").arg(QString::number((json_object.value("pressure_kPa").toDouble()/100), 'f', 2)));
        };

        /* temp */
        if(0 <= keys.indexOf(QLatin1String("temperature_C"), 0)) {
            output.append(QString(",T:%1 C").arg(QString::number(json_object.value("temperature_C").toDouble(), 'f', 2)));
        };

        /////// add other keys //////
        for (QString &key : keys) {
            if(0 <= exclude_keys.indexOf(QLatin1String(key.toLatin1()), 0)) {
                continue;
            };

            QString app = QString(",%1:%2")
                    .arg(key)
                    .arg(json_object.value(key.toLatin1()).toVariant().toString());
            output.append(app);
        };
    }

    return output;
}

void MainWindow::show_main_dialog_info(const quint32 &index, const QJsonObject& json_object)
{
    QString text_doc;

    text_doc.append('\n');
    QStringList keys = json_object.keys();

    /* sort this list */
    sort_qstring_list(keys);

    foreach (QString key, keys) {
        text_doc.append(key);
        text_doc.append(':');
        text_doc.append(json_object.value(key.toLatin1()).toVariant().toString());
        text_doc.append('\n');
    };

    /// notification dialog for the new messages
    QSpacerItem* horizontalSpacer = new QSpacerItem(240, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QMessageBox *message = new QMessageBox(QMessageBox::Icon::NoIcon, tr(""),
        /*test_doc.toJson(QJsonDocument::Compact)*/text_doc, QMessageBox::Button::Ok, this);
    QGridLayout* layout = (QGridLayout*)message->layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    message->setDefaultButton(QMessageBox::Button::Ok);
    message->open();
    connect(message, &QDialog::finished, this, [message] {
        message->deleteLater();
    });
    QTimer *timer = new QTimer(message);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [message] {
        message->accept();
    });
    timer->start(40000);
    /////////////////////////////////////////////////////////////////
}

void MainWindow::slot_main_tab_rtl433ProcessOutput(const QJsonObject& json_object)
{
    recs_statistics.inc_recive_cnt();
    ui->labelStatistics->setText(recs_statistics.get_statiscs_string());

    /* add record to the list */
    QListWidgetItem *_aitem = new QListWidgetItem(fill_item_from_json(
                                    recs_statistics.get_recive_cnt()%1000/*0...999*/, json_object));
    ui->listWidgetMainTable->addItem(_aitem);
    if((recs_statistics.get_recive_cnt()&1) != 0) {
        _aitem->setBackground(QBrush(Qt::gray));
    }

    /* show info dialog */
    show_main_dialog_info(recs_statistics.get_recive_cnt(), json_object);
}




///////////////////// rtl433 TAB /////////////////////

void MainWindow::slot_fillRTL433RawLog(const QString& one_line)
{
    quint32 lines = 0;

    ui->textEditRTL433RawLog->append(one_line);

    /* delete some lines from the text editor if need */
//    while((lines = ui->textEditRTL433RawLog->document()->blockCount()) > 10) {
//        const int lineToDelete = 0;
//        QTextBlock b = ui->textEditRTL433RawLog->document()->findBlockByLineNumber(lineToDelete);
//        if (b.isValid()) {
//            QTextCursor cursor(b);
//            cursor.select(QTextCursor::BlockUnderCursor);
//            cursor.removeSelectedText();
//            cursor.movePosition( QTextCursor::End );
//        }
//    };
}

void MainWindow::on_pushButtonRTL433Ctrl_clicked(bool state)
{
    qDebug() << "on_pushButtonRTL433Ctrl_clicked" << state;
    if (true == state) {
        ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: red");
        ui->doubleSpinBoxRTL433Freq->setEnabled(false);
        ui->pushButtonRTL433Ctrl->setText("Stop");
        ui->textEditRTL433RawLog->clear();
        Q_EMIT p_rtl433->start_rtl433(static_cast<quint32>(ui->doubleSpinBoxRTL433Freq->value()*1000)*1000);
    } else {
        ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: blue");
        ui->pushButtonRTL433Ctrl->setText("Start");
        ui->doubleSpinBoxRTL433Freq->setEnabled(true);
        Q_EMIT p_rtl433->stop_rtl433();
    }
}

///////////////////// Setting section in the TABS /////////////////////

void MainWindow::on_pushButtonReboot_clicked()
{
    ui->pushButtonReboot->setEnabled(false);

    /* reboot this device */
    QColor col = QColor(Qt::red);
    if(col.isValid()) {
       QString qss = QString("background-color: %1").arg(col.name());
       ui->pushButtonReboot->setStyleSheet(qss);
    };

    p_linux->reboot();
}

void MainWindow::on_pushButtonPowerOff_clicked()
{
    ui->pushButtonPowerOff->setEnabled(false);

    /* reboot this device */
    QColor col = QColor(Qt::red);
    if(col.isValid()) {
       QString qss = QString("background-color: %1").arg(col.name());
       ui->pushButtonPowerOff->setStyleSheet(qss);
    };

    p_linux->poweroff();
}

void MainWindow::on_settingsTabSliderBacklight_valueChanged(int value)
{
    const quint8 backlight = (value < 0) ? 0 : (value > 255) ? 255 : value;

    p_linux->set_backlight(backlight);
    sysCfg->getGeneralConfig()->setBacklightValue(backlight);
}


void MainWindow::on_checkBoxEnableDebug_clicked(bool checked)
{
    ui->tabWidgetMain->setTabVisible(EN_DEBUG_TAB_INDEX, checked);
}

bool MainWindow::init_setting_tab() {
    const quint8 backlight = sysCfg->getGeneralConfig()->getBacklightValue();

    ui->settingsTabSliderBacklight->setValue(backlight);

    return true;
}

void MainWindow::on_tabWidgetMain_currentChanged(int index)
{
    if(EN_SETTING_TAB_INDEX == index) {
        init_setting_tab();
    };
}
