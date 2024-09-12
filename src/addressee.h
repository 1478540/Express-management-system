#pragma once
#include<string>
#include"DbConnection.h"
using namespace std;
class Addressee
{
private:
	string name;
	string number;
	string address;
	string password;
	DbConnection* Db;
public:
	Addressee(string name, string number, string address, string password, DbConnection* Db);
	bool regist();
	bool pickup(string pickupCode);
	void checkPackage();
	
};



