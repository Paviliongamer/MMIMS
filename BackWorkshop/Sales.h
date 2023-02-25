#ifndef SALES_H
#define SALES_H
#define _CRT_SECURE_NO_WARNINGS
#include "DataConn.h"
#include "Date.h"
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

class Sales
{
private:
	double Tsale, Qprice, sprice;
	string date;
	int Quantity, SalesID;

public:
	Sales();
	void SalesMenu(string);
	void UpdateQty(string, int);
	//void DeleteSale();
	void AllSales(string);
	void AddSale(string);
	void Viewsale(string);
	void ViewTsale(string);
	void ViewTopQsale();
	void MonthSales();

};
#endif

Sales::Sales()
{
	sprice = 0;
	SalesID = 0;
	Tsale = 0.0;
	Qprice = 0.0;
	Quantity = 0;
}

void Sales::SalesMenu(string id)
{
	char c;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "|Mini Market Inventory Management System|" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t     >>>>>>><<<<<<<" << endl;
	cout << "\t     | Sales Menu |" << endl;
	cout << "\t     >>>>>>><<<<<<<" << endl;

	cout << "\nPlease select one of the options below : " << endl;
	cout << "\n\t----------------------" << endl;
	cout << "\t| 1.Add sales        |" << endl;
	cout << "\t----------------------" << endl;
	cout << "\t| 2.View total sales |" << endl;
	cout << "\t----------------------" << endl;
	cout << "\t| 3.Back (Main Menu) |" << endl;
	cout << "\t----------------------" << endl;


	cout << "\nYour choice: ";
	cin >> c;
	switch (c)
	{
	case '1':
		system("cls");
		AddSale(id);
		break;

	case '2':
		system("cls");
		Viewsale(id);
		break;

	case '3':
		system("cls");
		return;

	default:
		system("cls");
		cout << "Invalid input. Please try again!\n" << endl;
		SalesMenu(id);
		break;

	}
}

void Sales::AddSale(string id)
{
	cout << "Mini Market Inventory Management System (Sales)\n" << endl;
	Date d;
	date = d.getDate();
	cout << date << " is nice" << endl;
	cout << "Sales by staff " << id << endl;
	DataConn dc;
	string stockid;
	SalesID = -1;
	cout << "Enter item barcode: ";
	cin >> stockid;
	if (stockid != "0")
	{
		PreparedStatement* ps = dc.prepareStatement("INSERT INTO sales (SalesID, Sdate, UserID) values (?,?,?)");
		ps->setInt(1, NULL);
		ps->setString(2, date);
		ps->setString(3, id);
		ps->executeUpdate();
		ps->close();

		ps = dc.prepareStatement("SELECT LAST_INSERT_ID()");
		ResultSet* rs = ps->executeQuery();
		if (rs->next())
			SalesID = rs->getInt(1);
		rs->close();
		ps->close();

	}
	else
	{
		system("cls");
		return;
	}
	while (stockid != "0")
	{
		PreparedStatement* ps = dc.prepareStatement("SELECT * FROM Stock WHERE StockID = ?");
		ps->setString(1, stockid);
		ResultSet* rs = ps->executeQuery();

		if (rs->next())//if exist
		{
			if ((rs->getInt("Qty_in_hand")) <= 0)
			{
				cout << "Out of stock.\nTry another item barcode";
				goto lbl;
			}
			cout << rs->getString("StockName") << "\tRM" << rs->getDouble("Sprice") << endl;
			sprice = rs->getDouble("Sprice");
			rs->close();
			ps->close();
		}
		else//if doesnt exist
		{
			cout << "Item barcode not found.\nTry another StockID";
			goto lbl;
		}
	val:
		//get stockname, Sprice from database
		cout << "Enter quantity: ";
		while (!(cin >> Quantity))
		{
			cout << "Enter integer values only for quantity" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			goto val;
		}

		Qprice = Quantity * sprice;
		Tsale += Qprice;

		ps = dc.prepareStatement("INSERT INTO salestock values (?,?,?,?)");
		ps->setInt(1, SalesID);
		ps->setString(2, stockid);
		ps->setInt(3, Quantity);
		ps->setDouble(4, Qprice);
		ps->executeUpdate();
		ps->close();

		UpdateQty(stockid, Quantity);
	lbl:
		cout << "\n\nAdd another item barcode or 0 to stop: ";
		cin >> stockid;
	}
	//5% discount when total sales above RM100
	if (Tsale >= 100)
	{
		cout << "\nYou bought more than RM100 in single receipt." << endl;
		cout << "You're entitled for 5% discount" << endl;
		Tsale = Tsale * 0.95;
	}

	cout << "\nTotal price = RM " << fixed << setprecision(2) << Tsale << endl << endl;
	PreparedStatement* pas = dc.prepareStatement("Update sales set Tsale = ? where SalesID = ?");
	pas->setDouble(1, Tsale);
	pas->setInt(2, SalesID);
	pas->executeUpdate();
	pas->close();
	Tsale = 0.0;
	system("pause");
	system("cls");
}

//void Sales::DeleteSale(){}

void Sales::Viewsale(string id)
{
	char c;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "| Mini Market Inventory Management System (Sales) |" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\nPlease select one of the options below:\n" << endl;
	cout << "\t--------------------------------" << endl;
	cout << "\t| 1.View total sales           |" << endl;
	cout << "\t--------------------------------" << endl;
	cout << "\t| 2.View top sales product     |" << endl;
	cout << "\t--------------------------------" << endl;
	cout << "\t| 3.Total sales with receipt   |" << endl;
	cout << "\t--------------------------------" << endl;
	cout << "\t| 4.Back to sales menu         |" << endl;
	cout << "\t--------------------------------" << endl;

	cout << "\nYour choice: ";
	cin >> c;
	switch (c)
	{
	case '1':
	gg:
		system("cls");
		cout << "Enter sales date, format(yyyy-mm-dd): ";
		cin >> date;
		if (date.size() != 10)
		{
			cout << "Error in date format, follow format yyyy-mm-dd" << endl;
			system("pause");
			goto gg;
		}
		ViewTsale(date);
		break;

	case '2':
		system("cls");
		ViewTopQsale();
		break;

	case '3':
	tt:
		system("cls");
		cout << "Enter sales date, format(yyyy-mm-dd): ";
		cin >> date;
		if (date.size() != 10)
		{
			cout << "Error in date format, follow format yyyy-mm-dd" << endl;
			system("pause");
			goto tt;
		}
		AllSales(date);
		break;

	case '4':
		system("cls");
		SalesMenu(id);
		break;

	default:
		cout << "ERROR 404" << endl;
		system("cls");
		Viewsale(id);
		break;
	}

	}

void Sales::UpdateQty(string stockid, int qty)
{
	DataConn dc;
	int stockqty = 0, Newqty;
	PreparedStatement* ps = dc.prepareStatement("Select * from stock where StockID = ?");
	ps->setString(1, stockid);
	ResultSet* rs = ps->executeQuery();
	if (rs->next())
		stockqty = rs->getInt("Qty_in_hand");
	rs->close();
	ps->close();

	Newqty = stockqty - qty;

	ps = dc.prepareStatement("Update stock set Qty_in_hand = ? where StockID = ?");
	ps->setInt(1, Newqty);
	ps->setString(2, stockid);
	ps->executeUpdate();
	ps->close();

}

void Sales::ViewTsale(string date)
{
	DataConn dc;
	double TotalDate = 0.0;
	PreparedStatement* ps = dc.prepareStatement("Select SalesID,Tsale, UserName from sales natural join Yuser where Sdate = ?");
	ps->setString(1, date);
	ResultSet* rs = ps->executeQuery();
	/*if (rs->getString("SalesID") == NULL)
	{
		cout << "\nNo sales record found\n" << endl;
		system("pause");
		system("cls");
		return;
	}*/
	if (rs->next() == false)
	{
		cout << "\nNo Sales occured on " << date << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n ----------------------------------------------------------" << endl;
	cout << " SalesID\tTotal per sales\t\tStaff Name" << endl;
	cout << " ----------------------------------------------------------" << endl;

	while (rs->next())
	{
		cout << " " << rs->getInt("SalesID") << "\t\tRM" << fixed << setprecision(2) << rs->getDouble("Tsale") << "                  " << rs->getString("UserName") << endl;
		TotalDate += rs->getDouble("Tsale");
	}
	cout << " ----------------------------------------------------------" << endl;
	cout << "\n Total sales for " << date << " = RM " << fixed << setprecision(2) << TotalDate << "\n\n" << endl;
oo:
	rs->close();
	ps->close();
	system("pause");
	system("cls");
}

void Sales::ViewTopQsale()
{
	DataConn dc;
	cout << "Stock Summary\n" << endl;
	PreparedStatement* ps = dc.prepareStatement("select StockId, StockName, sum(Quantity) as Total_qty from salestock natural join stock group by stockid order by Total_qty DESC");
	ResultSet* rs = ps->executeQuery();
	if (rs->next() == false)
	{
		cout << "\nNo sales " << endl;
		goto op;
	}
	rs->beforeFirst();

	cout << "---------------------------------------------------------" << endl;
	cout << "Barcode\t\tStock Name\t\tQuantity sold" << endl;
	cout << "---------------------------------------------------------" << endl;
	while (rs->next())
	{
		cout << rs->getString("StockId") << "        " << rs->getString("StockName") << "\t\t\t" << rs->getInt("Total_qty") << endl;
	}
	cout << "---------------------------------------------------------" << endl;
op:
	rs->close();
	ps->close();
	cout << endl;
	system("pause");
	system("cls");
}

void Sales::MonthSales()
{
	DataConn dc;
	int month;
	int m = 0, n = 0;
	double stotal = 0.0;
	cout<<"Enter which month data you need: ";
	cin>>month;
	PreparedStatement* ps = dc.prepareStatement("SELECT SalesID, StockID, StockName, Quantity, Qprice from salestock NATURAL join stock natural join sales where Month(Sdate) = ? order by SalesID");
	ps->setInt(1, month);
	ResultSet* rs = ps->executeQuery();
	if (rs->next() == false)
	{
		cout << "\nNo Sales occured on " << month << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();
	
	cout << "\n--------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "SalesID\t\tBarcode\t\tStockName\t\tQuantity\t\tPrice\t\t\tTotal per Sales" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	int loop = 0;
	while (rs->next())
	{
		n = rs->getInt("SalesID");
		if (n != m)
		{
			cout << "\n" << rs->getInt("SalesID");
			ps = dc.prepareStatement("Select * from sales where SalesID = ?");
			ps->setInt(1, n);
			ResultSet* rs = ps->executeQuery();
			if (rs->next()) {
				stotal = rs->getDouble("Tsale");
			}
			cout << "\t\t\t\t\t\t\t\t\t\t\t\t\tRM" << fixed << setprecision(2) << stotal << endl;
		}
		cout << "\t\t" << rs->getString("StockID") << "\t" << rs->getString("StockName") << "\t\t" << rs->getInt("Quantity") << "\t\t\tRM " << fixed << setprecision(2) << rs->getDouble("Qprice") << endl;;
		//if(n!=m)
			//cout << "\t\t\t\t\t\t\t\t\t\t\t\t\tRM" << stotal << endl;
		m = rs->getInt("SalesID");
	}
	cout << "\n----------------------------------------------------------------------------------------------------------------------" << endl;
	rs->close();
	ps->close();
	cout << endl;

	ps = dc.prepareStatement("SELECT sum(Tsale) as total FROM sales where Month(Sdate)=?");
	ps->setInt(1, month);
	rs = ps->executeQuery();
	if (rs->next())
		cout << "Total sales for " << month << " is RM " << fixed << setprecision(2) << rs->getDouble("total") << endl;
	rs->close();
	ps->close();

	cout << endl;
	system("pause");
	system("cls");

}

void Sales::AllSales(string date)
{
	DataConn dc;
	int m = 0, n = 0;
	double stotal = 0.0;
	PreparedStatement* ps = dc.prepareStatement("SELECT SalesID, StockID, StockName, Quantity, Qprice from salestock NATURAL join stock natural join sales where Sdate = ? order by SalesID");
	ps->setString(1, date);
	ResultSet* rs = ps->executeQuery();
	if (rs->next() == false)
	{
		cout << "\nNo Sales occured on " << date << endl;
		system("pause");
		system("cls");
		return;
	}
	rs->beforeFirst();

	cout << "\n--------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "SalesID\t\tBarcode\t\tStockName\t\tQuantity\t\tPrice\t\t\tTotal per Sales" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	int loop = 0;
	while (rs->next())
	{
		n = rs->getInt("SalesID");
		if (n != m)
		{
			cout << "\n" << rs->getInt("SalesID");
			ps = dc.prepareStatement("Select * from sales where SalesID = ?");
			ps->setInt(1, n);
			ResultSet* rs = ps->executeQuery();
			if (rs->next()) {
				stotal = rs->getDouble("Tsale");
			}
			cout << "\t\t\t\t\t\t\t\t\t\t\t\t\tRM" << fixed << setprecision(2) << stotal << endl;
		}
		cout << "\t\t" << rs->getString("StockID") << "\t" << rs->getString("StockName") << "\t\t" << rs->getInt("Quantity") << "\t\t\tRM " << fixed << setprecision(2) << rs->getDouble("Qprice") << endl;;
		//if(n!=m)
			//cout << "\t\t\t\t\t\t\t\t\t\t\t\t\tRM" << stotal << endl;
		m = rs->getInt("SalesID");
	}
	cout << "\n----------------------------------------------------------------------------------------------------------------------" << endl;
	rs->close();
	ps->close();
	cout << endl;

	ps = dc.prepareStatement("SELECT sum(Tsale) as total FROM sales where Sdate=?");
	ps->setString(1, date);
	rs = ps->executeQuery();
	if (rs->next())
		cout << "Total sales for " << date << " is RM " << fixed << setprecision(2) << rs->getDouble("total") << endl;
	rs->close();
	ps->close();

	cout << endl;
	system("pause");
	system("cls");
}
