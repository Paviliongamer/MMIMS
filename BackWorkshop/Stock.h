#ifndef STOCK_H
#define STOCK_H

#include "DataConn.h"
using namespace std;

class Stock
{
private:
	string StockID, StockName, StockDetails;
	int QtyInHand, supplierid;
	double Sprice;

public:
	Stock();
	~Stock();
	void StockMenu();
	void AddStock();
	void DeleteStock();
	void UpdatePrice();
	void CheckAvail();
	void ProductSupplier();

};

#endif

Stock::Stock()
{
	StockID = "";
	StockName = "";
	StockDetails = "";
	QtyInHand = 0;
	Sprice = 0.0;
}
Stock::~Stock() {}

void Stock::StockMenu()
{
	char c;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "| Mini Market Inventory Management System |" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t     >>>>>>><<<<<<<" << endl;
	cout << "\t     | Stock Menu |" << endl;
	cout << "\t     >>>>>>><<<<<<<" << endl;

	cout << "\nPlease select one of the options below:\n" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 1. Add stock                        |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 2. Delete stock                     |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 3. Update selling price             |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 4. Check stock availability         |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 5. Change stock supplier details    |" << endl;
	cout << "\t---------------------------------------" << endl;
	cout << "\t| 6. Back(Main Menu)                  |" << endl;
	cout << "\t---------------------------------------" << endl;


	cout << "\nYour choice: ";
	cin >> c;
	switch (c)
	{
	case '1':
		system("cls");
		AddStock();
		system("cls");
		break;

	case '2':
		system("cls");
		DeleteStock();
		break;

	case '3':
		system("cls");
		UpdatePrice();
		break;

	case '4':
		system("cls");
		CheckAvail();
		break;

	case '5':
		system("cls");
		ProductSupplier();
		break;

	case '6':
		system("cls");
		return;


	default:
		system("cls");
		cout << "Invalid input. Please try again!\n" << endl;
		StockMenu();

	}

}

void Stock::AddStock()
{
	char c1;
	int loop = 0;
lbl:
	DataConn dc;
	system("cls");
	cout << "Add stock\n" << endl;
	cout << "Use this table as reference when entering SupplierID" << endl;
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Supplier");
	ResultSet* rs = ps->executeQuery();
	cout << "--------------------------------------------------------------------" << endl;
	cout << "SupplierID\tSupplier Name\t\t\tContact No" << endl;
	cout << "--------------------------------------------------------------------" << endl;

	while (rs->next())
	{
		cout << rs->getInt("SupplierID") << "\t\t" << rs->getString("Name") << "\t\t\t\t" << rs->getString("ContactNo") << endl;
	}
	rs->close();
	ps->close();
	cout << "--------------------------------------------------------------------" << endl;

	cout << "\nEnter SupplierID: ";
	cin >> supplierid;

	ps = dc.prepareStatement("SELECT * FROM Supplier where SupplierID = ?");
	ps->setInt(1, supplierid);
	rs = ps->executeQuery();
	if (rs->next())
	{
	rep:
		cout << "Enter item barcode: ";
		cin >> StockID;
		if (StockID.size() != 8)
		{
			loop++;
			cout << "Invalid barcode. Please try again." << endl;
			cout << "Barcode should contain 8 numbers only" << endl;
			goto rep;
		}
		//check stockID in database
		//Query
		PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
		ps->setString(1, StockID);
		ResultSet* rs = ps->executeQuery();
		if (rs->next())
		{
			system("cls");
			cout << "Barcode " << StockID << " already in the database. Try another barcode" << endl;
			loop++;
			system("pause");
			goto lbl;
		}
		rs->close();
		ps->close();

		cout << "Enter Stock Name: ";
		cin.ignore();
		getline(cin, StockName);
		cout << "Enter stock details: ";
		getline(cin, StockDetails);
	con:
		cout << "Enter selling price per quantity: RM";
		while (!(cin >> Sprice))
		{
			cout << "Enter appropriate (decimal or whole numbers) values only for price" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			goto con;
		}

		ps = dc.prepareStatement("INSERT INTO Stock VALUES(?,?,?,?,?,?)");
		ps->setString(1, StockID);
		ps->setString(2, StockName);
		ps->setInt(3, 0);
		ps->setString(4, StockDetails);
		ps->setDouble(5, Sprice);
		ps->setInt(6, supplierid);
		ps->executeUpdate();
		cout << "\nStock details added successfully" << endl;

		ps->close();
	}
	else
	{
		cout << "SupplierID not found. Please try another SupplierID or add new SupplierID first in Admin menu." << endl;
		cout << "Do you want to add another stock, Y for yes or others to stop: ";
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
			return;
		}

	}
	system("pause");
}

void Stock::DeleteStock()
{
	DataConn dc;
	system("cls");
	cout << "Delete stock\n" << endl;
	cout << "Please take note that you can only delete stock which has no sales involved " << endl;
	cout << "Please choose barcode from table below to delete\n" << endl;
	PreparedStatement* ps = dc.prepareStatement("SELECT * from stock where stockid not in(select stockid from salestock)");
	ResultSet* rs = ps->executeQuery();
	if (rs->next() == false)
	{
		cout << "No stocks that able to be deleted " << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "--------------------------------------------------------------------------------------------------" << endl;
	cout << "Barcode\t\tStock Name\t\tQuantity\tStock Details\t\t\tPrice" << endl;
	cout << "--------------------------------------------------------------------------------------------------" << endl;

	while (rs->next())
	{
		cout << rs->getString("StockID") << "\t" << rs->getString("StockName") << "\t\t" << rs->getInt("Qty_in_hand") << "\t\t" << rs->getString("Stock_details") << "\t\tRM" << fixed << setprecision(2) << rs->getDouble("Sprice") << endl;
	}
	rs->close();
	ps->close();
	cout << "--------------------------------------------------------------------------------------------------" << endl;

	cout << "\nEnter item barcode to delete: ";
	cin.ignore();
	getline(cin, StockID);
	ps = dc.prepareStatement("delete from stock where StockID = ?");
	ps->setString(1, StockID);
	if (ps->executeUpdate() == 1)
		cout << "\nAll information of item " << StockID << " is deleted\n" << endl;
	else
		cout << "Error while deleting the item!" << endl;
	ps->close();
	system("pause");
	system("cls");
}

void Stock::UpdatePrice()
{
	int loop = 0;
lbl:
	DataConn dc;
	system("cls");
	cout << "\nUpdate selling price\n" << endl;
	cout << "Enter item barcode to update selling price: ";
	if (loop == 0)
		cin.ignore();
	getline(cin, StockID);
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
	ps->setString(1, StockID);
	ResultSet* rs = ps->executeQuery();
	if (rs->next())
	{
		cout << "Old price: RM" << fixed << setprecision(2) << rs->getDouble("Sprice") << endl;
		rs->close();
		ps->close();
	}
	else
	{
		system("cls");
		cout << "Barcode " << StockID << " doesn't exist. Try another barcode" << endl;
		loop++;
		system("pause");
		goto lbl;
	}
ss:
	cout << "\nEnter new price: RM";
	while (!(cin >> Sprice))
	{
		cout << "Enter appropriate (decimal or whole numbers) values only for price" << endl;
		cin.clear();
		cin.ignore(123, '\n');
		goto ss;
	}

	ps = dc.prepareStatement("UPDATE Stock SET Sprice = ? WHERE StockID = ?");
	ps->setDouble(1, Sprice);
	ps->setString(2, StockID);
	if (ps->executeUpdate() == 1)
		cout << "\n\nNew price updated successfully\n" << endl;
	else
		cout << "Price not updated!" << endl;
	ps->close();
	system("pause");
	system("cls");
}

void Stock::CheckAvail()
{
	int loop = 0;
lbl:
	DataConn dc;
	system("cls");
	cout << "Check stock availability\n" << endl;
	cout << "Enter item barcode to check availability: ";
	if (loop == 0)
		cin.ignore();
	getline(cin, StockID);
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
	ps->setString(1, StockID);
	ResultSet* rs = ps->executeQuery();
	if (rs->next())
	{
		cout << "\n\nStock Name\t: " << rs->getString("StockName") << endl;
		cout << "Price\t\t: RM" << fixed << setprecision(2) << rs->getDouble("Sprice") << endl;
		cout << "Available\t: " << rs->getInt("Qty_in_hand") << " pcs\n" << endl;
		system("pause");

	}
	else
	{
		system("cls");
		cout << "Barcode " << StockID << " doesn't exist. Try another barcode" << endl;
		loop++;
		system("pause");
		goto lbl;
	}
	rs->close();
	ps->close();
	system("cls");
}

void Stock::ProductSupplier()
{
	int supplier;
	DataConn dc;
	system("cls");
oo:
	cout << "Change stock supplier details\n" << endl;
	cout << "Enter item barcode to change supplier: ";
	cin >> StockID;
	PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
	ps->setString(1, StockID);
	ResultSet* rs = ps->executeQuery();
	if (rs->next())
	{
		cout << "\nCurrent supplier : " << rs->getInt("SupplierID") << endl;
	}
	else
	{
		cout << "Error !" << endl;
		system("pause");
		system("cls");
		goto oo;
	}
	rs->close();
	ps->close();

	cout << "\nUse this table as reference when entering SupplierID" << endl;
	ps = dc.prepareStatement("SELECT * FROM Supplier");
	rs = ps->executeQuery();

	if (rs->next() == false)
	{
		cout << "No Supplier data found. Add supplier details first " << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n----------------------------------------------------------------------" << endl;
	cout << "SupplierID\tSupplier Name\t\t\tContact No" << endl;
	cout << "\n----------------------------------------------------------------------" << endl;
	while (rs->next())
	{
		cout << rs->getInt("SupplierID") << "\t\t" << rs->getString("Name") << "\t\t\t" << rs->getString("ContactNo") << endl;
	}
	cout << "\n----------------------------------------------------------------------" << endl;

	rs->close();
	ps->close();

	cout << "\nEnter new SupplierID: ";
	cin >> supplier;
	ps = dc.prepareStatement("Update stock set SupplierID = ?  where StockID = ?;");
	ps->setInt(1, supplier);
	ps->setString(2, StockID);
	ps->executeUpdate();
	cout << "\nChanges saved\n" << endl;
	system("pause");
	system("cls");
}