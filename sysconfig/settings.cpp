#include "settings.h"
#include <QSettings>
#include <QMetaEnum>
#include <QRegExp>
#include <QStringList>
#include <QMutexLocker>

Settings::Settings(const QString &filename) {
    const QMetaObject &mo = staticMetaObject;
    int idx = mo.indexOfEnumerator("Key");
    keys = mo.enumerator(idx);

    idx = mo.indexOfEnumerator("Section");
    sections = mo.enumerator(idx);

    conf = new QSettings(filename, QSettings::NativeFormat);
    Q_ASSERT(conf);
}

Settings::~Settings()
{
    conf->sync();
    delete(conf);
}

QVariant Settings::get(Key k, Section s) {
    QMutexLocker locker(&m_mutex);

    QString key = keyPath(s, k);
    return conf->value(key, defaults[key]);
}

QVariant Settings::resetKeyToDefault(const Key & k, const Section & s) {
    const QString key = keyPath(s, k);

    ValueRef(*this, keyPath(s, k)) = defaults[key];

    return (conf->value(key, defaults[key]));
}


Settings::ValueRef Settings::set(Key k, Section s) {
    QMutexLocker locker(&m_mutex);

    return ValueRef(*this, keyPath(s, k));
}

void Settings::setDefaults(const QString &str){

    QMutexLocker locker(&m_mutex);

    //section/key : value
    //section - optional
    QRegExp rxRecord("^\\s*(((\\w+)/)?(\\w+))\\s*:\\s*([^\\s].{0,})\\b\\s*$");

    auto kvs = str.split(QRegExp(";\\W*"), QString::SkipEmptyParts); //key-values
    for(auto kv : kvs){
        if(rxRecord.indexIn(kv) != -1){
            QString section = rxRecord.cap(3);
            QString key = rxRecord.cap(4);
            QString value = rxRecord.cap(5);

            int iKey = keys.keyToValue(key.toLocal8Bit().data());
            if(iKey != -1){
                int iSection = sections.keyToValue(section.toLocal8Bit().data());
                if(section.isEmpty() || iSection != -1){
                    defaults[rxRecord.cap(1)] = value;
                }
            }
        }
    }
}

//Settings::ValueRef-----------------------------------------------------------
Settings::ValueRef & Settings::ValueRef::operator = (const QVariant &data) {
    parent.conf->setValue(keyPath, data);
    return *this;
}

QString Settings::sectionToString(Section s) {
    const QMetaObject &mo = staticMetaObject;
    int idx = mo.indexOfEnumerator("Section");
    return QString(mo.enumerator(idx).valueToKey(s));
}

QString Settings::keyToString(Key k) {
    const QMetaObject &mo = staticMetaObject;
    int idx = mo.indexOfEnumerator("Key");
    return QString(mo.enumerator(idx).valueToKey(k));
}

//PRIVATE METHODS--------------------------------------------------------------
QString Settings::keyPath(Section s, Key k){
    auto szSection = sections.valueToKey(s);
    auto szKey = keys.valueToKey(k);
    return ((General == s) ?
        QString("%1").arg(szKey) :
        QString("%2/%1").arg(szKey).arg(szSection));
}
