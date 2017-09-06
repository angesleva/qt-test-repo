#ifndef HOURLYCONTRACT_H
#define HOURLYCONTRACT_H

#include "basecontract.h"

class HourlyContract: public BaseContract
{
    Q_INTERFACES(BaseContract)

public:
    HourlyContract(QObject *parent);
    QList<QPair<QString,QString>> readContractDetails(QSqlRecord personRecord);
    QString writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee);
    QList<QPair<QString,QString>> readPaymentInputFields(QSqlRecord personRecord);
    QString writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee);
    bool deleteContract(QSqlRecord employee);
    QList<QString> readEditableFieldTypes();

private:
    //A reference to the sales initiative table holding the specific data.
    QSqlRelationalTableModel* hourlyContractTableModel;
};

#endif // HOURLYCONTRACT_H
