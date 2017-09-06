#ifndef SALESCONTRACT_H
#define SALESCONTRACT_H

#include "basecontract.h"

class SalesContract: public BaseContract
{
Q_INTERFACES(BaseContract)

public:
    SalesContract(QObject *parent);
    QList<QPair<QString,QString>> readContractDetails(QSqlRecord personRecord);
    QString writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee);
    QList<QPair<QString,QString>> readPaymentInputFields(QSqlRecord personRecord);
    QString writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee);
    bool deleteContract(QSqlRecord employee);

private:
    QSqlRecord findSalesContract(QSqlRecord personRecord);
    //A reference to the sales initiative table holding the specific data.
    QSqlRelationalTableModel* salesContractTableModel;
};

#endif // SALESCONTRACT_H
