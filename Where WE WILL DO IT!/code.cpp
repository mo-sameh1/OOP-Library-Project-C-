#include <bits/stdc++.h>
#include <libpq-fe.h>
using namespace std;

int main()
{
    PGresult *result;
    PGconn *connection;
    connection = PQconnectdb("host=34.136.252.133 port=5432  dbname=testdb2 user=testuser password=123");
    if (PQstatus(connection) == CONNECTION_OK)
    {
        printf("connection made\n");
    }
    else
        printf("connection failed: %s\n", PQerrorMessage(connection));

    PQfinish(connection);
    return 0;
}
