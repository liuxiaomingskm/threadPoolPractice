#include "pch.h"
#include "public.h"
#include "Connection.h"
#include <iostream>
#include <mutex>
using namespace std;

std::mutex g_MTX;
Connection::Connection() {
    std::lock_guard<std::mutex> lock(g_MTX);
    // wrap mysql connection
    _conn = mysql_init(nullptr);
    if(_conn == nullptr) {cout << "_conn is null!" << endl; }
}

Connection::~Connection() {
    if (_conn != nullptr){
        mysql_close(_conn);
    }
}
bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname) {
    MYSQL *p = mysql_real_connect(_conn,ip.c_str(), user.c_str(),password.c_str(),dbname.c_str(),
                                  port, nullptr,0);
    return p != nullptr;
}

bool Connection::update(string sql) {
    if (mysql_query(_conn, sql.c_str())){
        LOG("Update failed: " + sql);
        return false;
    }
    return true;
}

MYSQL_RES* Connection::query(string sql) {
    if(mysql_query(_conn, sql.c_str())){
        LOG("Query failed: " + sql);
        return nullptr;
    }
    return mysql_use_result(_conn);
}