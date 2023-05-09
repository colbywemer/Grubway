
#ifndef RECIPTSH_H
#define RECIPTSH_H

#define TAX_RATE            7.0

#include "account.h"

#include <QDateTime>
#include <QString>

#include <map>

using namespace std;

struct Receipts{
    int id;
    Account account;
    double total;
    QDateTime datetime;
    map<QString, int> items;
    map<QString, double> prices;
};

#endif // RECIPTSH_H
