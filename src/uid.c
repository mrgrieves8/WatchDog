
#include <sys/types.h> /* pid_t, time_t, size_t */
#include <unistd.h> /* getpid() */
#include <time.h> /* time() */
#include <stdatomic.h>  /* atomic_int, atomic_load(), atomic_store() */

#include "uid.h"


const ilrd_uid_t BadUID = {0};

static atomic_int counter;

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid = {0};
	uid.timestamp = time(NULL);
	
	if ((time_t) -1 == uid.timestamp)
	{
		return BadUID;
	}
	
	uid.counter = counter;
	uid.pid = getpid();
	
	atomic_fetch_add(&counter, 1);
	
	return uid;
}

int IsSameUID(const ilrd_uid_t uid1, const ilrd_uid_t uid2)
{
	return (uid1.timestamp == uid2.timestamp && 
				uid1.counter == uid2.counter && 
							uid1.pid == uid2.pid);
}
