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
    int contact_id;
public:
    Contact();
    QString getContactName()const;
    QString getContactPhoneNumber()const;
    QString getContactEmail()const;
    QDate getContactDateOfBirth()const;
    int getContactAge()const;
    int getContactID() const;

    void setContactName(const QString &name);
    void setContactPhoneNumber(const QString &phoneNumber);
    void setContactEmail(const QString &email);
    void setContactDateOfBirth(const QDate &date);
    void setContactAge(const int &age);
    void setContactID(const int &id);

    int CalculateAge();
};

class ContactModel : public QAbstractTableModel
{
    // Q_OBJECT

private:

  QList<Contact> myContact;

  static ContactModel *_instance;
  typedef bool (ContactModel::*FredMemFn)(int ,int );

  ContactModel();


public:

   static ContactModel * GetInstance();

   void setDataContact(const QList<Contact> &_myContact);

   const QList<Contact> &getMyContact() const;

   void editIndexDataContact(const int &index, const Contact &contact);
   void removeIndexDataContact(const int &index);
   void appendDataContact(const Contact &contact);


   int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
   int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

   Qt::ItemFlags flags(const QModelIndex &index) const override;
   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
   bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
   bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

};

#endif // CONTACT_H
