#ifndef PURCHASE_H
#define PURCHASE_H
#include "DataConn.h"
#include <string>
#include <iostream>
using namespace std;
class Purchase
{
private:
	string date = "", userid;
	double Tpurchase = 0.0, Pprice = 0.0;
	int Upquantity = 0, PurchaseID = 0;

public:
	Purchase();
	void PurchaseMenu(string);
	void AddPurchase(string);
	//void DeletePurchase();
	void Viewpurchase();
	void ViewAllpurchase();
};
#endif

Purchase::Purchase() {}

void Purchase::PurchaseMenu(string id)
{
	char c;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "| Mini Market Inventory Management System |" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
	cout << "\t    >>>>>>>>><<<<<<<<" << endl;
	cout << "\t    | Purchase Menu |" << endl;
	cout << "\t    >>>>>>>>><<<<<<<<" << endl;
	cout << "\nPlease select one of the options below : \n" << endl;
	cout << "\t----------------------------------" << endl;
	cout << "\t| 1. Add purchase                |" << endl;
	cout << "\t----------------------------------" << endl;
	cout << "\t| 2. View total purchase         |" << endl;
	cout << "\t----------------------------------" << endl;
	cout << "\t| 3. Total purchase with receipt |" << endl;
	cout << "\t----------------------------------" << endl;
	cout << "\t| 4. Back(Main Menu)             |" << endl;
	cout << "\t----------------------------------" << endl;

	cout << "\nYour choice: ";
	cin >> c;
	switch (c)
	{
	case '1':
		AddPurchase(id);
		break;

	case '2':
		Viewpurchase();
		break;

	case '3':
		ViewAllpurchase();
		break;

	case '4':
		system("cls");
		return;

	default:
		system("cls");
		cout << "Invalid input. Please try again!\n" << endl;
		PurchaseMenu(id);

	}

}

void Purchase::AddPurchase(string id)
{
	char c1, c2;
	system("cls");
	DataConn dc;
	double total = 0.0;
	string stockid;
	int currqty = 0, newqty = 0;
	userid = id;
	Date d;
	date = d.getDate();
	cout << "Mini Market Inventory Management System (Purchase)\n" << endl;
x:
	cout << "Enter item barcode: ";
	cin.ignore();
	getline(cin, stockid);

	if (stockid != "0") {
		PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
		ps->setString(1, stockid);
		ResultSet* rs = ps->executeQuery();
		if (rs->next()) {
			//Create purchaseID
			PreparedStatement* ps = dc.prepareStatement("INSERT INTO purchase (PurchaseID, Pdate, UserID) values (?,?,?)");
			ps->setInt(1, NULL);
			ps->setString(2, date);
			ps->setString(3, id);
			ps->executeUpdate();
			ps->close();

			ps = dc.prepareStatement("SELECT LAST_INSERT_ID()");
			ResultSet* rs = ps->executeQuery();
			if (rs->next())
				PurchaseID = rs->getInt(1);
			rs->close();
			ps->close();
		}
		else
		{
			cout << "Barcode not found, try another item barcode or add stock in Stock Menu" << endl;
			cout << "Do you want to add another purchase, Y for yes or others to stop: ";
			cin >> c1;
			if (c1 == 'y' || c1 == 'Y')
			{
				system("cls");
				goto x;
			}
			else
			{
				system("pause");
				system("cls");
				return;
			}

		}
		rs->close();
		ps->close();
	}
	else
	{
		system("pause");
		system("cls");
	}

	while (stockid != "0")
	{
		PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
		ps->setString(1, stockid);
		ResultSet* rs = ps->executeQuery();
		if (rs->next())//if exist
		{
			currqty = rs->getInt("Qty_in_hand");
			rs->close();
			ps->close();
		}
		else
		{
			cout << "Barcode not found, try another item barcode or add stock in Stock Menu" << endl;
			cout << "Do you want to add another purchase, Y for yes or others to stop: ";
			cin >> c1;
			if (c1 == 'y' || c1 == 'Y')
			{
				system("cls");
				goto lbl;
			}
			else
			{
				system("pause");
				system("cls");
				goto jj;
				return;
			}
		}
		//check supplierid in database, if doesnt exist, print message to add supplier details first
	pp:
		cout << "Enter stock purchase per quantity price: RM";
		while (!(cin >> Pprice))
		{
			cout << "Enter appropriate (decimal or whole numbers) values only for price" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			goto pp;
		}
	quan:
		cout << "Enter purchase quantity: ";
		while (!(cin >> Upquantity))
		{
			cout << "Enter integer values only for price" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			goto quan;
		}

		newqty = currqty + Upquantity;
		total = Upquantity * Pprice;
		Tpurchase += total;
		//Update quantity in stock table

		ps = dc.prepareStatement("Update stock set Qty_in_hand = ? where StockID = ?");
		ps->setInt(1, newqty);
		ps->setString(2, stockid);
		ps->executeUpdate();
		ps->close();


		//insert into purchasestock table
		ps = dc.prepareStatement("INSERT INTO purchasestock values (?,?,?,?)");
		ps->setInt(1, PurchaseID);
		ps->setString(2, stockid);
		ps->setInt(3, Upquantity);
		ps->setDouble(4, Pprice);
		ps->executeUpdate();
		ps->close();
		cout << "\nThe quantity for item " << stockid << " is updated to " << newqty << endl;
		cout << "\nPurchase added successfully\n" << endl;
	lbl:
		cout << "Enter item barcode to add another purchase or 0 to stop: ";
		cin.ignore();
		getline(cin, stockid);
	}
jj:	//Insert into purchase
	PreparedStatement* ps = dc.prepareStatement("Update purchase set Tpurchase = ? where PurchaseID = ?");
	ps->setDouble(1, Tpurchase);
	ps->setInt(2, PurchaseID);
	ps->executeUpdate();
	ps->close();
	Tpurchase = 0.0;

	system("pause");
	system("cls");
}


void Purchase::Viewpurchase()
{
	system("cls");
	DataConn dc;
	double totalp = 0.0;
	cout << "Mini Market Inventory Management System (Purchase)\n" << endl;
label:
	cout << "Enter date to view purchases, format(yyyy-mm-dd): ";
	cin >> date;
	if (date.size() != 10)
	{
		system("cls");
		cout << "Error in date format, follow format yyyy-mm-dd" << endl;
		system("pause");
		goto label;
	}
	//get the whole table of Purchase for specified date
	PreparedStatement* ps = dc.prepareStatement("SELECT PurchaseID,Tpurchase,UserName FROM Purchase natural join yuser WHERE Pdate = ?");
	ps->setString(1, date);
	ResultSet* rs = ps->executeQuery();
	if (rs->next() == false)
	{
		cout << "\nNo Purchase occured on " << date << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n-------------------------------------------------" << endl;
	cout << "PurchaseID\tTotal Price\tStaff name" << endl;
	cout << "-------------------------------------------------" << endl;
	while (rs->next())//if exist
	{
		cout << rs->getInt("PurchaseID") << "\t\t" << "RM" << fixed << setprecision(2) << rs->getDouble("Tpurchase") << "         " << rs->getString("UserName") << endl;
		totalp += rs->getDouble("Tpurchase");
	}
	cout << "------------------------------------------------" << endl;
	//Calculate total for specified date
	cout << "\nTotal Purchases on " << date << " = RM " << fixed << setprecision(2) << totalp << endl << endl;
	system("pause");
	system("cls");
	rs->close();
	ps->close();

	//Calculate total for specified date
}

void Purchase::ViewAllpurchase()
{
	system("cls");
	DataConn dc;
	int temp1 = 0, temp2 = 0;
	double ptotal = 0;
	cout << "Mini Market Inventory Management System (Purchase)\n" << endl;
label:
	cout << "Enter date to view purchases, format(dd-mm-yyyy): ";
	cin >> date;
	if (date.size() != 10)
	{
		system("cls");
		cout << "Error in date format, follow format dd-mm-yyyy" << endl;
		system("pause");
		goto label;
	}
	PreparedStatement* ps = dc.prepareStatement("SELECT UserName,PurchaseID,StockID,Upquantity,(Pprice*upquantity) as Total FROM `purchase` natural join purchasestock natural join yuser where Pdate = ? order by PurchaseID;");
	ps->setString(1, date);
	ResultSet* rs = ps->executeQuery();

	if (rs->next() == false)
	{
		cout << "\nNo Purchase occured on " << date << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n---------------------------------------------------------------------------------------------------------------" << endl;
	cout << "PurchaseID\tStaff name\tStockID\t\tQuantity\tPurchase amount\t\tTotal per purchase" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------" << endl;
	while (rs->next())//if exist
	{
		temp1 = rs->getInt("PurchaseID");
		if (temp1 != temp2)
		{
			cout << "\n" << rs->getInt("PurchaseID");
			ps = dc.prepareStatement("Select * from purchase where PurchaseID = ?");
			ps->setInt(1, temp1);
			ResultSet* rs = ps->executeQuery();
			if (rs->next()) {
				ptotal = rs->getDouble("Tpurchase");
			}
			cout << "\t\t\t\t\t\t\t\t\t\t\tRM" << fixed << setprecision(2) << ptotal << endl;

		}
		cout << "\t\t" << rs->getString("UserName") << "\t\t" << rs->getString("StockID") << "\t" << rs->getInt("Upquantity") << "\t\tRM " << fixed << setprecision(2) << rs->getDouble("Total") << endl;
		temp2 = rs->getInt("PurchaseID");
	}
	cout << "\n---------------------------------------------------------------------------------------------------------------" << endl;
	ps = dc.prepareStatement("SELECT sum(Tpurchase) as Total FROM `purchase` where Pdate = ?");
	ps->setString(1, date);
	rs = ps->executeQuery();
	if (rs->next())
		cout << "\nTotal purchase for " << date << " is RM " << fixed << setprecision(2) << rs->getDouble("Total") << endl;
	rs->close();
	ps->close();

	cout << endl;


	system("pause");
	system("cls");
}