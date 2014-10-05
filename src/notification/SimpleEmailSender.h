#ifndef SIMPLEEMAILSENDER_H
#define SIMPLEEMAILSENDER_H


#include <iostream>
#include <netdb.h>
#include <cstring>

#define bzero(ptr) memset(ptr, 0, sizeof(ptr))


class SimpleEmailSender
{
public:
	SimpleEmailSender();
	void setHost(char *hostname);
	void setPort(int port);
	void setMyEmail(char *myemail);
	void setLogin(char *login); // in base64
	void setPassword(char *password); // in base64
	void setFrom(char *from);
	void setTo(char *to);
	void setSubject(char *subject);
	void setData(char *data);

	int Send();

protected:
	void sendPacket(int sockfd, char *buff);

	bool isCharStringEmpty(char* str)
	{
		std::cout<<"STR:"<<str<<std::endl;
		if(!strlen(str))
			return true;
		return false;
	}


private:
	char hostname[128];
	int port;
	char login[128];
	char myEmail[128];
	char base64Login[128];
	char password[128];
	char base64Password[128];
	char from[128];
	char to[128];
	char subject[1024];
	char data[4096];
	int sockfd;
	struct hostent *host;
	struct sockaddr_in servaddr;
};

#endif