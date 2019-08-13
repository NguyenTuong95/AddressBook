#include "addressbook.h"
#include "ui_addressbook.h"
#include "insertdatadialog.h"

#include <QDebug>

#define INDEX_COLUMN_ID                                 0
#define INDEX_COLUMN_NAME                               1
#define INDEX_COLUMN_PHONE_NUMBER                       2
#define INDEX_COLUMN_EMAIL                              3
#define INDEX_COLUMN_DATE_OF_BIRTH                      4
#define INDEX_COLUMN_AGE                                5


AddressBook::AddressBook(QWidget *parent) :
    QMainWindow(parent), Proxy(new QSortFilterProxyModel),
    ui(new Ui::AddressBook), fileSystems(new Filesystems),
    dbAddressBook(new Database("QODBC3"))
{
    ui->setupUi(this);
    db = dbAddressBook->connect("TUONGNV-D5", "AddressBook", "sa", "123");

    connect(ui->table_Contact, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableContactSeclectClicked(QModelIndex)));

    connect(ui->pbtn_Add,SIGNAL(clicked()), this, SLOT(clickAddButton()));
    connect(ui->pbtn_Remove, SIGNAL(clicked()), this, SLOT(clickRemoveButton()));
    connect(ui->pbtn_Find, SIGNAL(clicked()), this, SLOT(clickFindButton()));
    connect(ui->pbtn_Sort, SIGNAL(clicked()), this, SLOT(clickSortButton()));
    connect(ui->pbtn_Edit, SIGNAL(clicked()), this, SLOT(clickEditButton()));
    connect(ui->lnt_Find, SIGNAL(editingFinished()), this, SLOT(setStatusBtnFind()));
    contactModel = ContactModel::GetInstance();
    setupTabs();
    init();

}

void AddressBook::setupTabs(){

    //fileSystems->ReadDataFromFile();
    QList<Contact> listContact = readDataFromDatabaseToList();

    contactModel->setDataContact(listContact);
    Proxy->setSourceModel(contactModel);
    ui->table_Contact->setModel(Proxy);
    ui->table_Contact->resizeColumnsToContents();
    Proxy->setSortLocaleAware(true);
    ui->table_Contact->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->table_Contact->setEditTriggers(QAbstractItemView::DoubleClicked);
   // ui->table_Contact->setEditTriggers(QAbstractItemView::AnyKeyPressed);
}

AddressBook * AddressBook::_instance = nullptr;

AddressBook * AddressBook::GetInstance() {
    if (!_instance)
        _instance = new AddressBook;
    return _instance;
}

void AddressBook::init(){
    CurrentRowsSelect                                   = -1;
    CurrentColumnsSelect                                = -1;
    CurrentRowsDoubleClick                              = -1;
    CurrentColumnDoubleClick                            = -1;
    dataKeyPress                                        = "";
    isDoubleClick                                       = false;
    ui->pbtn_Edit->                                     setEnabled(false);
    ui->pbtn_Remove->                                   setEnabled(false);
    ui->pbtn_Sort->                                     setEnabled(false);
    setStatusBtnFind();
}

void AddressBook::onTableContactSeclectClicked(const QModelIndex &index){
  ui->pbtn_Edit->                                       setEnabled(true);
  ui->pbtn_Remove->                                     setEnabled(true);
  ui->pbtn_Sort->                                       setEnabled(true);
  CurrentRowsSelect                                     = index.row();
  CurrentColumnsSelect                                  = index.column();
  qDebug() << CurrentRowsSelect;
  if(CurrentRowsDoubleClick >= 0 && checkContact()){
      Contact contact_ = getDataFromRowsInTable(CurrentRowsDoubleClick);
      if(db == nullptr || !db->open()){
          qDebug()<< "error:"<< db->lastError().text();
      }else {
        dbAddressBook->updateData(contact_);
      }
      QMessageBox::information(this, tr("Success!"),
                       tr("Edit data done"));
  }
  CurrentRowsDoubleClick                                = -1;
  CurrentColumnDoubleClick                              = -1;
  dataKeyPress                                          = "";
  isDoubleClick                                         = false;
}

bool AddressBook::checkContact(){
    InsertDataDialog *dialog = new  InsertDataDialog;
    switch(CurrentColumnDoubleClick){
        case INDEX_COLUMN_NAME :
            if(!dialog->IsPersonName(dataKeyPress)){
                dialog->ErrorMessageBox(dialog->        InValidPersonNameCode);
                goto oldData;
            } else {return true;}
        case INDEX_COLUMN_PHONE_NUMBER:
            if(!dialog->IsPhoneNumber(dataKeyPress)){
                dialog->ErrorMessageBox(dialog->        InValidPhoneNumberCode);
                goto oldData;
            }else {return true;}
        case INDEX_COLUMN_EMAIL:
            if(!dialog->IsEmailAddress(dataKeyPress)){
                dialog->ErrorMessageBox(dialog->        InValidEmailCode);
                goto oldData;
            }else {return true;}
       case INDEX_COLUMN_DATE_OF_BIRTH:
            if(!dialog->IsValidDate(dataKeyPress)){
                dialog->ErrorMessageBox(dialog->        InValidDateCode);
                goto oldData;
            }else {return true;}
       case INDEX_COLUMN_AGE:
            if(!dialog->IsValidAge(dataKeyPress)){
                dialog->ErrorMessageBox(dialog->        InValidAgeCode);
                goto oldData;
            }else {return true;}
    }
oldData:{
    if((db == nullptr || !db->open())){
        qDebug()<< "error:"<< db->lastError().text();
        delete dialog;
        return false;
    }else{
        Contact contact_;
        int ID = contactModel->index(CurrentRowsDoubleClick, INDEX_COLUMN_ID).data().toInt();
        contact_ = dbAddressBook->selectDataFromTableWhereIdIs(ID);
        setDataContactToRows(CurrentRowsDoubleClick, contact_);
        db->close();
        delete dialog;
        return false;
    }
  }
}

QList<Contact> AddressBook::readDataFromDatabaseToList(){
    QList<Contact> outListContact;
    if(db == nullptr || !db->open()){
        qDebug()<< "error:"<< db->lastError().text();
    }
    else{
        Contact contact_;
        QSqlQuery query;
          if(query.exec("SELECT * FROM Contact")){
             while(query.next()){
                 contact_.setContactID(query.value(                                     INDEX_COLUMN_ID).toInt());
                 contact_.setContactName(query.value(                                   INDEX_COLUMN_NAME).toString());
                 contact_.setContactPhoneNumber(query.value(                            INDEX_COLUMN_PHONE_NUMBER).toString());
                 contact_.setContactEmail(query.value(                                  INDEX_COLUMN_EMAIL).toString());
                 contact_.setContactDateOfBirth(query.value(                            INDEX_COLUMN_DATE_OF_BIRTH).toDate());
                 contact_.setContactAge(query.value(                                    INDEX_COLUMN_AGE).toInt());
                 outListContact.append(contact_);
             }
          }
          else{
              qDebug()<< "error:"<< db->lastError().text();
          }
          db->close();
    }
   return outListContact;
}

void AddressBook::setStatusBtnFind(){
    QString LineEditText = ui->lnt_Find->text();
    if(LineEditText.isEmpty())
        ui->pbtn_Find->setEnabled(false);
    else
        ui->pbtn_Find->setEnabled(true);
}

void AddressBook::clickEditButton(){
    CurrentMode = EditDataMode;
    InsertDataDialog *dialog = new  InsertDataDialog;
    if(dialog->exec() == 1){
       Contact contact_;
        dialog->getDataContactChange(contact_);
        if((!contact_.getContactName().isEmpty()) &&
           (!contact_.getContactPhoneNumber().isEmpty()) &&
           (!contact_.getContactEmail().isEmpty())){
            int id = ui->table_Contact->model()->index(CurrentRowsSelect,                       INDEX_COLUMN_ID).data().toInt();
            contact_.setContactID(id);
            contactModel->editIndexDataContact(CurrentRowsSelect, contact_);
            ui->table_Contact->reset();
            ui->table_Contact->show();
            if(db != nullptr && db->open()){
                dbAddressBook->updateData(contact_);
            }else{
                qDebug()<< "error:"<< db->lastError().text();
            }
            db->close();
        }
    }
    delete dialog;
}

void AddressBook::setDataContactToRows(const int &indexRows, const Contact &contact_){

    QModelIndex index = contactModel->index(indexRows, INDEX_COLUMN_ID,     QModelIndex());
    contactModel->setData(index,contact_.getContactID(),                    Qt::EditRole);
    index = contactModel->index(indexRows, INDEX_COLUMN_NAME,               QModelIndex());
    contactModel->setData(index, contact_.getContactName(),                 Qt::EditRole);
    index = contactModel->index(indexRows, INDEX_COLUMN_PHONE_NUMBER,       QModelIndex());
    contactModel->setData(index, contact_.getContactPhoneNumber(),          Qt::EditRole);
    index = contactModel->index(indexRows, INDEX_COLUMN_EMAIL,              QModelIndex());
    contactModel->setData(index, contact_.getContactEmail(),                Qt::EditRole);
    index = contactModel->index(indexRows, INDEX_COLUMN_DATE_OF_BIRTH,      QModelIndex());
    contactModel->setData(index, contact_.getContactDateOfBirth(),          Qt::EditRole);
    index = contactModel->index(indexRows, INDEX_COLUMN_AGE,                QModelIndex());
    contactModel->setData(index, contact_.getContactAge(),                  Qt::EditRole);
}

void AddressBook::clickAddButton(){
    CurrentMode = InsertDataMode;
    InsertDataDialog *dialog = new  InsertDataDialog;
    if(dialog->exec() == 1){
        Contact contact_;
        dialog->getDataContactChange(contact_);
        if((!contact_.getContactName().isEmpty()) &&
            (!contact_.getContactPhoneNumber().isEmpty()) &&
            (!contact_.getContactEmail().isEmpty())){
            int numrows = contactModel->rowCount(                           QModelIndex());
            QList<Contact> outListContact = contactModel->getMyContact();
            contactModel->insertRows(numrows, 1,                            QModelIndex());
            if(db != nullptr && db->open()){
                dbAddressBook->insertData(contact_);
                int max_id = dbAddressBook->getMaxValueOfColumn("ID").toInt();
                contact_.setContactID(max_id);
            }else{
                qDebug()<< "error:"<< db->lastError().text();
            }
            db->close();
            setDataContactToRows(numrows, contact_);
            ui->table_Contact->resizeColumnsToContents();
            ui->table_Contact->reset();
            ui->table_Contact->show();

            //fileSystems->WriteDataToFile();

            init();
        }
    }
    delete dialog;
}

void AddressBook::clickRemoveButton(){
    int id = ui->table_Contact->model()->index(CurrentRowsSelect,                       INDEX_COLUMN_ID).data().toInt();
    contactModel->removeRows(CurrentRowsSelect, 1,                          QModelIndex());

    if(db != nullptr && db->open()){
        dbAddressBook->deleteDataWhereIdIs(id);
    }else{
        qDebug()<< "error:"<< db->lastError().text();
    }
    db->close();
    ui->table_Contact->reset();
    ui->table_Contact->show();
    //fileSystems->WriteDataToFile();
    init();
}

Contact AddressBook::getDataFromRowsInTable(const int &rows){
    Contact outContact;
    outContact.setContactID(contactModel->index(rows,                                   INDEX_COLUMN_ID).data().toInt());
    outContact.setContactName(contactModel->index(rows,                                 INDEX_COLUMN_NAME).data().toString());
    outContact.setContactPhoneNumber(contactModel->index(rows,                          INDEX_COLUMN_PHONE_NUMBER).data().toString());
    outContact.setContactEmail(contactModel->index(rows,                                INDEX_COLUMN_EMAIL).data().toString());
    outContact.setContactDateOfBirth(contactModel->index(rows,                          INDEX_COLUMN_DATE_OF_BIRTH).data().toDate());
    outContact.setContactAge(contactModel->index(rows,                                  INDEX_COLUMN_AGE).data().toInt());
    return outContact;
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

QList<Contact> AddressBook::readDataFromTableToList(){
    QList<Contact> outListContact;
    Contact contact_;
    int numRows = contactModel->rowCount(QModelIndex());
    for(int currentRow = 0; currentRow < numRows; ++currentRow){
       contact_.setContactID(ui->table_Contact->model()->index(currentRow,              INDEX_COLUMN_ID).data().toInt());
       contact_.setContactName(ui->table_Contact->model()->index(currentRow,            INDEX_COLUMN_NAME).data().toString());
       contact_.setContactPhoneNumber(ui->table_Contact->model()->index(currentRow,     INDEX_COLUMN_PHONE_NUMBER).data().toString());
       contact_.setContactEmail(ui->table_Contact->model()->index(currentRow,           INDEX_COLUMN_EMAIL).data().toString());
       contact_.setContactDateOfBirth(ui->table_Contact->model()->index(currentRow,     INDEX_COLUMN_DATE_OF_BIRTH).data().toDate());
       contact_.setContactAge(ui->table_Contact->model()->index(currentRow,             INDEX_COLUMN_AGE).data().toInt());
       outListContact.append(contact_);
    }
    return outListContact;
}

void AddressBook::clickSortButton(){    
    ui->table_Contact->setSortingEnabled(true);
    ui->table_Contact->sortByColumn(CurrentColumnsSelect, Qt::AscendingOrder);
    ui->table_Contact->reset();
    ui->table_Contact->show();
    QList<Contact> ListContact = readDataFromTableToList();
    for(int currentRow = 0; currentRow < ListContact.size(); ++currentRow){
        Contact contact_;
        contact_.setContactID(                                                          ListContact.at(currentRow).getContactID());
        contact_.setContactName(                                                        ListContact.at(currentRow).getContactName());
        contact_.setContactPhoneNumber(                                                 ListContact.at(currentRow).getContactPhoneNumber());
        contact_.setContactEmail(                                                       ListContact.at(currentRow).getContactEmail());
        contact_.setContactDateOfBirth(                                                 ListContact.at(currentRow).getContactDateOfBirth());
        contact_.setContactAge(                                                         ListContact.at(currentRow).getContactAge());
        setDataContactToRows(currentRow, contact_);
    }
    ui->table_Contact->setSortingEnabled(false);
    db->close();
    init();
}

int AddressBook::getCurrentRowsSelect()const{
    return CurrentRowsSelect;
}

void AddressBook::deleteInstance(){
    if (!_instance)
            delete _instance;
        _instance = nullptr;
}

void AddressBook::keyReleaseEvent(QKeyEvent *event){
    if(isDoubleClick){
       InsertDataDialog *dialog = new  InsertDataDialog;
       dataKeyPress +=  event->text();
       int length = dataKeyPress.length();
//        if(event->key() == Qt::Key_Delete ){
//            dataKeyPress.resize(0);
//        }
        if(event->key() == Qt::Key_Backspace){
            dataKeyPress.resize(length - 2);
        }
        length = dataKeyPress.length();
        bool isSpecicalCharacter = dialog->IsSpecicalCharacter(dataKeyPress);
        if(isSpecicalCharacter && length > 0){
            dataKeyPress                                = "";
            QMessageBox::information(this, tr("Special Characters!"),
                                     tr("Please enter diferent [!#$%^&*[](),?\":{}|<>]"));
            //dataKeyPress.resize(length - 1);
            if(db == nullptr || !db->open()){
                qDebug()<< "error:"<< db->lastError().text();
            }else{
                Contact contact_;
                int ID = contactModel->index(CurrentRowsDoubleClick, INDEX_COLUMN_ID).data().toInt();
                contact_ = dbAddressBook->selectDataFromTableWhereIdIs(ID);
                setDataContactToRows(CurrentRowsDoubleClick, contact_);
                CurrentColumnDoubleClick                = -1;
                CurrentRowsDoubleClick                  = -1;
            }
            db->close();
        }
        qDebug() << dataKeyPress;
        delete dialog;
    }
    return;
}

AddressBook::~AddressBook(){
    delete ui;
    deleteInstance();
    delete contact;
    delete fileSystems;
    delete contactModel;
    delete Proxy;
    delete dbAddressBook;
    delete myLabelText;
}

void AddressBook::on_table_Contact_doubleClicked(const QModelIndex &index)
{
    CurrentColumnDoubleClick = index.column();
    CurrentRowsDoubleClick = index.row();
    isDoubleClick = true;
}
