#include "hourlycontract.h"

HourlyContract::HourlyContract(QObject *parent) : BaseContract(parent)
{
    //No contract table exists for this type of contract as the hourly contract
    //uses the baseSalary field of the persons table, so this is just an empty
    //constructor ensuring the parent relationship.
}

QList<QPair<QString,QString>> HourlyContract::readContractDetails(QSqlRecord personRecord)
{
    //Just the base salary info is required.
    QPair<QString,QString> baseSalary;
    baseSalary.first = "Salary / hour:";
    baseSalary.second = personRecord.value("baseSalary").toString();
    contractDetailList.insert(contractDetailList.count(),baseSalary);

    return contractDetailList;
}

QString HourlyContract::writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee)
{
    //Just the baseSalary field of the persons table needs to be updated.
    double baseSalary = 0;
    QString resultMessage = "OK";

    //1. Check the validity of the input
    for (int i = 0; i < contractDetails.count(); i++) {
        bool isNumber;
        if (QString::compare(contractDetails.at(i).first, "Salary / hour:")==0)
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
    }

    //2. If input was valid, the actual saving or updating can take place.
    if(QString::compare(resultMessage,"OK",Qt::CaseInsensitive)==0)
    {
        //2.0 Update the baseSalary of the existing record.
        QSqlQuery query;
        QString sqlQuery = QString("UPDATE %1 SET baseSalary=:baseSalary WHERE id=:id").arg("persons");
        query.prepare(sqlQuery);
        query.bindValue(":baseSalary", baseSalary);
        query.bindValue(":id", employee.value("id").toInt());
        query.exec();
    }

    return resultMessage;
}

QList<QPair<QString,QString>> HourlyContract::readPaymentInputFields(QSqlRecord personRecord)
{
    //In case of a hourly contract, only the total working hours / Month are needed.
    QPair<QString,QString> hoursInTotal;
    hoursInTotal.first = "Hours in total:";
    hoursInTotal.second = "";
    paymentInfoList.insert(paymentInfoList.count(),hoursInTotal);

    return paymentInfoList;
}

QString HourlyContract::writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee)
{
    //A paid salary record has to be created for a specific Month, if not already existing.

    double hoursInTotal = 0;
    int month, year;
    QString resultMessage = "OK";

    QString monthAndYearCheckResult = checkMonthAndYearInput(paymentInput, &month, &year);
    if(QString::compare(monthAndYearCheckResult,"OK",Qt::CaseInsensitive)!=0)return monthAndYearCheckResult;

    //1. Check the validity of the input
    for (int i = 0; i < paymentInput.count(); i++) {
        bool isNumber;

        if (QString::compare(paymentInput.at(i).first, "Hours in total:")==0)
        {
            hoursInTotal = paymentInput.at(i).second.toDouble(&isNumber);
            if(isNumber)//This was a double
            {
                hoursInTotal = (QString::number(hoursInTotal,'f',2)).toDouble();
                if(hoursInTotal < 0 || hoursInTotal > 31*24)
                {
                    resultMessage = tr("Hours in total has to be defined as a number between 0 and 31*24");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Hours in total value format error. Please provide only decimal numbers");
                break;
            }

        }
    }

    //2. If input was valid, the actual saving or updating can take place.
    if(QString::compare(resultMessage,"OK",Qt::CaseInsensitive)==0)
    {
        resultMessage = savePaymentRecord(year,month,employee.value("baseSalary").toDouble()*hoursInTotal,employee);
    }

    return resultMessage;
}


bool HourlyContract::deleteContract(QSqlRecord employee)
{
    //There's actually nothing to delete, as the hourly contract only contains
    //The Salary/hours that is a field of the persons table.
    return true;
}

