#include "escapetechnion.h"
#include "mtm_ex3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_t* Node;
struct node_t {
    int n;
    Node next;
};

Node copyNode(Node node)
{
    Node new_node = malloc(sizeof(struct node_t));
    if(new_node == NULL)
    {
        return NULL;
    }
    new_node->n = node->n;
    if(node->next != NULL)
    {
        new_node->next = copyNode(node->next);
    }
    return new_node;
}

Node mergaeNodes(Node head1, Node head2)
{
    Node node = head1->next;
    while(node != NULL)
    {
        node = node->next;
    }
    node->next = head2;
    return head1;
}

Node concatLists(Node head1, Node head2, bool *(check)(int))
{
    Node copy1 = copyNode(head1);
    Node copy2 = copyNode(head2);
    if(copy1 == NULL || copy2 == NULL)
    {
        return NULL;
    }
    copy1 = mergaeNodes(copy2);
    Node node = copy1, temp_node;
    while(node != NULL)
    {
        if(!check)
        {
            temp_node = node->next;
            if(temp_node == NULL)
            {
                node = NULL;
                return copy1;
            }
            node->next = node->next->next;
            temp_node->next - NULL;
            //destroyList(temp_node);
        }
    }

}


int comp(void* a, void* b)
{
    int a1 = *(int*)a;
    int b1 = *(int*)b;
    return a1 - b1;
}
/**
 * Receives an object, its size, a sorted array, its size and a comparison
 * function, and returns the location of the object in the array.
 * @param item: The item we're trying to find in the array. Must be void*.
 * @param size: Its size. Needed to go through the array. Must be size_t.
 * @param arr: The sorted array we're searching through. Must be void*.
 * @param len: The length of the array. Most be size_t.
 * @param compare: A comparison function that receives two objects, and returns
 * the following:
 * A positive value if the object a is bigger than object b.
 * Zero is they're equal.
 * A negative value if object b is bigger than object a.
 * @return
 * -1 if the item isn't in the array.
 * The item's location in the array otherwise.
 */
int binaryFind(void *item, size_t size, void *arr, size_t len, int (*compare)(void *, void *))
{
    int left = 0;
    int right = len - 1;
    int middle, comparison;
    char* array = arr;
    while (left <= right) {
        middle = (left + right)/2;
        comparison = compare(array + middle * size, item);
        if (comparison > 0)
            right = middle - 1;
        else if (comparison < 0)
            left = middle + 1;
        else
            return middle;
    }
    return -1;
}
int main(){
    int arr[7] = {1, 3, 5, 7, 9, 11, 13};
    int search = 3;
    printf("%d\n", binaryFind(&search, sizeof(int), arr, 7, comp));
    search = 2;
    printf("%d\n", binaryFind(&search, sizeof(int), arr, 7, comp));
    search = 13;
    printf("%d\n", binaryFind(&search, sizeof(int), arr, 7, comp));
    /*
    EscapeTechnion escape = create_escapetechnion();
    MtmErrorCode code = escapetechnion_set_output_channel(escape, stdout);
    code = escapetechnion_add_company(escape, "company1@gmail", 0);
    code =escapetechnion_add_company(escape, "company2@gmail", 2);
    code =escapetechnion_add_company(escape, "company3@gmail", 0);
    code =escapetechnion_add_company(escape, "company4@gmail", 8);
    code =escapetechnion_add_company(escape, "company5@gmail", 16);
    code =    escapetechnion_add_company(escape, "company6@gmail", 6);
    code =    escapetechnion_add_company(escape, "company7@gmail", 1);
    code =    escapetechnion_add_room(escape, "company1@gmail", 2, 80, 2, "05-15", 4);
    code =    escapetechnion_add_room(escape, "company2@gmail", 1, 4, 3, "08-19", 1);
    code =    escapetechnion_add_room(escape, "company2@gmail", 4, 20, 4, "02-22", 5);
    code =    escapetechnion_add_room(escape, "company3@gmail", 5, 24, 4, "02-22", 7);
    code =    escapetechnion_add_room(escape, "company4@gmail", 2, 24, 4, "06-18", 8);
    code =    escapetechnion_add_room(escape, "company5@gmail", 8, 60, 2, "08-09", 2);
    code =    escapetechnion_add_room(escape, "company6@gmail", 11, 120, 7, "18-24", 9);
    code =    escapetechnion_add_room(escape, "company6@gmail", 14, 200, 2, "15-23", 1);
    code =     escapetechnion_add_room(escape, "company7@gmail", 12, 12, 2, "12-22", 6);
    code =    escapetechnion_add_customer(escape, "1@", 0, 1);
    code =   escapetechnion_add_customer(escape, "2@", 1, 4);
    code =    escapetechnion_add_customer(escape, "3@", 0, 1);
    code =   escapetechnion_add_customer(escape, "4@", 1, 5);
    code =   escapetechnion_add_customer(escape, "5@", 8, 1);
    code =      escapetechnion_add_customer(escape, "6@", 16, 7);
    code =      escapetechnion_create_order(escape, "1@", 0, 2, "0-10", 4);
    code =     escapetechnion_create_order(escape, "2@", 0, 2, "0-5", 4);
    code =     escapetechnion_create_order(escape, "1@", 0, 2, "1-10", 5);
    code =    escapetechnion_create_order(escape, "3@", 2, 4, "0-02", 4);
    code =    escapetechnion_create_order(escape, "3@", 2, 4, "0-03", 4);
    code =    escapetechnion_create_order(escape, "5@", 16, 8, "0-08", 5);
    code =     escapetechnion_create_order(escape, "6@", 6, 11, "1-23", 5);
    code =     escapetechnion_create_order(escape, "4@", 6, 11, "2-23", 5);
    code =    escapetechnion_remove_room(escape, 0, 2);
    code =    escapetechnion_reportday(escape);
    code = escapetechnion_remove_company(escape, "company5@gmail");
    code = escapetechnion_create_order(escape, "2@", 8, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "1@", 0, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "3@", 0, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "1@", 1, 12, "0-11", 5);
    code = escapetechnion_create_order(escape, "1@", 16, 8, "0-11", 5);
    code = escapetechnion_remove_customer(escape, "4@");
    code = escapetechnion_reportday(escape);
    code = escapetechnion_remove_customer(escape, "4@");
    escapetechnion_reportbest(escape);
    code = escapetechnion_reportday(escape);
    code = escapetechnion_recommended_room(escape, "3@", 6);
    code = escapetechnion_reportday(escape);

    return code;*/


}