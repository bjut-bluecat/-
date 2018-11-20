//
//  main.cpp
//  Server
//
//  Created by 蓝猫 on 2018/11/20.
//  Copyright © 2018年 蓝猫. All rights reserved.
//

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <memory>
//#include <error.h>
using namespace std;
typedef struct my_msg
{
    long int my_msg_type;//数据类型
    char text[BUFSIZ];//消息缓冲区大小
}msgbuf;
class Server
{
private:
    int runningFlg;
    int msgid;
    long int msg_to_receive;
public:
    Server();
    void GetMsg();
};
Server::Server():runningFlg(1),msg_to_receive(0)
{
    msgid=msgget((key_t)1234, 0666|IPC_CREAT);
    if(msgid==-1)
    {
        cout<<"create error"<<endl;
        exit(1);
    }
    else
    {
        cout<<"create message queue sucess!"<<endl;
    }
}
void Server::GetMsg()
{
    while (runningFlg)
    {
        msgbuf msg;
        if(msgrcv(msgid, (void *)&msg, BUFSIZ, msg_to_receive, 0)==-1)
        {
            perror("receive failed\n");
            exit(1);
        }
        else
        {
            cout<<"receive success!"<<endl;
        }
        cout<<"接收的信息是："<<msg.text<<endl;
        if(strncmp(msg.text, "end", 3)==0)
        {
            runningFlg=0;
        }
    }
    if(msgctl(msgid, IPC_RMID, 0)==-1)
    {
        perror("删除消息队列失败\n");
        exit(1);
    }
    
}
int main(int argc, const char * argv[])
{
    cout<<"服务端程序"<<endl;
    shared_ptr<Server> cli=make_shared<Server>();
    cli->GetMsg();
    return 0;
}
