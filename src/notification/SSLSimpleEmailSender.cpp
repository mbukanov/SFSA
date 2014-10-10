#include "SSLSimpleEmailSender.h"

SSLSimpleEmailSender::SSLSimpleEmailSender()
{
	/* Init OpenSSL */
	SSL_load_error_strings();
	SSL_library_init();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	ctx = SSL_CTX_new(SSLv3_client_method());
	bio = BIO_new_ssl_connect(ctx);

	if(bio == NULL)
	{
		printf("Failure\n");
		// throw
	}else{
		printf("Success\n");
	}
	BIO_get_ssl(bio, &ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	bzero(hostname);
	bzero(login);
	bzero(password);
	bzero(from);
	bzero(to);
	bzero(subject);
	bzero(data);
}

void SSLSimpleEmailSender::setMyEmail(char *myemail)
{
	strcpy(myEmail, myemail);
}

void SSLSimpleEmailSender::sendPacket(char *buff)
{
	char recvline[2048];
	bzero(recvline);

	std::cout<<"SEND: "<<buff<<std::endl;

	BIO_write(bio, buff, strlen(buff));
	BIO_read(bio, recvline, sizeof(recvline));

	std::cout<<"RECV: "<<recvline<<std::endl;
}

void SSLSimpleEmailSender::connect()	// DONT FORGET ! IT S CONNECT!
{
	if(!isCharStringEmpty(port) && !isCharStringEmpty(hostname) && bio != NULL)
	{
		char tmp[64];
		memset(tmp, 0, sizeof(tmp));
		strcat(tmp, this->hostname);
		strcat(tmp, ":");
		strcat(tmp, this->port);
		std::cout<<"CONNECT TO: "<<tmp<<std::endl;
		BIO_set_conn_hostname(bio, tmp);
	}
}

void SSLSimpleEmailSender::setHost(char *hostname)
{
	strcpy(this->hostname, hostname);
}

void SSLSimpleEmailSender::setPort(std::string port)
{
	strcpy(this->port,(char*)(port.c_str()));
}

void SSLSimpleEmailSender::setLogin(char *login)
{
	strcpy(this->login, login);
	char *base64;
	base64_encode(login, &base64);
	strcpy(this->base64Login, base64);
}

void SSLSimpleEmailSender::setPassword(char *password)
{
	strcpy(this->password, password);
	char *base64;
	base64_encode(password, &base64);
	strcpy(this->base64Password, base64);
}

void SSLSimpleEmailSender::setFrom(char *from)
{
	strcpy(this->from, from);
}

void SSLSimpleEmailSender::setTo(char *to)
{
	strcpy(this->to, to);
}

void SSLSimpleEmailSender::setSubject(char *subject)
{
	strcpy(this->subject, subject);
}

void SSLSimpleEmailSender::setData(char *data)
{
	strcpy(this->data, data);
}

int SSLSimpleEmailSender::Send()
{
	if(BIO_do_connect(bio) != 1)
	{
		printf("Failure connect\n");
		return 1;
	}else{
		printf("Success connect\n");
	}

	if(BIO_do_handshake(bio) != 1)
	{
		printf("Failure handshake\n");
		return 1;
	}else{
		printf("Success handshake\n");
	}
	
	if(isCharStringEmpty(hostname))
		throw (char const*)"Hostname is empty";
	if(isCharStringEmpty(login))
		throw (char const*)"Login is empty";
	if(isCharStringEmpty(password))
		throw (char const*)"Password is empty";
	if(isCharStringEmpty(from))
		throw (char const*)"From is empty";
	if(isCharStringEmpty(to))
		throw (char const*)"'To' is empty";

	char buff[256];
	bzero(buff);

	strcpy(buff, "EHLO ");
	strcat(buff, hostname);
	strcat(buff, "\r\n");

	sendPacket( buff);
	sendPacket( (char*)"AUTH LOGIN\r\n");

	bzero(buff);
	strcpy(buff, base64Login);
	strcat(buff, "\r\n");
	sendPacket( buff);

	bzero(buff);
	strcpy(buff, base64Password);
	strcat(buff, "\r\n");
	sendPacket( buff	);

	// shit-code...

	bzero(buff);
	strcpy(buff, (char*)"MAIL FROM:<");
	strcat(buff, myEmail);
	strcat(buff, (char*)">\r\n");
	sendPacket( buff);

	bzero(buff);
	strcpy(buff, "RCPT TO:<");
	strcat(buff, to);
	strcat(buff, ">\r\n");
	sendPacket( buff);
	sendPacket( (char*)"DATA\r\n");

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
	strcat(buff, (char*)"\r\n\r\n");
	strcat(buff, data);
	strcat(buff, (char*)"\r\n.\r\n");
	sendPacket( buff);

	sendPacket( (char*)"QUIT\r\n");

	return 0;
}