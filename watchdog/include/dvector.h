#ifndef __DVECTOR_H__
#define __DVECTOR_H__

#include <stddef.h> /* size_t */

typedef struct dvector dvector_t;

/*
Create
description: Creates a new dvector_t instance. 
input: wanted capacity of new vector, the size of each element in the vector in bytes
return: a pointer to the newly created vector
time complexity: O(1)
space complexity: O(n)

*/
dvector_t *DvectorCreate(size_t capacity, size_t element_size);

/*
DestoryVector
description: Destroys dynamic vector.
input: a pointer to the vector
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void DvectorDestroy(dvector_t *vector);

/*
PushBack
description: appends new element to the vector.
input: a pointer to the vector, a pointer to the new element
return: nothing
time complexity  O(n) worst case / O(1) amortized
space complexity:  O(n) worst case / O(1) amortized

*/
int DvectorPushBack(dvector_t *vector, const void *new_element);

/*
PopBack
description: removes last element from the vector.
input: a pointer to the vector
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void DvectorPopBack(dvector_t *vector);

/*
Shrink
description: shrinks the vector to match its size
input: a pointer to the vector
return: return 0 if success or -1 if failed
time complexity: O(n)
space complexity: O(n)

*/       
int DvectorShrink(dvector_t *vector);

/*
Reserve
description: reserve specified size to the vector
input: a pointer to the vector, the new desired size
return: return 0 if success or -1 if failed
time complexity: O(n)
space complexity: O(1)

*/
int DvectorReserve(dvector_t *vector, size_t new_size);

/*
Size
description: returns how many elements are in the vector
input: a pointer to the vector
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
size_t DvectorSize(const dvector_t *vector);

/*
Capacity
description: returns how many elements are allowed in the vector (or how much space was allocated)
input: a pointer to the vector
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
size_t DvectorCapacity(const dvector_t *vector);

/*
GetElement
description: returns a pointer to the element in the specified index
input: a pointer to the vector, the desired index
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void *DvectorGetElement(const dvector_t *vector, size_t index);


#endif /*__DVECTOR_H__*/
