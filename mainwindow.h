#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <rtl_433.h>
#include "freq_change_form.h"
#include "startdialog.h"

/* some defines */
#define MAIN_TAB_CURENT_INDEX (1)
//#define MAIN_TAB_CURENT_INDEX (0)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonMainStart_clicked();

/* main TAB */
    void slot_tmain_start_dialog_apply(const QList<rtl_433_supported_protocols> &proto, const quint32 & freq);
    void slot_tmain_start_dialog_break(void);

/* rtl433 TAB */
    void on_pushButtonRTL433Ctrl_clicked(bool state);
    void slot_fillRTL433RawLog(const QString& one_line);

/* DEBUG TAB */
    void on_testPushButton_pressed();
    void on_testPushButton_2_pressed();

private:
// objects
    rtl_433 *p_rtl433;
    Ui::MainWindow *ui;

    freq_change_form *p_freq_form;
    StartDialog *p_StartDialog;

/* functions */
    /* init all forms */
    bool initAllSubForms(QWidget *parent = NULL);
    bool initAll();

//test onlys
    int plus_minus_cnt;
};

#endif // MAINWINDOW_H
