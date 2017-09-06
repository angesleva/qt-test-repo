#ifndef MONTHLYCONTRACT_H
#define MONTHLYCONTRACT_H

#include "basecontract.h"

class MonthlyContract: public BaseContract
{
    Q_INTERFACES(BaseContract)

public:
    MonthlyContract(QObject *parent);
    QList<QPair<QString,QString>> readContractDetails(QSqlRecord personRecord);
    QString writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee);
    QList<QPair<QString,QString>> readPaymentInputFields(QSqlRecord personRecord);
    QString writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee);
    bool deleteContract(QSqlRecord employee);


private:
    //A reference to the sales initiative table holding the specific data.
    QSqlRelationalTableModel* monthlyContractTableModel;
};

#endif // MONTHLYCONTRACT_H
