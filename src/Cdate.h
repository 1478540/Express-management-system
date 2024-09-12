#pragma once
#include<Ctime>
using namespace std;
struct Cdate {
	int year;
	int month;
	int day;
public:
	Cdate() {
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量

		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间

		this->year = t.tm_year + 1900;
		this->month = t.tm_mon + 1;
		this->day = t.tm_mday;
	}
	void initDate(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
	}
};


