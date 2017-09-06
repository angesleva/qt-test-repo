#include "paymentinfomodel.h"
#include "basecontract.h"
#include "modeloperations.h"

PaymentInfoModel::PaymentInfoModel(QObject *parent ) : QAbstractListModel(parent)
{
    //1. personsModel model set to reference persons table.
    personsModel = new QSqlRelationalTableModel(this);
    personsModel->setTable("persons");
    personsModel->setRelation(personsModel->fieldIndex("contractTypeId"), QSqlRelation("contractTypes", "id", "contractType"));
    personsModel->select();

    //2. paymentInfoModel set to reference the salary table.

    paymentInfoModel = new QSqlRelationalTableModel(this);
    paymentInfoModel->setTable("paidSalary");
    paymentInfoModel->setRelation(paymentInfoModel->fieldIndex("personid"), QSqlRelation("persons", "id", "name"));
    paymentInfoModel->select();
    paymentInfoModel->sort(paymentInfoModel->fieldIndex("month"),Qt::DescendingOrder);
    paymentInfoModel->sort(paymentInfoModel->fieldIndex("year"),Qt::DescendingOrder);

    //3. Load the payment info data from SQL table to the class model.
    for(int i=0;i<paymentInfoModel->rowCount();i++) {
        append(paymentInfoModel->record(i).value("month").toString(), paymentInfoModel->record(i).value("year").toString(), paymentInfoModel->record(i).value("paidSum").toString(), paymentInfoModel->record(i).value("personid").toInt());
    }

}

int PaymentInfoModel::rowCount(const QModelIndex &) const
{
    //Just return the count of rows that corrensponds to the payment info
    return paymentInfoModel->rowCount();
}

QVariant PaymentInfoModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
    {
        switch (role) {
        case PaymentMonthRole: return m_PaymentInfo.at(index.row()).paymentMonth;
        case PaymentYearRole: return m_PaymentInfo.at(index.row()).paymentYear;
        case PaymentSumRole: return m_PaymentInfo.at(index.row()).paymentSum;
        case PersonIdRole: return m_PaymentInfo.at(index.row()).personId;
        default: return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> PaymentInfoModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { PaymentMonthRole, "paymentMonth" },
        { PaymentYearRole, "paymentYear" },
        { PaymentSumRole, "paymentSum" },
        { PersonIdRole, "personId" }
    };
    return roles;
}

QVariantMap PaymentInfoModel::get(int row)
{
    const PaymentInfo paymentInfo = m_PaymentInfo.value(row);
    return { {"paymentMonth", paymentInfo.paymentMonth},
        {"paymentYear", paymentInfo.paymentYear},
        {"paymentSum", paymentInfo.paymentSum},
        {"personId", paymentInfo.personId}};
}

void PaymentInfoModel::append(const QString &paymentMonth, const QString &paymentYear, const QString &paymentSum, int personId)
{
    int row = 0;
    while (row < m_PaymentInfo.count())
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_PaymentInfo.insert(row, {paymentMonth, paymentYear, paymentSum, personId});
    endInsertRows();
}

void PaymentInfoModel::setPaymentFilter(int employeeId)
{
    //Clearing the whole payment datamodel when the employeeId is changed.
    //To be checked, whether this could be improved.
    removeAll();

    paymentInfoModel->sort(paymentInfoModel->fieldIndex("month"),Qt::DescendingOrder);
    paymentInfoModel->sort(paymentInfoModel->fieldIndex("year"),Qt::DescendingOrder);
    paymentInfoModel->setFilter("personId == '" + QString::number(employeeId)  + '\'');
    int rowCount = paymentInfoModel->rowCount();
    for(int i=0;i<rowCount;i++) {
        append(paymentInfoModel->record(i).value("month").toString(),
               paymentInfoModel->record(i).value("year").toString(),
               paymentInfoModel->record(i).value("paidSum").toString(),
               paymentInfoModel->record(i).value("personid").toInt());
    }
    //emit dataChanged(index(0),index(personsModel->rowCount()));
}

void PaymentInfoModel::removeAll()
{
    int lastRow = m_PaymentInfo.count()-1;
    if(lastRow>=0) {
        beginRemoveRows(QModelIndex(), 0, m_PaymentInfo.count()-1);
        m_PaymentInfo.clear();
        endRemoveRows();
    }
}

QList<QString> PaymentInfoModel::readPaymentInputfields (int employeeId)
{
    personsModel->select(); //Making sure the most recent data has been read into the model
    QSqlRecord personRecord = ModelOperations::getPersonRecord(personsModel,employeeId);
    QString contractType = personRecord.value("contractType").toString();
    BaseContract* contract = BaseContract::Create(contractType,this);
    QList<QPair<QString,QString>> paymentInputFields = contract->readPaymentInputFields(personRecord);
    int inputFieldCount =paymentInputFields.count();
    QList<QString> paymentInputFieldList;
    for(int i=0;i<inputFieldCount;i++) {
        paymentInputFieldList.append(paymentInputFields[i].first);
        paymentInputFieldList.append(paymentInputFields[i].second);
    }
    delete contract;
    return paymentInputFieldList;
}

QString PaymentInfoModel::savePaymentInputFields(const QString &year, const QString &month, const QList<QString> &labelList, const QList<QString> &editList, int employeeId)
{
    QString saveResult;
    QSqlRecord personRecord = ModelOperations::getPersonRecord(personsModel,employeeId);
    QString contractType = personRecord.value("contractType").toString();

    QList<QPair<QString,QString>> paymentFieldList;

    QPair<QString,QString> yearPair;
    yearPair.first = "year";
    yearPair.second = year;
    paymentFieldList.append(yearPair);

    QPair<QString,QString> monthPair;
    monthPair.first = "Month";
    monthPair.second = month;
    paymentFieldList.append(monthPair);

    for(int i=0;i < labelList.count();i++) {
        QPair<QString,QString> newPair;
        newPair.first = labelList.at(i);
        newPair.second = editList.at(i);
        paymentFieldList.append(newPair);
    }

    BaseContract* contract = BaseContract::Create(contractType,this);
    if(contract)
        saveResult = contract->writePaymentData(paymentFieldList,personRecord);
    delete contract;

    return saveResult;
}


