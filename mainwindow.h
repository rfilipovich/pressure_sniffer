#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <rtl_433.h>
#include "freq_change_form.h"
#include "startdialog.h"
#include "linux_shell.h"

class SysConfig;

enum {
    EN_MAIN_TAB_INDEX   =0,
    EN_SETTING_TAB_INDEX=2,
    EN_DEBUG_TAB_INDEX  =3,
};

/* some defines */
#define MAIN_TAB_CURENT_INDEX (2)
//!#define MAIN_TAB_CURENT_INDEX ( EN_MAIN_TAB_INDEX)

namespace Ui {
class MainWindow;
}


class statistics_t {
    int recive_recs;
    int unical_recs;

public:
    explicit statistics_t() {
        reset();
    }

    void reset(void) {
        recive_recs = 0;
        unical_recs = 0;
    }

    inline void inc_recive_cnt() {
        recive_recs++;
    }

    inline int get_recive_cnt() {
        return recive_recs;
    }

    inline void inc_unical_cnt() {
        unical_recs++;
    }

    QString get_statiscs_string() {
        QString out("all: %1, unical: %2");
        return out.arg(recive_recs).arg(unical_recs);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /* not use default constructor */
    MainWindow(QWidget *parent = Q_NULLPTR) :
        QMainWindow(parent) {
        Q_ASSERT(true);
    };

public:
    MainWindow(SysConfig *sysCfg, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private slots:
    void on_pushButtonMainStart_clicked();

/* main TAB */
    void slot_tmain_start_dialog_apply(const QList<quint16> &proto_id_list, const quint32 & freq);
    void slot_tmain_start_dialog_break(void);
    void slot_main_tab_rtl433ProcessOutput(const QJsonObject& json_object);
    void slot_main_tab_rtl433Finished();

/* rtl433 TAB */
    void on_pushButtonRTL433Ctrl_clicked(bool state);
    void slot_fillRTL433RawLog(const QString& one_line);

/* DEBUG TAB */
    void on_pushButtonReboot_clicked();

    void on_pushButtonPowerOff_clicked();

    void on_settingsTabSliderBacklight_valueChanged(int value);

    void on_checkBoxEnableDebug_clicked(bool checked);

    void on_tabWidgetMain_currentChanged(int index);

private:
// objects
    rtl_433 *p_rtl433;
    linux_shell *p_linux;
    Ui::MainWindow *ui;
    SysConfig *sysCfg;

    freq_change_form *p_freq_form;
    StartDialog *p_StartDialog;

    statistics_t recs_statistics;

/* functions */
    /* init all forms */
    bool initAllSubForms(QWidget *parent = NULL);
    bool initAll();

    /* corrret fill  */
    QString fill_item_from_json(const quint32 &index, const QJsonObject& json_object);
    void show_main_dialog_info(const quint32 &index, const QJsonObject& json_object);

// this function calling when you open the tab
    bool init_setting_tab();
};

#endif // MAINWINDOW_H
