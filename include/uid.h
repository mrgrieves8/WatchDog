#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h>/* pid_t time_t size_t*/

typedef struct
{
    time_t timestamp; 
    size_t counter; 
    pid_t pid;
} ilrd_uid_t;

extern const ilrd_uid_t BadUID;

/*
UIDCreate
description: create a unique ID
input: nothing
return: unique ID
time complexity: O(1)
space complexity: O(1)
*/
ilrd_uid_t UIDCreate(void);

/*
IsSameUID
description: checks if 2 UIDs are the same
input: get 2 ilrd_uid_t
return: 1 if they equals otherwise 0
time complexity: O(1)
space complexity: O(1)
*/
int IsSameUID(const ilrd_uid_t uid1, const ilrd_uid_t uid2);


#endif /*__UID_H__*/
