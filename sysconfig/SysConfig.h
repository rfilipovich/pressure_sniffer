#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <QObject>
#include <QString>
#include <QMutex>


#include "generalconfig.h"

/* path for config */
const QString config_path("/userdata/");

class Settings;

class SysConfig : public QObject {

    Q_OBJECT

/* not used default constructor */
    SysConfig(QObject *parent = Q_NULLPTR) :
        QObject(parent)
        {Q_ASSERT(true); }


    friend class GeneralConfig;

public:
    SysConfig(const QString &filename, QObject *parent = Q_NULLPTR);
    virtual ~SysConfig();

    bool init();
    void sync();

    GeneralConfig *getGeneralConfig();

    Settings *st;
    QMutex network_mutex;


private:
    //Settings *st;

    GeneralConfig *generalConfig;

    QString cfgFile;
};

#endif /* SYSCONFIG_H */
