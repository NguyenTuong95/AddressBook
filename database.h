#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QString>
#include "contact.h"

class Database
{
public:
    Database(const char* driver);
    ~Database();

    QSqlDatabase* connect( const QString& server,
                           const QString& databaseName,
                           const QString& userName,
                           const QString& password );
    void disConnect();
    int selectRowCountResult(QSqlQuery* query);
    bool execute(QSqlQuery* query);
    void updateData(const Contact &contact_);
    Contact selectDataFromTableWhereIdIs(const int &ID);
    void deleteDataWhereIdIs(const int &ID);
    void orderBy(const QString &clolumn_name, const QString &sort_type);
    void insertData(const Contact &contact_);
    QString getMaxValueOfColumn(const QString &clolumn_name);

private:
    QSqlDatabase* db;
    QString DatabaseName;
};

#endif // DATABASE_H
