#include "log.h"

bool g_enableFullLogging;

static const char *const logstrs[_LOG_LEVEL_BORDER] = {
    [LOG_ERR]  =    "ERROR",
    [LOG_WARN] =    "WARNING",
    [LOG_INFO] =    "INFO",
    [LOG_DBG]  =    "DBG",
    [LOG_TEST] =    "TEST",
};

#define logstrs_size (sizeof(logstrs)/sizeof(*logstrs))

/*
 * Records in _LOG_LEVEL_BORDER not commpared with logstrs size. Fix It and restart building process!"
*/
//BUILD_BUG_ON(logstrs_size > _LOG_LEVEL_BORDER);


void _LOG(const char *const preprint_msg, const en_log_level level, const QString str) {
#if !defined(DEBUG)
    if(g_enableFullLogging || (level <= LOG_INFO)) {
#endif
        //qDebug("%s", qPrintable(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss") + " [" + (level) + "] " +  PRE_PRINT_MESSAGE + ": " + (str)));
        fprintf(stderr, "%s\n", qPrintable(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss") +
            " [" + logstrs[level%(sizeof(logstrs)/sizeof(*logstrs))] + "] " +  preprint_msg + ": " + (str)));
#if !defined(DEBUG)
    }
#endif
}
