// NOTE TO SELF: PASSWORD TO MYSQL SERVER IS EXPOSED. DO NOT PUBLISH AS IS.
/*
-------------------------------------------------------------------------------------
File name       = Assignment7.cpp
Creation date   = 4/23/2025
-------------------------------------------------------------------------------------
Author          = Leo Cabezas Amigo (KUID 3109786)
Class           = EECS 348
Assignment #    = 7
Description     = 
-------------------------------------------------------------------------------------
Inputs          = 
Outputs         = 
-------------------------------------------------------------------------------------
Collaborators   = None.
Sources used    = 
-------------------------------------------------------------------------------------
*/

#include <iostream> // Basic C++ I/O library.
#include <fstream>  // Implements file streams.
//#include <sstream>  // Implements string streams.
#include <string>   // Implements std::string.
//#include <vector>   // Implements std::vector.
#include <iomanip>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

class SQLPractice {
    public:
        SQLPractice(std::string ip, std::string username, std::string password){
            this->ip = ip;
            this->username = username;
            this->password = password;

            this->connectSQL();
            return;
        }

        void printMatches(sql::ResultSet* res) {
            sql::ResultSetMetaData* metadata = res->getMetaData();
            int num_cols = metadata->getColumnCount();

            // Print column headers
            for (int i = 1; i <= num_cols; ++i) {
                std::cout << std::setw(15) << std::left << metadata->getColumnLabel(i);
            }
            std::cout << std::endl;

            // Print separator line
            for (int i = 1; i <= num_cols; ++i) {
                std::cout << std::setw(15) << std::left << std::string(15, '-');
            }
            std::cout << std::endl;

            // Print all rows
            while (res->next()) {
                for (int i = 1; i <= num_cols; ++i) {
                    std::cout << std::setw(15) << std::left << res->getString(i);
                }
                std::cout << "\n";
            }
        }

        void executeQuery1(){
            sql::ResultSet* res;    // Stores the query results.
            res = this->stmt->executeQuery("SELECT * FROM Student WHERE StdMajor = 'IS';");
            printMatches(res);
            std::cout << "\n";
            return;
        }


    private:
        std::string ip;
        std::string username;
        std::string password;

        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;               // To establish the connection.
        sql::Statement* stmt;               // To execute an SQL statement. 

        void connectSQL(){
            this->driver = sql::mysql::get_mysql_driver_instance();
            this->con = driver->connect(this->ip, this->username, this->password);
            this->stmt = con->createStatement();
            
            this->stmt->execute("USE " + this->username + ";");
            return;
        }
};

void executeAllQueries(){
    std::string ip = "mysql.eecs.ku.edu";
    std::string username = "348s25_l859c248";
    std::string password = "aRi3vu3t";
    
    SQLPractice sql(ip, username, password);
    
    std::cout << "================================================ SQL QUERY 1 ================================================\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "======> Retrieve all students majoring in 'IS'.\n";
    std::cout << "-----------------------------------------------------\n";
    sql.executeQuery1();
    std::cout << "\n";

    return;
}

int main(int argc, char** argv){
    /*
    std::ifstream file;
    if (argc > 1){  // If a filepath is specified (argv[1]).
        file.open(argv[1]);     // Opens file specified by runtime args.
        if (!file.is_open()){
            std::cerr << "Error in 'main()': Could not open specified file.\n";
            return 1;
        }
    } else {        // If no filepath is specified.
        file.open("Assignment7_Test_File.txt");
        if (!file.is_open()){
            std::cerr << "Error in 'main()': Could not open default file 'Assignment7_Test_File.txt'.\n";
            return 1;
        }
    }
    */

    executeAllQueries();

    // file.close();   // Closes the file after reading its data.
    return 0;
}