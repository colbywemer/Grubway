#ifndef ORDERS_H
#define ORDERS_H

#include "account.h"

#include <QDateTime>
#include <QString>

#include <map>

using namespace std;

struct Order{
    int id;
    Account account;
    double total;
    map<QString, int> items;
    QString status, notes;
    QDateTime pickupDate;
};

#endif // ORDERS_H
