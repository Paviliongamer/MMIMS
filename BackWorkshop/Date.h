#ifndef DATE_H
#define DATE_H
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
using namespace std;

class Date
{
private:
	time_t curr_time;
	tm* curr_tm;
	char date_string[50];

public:
	Date();
	string getDate();
};
#endif

Date::Date() {}

string Date::getDate()
{
	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 25, "%Y-%m-%d", curr_tm);

	return date_string;
}