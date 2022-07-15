
//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new list_iterator_t with the given start
 * node. NULL on failure.
 */

//传入一个链表节点指针，创建一个链表迭代器(指向传入的节点)，失败则返回NULL
list_iterator_t *list_iterator_new_from_node(list_node_t *node, list_direction_t direction)
{
  list_iterator_t *self;
  if (!(self = (list_iterator_t *)LIST_MALLOC(sizeof(list_iterator_t)))) //申请空间失败，则返回NULL
    return NULL;
  self->next = node;
  self->direction = direction;
  return self;
}

/*
 * Allocate a new list_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */

//传入一个链表指针，创建一个链表迭代器(根据迭代方向来决定指向头结点还是尾节点)，失败则返回NULL
list_iterator_t *list_iterator_new(list_t *list, list_direction_t direction)
{
  list_node_t *node = direction == LIST_HEAD
                          ? list->head
                          : list->tail;                //判断迭代方向
  return list_iterator_new_from_node(node, direction); //传入头结点/尾节点指针以及迭代方向，创建一个迭代器并返回
}

/*
 * Return the next list_node_t or NULL when no more
 * nodes remain in the list.
 */

//迭代器进行迭代，返回当前链表节点的指针(即迭代器已经指向了下一个链表节点，但返回的是当前链表节点的指针)
list_node_t *list_iterator_next(list_iterator_t *self)
{
  list_node_t *curr = self->next; //当前指向的节点
  if (curr)                       //若不为NULL
  {
    // direction为LIST_HEAD，则从头结点开始向尾节点方向进行迭代，否则(为LIST_TAIL)，则从尾结点开始向头节点方向进行迭代
    self->next = self->direction == LIST_HEAD
                     ? curr->next
                     : curr->prev;
  }
  return curr;
}

/*
 * Free the list iterator.
 */
//销毁链表迭代器
void list_iterator_destroy(list_iterator_t *self)
{
  LIST_FREE(self);
  self = NULL;
}
