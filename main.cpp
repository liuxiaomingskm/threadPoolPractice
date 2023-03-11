#include "pch.h"
#include <iostream>
#include "Connection.h"
#include "CommonConnectionPool.h"
using namespace std;

int main() {


// single thread to test running time without using thread pool
//    clock_t begin = clock();
//
//    Connection conn;
//    conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
//
//    for (int i = 0; i < 1000; i++) {
//        Connection conn;
//        conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
//        char sql[1024] = {0};
//        sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
//        conn.update(sql);
//    }
//
//    clock_t end = clock();
//    cout << "The time take is : " << (end - begin) << "ms" << endl;

//    four threads to test running time without using thread pool

  clock_t begin = clock();

 // 不知道为什么这里必须创建Connection实例。。。
    // Connection conn;
    // conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");

    thread t1([](){

        for (int i = 0; i < 2500; i++) {
            Connection conn;
            conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn.update(sql);
        }
    });

    thread t2([](){

        for (int i = 0; i < 2500; i++) {
            Connection conn;
            conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn.update(sql);
        }
    });

    thread t3([](){

        for (int i = 0; i < 2500; i++) {
            Connection conn;
            conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn.update(sql);
        }
    });

    thread t4([](){

        for (int i = 0; i < 2500; i++) {
            Connection conn;
            conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn.update(sql);
        }
    });

    thread t5([](){

        for (int i = 0; i < 2500; i++) {
            Connection conn;
            conn.connect("127.0.0.1", 3306, "root","liu54420322","chat");
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn.update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    clock_t end = clock();
    cout << "The time take is : " << (end - begin) << "ms" << endl;


//    four threads to test running time with thread pool

/*
    clock_t  begin = clock();
    thread t1([](){

            ConnectionPool *cp = ConnectionPool::getConnectionPool();
            for (int i = 0; i < 250; i++) {
                shared_ptr<Connection> conn = cp->getConnection();
                char sql[1024] = {0};
                sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
                conn->update(sql);
        }
    });

    thread t4([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++) {
            shared_ptr<Connection> conn = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++) {
            shared_ptr<Connection> conn = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn->update(sql);
        }
    });

    thread t3([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 250; i++) {
            shared_ptr<Connection> conn = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s', %d, '%s')","zhangsan",18,"male");
            conn->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    clock_t end = clock();
    cout << "The time take is : " << (end - begin) << "ms" << endl;

*/
    return 0;
}