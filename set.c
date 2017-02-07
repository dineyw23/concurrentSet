#include "set.h"

typedef struct setNode {
	struct setNode* next;
	pthread_mutex_t* mutex;
	void* data;
}setNode_t;

typedef struct set {
	setNode_t* head;
	int (*compare)(void* , void* );
}set_t;


/*
 * setNode_new: Helper function to create a new setNode
 * @data: the user's data
 * @next: next pointer of current element
 * 
 * setNode_t* to the new node upon success, or NULL upon malloc failure
 * Memory Requirement: memory for allocating stuct setNode on heap
 *  
*/
static setNode_t* setNode_new(void* data, setNode_t* next)
{

	setNode_t* new = malloc(sizeof(setNode_t));
	if (new == NULL){
		fprintf(stderr, "%s\n","Malloc setNode_t");
		return NULL;
	} 

	new->mutex = malloc(sizeof(pthread_mutex_t));
	if(new->mutex == NULL){
		fprintf(stderr, "%s\n","Malloc mutex_t");
		return NULL;
	}
	pthread_mutex_init(new->mutex,NULL);
	new->data = data;
	new->next = next;
	return new;
}

/*
 * new_set: create a new set
 * @comapre: function to compare user-defined data object(REQUIRED)
 * 
 * set_t* to the new set upon success, or NULL upon malloc failure
 * Memory Requirement: memory for allocating stuct set on heap
 * 
 * Idea of compare taken from qsort in linux  source code. 
 * To gaurantee the any user-defined data type is acceptable. Comparison function 
 * should be provided by user.
 */ 
set_t* new_set(int (*compare)(void* , void* ))
{
	if(compare == NULL){
		fprintf(stderr, "%s\n", "Input type can be anything but compare function required.");
		return NULL;
	}
	set_t* setHead = malloc(sizeof(set_t));
	if (setHead == NULL){
		fprintf(stderr, "%s\n","Malloc set_t");
		return NULL;
	}
	setHead->compare = compare;
	setHead->head = setNode_new(0,NULL);
	return setHead;
}


/*
 * set_contains: find data matched with a given key, if any
 * @set:  the target set
 * @data:  the key to be matched
 * @return: Returns a bool indicating whether specified key/Object exsists in set
 * 
 * Memory: None - Just iterates over the list
 * Time Complexity: O(n) where n is number of elements in the list
 */
bool set_contains(set_t* set, void* data)
{
 pthread_mutex_lock(set->head->mutex);
  setNode_t* elem = set->head;
  if (elem->next == NULL){
    pthread_mutex_unlock(elem->mutex);
    return false;
  }
  setNode_t* prev = elem;

  while (elem->next != NULL && set->compare(elem->next->data,data) <= 0){
     if(set->compare(elem->next->data,data) == 0){
      pthread_mutex_unlock(elem->mutex);
      return true;
    }
    prev = elem;
    elem = elem->next;
    pthread_mutex_lock(elem->mutex);
    pthread_mutex_unlock(prev->mutex);
  }
    if(set->compare(elem->data,data) == 0){
    	pthread_mutex_unlock(elem->mutex);
    return true;
  }
  pthread_mutex_unlock(elem->mutex);
  return false;

}

/*
 * set_add: add data to the set specified at the correct position 
 * @set:  the target set
 * @data:  the data to be added to the set_t set
 * @return: Returns a bool indicating whether specified key/Object was added in set or not
 * 
 * Memory: Just iterates over the list and call setNode_new which allocates memory
 * Worst Case Time Complexity: O(n) where n is number of elements in the list
 */
bool set_add(set_t* set, void* data)
{	   	
	  setNode_t* elem = set->head;
	  pthread_mutex_lock(elem->mutex);
	  if (elem->next == NULL){
	    setNode_t *newElem = setNode_new(data,NULL);
	    elem->next = newElem;
	    pthread_mutex_unlock(elem->mutex);
	    return true;
	  }
	 
	  setNode_t* prev = elem;
	  while (elem->next != NULL && set->compare(elem->next->data,data) <= 0){
	
		if(set->compare(elem->next->data,data) == 0){
	      pthread_mutex_unlock(elem->mutex);
	      return false;
	    }
	    prev = elem;
	    elem = elem->next;
	    pthread_mutex_lock(elem->mutex);
	    pthread_mutex_unlock(prev->mutex);
	  }
	 if (elem->data == data){
	    pthread_mutex_unlock(elem->mutex);
	    return false;
	}
	setNode_t *newElem = setNode_new(data, elem->next);
	elem->next = newElem;
	pthread_mutex_unlock(elem->mutex);
	return 1;
}

/*
 * set_del: delete the data of the set 
 * @set:  the target set
 * @data:  the data to be delete to the set_t set
 * @return: Returns a bool indicating whether specified key/Object was delete from set or not
 * 
 * Memory: Just iterates over the list and free memory allocated by setNode
 * Worst Case Time Complexity: O(n) where n is number of elements in the list
 */

bool set_del(set_t* set, void* val)
{
  setNode_t* prev = set->head;
  pthread_mutex_lock(prev->mutex);
  if (prev->next == NULL){
    pthread_mutex_unlock(prev->mutex);
    return false;
  }

  setNode_t* elem = prev->next;
  pthread_mutex_lock(elem->mutex);

  while (elem->next != NULL && set->compare(elem->data,val) <= 0){
    if (set->compare(elem->data,val) == 0){
      prev->next = elem->next;
      pthread_mutex_unlock(elem->mutex);
      //free(elem->data); //User 
      pthread_mutex_destroy(elem->mutex);
      free(elem->mutex);
      free(elem);
      pthread_mutex_unlock(prev->mutex);
      return true;
    }

    pthread_mutex_unlock(prev->mutex);
    prev = elem;
    elem = elem->next;
    pthread_mutex_lock(elem->mutex);
  }
  if (set->compare(elem->data,val) == 0){
      prev->next = elem->next;
      pthread_mutex_unlock(elem->mutex);
      //free(elem->data);
      pthread_mutex_destroy(elem->mutex);
      free(elem->mutex);
      free(elem);
      pthread_mutex_unlock(prev->mutex);
      return true;
  }
  pthread_mutex_unlock(elem->mutex);
  pthread_mutex_unlock(prev->mutex);
  return false;
}
