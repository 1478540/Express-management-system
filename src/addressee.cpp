#include "addressee.h"
#include<iostream>
#include<iomanip>
using namespace std;

Addressee::Addressee(string name, string number, string address, string password, DbConnection* Db)
{
	this->name = name;
	this->number = number;
	this->password = password;
	this->address = address;
	this->Db = Db;
}

bool Addressee::regist()
{
	string SQL;
	char sql[200];
	SQL = "select * from addressee where number='" + number + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	if (mysql_num_rows(res)) {
		return false;
	}
	else {
		SQL = "insert into addressee value('" + name + "','" + number + "','" + address + "','" + password + "'" + ");";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		return true;
	}
	
}


bool Addressee::pickup(string pickupCode)
{
	string SQL;
	char sql[200];
	SQL = "select * from package where pickup_code='" + pickupCode + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}
	else {
		SQL = "update package set exist= false where pickup_code='" + pickupCode + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		return true;
	}
}

void Addressee::checkPackage()
{
	string SQL;
	char sql[600];
	SQL = "select sender.name as sender_name,sender.number as sender_number, addressee.name as addressee_name, addressee.number as addressee_number,package.tracking_number,package.courier_company,package.weight,package.zip_code,package.pickup_code,package.exist from sender, addressee, sa, package where sender.number = sa.sender_number and addressee.number = sa.addressee_number and sa.tracking_number = package.tracking_number and addressee.number='" + number + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);

	MYSQL_ROW row;
	if (mysql_num_rows(res) == 0) {
		cout << "查询失败，没有您相关包裹信息" << endl;
	}
	else {
		cout << "查询成功，以下是您所有快件" << endl;
		MYSQL_FIELD* fields;
		unsigned int num_fields = mysql_num_fields(res);
		fields = mysql_fetch_fields(res);
		for (unsigned int i = 0; i < num_fields; i++)
		{
			cout << setw(18) << setiosflags(ios::left) << fields[i].name;
		}
		cout << endl;

		while ((row = mysql_fetch_row(res)))
		{
			for (unsigned int i = 0; i < num_fields; i++)
			{
				if (i == num_fields - 1) {
					if (strcmp(row[i], "1") == 0) {
						cout << setw(18) << setiosflags(ios::left) << "未取";
					}
					else {
						cout << setw(18) << setiosflags(ios::left) << "已取";
					}
					break;
				}
				cout << setw(18) << setiosflags(ios::left) << row[i];
			}
			cout << endl;
		}
	}
}
