#include <QDebug>

#include "data_structs.h"
#include <QFile>
#include <QSharedPointer>

/**
 * @brief data_structs_register_meta_types
 * added all metatypes for used in queue connect modes
 */
void data_structs_register_meta_types(void) {

}


/********* class date&time *******/
void dateTime::setDateTime(quint32 year, quint32 month, quint32 day,
                     quint32 hour, quint32 minute, quint32 second) {
    dt = QDateTime(QDate(year, month, day), QTime(hour, minute, second), Qt::LocalTime);
    active_second = true;
}

void dateTime::setDateTime(quint32 year, quint32 month, quint32 day,
                     quint32 hour, quint32 minute) {
    dt = QDateTime(QDate(year, month, day), QTime(hour, minute), Qt::LocalTime);
}

void dateTime::setDateTime(const dateTime& dt) {
    this->dt = dt.dt;
    this->active_second = dt.active_second;
}

void dateTime::setDateTime(const QDateTime& dt) {
    this->dt = dt;
    this->active_second = true;
}

QDateTime dateTime::getDateTime() const {
    return dt;
}

quint32 dateTime::getYear() const {
    return dt.date().year();
}

quint32 dateTime::getMonth() const {
    return dt.date().month();
}

quint32 dateTime::getDay() const {
    return dt.date().day();
}

quint32 dateTime::getHour() const {
    return dt.time().hour();
}

quint32 dateTime::getMinute() const {
    return dt.time().minute();
}

quint32 dateTime::getSecond() const {
    return dt.time().second();
}

/* alternative constructors */
dateTime::dateTime(const QDateTime & dt) {
    setDateTime(dt);
}

dateTime::dateTime(quint32 year, quint32 month, quint32 day,
             quint32 hour, quint32 minute, quint32 second) {
    setDateTime(year, month, day, hour, minute, second);
}

bool dateTime::isActiveSecond() const {
    return this->active_second;
}

dateTime::dateTime(quint32 year, quint32 month, quint32 day,
    quint32 hour, quint32 minute) {
    setDateTime(year, month, day, hour, minute);
}


