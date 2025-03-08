#include <stddef.h>
#include <errno.h>

#include "../Unity/src/unity.h"
#include "../src/memman.h"


void setUp()
{

}

void tearDown()
{

}

void test_allocate_S() {
    void* p = allocate(100);
    TEST_ASSERT_NOT_NULL(p);
}

void test_alignment_8byte() {
    void* p = allocate(100);
    TEST_ASSERT(((uintptr_t)p%8)==0);
}

void test_allocate_PTRDIFF_MAX_F() {
    void* p = allocate(__PTRDIFF_MAX__);
	TEST_ASSERT_NULL(p);
	TEST_ASSERT(errno == ENOMEM);
}

void test_allocate_size_aligned() {
    void* p = allocate(100);
	header_t* h = get_header_of_ptr(p);
	print_header_info(h);
    TEST_ASSERT(h->size==112);
}


int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_allocate_S);
	RUN_TEST(test_alignment_8byte);
	RUN_TEST(test_allocate_PTRDIFF_MAX_F);
	RUN_TEST(test_allocate_size_aligned);

	
	UNITY_END();

	return 0;
}
