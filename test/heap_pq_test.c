#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */

#include "heap_pq.h"

#define TESTNUM 7

static char failed_tests_print[200] = {'\0'};

static int TestHeapPQEnqueue();
static int TestHeapPQDequeue();
static int TestHeapPQSize();
static int TestHeapPQPeek();

static int TestHeapPQIsEmpty();
static int TestHeapPQClear();
static int TestHeapPQErase();

struct cat
{
	char *name;
	char *favorite_food;
	int age;
};

int CompareCats(const void *data, const void *param)
{	
	return (((struct cat *)data)->age - ((struct cat *)param)->age);
}

int FindFavoriteFood(const void *data, const void *param)
{	
	return !(strcmp(((struct cat *)data)->favorite_food, (char *)param));
}

void AddFailedTest(const char *str);

int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestHeapPQEnqueue();
	printf("Tested Enqueue\n");
	failed_tests_num += TestHeapPQDequeue();
	printf("Tested Dequeue\n");
	failed_tests_num += TestHeapPQSize();
	printf("Tested Size\n");
	failed_tests_num += TestHeapPQPeek();
	printf("Tested Peek\n");
	
	failed_tests_num += TestHeapPQIsEmpty();
	printf("Tested IsEmpty\n");
	failed_tests_num += TestHeapPQClear();
	printf("Tested Clear\n");
	failed_tests_num += TestHeapPQErase();
	printf("Tested Erase\n");
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
    return 0;
}


void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}

static int TestHeapPQEnqueue()
{
	heap_pq_t *queue = NULL;
	struct cat honey = {0};
	struct cat garfield = {0};
	struct cat get_cat = {0};
	int status = 0;
	size_t count = 0;
	
	honey.name = "honey";
	honey.favorite_food = "tuna";
	honey.age = 1;
	
	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;

	/* create queue */
	queue = HeapPQCreate(CompareCats);

	/* Enqueue honey */
	status = HeapPQEnqueue(queue, &honey);

	get_cat = *(struct cat *)HeapPQPeek(queue);
	
	if (honey.name != get_cat.name || status)
	{
		AddFailedTest("TestHeapPQEnqueue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue garfield*/
	status = HeapPQEnqueue(queue, &garfield);
	
	count = HeapPQSize(queue);
	
	if (status || (count != 2))
	{
		AddFailedTest("TestHeapPQEnqueue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQDequeue()
{
	heap_pq_t *queue = NULL;
	struct cat honey = {0};
	struct cat garfield = {0};
	struct cat get_cat = {0};
	size_t count = 0;
	
	honey.name = "honey";
	honey.favorite_food = "tuna";
	honey.age = 1;
	
	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;
	
	/* create queue */
	queue = HeapPQCreate(CompareCats);

	/* Enqueue honey */
	HeapPQEnqueue(queue, &honey);

	/* Enqueue garfield*/
	HeapPQEnqueue(queue, &garfield);
	
	/* Dequeue honey */
	HeapPQDequeue(queue);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	
	if (garfield.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQDequeue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	count = HeapPQSize(queue);
	
	if (count != 1)
	{
		AddFailedTest("TestHeapPQDequeue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Dequeue garfield */
	HeapPQDequeue(queue);
	
	count = HeapPQSize(queue);
	
	if (count != 0)
	{
		AddFailedTest("TestHeapPQDequeue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	if (!HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQDequeue\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQSize()
{
	heap_pq_t *queue = NULL;
	struct cat honey = {0};
	struct cat garfield = {0};
	size_t count = 0;
	
	honey.name = "honey";
	honey.favorite_food = "tuna";
	honey.age = 1;
	
	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;

	/* create queue */
	queue = HeapPQCreate(CompareCats);
	
	count = HeapPQSize(queue);
	if (count != 0)
	{
		AddFailedTest("TestHeapPQSize\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue honey */
	HeapPQEnqueue(queue, &honey);
	
	count = HeapPQSize(queue);
	if (count != 1)
	{
		AddFailedTest("TestHeapPQSize\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue garfield*/
	HeapPQEnqueue(queue, &garfield);
	
	count = HeapPQSize(queue);
	if (count != 2)
	{
		AddFailedTest("TestHeapPQSize\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Dequeue honey */
	HeapPQDequeue(queue);

	count = HeapPQSize(queue);
	if (count != 1)
	{
		AddFailedTest("TestHeapPQSize\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Dequeue garfield */
	HeapPQDequeue(queue);
	
	count = HeapPQSize(queue);
	if (count != 0)
	{
		AddFailedTest("TestHeapPQSize\n");
		HeapPQDestroy(queue);
		return 1;
	}

	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQPeek()
{
	heap_pq_t *queue = NULL;
	struct cat garfield = {0};
	struct cat meshugah = {0};
	struct cat bentzi = {0};
	struct cat diana = {0};
	struct cat get_cat = {0};

	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;
	
	meshugah.name = "meshugah";
	meshugah.favorite_food = "garfield";
	meshugah.age = 7;
	
	bentzi.name = "bentzi";
	bentzi.favorite_food = "melafefon";
	bentzi.age = 12;
	
	diana.name = "diana";
	diana.favorite_food = "salmon";
	diana.age = 4;
	
	/* create queue */
	queue = HeapPQCreate(CompareCats);

	/* Enqueue garfield */
	HeapPQEnqueue(queue, &garfield);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (garfield.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue meshugah*/
	HeapPQEnqueue(queue, &meshugah);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (meshugah.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue bentzi */
	HeapPQEnqueue(queue, &bentzi);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (meshugah.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Enqueue diana*/
	HeapPQEnqueue(queue, &diana);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (diana.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Dequeue diana */
	HeapPQDequeue(queue);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (meshugah.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}

	
	/* Dequeue meshugah */
	HeapPQDequeue(queue);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (bentzi.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Dequeue bentzi */
	HeapPQDequeue(queue);
	
	get_cat = *(struct cat *)HeapPQPeek(queue);
	if (garfield.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQPeek\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQIsEmpty()
{
	heap_pq_t *queue = NULL;
	struct cat honey = {0};
	
	honey.name = "honey";
	honey.favorite_food = "tuna";
	honey.age = 1;
	
	/* create queue */
	queue = HeapPQCreate(CompareCats);
	
	if (!HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQIsEmpty\n");
		HeapPQDestroy(queue);
		return 1;
	}
	/* Enqueue honey */
	HeapPQEnqueue(queue, &honey);
	
	if (HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQIsEmpty\n");
		HeapPQDestroy(queue);
		return 1;
	}
	/* Dequeue honey */
	HeapPQDequeue(queue);
	
	if (!HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQIsEmpty\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQClear()
{
	heap_pq_t *queue = NULL;
	struct cat garfield = {0};
	struct cat meshugah = {0};
	struct cat bentzi = {0};
	size_t count = 0;

	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;
	
	meshugah.name = "meshugah";
	meshugah.favorite_food = "garfield";
	meshugah.age = 7;
	
	bentzi.name = "bentzi";
	bentzi.favorite_food = "melafefon";
	bentzi.age = 12;
	
	/* create queue */
	queue = HeapPQCreate(CompareCats);

	/* Enqueue garfield */
	HeapPQEnqueue(queue, &garfield);
	
	/* Enqueue meshugah*/
	HeapPQEnqueue(queue, &meshugah);
	
	/* Enqueue bentzi */
	HeapPQEnqueue(queue, &bentzi);
	
	count = HeapPQSize(queue);
	if (0 == count)
	{
		AddFailedTest("TestHeapPQClear\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	if (HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQClear\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	/* Clear Queue */
	HeapPQClear(queue);
	
	count = HeapPQSize(queue);
	if (0 != count)
	{
		AddFailedTest("TestHeapPQClear\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	if (!HeapPQIsEmpty(queue))
	{
		AddFailedTest("TestHeapPQClear\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}

static int TestHeapPQErase()
{
	heap_pq_t *queue = NULL;
	struct cat garfield = {0};
	struct cat meshugah = {0};
	struct cat bentzi = {0};
	struct cat diana = {0};
	struct cat get_cat = {0};
	size_t count = 0;

	garfield.name = "garfield";
	garfield.favorite_food = "lasagna";
	garfield.age = 43;
	
	meshugah.name = "meshugah";
	meshugah.favorite_food = "garfield";
	meshugah.age = 7;
	
	bentzi.name = "bentzi";
	bentzi.favorite_food = "melafefon";
	bentzi.age = 12;
	
	diana.name = "diana";
	diana.favorite_food = "salmon";
	diana.age = 4;
	
	/* create queue */
	queue = HeapPQCreate(CompareCats);

	/* Enqueue garfield */
	HeapPQEnqueue(queue, &garfield);
	
	/* Enqueue meshugah*/
	HeapPQEnqueue(queue, &meshugah);
	
	/* Enqueue bentzi */
	HeapPQEnqueue(queue, &bentzi);
	
	/* Enqueue diana */
	HeapPQEnqueue(queue, &diana);
	
	get_cat = *(struct cat *)HeapPQErase(queue, FindFavoriteFood, "melafefon");
	if (bentzi.name != get_cat.name)
	{
		AddFailedTest("TestHeapPQErase1\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	count = HeapPQSize(queue);
	if (3 != count)
	{
		AddFailedTest("TestHeapPQErase2\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	if (NULL != HeapPQErase(queue, FindFavoriteFood, "tuna"))
	{
		AddFailedTest("TestHeapPQErase3\n");
		HeapPQDestroy(queue);
		return 1;
	}
	
	HeapPQDestroy(queue);
	return 0;
}




