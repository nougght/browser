#include "util.h"


QString formatTimeLocal(Timestamp timestamp) {
    QDateTime localTime = QDateTime::fromSecsSinceEpoch(timestamp.Seconds()).toLocalTime();
    return localTime.toString("dd.MM.yyyy HH:mm:ss");
}