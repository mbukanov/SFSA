#include <iostream>


class NotificationEmail
{
public:
	virtual void Alert(){ std::cout<<"Email notification"<<std::endl; }
private:

};