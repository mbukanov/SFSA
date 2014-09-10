#include <iostream>
#include <netdb.h>
#include "../base64.h"
#include "../baseMols.h"

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

SimpleEmailSender::SimpleEmailSender()
{
	bzero(hostname);
	bzero(login);
	bzero(password);
	bzero(from);
	bzero(to);
	bzero(subject);
	bzero(data);
	memset(&servaddr, 0, sizeof(servaddr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
}

void SimpleEmailSender::setMyEmail(char *myemail)
{
	strcpy(myEmail, myemail);
}

void SimpleEmailSender::sendPacket(int sockfd, char *buff)
{
	char recvline[2048];
	bzero(recvline);

	std::cout<<"SEND: "<<buff<<std::endl;
	send(sockfd, buff, strlen(buff), 0);
	recv(sockfd, recvline, sizeof(recvline), 0);
	std::cout<<"RECV: "<<recvline<<std::endl;
}

void SimpleEmailSender::setHost(char *hostname)
{
	strcpy(this->hostname, hostname);
	host = gethostbyname(hostname);
	memcpy((char*)&(servaddr.sin_addr), host->h_addr, host->h_length);
}

void SimpleEmailSender::setPort(int port)
{
	this->port = port;
	servaddr.sin_port = htons(port);
}

void SimpleEmailSender::setLogin(char *login)
{
	strcpy(this->login, login);
	char *base64;
	base64_encode(login, &base64);
	strcpy(this->base64Login, base64);
}

void SimpleEmailSender::setPassword(char *password)
{
	strcpy(this->password, password);
	char *base64;
	base64_encode(password, &base64);
	strcpy(this->base64Password, base64);
}

void SimpleEmailSender::setFrom(char *from)
{
	strcpy(this->from, from);
}

void SimpleEmailSender::setTo(char *to)
{
	strcpy(this->to, to);
}

void SimpleEmailSender::setSubject(char *subject)
{
	strcpy(this->subject, subject);
}

void SimpleEmailSender::setData(char *data)
{
	strcpy(this->data, data);
}

int SimpleEmailSender::Send()
{
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connect error\n");
		return -1;
	}

	char buff[256];
	bzero(buff);

	recv(sockfd, buff, sizeof(buff), 0);
	bzero(buff);

	strcpy(buff, "EHLO ");
	strcat(buff, hostname);
	strcat(buff, "\r\n");

	sendPacket(sockfd, buff);
	sendPacket(sockfd, (char*)"AUTH LOGIN\r\n");

	bzero(buff);
	strcpy(buff, base64Login);
	strcat(buff, "\r\n");
	sendPacket(sockfd, buff);

	bzero(buff);
	strcpy(buff, base64Password);
	strcat(buff, "\r\n");
	sendPacket(sockfd, buff	);

	// shit-code...

	bzero(buff);
	strcpy(buff, (char*)"MAIL FROM:<");
	strcat(buff, myEmail);
	strcat(buff, (char*)">\r\n");
	sendPacket(sockfd, buff);

	bzero(buff);
	strcpy(buff, "RCPT TO:<");
	strcat(buff, to);
	strcat(buff, ">\r\n");
	sendPacket(sockfd, buff);
	sendPacket(sockfd, (char*)"DATA\r\n");

	bzero(buff);
	strcpy(buff, (char*)"From:");
	strcat(buff, from);
	strcat(buff, (char*)"<");
	strcat(buff, myEmail);
	strcat(buff, (char*)">\r\n");

	strcat(buff, (char*)"TO:");
	strcat(buff, to);
	strcat(buff, (char*)"\r\nSubject:");
	strcat(buff, subject);
	strcat(buff, (char*)"\r\n");
	strcat(buff, data);
	strcat(buff, (char*)"\r\n.\r\n");
	sendPacket(sockfd, buff);

	sendPacket(sockfd, (char*)"QUIT\r\n");

	return 0;
}

class NotificationEmail : public BaseMOLS
{
public:
	virtual void Alert();
private:
};

void NotificationEmail::Alert()
{
	SimpleEmailSender email;
	email.setHost((char*)"smtp.list.ru");
	email.setPort(25);
	email.setFrom((char*)"SFSA");
	email.setMyEmail((char*)"yourmail@hostname");
	email.setTo((char*)"dqamex@gmail.com");
	email.setSubject((char*)"This is SUBJECT!!!");
	email.setLogin((char*)"yourlogin");
	email.setPassword((char*)"yourpassword");
	email.setData((char*)"ok");

	email.Send();
}
