#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "test_utilities.h"
#include "list_mtm1.h"

/**
 * Student will be the element of the generic list
 */
typedef struct student_t {
    char *name;
    int average_grade;
} *Student;

static Student createStudent(char *name, int average_grade)
{
    assert(name != NULL);
    Student student = malloc(sizeof(*student));
    if (student == NULL) {
        return NULL;
    }
    student->name = malloc(strlen(name)+1);
    strcpy(student->name, name);
    student->average_grade = average_grade;
    return student;
}

static ListElement copyStudent(ListElement student)
{
    assert(student);
    Student ptr = student;
    return createStudent(ptr->name, ptr->average_grade);
}

static void freeStudent(ListElement student)
{
    assert(student);
    Student ptr = student;
    free(ptr->name);
    free(ptr);
}

static int compareGrade(ListElement st1, ListElement st2)
{
    assert(st1);
    assert(st2);
    Student ptr1 = st1, ptr2 = st2;
    return ptr1->average_grade - ptr2->average_grade;
}

static int compareName(ListElement st1, ListElement st2)
{
    assert(st1);
    assert(st2);
    Student ptr1 = st1, ptr2 = st2;
    return strcmp(ptr1->name, ptr2->name);
}

static bool filterByGrade(ListElement student, ListFilterKey grade)
{
    assert(student);
    if (grade == NULL) {
        return false;
    }
    Student ptr = student;
    return ptr->average_grade >= *(int*)grade;
}

static bool testListCreate()
{
    ASSERT_TEST(listCreate(NULL, NULL) == NULL);
    ASSERT_TEST(listCreate(copyStudent, NULL) == NULL);
    ASSERT_TEST(listCreate(NULL, freeStudent) == NULL);
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(list != NULL);
    listDestroy(list);
    return true;
}

static bool testListCopy()
{
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    Student st2 = createStudent("John", 70);
    assert(st2);
    Student st3 = createStudent("Ron", 95);
    assert(st3);
    ASSERT_TEST(listInsertLast(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st3) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    ASSERT_TEST(listCopy(NULL) == NULL);
    List copy = listCopy(list);
    ASSERT_TEST(copy != NULL);
    ASSERT_TEST(listGetSize(copy) == listGetSize(list));

    /* The current element of the copied list should be the same as the current
       element of the original list. In this case it's the first one.
       This is checked through comparison of the students' grades, which are
       unique in this case      */
    Student ptr = listGetCurrent(copy);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);

    ptr = listGetNext(copy);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    ptr = listGetNext(copy);
    ASSERT_TEST(ptr->average_grade == st3->average_grade);
    listDestroy(list);
    listDestroy(copy);
    freeStudent(st1);
    freeStudent(st2);
    freeStudent(st3);
    return true;
}

static bool testListGetSize()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetSize(list) == 0);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    Student st2 = createStudent("John", 70);
    assert(st2);
    Student st3 = createStudent("Ron", 95);
    assert(st3);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st3) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    freeStudent(st3);
    return true;
}

static bool testListGetFirst()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetFirst(list) == NULL);
    Student student = createStudent("Dan", 85);
    ASSERT_TEST(listInsertFirst(list, student) == LIST_SUCCESS);
    Student ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == student->average_grade);
    listDestroy(list);
    freeStudent(student);
    return true;
}

static bool testListGetNext()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetNext(list) == NULL);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listGetNext(list) == NULL);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    Student ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListGetCurrent()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    Student ptr = listGetCurrent(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    ptr = listGetCurrent(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListInsertFirst()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listInsertFirst(NULL, NULL) == LIST_NULL_ARGUMENT);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    Student ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListInsertLast()
{
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listInsertLast(NULL, NULL) == LIST_NULL_ARGUMENT);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertLast(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    Student ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListInsertBeforeCurrent()
{
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    ASSERT_TEST(listInsertBeforeCurrent(list, st1) == LIST_INVALID_CURRENT);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertBeforeCurrent(list, st2) == LIST_SUCCESS);
    Student ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListInsertAfterCurrent()
{
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    ASSERT_TEST(listInsertAfterCurrent(list, st1) == LIST_INVALID_CURRENT);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertAfterCurrent(list, st2) == LIST_SUCCESS);
    Student ptr = listGetCurrent(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListRemoveCurrent()
{
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) != NULL);
    Student st2 = createStudent("John", 70);
    assert(st2);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    Student ptr = listGetCurrent(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetCurrent(list) == NULL);
    ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    return true;
}

static bool testListSort()
{
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    Student st2 = createStudent("John", 70);
    Student st3 = createStudent("Ron", 95);
    Student st4 = createStudent("Joseph", 80);
    Student st5 = createStudent("Aaron", 90);
    ASSERT_TEST(listInsertLast(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st5) == LIST_SUCCESS);

    /**
     * Sorting by Grade (from lowest to highest):
     *  1. st2: 70
     *  2. st4: 80
     *  3. st1: 85
     *  4. st5: 90
     *  5. st3: 95
     */
    ASSERT_TEST(listSort(list, compareGrade) == LIST_SUCCESS);
    Student ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st4->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st5->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st3->average_grade);

    /**
     * Sorting by Name (in lexicographic order):
     *  1. st5: Aaron
     *  2. st1: Dan
     *  3. st2: John
     *  4. st4: Joseph
     *  5. st3: Ron
     */
    ASSERT_TEST(listSort(list, compareName) == LIST_SUCCESS);
    ptr = listGetFirst(list);
    ASSERT_TEST(ptr->average_grade == st5->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st2->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st4->average_grade);
    ptr = listGetNext(list);
    ASSERT_TEST(ptr->average_grade == st3->average_grade);

    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    freeStudent(st3);
    freeStudent(st4);
    freeStudent(st5);
    return true;
}

static bool testListFilter()
{
    int filter_key = 85;
    ASSERT_TEST(listFilter(NULL, filterByGrade, &filter_key) == NULL);
    List list = listCreate(copyStudent, freeStudent);
    Student st1 = createStudent("Dan", 85);
    Student st2 = createStudent("John", 70);
    Student st3 = createStudent("Ron", 95);
    Student st4 = createStudent("Joseph", 80);
    Student st5 = createStudent("Aaron", 90);
    ASSERT_TEST(listInsertLast(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st4) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, st5) == LIST_SUCCESS);
    ASSERT_TEST(listFilter(list, NULL, &filter_key) == NULL);
    List filtered_empty = listFilter(list, filterByGrade, NULL);
    ASSERT_TEST(filtered_empty != NULL);
    ASSERT_TEST(listGetFirst(filtered_empty) == NULL);

    /**
     * Filtering by grade:
     * The filter key (the minimum grade for the students to be included in the
     * filtered list) is 85.
     * These students will be included (in this exact order):
     *  1. st1: 85
     *  2. st3: 95
     *  3. st5: 90
     */
    List filtered_list = listFilter(list, filterByGrade, &filter_key);
    ASSERT_TEST(filtered_list != NULL);

    /*The internal iterator of the filtered list should point at the
      first element. This is checked through comparing the grades, which are
      unique in this case       */
    Student ptr = listGetCurrent(filtered_list);
    ASSERT_TEST(ptr->average_grade == st1->average_grade);

    ptr = listGetNext(filtered_list);
    ASSERT_TEST(ptr->average_grade == st3->average_grade);
    ptr = listGetNext(filtered_list);
    ASSERT_TEST(ptr->average_grade == st5->average_grade);
    listDestroy(list);
    listDestroy(filtered_list);
    listDestroy(filtered_empty);
    freeStudent(st1);
    freeStudent(st2);
    freeStudent(st3);
    freeStudent(st4);
    freeStudent(st5);
    return true;
}

static bool testListClear()
{
    ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
    List list = listCreate(copyStudent, freeStudent);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    Student st1 = createStudent("Dan", 85);
    assert(st1);
    Student st2 = createStudent("John", 70);
    assert(st2);
    Student st3 = createStudent("Ron", 95);
    assert(st3);
    ASSERT_TEST(listInsertFirst(list, st1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, st3) == LIST_SUCCESS);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) == NULL);
    listDestroy(list);
    freeStudent(st1);
    freeStudent(st2);
    freeStudent(st3);
    return true;
}


int main()
{
    RUN_TEST(testListCreate);
    RUN_TEST(testListFilter);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListGetCurrent);
    RUN_TEST(testListInsertFirst);
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListSort);
    RUN_TEST(testListClear);
    return 0;
}