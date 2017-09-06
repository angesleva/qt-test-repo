
#include "contractfactory.h"


//The contract factory creates a new contract upon its construction, which means a new
//instance has to be created every time a new contract object is needed.
//This way the factory life time is linked to the contract object lifetime.
//Alternatively getContract() could create the new object
//The deletion of the contract should then also be taken care of before getting a new one from
//the same factory instance.

ContractFactory::ContractFactory(QString contractType, QObject *parent) : QObject(parent), contract(NULL)
{
    //Static method of an abstract class.
    contract = BaseContract::Create(contractType, this);
}

BaseContract * ContractFactory::getContract()
{
    return contract;
}
