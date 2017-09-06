#ifndef DATABASE_H
#define DATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;

    //1. A table created and prefilled for contract types: Just a primary key and the type name.

    query.exec("create table contractTypes (id int primary key,"
                                           "contractType varChar(20))");

    query.exec("insert into contractTypes values(0,'<all>')");
    query.exec("insert into contractTypes values(1,'Monthly')");
    query.exec("insert into contractTypes values(2,'Hourly')");
    query.exec("insert into contractTypes values(3,'Sales')");

    //2.The persons table where the contract type is a reference to the contract type table.

    query.exec("create table persons (id int primary key, "
                                     "name varchar(40), "
                                     "socialSecrNbr varchar(40),"
                                     "contractTypeId int,"
                                     "baseSalary decimal(10,2))");

    query.exec("insert into persons values(1, 'Nine-to-five Nick', '180474-XXX',1 ,'5000')");
    query.exec("insert into persons values(2, 'Sales Pro','010100-123',3,'2500')");
    query.exec("insert into persons values(3, 'Hourly Hank', '311217-456',2,'50')");


    //3. The sales initiative table will provide info about the Montly target sum and bonus percentage for an individual sales person.
    //   It references the person table

    query.exec("create table salesInitiative (id int primary key, "
                                             "targetSum decimal(10,2), "
                                             "bonusPercentage decimal(3,2), "
                                             "personid int)");

    query.exec("insert into salesInitiative values(1, '3000', '50', 2)"); //Initiative for Sales Pro

    //4. Hours per Month is a table to be filled for an hourly worker.
    //   It also references the person table.


    /*query.exec("create table hoursPerMonth   (id int primary key, "
                                             "hours int "
                                             "month int, "
                                             "year int, "
                                             "personid int) ");
    query.exec("insert into hoursPerMonth values(1, 20, 2, 2017,3)");*/ //February hours for Hank

    //5. The paid salary table holds information about the total, calculated Montly payments for an individual.
    //   It also references the person table.

    query.exec("create table paidSalary (id int primary key, "
                                        "month int,"
                                        "year int,"
                                        "paidSum decimal(10,2), "
                                        "personid int) ");
    query.exec("insert into paidSalary values(1, 1, 2017, 2000,3)");
    query.exec("insert into paidSalary values(2, 1, 2017, 2500,2)");
    query.exec("insert into paidSalary values(3, 1, 2017, 5000,1)");

    return true;
}

#endif // DATABASE_H
