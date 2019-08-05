#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "contact.h"
#include "filesystems.h"

#include <QMainWindow>

#include <QDateTime>
#include <QtWidgets>
#include <QString>

#include <QAbstractTableModel>

#include <QItemSelection>

#include <QTabWidget>

#include <QItemSelectionModel>


namespace Ui {
class AddressBook;
}

class AddressBook : public QMainWindow , protected Contact
{
    Q_OBJECT

private:
    static AddressBook * _instance;
    Ui::AddressBook *ui;
    Contact *contact;
    ContactModel  *contactModel;
    Filesystems *fileSystems;

    int CurrentRowsSelect;

    AddressBook(QWidget *parent = nullptr);
    ~AddressBook();


public:    

    enum Mode{
        InsertDataMode,
        EditDataMode
    };
    Mode CurrentMode;
     void init();
     void getDataFromSelectRowsInTable(Contact &outContact);
     void WiriteDataToFile();
     int getCurrentRowsSelect();

     static AddressBook * GetInstance();
     void DeleteInstance();
     bool nameExistInContact(const QString &contactName);

private slots:
    void onEnableBtnEditAndBtnRemote(QModelIndex index);

    void setStatusBtnFind();
    void clickAddButton();
    void clickEditButton();
    void clickRemoveButton();
    void on_pbtn_Edit_clicked();
    void clickFindButton();

};

#endif // ADDRESSBOOK_H
