//
// Created by Apple on 3/4/23.
//
#include "pch.h"
#include "CommonConnectionPool.h"
#include "public.h"
#include <iostream>
#include <condition_variable>


using namespace std;

//线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool() {
    // the pool is a thread safe and created exact once, we call it the static initialization order fiasco idiom,
    // refer https://www.modernescpp.com/index.php/thread-safe-initialization-of-data
    // 静态局部变量的初始化， 有编译器自动进行lock和unlock
    static ConnectionPool pool;
    return &pool;
}

//从配置文件中加载配置项
bool ConnectionPool::loadConfigFile() {
    FILE * pf = fopen("../mysql.cnf", "r");
    if (pf == nullptr) {
        LOG("mysql.cnf file is not existed!");
        return false;
    }

    while (!feof(pf)) {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find('=',0);
        if (idx == -1) {continue;}

        //password = 123456\n
        int endidx = str.find('\n',idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx+1, endidx - idx - 1);

// switch statement is not permitted for string
        if (key == "ip") {
            _ip = value;
        } else if (key == "port") {
            _port = atoi(value.c_str());
        } else if (key == "username") {
            _username = value;
        } else if (key == "password") {
            _password = value;
        } else if (key == "dbname") {
            _dbname = value;
        } else if (key == "initSize") {
            _initSize = atoi(value.c_str());
        } else if (key == "maxSize") {
            _maxSize = atoi(value.c_str());
        } else if (key == "maxIdleTime") {
            _maxIdleTime = atoi(value.c_str());
        } else if (key == "connectionTimeOut") {
            _connectionTimeout = atoi(value.c_str());
        }
    }
    return true;
}

ConnectionPool::ConnectionPool() {
    //加载配置项
    if (!loadConfigFile()) {
        LOG("Something wrong with the configuration file!");
        return;
    }

    //创建初始数量的链接
    for (int i = 0; i < _initSize; i++) {
        Connection* p = new Connection();
        p ->connect(_ip, _port,_username,_password,_dbname);
        p -> refreshAliveTime(); //刷新进入队列时间点
        _connectionQue.push(p);
        _connectionCnt++;
    }

    //启动一个新线程，作为连接的生产者 linux thread = pthread -> create
//    ConnectionPool obj; 这里不能自建实例然后调用该实例的produceConnectionTask方法，因为实例存在父进程，而子线程无法访问，会跳出SEGBUS
// 错误，正确的是用当前例子 也就是static thread pool，因为是static变量 所以子线程也能访问
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();

    //启动一个新线程，扫描超过maxIdleTime时间的链接并回收
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

void ConnectionPool::produceConnectionTask() {
    for (;;) {
        unique_lock<mutex> lock(_queueMutex); // 局部unique lock，退出时即使遇见exception自动解锁
        while (!_connectionQue.empty() || _connectionCnt >= _maxSize) {
            cv.wait(lock); //队列非空，生产线程处于等待状态
        }

        //链接数量没有达到上限，继续创建新的连接
            Connection* p = new Connection();
            p -> connect(_ip, _port, _username, _password, _dbname);
            p -> refreshAliveTime();
            _connectionQue.push(p);
            _connectionCnt++;


        //通知消费者线程，可以消费了
        cv.notify_all();
    }
}

//给外部提供接口，从链接池中获取一个可用的空闲链接
shared_ptr<Connection> ConnectionPool::getConnection() {
    unique_lock<mutex> lock(_queueMutex);
    while (_connectionQue.empty()) {
        //sleep
        if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout))) {
            if (_connectionQue.empty()) {
                LOG("获取空闲链接超时了。。。获取链接失败！");
                return nullptr;
            }
        }

    }

    //更改默认析构函数，将connection放回pool中,自定义析构函数，自定义删除器
    shared_ptr<Connection> sp(_connectionQue.front(),[&](Connection *pcon){
        //这里是在服务器应用线程中调用的，所以一定要考虑线程安全
        unique_lock<mutex> lock(_queueMutex);
        pcon->refreshAliveTime();//刷新一下开始空闲的起始时间
        _connectionQue.push(pcon);
    });

    _connectionQue.pop();
    cv.notify_all(); // 通知生产者生产

    return sp;
}

// 扫描超过maxIdleTime时间的空闲链接，进行对链接的回收
void ConnectionPool::scannerConnectionTask() {
    for(;;){
        // 通过sleep模拟定时效果
        cout << "begin to run scanner" << endl;
        this_thread::sleep_for(chrono::seconds (_maxIdleTime));

        //扫描整个队列，释放多余的链接
        unique_lock<mutex> lock(_queueMutex);
        while (!_connectionQue.empty()) {
                Connection* p = _connectionQue.front();
                cout << "get Alive time is : " << p -> getAliveTime() << endl;
                if (p-> getAliveTime() >= (_maxIdleTime * 1000)) {
                    _connectionQue.pop();
                    _connectionCnt--;
                    delete p; //调用~Connection()释放链接
                    cout << "delete p" << endl;
                    cv.notify_all();
                } else {break;}
        }
        }
}