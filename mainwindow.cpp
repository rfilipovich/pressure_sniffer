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


//// addons
#define ARRAY_SIZE(a)                               \
  ((sizeof(a) / sizeof(*(a))) /                     \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


/* main class */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), p_rtl433(NULL),
    ui(new Ui::MainWindow)
{
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
    p_freq_form = new freq_change_form(parent);
    p_freq_form->hide();

    p_StartDialog = new StartDialog(parent);

    return true;
}

bool MainWindow::initAll()
{
    //!    connect(client, &ClientSideEmulation::onFinishRead, this, &MainWindow::slot_fillDbgLog);

    initAllSubForms(this);

/* preinit forms if need */
    ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: blue");
    ui->pushButtonMainStart->setStyleSheet("background-color: blue");

/* init class */
    p_rtl433 = new rtl_433(this);

    /* sometimes need stop firstly the rtl_433 */
    Q_EMIT p_rtl433->stop_rtl433();

/* connetion slots for the rtl_433 */
    connect(p_rtl433, &rtl_433::signal_rtl433ProcessRawOutput, this, &MainWindow::slot_fillRTL433RawLog);
 //....

/* setting the rtl433 form */
    QScrollBar *textEditRTL433RawLogBar = new QScrollBar();
    textEditRTL433RawLogBar->setStyleSheet("QScrollBar:vertical { width: 30px; } ");

    //
    ui->textEditRTL433RawLog->setVerticalScrollBar(textEditRTL433RawLogBar);

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

QString MainWindow::fill_item_from_json(const quint32 &index, const QJsonObject& json_object)
{
    QString output;

    QString type;
    if(json_object.value("type").toString() == QString("TPMS"))
    {/* pressure sensors! */
        QString model = json_object.value("model").toString();
        if(model.length() > 10) {
        /* delete midle for mte string, len must be maximum 10 chars */
            model = model.left(5) + QString('.') + model.right(5);
        }
        output = QString("TPMS[%1](%2)>ID:%3,P:%4 Bar,T:%5 C")
                .arg(index, 3, 10, QChar('0'))
                .arg(model)
                .arg(json_object.value("id").toString())
                .arg(QString::number((json_object.value("pressure_kPa").toDouble()/100), 'f', 2))
                .arg(QString::number(json_object.value("temperature_C").toDouble(), 'f', 2))
                .arg(json_object.value("flags").toString());
    } else {
    /* all other devise is there */
        //TODO:!!!!

    }

    return output;
}

void MainWindow::show_main_dialog_info(const quint32 &index, const QJsonObject& json_object)
{
    /////////////////////////////////////////////////////////////////
    ////// test varian printing the recived value to the dialog //////
    /// test
    QJsonDocument test_doc(json_object);

    /// notification dialog for the new messages
    QMessageBox *message = new QMessageBox(QMessageBox::Icon::NoIcon, tr(""),
        test_doc.toJson(QJsonDocument::Compact), QMessageBox::Button::Ok, this);
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

///////////////////// Debug section in the TABS /////////////////////


void MainWindow::on_testPushButton_pressed()
{
    QList<rtl_433_supported_protocols> list;
    p_rtl433->get_supported_protocols_rtl433(list);

    qDebug() << list << Qt::endl;
}

void MainWindow::on_testPushButton_2_pressed()
{
p_freq_form->show();
}

//!testPushButton_2

