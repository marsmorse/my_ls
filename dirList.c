#include "./dirList.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*


    type numbers correspond to inode(7) file types
*/
typedef struct dirNodeObj
{
    long int seconds;
    long int nanosec;
    char name[255]; 
    dirNode next;
    dirNode prev;
} dirNodeObj;

typedef struct dirListObj
{
    u_int8_t isSortedByTime;
    int length;
    int nondir_count;
    int dir_count;
    dirNode head;
    dirNode tail;
} dirListObj;

void printList(dirList L)
{
    dirNode it = L->head;
    while (it != NULL)
    {
        write(1, it->name, strlen(it->name));
        write(1, "  ", 2);
        it = it->next;
    }
}

//returns 1 if node 1 is less than node 2
int cmpNodesByName(dirNode node1, dirNode node2)
{
    if (strcmp(node1->name, node2->name) < 0)
            return 1;
    return 0;
}
int cmpNodesByTime(dirNode node1, dirNode node2)
{
    if (node1->seconds > node2->seconds)
    {
        return 1;
    }
    else if (node1->seconds == node2->seconds)
    {
        if (node1->nanosec > node2->nanosec)
            return 1;
        else if (node1->nanosec == node2->nanosec)
        {
            if (strcmp(node1->name, node2->name) < 0)
                return 1;
        }
    }
    return 0;
}

//appends new_node after node
void insertNode(dirNode node, dirNode new_node)
{
    new_node->next = node->next; 
    node->next = new_node;
}

void swapNodes(dirNode node1, dirNode node2)
{
    dirNodeObj temp;
    temp.seconds = node1->seconds;
    temp.nanosec = node1->nanosec;
    strcpy(temp.name, node1->name);
    node1->seconds = node2->seconds;
    node1->nanosec = node2->nanosec;
    strcpy(node1->name, node2->name);
    node2->seconds = temp.seconds;
    node2->nanosec = temp.nanosec;
    strcpy(node2->name, temp.name);
}

void insertionSort(dirList L)
{
    dirNode sorted_it;
    if (L->length <= 1)
        return;
    dirNode unsorted_it = L->head->next;
    while (unsorted_it != NULL)
    {
        sorted_it = unsorted_it;
        while (sorted_it->prev != NULL && (L->isSortedByTime ? cmpNodesByTime(sorted_it, sorted_it->prev) : cmpNodesByName(sorted_it, sorted_it->prev)))
        {
            swapNodes(sorted_it, sorted_it->prev);
            sorted_it = sorted_it->prev;
        }
        unsorted_it = unsorted_it->next;
    }
    sorted_it = NULL;
    unsorted_it = NULL;
}

int getDirCount(dirList self)
{
    return self->dir_count;
}

int getNonDirCount(dirList self)
{
    return self->nondir_count;
}

char *getDirObjName(dirList self, int index)
{
    dirNode it = self->head;
    if (index >= self->length)
        return "ERROR";
    for (int i = 0; i < index; i++)
    {
        it = it->next;
    }
    return it->name;
}

int getLength(dirList self)
{
    return self->length;
}

void addDir(dirList self, long int sec, long int nanosec, char *name)
{
    dirNode node = newdirNode(sec, nanosec, name);
    if (self->length == 0)
    {
        //node is first node of the list
        self->head = node;
        self->tail = node;
    } 
    else
    {
        node->prev = self->tail;
        self->tail->next = node;
        self->tail = node;
    }
    self->length++;
}

dirNode newdirNode(long int s, long int ns, char *n)
{
    dirNode dn = malloc(sizeof(dirNodeObj));
    dn->seconds = s;
    dn->nanosec = ns;
    dn->next = NULL;
    dn->prev = NULL;
    strcpy(dn->name, n);
    return dn;
}

dirList newDirList()
{
    dirList dr = malloc(sizeof(dirListObj));
    dr->isSortedByTime = 0;
    dr->head = NULL;
    dr->length = 0;
    dr->dir_count = 0;
    dr->nondir_count = 0;
    return dr;
}

void setTFlag(dirList self, u_int8_t flag)
{
    self->isSortedByTime = flag;
}

void deleteDirList(dirList self)
{
    dirNode it = self->head;
    self->head = NULL;
    self->tail = NULL;
    dirNode n = it; 
    while (it != NULL) {
        n = it;
        it = it->next;
        n->next = NULL;
        n->prev = NULL;
        free(n);
    }
    free(self);
    self = NULL;
}