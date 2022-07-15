
//
// list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new list_t. NULL on failure.
 */

list_t *list_new(void) //创建一个新链表
{
  list_t *self;
  if (!(self = (list_t *)LIST_MALLOC(sizeof(list_t)))) //申请空间失败，则返回NULL
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->free = NULL;
  self->match = NULL;
  self->len = 0;
  return self;
}

/*
 * Free the list.
 */

void list_destroy(list_t *self) //销毁链表
{
  unsigned int len = self->len;
  list_node_t *next;
  list_node_t *curr = self->head;

  while (len--) //遍历链表，free链表节点
  {
    next = curr->next;
    if (self->free) //若定义了free函数(链表节点的val元素需要被free)，则先对节点的val元素进行free
      self->free(curr->val);
    LIST_FREE(curr); //再free链表节点
    curr = next;     //下一个节点
  }

  LIST_FREE(self);
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 */

list_node_t *list_rpush(list_t *self, list_node_t *node) //尾插，返回值为插入的链表节点指针
{
  if (!node)
    return NULL;

  if (self->len) //若链表已经有节点，则直接从尾部插入
  {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  }
  else //若链表为空，则将头结点指针和尾节点指针均指向当前节点
  {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node; //返回当前节点指针
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 */

list_node_t *list_lpush(list_t *self, list_node_t *node) //头插，返回值为插入的链表节点指针
{
  if (!node)
    return NULL;

  if (self->len) //若链表已经有节点，则直接从头部插入
  {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  }
  else //若链表为空，则将头结点指针和尾节点指针均指向当前节点
  {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node; //返回当前节点指针
}

/*
 * Return / detach the last node in the list, or NULL.
 */

list_node_t *list_rpop(list_t *self) //删除尾部节点，返回删除的节点指针(只是在链表中删除了，还没有free这个节点)
{
  if (!self->len)
    return NULL;

  list_node_t *node = self->tail;

  if (--self->len) //若删除后链表不为空
  {
    (self->tail = node->prev)->next = NULL; //尾节点指针指向前一个节点，并将其next指针置为NULL
  }
  else //删除后链表为空
  {
    self->tail = self->head = NULL; //头结点指针和尾节点指针均置为NULL
  }

  node->next = node->prev = NULL;
  return node; //返回这个被删除的节点的指针
}

/*
 * Return / detach the first node in the list, or NULL.
 */

list_node_t *list_lpop(list_t *self) //删除头部节点，返回删除的节点指针(只是在链表中删除了，还没有free这个节点)
{
  if (!self->len)
    return NULL;

  list_node_t *node = self->head;

  if (--self->len) //若删除后链表不为空
  {
    (self->head = node->next)->prev = NULL; //头节点指针指向后一个节点，并将其prev指针置为NULL
  }
  else //删除后链表为空
  {
    self->head = self->tail = NULL; //头结点指针和尾节点指针均置为NULL
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return the node associated to val or NULL.
 */

//在链表中查找val元素指针与传入的val指针相同的节点，找到则返回节点指针，否则返回NULL，时间复杂度O(n)
list_node_t *list_find(list_t *self, void *val)
{
  list_iterator_t *it = list_iterator_new(self, LIST_HEAD); //创建一个迭代器，从头结点开始向尾部进行迭代
  list_node_t *node;

  while ((node = list_iterator_next(it))) //迭代器迭代，并将迭代到的链表节点指针赋值给node
  {
    if (self->match) //若定义了match函数
    {
      if (self->match(val, node->val)) //使用match函数判断当前节点的val和传入的val是否相等
      {
        //若相等，则删除迭代器，并返回当前节点指针
        list_iterator_destroy(it);
        return node;
      }
    }
    else //若没定义match函数，则手动判断
    {
      if (val == node->val) //若当期节点的val指针和传入的val指针相同，注意这里判断的是指针相同而非值相同
      {
        //若相同，则删除迭代器，并返回当前节点指针
        list_iterator_destroy(it);
        return node;
      }
    }
  }
  //循环退出，说明没找到含有val元素指针的链表节点，删除迭代器，返回NULL
  list_iterator_destroy(it);
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 */

//传入index，若为正数，则返回第index+1个元素([0,n-1]->第1~n个)，若为负数，则返回倒数第-index(如index=-1，则表示倒数第1个元素)个元素
list_node_t *list_at(list_t *self, int index)
{
  list_direction_t direction = LIST_HEAD; //从链表头结点开始向后迭代

  if (index < 0) //若传入的index为负数，如-x，则视为倒数第x个，从尾部开始向前迭代
  {
    direction = LIST_TAIL;
    index = ~index; //对一个数取反的结果为其相反数-1，~index=-index-1，例如-1 -> 0
  }

  if ((unsigned)index < self->len) // index小于链表长度，则找到对应的第index个元素，然后返回
  {
    list_iterator_t *it = list_iterator_new(self, direction); //创建迭代器
    list_node_t *node = list_iterator_next(it);               //迭代器先进行一次迭代(因为index的范围为[0,n-1])
    while (index--)                                           //迭代器迭代，index递减，直到index为0为止
      node = list_iterator_next(it);
    list_iterator_destroy(it); //销毁迭代器
    return node;               //返回对应位置的节点指针
  }
  //若index大于等于链表长度，则超出了链表范围，返回NULL
  return NULL;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 */

//传入一个节点指针，在链表中删除该节点，并销毁该节点
void list_remove_destroy(list_t *self, list_node_t *node)
{
  //在链表中删除该节点
  node->prev
      ? (node->prev->next = node->next)
      : (self->head = node->next);

  node->next
      ? (node->next->prev = node->prev)
      : (self->tail = node->prev);

  // free该节点的val对象
  if (self->free)
    self->free(node->val);
  // free该节点
  LIST_FREE(node);
  --self->len;
}

//传入一个节点指针，在链表中删除该节点，不销毁该节点
void list_remove(list_t *self, list_node_t *node)
{
  //在链表中删除该节点
  node->prev
      ? (node->prev->next = node->next)
      : (self->head = node->next);

  node->next
      ? (node->next->prev = node->prev)
      : (self->tail = node->prev);

  --self->len;
}
