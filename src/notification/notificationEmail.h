#ifndef NOTIFICATIONEMAIL_H
#define NOTIFICATIONEMAIL_H

/*
note: pattern Strategy

ConcreteStrategy NotificationEmail
*/


#include <iostream>
#include <netdb.h>
#include "notification.h"
#include "SimpleEmailSender.h"
#include "../IConfig/IConfig.h"

#define bzero(ptr) memset(ptr, 0, sizeof(ptr))


class NotificationEmail : public Notification
{
public:
	virtual void Alert()
	{
		SimpleEmailSender email;

		email.setHost((char*) IConfig::Instance()->get("email", "hostname").c_str());
		email.setPort(std::stoi(IConfig::Instance()->get("email", "port").c_str()));
		email.setFrom((char*) IConfig::Instance()->get("email", "from").c_str());
		email.setTo((char*) IConfig::Instance()->get("email", "to").c_str());
		email.setMyEmail((char*) IConfig::Instance()->get("email", "email").c_str());
		email.setSubject((char*) IConfig::Instance()->get("email", "subject").c_str());
		email.setLogin((char*) IConfig::Instance()->get("email", "login").c_str());
		email.setPassword((char*) IConfig::Instance()->get("email", "password").c_str());
		email.setData((char*) IConfig::Instance()->get("email", "data").c_str());
		
		try
		{
			email.Send();
		}catch(char const* e){
			std::cout<<"ERROR: "<<e<<std::endl;
			exit(0);
		}
	}
private:
};

#endif