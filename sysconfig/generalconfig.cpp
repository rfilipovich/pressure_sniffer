
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
MainStateConfigVar::State GeneralConfig::getMainState() const
{
    return static_cast<MainStateConfigVar::State>(sysConfig->st->get(Settings::MainState, Settings::General).toInt());
}

void GeneralConfig::setMainState(MainStateConfigVar::State next_state)
{
    sysConfig->st->set(Settings::MainState, Settings::General) = next_state;
}
