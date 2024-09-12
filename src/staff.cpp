#include "staff.h"
#include"package.h"
#include<iostream>
#include<iomanip>
using namespace std;

Staff::Staff(Cdate* date,DbConnection& Db,string password)
{
	this->date = date;
	this->password = password;
	this->Db = &Db;
}

void Staff::checkPackage_number(string number)
{
	string SQL;
	char sql[600];
	SQL = "select sender.name as sender_name,sender.number as sender_number, addressee.name as addressee_name, addressee.number as addressee_number,package.tracking_number,package.courier_company,package.weight,package.zip_code,package.pickup_code,package.exist from sender, addressee, sa, package where sender.number = sa.sender_number and addressee.number = sa.addressee_number and sa.tracking_number = package.tracking_number and addressee.number='" + number + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	
	MYSQL_ROW row;
	if (mysql_num_rows(res) == 0) {
		cout << "查询失败，没有该用户或该用户没有包裹" << endl;
	}
	else {
		cout << "查询成功，以下是该用户所有快件" << endl;
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
					if (strcmp(row[i],"1")==0) {
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

void Staff::checkPackage_pickupCode(string pickupCode)
{
	string SQL;
	char sql[600];
	SQL = "select sender.name as sender_name,sender.number as sender_number, addressee.name as addressee_name, addressee.number as addressee_number,package.tracking_number,package.courier_company,package.weight,package.zip_code,package.pickup_code,package.exist from sender, addressee, sa, package where sender.number = sa.sender_number and addressee.number = sa.addressee_number and sa.tracking_number = package.tracking_number and package.pickup_code='" + pickupCode + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);

	MYSQL_ROW row;
	if (mysql_num_rows(res) == 0) {
		cout << "查询失败，取件码错误或该包裹已被取走" << endl;
	}
	else {
		cout << "查询成功，以下是该包裹信息" << endl;
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

void Staff::checkPackage_ALL()
{
	string SQL;
	char sql[600];
	SQL = "select sender.name as sender_name,sender.number as sender_number, addressee.name as addressee_name, addressee.number as addressee_number,package.tracking_number,package.courier_company,package.weight,package.zip_code,package.pickup_code,package.exist from sender, addressee, sa, package where sender.number = sa.sender_number and addressee.number = sa.addressee_number and sa.tracking_number = package.tracking_number;";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);

	MYSQL_ROW row;
	if (mysql_num_rows(res) == 0) {
		cout << "查询失败，没有任何包裹的信息" << endl;
	}
	else {
		cout << "查询成功，以下是所有的包裹信息" << endl;
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

bool Staff::getPackage(string pickupCode)
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

void Staff::checkDate(Cdate goalDate)
{
	string SQL;
	char sql[200];
	SQL = "select * from detaileddate where receipt_date='" + to_string(goalDate.year) + '-' + to_string(goalDate.month) + '-' + to_string(goalDate.day) + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	cout << "当天本驿站一共收件" << mysql_num_rows(res) << "件" << endl;
	SQL = "select * from detaileddate where pickup_date='" + to_string(goalDate.year) + '-' + to_string(goalDate.month) + '-' + to_string(goalDate.day) + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	res = Db->executeQuery(sql);
	cout << "当天本驿站一共取件" << mysql_num_rows(res) << "件" << endl;


	cout << endl << "当日所有快递公司的收取件情况如下" << endl;
	SQL = "select * from subview9;";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	res = Db->executeQuery(sql);
	MYSQL_ROW row;
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
			if (row[i] == NULL) {
				cout << setw(18) << setiosflags(ios::left) << 0;
				continue;
			}
			cout << setw(18) << setiosflags(ios::left) << row[i];
		}
		cout << endl;
	}


	cout << endl << "历史记录以来所有快递公司的收取件情况如下" << endl;
	SQL = "select * from subview7;";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	res = Db->executeQuery(sql);
	num_fields = mysql_num_fields(res);
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
			if (row[i] == NULL) {
				cout << setw(18) << setiosflags(ios::left) << 0;
				continue;
			}
			cout << setw(18) << setiosflags(ios::left) << row[i];
		}
		cout << endl;
	}
	cout << endl << endl;

}



bool Staff::addPackage(string addresseeNumber, string senderName, string senderNumber, string senderAddress, string trackingNumber, string courierCompany, int weight, string zipCode,string pickupCode)
{
	string SQL;
	char sql[200];
	SQL = "select * from addressee where number='" + addresseeNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	if (mysql_num_rows(res) == 0)
		return false;
	
	Package package(addresseeNumber, senderName, senderNumber, senderAddress, trackingNumber, courierCompany, weight, zipCode,pickupCode, this->Db);
	if (!package.packageJudge()) {
		package.addInformation();
		return true;
	}
	else {
		return false;
	}

}

bool Staff::dropPackage(string trackingNumber)
{
	string SQL;
	char sql[200];
	SQL = "select * from sa where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}
	else {
		//删除package表中信息
		SQL = "delete from package where tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);

		//删除sa表中信息
		SQL = "delete from sa where tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);

		//删除sender表中信息
		string senderName = row[0];
		SQL = "select * from sa where sender_number='" + senderName + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		MYSQL_RES* res = Db->executeQuery(sql);
		if (mysql_num_rows(res) == 0)
		{
			SQL = "delete from sender where name='" + senderName + "';";
			strcpy_s(sql, SQL.length() + 1, SQL.c_str());
			Db->executeSQL(sql);
		}
		return true;
	}
}

void Staff::dropPackageALL()
{
	string SQL;
	char sql[200];
	//删除sa表中信息
	SQL = "delete from sa where tracking_number in (select tracking_number from package where exist = false); ";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	Db->executeSQL(sql);

	//删除package表中信息
	SQL = "delete from package where exist= false;";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	Db->executeSQL(sql);
	cout << "删除成功，所有已取件的快件信息都已被删除" << endl;
}

bool Staff::subAlterPackage1(string trackingNumber, int type, string newInformation)
{
	string SQL;
	char sql[200];
	SQL = "select * from sa where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}

	switch (type) {
	case 1:
		SQL = "update sender,sa set sender.name='"+newInformation+"' where sa.sender_number=sender.number and sa.tracking_number='"+trackingNumber+"';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	case 2:
		SQL = "update sender,sa set sender.number='" + newInformation + "' where sa.sender_number=sender.number and sa.tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	case 3:
		SQL = "update sender,sa set sender.address='" + newInformation + "' where sa.sender_number=sender.number and sa.tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	default:
		break;
	}
	return true;
}

bool Staff::subAlterPackage2(string trackingNumber, int type, string newInformation)
{
	string SQL;
	char sql[200];
	SQL = "select * from sa where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}

	SQL = "select * from addressee where number='" + newInformation + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	res = Db->executeQuery(sql);
	row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}


	SQL = "update sa set sa.addressee_number='" + newInformation + "' where sa.tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	Db->executeSQL(sql);

	return true;
}

bool Staff::subAlterPackage3(string trackingNumber, int type, string newInformation)
{
	string SQL;
	char sql[200];
	SQL = "select * from package where tracking_number='" + trackingNumber + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res) == 0) {
		return false;
	}

	switch (type) {
	case 1:
		SQL = "update package set courier_company ='" + newInformation + "' where tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	case 2:
		SQL = "update package set weight = " + newInformation + " where tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	case 3:
		SQL = "update package set zip_code = " + newInformation + " where tracking_number='" + trackingNumber + "';";
		strcpy_s(sql, SQL.length() + 1, SQL.c_str());
		Db->executeSQL(sql);
		break;
	default:
		break;
	}
	return true;
}




bool Staff::identify_staff(string password)
{
	if (this->password == password)
		return true;
	else
		return false;
}






