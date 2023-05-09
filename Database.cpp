#include <common.h>

#include <Account>
#include <Item>
#include <Logs>
#include <Orders>
#include <Receipts>

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QVariant>

#include <sstream>

using namespace std;

#define VERSION 9
#define ERROR -1

void nukeTables(QSqlDatabase& db)
{
    if (!db.open())
    {
        qDebug() << "Failed to open database";

        exit(ERROR);
    }

    QStringList tables = db.tables();

    // Delete all tables in the database
    foreach (QString table, tables)
    {
        QSqlQuery query2(db);

        if (table != "Version")
        {
            query2.exec("DROP TABLE " + table);
        }
    }
}

void createTables()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Items.db");

    if (!db.open())
    {
        qDebug() << "Failed to open database";

        exit(ERROR);
    }

    QSqlQuery query(db);

    if (!query.exec("CREATE TABLE IF NOT EXISTS Version(id INTEGER PRIMARY KEY AUTOINCREMENT, version INTEGER)"))
    {
        qDebug() << "Failed to create Orders table:";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    query.prepare("SELECT COUNT(*) FROM Version");

    if (query.exec() && query.first())
    {
        int count = query.value(Empty).toInt();

        if (count > Empty)
        {
            query.prepare("SELECT * FROM Version WHERE id = 1");

            if(query.exec() && query.first())
            {
                if(query.value("version").toInt() != VERSION)
                {
                    qDebug() << "Outdated database... updating";

                    nukeTables(db);

                    qDebug() << "Nuked Tables";

                    query.prepare("UPDATE Version SET version = :version WHERE id = 1");
                    query.bindValue(":version", VERSION);

                    if (!query.exec())
                    {
                        qDebug() << "Failed to update data:";

                        exit(ERROR);
                    }
                }
            }
        }

        else
        {
            qDebug() << "Outdated database... updating";

            nukeTables(db);

            qDebug() << "Nuked Tables";

            query.prepare("INSERT INTO Version (version) VALUES (:version)");
            query.bindValue(":version", VERSION);

            if (!query.exec())
            {
                qDebug() << "Failed to insert data:";

                exit(ERROR);
            }
        }
    }

    else
    {
        qDebug() << "Failed to access Version table";

        exit(ERROR);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Items(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, price REAL, department TEXT, isPerPound INTEGER, stock REAL, sold REAL)"))
    {
        qDebug() << "Failed to create items table";

        exit(ERROR);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Accounts(id INTEGER PRIMARY KEY AUTOINCREMENT, firstName TEXT, lastName TEXT, phoneNumber TEXT, username TEXT, password TEXT, position TEXT, address TEXT, email TEXT, code TEXT)"))
    {
        qDebug() << "Failed to create accounts table";

        exit(ERROR);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Orders(id INTEGER PRIMARY KEY AUTOINCREMENT, items TEXT, total REAL, pickupDate DATETIME, status TEXT, notes TEXT, accountId INTEGER, FOREIGN KEY(accountId) REFERENCES Accounts(id))"))
    {
        qDebug() << "Failed to create Orders table";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Receipts(id INTEGER PRIMARY KEY AUTOINCREMENT, items TEXT, prices TEXT, total REAL, accountId INTEGER, datetime DATETIME, FOREIGN KEY(accountId) REFERENCES Accounts(id))"))
    {
        qDebug() << "Failed to create Orders table";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Logs(id INTEGER PRIMARY KEY AUTOINCREMENT, action TEXT, accountId INTEGER, datetime DATETIME, type TEXT, FOREIGN KEY(accountId) REFERENCES Accounts(id))"))
    {
        qDebug() << "Failed to create Orders table";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }
}

vector<Item> selectAllItems()
{
    vector<Item> items;

    QSqlQuery query;

    if (query.exec("SELECT * FROM Items"))
    {
        while (query.next())
        {
            Item item;
            item.id = query.value("id").toInt();
            item.name = query.value("name").toString();
            item.price = query.value("price").toDouble();
            item.department = query.value("department").toString();
            item.isPerPound = query.value("isPerPound").toBool();
            item.stock = query.value("stock").toDouble();
            item.sold = query.value("sold").toDouble();

            items.push_back(item);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return items;
}

vector<Item> selectAllItemsByFilter(QString filter, QString keyword)
{
    vector<Item> items;
    QSqlQuery query;
    query.prepare("SELECT * FROM Items WHERE " + filter + " = :keyword");
    query.bindValue(":keyword", keyword);
    if (query.exec())
    {
        while (query.next())
        {
            Item item;
            item.id = query.value("id").toInt();
            item.name = query.value("name").toString();
            item.price = query.value("price").toDouble();
            item.department = query.value("department").toString();
            item.isPerPound = query.value("isPerPound").toBool();
            item.stock = query.value("stock").toDouble();
            item.sold = query.value("sold").toDouble();

            items.push_back(item);
        }
    }

    else
    {
        qDebug() << "Failed to select data";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    return items;
}

Item selectItemById(int id)
{
    Item item;
    QSqlQuery query;

    query.prepare("SELECT * FROM Items WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.first())
    {
        item.id = query.value("id").toInt();
        item.name = query.value("name").toString();
        item.price = query.value("price").toDouble();
        item.department = query.value("department").toString();
        item.isPerPound = query.value("isPerPound").toBool();
        item.stock = query.value("stock").toDouble();
        item.sold = query.value("sold").toDouble();
    }

    return item;
}

Item selectItemByName(QString name)
{
    Item item;
    QSqlQuery query;

    query.prepare("SELECT * FROM Items WHERE name = :name");
    query.bindValue(":name", name);

    if (query.exec() && query.first())
    {
        item.id = query.value("id").toInt();
        item.name = query.value("name").toString();
        item.price = query.value("price").toDouble();
        item.department = query.value("department").toString();
        item.isPerPound = query.value("isPerPound").toBool();
        item.stock = query.value("stock").toDouble();
        item.sold = query.value("sold").toDouble();
    }

    return item;
}

vector<Item> selectAllItemsByName(QString name)
{
    vector<Item> items;

    QSqlQuery query;

    query.prepare("SELECT * FROM Items WHERE name = :name");
    query.bindValue(":name", name);

    if (query.exec())
    {
        while(query.next())
        {
            Item item;
            item.id = query.value("id").toInt();
            item.name = query.value("name").toString();
            item.price = query.value("price").toDouble();
            item.department = query.value("department").toString();
            item.isPerPound = query.value("isPerPound").toBool();
            item.stock = query.value("stock").toDouble();
            item.sold = query.value("sold").toDouble();

            items.push_back(item);
        }
    }

    return items;
}

void createItem(Item item)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Items (name, price, department, isPerPound, stock, sold) VALUES (:name, :price, :department, :isPerPound, :stock, :sold)");
    query.bindValue(":name", item.name);
    query.bindValue(":price", item.price);
    query.bindValue(":department", item.department);
    query.bindValue(":isPerPound", item.isPerPound);
    query.bindValue(":stock", item.stock);
    query.bindValue(":sold", item.sold);

    if (!query.exec())
    {
        qDebug() << "Failed to insert data: " << query.lastError().text();

        exit(ERROR);
    }
}

void deleteItem(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Items where id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Failed to delete data:";

        exit(ERROR);
    }
}

void updateItem(Item item){
    QSqlQuery query;

    query.prepare("UPDATE Items SET name = :name, price = :price, department = :department, isPerPound = :isPerPound, stock = :stock, sold = :sold WHERE id = :id");
    query.bindValue(":id", item.id);
    query.bindValue(":name", item.name);
    query.bindValue(":price", item.price);
    query.bindValue(":department", item.department);
    query.bindValue(":isPerPound", item.isPerPound);
    query.bindValue(":stock", item.stock);
    query.bindValue(":sold", item.sold);

    if (!query.exec())
    {
        qDebug() << "Failed to update data:" << query.lastError().text();

        exit(ERROR);
    }
}

vector<Account> selectAllAccounts()
{
    vector<Account> accounts;

    QSqlQuery query;

    if (query.exec("SELECT * FROM Accounts"))
    {
        while (query.next())
        {
            Account account;
            account.id = query.value("id").toInt();
            account.firstName = query.value("firstName").toString();
            account.lastName = query.value("lastName").toString();
            account.phoneNumber = query.value("phoneNumber").toString();
            account.username = query.value("username").toString();
            account.password = query.value("password").toString();
            account.position = query.value("position").toString();
            account.address = query.value("address").toString();
            account.email = query.value("email").toString();
            account.code = query.value("code").toString();

            accounts.push_back(account);
        }
    }

    return accounts;
}

vector<Account> selectAllAccountsByFilter(QString filter, QString keyword)
{
    vector<Account> accounts;
    QSqlQuery query;
    query.prepare("SELECT * FROM Accounts WHERE " + filter + " = :keyword");
    query.bindValue(":keyword", keyword);
    if (query.exec())
    {
        while (query.next())
        {
            Account account;
            account.id = query.value("id").toInt();
            account.firstName = query.value("firstName").toString();
            account.lastName = query.value("lastName").toString();
            account.phoneNumber = query.value("phoneNumber").toString();
            account.username = query.value("username").toString();
            account.password = query.value("password").toString();
            account.position = query.value("position").toString();
            account.address = query.value("address").toString();
            account.email = query.value("email").toString();
            account.code = query.value("code").toString();

            accounts.push_back(account);
        }
    }

    return accounts;
}

Account selectAccountById(int id){
    Account account;
    QSqlQuery query;

    query.prepare("SELECT * FROM Accounts WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.first())
    {
        account.id = query.value("id").toInt();
        account.firstName = query.value("firstName").toString();
        account.lastName = query.value("lastName").toString();
        account.phoneNumber = query.value("phoneNumber").toString();
        account.username = query.value("username").toString();
        account.password = query.value("password").toString();
        account.position = query.value("position").toString();
        account.address = query.value("address").toString();
        account.code = query.value("code").toString();
        account.email = query.value("email").toString();
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return account;
}

bool administratorExist() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Accounts WHERE position = 'Administrator'");
    if (query.exec() && query.first()) {
        int count = query.value(0).toInt();
        return count > 0;
    } else {
        qDebug() << "Failed to select data";
        exit(ERROR);
    }
}


Account selectAccountByUsername(QString username)
{
    Account account;
    QSqlQuery query;

    query.prepare("SELECT * FROM Accounts WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.first())
    {
        account.id = query.value("id").toInt();
        account.firstName = query.value("firstName").toString();
        account.lastName = query.value("lastName").toString();
        account.phoneNumber = query.value("phoneNumber").toString();
        account.username = query.value("username").toString();
        account.password = query.value("password").toString();
        account.position = query.value("position").toString();
        account.address = query.value("address").toString();
        account.email = query.value("email").toString();
        account.code = query.value("code").toString();
    }

    return account;
}

Account accountCodeExists(QString code)
{
    QSqlQuery query;
    Account account;
    query.prepare("SELECT * FROM Accounts WHERE code = :code");
    query.bindValue(":code", code);
    if (query.exec() && query.first())
    {
        account.id = query.value("id").toInt();
        account.firstName = query.value("firstName").toString();
        account.lastName = query.value("lastName").toString();
        account.phoneNumber = query.value("phoneNumber").toString();
        account.username = query.value("username").toString();
        account.password = query.value("password").toString();
        account.position = query.value("position").toString();
        account.address = query.value("address").toString();
        account.email = query.value("email").toString();
        account.code = query.value("code").toString();
    }

    return account;
}

void createAccount(Account account)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Accounts (firstName, lastName, phoneNumber, username, password, position, address, email, code) VALUES (:firstName, :lastName, :phoneNumber, :username, :password, :position, :address, :email, :code)");
    query.bindValue(":firstName", account.firstName);
    query.bindValue(":lastName", account.lastName);
    query.bindValue(":phoneNumber", account.phoneNumber);
    query.bindValue(":username", account.username);
    query.bindValue(":password", account.password);
    query.bindValue(":position", account.position);
    query.bindValue(":address", account.address);
    query.bindValue(":email", account.email);
    query.bindValue(":code", account.code);

    if (!query.exec())
    {
        qDebug() << "Failed to insert data:";

        exit(ERROR);
    }
}

void deleteAccount(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Accounts WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Failed to delete data:";

        exit(ERROR);
    }
}

void updateAccount(Account account){
    QSqlQuery query;

    query.prepare("UPDATE Accounts SET firstName = :firstName, lastName = :lastName, phoneNumber = :phoneNumber, username = :username, password = :password, position = :position, address = :address, email = :email, code = :code WHERE id = :id");
    query.bindValue(":id", account.id);
    query.bindValue(":firstName", account.firstName);
    query.bindValue(":lastName", account.lastName);
    query.bindValue(":phoneNumber", account.phoneNumber);
    query.bindValue(":username", account.username);
    query.bindValue(":password", account.password);
    query.bindValue(":position", account.position);
    query.bindValue(":address", account.address);
    query.bindValue(":email", account.email);
    query.bindValue(":code", account.code);

    if (!query.exec())
    {
        qDebug("Failed to update data:");

        exit(ERROR);
    }
}

vector<Order> selectAllOrders()
{
    vector<Order> orders;
    QSqlQuery query;
    if (query.exec("SELECT Orders.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Orders JOIN Accounts ON Orders.accountId = Accounts.id"))
    {
        while (query.next())
        {
            Order order;
            order.id = query.value("id").toInt();
            order.total = query.value("total").toDouble();
            order.account.firstName = query.value("firstName").toString();
            order.account.lastName = query.value("lastName").toString();
            order.account.username = query.value("username").toString();
            order.account.address = query.value("address").toString();
            order.account.email = query.value("email").toString();
            order.account.phoneNumber = query.value("phoneNumber").toString();
            order.account.id = query.value("accountId").toInt();
            order.status = query.value("status").toString();
            order.pickupDate = query.value("pickupDate").toDateTime();
            order.notes = query.value("notes").toString();

            QString items = query.value("items").toString();

            istringstream ss(items.toStdString());
            string item;

            while (getline(ss, item, ','))
            {
                order.items[QString::fromStdString(item)]++;
            }

            orders.push_back(order);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return orders;
}

vector<Order> selectAllFilteredOrdersByDate(QString date)
{
    vector<Order> orders;

    QSqlQuery query;

    QDateTime start = QDateTime::fromString(date + " 00:00:00", "MM/dd/yyyy hh:mm:ss");
    QDateTime end = QDateTime::fromString(date + " 23:59:59", "MM/dd/yyyy hh:mm:ss");

    query.prepare("SELECT Orders.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Orders JOIN Accounts ON Orders.accountId = Accounts.id WHERE pickupDate BETWEEN :start AND :end");
    query.bindValue(":start", start);
    query.bindValue(":end", end);

    if (query.exec())
    {
        while (query.next())
        {
            Order order;
            order.id = query.value("id").toInt();
            order.total = query.value("total").toDouble();
            order.account.firstName = query.value("firstName").toString();
            order.account.lastName = query.value("lastName").toString();
            order.account.username = query.value("username").toString();
            order.account.address = query.value("address").toString();
            order.account.email = query.value("email").toString();
            order.account.phoneNumber = query.value("phoneNumber").toString();
            order.account.id = query.value("accountId").toInt();
            order.status = query.value("status").toString();
            order.pickupDate = query.value("pickupDate").toDateTime();
            order.notes = query.value("notes").toString();

            QString items = query.value("items").toString();

            istringstream ss(items.toStdString());
            string item;

            while (getline(ss, item, ','))
            {
                order.items[QString::fromStdString(item)]++;
            }

            orders.push_back(order);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return orders;
}

vector<Order> selectAllFilteredOrdersByName(QString firstName, QString lastName)
{
    vector<Order> orders;

    QSqlQuery query;

    query.prepare("SELECT Orders.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Orders JOIN Accounts ON Orders.accountId = Accounts.id WHERE Accounts.firstName = :firstName AND Accounts.lastName = :lastName");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);

    if (query.exec())
    {
        while (query.next())
        {
            Order order;
            order.id = query.value("id").toInt();
            order.total = query.value("total").toDouble();
            order.account.firstName = query.value("firstName").toString();
            order.account.lastName = query.value("lastName").toString();
            order.account.username = query.value("username").toString();
            order.account.address = query.value("address").toString();
            order.account.email = query.value("email").toString();
            order.account.phoneNumber = query.value("phoneNumber").toString();
            order.account.id = query.value("accountId").toInt();
            order.status = query.value("status").toString();
            order.pickupDate = query.value("pickupDate").toDateTime();
            order.notes = query.value("notes").toString();

            QString items = query.value("items").toString();

            istringstream ss(items.toStdString());
            string item;

            while (getline(ss, item, ','))
            {
                order.items[QString::fromStdString(item)]++;
            }

            orders.push_back(order);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return orders;
}

vector<Order> selectAllFilteredOrdersByStatus(QString status)
{
    vector<Order> orders;

    QSqlQuery query;

    query.prepare("SELECT Orders.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Orders JOIN Accounts ON Orders.accountId = Accounts.id WHERE status = :status");
    query.bindValue(":status", status);

    if (query.exec())
    {
        while (query.next())
        {
            Order order;
            order.id = query.value("id").toInt();
            order.total = query.value("total").toDouble();
            order.account.firstName = query.value("firstName").toString();
            order.account.lastName = query.value("lastName").toString();
            order.account.username = query.value("username").toString();
            order.account.address = query.value("address").toString();
            order.account.email = query.value("email").toString();
            order.account.phoneNumber = query.value("phoneNumber").toString();
            order.account.id = query.value("accountId").toInt();
            order.status = query.value("status").toString();
            order.pickupDate = query.value("pickupDate").toDateTime();
            order.notes = query.value("notes").toString();

            QString items = query.value("items").toString();

            istringstream ss(items.toStdString());
            string item;

            while (getline(ss, item, ','))
            {
                order.items[QString::fromStdString(item)]++;
            }

            orders.push_back(order);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return orders;
}

Order selectOrderById(int id)
{
    Order order;
    QSqlQuery query;
    query.prepare("SELECT Orders.*, Accounts.firstName, Accounts.lastName, Accounts.email, Accounts.address, Accounts.username, Accounts.phoneNumber "
        "FROM Orders JOIN Accounts ON Orders.accountId = Accounts.id "
        "WHERE Orders.id = :orderId");

    query.bindValue(":orderId", id);

    if (query.exec() && query.first())
    {
        order.id = query.value("id").toInt();
        order.total = query.value("total").toDouble();
        order.account.firstName = query.value("firstName").toString();
        order.account.lastName = query.value("lastName").toString();
        order.account.username = query.value("username").toString();
        order.account.address = query.value("address").toString();
        order.account.email = query.value("email").toString();
        order.account.phoneNumber = query.value("phoneNumber").toString();
        order.account.id = query.value("accountId").toInt();
        order.status = query.value("status").toString();
        order.pickupDate = query.value("pickupDate").toDateTime();
        order.notes = query.value("notes").toString();

        QString items = query.value("items").toString();

        istringstream ss(items.toStdString());
        string item;

        while (getline(ss, item, ','))
        {
            order.items[QString::fromStdString(item)]++;
        }
    }

    else
    {
        qDebug() << "Failed to select data";
    }

    return order;
}

void createOrder(Order order)
{
    QString items;

    for(const auto& [item, count] : order.items)
    {
        for(int i = 0; i < count; i++)
        {
            items += item + ",";
        }
    }

    items.remove(items.length() - 1, 1);

    QSqlQuery query;

    query.prepare("INSERT INTO Orders (items, total, pickupDate, status, notes, accountId) VALUES (:items, :total, :pickupDate, :status, :notes, :accountId)");
    query.bindValue(":total", order.total);
    query.bindValue(":accountId", order.account.id);
    query.bindValue(":pickupDate", order.pickupDate);
    query.bindValue(":status", order.status);
    query.bindValue(":notes", order.notes);
    query.bindValue(":items", items);

    if (!query.exec())
    {
        qDebug("Failed to insert data:");

        exit(ERROR);
    }
}

void deleteOrder(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Orders where id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Failed to delete data:";

        exit(ERROR);
    }
}

void updateOrder(Order order)
{
    QString items;

    for(const auto& [item, count] : order.items)
    {
        for(int i = 0; i < count; i++)
        {
            items += item + ",";
        }
    }

    items.remove(items.length() - 1, 1);

    QSqlQuery query;

    query.prepare("UPDATE Orders SET items = :items, total = :total, accountId = :accountId, status = :status, pickupDate = :pickupDate, notes = :notes WHERE id = :id");
    query.bindValue(":id", order.id);
    query.bindValue(":total", order.total);
    query.bindValue(":accountId", order.account.id);
    query.bindValue(":status", order.status);
    query.bindValue(":pickupDate", order.pickupDate);
    query.bindValue(":notes", order.notes);
    query.bindValue(":items", items);

    if (!query.exec())
    {
        qDebug() << "Failed to update data:";

        exit(ERROR);
    }
}

vector<Receipts> selectAllRecipts(){
    vector<Receipts> receipts;

    QSqlQuery query;

    if (query.exec("SELECT Receipts.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Receipts JOIN Accounts ON Receipts.accountId = Accounts.id"))
    {
        while (query.next())
        {
            Receipts receipt;
            receipt.id = query.value("id").toInt();
            receipt.total = query.value("total").toDouble();
            receipt.account.firstName = query.value("firstName").toString();
            receipt.account.lastName = query.value("lastName").toString();
            receipt.account.username = query.value("username").toString();
            receipt.account.address = query.value("address").toString();
            receipt.account.email = query.value("email").toString();
            receipt.account.phoneNumber = query.value("phoneNumber").toString();
            receipt.account.id = query.value("accountId").toInt();
            receipt.datetime = query.value("datetime").toDateTime();

            QString items = query.value("items").toString(),
                prices = query.value("prices").toString();

            istringstream ss(items.toStdString()),
                ps(prices.toStdString());

            string item, priceString;

            while (getline(ss, item, ',') && getline(ps, priceString, ','))
            {
                receipt.items[QString::fromStdString(item)]++;
                receipt.prices[QString::fromStdString(item)] += stod(priceString);
            }

            receipts.push_back(receipt);
        }
    }

    else
    {
        qDebug() << "Failed to select data:" << query.lastError().text();

        exit(ERROR);
    }

    return receipts;
}

Receipts selectReceiptById(int id)
{
    Receipts receipt;
    QSqlQuery query;

    query.prepare("SELECT Receipts.*, Accounts.firstName, Accounts.lastName, Accounts.email, Accounts.address, Accounts.username, Accounts.phoneNumber "
        "FROM Receipts JOIN Accounts ON Receipts.accountId = Accounts.id "
        "WHERE Receipts.id = :orderId");

    query.bindValue(":orderId", id);

    if (query.exec() && query.first())
    {
        receipt.id = query.value("id").toInt();
        receipt.total = query.value("total").toDouble();
        receipt.account.firstName = query.value("firstName").toString();
        receipt.account.lastName = query.value("lastName").toString();
        receipt.account.username = query.value("username").toString();
        receipt.account.address = query.value("address").toString();
        receipt.account.email = query.value("email").toString();
        receipt.account.phoneNumber = query.value("phoneNumber").toString();
        receipt.account.id = query.value("accountId").toInt();
        receipt.datetime = query.value("datetime").toDateTime();

        QString items = query.value("items").toString(),
            prices = query.value("prices").toString();

        istringstream ss(items.toStdString()),
            ps(prices.toStdString());

        string item, priceString;

        while (getline(ss, item, ',') && getline(ps, priceString, ','))
        {
            receipt.items[QString::fromStdString(item)]++;
            receipt.prices[QString::fromStdString(item)] += stod(priceString);
        }
    }

    else
    {
        qDebug() << "Failed to select data";

        exit(ERROR);
    }

    return receipt;
}

void createReceipt(Receipts receipt)
{
    QString items, prices;

    for (const auto& pair : receipt.items)
    {
        QString item = pair.first;
        int quantity = pair.second;

        double price = receipt.prices[item];

        for (int i = 0; i < quantity; i++)
        {
            items += item + ",";
            prices += QString::number(price/quantity) + ",";
        }
    }

    items.remove(items.length() + Decrement, 1);
    prices.remove(prices.length() + Decrement, 1);

    QSqlQuery query;

    query.prepare("INSERT INTO Receipts (items, total, accountId, datetime, prices) VALUES (:items, :total, :accountId, :datetime, :prices)");
    query.bindValue(":total", receipt.total);
    query.bindValue(":accountId", receipt.account.id);
    query.bindValue(":items", items);
    query.bindValue(":prices", prices);
    query.bindValue(":datetime", receipt.datetime);

    if (!query.exec())
    {
        qDebug() << "Failed to create data:" << query.lastError().text();

        exit(ERROR);
    }
}

void deleteReceipt(int id){
    QSqlQuery query;

    query.prepare("DELETE FROM Receipts where id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Failed to delete data:";

        exit(ERROR);
    }
}

void updateReceipt(Receipts receipt)
{
    QString items, prices;

    for (const auto& pair : receipt.items)
    {
        QString item = pair.first;
        int quantity = pair.second;

        double price = receipt.prices[item];

        for (int i = 0; i < quantity; i++)
        {
            items += item + ",";
            prices += QString::number(price/quantity) + ",";
        }
    }

    items.remove(items.length() + Decrement, 1);
    prices.remove(prices.length() + Decrement, 1);

    QSqlQuery query;

    query.prepare("UPDATE Receipts SET items = :items, total = :total, accountId = :accountId, datetime = :datetime, items = :items WHERE id = :id");
    query.bindValue(":id", receipt.id);
    query.bindValue(":total", receipt.total);
    query.bindValue(":accountId", receipt.account.id);
    query.bindValue(":items", items);
    query.bindValue(":prices", prices);
    query.bindValue(":datetime", receipt.datetime);

    if (!query.exec())
    {
        qDebug() << "Failed to update data:";

        exit(ERROR);
    }
}

vector<Logs> selectAllLogs()
{
    vector<Logs> logs;

    QSqlQuery query;

    if (query.exec("SELECT Logs.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Logs JOIN Accounts ON Logs.accountId = Accounts.id ORDER BY datetime DESC"))
    {
        while (query.next())
        {
            Logs log;
            log.id = query.value("id").toInt();
            log.account.firstName = query.value("firstName").toString();
            log.account.lastName = query.value("lastName").toString();
            log.account.username = query.value("username").toString();
            log.account.address = query.value("address").toString();
            log.account.email = query.value("email").toString();
            log.account.phoneNumber = query.value("phoneNumber").toString();
            log.account.id = query.value("accountId").toInt();
            log.action = query.value("action").toString();
            log.time = query.value("datetime").toDateTime();
            logs.push_back(log);
        }
    }

    else
    {
        qDebug() << "Failed to select data";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    return logs;
}

void createLog(Logs log)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Logs (action, datetime, accountId, type) VALUES (:action, :datetime, :accountId, :type)");
    query.bindValue(":action", log.action);
    query.bindValue(":accountId", log.account.id);
    query.bindValue(":datetime", log.time);
    query.bindValue(":type", log.type);


    if (!query.exec())
    {
        qDebug() << "Failed to insert data:";

        exit(ERROR);
    }
}

void deleteLog(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Logs where id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Failed to delete data:";

        exit(ERROR);
    }
}

void updateLog(Logs log)
{
    QSqlQuery query;

    query.prepare("UPDATE Logs SET action = :action, datetime = :datetime, accountId = :accountId WHERE id = :id");
    query.bindValue(":id", log.id);
    query.bindValue(":accountId", log.account.id);
    query.bindValue(":datetime", log.time);

    if (!query.exec())
    {
        qDebug() << "Failed to update data:";

        exit(ERROR);
    }
}

vector<Logs> selectAllLogsByDate(QString date)
{
    vector<Logs> logs;

    QSqlQuery query;
    QDateTime start = QDateTime::fromString(date + " 00:00:00", "MM/dd/yyyy hh:mm:ss");
    QDateTime end = QDateTime::fromString(date + " 23:59:59", "MM/dd/yyyy hh:mm:ss");

    query.prepare("SELECT Logs.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Logs JOIN Accounts ON Logs.accountId = Accounts.id WHERE datetime BETWEEN :start AND :end ORDER BY datetime DESC");
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    if (query.exec())
    {
        while (query.next())
        {
            Logs log;
            log.id = query.value("id").toInt();
            log.account.firstName = query.value("firstName").toString();
            log.account.lastName = query.value("lastName").toString();
            log.account.username = query.value("username").toString();
            log.account.address = query.value("address").toString();
            log.account.email = query.value("email").toString();
            log.account.phoneNumber = query.value("phoneNumber").toString();
            log.account.id = query.value("accountId").toInt();
            log.action = query.value("action").toString();
            log.time = query.value("datetime").toDateTime();
            logs.push_back(log);
        }
    }

    else
    {
        qDebug() << "Failed to select data";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    return logs;
}

vector<Logs> selectAllLogsByFilter(QString filter, QString keyword)
{
    vector<Logs> logs;

    QSqlQuery query;

    query.prepare("SELECT Logs.*, Accounts.firstName, Accounts.lastName, Accounts.phoneNumber, Accounts.username, Accounts.address, Accounts.email FROM Logs JOIN Accounts ON Logs.accountId = Accounts.id WHERE " + filter + " = :keyword ORDER BY datetime DESC");
    query.bindValue(":keyword", keyword);
    if (query.exec())
    {
        while (query.next())
        {
            Logs log;
            log.id = query.value("id").toInt();
            log.account.firstName = query.value("firstName").toString();
            log.account.lastName = query.value("lastName").toString();
            log.account.username = query.value("username").toString();
            log.account.address = query.value("address").toString();
            log.account.email = query.value("email").toString();
            log.account.phoneNumber = query.value("phoneNumber").toString();
            log.account.id = query.value("accountId").toInt();
            log.action = query.value("action").toString();
            log.time = query.value("datetime").toDateTime();
            logs.push_back(log);
        }
    }

    else
    {
        qDebug() << "Failed to select data";
        qDebug() << query.lastError().text();

        exit(ERROR);
    }

    return logs;
}

