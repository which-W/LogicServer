#include <iostream>
#include "CServer.h"
#include <thread>
#include <mutex>
#include <csignal>
#include "AsioIOServicePool.h"
using namespace std;

//bool bstop = false;
//std::condition_variable cond_quit;
//std::mutex mutex_quit;
//
//void sig_handler(int sig) {
//    if (sig == SIGINT || sig == SIGTERM) { //SIGINT捕获的是ctrl + c SIGTERM捕获的是q
//        std::unique_lock<std::mutex> unique_mutex(mutex_quit);
//        bstop = true;
//        cond_quit.notify_one();
//    }
//}
int main()
{
    try {
        auto pool = AsioIOServicePool::GetInstance();
        boost::asio::io_context  io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM); //注册信号
        signals.async_wait([&io_context,pool](auto, auto) {
            io_context.stop();
            pool->Stop();
            });
        CServer s(io_context, 10086);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
}