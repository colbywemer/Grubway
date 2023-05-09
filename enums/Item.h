#ifndef ITEM_H
#define ITEM_H

#include <QString>

struct Item
{
    int id;
    QString name, department;
    bool isPerPound;
    double price,
        stock = 0, sold = 0;
};

#endif // ITEM_H
