#ifndef CONTACT_H
#define CONTACT_H
#include <QString>
#include <QDate>
#include <QAbstractTableModel>

class Contact{

private:
    QString contact_name;
    QString contact_phone_number;
    QString contact_email;
    QDate contact_date_of_birth;
    int contact_age;
public:
    Contact();
    QString getContactName()const;
    QString getContactPhoneNumber()const;
    QString getContactEmail()const;
    QDate getContactDateOfBirth()const;
    int getContactAge()const;

    void setContactName(const QString &name);
    void setContactPhoneNumber(const QString &phoneNumber);
    void setContactEmail(const QString &email);
    void setContactDateOfBirth(const QDate &date);
    void setContactAge(const int &age);
    int CalculateAge();
};

class ContactModel : public QAbstractTableModel, public Contact
{
    // Q_OBJECT

private:

  QList<Contact> myContact;

  static ContactModel *_instance;
  ContactModel();

public:

    static ContactModel * GetInstance();

    void setDataContact(const QList<Contact> *&_myContact);

    QList<Contact> getMyContact();

    void editIndexDataContact(const int &index, const Contact &contact);
    void removeIndexDataContact(const int &index);
    void appendDataContact(const Contact &contact);
    void sortDataByName();


    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;  
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

};

#endif // CONTACT_H
