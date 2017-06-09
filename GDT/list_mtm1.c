#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list_mtm1.h"

/**
* The List stores its elements in a node structure, where each node contains
* the element itself (of ListElement type), and a pointer to the next node.
* By default, when the list is empty of elements, it contains a single empty
* node, which is used as the header of the Node.
*/
typedef struct node_t {
    ListElement element;
    struct node_t* next;
} *Node;

/**
* The List Container, implemented as struct and contains the following fields:
*
*  node - A pointer to the node containing the elements of the list
*  size - Contains the number of elements in the list
*  current_element - The internal iterator of the list; used as a pointer to
*                    a node of the list, in order to iterate over the
*                    elements of the list
*  copy_function - A pointer to the function of copying an element of the list
*                  provided upon creation of the list
*  free_function - A pointer to the function of deallocating an element of the
*                  list provided upon creation of the list
*/
struct list_t {
    Node node;
    int size;
    Node current_element;
    CopyListElement copy_function;
    FreeListElement free_function;
};

/**
 * Static function of the list
 * Creates a new node and initializes it with a copy of the received element,
 * and then returns this node.
 *
 * @param list - The list which the node is created for
 * @param element - the element which is intended to be copied to the new node
 * @return
 * NULL if memory allocation problem occurred
 * The new node with a copy of the element
 */
static Node createNewElementNode(List list, ListElement element)
{
    assert(list);
    ListElement copy = list->copy_function(element);
    if (copy == NULL) {
        return NULL;
    }
    Node node = malloc(sizeof(*node));
    if (node == NULL) {
        free(copy);
        return NULL;
    }
    node->element = copy;
    return node;
}

/**
 * Static function of the list
 * Finds the node preceding to the one the internal iterator points at
 *
 * @param list - The list on which the function operates
 * @return
 * The preceding node, as described above
 */
static Node getPrevNode(List list)
{
    assert(list != NULL);
    Node prev_node = list->node;
    while (prev_node->next != list->current_element) {
        prev_node = prev_node->next;
    }
    return prev_node;
}

/**
 * Static function of the list
 * Calculates the position of the node which the internal iterator points at,
 * as a serial number relative to the header node.
 * @example
 * If the iterator points at the third node from the header, the function will
 * return 3
 *
 * @param list - The list on which the function operates
 * @return
 * The position of the node, as described above
 */
static int calculateCurrent(List list)
{
    Node ptr = list->node->next;
    int place = 1;
    while (ptr != list->current_element) {
        ptr = ptr->next;
        place++;
    }
    return place;
}

/**
 * Static function of the list
 * Restores the internal iterator to point at the node in the same place
 * relatively to the header node, while the place is passed as a parameter.
 * @example
 * If the iterator previously pointed at the second node from the header node,
 * After sorting the list it will be pointing at the same node, whose place is
 * not necessarily the same. Therefore, the function will make the iterator
 * point back at the second node.
 *
 * @param list - The list on which the function operates
 * @param current_place - The position of the node the iterator should point at
 */
static void restoreCurrent(List list, int current_place)
{
    list->current_element = list->node->next;
    for (int i = 1; i < current_place; i++) {
        list->current_element = list->current_element->next;
    }
}

/**
 * Static function of the list
 * Creates a filtered or unfiltered copy of the list:
 * If a filtering function is provided, then the copy will be filtered according
 * to it. Otherwise, a full copy will will be created.
 * NOTE: Non-provided filtering function and key are defined as NULL
 * NOTE: The state of the internal iterator is undefined after this function
 *
 * @param list - The list on which the function operates
 * @param filterElement - The provided filtering function
 * @param key - Additional parameter used in the filtering function
 * @return
 * NULL if memory allocation problem occurred
 * Copy of the list (full or filtered, depending on the parameters)
 */
static List listCopyByFilter(List list, FilterListElement filterElement,
                             ListFilterKey key)
{
    if (list == NULL) {
        return NULL;
    }
    List new_list = listCreate(list->copy_function, list->free_function);
    LIST_FOREACH(ListElement, element, list) {

        /*The element should be copied if no filtering function passed or
          if it passed the filtering    */
        if ( filterElement == NULL || filterElement(element, key)) {

            //Insertion of the elements conserving the order in the original
            if (listInsertLast(new_list, element) != LIST_SUCCESS) {
                listDestroy(new_list);
                return NULL;
            }
        }
    }
    return new_list;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement)
{
    if (copyElement == NULL || freeElement == NULL) {
        return NULL;
    }
    List list = malloc(sizeof(*list));
    if (list == NULL) {
        return NULL;
    }
    list->node = malloc(sizeof(struct node_t));
    if (list->node == NULL) {
        return NULL;
    }
    list->node->element = NULL;
    list->node->next = NULL;
    list->copy_function = copyElement;
    list->free_function = freeElement;
    list->current_element = list->node->next;
    list->size = 0;
    return list;
}

List listCopy(List list)
{
    if (list == NULL) {
        return NULL;
    }

    //Store the place of the iterator
    int current_place = calculateCurrent(list);

    List new_list = listCopyByFilter(list, NULL, NULL);
    if (new_list == NULL) {
        return NULL;
    }
    /*Restore the position of the iterator in the original list, and set it
      the same in the new list     */
    restoreCurrent(list, current_place);
    restoreCurrent(new_list, current_place);

    return new_list;
}

int listGetSize(List list)
{
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

ListElement listGetFirst(List list)
{
    if (list == NULL) {
        return NULL;
    }
    if (listGetSize(list) == 0) {
        return NULL;
    }
    /*The first node is the header node, so the first element is located in
      the next node to the header   */
    list->current_element = list->node->next;

    return list->node->next->element;
}

ListElement listGetNext(List list)
{
    if (list == NULL) {
        return NULL;
    }
    if (list->current_element == NULL) {
        return NULL;
    }
    list->current_element = list->current_element->next;
    if (list->current_element == NULL) {
        return NULL;
    }
    return list->current_element->element;
}

ListElement listGetCurrent(List list)
{
    if (list == NULL) {
        return NULL;
    }
    //Return NULL in case the current element is NULL
    return list->current_element==NULL ? NULL : list->current_element->element;
}

ListResult listInsertFirst(List list, ListElement element)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //Create a new node for the new element
    Node new_node = createNewElementNode(list, element);

    if (new_node == NULL)
        return LIST_OUT_OF_MEMORY;
    /* "Push" the new node between the header node and the next node
       (which may be NULL as well)      */
    new_node->next = list->node->next;
    list->node->next = new_node;

    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node new_node = createNewElementNode(list, element);
    if (new_node == NULL)
        return LIST_OUT_OF_MEMORY;

    //Find the last node
    Node this_node = list->node;
    while(this_node->next != NULL) {
        this_node = this_node->next;
    }
    /* Set the new node as last, and make it point to NULL (As required from
       the last node       */
    this_node->next = new_node;
    new_node->next = NULL;

    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->current_element == NULL) {
        return LIST_INVALID_CURRENT;
    }
    Node new_node = createNewElementNode(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    //Find the previous node, and "push" the new one between it and the current
    Node prev_node = getPrevNode(list);
    prev_node->next = new_node;
    new_node->next = list->current_element;

    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->current_element == NULL) {
        return LIST_INVALID_CURRENT;
    }
    Node new_node = createNewElementNode(list, element);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    //"Push" the new node between the current and the next one
    new_node->next = list->current_element->next;
    list->current_element->next = new_node;

    list->size++;
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (list->current_element == NULL) {
        return LIST_INVALID_CURRENT;
    }
    //Link the previous node with the next node
    Node prev_node = getPrevNode(list);
    prev_node->next = list->current_element->next;

    list->free_function(list->current_element->element);
    list->current_element->next = NULL;
    free(list->current_element);
    list->current_element = NULL;
    list->size--;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement)
{
    /**
     * This function sorts the list using Bubble Sort algorithm
     */
    if (list == NULL || compareElement == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //Save the position of the node the iterator currently points at
    int current_place = calculateCurrent(list);

    int size_to_sort = list->size;
    while (size_to_sort > 1) {
        Node ptr = list->node->next, pre_ptr = list->node;
        for (int i = 1; i < size_to_sort; i++) {
            /*if the provided comparison function returns a value greater
              than 0, then the element in the first parameter should
              advance further. Therefore it's switched with the next one.    */
            if (compareElement(ptr->element, ptr->next->element) > 0) {
                pre_ptr->next = ptr->next;
                ptr->next = ptr->next->next;
                pre_ptr->next->next = ptr;
            }
            //Otherwise, keep looking for a smaller element to be switched with
            else {
                ptr = ptr->next;
            }
            pre_ptr = pre_ptr->next;
        }
        size_to_sort--;
    }
    restoreCurrent(list, current_place);
    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key)
{
    if (list == NULL || filterElement == NULL) {
        return NULL;
    }
    //Get a filtered copy from the following function
    List new_list =  listCopyByFilter(list, filterElement, key);

    if (new_list == NULL) {
        return NULL;
    }
    /* Set the iterator to point at the first node after the header node
       (may be NULL as well)        */
    new_list->current_element = new_list->node->next;

    return new_list;
}

ListResult listClear(List list)
{
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    /* Remove each time the first node (after the header), until it's NULL
       (Each time the first node is removed, the second one will become the
       first, and will be removed as the first next time as well)        */
    for (list->current_element = list->node->next; list->current_element;
         list->current_element = list->node->next) {
        listRemoveCurrent(list);
    }
    return LIST_SUCCESS;
}

void listDestroy(List list)
{
    if (listClear(list) == LIST_NULL_ARGUMENT) {
        return;
    }
    free(list->node);
    list->copy_function = NULL;
    list->free_function = NULL;
    free(list);
}