#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */
#include <stdlib.h> /* free() */
#include <assert.h>

#include "heap.h"

#define TESTNUM 6

static char failed_tests_print[300] = {'\0'};

static void AddFailedTest(const char *str);

static int TestPush(void);
static int TestPop(void);
static int TestPeek(void);
static int TestSize(void);

static int TestIsEmpty(void);
static int TestRemove(void);

struct cat
{
	char *name;
	char *favorite_food;
	int age;
};
	
int Compare(const void *data,const void *param)
{	
	return (*(int *)data - *(int *)param);
}

int CompareCats(const void *data,const void *param)
{	
	return (((struct cat *)data)->age - ((struct cat *)param)->age);
}

int FindFavoriteFood(const void *data, const void *param)
{	
	return !(strcmp(((struct cat *)data)->favorite_food, (char *)param));
}


int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestPush();
	printf("Tested Push\n");
	failed_tests_num += TestPop();
	printf("Tested Pop\n");
	failed_tests_num += TestPeek();
	printf("Tested Peek\n");
	failed_tests_num += TestSize();
	printf("Tested Size\n");
	

	failed_tests_num += TestIsEmpty();
	printf("Tested IsEmpty\n");
	failed_tests_num += TestRemove();
	printf("Tested Remove\n");
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
    return 0;
}

static void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}

static int TestPush(void)
{
	heap_t *heap = HeapCreate(Compare);
	int elements[15] = {30, 40, 20, 70, 80, 
						15, 10, 55, 35, 100, 
						25, 90, 60, 50, 45};
	int min = 0;
	size_t loop_count = 0;
	
	HeapPush(heap, elements);

	if (HeapSize(heap) != 1)
	{
		AddFailedTest("TestPush1\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements + 1);

	if (HeapSize(heap) != 2)
	{
		AddFailedTest("TestPush2\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (30 != min)
	{
		AddFailedTest("TestPush3\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements + 2);

	if (HeapSize(heap) != 3)
	{
		AddFailedTest("TestPush4\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (20 != min)
	{
		AddFailedTest("TestPush5\n");
		HeapDestroy(heap);
		return 1;
	}
	
	for (loop_count = 3; loop_count < 15; ++loop_count)
	{
		HeapPush(heap, elements + loop_count);
	}

	if (HeapSize(heap) != 15)
	{
		AddFailedTest("TestPush6\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (10 != min)
	{
		AddFailedTest("TestPush7\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapDestroy(heap);
	return 0;
}

static int TestPop(void)
{
	heap_t *heap = HeapCreate(Compare);
	int elements[15] = {30, 40, 20, 70, 80, 
						15, 10, 55, 35, 100, 
						25, 90, 60, 50, 45};
	int min = 0;
	size_t loop_count = 0;
	
	for (loop_count = 0; loop_count < 15; ++loop_count)
	{
		HeapPush(heap, elements + loop_count);
	}

	HeapPop(heap);

	if (HeapSize(heap) != 14)
	{
		AddFailedTest("TestPop1\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (15 != min)
	{
		AddFailedTest("TestPop2\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 13)
	{
		AddFailedTest("TestPop3\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (20 != min)
	{
		AddFailedTest("TestPop4\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 12)
	{
		AddFailedTest("TestPop5\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (25 != min)
	{
		AddFailedTest("TestPop6\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 11)
	{
		AddFailedTest("TestPop7\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (30 != min)
	{
		AddFailedTest("TestPop8\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 10)
	{
		AddFailedTest("TestPop9\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (35 != min)
	{
		AddFailedTest("TestPop10\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 9)
	{
		AddFailedTest("TestPop11\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (40 != min)
	{
		AddFailedTest("TestPop12\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);

	if (HeapSize(heap) != 8)
	{
		AddFailedTest("TestPop13\n");
		HeapDestroy(heap);
		return 1;
	}
	
	min = *(int *)HeapPeek(heap);
	if (45 != min)
	{
		AddFailedTest("TestPop14\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapDestroy(heap);
	return 0;
}

static int TestPeek(void)
{
	heap_t *heap = HeapCreate(Compare);
	int elements[15] = {30, 40, 20, 70, 80, 
						15, 10, 55, 35, 100, 
						25, 90, 60, 50, 45};
	int min = 0;
	size_t loop_count = 0;
	
	HeapPush(heap, elements);
	min = *(int *)HeapPeek(heap);
	if (min != 30)
	{
		AddFailedTest("TestPeek1\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+1);
	min = *(int *)HeapPeek(heap);
	if (min != 30)
	{
		AddFailedTest("TestPeek2\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+2);
	min = *(int *)HeapPeek(heap);
	if (min != 20)
	{
		AddFailedTest("TestPeek3\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+3);
	min = *(int *)HeapPeek(heap);
	if (min != 20)
	{
		AddFailedTest("TestPeek4\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+4);
	min = *(int *)HeapPeek(heap);
	if (min != 20)
	{
		AddFailedTest("TestPeek5\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+5);
	min = *(int *)HeapPeek(heap);
	if (min != 15)
	{
		AddFailedTest("TestPeek6\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements+6);
	min = *(int *)HeapPeek(heap);
	if (min != 10)
	{
		AddFailedTest("TestPeek7\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 15)
	{
		AddFailedTest("TestPeek8\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 20)
	{
		AddFailedTest("TestPeek9\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 30)
	{
		AddFailedTest("TestPeek10\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 40)
	{
		AddFailedTest("TestPeek11\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 70)
	{
		AddFailedTest("TestPeek12\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPop(heap);
	min = *(int *)HeapPeek(heap);
	if (min != 80)
	{
		AddFailedTest("TestPeek13\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapDestroy(heap);	
	return 0;
}

static int TestSize(void)
{
	heap_t *heap = HeapCreate(Compare);
	int elements[15] = {30, 40, 20, 70, 80, 
						15, 10, 55, 35, 100, 
						25, 90, 60, 50, 45};
	int min = 0;
	size_t loop_count = 0;
	
	for (loop_count = 0; loop_count < 15; ++loop_count)
	{
		HeapPush(heap, elements + loop_count);
		if (HeapSize(heap) != loop_count+1)
		{
			AddFailedTest("TestSize1\n");
			HeapDestroy(heap);
			return 1;
		}
	}
	
	for (loop_count = 0; loop_count < 15; ++loop_count)
	{
		HeapPop(heap);
		if (HeapSize(heap) != 14 - loop_count)
		{
			AddFailedTest("TestSize2\n");
			HeapDestroy(heap);
			return 1;
		}
	}
	
	HeapDestroy(heap);
	return 0;
}

static int TestIsEmpty(void)
{
	heap_t *heap = HeapCreate(Compare);
	int elements[15] = {30, 40, 20, 70, 80, 
						15, 10, 55, 35, 100, 
						25, 90, 60, 50, 45};
	
	if (!HeapIsEmpty(heap))
	{
		AddFailedTest("TestIsEmpty1\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapPush(heap, elements);
	if (HeapIsEmpty(heap))
	{
		AddFailedTest("TestIsEmpty2\n");
		HeapDestroy(heap);
		return 1;
	}

	HeapPop(heap);
	if (!HeapIsEmpty(heap))
	{
		AddFailedTest("TestIsEmpty3\n");
		HeapDestroy(heap);
		return 1;
	}

	
	HeapDestroy(heap);
	return 0;
}

static int TestRemove(void)
{
	heap_t *heap = HeapCreate(CompareCats);
	struct cat honey = {0};
	struct cat garfield = {0};
	struct cat meshugah = {0};
	struct cat bentzi = {0};
	struct cat diana = {0};
	struct cat get_cat = {0};
	
	honey.name = "honey";
	honey.favorite_food = "tuna";
	honey.age = 1;
	
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
	
	HeapPush(heap, &honey);
	HeapPush(heap, &garfield);
	HeapPush(heap, &meshugah);
	HeapPush(heap, &bentzi);
	HeapPush(heap, &diana);
	
	get_cat = *(struct cat *)HeapRemove(heap, FindFavoriteFood, "melafefon");
	
	if (CompareCats(&get_cat, &bentzi))
	{
		AddFailedTest("TestRemove1\n");
		HeapDestroy(heap);
		return 1;
	}
	
	if (HeapSize(heap) != 4)
	{
		AddFailedTest("TestRemove2\n");
		HeapDestroy(heap);
		return 1;
	}
	
	HeapDestroy(heap);
	return 0;
}










