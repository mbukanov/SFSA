#ifndef NOTIFICATION_H
#define NOTIFICATION_H


class Notification
{
public:
	virtual ~Notification(){}
	virtual void Alert() = 0;
};


#endif