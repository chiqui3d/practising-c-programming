/**
 * libsc-collections: Simple data structure implementations
 * @file sc-collections.h
 */

#ifndef SC_COLLECTIONS_H
#define SC_COLLECTIONS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

/*************
 * Iterator
 *************/

/**
 * @brief Generic iterator interface.
 *
 * A generic iterator interface which can be implemented by data structures or
 * even as a generator.
 *
 * Implementations MUST populate all three function pointers, and should only
 * use the data fields declared.
 */
struct sc_iterator {
	/** @brief the container data structure */
	void *ds;
	/** @brief zero-based index for the iterator */
	int index;
	/** @brief an integer field to use as state */
	int state_int;
	/** @brief a pointer field to use as state */
	void *state_ptr;

	/**
	 * @brief Return true if sc_iterator.next will return another value
	 * @param iter Iterator instance
	 */
	bool (*has_next)(struct sc_iterator *iter);

	/**
	 * @brief Return the next value.
	 * @param iter iterator instance
	 */
	void *(*next)(struct sc_iterator *iter);

	/**
	 * @brief Free any resources associated with the iterator.
	 * @param iter iterator instance
	 */
	void (*close)(struct sc_iterator *iter);
};

/**
 * @brief Function which does nothing, and satisfies the signature of close(), w
 * @param iter iterator instance
 */
void sc_iterator_close_noop(struct sc_iterator *iter);
/**
 * @brief Return an empty iterator.
 * @returns An iterator whose has_next returns false
 */
struct sc_iterator sc_iterator_empty(void);
/**
 * @brief Return an iterator which returns a single value.
 * @param value The value to be returned by the iterator
 * @returns an iterator which returns only @a value
 */
struct sc_iterator sc_iterator_single_value(void *value);

/**
 * @brief Concatenate n iterators.
 * @param its Array of iterators which will be combined
 * @param n Number of iterators contained in its
 * @return An iterator which will draw from each component iterator in `its`
 * until all iterators are consumed.
 *
 * This function takes ownership of the @a its pointer; it will be freed upon
 * calling sc_iterator.close on the returned iterator.
 */
struct sc_iterator sc_iterator_concat(struct sc_iterator *its, size_t n);

/**
 * @brief Concatenate two iterators -- a convenience method for sc_iterator_concat()
 * @param left First iterator
 * @param right Second iterator
 * @return New iterator over both
 */
struct sc_iterator sc_iterator_concat2(struct sc_iterator left,
                                       struct sc_iterator right);
/**
 * @brief Concatenate three iterators -- a convenience method for sc_iterator_concat()
 * @param a First iterator
 * @param b Second iterator
 * @param c Third iterator
 * @return New iterator over all three
 */
struct sc_iterator sc_iterator_concat3(struct sc_iterator a,
                                       struct sc_iterator b,
                                       struct sc_iterator c);

/**
 * @brief Return an iterator which yields items out of an array.
 * @param array An array of items to yield
 * @param len Number of items in the array
 * @param own If true is passed here, then the array will be freed when close()
 * is called on the returned iterator. If false is passed here, then it is the
 * caller's responsibility to free the array sometime after the iterator is
 * consumed.
 * @return An iterator which yields from the array
 */
struct sc_iterator sc_iterator_array(void **array, int len, bool own);
/**
 * @brief Return an iterator which will yield each item passed as an argument
 * @param n Number of items provided in the args
 * @param ... Several void* pointers to return from the iterator
 * @return An iterator which yields the items passed in the function.
 */
struct sc_iterator sc_iterator_from_args(int n, ...);

/*************
 * Hash Table
 *************/

/**
 * @brief A hash function
 * @param to_hash pointer to the data to hash
 * @return A uniformly distributed integer
 */
typedef unsigned int (*sc_hash_t)(void *to_hash);

/**
 * @brief A comparison function
 * @param left One item of the comparison
 * @param right Other item of the comparison
 * @return 0 when equal, nonzero otherwise
 */
typedef int (*sc_comp_t)(void *left, void *right);

/**
 * @brief A print function
 * @param f file to write to
 * @param data data to print (either a key or value, depending on use)
 * @return an integer which is ignored
 *
 * This typedef is used mainly to allow diagnostic printing vi sc_ht_print().
 */
typedef int (*sc_print_t)(FILE *f, void *data);

/**
 * @brief Data structure representing a hash table.
 *
 * Please note that all fields of the hash table should be considered private.
 * The struct is declared in the header only to allow it to be embedded within
 * other structs.
 */
struct sc_hashtable {
	unsigned long length;    /* number of items currently in the table */
	unsigned long allocated; /* number of items allocated */

	unsigned int key_size;
	unsigned int value_size;

	sc_hash_t hash;
	sc_comp_t equal;

	void *table;
};

/**
 * @brief Initialize a hash table in memory already allocated.
 * @param table A pointer to the table to initialize.
 * @param hash_func A hash function for the table.
 * @param equal A comparison function for void pointers
 * @param key_size Size of keys.
 * @param value_size Size of values.
 *
 * This function assumes that the memory for a sc_hashtable is already
 * allocated. It should be paired with a call to sc_ht_destroy().
 */
void sc_ht_init(struct sc_hashtable *table, sc_hash_t hash_func,
                sc_comp_t equal, unsigned int key_size,
                unsigned int value_size);
/**
 * @brief Allocate and initialize a hash table.
 * @param hash_func A function that takes one void* and returns a hash value
 * generated from it.  It should be a good hash function.
 * @param equal A comparison function for void pointers.
 * @param key_size Size of keys.
 * @param value_size Size of values.
 * @returns A pointer to the new hash table.
 *
 * This function allocates the space for a sc_hashtable via malloc(). It should
 * be paired with a call to sc_ht_delete().
 */
struct sc_hashtable *sc_ht_create(sc_hash_t hash_func, sc_comp_t equal,
                                  unsigned int key_size,
                                  unsigned int value_size);
/**
 * @brief Free any resources used by the hash table, but doesn't free the
 * pointer.  Doesn't perform any actions on the data as it is deleted.
 *
 * If pointers are contained within the hash table, they are not freed.
 * @param table The table to destroy.
 */
void sc_ht_destroy(struct sc_hashtable *table);
/**
 * @brief Free the hash table and its resources.	No pointers contained in
 * the table will be freed.
 * @param table The table to free.
 */
void sc_ht_delete(struct sc_hashtable *table);

/**
 * @brief Insert data into the hash table.
 *
 * Expands the hash table if the load factor is below a threshold.	If the
 * key already exists in the table, then the function will overwrite it with the
 * new data provided.
 * @param table A pointer to the hash table.
 * @param key The key to insert.
 * @param value The value to insert at the key.
 */
void sc_ht_insert(struct sc_hashtable *table, void *key, void *value);
/** @see sc_ht_insert */
void sc_ht_insert_ptr(struct sc_hashtable *table, void *key, void *value);
/**
 * @brief Remove the key, value pair stored in the hash table.
 *
 * This function does not call a deleter on the stored data.
 * @param table A pointer to the hash table.
 * @param key The key to delete.
 * @return -1 on failure, 0 otherwise
 */
int sc_ht_remove(struct sc_hashtable *table, void *key);
/** @see sc_ht_remove */
int sc_ht_remove_ptr(struct sc_hashtable *table, void *key);
/**
 * @brief Return the value associated with the key provided.
 * @param table A pointer to the hash table.
 * @param key The key whose value to retrieve.
 * @returns The value associated the key, NULL if not found
 */
void *sc_ht_get(struct sc_hashtable const *table, void *key);
/** @see sc_ht_get */
void *sc_ht_get_ptr(struct sc_hashtable const *table, void *key);
/**
 * @brief Return the key associated with the key provided.
 * This may sound silly, but as long as @a key hashes and compares to the stored
 * key, it may actually be different value. If you use the hash table as a
 * set, you'll need a way to look up the pointer associated with the original
 * key -- this is how you do it.
 * @param table A pointer to the hash table.
 * @param key The key to retrieve.
 * @returns The original key associated the key, NULL if not found
 */
void *sc_ht_get_key(struct sc_hashtable const *table, void *key);
/** @see sc_ht_get_key */
void *sc_ht_get_key_ptr(struct sc_hashtable const *table, void *key);
/**
 * @brief Return true when a key is contained in the table.
 * @param table A pointer to the hash table.
 * @param key The key to search for.
 * @returns Whether the key is present.
 */
bool sc_ht_contains(struct sc_hashtable const *table, void *key);
/** @see sc_ht_contains */
bool sc_ht_contains_ptr(struct sc_hashtable const *table, void *key);
/**
 * Return the number of key, value pairs stored in the hash table.
 * @param table A pointer to the hash table.
 * @returns The number of key, value pairs stored in the hash table.
 */
unsigned long sc_ht_length(const struct sc_hashtable *table);

/**
 * @brief Return the hash of the data, interpreting it as a string.
 * @param data The string to hash, assuming that the value contained is a char*.
 * @returns The hash value of the string.
 */
unsigned int sc_ht_string_hash(void *data);

/**
 * @brief A comparison function for strings. Simply wraps strcmp()
 */
int sc_ht_string_comp(void *left, void *right);
/**
 * @brief A comparison functions for integers.
 */
int sc_ht_int_comp(void *left, void *right);

/**
 * Return an iterator over the keys of the hash table. Each item returned by the
 * next() function is a pointer to data, similar to the sc_ht_get_key() method.
 * Consider using sc_ht_iter_keys_ptr() if you are storing pointers.
 * @warning The iterator implementation DOES NOT support modifying the table as
 * you iterate.
 * @param table The table you wish to iterate over
 * @return An iterator over the keys
 */
struct sc_iterator sc_ht_iter_keys(struct sc_hashtable *table);
/**
 * Return an iterator over the keys of the hash table. We assume that each key
 * stored in the hash table is a pointer, and dereference that to return the
 * actual pointer rather than a double pointer.
 * Consider using sc_ht_iter_keys() if you are not storing pointers.
 * @warning The iterator implementation DOES NOT support modifying the table as
 * you iterate.
 * @param table The table you wish to iterate over
 * @return An iterator over the keys
 */
struct sc_iterator sc_ht_iter_keys_ptr(struct sc_hashtable *table);
/**
 * Return an iterator over the values of the hash table. Each item returned by
 * the next() function is a pointer to data, similar to the sc_ht_get() method.
 * Consider using sc_ht_iter_values_ptr() if you are storing pointers.
 * @warning The iterator implementation DOES NOT support modifying the table as
 * you iterate.
 * @param table The table you wish to iterate over
 * @return An iterator over the values
 */
struct sc_iterator sc_ht_iter_values(struct sc_hashtable *table);
/**
 * Return an iterator over the values of the hash table. We assume that each
 * value stored in the hash table is a pointer, and dereference that to return
 * the actual pointer rather than a double pointer.
 * Consider using sc_ht_iter_values() if you are not storing pointers.
 * @warning The iterator implementation DOES NOT support modifying the table as
 * you iterate.
 * @param table The table you wish to iterate over
 * @return An iterator over the values
 */
struct sc_iterator sc_ht_iter_values_ptr(struct sc_hashtable *table);

/*******************
 * Character buffer
 *******************/

/**
 * @brief A character buffer utility that is easier to handle than a char*.
 *
 * This character buffer provides an interface for string processing that allows
 * you to be efficient, while not needing to handle nearly as much of the
 * allocations that are necessary. It automatically expands as you add to it.
 */
struct sc_charbuf {
	/**
	 * @brief Pointer to malloc() allocated buffer for the data
	 *
	 * This field should be accessed mostly as read-only during the duration
	 * of the lifetime of the charbuf. Any modifications to the buffer MUST
	 * maintain the capacity and length field definitions below.
	 *
	 * This field can be "exported" (i.e. copied out), allowing the
	 * application to take ownership of the buffer (and responsibility for
	 * freeing it).
	 */
	char *buf;
	/**
	 * @brief Allocated size of the buffer.
	 */
	int capacity;
	/**
	 * @brief Length of the string in the buffer (NUL excluded).
	 */
	int length;
};

/**
 * @brief Initialize a brand-new character buffer.
 * @param obj The struct sc_charbuf to initialize.
 * @param capacity Initial capacity of the buffer.
 *
 * Initialize the struct sc_charbuf with a new buffer of given capacity. The
 * buffer is initialized to an empty string.
 *
 * Normally, this function should be paired with a call to sc_cb_destroy() to
 * free the underlying buffer when you're done using it. However, one exception
 * to this rule is that you may "export" the sc_charbuf.buf field for your own
 * management and use. Once you do so, you MUST NOT use any sc_cb API (except
 * for a subsequent sc_cb_init(), to reinitialize with a new buffer). Further,
 * when you are done with the exported buffer, you MUST free it via free().
 */
void sc_cb_init(struct sc_charbuf *obj, int capacity);
/**
 * @brief Deallocate the contents of the string buffer.
 * @param obj The character buffer to deallocate.
 */
void sc_cb_destroy(struct sc_charbuf *obj);

/**
 * @brief Concat a string onto the end of the character buffer.
 * @param obj The buffer to concat onto.
 * @param str The string to concat.
 */
void sc_cb_concat(struct sc_charbuf *obj, const char *str);
/**
 * @brief Append a character onto the end of the character buffer.
 * @param obj The buffer to append onto.
 * @param next The character to append.
 */
void sc_cb_append(struct sc_charbuf *obj, char next);
/**
 * @brief Reallocate the buffer to the exact size of the contained string.
 * @param obj The buffer to reallocate.
 */
void sc_cb_trim(struct sc_charbuf *obj);
/**
 * @brief Empty the buffer of its contents.
 * @param obj The buffer to clear.
 */
void sc_cb_clear(struct sc_charbuf *obj);
/**
 * @brief Append exactly count bytes onto the buffer
 * @param obj Buffer to append to
 * @param src Source string
 * @param count Number of bytes
 *
 * @warning This function allows you to copy NUL bytes into the buffer. While no
 * charbuf functions currently break when the buffer contains NUL bytes, the
 * code is built around the assumption of a NUL terminated string. Be careful how
 * you handle this situation.
 */
void sc_cb_memcpy(struct sc_charbuf *obj, const char *src, ssize_t count);
/**
 * @brief Format and print a string onto the end of a character buffer.
 * @param obj The object to print onto.
 * @param format The format string to print.
 * @param ... The arguments to the format string.
 */
void sc_cb_printf(struct sc_charbuf *obj, const char *format, ...);
/**
 * @brief Format and print a string onto the struct sc_charbuf using a va_list.
 * @param obj The struct sc_charbuf to print into.
 * @param format The format string to print.
 * @param va The vararg list.
 */
void sc_cb_vprintf(struct sc_charbuf *obj, const char *format, va_list va);

/***************
 * Linked List
 *
 * This is essentially the Linux kernel linked list library, but a bit more
 * basic. We don't rely on the nonstandard GNU `typeof()` keyword, and we don't
 * use inline functions either.
 ***************/

/**
 * A linked list node.
 *
 * This structure should be used in two different ways. First, it must be
 * embedded within whatever data structure you'd like to store in a list:
 *
 * @code{.c}
 *     struct some_item_in_a_list {
 *       ... fields ...
 *       struct sc_list_head list;
 *     }
 * @endcode
 *
 * If your structure will be included in several lists at the same time, you can
 * include the list node multiple times with different names. The second way
 * this struct can be used is as the "head" of a list. The head of the list is
 * the variable you use to access it. List heads can be global variables, local
 * variables, or included in structs.
 *
 * Regardless of its use, you MUST call sc_list_init() on every struct before it
 * can be used.
 */
struct sc_list_head {
	struct sc_list_head *next;
	struct sc_list_head *prev;
};

/**
 * @brief Initialize a list's next/prev pointers.
 *
 * @param head the list node to initialized
 *
 * This function must be called before a list head can be used. This sets the
 * next and prev pointers to point to this list itself.
 */
void sc_list_init(struct sc_list_head *head);

/**
 * @brief Insert @a item at the beginning of list headed by @a head.
 * @param head List head which we insert into
 * @param item List node to insert
 */
void sc_list_insert(struct sc_list_head *head, struct sc_list_head *item);

/**
 * @brief Insert @a item at the end of list headed by @a head.
 * @param head List head which we insert into
 * @param item List node to insert
 */
void sc_list_insert_end(struct sc_list_head *head, struct sc_list_head *item);

/**
 * @brief Remove @a item from whatever list contains it.
 * @param item List node to remove
 *
 * Note that this function does not require a pointer to the contained list.
 * Everything it needs can be found from @a item.
 */
void sc_list_remove(struct sc_list_head *item);

/**
 * @brief Move all the items from list `from` to the beginning of `to`.
 * @param from source list head (note that @a from itself is not actually
 * moved into @a to, since we assume it is a static list head).
 * @param to destination list head
 *
 * If @a from is the head of a list containing [A, B, C] and @a to is a list
 * containing [D], then the result would be [A, B, C, D]. The @a from list is
 * always emptied as a result.
 */
void sc_list_move(struct sc_list_head *from, struct sc_list_head *to);

/**
 * @brief Return the length of a list. This is an O(N) operation.
 * @param head head of the list (head element is not included in count)
 * @returns number of elements in the list (excluding @a head)
 */
int sc_list_len(struct sc_list_head *head);

/**
 * @brief Iterate through struct list_head nodes of a list.
 * @param variable a variable name (of type `struct sc_list_head *`) which is
 * set to point at each element in the loop body
 * @param head pointer (of type `struct sc_list_head *`) to the head of the list
 *
 * @warning This is probably not what you want! See #sc_list_for_each_entry() for
 * simpler iteration.
 *
 * This macro allows you to iterate through each `struct sc_list_head *` pointer
 * in a list. Normally, `these` nodes are embedded within a structure at some
 * offset, which means that you need to do some pointer arithmetic to get at the
 * data you care about. The #sc_list_for_each_entry() macro takes care of this
 * for you.
 *
 * As the name may imply, this should be used as if it were a specialized for
 * loop, with a code block to execute for each iteration. The loop may be
 * terminated via break, skipped via continue, etc. The list pointers MUST NOT
 * be modified during iteration.
 */
#define sc_list_for_each(variable, head)                                       \
	for (variable = (head)->next; variable != (head);                      \
	     variable = variable->next)

/**
 * @brief Offset, in bytes, of a field named `member` within type `type`.
 * @param type The name of a type, probably a struct (e.g `struct foo`)
 * @param member The name of a member field within @a type
 */
#define sc_list_offsetof(type, member) ((size_t) & (((type *)0)->member))

/**
 * @brief Return a pointer to the struct containing a member:
 * @param ptr pointer to a member within a struct
 * @param type type of the containing struct (not a pointer, just the type)
 * @param member field name of member within the struct
 */
#define sc_container_of(ptr, type, member)                                     \
	((type *)((char *)ptr - sc_list_offsetof(type, member)))

/**
 * @brief Iterate over every item in a list. Use like a normal for statement.
 * @param var_ptr variable which is a pointer to the container type, used as the
 *     iteration variable
 * @param head pointer to struct list_head which is the header of the list
 * @param field_name name of the struct list_head within the container type
 * @param container_type type of the struct containing struct list_head (not
 *     pointer)
 *
 * This macro differs from the Linux kernel "list_for_each_entry" in that it
 * takes a fourth argument, @a container_type. Unfortunately, the `typeof`
 * construct used by the kernel to implement this is not a standard part of C89.
 * To help this library be maximally compatible, I've added this fourth
 * argument.
 *
 * @warning This interface **does not** allow the list pointers to be modified.
 * If you need this, see #sc_list_for_each_safe().
 *
 * Sample code:
 *
 * @code{.c}
 *     struct my_example {
 *         struct sc_list_head list;
 *         char *message;
 *     }
 *     struct list_head head;
 *
 *     // assume everything is initialized and populated
 *
 *     struct my_example *iter;
 *     sc_list_for_each_entry(iter, &head, list, struct my_example) {
 *         printf("list entry: \"%s\"\n", iter->message);
 *     }
 * @endcode
 */
#define sc_list_for_each_entry(var_ptr, head, field_name, container_type)      \
	for (var_ptr = sc_container_of((head)->next, container_type,           \
	                               field_name);                            \
	     &var_ptr->field_name != (head);                                   \
	     var_ptr = sc_container_of(var_ptr->field_name.next,               \
	                               container_type, field_name))

/**
 * @brief Iterate over every item within a list, allowing for modification
 * @param var_ptr variable which is a pointer to the container type, used as the
 *          iteration variable
 * @param next_ptr variable to hold the next value
 * @param head pointer to struct list_head which is the header of the list
 * @param field_name name of the struct list_head within the container type
 * @param container_type type of the struct containing struct list_head (not
 * pointer)
 *
 * The major difference of this, compared to #sc_list_for_each_entry(), is that
 * it requires two pointers (@a var_ptr and @a next_ptr) for tracking both the
 * current item *and* the next item. By fetching the next item before the body
 * of the list, you are able to modify @a var_ptr, including removing it from
 * the list and even freeing its memory, during the body of the loop.
 *
 * Of course, the result of pre-computing the value is that, if you insert an
 * item into the list (for example, adding an element to the end of the current
 * list), then there's a chance that it could be missed. So please understand
 * that this construct is _not_ magic, it is just a bit of pre-computing for the
 * common cases of removing elements.
 */
#define sc_list_for_each_safe(var_ptr, next_ptr, head, field_name,             \
                              container_type)                                  \
	for (var_ptr = sc_container_of((head)->next, container_type,           \
	                               field_name),                            \
	    next_ptr = sc_container_of(var_ptr->field_name.next,               \
	                               container_type, field_name);            \
	     &var_ptr->field_name != (head); var_ptr = next_ptr,               \
	    next_ptr = sc_container_of(var_ptr->field_name.next,               \
	                               container_type, field_name))

/***************
 * Dynamic Array
 *
 * A dynamically resizing array. The API here uses macros to ease the
 * difficulty, but unfortunately there's no way to avoid declaring the type of
 * the array contents in each operation.
 ***************/

/**
 * @brief Dynamically expanding array
 *
 * This struct represents a generic array which can expand (or even contract, if
 * we implement it). The array elements should be accessed with the convenience
 * macro sc_arr(), which performs the proper casting.
 */
struct sc_array {
	/** @brief Capacity of the array, in elements */
	size_t cap;
	/** @brief Number of elements used by this array */
	size_t len;
	/** @brief Buffer backing this array */
	void *arr;
};

/**
 * @brief Return the array buffer casted to `type *`
 * @param arr_ptr an argument of type ``struct sc_array *``, the array to access
 * @param type the type of element stored in the buffer
 * @returns a pointer to the underlying buffer for indexing
 *
 * An example of how this is intended to be used:
 *
 * @code{.c}
 *     printf("index %d: %d\n", i, sc_arr(&intarr, int)[i]);
 * @endcode
 */
#define sc_arr(arr_ptr, type) ((type *)(arr_ptr)->arr)

/*
 * These are INTERNAL API functions. They are merely declared here since macros
 * must use them. Do not directly use any function prefixed by sc_arr__
 */
void sc_arr__init(struct sc_array *arr, size_t init_cap, size_t elemsize);
void sc_arr__reserve(struct sc_array *arr, size_t idx, size_t count,
                     size_t elemsize);
void sc_arr__remove(struct sc_array *arr, size_t idx, size_t count,
                    size_t elemsize);

/**
 * @brief Initialize a dynamic array
 * @param arr pointer to the array
 * @param type the type of object you wish to store
 * @param init_cap the initial capacity
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_init(arr, type, init_cap)                                       \
	sc_arr__init(arr, init_cap, sizeof(type))

/**
 * @brief Reserve space within the array, moving elements
 * @param arr_ptr pointer to the array
 * @param type type of the contents stored in the array
 * @param idx index to reserve space at
 * @param count number of slots to reserve
 *
 * This function reserves space for `count` elements at index `idx`. It ensures
 * that there is space for `len + count` items (reallocating if necessary).
 * Then, it ensures that the array slots at `idx` are empty, by moving array
 * elements up if necessary. The empty slots are uninitialized and their
 * contents are undefined.
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_reserve(arr_ptr, type, idx, count)                              \
	sc_arr__reserve((arr_ptr), idx, count, sizeof(type));

/**
 * @brief Append an item to the end of the array.
 * @param arr_ptr pointer to the array
 * @param type type of items stored in the array
 * @param val value to append
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_append(arr_ptr, type, val)                                      \
	do {                                                                   \
		sc_arr_reserve(arr_ptr, type, (arr_ptr)->len, 1);              \
		sc_arr(arr_ptr, type)[(arr_ptr)->len - 1] = val;               \
	} while (0)

/**
 * @brief Insert an element at `idx` in the array, possibly shifting elements.
 * @param arr_ptr pointer to the array
 * @param type type of items stored in the array
 * @param idx index to insert at
 * @param val value to insert
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_insert(arr_ptr, type, idx, val)                                 \
	do {                                                                   \
		sc_arr_reserve(arr_ptr, type, idx, 1);                         \
		sc_arr(arr_ptr, type)[idx] = val;                              \
	} while (0)

/**
 * @brief Remove a single element from the array, possibly shifting elements
 * @param arr_ptr pointer to the array
 * @param type type of items stored in the array
 * @param idx index of array to delete
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_remove(arr_ptr, type, idx)                                      \
	sc_arr__remove(arr_ptr, idx, 1, sizeof(type))

/**
 * @brief Remove many elements from the array, possibly shifting elements
 * @param arr_ptr pointer to the array
 * @param type type of items stored in the array
 * @param idx index of first item to remove
 * @param count number of items to remove
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
#define sc_arr_remove_many(arr_ptr, type, idx, count)                          \
	sc_arr__remove(arr_ptr, idx, count, sizeof(type))

/**
 * @brief Destroy the array
 * @param arr array to destroy
 *
 * This need not be done! If the user would like to take ownership of the array
 * once construction is complete, they may simply start using the `arr` field
 * and ensure that they do not use the `struct sc_array` or its functions
 * anymore. Once done with the `arr` field it should be freed with `free()`.
 *
 * This is a macro! The type field is provided as if it were an argument.
 */
void sc_arr_destroy(struct sc_array *arr);
//
#endif // SC_COLLECTIONS_H
