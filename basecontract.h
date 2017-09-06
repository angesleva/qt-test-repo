#ifndef BASECONTRACT_H
#define BASECONTRACT_H

#include <QString>
#include <QtSql>


class BaseContract: public QObject
{
Q_OBJECT
public:
    // returns the details as a list of string pairs and hiding the actual implementation.
    BaseContract(QObject* parent);
    virtual ~BaseContract() {}
    //The idea here is to provide the contract details as a key, value pair of two strings.
    //The UI can then take care of displaying this info, so the appearance won't slip into
    //the factory classes.
    virtual QList<QPair<QString,QString>> readContractDetails(QSqlRecord personRecord)=0;
    virtual QString writeContractDetails(QList<QPair<QString,QString>> contractDetails, QSqlRecord employee)=0;
    virtual QList<QPair<QString,QString>> readPaymentInputFields(QSqlRecord personRecord)=0;
    virtual QString writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee)=0;
    virtual bool deleteContract(QSqlRecord employee) = 0;
    static BaseContract* Create(QString contractType, QObject *factory);
    //This method is common to all contract types
    QString checkMonthAndYearInput(QList<QPair<QString,QString>> paymentInput, int* month, int* year);
    QString savePaymentRecord(int year, int month, double payment, QSqlRecord person);


protected:
    //Using class variables, although they are not necessary in this case.
    QList<QPair<QString,QString>> contractDetailList;
    QList<QPair<QString,QString>> paymentInfoList;
    QSqlRelationalTableModel* paidSalaryModel;
};

Q_DECLARE_INTERFACE(BaseContract,"BaseContract Ilari 1.0")




#endif // BASECONTRACT_H

