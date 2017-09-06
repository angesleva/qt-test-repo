#ifndef CONTRACTTYPEMODEL_H
#define CONTRACTTYPEMODEL_H

#include <QAbstractListModel>
#include <QtSql>

class ContractTypeModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(int testProp READ readTestProp NOTIFY testPropChanged)
    Q_PROPERTY(bool allIncluded READ allIncluded WRITE setAllIncluded)

public:
    bool allIncluded();
    void setAllIncluded(bool allIncluded);

    //int readTestProp();

    enum ContractTypeRoles {
        ContractTypeRole = Qt::DisplayRole//,
        //TypeIdRole = Qt::UserRole
    };

    Q_ENUM(ContractTypeRoles)

    ContractTypeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void append(const QString &contractTypeName, const int typeId);
    Q_INVOKABLE QList<QString> contractTypeFields(QString contractTypeName, int employeeId);
    Q_INVOKABLE QVariantMap getContract(QString contractTypeName);
    Q_INVOKABLE int getContractIndex(QString contractTypeName);
    //Q_INVOKABLE int getEmployeeId(QString employeeName);



signals:
    //void testPropChanged();

public slots:

private:
    struct ContractType {
        QString typeName;
        QString typeID;
    };

    void removeAll();

    QList<ContractType> m_contractTypes;
    QSqlRelationalTableModel *contractTypeModel;
    QSqlRelationalTableModel *personsModel;
    bool _allIncluded;

};

#endif // CONTRACTTYPEMODEL_H
