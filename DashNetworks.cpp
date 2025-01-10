#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int main(void) {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    std::string querytly;
    std::string name;
    std::string database;
    std::string password;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        std::system("clear");
        std::cout<<"Ingresa el nombre de la DB: ";
        std::getline(std::cin, database);
        std::cout<<"Ingresa el usuario de DB: ";
        std::getline(std::cin, name);
        std::cout<<"Ingresa la contraseña de la DB: ";
        std::getline(std::cin, password);
        con = driver->connect("tcp://127.0.0.1:3306", name, password);
        stmt = con->createStatement();
        stmt->execute(database);
        system("clear");
        system("figlet MySQL");
        cout << "\n" << endl;

        while (true) {
            cout << "\e[1;91mQUERY>> ";
            std::getline(std::cin, querytly);
            if (querytly == "EXIT")
                break;
			if (querytly == "CLEAR")
				std::system("clear");
            try {
                if (querytly.find("SELECT * FROM ") == 0) {
                    std::string tableName = querytly.substr(14);  // Extract table name from the query
                    std::string fullQuery = "SELECT * FROM " + tableName;
                    res = stmt->executeQuery(fullQuery);

                    // Get column names
                    sql::ResultSetMetaData *metaData = res->getMetaData();
                    int columnCount = metaData->getColumnCount();
                    std::cout << std::endl;
                    for (int i = 1; i <= columnCount; i++) {
                        std::cout << std::left << std::setw(25) << metaData->getColumnName(i);
                    }
                    std::cout << std::endl;

                    while (res->next()) {
                        for (int i = 1; i <= columnCount; i++) {
                            std::cout << std::left << std::setw(25) << res->getString(i);
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                } else {
                    res = stmt->executeQuery(querytly);

                    while (res->next()) {
                        std::cout << ">> " << res->getString(1) << std::endl;
                    }
                }

                delete res;
            } catch (sql::SQLException &e) {
            	if(querytly == "CLEAR")
            		std::system("clear");
            	else
                	std::cout << "Error MYSQL: " << e.what() << std::endl;
            }
        }

        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cout << "Error MYSQL: " << e.what() << std::endl;
    }

    return 0;
}
