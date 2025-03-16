#ifndef RTL_433_H
#define RTL_433_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStringList>
#include <QProcess>
#include <QDebug>

class rtl_433_supported_protocols : public QObject
{
    Q_OBJECT

public:
    rtl_433_supported_protocols(QObject *parent = Q_NULLPTR) :
            QObject(parent), proto_name(""),
            proto_id(0), disabled(true)
    {}

    rtl_433_supported_protocols(const rtl_433_supported_protocols &obj) :
        QObject(Q_NULLPTR)
    {
        this->proto_name = obj.proto_name;
        this->proto_id = obj.proto_id;
        this->disabled = obj.disabled;
    }

    rtl_433_supported_protocols& operator=(const rtl_433_supported_protocols& right) {
        if (this == &right) {
            return *this;
        }

        proto_name = right.proto_name;
        proto_id = right.proto_id;
        disabled = right.disabled;

        return *this;
    }

    rtl_433_supported_protocols(QObject *parent, const quint16 &proto_id, const  QString &proto_name, const bool disabled) :
        QObject(parent)
    {
        this->proto_id = proto_id;
        this->proto_name = proto_name;
        this->disabled = disabled;
    }

    virtual ~rtl_433_supported_protocols()
    {}

/* metods */
    QString get_proto_name() const {
        return proto_name;
    }
    quint16 get_proto_id() const {
        return proto_id;
    }
    bool is_disabled() const {
        return disabled;
    }

    friend QDebug operator<<(QDebug dbg, const rtl_433_supported_protocols &one_rec)
    {
        dbg.nospace() << "[proto_id]=" << one_rec.get_proto_id();
        return dbg.maybeSpace();
    }

protected:
    QString proto_name;
    quint16 proto_id;
    bool disabled;
};

Q_DECLARE_METATYPE(rtl_433_supported_protocols)



class rtl_433 : public QObject
{
    Q_OBJECT

    QProcess proc;
    QString rtl433_proc_name;
    bool _is_started;

/* will cache this list */
    QList<rtl_433_supported_protocols> proto_supp_cache;

public:
    rtl_433(QObject *parent = Q_NULLPTR);
    bool is_started(void) {
        return _is_started;
    }

public Q_SLOTS:
/*?*/    void run();

    void start_rtl433(const quint32 freq_hz, const QList<quint16> &prot_list = QList<quint16>());
	void stop_rtl433();

    /* get supported protocols from the rtl_433 app */
    void get_supported_protocols_rtl433(QList<rtl_433_supported_protocols> &supp_proto);

private Q_SLOTS:
    void processOutput();
    void processErrorOutput();

Q_SIGNALS:
    void rtl433Error(QString &err_message);
    void rtl433Finished();
    void rtl433ProcessOutput(const QString& json_line);
    void rtl433ProcessRawOutput(const QString& one_line);
};



#endif // RTL_433_H
