#ifndef GENERALCONFIG_H
#define GENERALCONFIG_H

#include <QObject>
#include "data_structs.h"


class SysConfig;
class GeneralConfig : public QObject {
    Q_OBJECT

public:
    GeneralConfig(QObject *parent);

    MainStateConfigVar::State getMainState() const;
    void setMainState(MainStateConfigVar::State next_state);

private:
    SysConfig *sysConfig;

};

#endif // GENERALCONFIG_H
