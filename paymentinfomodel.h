#ifndef PAYMENTINFOMODEL_H
#define PAYMENTINFOMODEL_H


#include <QAbstractListModel>
#include <QtSql>

class PaymentInfoModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(int testProp READ readTestProp NOTIFY testPropChanged)
    //Q_PROPERTY(bool allIncluded READ allIncluded WRITE setAllIncluded)

public:
    //bool allIncluded();
    //void setAllIncluded(bool allIncluded);

    //int readTestProp();

    enum PaymentInfoRole {
        PaymentMonthRole = Qt::DisplayRole,
        PaymentYearRole = Qt::UserRole,
        PaymentSumRole,
        PersonIdRole
    };

    Q_ENUM(PaymentInfoRole)

    PaymentInfoModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE void append(const QString &paymentMonth, const QString &paymentYear, const QString &paymentSum, int personId);
    Q_INVOKABLE void setPaymentFilter(int employeeId);
    Q_INVOKABLE QList<QString> readPaymentInputfields (int employeeId);
    Q_INVOKABLE QString savePaymentInputFields(const QString &year, const QString &month, const QList<QString> &labelList, const QList<QString> &editList, int employeeId);
    /*Q_INVOKABLE QList<QString> contractTypeFields(QString contractTypeName, int employeeId);
    Q_INVOKABLE QVariantMap getContract(QString contractTypeName);
    Q_INVOKABLE int getContractIndex(QString contractTypeName);*/
    //Q_INVOKABLE int getEmployeeId(QString employeeName);



signals:


public slots:

private:
    /*SQL Model (without record id)*/
    /*  paidSalary
        (id int primary key, "
        "month int,"
        "year int,"
        "paidSum decimal(10,2), "
        "personid int) ");*/

    struct PaymentInfo {
        QString paymentMonth;
        QString paymentYear;
        QString paymentSum;
        int personId;

    };

    void removeAll();

    QList<PaymentInfo> m_PaymentInfo;
    QSqlRelationalTableModel *paymentInfoModel;
    QSqlRelationalTableModel *personsModel;
    //bool _allIncluded;

};

#endif // PAYMENTINFOMODEL_H
