#define PRE_PRINT_MESSAGE "RTL"

#include <QProcess>

#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QStringList>

#include "rtl_433.h"
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


rtl_433::rtl_433(QObject *parent) :
    QObject(parent), rtl433_proc_name("rtl_433")
{

}

void rtl_433::run() {
/* added creating object for this thread */
}


void rtl_433::stop_rtl433()
{
    Q_ASSERT(!rtl433_proc_name.isEmpty());

#if !defined(USE_DEBUGGER)
    QProcess kill_proc;
    QStringList args_list = {rtl433_proc_name};

    kill_proc.start(QString("killall"), args_list);
    kill_proc.waitForStarted();
    kill_proc.waitForFinished();
#endif

    disconnect(&proc, &QProcess::readyReadStandardOutput, this, &rtl_433::processOutput);
    disconnect(&proc, &QProcess::readyReadStandardError, this, &rtl_433::processErrorOutput);

    LOG(LOG_INFO, QString("Stopped %1 successful.").arg(rtl433_proc_name));
    Q_EMIT rtl433Finished();
}

void rtl_433::start_rtl433(const quint32 freq_hz, const QStringList &prot_list) {
    QString cmd = "%1 -f %2 -F json %3";
    QString arg_prot_list;

//    QProcess proc;
    /* apply args */
    cmd = cmd.arg(rtl433_proc_name).arg(freq_hz).arg(arg_prot_list);

    connect(&proc, &QProcess::readyReadStandardOutput, this, &rtl_433::processOutput);
    connect(&proc, &QProcess::readyReadStandardError, this, &rtl_433::processErrorOutput);

    proc.start(cmd);
    proc.waitForStarted();

    /* get return value */
    if(proc.exitCode() != 0) {
        LOG(LOG_ERR, QString("Error while running %1: %2; exitCode: %3").arg(cmd).arg(QString(proc.readAllStandardError())).arg(proc.exitCode()));
        Q_EMIT rtl433Error();
        return;
    };

    return;
}

void rtl_433::processOutput() {
    QString res;
    QString one_raw_line = proc.readAllStandardOutput();

    /* send raw for the rtl433 dbg */
    Q_EMIT rtl433ProcessRawOutput(one_raw_line);

    //QString percent = proc.readAllStandardError();
    one_raw_line.remove(QRegExp("[# %]"));
    QStringList percentList = one_raw_line.split('\r', QString::SkipEmptyParts);
    if (!percentList.isEmpty()) {
        res = percentList.last();
    } else {
        res = QString();
    }
    //qDebug() << percent;
    Q_EMIT rtl433ProcessOutput(res);
}

void rtl_433::processErrorOutput() {
    QString res;
    QString one_raw_line = proc.readAllStandardError();

    /* send raw for the rtl433 dbg */
    Q_EMIT rtl433ProcessRawOutput(one_raw_line);
}
