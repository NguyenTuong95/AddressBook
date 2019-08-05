#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H
#include <addressbook.h>


class AddressWidget
{
public:
    AddressWidget();
    void readFromFile(const QString &pathFile);
    void writeToFile(const QString &pathFile);

};

#endif // ADDRESSWIDGET_H
