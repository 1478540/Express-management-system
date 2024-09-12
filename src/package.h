#pragma once
#include<string>
#include"DbConnection.h"
using namespace std;

class Package
{
private:
	DbConnection* Db;
	string trackingNumber;
	string courierCompany;
	string addresseeName;
	string addresseeNumber;
	string addresseeAddress;
	string zipCode;
	string senderName;
	string senderNumber;
	string senderAddress;
	int weight;
	string pickupCode;
public:
	Package(string addresseeNumber, string senderName, string senderNumber, string senderAddress, string trackingNumber, string courierCompany, int weight, string zipCode,string pickupCode,DbConnection* Db);

	string getTrackingNumber();
	string getPickupCode();

	void addInformation();

	bool senderJudge();
	bool SAJudge();
	bool packageJudge();
};





