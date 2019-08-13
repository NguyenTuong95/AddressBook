#include "contact.h"
#include <QtAlgorithms>

#define INDEX_ID                        0
#define INDEX_NAME                      1
#define INDEX_PHONE_NUMBER              2
#define INDEX_EMAIL                     3
#define INDEX_DATE_OF_BIRTH             4
#define INDEX_AGE                       5

Contact::Contact(){
    contact_name = "";
    contact_phone_number = "";
    contact_email = "";
    contact_age = 1;
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
int Contact::getContactID() const{
    return contact_id;
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
void Contact::setContactID(const int &id){
    contact_id = id;
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

void ContactModel::setDataContact(const QList<Contact> &_myContact){
    if(!_myContact.isEmpty()){
        myContact.clear();
        myContact = _myContact;
    }
    return;
}
void ContactModel::editIndexDataContact( const int &index,  const Contact &contact){
    myContact[index].setContactName(contact.getContactName());
    myContact[index].setContactEmail(contact.getContactEmail());
    myContact[index].setContactPhoneNumber(contact.getContactPhoneNumber());
    myContact[index].setContactDateOfBirth(contact.getContactDateOfBirth());
    myContact[index].setContactAge(contact.getContactAge());
    return;
}
void ContactModel::removeIndexDataContact(const int &index){
    myContact.removeAt(index);
    return;
}
void ContactModel::appendDataContact(const Contact &contact){
    myContact.append(contact);
    return;
}

const QList<Contact> &ContactModel::getMyContact() const{
    return myContact;
}
int ContactModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    if(parent.isValid()) return 0;
    return myContact.length();
}
int ContactModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    if(parent.isValid()) return 0;
        return 6;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || role != Qt::DisplayRole) {   return QVariant();
    }

    if (role == Qt::DisplayRole) {
            const auto &contact = myContact.at(index.row());
            switch (index.column()) {
                case INDEX_ID:
                    return contact.getContactID();
                case INDEX_NAME:
                    return contact.getContactName();
                case INDEX_PHONE_NUMBER:
                    return contact.getContactPhoneNumber();
                case INDEX_EMAIL:
                    return contact.getContactEmail();
                case INDEX_DATE_OF_BIRTH:
                    return contact.getContactDateOfBirth();
                case INDEX_AGE:
                    return  contact.getContactAge();
                default:
                    return QVariant();
            }
        }
    else if (role == Qt::TextAlignmentRole){
        if (index.column() == 0)
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
            case INDEX_ID:
                return tr("ID");
            case INDEX_NAME:
               return tr("NAME");
            case INDEX_PHONE_NUMBER:
               return tr("PHONENUMBER");
            case INDEX_EMAIL:
               return tr("EMAIL");
            case INDEX_DATE_OF_BIRTH:
               return tr("DATEOFBIRTH");
            case INDEX_AGE:
               return tr("AGE");
            default:
                 return QVariant();
        }
    }
    return section + 1;
   // return QVariant();
}

bool ContactModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    beginInsertRows(QModelIndex(), position, position + rows - 1);

  //  Contact contact;

    for (int row = 0; row < rows; ++row)
        myContact.insert(position, Contact());

    endInsertRows();
    return true;
}

bool ContactModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        myContact.removeAt(position);

    endRemoveRows();
    return true;
}

bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        auto contact = myContact.at(index.row());

        switch (index.column()) {
            case INDEX_ID:
                 contact.setContactID(value.toInt());
                break;
            case INDEX_NAME:
                contact.setContactName(value.toString());
                break;
            case INDEX_PHONE_NUMBER:
                contact.setContactPhoneNumber(value.toString());
                break;
            case INDEX_EMAIL:
                contact.setContactEmail(value.toString());
                break;
            case INDEX_DATE_OF_BIRTH:
                contact.setContactDateOfBirth(value.toDate());
                break;
            case INDEX_AGE:
                 contact.setContactAge(value.toInt());
                 break;
            default:
                return false;
        }

        myContact.replace(row, contact);

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
