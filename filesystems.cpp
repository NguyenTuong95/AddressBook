#include "filesystems.h"
#include <QDateTime>

#define INDEX_ID                        0
#define INDEX_NAME                      1
#define INDEX_PHONE_NUMBER              2
#define INDEX_EMAIL                     3
#define INDEX_DATE_OF_BIRTH             4
#define INDEX_AGE                       5

Filesystems::Filesystems(): FileData(PATH_FILE), contact(new Contact)
{
    contactModel = ContactModel::GetInstance();
}
void Filesystems::ReadDataFromFile(){

    if (FileData.open(QFile::ReadOnly)) {
        QTextStream stream(&FileData);

        QString line;// = stream.readLine();
        QStringList list;// = line.simplified().split(',');
        int MinSizeList = 4;
        while (!stream.atEnd()) {
            line = stream.readLine();
            if (!line.contains('#') && line.contains(',') && !line.isEmpty()) {
                list = line.simplified().split(',');
                if(list.size() >= MinSizeList){
                    Contact contact_;
                    contact_.setContactName(list.at(INDEX_NAME));
                    contact_.setContactPhoneNumber(list.at(INDEX_PHONE_NUMBER));
                    contact_.setContactEmail(list.at(INDEX_EMAIL));
                    contact_.setContactDateOfBirth(QDate::fromString(list.at(INDEX_DATE_OF_BIRTH), "yyyyMMdd"));
                    contact_.setContactAge(list.at(INDEX_AGE).toInt());
                    contactModel->appendDataContact(contact_);
                }
            }
        }
    }
    FileData.close();
}

void Filesystems::WriteDataToFile(){

    if(FileData.open(QIODevice::WriteOnly | QIODevice::Text)){
        QList<Contact> myContact_ = contactModel->getMyContact();
        QTextStream in(&FileData);
        for(int i = 0; i < myContact_.size(); ++i){
            in << myContact_.at(i).getContactName()   << "," <<   myContact_.at(i).getContactPhoneNumber() << ","
              << myContact_.at(i).getContactEmail()   << "," <<   myContact_.at(i).getContactDateOfBirth().toString("yyyyMMdd") << "\n";
        }
    }
    FileData.flush();
    FileData.close();
}
Filesystems::~Filesystems(){
    delete  contact;
    delete contactModel;
}
