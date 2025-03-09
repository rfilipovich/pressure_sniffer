#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QTextBlock>

/* some defines */
#define MAIN_TAB_CURENT_INDEX (1)


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


bool MainWindow::initAll()
{
    //!    connect(client, &ClientSideEmulation::onFinishRead, this, &MainWindow::slot_fillDbgLog);

/* preinit forms if need */
    ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: blue");

/* init class */
    p_rtl433 = new rtl_433(this);

    /* sometimes need stop firstly the rtl_433 */
    Q_EMIT p_rtl433->stop_rtl433();

/* connetion slots for the rtl_433 */
    connect(p_rtl433, &rtl_433::rtl433ProcessRawOutput, this, &MainWindow::slot_fillRTL433RawLog);
 //....

/* setting the rtl433 form */
    QScrollBar *textEditRTL433RawLogBar = new QScrollBar();
    textEditRTL433RawLogBar->setStyleSheet("QScrollBar:vertical { width: 30px; }");
    ui->textEditRTL433RawLog->setVerticalScrollBar(textEditRTL433RawLogBar);

    return true;
}

void MainWindow::on_pushButtonPlus_clicked()
{
    plus_minus_cnt += 10;
    if(plus_minus_cnt >100) plus_minus_cnt = 100;
    ui->progressBar->setValue(plus_minus_cnt);
}

void MainWindow::on_pushButtonMinus_clicked()
{
    plus_minus_cnt -= 10;
    if(plus_minus_cnt < 0) plus_minus_cnt = 0;
    ui->progressBar->setValue(plus_minus_cnt);
}

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
        ui->textEditRTL433RawLog->clear();
        Q_EMIT p_rtl433->start_rtl433(static_cast<quint32>(ui->doubleSpinBoxRTL433Freq->value()*1000)*1000, QStringList());
    } else {
        ui->pushButtonRTL433Ctrl->setStyleSheet("background-color: blue");
        ui->doubleSpinBoxRTL433Freq->setEnabled(true);
        Q_EMIT p_rtl433->stop_rtl433();
    }
}
