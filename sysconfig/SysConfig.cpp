#include <QString>
#include <QFile>

#include "SysConfig.h"
#include "generalconfig.h"
#include "settings.h"

SysConfig::SysConfig(const QString &filename, QObject *parent) :
        QObject(parent), st(Q_NULLPTR)

{
    Q_ASSERT(!filename.isEmpty());

    cfgFile = filename;

    generalConfig = new GeneralConfig(this);
    Q_ASSERT(generalConfig);

     st = new Settings(filename);
    Q_ASSERT(st);
}

SysConfig::~SysConfig()
{
    if(Q_NULLPTR != st) {
        delete st;
        st = NULL;
    }
}

bool SysConfig::init(void)
{
    /* Apply default config */
    QFile cfgDefaults(":/configs/default.cfg");
    cfgDefaults.open(QIODevice::ReadOnly);
    st->setDefaults(cfgDefaults.readAll());

    return true;
}

GeneralConfig *SysConfig::getGeneralConfig() {
    return generalConfig;
}

void SysConfig::sync() {
    st->conf->sync();
}
