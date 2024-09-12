#pragma once
#include<Ctime>
using namespace std;
struct Cdate {
	int year;
	int month;
	int day;
public:
	Cdate() {
		struct tm t;   //tm�ṹָ��
		time_t now;  //����time_t���ͱ���

		time(&now);      //��ȡϵͳ���ں�ʱ��
		localtime_s(&t, &now);   //��ȡ�������ں�ʱ��

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


