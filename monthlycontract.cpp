#include "monthlycontract.h"

MonthlyContract::MonthlyContract(QObject *parent) : BaseContract(parent)
{
//No contract table exists for this type of contract, so this is just an empty
//constructor ensuring the parent relationship.
}

QList<QPair<QString,QString>> MonthlyContract::readContractDetails(QSqlRecord personRecord)
{
    //Just the base salary info is required for monthly contract.

    QPair<QString,QString> baseSalary;
    baseSalary.first = "Salary / Month:";
    baseSalary.second = personRecord.value("baseSalary").toString();
    contractDetailList.insert(contractDetailList.count(),baseSalary);

return contractDetailList;
}

QString MonthlyContract::writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee)
{
//Just the baseSalary field of the persons table needs to be updated in case of a monthly contract.

double baseSalary = 0;
QString resultMessage = "OK";

//1. Check the validity of the input
for (int i = 0; i < contractDetails.count(); i++) {
    bool isNumber;
    if (QString::compare(contractDetails.at(i).first, "Salary / Month:")==0)
    {
        baseSalary = contractDetails.at(i).second.toDouble(&isNumber);
        if(isNumber)//This was a double
        {
            baseSalary = (QString::number(baseSalary,'f',2)).toDouble();
            if(baseSalary < 0 || baseSalary > 10000)
            {
                resultMessage = tr("Base salary has to be defined as a number between 1 and 10000");
                break;
            }
        }
        else
        {
            resultMessage = tr("Base salary value format error. Please provide only decimal numbers");
            break;
        }

    }

    //2. If input was valid, the actual saving or updating can take place.
    if(QString::compare(resultMessage,"OK",Qt::CaseInsensitive)==0)
    {
        //2.1 Update the baseSalary of the existing record.
        QSqlQuery query;
        QString sqlQuery = QString("UPDATE %1 SET baseSalary=:baseSalary WHERE id=:id").arg("persons");
        query.prepare(sqlQuery);
        query.bindValue(":baseSalary", baseSalary);
        query.bindValue(":id", employee.value("id").toInt());
        query.exec();
    }

}
return resultMessage;

}

QList<QPair<QString,QString>> MonthlyContract::readPaymentInputFields(QSqlRecord personRecord)
{
    //Monthly contract requires no extra input for calculating the salary, so an empty table personsModel is returned.
    return paymentInfoList;
}
QString MonthlyContract::writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee)
{
    //A paid salary record will be created for a specific Month and year, if not already existing.

    int month, year;
    QString resultMessage = "OK";

    //1. Month and year check as normally.
    QString monthAndYearCheckResult = checkMonthAndYearInput(paymentInput, &month, &year);
    if(QString::compare(monthAndYearCheckResult,"OK",Qt::CaseInsensitive)!=0)return monthAndYearCheckResult;


    //2. If the input was valid, the actual saving of the payment record can take place.
    if(QString::compare(resultMessage,"OK",Qt::CaseInsensitive)==0)
    {
        resultMessage = savePaymentRecord(year,month,employee.value("baseSalary").toDouble(),employee);
    }
    return resultMessage;
}

bool MonthlyContract::deleteContract(QSqlRecord employee)
{
    //There's actually nothing to delete, as the montly contract only contains
    //The Salary / Month that is a field of the persons table.
    return true;
}
