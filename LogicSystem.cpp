#include "LogicSystem.h"

LogicSystem::LogicSystem() :_b_stop(false) {
	RegisterCallBacks();
	_worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg() {
    for (;;) {
        std::unique_lock<std::mutex> unique_lk(_mutex);
        //�ж϶���Ϊ�������������������ȴ������ͷ���
        while (_msg_que.empty() && !_b_stop) {
            _consume.wait(unique_lk);
        }
        //�ж��Ƿ�Ϊ�ر�״̬���������߼�ִ��������˳�ѭ��
        if (_b_stop) {
            while (!_msg_que.empty()) {
                auto msg_node = _msg_que.front();
                cout << "recv_msg id  is " << msg_node->_recvnode->GetRecMsgNodeID() << endl;
                auto call_back_iter = _func_callback.find(msg_node->_recvnode->GetRecMsgNodeID());
                if (call_back_iter == _func_callback.end()) {
                    _msg_que.pop();
                    continue;
                }
                call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetRecMsgNodeID(),
                    std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
                _msg_que.pop();
            }
            break;
        }
        //���û��ͣ������˵��������������
        auto msg_node = _msg_que.front();
        cout << "recv_msg id  is " << msg_node->_recvnode->GetRecMsgNodeID() << endl;
        auto call_back_iter = _func_callback.find(msg_node->_recvnode->GetRecMsgNodeID());
        if (call_back_iter == _func_callback.end()) {
            _msg_que.pop();
            continue;
        }
		//����HandHead��HandleMsg�Ļص�����
        call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetRecMsgNodeID(),
            std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));
        //����HandHead����
      /*  call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetRecMsgNodeID(),
            std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));*/
        _msg_que.pop();
     }
}

void LogicSystem::PostMsgToQueue(shared_ptr<LogicNode> msg) {
    std::unique_lock<std::mutex> unique_lk(_mutex);
    _msg_que.push(msg);
    //��0��Ϊ1����֪ͨ�ź�
    if (_msg_que.size() == 1) {
        unique_lk.unlock();
        _consume.notify_one();
    }
}
void LogicSystem::HelloWordCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	cout << "HelloWordCallBack msg id is " << msg_id << " msg data is " << msg_data << endl;
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
        << root["data"].asString() << endl;
    root["data"] = "server has received msg, msg data is " + root["data"].asString();
    std::string return_str = root.toStyledString();
    session->Send(return_str, root["id"].asInt());
}

void LogicSystem::RegisterCallBacks() {
	_func_callback[MSG_HELLO_WORLD] = std::bind(&LogicSystem::HelloWordCallBack,this
		, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

LogicSystem::~LogicSystem() {
    _b_stop = true;
    _consume.notify_one();
    _worker_thread.join();
}