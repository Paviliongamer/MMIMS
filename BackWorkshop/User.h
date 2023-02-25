#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>
#include <conio.h>
using namespace std;
#include "DataConn.h"

class User
{
private:
	string UserName, UserID, UserType;
	char pwd[30] = "";
public:
	User();
	void RegUser();
};
#endif

User::User() {}

void User::RegUser()
{
	int loop = 0;
lab:
	DataConn dc;
	char ch;
	int i = 0;
	cout << "Add new user for Mini Market Inventory Management System\n" << endl;
	cout << "Enter new UserID: ";
	if (loop == 0)
		cin.ignore();
	getline(cin, UserID);
	//Query
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM YUser WHERE UserID = ?");
	ps->setString(1, UserID);
	ResultSet* rs = ps->executeQuery();
	if (rs->next())
	{
		system("cls");
		cout << "UserID " << UserID << " already exist. Try another UserID" << endl;
		loop++;
		goto lab;
	}
	rs->close();
	ps->close();
	cout << "Create password: ";

	while ((ch = _getch()) != 13)
	{
		pwd[i] = ch;
		i++;
		cout << "*";
	}
	cout << "\nEnter User name: ";
	getline(cin, UserName);
	cout << "\nEnter user category(Cashier or Admin) : ";
	getline(cin, UserType);
	while (UserType != "Cashier" && UserType != "Admin")
	{
		cout << "Invalid user category! " << endl;
		cout << "Enter user category(Cashier or Admin) : ";
		getline(cin, UserType);
	}
	ps = dc.prepareStatement("INSERT INTO  YUser VALUES(?,?,?,?)");
	ps->setString(1, UserID);
	ps->setString(2, pwd);
	ps->setString(3, UserType);
	ps->setString(4, UserName);
	ps->executeUpdate();
	cout << "\nNew user created successfully." << endl;
	ps->close();
	for (int j = 0; j < 30; j++)
	{
		pwd[j] = NULL;
	}
	system("pause");
	system("cls");
}