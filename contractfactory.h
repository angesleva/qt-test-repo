#ifndef CONTRACTFACTORY_H
#define CONTRACTFACTORY_H

#include "basecontract.h"


class ContractFactory:public QObject
{
Q_OBJECT
public:
    ContractFactory(QString contractType,QObject *parent);
    //~ContractFactory();
    BaseContract* getContract();

private:
    BaseContract* contract;
};

#endif // CONTRACTFACTORY_H
