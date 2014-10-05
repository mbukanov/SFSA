#include "notifier.h"

/*
note: pattern Strategy

Context
*/

Notifier::~Notifier()
{
	delete _notification;
}

void Notifier::Alert()
{
	_notification->Alert();
}
