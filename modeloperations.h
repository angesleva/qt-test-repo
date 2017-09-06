#ifndef MODELOPERATIONS_H
#define MODELOPERATIONS_H

#include <QObject>
#include <QtSql>


class ModelOperations: public QObject
{
Q_OBJECT
public:
    ModelOperations(QObject* parent);

    static int findFreePersonId(QSqlRelationalTableModel *personsModel);
    static bool isPersonNameFree(QSqlRelationalTableModel *personsModel, const QString &personName, QSqlRecord personRecord);
    static bool isSocialSecrNbrFree(QSqlRelationalTableModel *personsModel, const QString &soSecrNbr, QSqlRecord personRecord);
    static int findContractTypeId(QSqlRelationalTableModel *contractTypeModel, const QString &contractTypeName);
    static QString findContractTypeName(QSqlRelationalTableModel *contractTypeModel, int contractTypeId);
    static QSqlRecord createNewPersonRecord(QSqlRelationalTableModel *personsModel, const QString &personName, const QString &soSecNbr, const int contractTypeId);
    static bool updatePersonRecord(QSqlRelationalTableModel *personsModel, int personID, const QString &personName, const QString &soSecNbr, const int contractTypeId);
    static QSqlRecord getPersonRecord(QSqlRelationalTableModel *personsModel, int employeeId);
};

#endif // MODELOPERATIONS_H
