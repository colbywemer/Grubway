#ifndef LOGS_H
#define LOGS_H

#include "account.h"

#include <QDateTime>
#include <QString>

struct Logs
{
    int id;
    QString action;
    QDateTime time;
    Account account;
    QString type;
};

#endif // LOGS_H
