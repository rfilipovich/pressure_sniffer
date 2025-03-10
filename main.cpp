#define PRE_PRINT_MESSAGE "MAIN"

#include "mainwindow.h"
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui>
#include <iostream>
#include <QSettings>
#include "rtl_433.h"
#include "SysConfig.h"
#include "log.h"

/* null handel for disabling messages in release mode */
//typedef void (*QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
void myMessageOutput(QtMsgType /*type*/, const QMessageLogContext &/*context*/, const QString &/*msg*/)
{
}

int main(int argc, char *argv[])
{
#if !defined(DEBUG)
    qInstallMessageHandler(myMessageOutput);
#endif

    /* fill application info */
    QCoreApplication::setOrganizationName("LLL");

    QApplication a(argc, argv);

    qRegisterMetaType<rtl_433_supported_protocols>();


    /* open system config */
    SysConfig cfg(QString("%1/%2.conf")
        .arg(config_path)
        .arg(QCoreApplication::applicationName()));
    cfg.init();


    MainWindow w;
    w.show();

    return a.exec();
}
