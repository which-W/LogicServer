#pragma once
#include<thread>
#include<map>
#include<functional>
#include<iostream>
#include<queue>
#include"Singleton.h"
#include"CSession.h"
#include<json/reader.h>
#include<json/value.h>
#include<json/json.h>
typedef function<void(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)> FunCallBack;


class LogicSystem : public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQueue(std::shared_ptr<LogicNode> msg);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();
	void HelloWordCallBack(std::shared_ptr<CSession> session, short msg_id, string msg_data);

	std::queue<std::shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::thread _worker_thread;
	std::condition_variable _consume;
	bool _b_stop;
	//将消息的id与回调函数相绑定
	std::map<short, FunCallBack> _func_callback;
};

