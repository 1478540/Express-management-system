#include<iostream>
#include"DbConnection.h"
#include"system.h"
#include"staff.h"
#include"Cdate.h"
using namespace std;

int main() {
	Cdate date;
	DbConnection Db;
	Staff staff(&date, Db, "123");
	System express_management_system(staff, Db);

	express_management_system.initInterface();

	return 0;
}