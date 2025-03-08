#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButtonPlus_clicked();

    void on_pushButtonMinus_clicked();
    void slot_fillDbgLog(QByteArray &data);

private:
    Ui::MainWindow *ui;
    int i;
};

#endif // MAINWINDOW_H
