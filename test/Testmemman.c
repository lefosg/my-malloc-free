#include <errno.h>

#include "../Unity/src/unity.h"
#include "../src/memman.h"


void setUp()
{
}

void tearDown()
{
}

// ======================= ALOCATE =======================

//allocate returns not NULL
void test_allocate_S() {
    void* p = allocate(100);
    TEST_ASSERT_NOT_NULL(p);
}

//cannot allocate more than __PTRDIFF_MAX__
void test_allocate_PTRDIFF_MAX_F() {
    void* p = allocate(__PTRDIFF_MAX__);
	TEST_ASSERT_NULL(p);
	TEST_ASSERT(errno == ENOMEM);

	p = allocate(-1);
	TEST_ASSERT_NULL(p);
	TEST_ASSERT(errno == ENOMEM);
}

//test alignment is correct
void test_allocate_size_aligned() {
    void* p = allocate(100);
	header_t* hp = get_header_of_ptr(p);
    TEST_ASSERT_EQUAL(104, hp->size);

	void* q = allocate(7);
	header_t* hq = get_header_of_ptr(q);
    TEST_ASSERT_EQUAL(8, hq->size);

	//also check that the header and pointer are aligned
	hp = get_header_of_ptr(p);
    TEST_ASSERT_EQUAL(0, ((uintptr_t)hp%8));
    TEST_ASSERT_EQUAL(0, (uintptr_t)p%8);
}





int main(void)
{
	// allocate tests
	UNITY_BEGIN();

	// helper functions tests

	
	// allocate tests
	RUN_TEST(test_allocate_S);
	RUN_TEST(test_allocate_size_aligned);
	RUN_TEST(test_allocate_size_aligned);

	// free tests


	return UNITY_END();

}