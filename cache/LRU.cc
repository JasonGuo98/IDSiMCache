#include "LRU.hh"

static void list_remove_and_rpush(list_t * self, list_node_t * node)
{

   node->prev
    ? (node->prev->next = node->next)
    : (self->head = node->next);

  node->next
    ? (node->next->prev = node->prev)
    : (self->tail = node->prev);
    --self->len;
    list_rpush(self, node);    
}
uint64_t LRUCache::set(uint64_t key,  uint64_t value = 0)
{
    auto iter = lookup_table.find(key);
    if (iter != lookup_table.end())
    {
        list_remove_and_rpush(list_p, iter->second);
        return 0;
    }


    oneCacheLine_t * cl = (oneCacheLine_t * )malloc(sizeof(oneCacheLine_t));
    assert(cl!=NULL);
    cl->key = key;
    cl->value = value;
    cl->prio = current_access_count;

    list_node_t * node_p = list_node_new(cl);
    list_rpush(list_p, node_p);
    lookup_table[key] = node_p;

    current_size ++;

    if (current_size > cache_capacity)
    {
        list_node_t *lru_item = list_lpop(list_p);
        
        assert(lru_item);
        oneCacheLine_t * del_cl = (oneCacheLine_t * )(lru_item->val);
        
        uint64_t del_key = del_cl->key;
        auto iter = lookup_table.find(del_key);
        assert(iter!=lookup_table.end());
        lookup_table.erase(iter);
        free(del_cl);
        free(lru_item);
        current_size--;
    }

    return 0;
}

uint64_t LRUCache::get(uint64_t key,  void * p)
{
    
    auto iter = lookup_table.find(key);

    if(iter != lookup_table.end())
    {
        list_remove_and_rpush(list_p, iter->second);
        return 1;
    }
    return 0;
}

uint64_t LRUCache::next()
{
    current_access_count++;
    return current_access_count;
}

uint64_t LRUCache::reset(uint64_t new_capacity = 0)
{
    while(1)
    {
        list_node_t *lru_item = list_lpop(list_p);
        if(! lru_item)
            break;
        free(lru_item->val);
        free(lru_item);
    }
    list_destroy(list_p);
    list_p = list_new();
    lookup_table.clear();
    current_size = 0;
    current_access_count = 0;
    if(new_capacity != 0)
    {
        cache_capacity = new_capacity;
    }
    return 0;
}

LRUCache:: ~LRUCache()
{
    this->reset();
    list_destroy(list_p);
} 