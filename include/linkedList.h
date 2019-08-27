#ifndef CUTILS_LINKED_LIST_H
#define CUTILS_LINKED_LIST_H

#include "errors.h"

#define CUTILS_DEF_LINKED_LIST_H(TYPE, NAME)\
	struct NAME;\
	struct NAME##Node;\
	typedef struct NAME{\
		struct NAME##Node *head;\
		struct NAME##Node *tail;\
		size_t nodeCount;\
	} NAME;\
	typedef struct NAME##Node{\
		TYPE data;\
		struct NAME##Node *prev;\
		struct NAME##Node *next;\
		struct NAME *list;\
	} NAME##Node;\
\
	void NAME##Init(NAME *list, size_t initialNodes);\
	NAME* NAME##New(size_t initialNodes);\
	int NAME##Copy(NAME *dst, NAME *src);\
	void NAME##Move(NAME *dst, NAME *src);\
	void NAME##Swap(NAME *a, NAME *b);\
	void NAME##Free(NAME *list);\
	int NAME##Resize(NAME *list, size_t count);\
	void NAME##AllocNodes(size_t count, NAME *list, NAME##Node **head, NAME##Node **tail);\
	size_t NAME##FreeNodes(NAME##Node *nodes);\
	int NAME##PushBack(NAME *list, TYPE x);\
	int NAME##PushBackNode(NAME *list, NAME##Node *x);\
	int NAME##PushBackList(NAME *list, NAME *x);\
	void NAME##ConnectNode(NAME##Node *node, NAME##Node *x);\
	void NAME##ConnectNodes(NAME##Node *node, NAME##Node *x);\
	void NAME##ConnectList(NAME##Node *node, NAME *x);\
	void NAME##RemoveNode(NAME##Node *node);\
	void NAME##RemoveNodes(NAME##Node *start, NAME##Node *end);\
	int NAME##Insert(NAME##Node *node, TYPE x);\
	int NAME##InsertNode(NAME##Node *node, NAME##Node *x);\
	int NAME##InsertNodes(NAME##Node *node, NAME##Node *x);\
	int NAME##InsertList(NAME##Node *node, NAME *x);\

#define CUTILS_DEF_LINKED_LIST_C(TYPE, NAME)\
	void NAME##Init(NAME *list, size_t initialNodes){\
		if(initialNodes == 0){\
			list->head = NULL;\
			list->tail = NULL;\
			list->nodeCount = 0;\
		}\
	}\
\
	NAME* NAME##New(size_t initialNodes){\
		NAME *ret = malloc(sizeof(NAME));\
		if(ret == NULL){\
			return NULL;\
		}\
		NAME##Init(ret, initialNodes);\
		return ret;\
	}\
\
	int NAME##Copy(NAME *dst, NAME *src){\
		int err = NAME##Resize(dst, src->nodeCount);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		NAME##Node *currentSrc = src->head;\
		NAME##Node *currentDst = dst->head;\
		/*doesn't matter which we check, both lists should be the same size now*/\
		while(currentSrc != NULL){\
			currentDst->data = currentSrc->data;\
			currentDst = currentDst->next;\
			currentSrc = currentSrc->next;\
		}\
\
		return CUTILS_OK;\
	}\
\
	void NAME##Move(NAME *a, NAME *b){\
		*a = *b;\
		memset(b, 0, sizeof(NAME));\
	}\
\
	void NAME##Free(NAME *list){\
		NAME##Node *current = list->head;\
		NAME##Node *next;\
		while(current != NULL){\
			next = current->next;\
			free(current);\
			current = next;\
		}\
		memset(list, 0, sizeof(NAME));\
	}\
\
	int NAME##Resize(NAME *list, size_t count){\
		if(list->nodeCount > count){\
			/*doesn't make sense to use NAME##FreeNodes here we would have to traverse*/\
			/*the list in reverse only for NAME##FreeNodes to traverse it forward again*/\
			NAME##Node *current = list->tail;\
			NAME##Node *prev = current->prev;\
			while(list->nodeCount > count){\
				free(current);\
				current = prev;\
				prev = current->prev;\
				list->nodeCount--;\
			}\
			list->tail = current;\
		}\
		else if(list->nodeCount < count){\
			size_t diff = count-list->nodeCount;\
			NAME##Node *head, *tail;\
			NAME##AllocNodes(diff, list, &head, &tail);\
			if(head == NULL){\
				return CUTILS_NOMEM;\
			}\
\
			if(list->nodeCount == 0){\
				list->head = head;\
				list->tail = tail;\
			}\
			else{\
				list->tail->next = head;\
				head->prev = list->tail;\
				list->tail = tail;\
			}\
		}\
\
		list->nodeCount = count;\
\
		return CUTILS_OK;\
	}\
\
	void NAME##AllocNodes(size_t count, NAME *list, NAME##Node **head, NAME##Node **tail){\
		*head = *tail = NULL;\
\
		if(count == 0){\
			return;\
		}\
\
		*head = malloc(sizeof(NAME##Node));\
		if(*head == NULL){\
			return;\
		}\
		(*head)->prev = (*head)->next = NULL;\
		(*head)->list = list;\
\
		*tail = *head;\
		/*-1 because we already have one allocated*/\
		for(size_t i = 0; i < count-1; i++){\
			NAME##Node *tmp = malloc(sizeof(NAME##Node));\
			if(tmp == NULL){\
				NAME##FreeNodes(*head);\
				*head = NULL;\
				*tail = NULL;\
				return;\
			}\
\
			tmp->prev = *tail;\
			tmp->list = list;\
			(*tail)->next = tmp;\
			*tail = tmp;\
		}\
		(*tail)->next = NULL;\
	}\
\
	size_t NAME##FreeNodes(NAME##Node *nodes){\
		NAME##Node *current = nodes;\
		NAME##Node *next;\
		size_t nodesDeleted = 0;\
		while(current != NULL){\
			/*dont put this at the bottom because then we derefrence a null pointer*/\
			next = current->next;\
			free(current);\
			current = next;\
			nodesDeleted++;\
		}\
		return nodesDeleted;\
	}\
\
	int NAME##PushBack(NAME *list, TYPE x){\
		int err = NAME##Resize(list, list->nodeCount+1);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		list->tail->data = x;\
\
		return CUTILS_OK;\
	}\
\
	int NAME##PushBackNode(NAME *list, NAME##Node *x){\
		int err = NAME##Resize(list, list->nodeCount+1);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		list->tail->data = x->data;\
\
		return CUTILS_OK;\
	}\
\
	int NAME##PushBackList(NAME *list, NAME *x){\
		if(list->nodeCount == 0){\
			return NAME##Copy(list, x);\
		}\
\
		NAME##Node *oldTail = list->tail;\
		int err = NAME##Resize(list, list->nodeCount+x->nodeCount);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		NAME##Node *currentList = oldTail->next;\
		NAME##Node *currentX = x->head;\
		while(currentList != NULL){\
			currentList->data = currentX->data;\
			currentList = currentList->next;\
			currentX = currentX->next;\
		}\
\
		return CUTILS_OK;\
	}\
\
	void NAME##ConnectNode(NAME##Node *node, NAME##Node *x){\
		NAME *list = node->list;\
\
		x->next = node->next;\
		x->prev = node;\
		x->list = list;\
\
		if(node->next != NULL){\
			node->next->prev = x;\
		}\
		node->next = x;\
		if(list->tail == node){\
			list->tail = x;\
		}\
		list->nodeCount++;\
	}\
\
	void NAME##ConnectNodes(NAME##Node *node, NAME##Node *x){\
		NAME *list = node->list;\
\
		NAME##Node *head = x;\
		NAME##Node *tail;\
		NAME##Node *tmp = x;\
		size_t nodeCount = 0;\
		while(tmp != NULL){\
			tail = tmp;\
			tmp->list = list;\
			tmp = tmp->next;\
			nodeCount++;\
		}\
\
		head->prev = node;\
		tail->next = node->next;\
		if(node->next != NULL){\
			node->next->prev = tail;\
		}\
		node->next = head;\
		if(list->tail == node){\
			list->tail = tail;\
		}\
\
		list->nodeCount += nodeCount;\
	}\
\
	void NAME##ConnectList(NAME##Node *node, NAME *x){\
		NAME *list = node->list;\
\
		NAME##Node *current = x->head;\
		while(current != NULL){\
			current->list = list;\
		}\
\
		x->head->prev = node;\
		x->tail->next = node->next;\
		if(node->next != NULL){\
			node->next->prev = x->tail;\
		}\
		node->next = x->head;\
		if(list->tail == node){\
			list->tail = x->tail;\
		}\
\
		list->nodeCount += x->nodeCount;\
		memset(x, 0, sizeof(NAME));\
	}\
\
	void NAME##RemoveNode(NAME##Node *node){\
		/*if node is head and tail i.e. 1 length list*/\
		if(node->prev == NULL && node->next == NULL){\
			node->list->head = NULL;\
			node->list->tail = NULL;\
		}\
		/*node is head*/\
		else if(node->prev == NULL){\
			node->list->head = node->next;\
			node->next->prev = NULL;\
		}\
		/*node is tail*/\
		else if(node == node->list->tail){\
			node->list->tail = node->prev;\
			node->prev->next = NULL;\
		}\
		/*not head or tail*/\
		else{\
			node->prev->next = node->next;\
			node->next->prev = node->prev;\
		}\
\
		node->list->nodeCount--;\
		free(node);\
	}\
\
	void NAME##RemoveNodes(NAME##Node *start, NAME##Node *end){\
		NAME *list = start->list;\
		NAME##Node *before = start->prev;\
		NAME##Node *after= end->next;\
\
		NAME##Node *current = start;\
		NAME##Node *next;\
		size_t counter = 0;\
		while(current != end->next){\
			next = current->next;\
			free(current);\
			current = next;\
			counter++;\
		}\
\
		list->nodeCount -= counter;\
\
		if(list->nodeCount == 0){\
			list->head = NULL;\
			list->tail = NULL;\
			return;\
		}\
\
		if(start == list->head){\
			list->head = after;\
			after->prev = NULL;\
			return;\
		}\
\
		if(end == list->tail){\
			list->tail = before;\
			before->next = NULL;\
			return;\
		}\
\
		before->next = after;\
		after->prev = before;\
	}\
\
	int NAME##Insert(NAME##Node *node, TYPE x){\
		NAME *list = node->list;\
\
		NAME##Node *newNode = malloc(sizeof(NAME##Node));\
		if(newNode == NULL){\
			return CUTILS_NOMEM;\
		}\
\
		newNode->prev = node->prev;\
		newNode->next = node;\
		newNode->data = x;\
		newNode->list = list;\
		node->prev = newNode;\
\
		if(newNode->prev != NULL){\
			newNode->prev->next = newNode;\
		}\
		if(list->head == node){\
			list->head = newNode;\
		}\
\
		list->nodeCount++;\
\
		return CUTILS_OK;\
	}\
\
	int NAME##InsertNode(NAME##Node *node, NAME##Node *x){\
		return NAME##Insert(node, x->data);\
	}\
\
	int NAME##InsertNodes(NAME##Node *node, NAME##Node *x){\
		NAME *list = node->list;\
\
		size_t nodeCount = 0;\
		NAME##Node *tmp = x;\
		while(tmp != NULL){\
			tmp = tmp->next;\
			nodeCount++;\
		}\
\
		NAME##Node *head, *tail;\
		NAME##AllocNodes(nodeCount, list, &head, &tail);\
		if(head == NULL){\
			return CUTILS_NOMEM;\
		}\
\
		NAME##Node *currentAlloc = head;\
		NAME##Node *currentX = x;\
		while(currentAlloc != NULL){\
			currentAlloc->data = currentX->data;\
			currentAlloc = currentAlloc->next;\
			currentX = currentX->next;\
		}\
\
		head->prev = node->prev;\
		tail->next = node;\
		if(node->prev != NULL){\
			node->prev->next = head;\
		}\
		node->prev = tail;\
		if(list->head == node){\
			list->head = head;\
		}\
\
		return CUTILS_OK;\
	}\
\
	int NAME##InsertList(NAME##Node *node, NAME *x){\
		NAME *list = node->list;\
\
		NAME##Node *head, *tail;\
		NAME##AllocNodes(x->nodeCount, list, &head, &tail);\
		if(head == NULL){\
			return CUTILS_NOMEM;\
		}\
\
		NAME##Node *currentAlloc = head;\
		NAME##Node *currentX = x->head;\
		while(currentAlloc != NULL){\
			currentAlloc->data = currentX->data;\
			currentAlloc = currentAlloc->next;\
			currentX = currentX->next;\
		}\
\
		head->prev = node->prev;\
		tail->next = node;\
		if(node->prev != NULL){\
			node->prev->next = head;\
		}\
		node->prev = tail;\
		if(list->head == node){\
			list->head = head;\
		}\
\
		list->nodeCount += x->nodeCount;\
\
		return CUTILS_OK;\
	}

#endif
