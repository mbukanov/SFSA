#ifndef NOTIFICATIONNONE_H
#define NOTIFICATIONNONE_H

/*
note: pattern Strategy

ConcreteStrategy NotificationNone
*/


#include <iostream>
#include "notification.h"


class NotificationNone : public Notification
{
public:
	virtual void Alert() { std::cout<<"NotificationNONE"<<std::endl; }
private:
};

#endif