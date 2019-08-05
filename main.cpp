#include "addressbook.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddressBook *w = AddressBook::GetInstance();

    w->show();

    return a.exec();
}
