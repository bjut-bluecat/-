//
//  main.cpp
//  Client
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
class Client
{
private:
    int runningflag;//运行标志
    int msgid;//消息标志服
public:
    Client();//构造方法
    void SendMsg();//客户端信息发送
    void str_char(string s,char *text);
};
Client::Client()
{
    runningflag=1;
    
    msgid=msgget((key_t)1234, IPC_CREAT|0666);//关键字值设置1234 权限666
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
void Client::SendMsg()
{
    //msgbuf msg;
    while (runningflag)
    {
        msgbuf msg;
        cout<<"请输入想要发送的信息"<<endl;
        //string temp;
        //cin>>temp;
        //str_char(temp, msg.text);
        fgets(msg.text, BUFSIZ, stdin);
        cout<<msg.text<<endl;
        if(msgsnd(msgid, (void*)&msg, BUFSIZ, 0)==-1)
        {
            perror("send failed\n");
            exit(1);
        }
        else
        {
            cout<<"send success!"<<endl;
        }
        if(strncmp(msg.text, "end", 3)==0)
        {
            runningflag=0;
        }
    }
}
void Client::str_char(string s,char *text)//字符串和char*的转换
{
    text=(char*)s.data();
}
int main(int argc, const char * argv[])
{
    cout<<"客户端程序"<<endl;
    shared_ptr<Client> cli=make_shared<Client>();
    cli->SendMsg();
    return 0;
}
