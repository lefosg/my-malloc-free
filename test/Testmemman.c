#include <errno.h>

#include "../Unity/src/unity.h"
#include "../src/memman.h"


void setUp()
{
}

void tearDown()
{
}

// ======================= ALLOCATE =======================

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



// ======================= FREE =======================

void test_free() {
	void* p1 = allocate(100);
	void* p2 = allocate(200);
	void* p3 = allocate(300);
	void* p4 = allocate(400);
	void* p5 = allocate(500);

	free(p2);

	free_blk_header_t* root = get_root();
	TEST_ASSERT_NOT_NULL(root->next);
	TEST_ASSERT_EQUAL(104, get_block_size(root->next));
	TEST_ASSERT_EQUAL(105, root->next->size);
	TEST_ASSERT_TRUE(block_is_free(root->next));
	TEST_ASSERT_NULL(root->next->next);

	//heap reset
	p2 = allocate(200);
}

void test_free_edge_case_only_one_blk() {
	void* p = allocate(100);
	free(p);

	header_t* hp = get_header_of_ptr(p);
	free_blk_header_t* root = get_root();
	TEST_ASSERT_NOT_NULL(root->next);
	TEST_ASSERT_EQUAL(104, get_block_size(root->next));
	TEST_ASSERT_EQUAL(105, root->next->size);
	TEST_ASSERT_TRUE(block_is_free(root->next));
	TEST_ASSERT_NULL(root->next->next);

	p = allocate(100);
	TEST_ASSERT_NULL(root->next);
	TEST_ASSERT_FALSE(block_is_free(hp));
}

void test_free_multiple() {
	void* p = allocate(100);
	void* q = allocate(200);
	void* r = allocate(300);
	free(p);
	free(r);

	free_blk_header_t* root = get_root();
	TEST_ASSERT_NOT_NULL(root->next);
	TEST_ASSERT_NOT_NULL(root->next->next);
	TEST_ASSERT_EQUAL(304, get_block_size(root->next));
	TEST_ASSERT_TRUE(block_is_free(root->next));
	TEST_ASSERT_TRUE(block_is_free(root->next->next));
}


int main(void)
{
	// allocate tests
	UNITY_BEGIN();

	// helper functions tests

	
	// allocate tests
	RUN_TEST(test_allocate_S);
	RUN_TEST(test_allocate_PTRDIFF_MAX_F);
	RUN_TEST(test_allocate_size_aligned);

	// free tests
	RUN_TEST(test_free);
	// RUN_TEST(test_free_edge_case_only_one_blk);
	// RUN_TEST(test_free_multiple);



	return UNITY_END();

}