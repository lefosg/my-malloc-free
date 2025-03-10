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
    TEST_ASSERT(hp->size==104);

	void* q = allocate(7);
	header_t* hq = get_header_of_ptr(q);
    TEST_ASSERT(hq->size==8);

	//also check that the header and pointer are aligned
	hp = get_header_of_ptr(p);
    TEST_ASSERT(((uintptr_t)hp%8)==0);
    TEST_ASSERT(((uintptr_t)p%8)==0);
}

//best fit successfully finds empty slot
void test_best_fit_S() {
    void* p = allocate(100);
    void* q = allocate(40);
    void* r = allocate(50);

	free(p);
	free(r);

	void* x = allocate(50);  //should allocate r instead of p

	header_t* hp = get_header_of_ptr(p);
	header_t* hr = get_header_of_ptr(r);
	//p should be free, and r should be allocated
	TEST_ASSERT_TRUE(block_is_free(hp));
	TEST_ASSERT_FALSE(block_is_free(hr));

	//heap reset
	p = allocate(100);
}

//best fit fails and heap extends
void test_best_fit_F() {
    void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);
	free(p);
	void* x = allocate(500);

	header_t* hx = get_header_of_ptr(x);
	header_t* hr = get_header_of_ptr(r);
	TEST_ASSERT_TRUE(hr->next==hx);

	//heap reset
	p = allocate(100);
}

//create some blocks, free a big one, and see if split works
void test_split() {
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	free(q);

	void* x = allocate(40);
	//now, get header(x)->next should be free, and header(x)->next->next should be header(r)
	header_t* hx = get_header_of_ptr(x);
	TEST_ASSERT_TRUE(block_is_free(hx->next));

	header_t* hr = get_header_of_ptr(r);
	TEST_ASSERT_TRUE(hx->next->next==hr);

	//also: we allocate 40 (aligned number). the new block has 16 bytes for the header, so the next block should have 200-40-16=48 bytes left
	TEST_ASSERT_EQUAL(144, get_block_size(hx->next));

	//heap reset
	void* y = allocate(144);
}

// ======================= FREE =======================


void test_free() {
	void* p = allocate(100);
	free(p);
	//there is not really an explicit way to test free
	//we know that the contents are not deleted from the heap, the block still exists
	//so we access the block header to check if it is marked as free
	header_t* hp = get_header_of_ptr(p);
	TEST_ASSERT_TRUE(block_is_free(hp));

	//heap reset
	p = allocate(100);
}

//test coalesce only with next block
void test_coalesce_next() {
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	free(r);
	free(q);

	//now, header(p)->next should be free, and header(p)->next->next should be NULL
	header_t* hp = get_header_of_ptr(p);

	TEST_ASSERT_NULL(hp->next->next);
	TEST_ASSERT_TRUE(block_is_free(hp->next));

	//size of new free block should be 200+304+16=520
	TEST_ASSERT_EQUAL(520, get_block_size(hp->next));	

	//heap reset
	q = allocate(520);
}

//test coalesce only with previous block, same checks apply with the previous test, but the order of free's is different
void test_coalesce_previous() {
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	free(q);
	free(r);

	//now, header(p)->next should be free, and header(p)->next->next should be NULL
	header_t* hp = get_header_of_ptr(p);

	TEST_ASSERT_TRUE(block_is_free(hp->next));
	TEST_ASSERT_NULL(hp->next->next);

	//size of new free block should be again 200+304+16=520
	TEST_ASSERT_EQUAL(520, get_block_size(hp->next));	

	//heap reset
	q = allocate(520);
}

//free all pointers to the heap -> should be left with one free block
void test_free_middle() {
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);
    void* x = allocate(400);

	free(p);
	free(r);

	free(q);

	header_t* hp = get_header_of_ptr(p);
	header_t* hx = get_header_of_ptr(x);
	//there should be two blocks now, one free (p+q+r), and one allocated (x)
	//size of free block should be 104+200+204+16+16=640 
	TEST_ASSERT_TRUE(block_is_free(hp));
	TEST_ASSERT_EQUAL(hx, hp->next);
	TEST_ASSERT_EQUAL(640, get_block_size(hp));

	//heap reset
	p = allocate(100);
	q = allocate(200);
	r = allocate(300);
	x = allocate(400);
}

void test_free_all() {
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	free(p);
	free(q);
	free(r);

	// the reason why this works is because data is still left intact on the heap, thus, accessible
	// it's bad practice and you must not access data after free'd, here we only do it for testing the allocator 
	header_t* hp = get_header_of_ptr(p);
	TEST_ASSERT_TRUE(block_is_free(hp));
	TEST_ASSERT_NULL(hp->next);

	//heap reset
	p = allocate(100);
	q = allocate(200);
	r = allocate(300);
}


// ======================= HELPER FUNCTIONS =======================

void test_get_block_size() {
	void* p = allocate(100);
	header_t* hp = get_header_of_ptr(p);
	TEST_ASSERT_EQUAL(104, get_block_size(hp));
}

void test_block_size_free_blk() {
	void* p = allocate(100);
	free(p);
	
	header_t* hp = get_header_of_ptr(p);
	size_t size = hp->size;
	TEST_ASSERT_EQUAL(105, size);

	//heap reset
	p = allocate(100);
}

void test_get_header_of_ptr() {
	// BUGGY!?! UNITY BREAKS STH IN HERE! WORKS NORMALLY IF TESTED IN ANOTHER C FILE
	//we create some pointers
	void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	//header of p, should be header(p)->size = 100, and header(p)->next should be equal to header(q), and so on
	header_t* hp = get_header_of_ptr(p);
	header_t* hq = get_header_of_ptr(q);
	TEST_ASSERT_EQUAL(104, hp->size);
	TEST_ASSERT_EQUAL(1, hp->next == hq);
}

void test_mark_block_free() {
	void* p = allocate(100);

	header_t* hp = get_header_of_ptr(p);
	mark_block_free(hp);
	TEST_ASSERT_TRUE(block_is_free(hp));

	//heap reset
	mark_block_allocated(hp);
}

void test_mark_block_allocated() {
	void* p = allocate(100);

	header_t* hp = get_header_of_ptr(p);
	mark_block_allocated(hp);
	TEST_ASSERT_FALSE(block_is_free(hp));

	free(p);
	TEST_ASSERT_TRUE(block_is_free(hp));
	mark_block_allocated(hp);
	TEST_ASSERT_FALSE(block_is_free(hp));
}

void test_block_is_free() {
	void* p = allocate(100);

	header_t* hp = get_header_of_ptr(p);
	TEST_ASSERT_FALSE(block_is_free(hp));

	free(p);
	TEST_ASSERT_TRUE(block_is_free(hp));

	//heap reset
	p = allocate(100);
}


int main(void)
{
	// allocate tests
	UNITY_BEGIN();

	// helper functions tests
	RUN_TEST(test_mark_block_free);
	RUN_TEST(test_get_block_size);
	RUN_TEST(test_block_size_free_blk);
	RUN_TEST(test_get_header_of_ptr);
	RUN_TEST(test_mark_block_allocated);
	RUN_TEST(test_block_is_free);
	
	// allocate tests
	RUN_TEST(test_allocate_S);
	RUN_TEST(test_allocate_PTRDIFF_MAX_F);
	RUN_TEST(test_allocate_size_aligned);
	RUN_TEST(test_best_fit_S);
	RUN_TEST(test_best_fit_F);
	RUN_TEST(test_split);
	
	// free tests
	RUN_TEST(test_free);
	RUN_TEST(test_coalesce_next);
	RUN_TEST(test_coalesce_previous);
	RUN_TEST(test_free_middle);
	RUN_TEST(test_free_all);

	
	
	return UNITY_END();

}