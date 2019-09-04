# linkedList

A way to define a linked list for any given type through two macros.
`CUTILS_DEF_LINKED_LIST_H` which defines the appropriate header info (struct and function
prototypes), and `CUTILS_DEF_LINKED_LIST_C` which define the functions.

## Struct

`head` First node in the list.

`tail` Last element in the list.

`nodeCount` Total nodes in the list.

`callback` The function to call when an element is about to removed from a list. Works like a destructor.

`usrptr` A custom data pointer to pass to the callback function.

```c
typedef struct NAME{
	struct NAME##Node *head;
	struct NAME##Node *tail;
	size_t nodeCount;

	NAME##RemoveFn callback;
	void *usrptr;
} NAME;

`data` The data held in the node.

`prev` The previous node in the list

`next` The next node in the list.

`list` A pointer to the actual list that this node is a part of.

```c
typedef struct NAME##Node{
	TYPE data;
	struct NAME##Node *prev;
	struct NAME##Node *next;
	struct NAME *list;
} NAME##Node;
```

## Functions

>`CUTILS_DEF_LINKED_LIST_H(TYPE, NAME)`

This macro defines the linked list struct. `TYPE` will be what datatype thes struct
holds, `NAME` is the actual struct name.

>`CUTILS_DEF_LINKED_LIST_C(TYPE, NAME, DEFAULT_CALLBACK)`

This macro defines the actual functions to be used with the struct. `TYPE` and `NAME` should be the same as th ones used with `CUTILS_DEF_LINKED_LIST_H`. `DEFAULT_CALLBACK` is the default value for the struct's `callback`.

>`int NAME##Init(NAME *list, size_t initialNodes)`

Intializes the list, and gives it `intialNodes` nodes to start with.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`NAME* NAME##New(size_t initialNodes)`

Allocates a new linked list, and intializes it with `initialNodes`.

This function returns a pointer to a `NAME` struct if no errors occurred, or `NULL` if it failed to allocate, or intialize the struct.

>`int NAME##Copy(NAME *dst, NAME *src)`

Resizes `dst` to have `src->nodeCount` nodes, and copies the node values from `src` to `dst`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`void NAME##Move(NAME *dst, NAME *src)`

Sets `dst` to `src`, and zeroes out `src`.

>`void NAME##Swap(NAME *a, NAME *b)`

Swaps the values of struct `a`, and struct `b`.

>`void NAME##Deinit(NAME *list)`

Deintializes the list by freeing all of its nodes.

>`void NAME##Free(NAME *list)`

Deinitalizes the list by freeing all of its nodes, and then freeing the list. Use this on structs that have been allocates with `NAME##New`.

>`void NAME##SetUserData(NAME *list, void *data)`

Sets `list->usrptr` to `data`.

>`void NAME##SetFreeCallback(NAME *list, NAME##RemoveFn callback)`

Sets `list->callback` to callback.

>`int NAME##Resize(NAME *list, size_t count)`

Resizes the amount of nodes `list` has to be the same as `count`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##AllocNodes(size_t count, NAME *list, NAME##Node **head, NAME##Node **tail)`

Alloctes `count` nodes. Sets `head` to point to the head of the allocated nodes, and `tail` to point the tail of the allocates nodes`. `node->list` will be set to `list`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBack(NAME *list, TYPE x)`

Attempts to add a node containing the value `x` to the end of the list.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBackNode(NAME *list, NAME##Node *x)`

Attempts to add a node containing the value `x->data` to the end of the list.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBackList(NAME *list, NAME *x)`

Attempts to add the values in all the nodes in `x` to the end of the list.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`void NAME##ConnectNode(NAME##Node *node, NAME##Node *x)`

Connects `x` to `the end to `node` by setting their prev, and next pointers to the appropriate values.`

>`void NAME##ConnectNodes(NAME##Node *node, NAME##Node *x)`

Connects the `x` to `node`, and the last node in `x` to whatever `node->next` points to.

>`void NAME##ConnectList(NAME##Node *node, NAME *x)`

Connects the `x` to `node`, and the last node in `x` to whatever `node->next` points to.

>`void NAME##ConnectNodeEnd(NAME *list, NAME##Node *x)`

Connects `x` to `list->tail`.

>`void NAME##ConnectNodesEnd(NAME *list, NAME##Node *x)`

Connects all the nodes in `x` to `list->tail`.

>`void NAME##ConnectListEnd(NAME *list, NAME *x)`

Connects all the nodes in `x` to `list->tail`.

>`void NAME##RemoveNode(NAME##Node *node)`

Removes `node` from the list that it belongs to.

>`void NAME##RemoveNodes(NAME##Node *start, NAME##Node *end)`

Removes all nodes from `start` to `end` from the list that they belong to.

>`int NAME##Insert(NAME##Node *node, TYPE x)`

Creates a new node with the value `x`, and inserts it into `node`'s place.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##InsertNode(NAME##Node *node, NAME##Node *x)`

Creates a new node with the value in `x`, and inserts it into `node`'s place.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##InsertNodes(NAME##Node *node, NAME##Node *x)`

Creates new nodes with the values in `x`, and inserts them into `node`'s place.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##InsertList(NAME##Node *node, NAME *x)`

Creates new nodes with the values in `x`, and inserts them into `node`'s place.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.
