//
// Created by Apple on 3/4/23.
//
#pragma once
#include <mysql.h>
#include <string>
#include <ctime>
using namespace std;

class Connection
{
public:
    Connection();
    ~Connection();

    //链接数据库
    bool connect(string ip,
                 unsigned short port,
                 string user,
                 string password,
                 string dbname);

    //更新操作
    bool update(string sql);

    //查询操作
    MYSQL_RES* query(string sql);

    void refreshAliveTime() noexcept {_alivetime= clock();}

    clock_t getAliveTime() const noexcept {return clock() - _alivetime;}

private:
    MYSQL *_conn;
    clock_t _alivetime; //记录链接进入队列的时间点





};