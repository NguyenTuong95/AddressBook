#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H
#include "database.h"
#include "contact.h"

#include "filesystems.h"
#include <QSortFilterProxyModel>
#include <QMainWindow>
#include <QDateTime>
#include <QtWidgets>
#include <QString>
#include <QAbstractTableModel>
#include <QItemSelection>
#include <QTabWidget>



namespace Ui {
    class AddressBook;
}


class AddressBook : public QMainWindow
{
    Q_OBJECT

private:
    QSortFilterProxyModel *Proxy;
    static AddressBook * _instance;
    Ui::AddressBook *ui;

    Contact *contact;
    ContactModel  *contactModel;
    Filesystems *fileSystems;
    Database *dbAddressBook;
    QSqlDatabase *db;

    int CurrentRowsSelect;
    int CurrentColumnsSelect;
    int CurrentRowsDoubleClick;
    int CurrentColumnDoubleClick;
    bool isDoubleClick;
    QString dataKeyPress;
    AddressBook(QWidget *parent = nullptr);
    ~AddressBook();

    void keyReleaseEvent(QKeyEvent *event);

     QLabel *myLabelText;

public:

    enum Mode{
         InsertDataMode,
         EditDataMode
     };
     Mode CurrentMode;

     void init();

     Contact getDataFromRowsInTable(const int &rows);

     int getCurrentRowsSelect()const;
     void setDataContactToRows(const int &indexRows, const Contact &contact_);
     bool nameExistInContact(const QString &contactName);
     static AddressBook * GetInstance();
     void deleteInstance();
     void setupTabs();
     bool checkContact();
     QList<Contact> readDataFromTableToList();
     QList<Contact> readDataFromDatabaseToList();

private slots:

    void onTableContactSeclectClicked(const QModelIndex &index);

    void setStatusBtnFind();
    void clickAddButton();
    void clickEditButton();
    void clickRemoveButton();

    void clickFindButton();
    void clickSortButton();

    void on_table_Contact_doubleClicked(const QModelIndex &index);
};

#endif // ADDRESSBOOK_H
