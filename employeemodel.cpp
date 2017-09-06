/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "employeemodel.h"
#include "modeloperations.h"
#include "basecontract.h"

EmployeeModel::EmployeeModel(QObject *parent ) : QAbstractListModel(parent)
{
    //1. personsModel model set to reference persons table.
    personsModel = new QSqlRelationalTableModel(this);
    personsModel->setTable("persons");
    personsModel->setRelation(personsModel->fieldIndex("contractTypeId"),
                              QSqlRelation("contractTypes", "id", "contractType"));
    personsModel->select();

    //2. contractTypeModel set to reference contractTypes table

    contractTypeModel = new QSqlRelationalTableModel(this);
    contractTypeModel->setTable("contractTypes");
    contractTypeModel->select();

    setContractFilter(0);
}

int EmployeeModel::rowCount(const QModelIndex &) const
{
    //Just returning the count of rows in the SQL table.
    return personsModel->rowCount();
}

QVariant EmployeeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case FullNameRole: return m_contacts.at(index.row()).fullName;
        case SoSecNbrRole: return m_contacts.at(index.row()).soSecNbr;
        case ContractTypeRole: return m_contacts.at(index.row()).contractType;
        case ContactIdRole: return m_contacts.at(index.row()).contactId;
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> EmployeeModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { FullNameRole, "fullName" },
        { SoSecNbrRole, "soSecNbr" },
        { ContractTypeRole, "contractType" },
        { ContactIdRole, "contactId"}
    };
    return roles;
}

QVariantMap EmployeeModel::get(int row)
{
    const Contact contact = m_contacts.value(row);
    return { {"fullName", contact.fullName}, {"soSecNbr", contact.soSecNbr},
        {"contractType", contact.contractType}, {"contactId", contact.contactId}};
}

void EmployeeModel::append(const QString &fullName, const QString &soSecNbr, const QString &contractType, int contactId)
{
    int row = 0;
    while (row < m_contacts.count() && fullName > m_contacts.at(row).fullName)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_contacts.insert(row, {fullName, soSecNbr, contractType, contactId});
    endInsertRows();
}

void EmployeeModel::set(int row, const QString &fullName, const QString &soSecNbr, const QString &contractType, int contactId)
{
    if (row < 0 || row >= m_contacts.count())
        return;

    m_contacts.replace(row, { fullName, soSecNbr, contractType, contactId});
    dataChanged(index(row, 0), index(row, 0), { FullNameRole, SoSecNbrRole, ContractTypeRole, ContactIdRole});
}

void EmployeeModel::remove(int row)
{
    if (row < 0 || row >= m_contacts.count())
        return;
    //At first the Sql records of this person are removed.
    if(deleteEmployeeRecord(m_contacts.at(row).contactId).compare("OK") ==0 ) {
        beginRemoveRows(QModelIndex(), row, row);
        m_contacts.removeAt(row);
        endRemoveRows();
    }
}

void EmployeeModel::removeAll()
{
    int lastRow = m_contacts.count()-1;
    if(lastRow>=0) {
        m_contacts.clear();

    }
}

void EmployeeModel::setContractFilter(int filterIndex)
{
    //Removing all contacts whenever the contract type is changed.
    //To be checked, whether this could be improved.
    beginResetModel();
    removeAll();
    if(filterIndex == 0) {
        personsModel->setFilter(QString()); //"<all>" == no filter needed.
    } else {
        personsModel->setFilter("contractTypeId == '" + QString::number(filterIndex)  + '\'');
    }

    //Filling the model again with data from sql table.
    int rowCount = personsModel->rowCount();
    for(int i=0;i<rowCount;i++) {
        QString contractType = personsModel->record(i).value("contractType").toString();
        append(personsModel->record(i).value("name").toString(),
               personsModel->record(i).value("socialSecrNbr").toString(),
               personsModel->record(i).value("contractType").toString(),
               personsModel->record(i).value("id").toInt());
    }
    //emit dataChanged(index(0),index(personsModel->rowCount()));
    endResetModel();
}

QString EmployeeModel::addOrUpdateEmployeeRecord(const QString &employeeName, const QString &soSecNbr,
                                                 const QString &contractType, int employeeId,
                                                 const QList<QString> &contractLabels, const QList <QString> &contractValues)
{
    //This function wraps the old c++ interfaces that took SQL data types as parameters to something that is more
    //accessible from javascript.

    QString returnMsg="";

    //Get the employee SQL record
    QSqlRecord employeeRecord = ModelOperations::getPersonRecord(personsModel,employeeId);
    int contractTypeId = ModelOperations::findContractTypeId(contractTypeModel, contractType);

    //Check that no person with this name already exists
    if(!ModelOperations::isPersonNameFree(personsModel, employeeName, employeeRecord)) {
        returnMsg = (tr("An employee with name %1 alreadyExists. Plase provide a unique name.").arg(employeeName));
        return returnMsg;
    }
    //Check that no person with this social secr nbr already exists
    if(!ModelOperations::isSocialSecrNbrFree(personsModel, soSecNbr, employeeRecord)) {
        returnMsg = (tr("The social security number you %1 provided alreadyExists. Plase provide a unique number.").arg(soSecNbr));
        return returnMsg;
    }
    //If no employee Id was provided, a new record will be created.
    if(employeeId<0) {
        employeeRecord = ModelOperations::createNewPersonRecord(personsModel, employeeName, soSecNbr, contractTypeId);
        //Update also the list model data to make it visible to the view.
        append(employeeName,soSecNbr,contractType, employeeRecord.value("id").toInt());
    } else { //If an employee record was provided, it will be updated.
        personsModel->select();
        ModelOperations::updatePersonRecord(personsModel,employeeId,employeeName,soSecNbr,contractTypeId);
        //The local data model also updated
        set(getEmployeeRow(employeeId), employeeName, soSecNbr,contractType,employeeId);
        employeeRecord = ModelOperations::getPersonRecord(personsModel,employeeId);
    }
    //Load the QSql model with the latest table data.
    personsModel->select();
    //Use the base contract object IF for saving the contract type dependent data.
    BaseContract* contract = BaseContract::Create(contractType, this);
    //Format the received string list as list of QPairs.
    QList<QPair<QString,QString>> contractFields;
    for(int i=0;i<contractValues.count();i+=1) {
        QPair<QString,QString> currentPair;
        currentPair.first=contractLabels.at(i);
        currentPair.second=contractValues.at(i);
        contractFields.insert(contractFields.count(),currentPair);
    }
    QString writeStatus = contract->writeContractDetails(contractFields,employeeRecord);

    delete contract;

    return writeStatus; //TO BE CORRECTED...
}

QString EmployeeModel::deleteEmployeeRecord(int employeeId)
{
    QString returnValue="";
    //1. Get the person record to be removed
    QSqlRecord employeeRecord = ModelOperations::getPersonRecord(personsModel,employeeId);
    //2. Remove the ContractInfo
    BaseContract *employeeContract = BaseContract::Create(employeeRecord.value("contractType").toString(),this);
    if(employeeContract) {
        employeeContract->deleteContract(employeeRecord);
    }
    //3. Remove possible payment records of this employee
    QSqlQuery query;
    QString sqlQuery = QString("DELETE FROM paidSalary WHERE personid = :personid ");
    query.prepare(sqlQuery);
    query.bindValue(":personid", employeeId);
    query.exec();
    //4. Finally delete the personRecord itself
    int rowCount = personsModel->rowCount();
    for(int i=0;i<rowCount;i++) {
        if(personsModel->record(i).value("id").toInt()==employeeId) {
            personsModel->removeRow(i);
            returnValue = "OK";
            break;
        }
    }
    return returnValue;
}

int EmployeeModel::getEmployeeRow(int employeeId)
{
    int returnValue = -1;
    int eCount = m_contacts.count();
    for(int i = 0; i < eCount ; i++) {
        if(m_contacts.at(i).contactId == employeeId) {
            returnValue = i;
            break;
        }
    }
    return returnValue;
}


int EmployeeModel::test()
{
    return 1;
}

