#include "salescontract.h"

SalesContract::SalesContract(QObject *parent) : BaseContract(parent)
{
    //Sales contract class will only process the salesInitiative table.
    QString salesInitiativeTableName = "salesInitiative";
    salesContractTableModel = new QSqlRelationalTableModel(this);
    salesContractTableModel->setTable(salesInitiativeTableName);
    //salesContractTableModel->setRelation(3,QSqlRelation(employeeTableName,"id","name"));
    salesContractTableModel->select();
}

QList<QPair<QString,QString>> SalesContract::readContractDetails(QSqlRecord personRecord)
{

    QSqlRecord contractRecord = findSalesContract(personRecord);

    QPair<QString,QString> baseSalary;
    baseSalary.first = "Base Salary:";
    QPair<QString,QString> targetSales;
    targetSales.first = "Target Sales:";
    QPair<QString,QString> salesBonus;
    salesBonus.first = "Bonus Percentage:";

    if(!personRecord.isEmpty())
    {
        baseSalary.second = personRecord.value("baseSalary").toString();
    }
    else baseSalary.second = "";

    if(!contractRecord.isEmpty())
    {
        targetSales.second = contractRecord.value("targetSum").toString();
        salesBonus.second = contractRecord.value("bonusPercentage").toString();
    }
    else
    {
        targetSales.second = "";
        salesBonus.second = "";
    }

    contractDetailList.insert(contractDetailList.count(),baseSalary);
    contractDetailList.insert(contractDetailList.count(),targetSales);
    contractDetailList.insert(contractDetailList.count(),salesBonus);


return contractDetailList;

}

QSqlRecord SalesContract::findSalesContract(QSqlRecord personRecord)
{
    QSqlRecord foundContractRecord;
    QSqlRecord browsedRecord;
    if(!personRecord.isEmpty())
    {
        int employeeID = personRecord.value("id").toInt();
        int rowCount = salesContractTableModel->rowCount();

        for (int i = 0; i < rowCount; i++) {
            browsedRecord =  salesContractTableModel->record(i);
            int personID = browsedRecord.value("personid").toInt();
            if (personID == employeeID)
            {
                foundContractRecord=browsedRecord;
                break;
            }
        }
    }
    return foundContractRecord;
}

QString SalesContract::writeContractDetails(QList<QPair<QString, QString> > contractDetails, QSqlRecord employee)
{
    double targetSum = 0, bonus = 0, baseSalary = 0;

    QString resultMessage = "OK";

    //1. Check the validity of the input
    for (int i = 0; i < contractDetails.count(); i++)
    {
        bool isNumber;
        if (QString::compare(contractDetails.at(i).first, "Base Salary:")==0)
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
        if (QString::compare("Target Sales:",contractDetails.at(i).first, Qt::CaseSensitive)==0)
        {
            targetSum = contractDetails.at(i).second.toDouble(&isNumber);
            if(isNumber)//This was a double
            {
                targetSum = (QString::number(targetSum,'f',2)).toDouble();
                if(targetSum<0 || targetSum > 10000)
                {
                    resultMessage = tr("Target Sales has to be defined as a number between 1 and 10000");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Target Sales value format error. Please provide only decimal numbers");
                break;
            }

        }

        if (QString::compare(contractDetails.at(i).first, "Bonus Percentage:",Qt::CaseSensitive)==0)
        {
            bonus = contractDetails.at(i).second.toDouble(&isNumber);
            if(isNumber)//This was a double
            {
                bonus = (QString::number(bonus,'f',2)).toDouble();
                if(bonus < 0 || bonus > 10000)
                {
                    resultMessage = tr("Bonus Percentage has to be defined as a number between 1 and 1000");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Bonus Percentage value format error. Please provide only integer numbers");
                break;
            }
        }
    }
    //An additional check making sure that sales target is bigger than base salary.
    //(Could not be compared above because the order of items cannot be assumed (Base Salary before Target sales)).
    if(targetSum<baseSalary)
    {
        resultMessage = tr("Target sales has to be set higher than base salary");
        return resultMessage;
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

        //2.1 Find out, whether this person has a contract record already.
        QSqlRecord contractRecord = findSalesContract(employee);

        //2.2.1 If the person does not have a record yet, a new one will be created.
        if(contractRecord.isEmpty())
        {
        int firstFreeID = 0;
        int currentID = 0;
        //search for the first free id
        for(int i=0;i<salesContractTableModel->rowCount();i++)
        {
            currentID = salesContractTableModel->record(i).value("id").toInt();
            if(currentID > firstFreeID) firstFreeID = currentID;
        }
        firstFreeID+=1; //This can be used as the first free Id.

        QSqlRecord record;

        QSqlField f1("id", QVariant::Int);
        QSqlField f2("targetSum", QVariant::Double);
        QSqlField f3("bonusPercentage", QVariant::Double);
        QSqlField f4("personid", QVariant::Int);

        f1.setValue(QVariant(firstFreeID));
        f2.setValue(QVariant(targetSum));
        f3.setValue(QVariant(bonus));
        f4.setValue(QVariant(employee.value("id").toInt()));

        record.append(f1);
        record.append(f2);
        record.append(f3);
        record.append(f4);

        salesContractTableModel->insertRecord(-1, record);
        }
        //2.2.2. Existing record will be updated
        else
        {
            QSqlQuery query;
            QString sqlQuery = QString("UPDATE %1 SET targetSum=:targetSum, bonusPercentage=:bonusPercentage,  personid=:employeeId WHERE id=:id").arg("salesInitiative");
            query.prepare(sqlQuery);
            query.bindValue(":targetSum", targetSum);
            query.bindValue(":bonusPercentage",bonus);
            query.bindValue(":employeeId",employee.value("id").toInt());
            query.bindValue(":id", contractRecord.value("id"));
            query.exec();
            salesContractTableModel->submitAll();
            salesContractTableModel->select();
        }
    }


return resultMessage;

}

QList<QPair<QString,QString>> SalesContract::readPaymentInputFields(QSqlRecord personRecord)
{
    //In case of a sales contract the sales for the Month impact the overall payment as the only parameter.

    QPair<QString,QString> salesThisMonth;
    salesThisMonth.first = "Sales in total:";
    salesThisMonth.second = "";
    paymentInfoList.insert(paymentInfoList.count(),salesThisMonth);

    return paymentInfoList;
}
QString SalesContract::writePaymentData(QList<QPair<QString,QString>> paymentInput, QSqlRecord employee)
{
    //A paid salary record has to be created for a specific Month, if not already existing.

    double salesThisMonth = 0;
    int month, year;
    QString resultMessage = "OK";

    QString monthAndYearCheckResult = checkMonthAndYearInput(paymentInput, &month, &year);
    if(QString::compare(monthAndYearCheckResult,"OK",Qt::CaseInsensitive)!=0)return monthAndYearCheckResult;

    //1. Check the validity of the input
    for (int i = 0; i < paymentInput.count(); i++) {
        bool isNumber;

        if (QString::compare(paymentInput.at(i).first, "Sales in total:")==0)
        {
            salesThisMonth = paymentInput.at(i).second.toDouble(&isNumber);
            if(isNumber)//This was a double
            {
                salesThisMonth = (QString::number(salesThisMonth,'f',2)).toDouble();
                if(salesThisMonth < 0 || salesThisMonth > 100000)
                {
                    resultMessage = tr("Sales in total has to be defined as a number between 0 and 100000");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Sales in total value format error. Please provide only decimal numbers");
                break;
            }

        }
    }

    //2. If input was valid, the actual saving or updating can take place.
    if(QString::compare(resultMessage,"OK",Qt::CaseInsensitive)==0)
    {
        double salesTarget = 0, bonusPercentage = 0;
        double baseSalary = employee.value("baseSalary").toDouble();
        double paymentThisMonth = baseSalary; //The minimum outcome.
        //2.1 At first the contract record will be read.
        QSqlRecord contractRecord = findSalesContract(employee);
        if(!contractRecord.isEmpty())
        {
            salesTarget = contractRecord.value("targetSum").toDouble();
            bonusPercentage = contractRecord.value("bonusPercentage").toDouble();
            if(salesThisMonth >= salesTarget) //The bonus condition is true;
            {
                //The assumption is that the bonus is calculated on top of the base salary.
                paymentThisMonth = baseSalary + (bonusPercentage)/100 * baseSalary;
            }
        }
        resultMessage = savePaymentRecord(year,month,paymentThisMonth,employee);
    }

    return resultMessage;
}

bool SalesContract::deleteContract(QSqlRecord employee)
{
    //Duplicate code for finding the record.
    //Should exist in a separate class
    QSqlRecord contractRecord;
    bool contractRecordFound = false, returnValue = false;
    int rowCount = salesContractTableModel->rowCount();
    int i = 0; //Represents row nbr.
    for (; i < rowCount; i++) {
        contractRecord =  salesContractTableModel->record(i);
        int personID = contractRecord.value("personid").toInt();
        if (personID == employee.value("id").toInt()) {
            contractRecordFound = true;
            break;
        }
    }
    if (contractRecordFound==true)
    {
        salesContractTableModel->removeRow(i);
        returnValue = true;
    }
    else returnValue = false;
    return returnValue;
}

