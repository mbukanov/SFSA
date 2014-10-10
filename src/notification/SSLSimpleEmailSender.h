#ifndef SSLSimpleEmailSender_H
#define SSLSimpleEmailSender_H


#include <iostream>
#include <netdb.h>
#include <cstring>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../base64.h"


#define bzero(ptr) memset(ptr, 0, sizeof(ptr))


class SSLSimpleEmailSender
{
public:
	SSLSimpleEmailSender();
	void setHost(char *hostname);
	void setPort(std::string port);
	void setMyEmail(char *myemail);
	void setLogin(char *login); // in base64
	void setPassword(char *password); // in base64
	void setFrom(char *from);
	void setTo(char *to);
	void setSubject(char *subject);
	void setData(char *data);
	void connect();

	int Send();

protected:
	void sendPacket(char *buff);

	bool isCharStringEmpty(char* str)
	{
		std::cout<<"STR:"<<str<<std::endl;
		if(!strlen(str))
			return true;
		return false;
	}


private:
	char hostname[128];
	char port[6];
	char login[128];
	char myEmail[128];
	char base64Login[128];
	char password[128];
	char base64Password[128];
	char from[128];
	char to[128];
	char subject[1024];
	char data[4096];
	
	SSL_CTX* ctx;
	SSL *ssl;
	BIO* bio;
};

#endif