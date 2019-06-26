#include <iostream>
#include <map>
#include <assert.h>
#include <stdio.h>

using namespace std;

class node
{
public:
	node(int ikey, int ivalue)
	{
		prev = next = NULL;
		key = ikey;
		value = ivalue;
	}
	~node()
	{
	}
public:
	int key;
	int value;
	node *prev;
	node *next;
};

/*双向链表*/
class doublelist 
{
public:
	doublelist()
	{
		head = tail = NULL;
	}
	~doublelist()
	{
	}

	void move_to_tail(node *pnode)
	{
		if(head == pnode) {
			head = pnode->next;
		}

		if(tail == pnode)
			return;

		if(pnode->prev)
			pnode->prev->next = pnode->next;
		if(pnode->next)
			pnode->next->prev = pnode->prev;

		inert_at_tail(pnode);
	}

	void inert_at_tail(node *pnode)
	{
		if(!tail){
			head = pnode;
			tail = pnode;
			pnode->next = NULL;
			pnode->prev = NULL;
			return;
		}

		tail->next = pnode;
		pnode->prev = tail;
		pnode->next = NULL;
		
		tail = pnode;
	}	

	int get_head(void)
	{
		assert(head != NULL);

		return head->key;
	}

	void remove_head(void)
	{
		if(head != NULL) {
			head->next->prev = NULL;
			node *pnode = head;
			head = head->next;
			delete pnode;
		}
	}

	void dump(void) 
	{
		node *pnode = head;
		while(pnode != NULL) {
			printf("(%d, %d) - ", pnode->key, pnode->value);
			pnode = pnode->next;
		}
		printf("\n");
	}

	

private:
	node *head, *tail;
};

class lrucache 
{
public:
	lrucache(int size)
	{
		maxsize = size;
	}

	~lrucache()
	{
		
	}

	int get(int key)
	{
		map<int, node *>::iterator it;
		it = lrumap.find(key);
		/*不存在则返回-1 */
		if(it == lrumap.end()) {
			return -1;
		}
		/*如果存在则将该节点移动到链表尾，并返回value */
		node *pnode = it->second;
		lrulist.move_to_tail(pnode);
		return pnode->value;
	}

	void put(int key, int value)
	{
		map<int, node*>::iterator it;
		it = lrumap.find(key);
		/*存在则更新节点，并将节点移动到链表尾*/
		if(it != lrumap.end()) {
			node *pnode = it->second;
			pnode->value = value;
			lrulist.move_to_tail(pnode);
			return;
		}

		/*如果不存在且没有超限，插入节点并将节点移动到链表尾*/
		if(lrumap.size() < maxsize) {
			node *pnode = new node(key, value);
			lrumap.insert(make_pair(key, pnode));
			lrulist.inert_at_tail(pnode);
		} else { /*否则删除链表头元素，插入新节点，并将节点移动到链表尾*/
			lrumap.erase(lrulist.get_head());
			lrulist.remove_head();
			node *pnode = new node(key, value);
			lrumap.insert(make_pair(key, pnode));
			lrulist.inert_at_tail(pnode);
		}
	}

	void dump(void)
	{
		map<int, node*>::iterator it;
		for(it = lrumap.begin(); it != lrumap.end(); it++) {
			printf("<%d, %d> - ", it->first, it->second->value);
		}
		printf("\n");
		lrulist.dump();
	}

private:
	map<int, node *> lrumap;
	doublelist lrulist;
	int maxsize;
};

int main(void) 
{
	lrucache *cache = new lrucache(3);
	cache->put(1, 1);
	cache->put(2, 2);
	cache->put(3, 10);
	cache->dump();
	int value = cache->get(1);
	printf("cache get %d value %d\n", 1, value);
	value = cache->get(2);
	printf("cache get %d value %d\n", 2, value);
	cache->dump();
	cache->put(4, 4);
	cache->put(5, 5);
	cache->put(6, 6);
	cache->put(7, 7);
	cache->dump();
	value = cache->get(3);
	printf("cache get %d value %d\n", 3, value);
	value = cache->get(6);
	printf("cache get %d value %d\n", 6, value);
	cache->dump(); 
	
	return 0;
	
}
