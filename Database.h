#ifndef DATABASE_H
#define DATABASE_H

#include <Account>
#include <Item>
#include <Logs>
#include <Orders>
#include <Receipts>

using namespace std;

Account accountCodeExists(QString code);

Account selectAccountById(int id);
Account selectAccountByUsername(QString username);

Item selectItemById(int id);
Item selectItemByName(QString name);

Order selectOrderById(int id);

Receipts selectReceiptById(int id);

vector<Account> selectAllAccounts();
vector<Account> selectAllAccountsByFilter(QString filter, QString keyword);

vector<Item> selectAllItems();
vector<Item> selectAllItemsByName(QString name);
vector<Item> selectAllItemsByFilter(QString filter, QString keyword);

vector<Logs> selectAllLogs();
vector<Logs> selectAllLogsByFilter(QString filter, QString keyword);
vector<Logs> selectAllLogsByDate(QString date);

vector<Order> selectAllFilteredOrdersByDate(QString date);
vector<Order> selectAllFilteredOrdersByName(QString firstName, QString lastName);
vector<Order> selectAllFilteredOrdersByStatus(QString status);

vector<Order> selectAllOrders();

vector<Receipts> selectAllRecipts();

void createTables();
void nukeTables();

void createAccount(Account account);
void deleteAccount(int id);
void updateAccount(Account account);

void createItem(Item item);
void deleteItem(int id);
void updateItem(Item item);

void createLog(Logs log);
void deleteLog(int id);
void updateLog(Logs log);

void createOrder(Order order);
void deleteOrder(int id);
void updateOrder(Order order);

void createReceipt(Receipts receipt);
void deleteReceipt(int id);
void updateReceipt(Receipts receipt);

bool administratorExist();

#endif // DATABASE_H
