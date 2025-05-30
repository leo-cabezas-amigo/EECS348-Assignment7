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
#include <string>   // Implements std::string.
#include <iomanip>  // To format table outputs with std::setw().

// MySQL C++ libraries
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

            // Print column headers.
            for (int i = 1; i <= num_cols; ++i) {
                std::cout << std::setw(15) << std::left << metadata->getColumnLabel(i);
            }
            std::cout << std::endl;

            // Print separator line.
            for (int i = 1; i <= num_cols; ++i) {
                std::cout << std::setw(15) << std::left << std::string(15, '-');
            }
            std::cout << std::endl;

            // Print all rows.
            while (res->next()) {
                for (int i = 1; i <= num_cols; ++i) {
                    std::cout << std::setw(15) << std::left << res->getString(i);
                }
                std::cout << "\n";
            }
        }

        void executeQuery1(){
            sql::ResultSet* res;    // Stores the query results.

            std::string query = "SELECT * FROM Student WHERE StdMajor = 'IS';";

            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery2() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT s.StdNo, s.StdFirstName, s.StdLastName, s.StdMajor
                FROM Student s
                JOIN Enrollment e ON s.StdNo = e.StdNo
                GROUP BY s.StdNo, s.StdFirstName, s.StdLastName, s.StdMajor
                HAVING COUNT(DISTINCT e.OfferNo) > 2;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery3() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT FacNo, FacFirstName, FacLastName, FacDept, FacRank, FacHireDate
                FROM Faculty
                WHERE FacDept = 'Physics' 
                AND TIMESTAMPDIFF(YEAR, FacHireDate, CURDATE()) > 5;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery4() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT StdMajor, COUNT(*) AS TotalStudents
                FROM Student
                GROUP BY StdMajor
                HAVING COUNT(*) > 50;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery5() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT c.CourseNo, c.CrsDesc
                FROM Course c
                JOIN Offering o ON c.CourseNo = o.CourseNo
                JOIN Faculty f ON o.FacNo = f.FacNo
                WHERE c.CrsDesc LIKE '%Data%'
                AND f.FacLastName = 'JOHNSON';
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery6() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT s.StdNo, s.StdFirstName, s.StdLastName
                FROM Student s
                WHERE NOT EXISTS (
                    SELECT 1
                    FROM Enrollment e
                    JOIN Offering o ON e.OfferNo = o.OfferNo
                    WHERE e.StdNo = s.StdNo
                    AND ((
                            -- Check if the student enrolled in the current semester
                            (MONTH(CURDATE()) IN (12, 1) AND o.OffTerm = 'WINTER' AND o.OffYear = YEAR(CURDATE()))  -- Winter
                            OR (MONTH(CURDATE()) IN (2, 3, 4, 5) AND o.OffTerm = 'SPRING' AND o.OffYear = YEAR(CURDATE()))  -- Spring
                            OR (MONTH(CURDATE()) IN (6, 7) AND o.OffTerm = 'SUMMER' AND o.OffYear = YEAR(CURDATE()))  -- Summer
                            OR (MONTH(CURDATE()) IN (8, 9, 10, 11) AND o.OffTerm = 'FALL' AND o.OffYear = YEAR(CURDATE()))  -- Fall
                        ) OR (
                            -- Check if the student enrolled in the previous semester
                            (MONTH(CURDATE()) IN (12, 1) AND o.OffTerm = 'FALL' AND o.OffYear = YEAR(CURDATE()) - 1)  -- Previous Fall
                            OR (MONTH(CURDATE()) IN (2, 3, 4, 5) AND o.OffTerm = 'WINTER' AND o.OffYear = YEAR(CURDATE()))  -- Previous Winter
                            OR (MONTH(CURDATE()) IN (6, 7) AND o.OffTerm = 'SPRING' AND o.OffYear = YEAR(CURDATE()))  -- Previous Spring
                            OR (MONTH(CURDATE()) IN (8, 9, 10, 11) AND o.OffTerm = 'SUMMER' AND o.OffYear = YEAR(CURDATE()))  -- Previous Summer
                        )
                    )
                );
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }       

        void executeQuery7() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT StdNo, StdFirstName, StdLastName, MAX(StdGPA) AS 2ndHighestGPA
                FROM Student
                WHERE StdGPA < (SELECT MAX(StdGPA) FROM Student);
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery8() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT s.StdNo, s.StdFirstName, s.StdLastName, StdGPA
                FROM Student s
                JOIN Faculty f ON s.StdNo = f.FacNo
                WHERE f.FacRank = 'ASST' AND s.StdGPA > 3.5;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery9() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT s.StdNo, s.StdFirstName, s.StdLastName, o.CourseNo, o.OffTerm, o.OffYear
                FROM Student s
                JOIN Enrollment e ON s.StdNo = e.StdNo
                JOIN Offering o ON e.OfferNo = o.OfferNo
                WHERE o.OffYear > 2022
                ORDER BY s.StdNo;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }  

        void executeQuery10() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query = R"(
                SELECT FacNo, FacFirstName, FacLastName, FacSalary
                FROM Faculty
                ORDER BY FacSalary DESC
                LIMIT 3;
            )";
            
            res = this->stmt->executeQuery(query);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery11() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query1 = R"(
                INSERT IGNORE INTO Student (StdNo, StdFirstName, StdLastName, StdCity, StdState, StdZip, StdMajor, StdClass, StdGPA)
                VALUES ('888-88-8888', 'ALICE', 'SMITH', 'TOPEKA', 'KS', '66610', 'CS', 'JR', 3.85);
            )";
            std::string query2 = "SELECT * FROM Student;";
            
            this->stmt->execute(query1);
            res = this->stmt->executeQuery(query2);
            printMatches(res);
            std::cout << "\n";
            return;
        }

        void executeQuery12() {
            sql::ResultSet* res;  // Stores the query results.
            
            // Raw string literal for a multi-line SQL query
            std::string query1 = R"(
                UPDATE Student
                SET StdCity = 'OVERLAND PARK', StdZip = '66204'
                WHERE StdFirstName = 'BOB' AND StdLastName = 'NORBERT';
            )";
            std::string query2 = "SELECT * FROM Student;";
            
            this->stmt->execute(query1);
            res = this->stmt->executeQuery(query2);
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
    
    std::cout << "\n";
    std::cout << "=============================================================== SQL QUERY 1 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Retrieve all students majoring in 'IS'.\n";
    std::cout << "\n";
    sql.executeQuery1();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 2 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Find the names of students who have enrolled in more than two courses.\n";
    std::cout << "\n";
    sql.executeQuery2();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 3 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> List all professors who have been teaching for more than 5 years in the 'Physics' department.\n";
    std::cout << "\n";
    sql.executeQuery3();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 4 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Retrieve the total number of students enrolled in each department, but only for departments with more than 50 students.\n";
    std::cout << "\n";
    sql.executeQuery4();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 5 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Find the course names and IDs of courses that have 'Data' in their title and are taught by 'Dr. Johnson'. \n";
    std::cout << "\n";
    sql.executeQuery5();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 6 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Display the students who have not enrolled in any courses in the past two semesters. \n";
    std::cout << "\n";
    sql.executeQuery6();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 7 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Retrieve the second-highest GPA among students. \n";
    std::cout << "\n";
    sql.executeQuery7();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 8 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Find the names of students who are also teaching assistants but have a GPA above 3.5. \n";
    std::cout << "\n";
    sql.executeQuery8();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 9 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> List all students along with their enrolled courses, but only for those who enrolled after 2022. \n";
    std::cout << "\n";
    sql.executeQuery9();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 10 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Retrieve the names and salaries of the top three highest-paid professors. \n";
    std::cout << "\n";
    sql.executeQuery10();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 11 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Insert a Junior Computer Science student named 'Alice Smith'd from Topeka, Kansas, 66610 with student number 888-88-8888 and\n";
    std::cout << "======> GPA 3.85 into the student table. Show the result using SELECT * FROM Student. \n";
    std::cout << "\n";
    sql.executeQuery11();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    std::cout << "=============================================================== SQL QUERY 12 ===============================================================\n";
    std::cout << "\n";
    std::cout << "======> Student Bob Norbert has moved to Overland Park, Kansas. Update the StdCity and StdZip in the student table to reflect this change.\n";
    std::cout << "======> Show the result using SELECT * FROM Student. \n";
    std::cout << "\n";
    sql.executeQuery12();
    std::cout << "===========================================================================================================================================\n";
    std::cout << "\n";

    return;
}

int main(int argc, char** argv){
    executeAllQueries();
    return 0;
}