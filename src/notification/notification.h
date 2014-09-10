#include <iostream>
#include "notificationNone.h"
#include "notificationEmail.h"


template<class NotificationParent>
class Notification	:	public NotificationParent
{};