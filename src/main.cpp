#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include "Menu.h"

using namespace std;

int main() {

  try {

        string username = "badblues";
        string password = "root";
        string driver = "{PostgreSQL Unicode}";
        string database = "rpbd";
        string server = "localhost";
        string port = "5432";


        string connectionString = "DRIVER=" + driver + ";DATABASE=" + database + ";SERVER=" +
                                  server + ";PORT=" + port + ";UID=" + username + ";PWD=" + password + ";";

        SQLHENV hEnv;
        SQLHDBC hDbc;


        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);

        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
        auto *connStr = (SQLCHAR *) connectionString.c_str();
        SQLRETURN sqlreturn = SQLDriverConnect(hDbc, nullptr, connStr, SQL_NTS, nullptr,
                                               0, nullptr, SQL_DRIVER_COMPLETE);

        if (sqlreturn == SQL_SUCCESS || sqlreturn == SQL_SUCCESS_WITH_INFO) {
            Menu menu = new Menu(hDbc);
            menu.showMenu();
            SQLDisconnect(hDbc);
        } else {
            cerr << "Connection failed\n";
            cerr << sqlreturn << "\n";
            cerr << connectionString << "\n";
        }

        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return 1;
    }


    return 0;
}

