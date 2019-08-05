#include "addresswidget.h"


AddressWidget::AddressWidget()
{

}

void AddressWidget::readFromFile(const QString &fileName)
{
    QFile file(fileName);


}

void AddressWidget::writeToFile(const QString &pathFile){
    AddressBook *Addr = AddressBook::getInstance();
    QFile file(pathFile);

        if (!file.open(QIODevice::WriteOnly)) {
               // QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;

        }

        QDataStream out(&file);
        out << Addr->getContacts();
}
