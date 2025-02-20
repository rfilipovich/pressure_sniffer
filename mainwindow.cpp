#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    i =0 ;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonPlus_clicked()
{
    i+=10;
    if(i >100) i =100;
    ui->progressBar->setValue(i);
}

void MainWindow::on_pushButtonMinus_clicked()
{
    i-=10;
    if(i < 0) i = 0;
    ui->progressBar->setValue(i);
}
