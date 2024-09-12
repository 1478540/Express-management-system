#pragma once
#include<iostream>
#include"staff.h"
#include"DbConnection.h"
#include"addressee.h"
using namespace std;
class System
{
private:
	Staff* staff;
	Addressee* addressee;
	DbConnection* Db;
public:
	System(Staff& staff,DbConnection& Db);
	~System();
	void initInterface();
	void addresseeInterface();
	void addrresseeFunction();
	void staffFunction();
	void addresseeLogin();
	void staffLogin();
	void addresseeRegist();
	void staffAdd();
	void staffAlter();
	void staffDelete();
private:
	void SetAddressee(Addressee& addreessee);
};



