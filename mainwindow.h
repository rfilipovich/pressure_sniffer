#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <rtl_433.h>

/* some defines */
//!#define MAIN_TAB_CURENT_INDEX (1)

#define MAIN_TAB_CURENT_INDEX (3)

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
// test only
    void on_pushButtonPlus_clicked();
    void on_pushButtonMinus_clicked();

/* rtl433 TAB */
    void on_pushButtonRTL433Ctrl_clicked(bool state);
    void slot_fillRTL433RawLog(const QString& one_line);

/* DEBUG TAB */
    void on_testPushButton_pressed();

private:
// objets
    rtl_433 *p_rtl433;
    Ui::MainWindow *ui;

/* functions */
    bool initAll();

//test onlys
    int plus_minus_cnt;
};

#endif // MAINWINDOW_H
