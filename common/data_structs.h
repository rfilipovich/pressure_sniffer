#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMetaEnum>
#include <QDateTime>
#include <QDataStream>
#include <QVector>
#include <QQueue>


extern void data_structs_register_meta_types(void);

/**
 * @brief The PioneerErrorCodesEnum enum
 */
enum PioneerErrorCodesEnum {
    NO_ERROR = 0,
    WRONG_USER_ERROR,
    WRONG_PASSWORD_ERROR,
    WRONG_CHECK_CODE_ERROR,
    PRICE_IS_INCORRECT_ERROR,
    QUANTITY_IS_INCORRECT,
    DISCOUNT_OR_VAR_IS_OUTOFBAND_ERROR,
    PAYMENT_TYPE_IS_INCORRECT_ERROR,
    PAYMENT_SUM_BYTYPE_IS_BELOW_ZERO_ERROR,
    NOT_ENOUGH_CASH_INBOX_SELECT_OTHER_PAYMENT_TYPE_ERROR,
    TAX_SELECTION_IS_WRONG_ERROR,
    SHIFT_ALREADY_IS_OPENED_ERROR,
    DEMO_SHIFT_ALREADY_IS_OPENED_ERROR,
    DEMO_COMMAND_ERROR_BECAUSE_DEMOMODE_IS_DISABLED,
    DEMO_COMMAND_ERROR_BECAUSE_SHIFT_IS_OPENED,
    NOT_ENOUGH_CASH_INBOX_ERROR,
    NOT_ENOUGH_RIGHTS,
    SHIFT_IS_NOT_OPENED_YET_ERROR,
    DEMO_SHIFT_IS_NOT_OPENED_YET_ERROR,
    SHIFT_IS_NOT_CLOSED_YET_ERROR,
    MODE_WAS_INCORRECTLY_SPECIFIED_ERROR,
    SPECIFIED_DATE_IS_OUTOFBAND_ERROR,
    SPECIFIED_TIME_IS_OUTOFBAND_ERROR,
    SPECIFIED_DOCS_ID_IS_OUTOFBAND_ERROR,
    NO_DATA_ERROR,
    UNRECOGNIZED_ERROR,
    WRONG_SIGNAL_DURATION_ERROR,
    WRONG_KKMPROTO_ID_ERROR,
    WRONG_IP_OR_DNS_ERROR,
    WRONG_TOKEN_ERROR,
    WRONG_PORT_ERROR,
    NO_CONNECTION_WITH_SERVER_ERROR,
    TAPE_FOR_CHECK_IS_OFF_ERROR,
    KKM_CAPE_IS_UP_ERROR,
    PRINTING_ERROR,
    CUTTING_PAPER_ERROR,
    OPENING_DRAWER_ERROR,
    BEEPING_ERROR,
    OFFLINE_MODE_ERROR,
    BLOCKED_MODE_ERROR,
    SBLOCKED_MODE_ERROR,
    PRINTER_ERROR,
    NFD_ERROR,
    DO_NOT_USE_DATETIME_AND_DOCUMENT_NUMBERS_TOGETHER,
    WRONG_SPECIFIED_DATETIME,
    SPECIFIED_CASHIER_NOT_EXIST,
    SPECIFIED_SECTION_NOT_EXIST,
    SERIAL_NUMBER_ON_SERVER_ERROR,
    REQUEST_PROCESSING_TIMEOUT_ERROR,
    RESPONSE_DONE_SIGNAL_TIMEOUT_ERROR,
    IS_OFFLINE_RECORD_OLDER_72_HOUR,
    DEPOSIT_OR_WITHDRAWAL_SUM_IS_ZERO_ERROR,
    NOT_ENOUGH_PAYMENT_SUM,
    WRITTING_TO_DATA_BASE_ERROR,
    TICKET_CANCELED,
    CANCEL_TICKET_ERROR,
    COMMAND_ERROR_BECAUSE_DEMO_SHIFT_IS_OPENED,
    DISCOUNT_ON_POSITION_NOT_ALLOWED,
    DISCOUNT_ON_CHECK_NOT_ALLOWED,
    MARKUP_ON_POSITION_NOT_ALLOWED,
    MARKUP_ON_CHECK_NOT_ALLOWED,
    DISCOUNT_AND_MARKUP_ON_CHECK_NOT_ALLOWED_TOGETHER,
    MISSING_REQUIRED_FIELD,
    OPEN_SHIFT_TIMEOUT_EXPIRED,
    READING_FROM_DATA_BASE_ERROR,
    WRONG_CURRENT_DATETIME,
    TAXES_ON_POSITION_AND_CHECK_NOT_ALLOWED_TOGETHER, //TAXES_ON_CHECK_FROM_PROTOCOL_IGNORED
    TAXES_FROM_PROTOCOL_AND_WEB_NOT_ALLOWED_TOGETHER, //TAXES_FROM_PROTOCOL_IGNORED
    WRONG_DATA_IN_FIELD,
    FIELD_DUBLICATION_ERROR,
    XML_READER_ERROR,
    REPORT_XZ_DURING_CLOSED_SHIFT_NOT_ALLOWED,
    COMBINATION_COMMODITY_DOCUMENT_PAYMENTS_ERROR,
    REGISTER_REQUEST_ERROR,
    CHECK_IS_ALREADY_OPENED_ERROR,
    CHECK_IS_NOT_OPENED_ERROR,
    FISCAL_STRINGS_AFTER_PAYMENTS_COMMAND_ERROR,
    PAYMENTS_IN_CLOSECHECK_AND_IN_PAYMENTSCHECK_NOT_ALLOWED_TOGETHER,
    STORNO_COMMODITY_ERROR
};



/**
 * @brief The DateTime class
 */
class dateTime {
public:
    dateTime() {}

protected:
    QDateTime dt;
    bool active_second = false;

public:
    /**
     * @brief setDateTime
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param minute
     * @param second
     */
    void setDateTime(quint32 year, quint32 month, quint32 day,
                     quint32 hour, quint32 minute, quint32 second);

    void setDateTime(quint32 year, quint32 month, quint32 day,
                     quint32 hour, quint32 minute);

    void setDateTime(const dateTime& dt);

    void setDateTime(const QDateTime& dt);

    QDateTime getDateTime() const;

    quint32 getYear() const;

    quint32 getMonth() const;

    quint32 getDay() const;

    quint32 getHour() const;

    quint32 getMinute() const;

    quint32 getSecond() const;

/* alternative constructors */
    /**
     * @brief dateTime
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param minute
     * @param second
     */
    dateTime(quint32 year, quint32 month, quint32 day,
             quint32 hour, quint32 minute, quint32 second);

    /**
     * @brief isActiveSecond
     * @return
     */
    bool isActiveSecond() const;

    /**
     * @brief dateTime
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param minute
     */
    dateTime(quint32 year, quint32 month, quint32 day,
             quint32 hour, quint32 minute);

    /**
     * @brief dateTime
     */
    dateTime(const QDateTime & dt);

    friend QDataStream &operator<<(QDataStream &out, const dateTime &dateT) {
        out  << dateT.dt << dateT.active_second; // dateTime
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, dateTime &dateT) {
        in  >> dateT.dt >> dateT.active_second; // dateTime
        return in;
    }
};


class BacklightConfigVar  {
    Q_GADGET
    //Q_ENUMS(State)
public:

    //enum State {
   //     ONLINE,
   //     OFFLINE,
    //    BLOCKED,
    //    SBLOCKED,
    //};

    BacklightConfigVar() :
       _value(100) {

    }

    BacklightConfigVar(quint8 value) {
        setValue(value);
    }

    quint8 getValues() const {
        return _value;
    }

    void setValue(quint8 new_value) {
        _value = new_value;
    }

private:
    quint8 _value;
};
Q_DECLARE_METATYPE(BacklightConfigVar)

#endif // DATA_STRUCTURES_H
