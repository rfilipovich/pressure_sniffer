#define PRE_PRINT_MESSAGE "RTL"

#include <QProcess>

#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QStringList>
#include <QRegularExpression>

#include "rtl_433.h"
#include <QtCore/QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

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
    _is_started = false;
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

    _is_started = false;

    LOG(LOG_INFO, QString("Stopped %1 successful.").arg(rtl433_proc_name));
    Q_EMIT rtl433Finished();
}

void rtl_433::start_rtl433(const quint32 freq_hz, const QList<quint16> &prot_list) {
    QString cmd = "%1 -f %2 -C si -F json %3";
    QString arg_prot_list = QString("");

    if(prot_list.count() > 0 && (prot_list.at(0) != 0)) {
        /* need add filtartion for the protocols */
        foreach (quint16 var, prot_list) {
            arg_prot_list.append(QString(" -R %1").arg(var));
        };
    }

    /* apply args */
    cmd = cmd.arg(rtl433_proc_name).arg(freq_hz).arg(arg_prot_list);

    connect(&proc, &QProcess::readyReadStandardOutput, this, &rtl_433::processOutput);
    connect(&proc, &QProcess::readyReadStandardError, this, &rtl_433::processErrorOutput);

    proc.start(cmd);
    proc.waitForStarted();

    /* get return value */
    if(proc.exitCode() != 0) {
        disconnect(&proc, &QProcess::readyReadStandardOutput, this, &rtl_433::processOutput);
        disconnect(&proc, &QProcess::readyReadStandardError, this, &rtl_433::processErrorOutput);

        QString error_code = QString("Error running %1: %2; exitCode: %3")
                                .arg(cmd).arg(QString(proc.readAllStandardError())).arg(proc.exitCode());
        LOG(LOG_ERR, error_code);
        Q_EMIT rtl433Error(error_code);
        Q_EMIT rtl433Finished();
        return;
    };

    _is_started = true;

    return;
}

void rtl_433::get_supported_protocols_rtl433(QList<rtl_433_supported_protocols> &supp_proto) {
    /* checking cache first */
    if(proto_supp_cache.length() >= 1) {
        supp_proto = proto_supp_cache;
        return;
    };

    QString cmd = "%1 -R";

    /* apply args */
    cmd = cmd.arg(rtl433_proc_name);

    proc.start(cmd);
    proc.waitForStarted();
    proc.waitForFinished(5000);

    /* get return value */
    if(proc.exitCode() != 0) {
        QString error_code = QString("Error running %1: %2; exitCode: %3")
                                .arg(cmd).arg(QString(proc.readAllStandardError())).arg(proc.exitCode());
        LOG(LOG_ERR, error_code);
        return;
    };

    /* parse output list and return */
    QString data = proc.readAllStandardError();
    QStringList data_list = data.split('\n');

    const QString regex_pattern = "\\[*\\]";
    QRegularExpression re(regex_pattern, QRegularExpression::CaseInsensitiveOption);
    re.setPatternOptions(QRegularExpression::MultilineOption);
    if(re.isValid())
    {
        for(const QString& input : data_list){
            QRegularExpressionMatchIterator it = re.globalMatch(input);
            if(it.hasNext()) {
                rtl_433_supported_protocols sup_proto_rec;
                bool is_disabled = false;

                QStringList temporary_list = input.split("  ", QString::SkipEmptyParts);
                if(temporary_list.length() < 2) {
                    is_disabled = true;
                    temporary_list = input.split("* ", QString::SkipEmptyParts);
                }
                if(temporary_list.length() == 2 ) {
                    QStringList temporary_list2 = temporary_list.at(0).split("  ", QString::SkipEmptyParts);
                    QStringList _proto_id = temporary_list2.at(0).split("[",  QString::SkipEmptyParts).at(0).split("]", QString::SkipEmptyParts);
                    if(_proto_id.length() >= 1) {
                        sup_proto_rec = rtl_433_supported_protocols(this, _proto_id.at(0).toInt(), QString(temporary_list.at(1)), is_disabled);

                        //QRegularExpressionMatch match = it.next();
                        //QStringList word = match.capturedTexts();// captured(1);
                        supp_proto.append(sup_proto_rec);
                    };
                }
            }
        };

        /* save to cache if need */
        if(supp_proto.length() >= 1) {
            proto_supp_cache = supp_proto;
        }

    };

    return;
}

void rtl_433::processOutput() {
    QString res;


    /** need add windows buffer because line can be without \n, need summ all to \n **/
    QString one_raw_line = proc.readAllStandardOutput();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(one_raw_line.toLatin1(), &error);
    if(!doc.isNull()) {
        QJsonObject jsonRootObj = doc.object();
        Q_EMIT rtl433ProcessOutput(jsonRootObj);
    } else {
        qDebug() << "parse JSON -> failed: " << one_raw_line;
    };

    /* send raw for the rtl433 dbg */
    Q_EMIT rtl433ProcessRawOutput(one_raw_line);
}

void rtl_433::processErrorOutput() {
    QString res;
    QString one_raw_line = proc.readAllStandardError();

    /* send raw for the rtl433 dbg */
    Q_EMIT rtl433ProcessRawOutput(one_raw_line);
}
