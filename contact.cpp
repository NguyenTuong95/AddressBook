#include "contact.h"
#include <QtAlgorithms>

Contact::Contact(){
    contact_name = "";
    contact_phone_number = "";
    contact_email = "";
    contact_age = 0;
}
QString Contact::getContactName() const{
    return contact_name;
}
QString Contact::getContactEmail()const{
    return contact_email;
}
QString Contact::getContactPhoneNumber()const{
    return contact_phone_number;
}
QDate Contact::getContactDateOfBirth()const {
    return contact_date_of_birth;
}
int Contact::getContactAge()const{
    return contact_age;
}
void Contact::setContactAge(const int &age){
    contact_age = age;
}
void Contact::setContactName(const QString &name){
    contact_name = name;
}
void Contact::setContactEmail(const QString &email){
    contact_email = email;
}
void Contact::setContactDateOfBirth(const QDate &date){
    contact_date_of_birth = date;
}
void Contact::setContactPhoneNumber(const QString &phoneNumber){
    contact_phone_number = phoneNumber;
}

int Contact::CalculateAge(){
   QDate currentDate = QDate::currentDate();
   int currentAge = currentDate.year() -  contact_date_of_birth.year();
   if((contact_date_of_birth.month() > currentDate.month()) ||
       (contact_date_of_birth.month() == currentDate.month() &&
        contact_date_of_birth.day() > currentDate.day())){
       --currentAge;
   }
   if(currentAge <= 0)
       currentAge = 1;
   contact_age = currentAge;
   return currentAge;
}
ContactModel::ContactModel()
{
}

ContactModel * ContactModel::_instance = nullptr;

ContactModel * ContactModel::GetInstance() {
    if (!_instance)
        _instance = new ContactModel;
    return _instance;
}

void ContactModel::setDataContact(const QList<Contact> *&_myContact){
    if(!_myContact->isEmpty()){
       // myContact = _myContact;
    }
    return;
}
void ContactModel::editIndexDataContact( const int &index,  const Contact &contact){
    myContact[index].setContactName(contact.getContactName());
    myContact[index].setContactEmail(contact.getContactEmail());
    myContact[index].setContactPhoneNumber(contact.getContactPhoneNumber());
    myContact[index].setContactDateOfBirth(contact.getContactDateOfBirth());
    myContact[index].CalculateAge();
    //myContact[index].
    return;
}
void ContactModel::removeIndexDataContact(const int &index){
    myContact.removeAt(index);
    return;
}

void ContactModel::sortDataByName(){
    int lenght = myContact.size();
    for(int i = 0; i < lenght - 1; ++i)
        for(int j = 0; j < lenght - i - 1; ++j){
            if(myContact.at(j).getContactName().toLower() > myContact.at(j+1).getContactName().toLower() ){
               myContact.swapItemsAt(j, j+1);
            }
        }
}

void ContactModel::appendDataContact(const Contact &contact){
    myContact.append(contact);
    sortDataByName();
    return;
}

QList<Contact> ContactModel::getMyContact(){

    return myContact;
}
int ContactModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return myContact.length();
}
int ContactModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
        return 5;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || role != Qt::DisplayRole) {   return QVariant();
    }
    if (index.column() == 0) {
          return myContact.at(index.row()).getContactName();
        } else if (index.column() == 1) {
            return myContact.at(index.row()).getContactPhoneNumber();
        } else if(index.column() == 2){
            return myContact.at(index.row()).getContactEmail();
        } else if(index.column() == 3){
            return myContact.at(index.row()).getContactDateOfBirth();
        } else if(index.column() == 4){
            return myContact[index.row()].getContactAge();
    }
    return QVariant();
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Name");
        } else if (section == 1) {
           return QString("Phone");
        }   else if(section == 2){
            return QString("Email");
        }else if(section == 3){
            return QString("Date Of Birth");
        }else if(section == 4){
            return QString("Age");
        }
    }
    return QVariant();
}

