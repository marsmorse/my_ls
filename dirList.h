#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct dirNodeObj *dirNode;
typedef struct dirListObj *dirList;

void addDir(dirList self, long int sec, long int nanosec, char *name);
dirList newDirList();
dirNode newdirNode(long int s, long int ns, char *n);
void insertNode(dirNode node, dirNode new_node);
//returns 1 if node 1 is less than node 2
int cmpNodesByTime(dirNode node1, dirNode node2);
int cmpNodesByName(dirNode node1, dirNode node2);
int getDirCount(dirList self);
char *getDirObjName(dirList self, int index);
int getLength(dirList self);

void updateFileTypeCount(dirList self);

void deleteDirList(dirList self);
void printList(dirList L);
void insertionSort(dirList L);
void setTFlag(dirList self, u_int8_t flag);
