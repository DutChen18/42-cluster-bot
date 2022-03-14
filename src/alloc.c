#include "bot.h"
#include <stdlib.h>
#include <assert.h>

alloc_t alloc;

static alloc_node_t *alloc_node_new(alloc_node_t *prev)
{
	alloc_node_t *node = malloc(sizeof(*node));
	node->ptr = NULL;
	node->size = 0;
	node->prev = prev;
	node->next = NULL;
	return node;
}

void alloc_new(alloc_t *alloc)
{
	alloc->node = alloc_node_new(NULL);
}

void *alloc_alloc(alloc_t *alloc, size_t size)
{
	if (alloc->node->size < size) {
		alloc->node->ptr = realloc(alloc->node->ptr, size);
		alloc->node->size = size;
	}
	void *ptr = alloc->node->ptr;
	if (alloc->node->next == NULL) {
		alloc->node->next = alloc_node_new(alloc->node);
	}
	alloc->node = alloc->node->next;
	return ptr;
}

void alloc_free(alloc_t *alloc, void *ptr)
{
	(void) ptr;
	assert(alloc->node->prev != NULL);
	assert(alloc->node->prev->ptr == ptr);
	alloc->node = alloc->node->prev;
}
