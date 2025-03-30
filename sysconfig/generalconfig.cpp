
#include "SysConfig.h"
#include "settings.h"
#include "generalconfig.h"

GeneralConfig::GeneralConfig(QObject *parent) :
        QObject(parent), sysConfig(Q_NULLPTR)
{
    sysConfig = qobject_cast<SysConfig *>(parent);
    Q_ASSERT(sysConfig);
}


/**
 * @brief GeneralConfig::getMainState
 * @return
 */
quint8 GeneralConfig::getBacklightValue() const
{
    return static_cast<quint8>(sysConfig->st->get(Settings::Backlight, Settings::General).toUInt());
}

void GeneralConfig::setBacklightValue(quint8 value)
{
    sysConfig->st->set(Settings::Backlight, Settings::General) = value;
}
