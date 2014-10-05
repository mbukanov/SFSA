#ifndef NOTIFIER_H
#define NOTIFIER_H


#include "notificationNone.h" // include notification.h
#include "notificationEmail.h"

class Notifier
{
public:
	Notifier(Notification *notification) : _notification(notification){}
	~Notifier();
	void Alert();

private:
	Notification* _notification;

};


#endif