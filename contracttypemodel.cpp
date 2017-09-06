#include "contracttypemodel.h"
#include "basecontract.h"
#include "contractfactory.h"
#include "modeloperations.h"

//This class would not necessarily need to inherit QAbstractListModel, but it is OK for now...
ContractTypeModel::ContractTypeModel(QObject *parent) : QAbstractListModel(parent)
{
    //1. contract type model set to reference contractTypes table.
    contractTypeModel = new QSqlRelationalTableModel(this);
    contractTypeModel->setTable("contractTypes");
    contractTypeModel->select();

    //2. personsModel set to reference persons table.
    personsModel = new QSqlRelationalTableModel(this);
    personsModel->setTable("persons");
    personsModel->select();

    _allIncluded = true;

    //Start by updating the contract types from the sql table.
    for(int i=0;i<contractTypeModel->rowCount();i++) {
        m_contractTypes.append({contractTypeModel->record(i).value("contractType").toString(), contractTypeModel->record(i).value("id").toString()});
    }
}

int ContractTypeModel::rowCount(const QModelIndex &) const
{
    //Just return the count of rows that corrensponds to the contract types
    return contractTypeModel->rowCount();
}


QVariant ContractTypeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount()) {
        switch (role) {
        case ContractTypeRole: return m_contractTypes.at(index.row()).typeName;
        default: return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> ContractTypeModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { ContractTypeRole, "typeName" }
    };
    return roles;
}

void ContractTypeModel::append(const QString &contractTypeName, const int typeId)
{
    int row = 0;
    while (row < m_contractTypes.count() && contractTypeName > m_contractTypes.at(row).typeName)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_contractTypes.insert(row, {contractTypeName, QString::number(typeId)});
    endInsertRows();
}

void ContractTypeModel::removeAll()
{
    int lastRow = m_contractTypes.count()-1;
    if(lastRow>=0) {
        beginResetModel();
        m_contractTypes.clear();
        endResetModel();
    }
}

bool ContractTypeModel::allIncluded()
{
    return _allIncluded;
}

void ContractTypeModel::setAllIncluded(bool allIncluded)
{
    removeAll();

    if(_allIncluded == true && allIncluded == false) {
        contractTypeModel->setFilter("id != '" + QString::number(0)  + '\'');  // "<all>" excluded
    }
    if(_allIncluded == false && allIncluded == true) {
        contractTypeModel->setFilter(QString());
    }

    int rowCount = contractTypeModel->rowCount();
    for(int i=0;i<rowCount;i++)
    {
        m_contractTypes.append({contractTypeModel->record(i).value("contractType").toString(), contractTypeModel->record(i).value("id").toString() });
    }

    _allIncluded = allIncluded;
}

QList<QString> ContractTypeModel::contractTypeFields(QString contractTypeName, int employeeId)
{
    QList<QString> salaryComponentList;

    //If no name was provided, the item at first index will be used.
    if(contractTypeName.isEmpty() || contractTypeName.isNull())
        contractTypeName = m_contractTypes.at(0).typeName;

    //1. Get the correct contract class
    BaseContract* contract = BaseContract::Create(contractTypeName, this);

    //2. Get the person record
    //Just in case..loading the sql data.
    personsModel->select();
    QSqlRecord personRecord = ModelOperations::getPersonRecord(personsModel, employeeId);

    //3. use the contract object to read the contract type dependend fields.

    QList<QPair<QString,QString>> contractDetails = contract->readContractDetails(personRecord);

    //4. convert the list of pairs as a string list (to ease the c++/js type conversion).
    for(int i=0;i<contractDetails.count();i++) {
        QPair<QString,QString> detailPair = contractDetails.at(i);
        salaryComponentList.append(detailPair.first);
        salaryComponentList.append(detailPair.second);
    }
    delete contract;

    return salaryComponentList;
}

QVariantMap ContractTypeModel::getContract(QString contractTypeName)
{
    for(int i=0;i<m_contractTypes.count();i++) {
        if(m_contractTypes.at(i).typeName.compare(contractTypeName)==0) {
            return {{"typeName", m_contractTypes.at(i).typeName},{"typeID",m_contractTypes.at(i).typeID}};
        }
    }

    return QVariantMap();
}

int ContractTypeModel::getContractIndex(QString contractTypeName)
{
    for(int i=0;i<m_contractTypes.count();i++) {
        if(m_contractTypes.at(i).typeName.compare(contractTypeName)==0) {
            return i;
        }
    }

    return -1;
}

