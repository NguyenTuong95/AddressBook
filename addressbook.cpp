#include "addressbook.h"
#include "ui_addressbook.h"
#include "insertdatadialog.h"

#include <QDebug>
#define INDEX_COLUMN_NAME                       0
#define INDEX_COLUMN_PHONE_NUMBER               1
#define INDEX_COLUMN_EMAIL                      2
#define INDEX_COLUMN_DATE_OF_BIRTH              3


AddressBook::AddressBook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddressBook), fileSystems(new Filesystems)
{
    ui->setupUi(this);

    QObject::connect(ui->table_Contact, SIGNAL(clicked(QModelIndex)), this, SLOT(onEnableBtnEditAndBtnRemote(QModelIndex)));
    connect(ui->pbtn_Add,SIGNAL(clicked()), this, SLOT(clickAddButton()));
    connect(ui->pbtn_Remove, SIGNAL(clicked()), this, SLOT(clickRemoveButton()));
    connect(ui->pbtn_Find, SIGNAL(clicked()), this, SLOT(clickFindButton()));

    QObject::connect(ui->lnt_Find, SIGNAL(editingFinished()), this, SLOT(setStatusBtnFind()));
    contactModel = ContactModel::GetInstance();

    fileSystems->ReadDataFromFile();
    ui->table_Contact->setModel(contactModel);
    ui->table_Contact->show();
    init();
}

AddressBook * AddressBook::_instance = nullptr;

AddressBook * AddressBook::GetInstance() {
    if (!_instance)
        _instance = new AddressBook;
    return _instance;
}

void AddressBook::init(){
   // ui->pbtn_Add->setEnabled(true);
    CurrentRowsSelect = -1;
    ui->pbtn_Edit->setEnabled(false);
    ui->pbtn_Remove->setEnabled(false);
    setStatusBtnFind();
}

void AddressBook::onEnableBtnEditAndBtnRemote(QModelIndex index){
  ui->pbtn_Edit->setEnabled(true);
  ui->pbtn_Remove->setEnabled(true);
  const QModelIndexList  indexes = ui->table_Contact->selectionModel()->selectedRows();
  CurrentRowsSelect = index.row();
}

void AddressBook::setStatusBtnFind(){
    QString LineEditText = ui->lnt_Find->text();
    if(LineEditText.isEmpty())
        ui->pbtn_Find->setEnabled(false);
    else
        ui->pbtn_Find->setEnabled(true);
}
void AddressBook::clickEditButton(){
    InsertDataDialog *dialog = new  InsertDataDialog;
    if(dialog->exec() == 1){
       Contact contact_;
        dialog->getDataContactChange(contact_);
        if((!contact_.getContactName().isEmpty()) && (!contact_.getContactPhoneNumber().isEmpty()) && (!contact_.getContactEmail().isEmpty())){
            contactModel->editIndexDataContact(CurrentRowsSelect, contact_);
            fileSystems->WriteDataToFile();

            ui->table_Contact->reset();

            ui->table_Contact->setModel(contactModel);
            ui->table_Contact->reset();
            ui->table_Contact->show();
            fileSystems->WriteDataToFile();
        }
    }
    delete dialog;
}

void AddressBook::clickAddButton(){
    CurrentMode = InsertDataMode;
    InsertDataDialog *dialog = new  InsertDataDialog;
    if(dialog->exec() == 1){
        Contact contact_;
        dialog->getDataContactChange(contact_);
        if((!contact_.getContactName().isEmpty()) && (!contact_.getContactPhoneNumber().isEmpty()) && (!contact_.getContactEmail().isEmpty())){
            contactModel->appendDataContact(contact_);
            ui->table_Contact->setModel(contactModel);
            ui->table_Contact->reset();
            ui->table_Contact->show();
            fileSystems->WriteDataToFile();
            init();
        }
    }
    delete dialog;
}
void AddressBook::clickRemoveButton(){
    contactModel->removeIndexDataContact(CurrentRowsSelect);
    ui->table_Contact->reset();
    ui->table_Contact->setModel(contactModel);
    ui->table_Contact->show();
    fileSystems->WriteDataToFile();
    init();
}

void AddressBook::getDataFromSelectRowsInTable(Contact &outContact){

    outContact.setContactName(contactModel->index(CurrentRowsSelect, INDEX_COLUMN_NAME).data().toString());
    outContact.setContactPhoneNumber(contactModel->index(CurrentRowsSelect, INDEX_COLUMN_PHONE_NUMBER).data().toString());
    outContact.setContactEmail(contactModel->index(CurrentRowsSelect, INDEX_COLUMN_EMAIL).data().toString());
    outContact.setContactDateOfBirth(contactModel->index(CurrentRowsSelect, INDEX_COLUMN_DATE_OF_BIRTH).data().toDate());
}

bool AddressBook::nameExistInContact(const QString &contactName){
    QList<Contact> listContact = contactModel->getMyContact();
    int length = listContact.size();
    if(contactName.isEmpty())
        return false;

    for(int i = 0; i < length; ++i){
        if(listContact.at(i).getContactName() == contactName)
            return true;
    }
    return false;
}
void AddressBook::clickFindButton(){

    QString LineEditText = ui->lnt_Find->text();

    if(nameExistInContact(LineEditText)){
        QList<Contact> listContact = contactModel->getMyContact();
        for(int i = 0; i < listContact.size(); ++i){
            if(listContact.at(i).getContactName() == LineEditText){
                ui->table_Contact->selectRow(i);
            }
        }
    }
    else{
        QMessageBox::information(this, tr("Name doesn't Exist"),
                         tr("Sorry, The name \"%1\" doesn't exist, Please Enter name exist.").arg(LineEditText));
                     return;
    }
}

int AddressBook::getCurrentRowsSelect(){
    return CurrentRowsSelect;
}

void AddressBook::DeleteInstance(){
    if (!_instance)
            delete _instance;
        _instance = nullptr;
}
AddressBook::~AddressBook(){
    delete ui;
    DeleteInstance();
    delete contact;
    delete fileSystems;
    delete contactModel;
}

void AddressBook::on_pbtn_Edit_clicked()
{
    CurrentMode = EditDataMode;
    clickEditButton();
}
