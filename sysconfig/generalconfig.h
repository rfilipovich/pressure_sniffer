#ifndef GENERALCONFIG_H
#define GENERALCONFIG_H

#include <QObject>
#include "data_structs.h"


class SysConfig;
class GeneralConfig : public QObject {
    Q_OBJECT

public:
    GeneralConfig(QObject *parent);

    quint8 getBacklightValue() const;
    void setBacklightValue(quint8 value);

private:
    SysConfig *sysConfig;

};

#endif // GENERALCONFIG_H
