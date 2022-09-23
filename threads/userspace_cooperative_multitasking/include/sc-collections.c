/**
 * list.c: a dead simple, C89 linked list library
 */

#include "sc-collections.h"

void sc_list_insert(struct sc_list_head *head, struct sc_list_head *item)
{
	item->next = head->next;
	item->prev = head;
	item->next->prev = item;
	head->next = item;
}

void sc_list_insert_end(struct sc_list_head *head, struct sc_list_head *item)
{
	item->next = head;
	item->prev = head->prev;
	head->prev->next = item;
	head->prev = item;
}

void sc_list_remove(struct sc_list_head *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
}

void sc_list_init(struct sc_list_head *head)
{
	head->prev = head;
	head->next = head;
}

void sc_list_move(struct sc_list_head *from, struct sc_list_head *to)
{
	struct sc_list_head *fstart, *fend, *tstart;

	/* empty list */
	if (from->next == from)
		return;

	fstart = from->next;
	fend = from->prev;
	tstart = to->next; /* could be the same as to */

	to->next = fstart;
	fstart->prev = tstart->prev;
	tstart->prev = fend;
	fend->next = tstart;

	sc_list_init(from);
}

int sc_list_len(struct sc_list_head *head)
{
	int count = 0;
	struct sc_list_head *node;
	sc_list_for_each(node, head)
	{
		count += 1;
	}
	return count;
}