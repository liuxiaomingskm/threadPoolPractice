//
// Created by Apple on 3/4/23.
//

#ifndef THREADPOOL_CONNECTIONPOOL_H
#define THREADPOOL_CONNECTIONPOOL_H

#include <string>
#include <queue>
#include "Connection.h"
#include <mutex>
#include <atomic>
#include <memory>
#include <functional> // 绑定器
#include <thread>
using namespace std;

/*
 * 实现连接池功能模块
 */
class ConnectionPool {
public:
    //获取连接池实例
    static ConnectionPool* getConnectionPool();
    //给外部提供接口，从连接池获取一个可用链接
    shared_ptr<Connection> getConnection();
private:
    ConnectionPool(); //构造函数私有化 -> 单例模式

    bool loadConfigFile(); //从配置文件中加载配置项

    //运行在独立的线程中，专门负责生产新的mysql链接
    void produceConnectionTask();

    //运行在独立线程中，专门负责消灭超过idleTime的链接
    void scannerConnectionTask();

    string _ip;
    unsigned short _port;
    string _username;
    string _password;
    string _dbname;

    int _initSize; // initial size
    int _maxSize; //max allowed size
    int _maxIdleTime; //max idle time
    int _connectionTimeout; //timeout for waiting connction

    queue<Connection*> _connectionQue;
    mutex _queueMutex;
    atomic_int _connectionCnt; //记录所创建的链接的总数量 不能超过maxSize
    condition_variable cv; //设置条件变量，用于链接生产线程和消费线程的通信

};


#endif //THREADPOOL_CONNECTIONPOOL_H
