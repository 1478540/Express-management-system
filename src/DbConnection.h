#pragma once
#include<mysql.h>
#include<fstream>

class DbConnection
{
private:
	char host[50];
	char username[50];
	char password[50];
	char database[50];

	MYSQL connection;
public:
	DbConnection();
	~DbConnection();
	void getDbInformation();
	MYSQL_RES* executeQuery(const char* sql);
	int executeSQL(const char* sql);
};

