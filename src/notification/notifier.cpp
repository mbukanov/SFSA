#include "notifier.h"

Notifier::~Notifier()
{
	delete _notification;
}

void Notifier::Alert()
{
	_notification->Alert();
}
