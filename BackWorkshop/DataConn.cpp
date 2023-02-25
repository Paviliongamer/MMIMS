#include "DataConn.h"

DataConn::DataConn()
{

	//SQL Driver
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
	//connection with woksop sql
	connection->setSchema("mini_mark");

}

DataConn::~DataConn()
{
	connection->close();
	delete connection;
}

sql::PreparedStatement* DataConn::prepareStatement(std::string query)
{
	return connection->prepareStatement(query);
}