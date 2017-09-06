#include "modeloperations.h"

ModelOperations::ModelOperations(QObject* parent)
{

}

int ModelOperations::findFreePersonId(QSqlRelationalTableModel *personsModel)
{
    int firstFreeID = 0;
    int currentID = 0;
    int rowCount = personsModel->rowCount();
    //1. Search for the first free id
    for(int i=0;i<rowCount;i++)
    {
        currentID = personsModel->record(i).value("id").toInt();
        if(currentID > firstFreeID) firstFreeID = currentID;
    }
    firstFreeID+=1; //This can be used as the first free Id.

    return firstFreeID;
}

bool ModelOperations::isPersonNameFree(QSqlRelationalTableModel *personsModel, const QString &personName, QSqlRecord personRecord)
{
    bool returnValue = true;
    bool personRecordIsEmpty = personRecord.isEmpty();

    //Go through the model records to see if there's a name match.
    for(int i=0;i<personsModel->rowCount();i++)
    {
        if(personRecordIsEmpty || personsModel->record(i).value("id") != personRecord.value("id")) //jump over the existing record index.
        {
            if(QString::compare(personsModel->record(i).value("name").toString(),personName,Qt::CaseSensitive)==0)
            {
                returnValue = false;
                break;
            }
        }
    }
    return returnValue;
}

bool ModelOperations::isSocialSecrNbrFree(QSqlRelationalTableModel *personsModel, const QString &soSecrNbr, QSqlRecord personRecord)
{
    bool returnValue = true;
    bool personRecordIsEmpty = personRecord.isEmpty();

    for(int i=0;i<personsModel->rowCount();i++)
    {
        if(personRecordIsEmpty || personsModel->record(i).value("id") != personRecord.value("id")) //jump over the existing record index.
        {
            if(QString::compare(personsModel->record(i).value("socialSecrNbr").toString(),soSecrNbr,Qt::CaseSensitive)==0)
            {
                returnValue = false;
                break;
            }
        }
    }
    return returnValue;
}

int ModelOperations::findContractTypeId(QSqlRelationalTableModel *contractTypeModel, const QString &contractTypeName)
{
    int row = 0;

    while (row < contractTypeModel->rowCount()) {
        QSqlRecord record = contractTypeModel->record(row);
        if (record.value("contractType") == contractTypeName)
            return record.value("id").toInt();
        else
            row++;
    }
    return -1;
}

QString ModelOperations::findContractTypeName(QSqlRelationalTableModel *contractTypeModel, int contractTypeId)
{
    int row = 0;

    while (row < contractTypeModel->rowCount()) {
        QSqlRecord record = contractTypeModel->record(row);
        if (record.value("id").toInt() == contractTypeId)
            return record.value("contractType").toString();
        else
            row++;
    }
    return QString();
}

QSqlRecord ModelOperations::createNewPersonRecord(QSqlRelationalTableModel *personsModel, const QString &personName, const QString &soSecNbr, const int contractTypeId)
{
    QSqlRecord newRecord;

    QSqlField f1("id", QVariant::Int);
    QSqlField f2("name", QVariant::String);
    QSqlField f3("socialSecrNbr", QVariant::String);
    QSqlField f4("contractType", QVariant::Int);

    f1.setValue(QVariant(ModelOperations::findFreePersonId(personsModel)));
    f2.setValue(QVariant(personName));
    f3.setValue(QVariant(soSecNbr));
    f4.setValue(QVariant(contractTypeId));

    newRecord.append(f1);
    newRecord.append(f2);
    newRecord.append(f3);
    newRecord.append(f4);

    personsModel->insertRecord(-1, newRecord);

    return newRecord;
}

bool ModelOperations::updatePersonRecord(QSqlRelationalTableModel *personsModel, int personID, const QString &personName, const QString &soSecNbr, const int contractTypeId)
{

    QSqlQuery query;
    bool updateSuccessful = true;
    if(query.prepare("UPDATE persons SET name=:name, socialSecrNbr=:socialSecrNbr, contractTypeId=:contractTypeId WHERE id=:userId"))
    {

        query.bindValue(":name", personName);
        query.bindValue(":socialSecrNbr", soSecNbr);
        query.bindValue(":contractTypeId", contractTypeId); //Testing...
        query.bindValue(":userId",personID);

        if(query.exec())
        {
            emit personsModel->submitAll();
            personsModel->select();
        }
        else
        {
            updateSuccessful = false;
            //QMessageBox::information(this,"", query.lastError().text());
        }
    }
    else
    {
        updateSuccessful = false;
        //QMessageBox::information(this,"",query.lastError().text() + "Prepare!");
    }
    return updateSuccessful;
}

QSqlRecord ModelOperations::getPersonRecord(QSqlRelationalTableModel *personsModel, int employeeId)
{
    int row = 0;

    while (row < personsModel->rowCount()) {
        QSqlRecord record = personsModel->record(row);
        if (record.value("id").toInt() == employeeId)
            return record;
        else
            row++;
    }
    return QSqlRecord();
}


