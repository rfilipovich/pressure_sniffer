#ifndef linux_shell_H
#define linux_shell_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QTimeZone>
#include <QProcess>

class linux_shell : public QObject
{
    Q_OBJECT

    QProcess proc;

public:
    linux_shell(QObject *parent = Q_NULLPTR);
    static bool kill_linux_process(const QString & processName);

    /**
     * @brief setSystemDateTime apply date and time to system time and rtc
     * @param newDateTime new dateTime (date, time, timezone)
     * @return
     */
    static bool setSystemDateTime(const QDateTime &newDateTime);
    static bool setSystemDateTimeFromRtc();

    static bool runCmd(const QString& cmd);

public Q_SLOTS:
    void run();
    void reboot();
    void poweroff();

    quint8 get_backlight();
    bool set_backlight(const quint8 new_backlight);


Q_SIGNALS:
    void logResult(const QStringList &log);

};

#endif // linux_shell_H
