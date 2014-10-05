#ifndef NOTIFICATION_H
#define NOTIFICATION_H

/*
note: pattern Strategy

Strategy
*/


class Notification
{
public:
	virtual ~Notification(){}
	virtual void Alert() = 0;
};


#endif