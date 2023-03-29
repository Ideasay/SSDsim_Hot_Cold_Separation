#include <stdio.h>
#include <stdlib.h>

typedef struct HotList
{
    unsigned int lpn;
    struct HotList *next; 
    struct HotList *prev;
}HotList;

typedef struct CandidateList
{
    unsigned int lpn;
    struct CandidateList *next; 
    struct CandidateList *prev; 
}CandidateList;

HotList *create_HotList(unsigned int lpn);
HotList *insert_HotList(HotList *head, unsigned int lpn);
void delete_HotList(HotList **head, unsigned int lpn);
HotList *Search_HotList(HotList* list,unsigned int lpn);
void print_HotList(HotList* list);

CandidateList *create_CandidateList(unsigned int lpn);
CandidateList *insert_CandidateList(CandidateList *head, unsigned int lpn);
void delete_CandidateList(CandidateList **head, unsigned int lpn);
CandidateList *Search_CandidateList(CandidateList* list,unsigned int lpn);
void print_CandidateList(CandidateList* list);

unsigned int Hotness_LRU(unsigned int lpn);
void Write_LRU(unsigned int lpn);
