#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <rtl_433.h>

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

/* rtl433 debug functionality */
    void on_pushButtonRTL433Ctrl_clicked(bool state);
    void slot_fillRTL433RawLog(const QString& one_line);

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
