#include "package.h"

Package::Package(string addresseeNumber, string senderName, string senderNumber, string senderAddress, string trackingNumber, string courierCompany, int weight, string zipCode,string pickupCode,DbConnection* Db)
{
	this->addresseeNumber = addresseeNumber;
	this->senderName = senderName;
	this->senderNumber = senderNumber;
	this->senderAddress = senderAddress;
	this->trackingNumber = trackingNumber;
	this->courierCompany = courierCompany;
	this->weight = weight;
	this->zipCode = zipCode;
	this->pickupCode = pickupCode;
	this->Db = Db;

	string SQL;
	char sql[200];
	SQL = "select * from addressee where number='" + addresseeNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	this->addresseeName = row[0];
	this->addresseeAddress = row[2];
}

string Package::getTrackingNumber()
{
	return string();
}

string Package::getPickupCode()
{
	return to_string(000000);
}

void Package::addInformation()
{
	string SQL;
	char sql[200];
	//录入表sender
	if (!this->senderJudge()) {
		SQL = "insert into sender value('" + senderName + "','" + senderNumber + "','" + senderAddress + "');";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
	}

	//录入表SA
	if (!this->SAJudge()) {
		SQL = "insert into SA value('" + senderNumber + "','" + trackingNumber + "','" + addresseeNumber + "');";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
	}
	
	//录入表package
	if (!this->packageJudge()) {
		SQL = "insert into package value('" + trackingNumber + "','" + courierCompany + "'," + to_string(weight) + ",'" + zipCode + "','" + pickupCode + "',true);";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
	}
}






bool Package::senderJudge()
{
	string SQL;
	char sql[200];
	SQL = "select * from sender where number='" + senderNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	if (mysql_num_rows(res) == 0)
		return false;
	else
		return true;
}

bool Package::SAJudge()
{
	string SQL;
	char sql[200];
	SQL = "select * from SA where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	if (mysql_num_rows(res) == 0)
		return false;
	else
		return true;
}

bool Package::packageJudge()
{
	string SQL;
	char sql[200];
	SQL = "select * from package where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	if (mysql_num_rows(res) == 0)
		return false;
	else
		return true;
}
