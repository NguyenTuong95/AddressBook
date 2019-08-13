#include "database.h"

#define INDEX_ID                        0
#define INDEX_NAME                      1
#define INDEX_PHONE_NUMBER              2
#define INDEX_EMAIL                     3
#define INDEX_DATE_OF_BIRTH             4
#define INDEX_AGE                       5

Database::Database(const char* driver)
{
     db = new QSqlDatabase( QSqlDatabase::addDatabase(driver) );
     DatabaseName = "";
}

Database::~Database(){
    qDebug() << "Called Destructor!";
    delete db;
}
QSqlDatabase* Database::connect( const QString& server,
                                     const QString& databaseName,
                                     const QString& userName,
                                     const QString& password ){
    db->setConnectOptions();
    db->setDatabaseName(QString("DRIVER={SQL Server Native Client 11.0};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4;WSID=.;Trusted_connection=yes")
                     .arg(server)
                     .arg(databaseName)
                     .arg(userName)
                     .arg(password));
    DatabaseName = databaseName;
 if(db->open()) {
        return db;
    }
    else {
        return nullptr;
    }
}
int Database::selectRowCountResult(QSqlQuery* query)
{
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes){
        qDebug() << query->lastError().text();
        return -1;
    }

    int recordCount = 0;
    while (query->next())
    {
        qDebug() << "Field 1 : " << query->value(0).toString()
                 << "Field 2 : " << query->value(1).toString();
        ++recordCount;
    }

    return recordCount;
}

bool Database::execute(QSqlQuery* query){
    db->transaction();
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query->lastError().text();
        db->rollback();
        return false;
    }
    db->commit();
    return true;
}

void Database::insertData(const Contact &contact_){
    QSqlQuery query;
    if( !query.prepare(
    QString("INSERT INTO CONTACT( NAME, PHONENUMBER, EMAIL, DATEOFBIRTH, AGE) VALUES ( ?, ?, ?, ?, ?)") ))
    {
        qDebug() <<"Error = " << db->lastError().text();
        return ;
    }else {
        query.addBindValue(contact_.getContactName());
        query.addBindValue(contact_.getContactPhoneNumber());
        query.addBindValue(contact_.getContactEmail());
        query.addBindValue(contact_.getContactDateOfBirth());
        query.addBindValue(contact_.getContactAge());
    }
    bool result = execute(&query);
    if( result )
       qDebug() << "Successful insert";
    else
       qDebug() << "Insert failed";
}
void Database::updateData(const Contact &contact_){
    QSqlQuery query =  QSqlQuery(*db);
    query.setForwardOnly(true);
    if( !query.prepare(
    QString("UPDATE CONTACT SET NAME = ? "
            ",PHONENUMBER = ? "
            ",EMAIL = ? "
            ",DATEOFBIRTH = ? "
            ",AGE = ? "
            "WHERE ID = ?"
            ))){
        qDebug() <<"Error = " << db->lastError().text();
        return;
    }else {
        query.addBindValue(contact_.getContactName());
        query.addBindValue(contact_.getContactPhoneNumber());
        query.addBindValue(contact_.getContactEmail());
        query.addBindValue(contact_.getContactDateOfBirth());
        query.addBindValue(contact_.getContactAge());
        query.addBindValue(contact_.getContactID());
    }
    bool result = execute(&query);
    if( result )
       qDebug() << "Successful Update";
    else
       qDebug() << "Update failed";

}

void Database::deleteDataWhereIdIs(const int &ID){
    QSqlQuery query = QSqlQuery(*db);
    query.setForwardOnly(true);
    if( !query.prepare(QString("DELETE FROM CONTACT WHERE ID = ? ")) ){
        qDebug() << "Error = " << db->lastError().text();
        return;
    }else {
        query.addBindValue(ID);
    }
    bool result = execute(&query);
    if( result )
       qDebug() << "Successful delete";
    else
       qDebug() << "Delete failed";
}
void Database::disConnect()
{
    qDebug() << "Disconnected From Database!";
    db->close();
}

Contact Database::selectDataFromTableWhereIdIs(const int &ID){
    QSqlQuery query = QSqlQuery(*db);
    Contact contact_;
    query.setForwardOnly(true);
    if( !query.exec(QString("SELECT *FROM CONTACT WHERE ID = %1 ").arg(ID)) ){
        qDebug() << "Error = " << db->lastError().text();
    }else {
        while(query.next()){
            contact_.setContactID(query.value(INDEX_ID).toInt());
            contact_.setContactName(query.value(INDEX_NAME).toString());
            contact_.setContactPhoneNumber(query.value(INDEX_PHONE_NUMBER).toString());
            contact_.setContactEmail(query.value(INDEX_EMAIL).toString());
            contact_.setContactDateOfBirth(query.value(INDEX_DATE_OF_BIRTH).toDate());
            contact_.setContactAge(query.value(INDEX_AGE).toInt());
        }
    }
    return contact_;
}

QString Database::getMaxValueOfColumn(const QString &clolumn_name){
    QSqlQuery query = QSqlQuery(*db);
    QString max = "";
    query.setForwardOnly(true);
    if( !query.exec(QString("SELECT MAX(%1) FROM CONTACT").arg(clolumn_name)) ){
        qDebug() << "Error = " << db->lastError().text();
    }else{
        while(query.next()){
          max =  query.value(0).toString();
        }
    }
    return max;
}

void Database::orderBy(const QString &clolumn_name, const QString &sort_type){
    QSqlQuery query = QSqlQuery(*db);
    query.setForwardOnly(true);
    if( !query.exec(QString("SELECT *FROM CONTACT ORDER BY %1 %2").arg(clolumn_name).arg(sort_type)) ){
        qDebug() << "Error = " << db->lastError().text();
    }
}
