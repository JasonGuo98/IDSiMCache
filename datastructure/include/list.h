/* modified by Xiaojun GUo for use in IDSiMCache 202206
 */

//
// list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef __CLIBS_LIST_H__
#define __CLIBS_LIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
  // #include <malloc.h>

  // Library version

#define LIST_VERSION "0.2.0"

// Memory management macros
#ifdef LIST_CONFIG_H
#define _STR(x) #x
#define STR(x) _STR(x)
#include STR(LIST_CONFIG_H)
#undef _STR
#undef STR
#endif

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

  /*
   * list_t iterator direction.
   */

  typedef enum //枚举量，标识方向
  {
    LIST_HEAD, //从链表头部开始，指向链表尾部，向后的方向
    LIST_TAIL  //从链表尾部开始，指向链表头部，向前的方向
  } list_direction_t;

  /*
   * list_t node struct.
   */

  typedef struct list_node //链表节点
  {
    struct list_node *prev;
    struct list_node *next;
    void *val; //对象指针
  } list_node_t;

  /*
   * list_t struct.
   */

  typedef struct //链表结构体
  {
    list_node_t *head;              //头节点指针
    list_node_t *tail;              //尾节点指针
    unsigned int len;               //链表长度
    void (*free)(void *val);        // free函数指针，用于释放链表节点的val元素(val元素是节点值对象的指针)
    int (*match)(void *a, void *b); // match函数指针，用于判断两个链表节点的val元素是否相等
  } list_t;

  /*
   * list_t iterator struct.
   */

  typedef struct //链表迭代器
  {
    list_node_t *next;
    list_direction_t direction; //迭代方向(LIST_HEAD表示从头部开始向后进行迭代,即从前向后,LIST_TAIL表示从尾部开始向前进行迭代，即从后向前)
  } list_iterator_t;

  // Node prototypes.

  //传入一个val元素对象指针，创建一个新链表节点，创建失败则返回NULL，val为缓存对象指针，删除链表节点时需先判断是否需要删除缓存对象
  list_node_t *list_node_new(void *val);

  // list_t prototypes.

  list_t *list_new(void); //创建一个新链表，创建失败则返回NULL

  void list_destroy(list_t *self); //销毁链表

  list_node_t *list_rpush(list_t *self, list_node_t *node); //尾插，返回值为插入的链表节点指针

  list_node_t *list_lpush(list_t *self, list_node_t *node); //头插，返回值为插入的链表节点指针

  list_node_t *list_rpop(list_t *self); //删除尾部节点，返回删除的节点指针(只是在链表中删除了，还没有free这个节点)

  list_node_t *list_lpop(list_t *self); //删除头部节点，返回删除的节点指针(只是在链表中删除了，还没有free这个节点)

  //在链表中查找val元素指针与传入的val指针相同的节点，找到则返回节点指针，否则返回NULL，时间复杂度O(n)
  list_node_t *list_find(list_t *self, void *val);

  //传入index，若为正数，则返回第index+1个元素([0,n-1]->第1~n个)，若为负数，则返回倒数第-index(如index=-1，则表示倒数第1个元素)个元素
  list_node_t *list_at(list_t *self, int index);

  void list_remove_destroy(list_t *self, list_node_t *node); //传入一个节点指针，在链表中删除该节点，并销毁(free)该节点

  void list_remove(list_t *self, list_node_t *node); //传入一个节点指针，在链表中删除该节点，并返回该节点指针

  // list_t iterator prototypes.

  //传入一个链表节点指针，创建一个链表迭代器(指向传入的节点)，失败则返回NULL
  list_iterator_t *list_iterator_new_from_node(list_node_t *node, list_direction_t direction);

  //传入一个链表指针，创建一个链表迭代器(根据迭代方向来决定指向头结点还是尾节点)，失败则返回NULL
  list_iterator_t *list_iterator_new(list_t *list, list_direction_t direction);

  //迭代器进行迭代，返回当前链表节点的指针(即迭代器已经指向了下一个链表节点，但返回的是当前链表节点的指针)
  list_node_t *list_iterator_next(list_iterator_t *self);

  //销毁链表迭代器
  void list_iterator_destroy(list_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __CLIBS_LIST_H__ */
