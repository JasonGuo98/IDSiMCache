
//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"
#include <stdio.h>
/*
 * Allocates a new list_node_t. NULL on failure.
 */

//传入一个val元素对象指针，创建一个新链表节点，创建失败则返回NULL，val为节点值对象指针，删除链表节点时需先判断是否需要删除节点值对象
list_node_t *list_node_new(void *val)
{
  list_node_t *self = NULL;
  if (!(self = (list_node_t *)LIST_MALLOC(sizeof(list_node_t)))) //若申请空间失败，返回NULL
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val; // val为节点值对象指针，删除链表节点时需先判断是否需要删除缓存对象
  return self;
}