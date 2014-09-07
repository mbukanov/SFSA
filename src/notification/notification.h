#include <iostream>
#include "notificationNone.h"
#include "notificationEmail.h"

/* 

What notification you whant?
I have:
Email notification
None notification // empty
and you can create your notification: 
just create class with method "void Alert();".
*/

template<class NotificationParent>
class Notification	:	public NotificationParent
{};