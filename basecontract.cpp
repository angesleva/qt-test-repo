#include "basecontract.h"
#include "salescontract.h"
#include "hourlycontract.h"
#include "monthlycontract.h"

BaseContract::BaseContract(QObject *parent) : QObject(parent){}

BaseContract* BaseContract::Create(QString contractType, QObject *factory)
{
    if(QString::compare(contractType, "Sales")==0)
    {
        return new SalesContract(factory);
    }
    else if(QString::compare(contractType, "Hourly")==0)
    {
        return new HourlyContract(factory);
    }
    else if(QString::compare(contractType, "Monthly")==0)
    {
        return new MonthlyContract(factory);
    }
    else return NULL;
}

QString BaseContract::savePaymentRecord(int year, int month, double payment, QSqlRecord person)
{
    QString returnValue = "OK";

    QString paidSalaryTableName = "paidSalary";
    paidSalaryModel = new QSqlRelationalTableModel(this);
    paidSalaryModel->setTable(paidSalaryTableName);
    paidSalaryModel->select();

    int firstFreeID = 0;
    int currentID = 0;
    int rowCount = paidSalaryModel->rowCount();
    //1. Search for the first free id
    for(int i=0;i<rowCount;i++)
    {
        currentID = paidSalaryModel->record(i).value("id").toInt();
        if(currentID > firstFreeID) firstFreeID = currentID;
    }
    firstFreeID+=1; //This can be used as the first free Id.

    //1. Search for a matching record to see, whether the salary for the Month in question has already been set.
    paidSalaryModel->setFilter("year ='" + QString::number(year) + "'" + " and " + "month='" + QString::number(month) + "'" + " and " + "personid='" + person.value("id").toString() + "'");

    //2a. If a record was found, return with a message.
    if(paidSalaryModel->rowCount()>0)
    {
        returnValue = tr("The salary for %1 has already been saved for the selected year and Month.").arg(person.value("name").toString());
    }

    //2b. No record found, so a new one can be saved and "OK" return message returned.
    else
    {


        QSqlRecord record;

        QSqlField f1("id", QVariant::Int);
        QSqlField f2("paidSum", QVariant::Double);
        QSqlField f3("month", QVariant::Double);
        QSqlField f4("year", QVariant::Int);
        QSqlField f5("personid",QVariant::Int);

        f1.setValue(QVariant(firstFreeID));
        f2.setValue(QVariant(payment));
        f3.setValue(QVariant(month));
        f4.setValue(QVariant(year));
        f5.setValue(QVariant(person.value("id").toInt()));

        record.append(f1);
        record.append(f2);
        record.append(f3);
        record.append(f4);
        record.append(f5);

        paidSalaryModel->insertRecord(-1, record);
        returnValue = tr("%1 was paid %2 euros in %3/%4 !").arg(person.value("name").toString()).arg(QString::number(payment)).arg(QString::number(month)).arg(QString::number(year));
    }

    return returnValue;
}

QString BaseContract::checkMonthAndYearInput(QList<QPair<QString,QString>> paymentInput, int* month, int* year)
{
    QString resultMessage = tr("OK");


    //1. Check the validity of the input
    for (int i = 0; i < paymentInput.count(); i++) {
        bool isNumber;
        if (QString::compare(paymentInput.at(i).first,"Month")==0)
        {
            *month = paymentInput.at(i).second.toInt(&isNumber);
            if(isNumber)//This was a double
            {
                *month = (QString::number(*month)).toInt();
                if(*month < 0 || *month > 12)
                {
                    resultMessage = tr("The Month number has to be between 1 and 12");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Month number format error. Please provide only integer numbers");
                break;
            }

        }

        if (QString::compare(paymentInput.at(i).first, "year")==0)
        {
            *year = paymentInput.at(i).second.toInt(&isNumber);
            if(isNumber)//This was a double
            {
                *year = (QString::number(*year)).toInt();
                if(*year < 2000 || *year > 2020)
                {
                    resultMessage = tr("The year number has to be given as an integer between 2000 and 2020");
                    break;
                }
            }
            else
            {
                resultMessage = tr("Month number format error. Please provide only integer numbers");
                break;
            }

        }
    }
    return resultMessage;
}
