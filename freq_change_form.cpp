#include "freq_change_form.h"
#include "ui_freq_change_form.h"

freq_change_form::freq_change_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::freq_change_form)
{
    ui->setupUi(this);
}

freq_change_form::~freq_change_form()
{
    delete ui;
}
