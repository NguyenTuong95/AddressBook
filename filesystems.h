#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#define PATH_FILE "C:/Users/TuongNV/Documents/AddressBook/dataAddressBook.txt"

#include <QFile>
#include <QTextStream>

#include "contact.h"

class Filesystems
{
private:

   QFile FileData;
   Contact *contact;
   ContactModel *contactModel;

public:

    Filesystems();
    ~Filesystems();
    void ReadDataFromFile();

    QList<Contact> getMyContact();

    void WriteDataToFile();


};

#endif // FILESYSTEMS_H
