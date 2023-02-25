#ifndef SUPPLIER_H
#define SUPPLIER_H
#include "DataConn.h"
#include <string>
#include <iostream>
using namespace std;

class Supplier
{
private:
	int SupplierID;
	string Name, ContactNo;
	char contact[12];

public:
	Supplier();
	void AddSupplier();
	void ViewSupplier();
	void SupplierMenu();

};
#endif

Supplier::Supplier()
{
	SupplierID = 0;
}

void Supplier::AddSupplier()
{
	int ran = rand();
	char ch;
	DataConn dc;
	cout << "Add new supplier\n" << endl;
	cout << "Enter supplier name: ";
	cin.ignore();
	getline(cin, Name);
to:
	cout << Name << "\nEnter contact number: ";
	getline(cin, ContactNo);

	for (int i = 0; i < ContactNo.size(); i++)
	{
		if ((ContactNo[i] < 48) || (ContactNo[i] > 57))
		{
			system("cls");
			cout << "Invalid input. Please enter numbers only!" << endl;
			goto to;
		}
	}

	//Insert into database
	PreparedStatement* ps = dc.prepareStatement("INSERT INTO Supplier (Name, ContactNo) VALUES (?,?)");
	ps->setString(1, Name);
	ps->setString(2, ContactNo);
	ps->executeUpdate();
	cout << "\n\nSupplier details added successfully\n" << endl;
	ps->close();
	Name = "";
	ContactNo = "";
	system("pause");
	system("cls");
}

void Supplier::ViewSupplier()
{
	cout << "Supplier details" << endl;
	cout << ">>>>>>>><<<<<<<<" << endl;
	DataConn dc;
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Supplier");
	ResultSet* rs = ps->executeQuery();

	if (rs->next() == false)
	{
		cout << "No supplier data found. Please add supplier details first." << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n----------------------------------------------------------------" << endl;
	cout << "SupplierID\tSupplier Name\t\t\tContact No" << endl;
	cout << "----------------------------------------------------------------" << endl;
	while (rs->next())
	{
		cout << rs->getInt("SupplierID") << "\t\t" << rs->getString("Name") << "\t\t\t" << rs->getString("ContactNo") << endl;
	}
	cout << "----------------------------------------------------------------" << endl;
	cout << "\nEnd of Supplier details\n" << endl;
	rs->close();
	ps->close();
	system("pause");
	system("cls");
}

void Supplier::SupplierMenu()
{
	char c;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "| Mini Market Inventory Management System |" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t     >>>>>>>><<<<<<<" << endl;
	cout << "\t     |Supplier Menu|" << endl;
	cout << "\t     >>>>>>>><<<<<<<" << endl;

	cout << "\nPlease select one of the options below:" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 1. Add supplier details             |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 2. View supplier details            |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 3. Back(Main Menu)                  |" << endl;
	cout << "\t---------------------------------------" << endl;

	cout << "\nYour choice: ";
	cin >> c;
	switch (c)
	{
	case '1':
		system("cls");
		AddSupplier();
		break;

	case '2':
		system("cls");
		ViewSupplier();
		break;

	case '3':
		system("cls");
		return;

	default:
		system("cls");
		cout << "Invalid input. Please try again!\n" << endl;
		SupplierMenu();

	}
}