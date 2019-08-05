#include "insertdatadialog.h"
#include "ui_insertdatadialog.h"
#include "addressbook.h"

#include <QRegularExpression>

InsertDataDialog::InsertDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertDataDialog), contact(new Contact)
{
    ui->setupUi(this);
    contactModel = ContactModel::GetInstance();

    AddressBook *mInstance = AddressBook::GetInstance();

    if(mInstance->CurrentMode == mInstance->EditDataMode){
        this->setWindowTitle("Edit Data Contact");
        Contact contact_;
        mInstance->getDataFromSelectRowsInTable(contact_);

        ui->lnt_Name->setText(contact_.getContactName());
        ui->lnt_PhoneNumber->setText(contact_.getContactPhoneNumber());
        ui->lnt_Email->setText(contact_.getContactEmail());
        ui->DateOfBirht->setDate(contact_.getContactDateOfBirth());
    }else if(mInstance->CurrentMode == mInstance->InsertDataMode){
        this->setWindowTitle("Insert Data Contact");
        ui->lnt_Name->setText("");
        ui->lnt_PhoneNumber->setText("");
        ui->lnt_Email->setText("");
    }

}

InsertDataDialog::~InsertDataDialog()
{
    delete ui;
    delete contact;
}
void InsertDataDialog::clickOKButton(){
    QString textName = ui->lnt_Name->text();
    QString textPhoneNumber = ui->lnt_PhoneNumber->text();
    QString textEmail = ui->lnt_Email->text();
    QDate DateOfBirht = ui->DateOfBirht->date();

    if(textName.isEmpty() || textPhoneNumber.isEmpty() || textEmail.isEmpty()){
        ErrorMessageBox(EmptyCode);
        return;
    }else if(!IsPersonName(textName)){
        ErrorMessageBox(InValidPersonNameCode);
        return;
    }else if(!IsPhoneNumber(textPhoneNumber)){
        ErrorMessageBox(InValidNumberCode);
        return;
    }else if(!IsEmailAddress(textEmail)){
        ErrorMessageBox(InValidEmailCode);
        return;
    }else if(!DateOfBirht.isValid()){

    }else{
        contact->setContactName(textName);
        contact->setContactPhoneNumber(textPhoneNumber);
        contact->setContactEmail(textEmail);
        contact->setContactDateOfBirth(DateOfBirht);
        hide();
    }
    ui->lnt_Name->clear();
    ui->lnt_PhoneNumber->clear();
    ui->lnt_Email->clear();
}

void InsertDataDialog::ErrorMessageBox(ErrorCode errorCode){
    currentErrorCode = errorCode;
    switch (currentErrorCode) {
        case EmptyCode :
            QMessageBox::information(this, tr("Empty Field"),
                                     tr("Please enter a name, phone number and email."));


            break;
        case InValidPersonNameCode:

            QMessageBox::information(this, tr("Invalid Person Name"),
                                 tr("Please try again."));
            break;
        case InValidEmailCode :
            QMessageBox::information(this, tr("Invalid Email Address"),
                                     tr("Please try again."));
            break;

        case InValidNumberCode :
            QMessageBox::information(this, tr("Invalid Phone Number"),
                                     tr("Please try again."));
            break;
        case InValidDateCode:
            QMessageBox::information(this, tr("Invalid Date Of Birth"),
                                     tr("Please try again."));
            break;
    }
}

void InsertDataDialog::getDataContactChange(Contact &outContact){
      outContact.setContactName(contact->getContactName());
      outContact.setContactPhoneNumber(contact->getContactPhoneNumber());
      outContact.setContactEmail(contact->getContactEmail());
      outContact.setContactDateOfBirth(contact->getContactDateOfBirth());
}

bool InsertDataDialog::IsPhoneNumber(const QString &str){

    QRegularExpression re("(09|0[2|3|6|8|9])+([0-9]{8})\b");
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}
bool InsertDataDialog::IsEmailAddress(const QString &str){
    QRegularExpression re("^[a-z][a-z0-9_\.]{5,32}@[a-z0-9]{2,}(\.[a-z0-9]{2,4}){1,2}$");
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}
bool InsertDataDialog::IsPersonName(const QString &str){
    QRegularExpression re("^[a-zA-Z]+(([',. -][a-zA-Z ])?[a-zA-Z]*)*$");
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}
bool InsertDataDialog::IsCharacter(const QString &str){
    return ( (str >= 'a' && str <= 'z') || (str >= 'A' && str <= 'Z'));
}
void InsertDataDialog::on_pbtn_OK_clicked()
{
    clickOKButton();
    accept();
}
