#ifndef INSERTDATADIALOG_H
#define INSERTDATADIALOG_H
#include <QtWidgets>
#include <QDialog>
#include "contact.h"

namespace Ui {
class InsertDataDialog;
}

class InsertDataDialog : public QDialog, protected Contact
{
    Q_OBJECT

public:
    explicit InsertDataDialog(QWidget *parent = nullptr);
    ~InsertDataDialog();
private slots:
    void clickOKButton();
    void on_pbtn_OK_clicked();

private:

    Ui::InsertDataDialog *ui; 

    Contact *contact;
    ContactModel *contactModel;

public:
    enum ErrorCode{
        EmptyCode,
        InValidPersonNameCode,
        InValidEmailCode,
        InValidNumberCode,
        InValidDateCode
    };
    ErrorCode currentErrorCode;

    void getDataContactChange(Contact &outContact);

    bool IsPhoneNumber(const QString &str);
    bool IsPersonName(const QString &str);
    bool IsCharacter(const QString &str);
    bool IsEmailAddress(const QString &str);
    bool IsValidDate(const QDate &date);
    void ErrorMessageBox(ErrorCode errorCode);
};

#endif // INSERTDATADIALOG_H
