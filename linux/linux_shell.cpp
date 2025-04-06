#define PRE_PRINT_MESSAGE "COM"

#include <QProcess>

#include <QFile>
#include <QTextStream>
#include <QThread>

#include "linux_shell.h"
#include <QtCore/QCoreApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#include "log.h"


/*
 * uncomment this option fix problem GDB via QProcess
 * in debugging time
*/
//#define USE_DEBUGGER


linux_shell::linux_shell(QObject *parent) :
    QObject(parent)
{

}

void linux_shell::run() {
/* added creating object for this thread */
}


bool linux_shell::kill_linux_process(const QString & processName)
{
    Q_ASSERT(!processName.isEmpty());
#if !defined(USE_DEBUGGER)
    QProcess proc;
    proc.start(QString("killall %1").arg(processName));
    proc.waitForStarted();
    proc.waitForFinished();
#endif
    return true;
}

bool linux_shell::setSystemDateTime(const QDateTime &newDateTime)
{
    QProcess proc;

    /* apply time to system */
    QString cmd_line = QString("date -s \"%1\"")
            .arg(newDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    system(cmd_line.toStdString().c_str());

    /* apply time to rtc */
    proc.start(QString("/sbin/hwclock -wu"));
    proc.waitForStarted();
    proc.waitForFinished();

    return (0 == proc.exitCode());
}

bool linux_shell::setSystemDateTimeFromRtc()
{
    QProcess proc;

    /* apply time to sytem from rtc */
    proc.start(QString("/sbin/hwclock -su"));
    proc.waitForStarted();
    proc.waitForFinished();

    return (0 == proc.exitCode());
}

bool linux_shell::runCmd(const QString& cmd) {
    QProcess proc;
    proc.start(cmd);
    proc.waitForStarted();
    proc.waitForFinished();
    /* get return value */
    if(proc.exitCode() != 0) {
        LOG(LOG_ERR, QString("Error while running %1: %2").arg(cmd).arg(QString(proc.readAllStandardError())));
        return false;
    }
    LOG(LOG_INFO, QString("Running %1 successful.").arg(cmd));
    return true;
}

void linux_shell::reboot()
{
    const QString _cmd = "reboot";
    system(_cmd.toStdString().c_str());
}

void linux_shell::poweroff()
{
    const QString _cmd = "poweroff";
    system(_cmd.toStdString().c_str());
}


const char *const _path_brigness = "/sys/class/backlight/backlight/brightness";

quint8 linux_shell::get_backlight()
{
    QProcess proc;
    const QString _cmd = "cat";
    const QStringList _args = { _path_brigness };

    proc.start(_cmd, _args);
    proc.waitForStarted();
    proc.waitForFinished();

    /* get return value */
    if(proc.exitCode() != 0) {
        LOG(LOG_ERR, QString("Error while running %1: %2")
            .arg(QString(__func__))
            .arg(QString(proc.readAllStandardError())));
        return 0;
    };

    bool is_true = false;
    quint8 baclight_value = proc.readAllStandardOutput().toUShort(&is_true, 10);

    if(false == is_true) {
        return 0;
    }

    return baclight_value;
}

bool linux_shell::set_backlight(const quint8 new_backlight)
{
    QProcess proc;
    int ret = 0;

    const QString _cmd = QString("echo -n %1 > %2")
            .arg(QString::number(new_backlight, 10))
            .arg(_path_brigness);

    ret = system(_cmd.toStdString().c_str());

    return (ret == 0);
}


const char *const _path_battery_adc_raw = "/sys/bus/iio/devices/iio\:device0/in_voltage1_raw";
const char *const _path_battery_voltage_scale = "/sys/bus/iio/devices/iio\:device0/in_voltage_scale";

/**
 * @brief linux_shell::get_battery_level
 * @return 0..100 current battery level
 */
quint8 linux_shell::get_battery_level()
{
    QProcess proc;
    const QString _cmd = "cat";
    quint16 adc_value = 0;
    double scaller_value = 0.0;

    {
        const QStringList _args = { _path_battery_adc_raw };

        proc.start(_cmd, _args);
        proc.waitForStarted();
        proc.waitForFinished();

        /* get return value */
        if(proc.exitCode() != 0) {
            LOG(LOG_ERR, QString("Error while running %1: %2")
                .arg(QString(__func__))
                .arg(QString(proc.readAllStandardError())));
            return 0;
        };

        bool is_true = false;
        adc_value = proc.readAllStandardOutput().toUShort(&is_true, 10);
        if(false == is_true) {
            return 0;
        };
    }

    {/* get voltage scaller value */
        const QStringList _args = { _path_battery_voltage_scale };

        proc.start(_cmd, _args);
        proc.waitForStarted();
        proc.waitForFinished();

        /* get return value */
        if(proc.exitCode() != 0) {
            LOG(LOG_ERR, QString("Error while running %1: %2")
                .arg(QString(__func__))
                .arg(QString(proc.readAllStandardError())));
            return 0;
        };

        bool is_true = false;
        scaller_value = proc.readAllStandardOutput().toDouble(&is_true);
        if(false == is_true) {
            return 0;
        }
    };

    /* calculte battery level */
    double battery_value = scaller_value * adc_value * ((3000.0+6200.0)/3000.0);

    quint8 percent = ((battery_value - 3000.0)/(4100.0 - 3000.0))*100;
    if(battery_value <= 3000.0)
        percent = 0;
    if(battery_value >= 4100.0)
       percent = 100;

    return  percent;
}

