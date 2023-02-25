#ifndef CONTROL_H
#define CONTROL_H
#include "DataConn.h"
#include <string>
#include <iostream>
#include <conio.h>
#include "DataConn.h"


using namespace std;
class Control 
{
public:
	Stock st;
	User us;
	Supplier sup;
	Sales sale;
	Purchase pur;
	string id;
	string Login(string&);
	void Menu(string, string);
	string Access;
};
#endif


string Control::Login(string& id)
{
	DataConn dc;
label:
	int i = 0, chr = 0, ch = 0;
	string userid, access, Password;
	char pd[30] = {};
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t| Welcome to Mini Market Inventory Management System | " << endl;
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\nLogin" << endl;
	cout << "~~~~~" << endl;
	cout << "\nUser ID : ";
	cin >> userid;
	cout << "Password: ";
	do
	{
		ch = _getch();

		if (ch == 13 || ch == ' ')
		{
			break;
		}
		if ((ch == 8 || ch == 127) && !Password.empty())
		{
			std::cout << "\b \b";
			Password.erase(Password.size() - 1);
			chr--;
		}
		else
		{
			Password += ch;
			std::cout << "*";
			chr++;
		}
	} while (ch != 13 || ch != ' ');
	string pwd = Password;

	//Query
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM YUser WHERE UserID = ? AND Userpwd = ?");
	//For UserID
	ps->setString(1, userid);
	//For Userpwd
	ps->setString(2, pwd);
	//Result from sql
	ResultSet* rs = ps->executeQuery();
	for (int j = 0; j < 30; j++)
	{
		pd[j] = NULL;
	}

	if (rs->next())
	{
		access = rs->getString("UserType");
		id = rs->getString("UserID");
		cout << "\n\nGood day, " << rs->getString("UserName") << "." << endl;
		cout << "Next, you will be redirected to " << rs->getString("UserType") << " Menu." << endl << endl;
		system("pause");
		system("cls");
		rs->close();
		ps->close();
		return access;
	}
	else
	{
		system("cls");
		cout << "Invalid UserID or Password.\nPlease try again\n" << endl;
		goto label;
	}
}

void Control::Menu(string acc, string id)
{
	if (acc == "Admin")
	{
		char choice;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "|Mini Market Inventory Management System|" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
		cout << "\t     >>>>>><<<<<<<" << endl;
		cout << "\t     | Main Menu |" << endl;
		cout << "\t     >>>>>><<<<<<<" << endl;
		cout << "\nPlease select one of the options below :\n" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 1.Register new user |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 2.Sales             |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 3.Purchase          |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 4.Stock             |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 5.Supplier          |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 6.Logout            |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 7.Exit              |" << endl;
		cout << "\t-----------------------" << endl;

		cout << "\nYour choice: ";
		cin >> choice;

		switch (choice)
		{
		case '1':
			system("cls");
			us.RegUser();
			Menu(acc, id);
			break;

		case '2':
			system("cls");
			sale.SalesMenu(id);
			Menu(acc, id);
			break;

		case '3':
			system("cls");
			pur.PurchaseMenu(id);
			Menu(acc, id);
			break;

		case '4':
			system("cls");
			st.StockMenu();
			Menu(acc, id);
			break;

		case '5':
			system("cls");
			sup.SupplierMenu();
			Menu(acc, id);
			break;

		case '7':
			cout << "\nThanks for using this system. See you again :)" << endl;
			exit(0);

		case '6':
			cout << "\nUser logged out successfully" << endl;
			system("pause");
			system("cls");
			Access = Login(id);
			Menu(Access, id);
			break;

		default:
			system("cls");
			cout << "\nInvalid input. Please try again!\n" << endl;
			Menu(Access, id);
		}
	}

	else
	{
		char choice = NULL;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "|Mini Market Inventory Management System|" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
		cout << "\t     >>>>>><<<<<<<" << endl;
		cout << "\t     | Main Menu |" << endl;
		cout << "\t     >>>>>><<<<<<<" << endl;
		cout << "\nPlease select one of the options below :\n" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 1.Add sales         |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 2.View total sales  |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 3.Logout            |" << endl;
		cout << "\t-----------------------" << endl;
		cout << "\t| 4.Exit              |" << endl;
		cout << "\t-----------------------" << endl;

		cout << "\nYour choice: ";
		cin >> choice;

		switch (choice)
		{
		case '1':
			system("cls");
			sale.AddSale(id);
			Menu(acc, id);
			break;

		case '2':
			system("cls");
			sale.Viewsale(id);
			Menu(acc, id);
			break;

		case '4':
			cout << "Thanks for using this system. See you again :)" << endl;
			exit(0);

		case '3':
			cout << "\nUser logged out successfully" << endl;
			system("pause");
			system("cls");
			Access = Login(id);
			Menu(Access, id);
			break;

		default:
			system("cls");
			cout << "Invalid input. Please try again!\n" << endl;
			Menu(acc, id);

		}

	}
}