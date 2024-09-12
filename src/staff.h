#pragma once
#include"package.h"
#include"Cdate.h"
#include<string>
#include"DbConnection.h"
using namespace std;

class Staff
{
private:
	Cdate* date;
	string password;
	DbConnection* Db;
public:
	Staff(Cdate* date,DbConnection& Db, string password);

	bool identify_staff(string password);

	void checkPackage_number(string number);
	void checkPackage_pickupCode(string pickupCode);
	void checkPackage_ALL();
	bool getPackage(string pickupCode);
	void checkDate(Cdate goalDate);

	bool addPackage(string addresseeNumber, string senderName, string senderNumber, string senderAddress, string trackingNumber, string courierCompany, int weight, string zipCode,string pickupCode);
	bool dropPackage(string trickingNumber);
	void dropPackageALL();
	bool subAlterPackage1(string trackingNumber, int type, string new_information);
	bool subAlterPackage2(string trackingNumber, int type, string new_information);
	bool subAlterPackage3(string trackingNumber, int type, string new_information);
};




