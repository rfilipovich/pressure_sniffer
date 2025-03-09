#ifndef RTL_433_H
#define RTL_433_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QProcess>

class rtl_433 : public QObject
{
    Q_OBJECT

    QProcess proc;
    QString rtl433_proc_name;

public:
    rtl_433(QObject *parent = Q_NULLPTR);

public Q_SLOTS:
    void run();
    void start_rtl433(const quint32 freq_hz, const QStringList &prot_list);
	void stop_rtl433();

private Q_SLOTS:
    void processOutput();
    void processErrorOutput();

Q_SIGNALS:
    void rtl433Error();
    void rtl433Finished();
    void rtl433ProcessOutput(const QString& json_line);
    void rtl433ProcessRawOutput(const QString& one_line);
};

#endif // RTL_433_H
