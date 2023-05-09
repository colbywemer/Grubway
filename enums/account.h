#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

struct Account
{
    int id;
    QString firstName, lastName, phoneNumber, address,
        username, password, email,
        position, code;
};

#endif // ACCOUNT_H
