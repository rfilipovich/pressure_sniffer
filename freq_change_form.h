#ifndef FREQ_CHANGE_FORM_H
#define FREQ_CHANGE_FORM_H

#include <QDialog>

namespace Ui {
class freq_change_form;
}

class freq_change_form : public QDialog
{
    Q_OBJECT

public:
    explicit freq_change_form(QWidget *parent = 0);
    ~freq_change_form();

private:
    Ui::freq_change_form *ui;
};

#endif // FREQ_CHANGE_FORM_H
