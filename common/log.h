#ifndef LOG_H
#define LOG_H

#include <QtCore/QCoreApplication>

#if !defined(PRE_PRINT_MESSAGE)
#define PRE_PRINT_MESSAGE "Other"
#endif

#include <QtDebug>
#include <QDateTime>

enum en_log_level {
    LOG_ERR = 0,
    LOG_WARN,
    LOG_INFO,
    LOG_DBG,
    LOG_TEST,

/* the end */
    _LOG_LEVEL_BORDER
};

void _LOG(const char *const preprint_msg, const en_log_level level, const QString str);

#define LOG(level, msg) _LOG(PRE_PRINT_MESSAGE, level, msg)

#endif // LOG_H
