#include "filesystems.h"
#include <QDateTime>

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
                    contact_.setContactName(list.at(0));
                    contact_.setContactPhoneNumber(list.at(1));
                    contact_.setContactEmail(list.at(2));
                    contact_.setContactDateOfBirth(QDate::fromString(list.at(3), "yyyyMMdd"));
                    contact_.CalculateAge();
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
