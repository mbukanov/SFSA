#include <iostream>
#include "../baseMols.h"

class NotificationNone : public BaseMOLS
{
public:
	virtual void Alert() { std::cout<<"NotificationNONE"<<std::endl; }
private:
};