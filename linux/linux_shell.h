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

    /**
     * @brief isDBFSmounted
     * @return return true if DBFS mounted fine
     */
    static bool isDBFSmounted(void);

public Q_SLOTS:
    void downloadCmd(const QString& cmd);
    void processOutput();
    void run();
    void reboot();

Q_SIGNALS:
    void logResult(const QStringList &log);
    void downloadError();
    void downloadFinished();
    void downloadProcessOutput(const QString& percent);
    void decompressOK();
    void decompressFAIL();

};

#endif // linux_shell_H
