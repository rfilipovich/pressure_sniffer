#include <QMovie>
#include "waitwindow.h"
#include "ui_waitwindow.h"

WaitWindow::WaitWindow(QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint),
    ui(new Ui::WaitWindow)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/main/one/waiting-wheel-300x300.gif");
    ui->processLabel->setMovie(movie);
    movie->start();
}

WaitWindow::~WaitWindow()
{
    delete ui;
}
