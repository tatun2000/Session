#include "SourceCode.h"

int main()
{
    SimpleTimer st; // An object of the class that will be used to measure the execution time of the program
    setlocale(LC_ALL, "rus");
    MYSQL* conn; // Connection pointer (Handle) (id of the specific connection for the operation)
    MYSQL_ROW row = 0; // Variable from which we will read the result
    MYSQL_RES* res = 0; // Pointer to the query result
    conn = mysql_init(0); // Initializing a new MySQL object
    conn = mysql_real_connect(conn, "localhost", "root", "Dima10910000", "Session", 3306, NULL, 0); // Connecting the object to the database

    if (conn)
    {
        cout << "Successful connection to database!\n"; // Connected to the database
        Main_Session(conn, row, res); // Main Function
    }
    else
    {
        cout << "Connection to database has failed!\n"; // Connection error
    }
    return 0;
}

