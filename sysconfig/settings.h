#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>
#include <QSettings>
#include <QMetaEnum>
#include <QMutex>

class Settings {
    Q_GADGET
    Q_ENUMS(Section)
    Q_ENUMS(Key)

public:

    QSettings *conf;

    enum Section {
        General,
    };

    enum Key {
        Id,
        MainState,
    };

    Settings(const QString &filename);
    virtual ~Settings();

    class ValueRef {
    public:
        ValueRef(Settings &st, const QString &kp) :
            parent(st), keyPath(kp){}
        ValueRef & operator = (const QVariant &d);
    private:
        Settings &parent;
        const QString keyPath;
    };

    void setDefaults(const QString &str);
    QVariant get(Key, Section /*s*/ = General);
    ValueRef set(Key, Section /*s*/ = General);
    QVariant resetKeyToDefault(const Key & k, const Section & s = General);

    static QString sectionToString(Section);
    static QString keyToString(Key);

private:
    QString keyPath(Section, Key);

    QMetaEnum keys;
    QMetaEnum sections;
    QMap<QString, QVariant> defaults;
    //QSettings *conf;
    QMutex	m_mutex;

    Settings();
    Settings(const Settings &);
    Settings & operator = (const Settings &);
};

#endif // SETTINGS_H
