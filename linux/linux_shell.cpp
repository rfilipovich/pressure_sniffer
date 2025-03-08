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



void linux_shell::reboot()
{
#if !defined(USE_DEBUGGER)
    QProcess proc;
    proc.start(QString("reboot"));
    proc.waitForStarted();
    proc.waitForFinished();
#endif
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

void linux_shell::downloadCmd(const QString& cmd) {
//    QProcess proc;
    //connect(&proc, &QProcess::readyReadStandardOutput, this, &linux_shell::processOutput);
    connect(&proc, &QProcess::readyReadStandardError, this, &linux_shell::processOutput);
    proc.start(cmd);
    proc.waitForStarted();
    proc.waitForFinished(30*60*1000);
    //disconnect(&proc, &QProcess::readyReadStandardOutput, this, &linux_shell::processOutput);
    disconnect(&proc, &QProcess::readyReadStandardError, this, &linux_shell::processOutput);
    /* get return value */
    if(proc.exitCode() != 0) {
        LOG(LOG_ERR, QString("Error while running %1: %2; exitCode: %3").arg(cmd).arg(QString(proc.readAllStandardError())).arg(proc.exitCode()));
        Q_EMIT downloadError();
        //wt->downloadError(QNetworkReply::NetworkError::UnknownNetworkError);
        return;
    }
    LOG(LOG_INFO, QString("Running %1 successful.").arg(cmd));
    Q_EMIT downloadFinished();
    //wt->downloadFinished();
    return;
}

void linux_shell::processOutput() {
    QString res;
    //QString percent = proc.readAllStandardOutput();
    QString percent = proc.readAllStandardError();
    percent.remove(QRegExp("[# %]"));
    QStringList percentList = percent.split('\r', QString::SkipEmptyParts);
    if (!percentList.isEmpty()) {
        res = percentList.last();
    } else {
        res = QString();
    }
    //qDebug() << percent;
    Q_EMIT downloadProcessOutput(res);
}

