#include <mysql_driver.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include <string>
#include <iomanip>
using namespace sql;

#ifndef DATACONN_H
#define DATACONN_H

class DataConn
{
private:
	sql::Connection* connection;

public:
	DataConn();
	~DataConn();
	sql::PreparedStatement* prepareStatement(std::string query);
};
#endif
