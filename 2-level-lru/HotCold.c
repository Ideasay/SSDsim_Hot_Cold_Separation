#include"HotCold.h"


#define HotList_Size 64
#define CandidateList_Size 64

HotList *Hot = NULL;
CandidateList *Candidate = NULL;

unsigned int Hotness_LRU(unsigned int lpn){
    if(Search_HotList(Hot, lpn)){
      //  printf("Hot\n");
        return 1;
    }
    //printf("Cold\n");
    return 0;
}

void Write_LRU(unsigned int lpn){

    static int HotList_length = 0, CandidateList_length = 0;

    if(Search_HotList(Hot,lpn) != NULL){
        
        delete_HotList(&Hot, lpn);
        Hot = insert_HotList(Hot, lpn);
        #ifdef DEBUG
            print_HotList(Hot);
        #endif 
    }
    else if(Search_CandidateList(Candidate,lpn) != NULL){
        
        if(HotList_length >= HotList_Size){
            HotList *temp = Hot;
            while(temp -> next) temp = temp -> next; //move to last element of HotList
            Candidate = insert_CandidateList(Candidate, temp -> lpn);
            CandidateList_length++;
            delete_HotList(&Hot, temp -> lpn);
            HotList_length--;
            //free(temp);
        }
        delete_CandidateList(&Candidate, lpn);
        CandidateList_length--;
        Hot = insert_HotList(Hot, lpn);
        HotList_length++;

        #ifdef DEBUG
            print_HotList(Hot);
            print_CandidateList(Candidate);
        #endif 

    }
    else{
        if(CandidateList_length >= CandidateList_Size){
            CandidateList *temp = Candidate;
            while(temp -> next) temp = temp -> next; //move to last element of CandidateList
            delete_CandidateList(&Candidate, temp -> lpn);
            //free(temp);
            CandidateList_length--;
        }
        Candidate = insert_CandidateList(Candidate, lpn);
        CandidateList_length++;

        #ifdef DEBUG
            print_CandidateList(Candidate);
        #endif 
    }
}

HotList *create_HotList(unsigned int lpn){
    HotList *temp;
    temp = (HotList*) malloc(sizeof(HotList));
    temp -> lpn = lpn;
    temp -> next = NULL;
    temp -> prev = NULL;
    //printf("Create HotList %d\n",temp->lpn);
	return temp;
}

HotList *insert_HotList(HotList *head, unsigned int lpn){

    HotList *temp = create_HotList(lpn);

    /* empty list */
    if(head == NULL){
        free(head);
        return temp;
    }

    /* back to first element */
    while(head -> prev != NULL) head = head -> prev;


    /* insert lpn to the front of list */
    temp -> next = head;
    head -> prev = temp;

    return temp;
}

void delete_HotList(HotList **head, unsigned int lpn){

    HotList *target = *head;

    while(target){
        if(target -> lpn == lpn) break;
        target = target -> next;
    }

    if(target -> prev == NULL && target -> next == NULL){
        *head = NULL;
    }
    else if(target -> prev == NULL){
        target -> next -> prev = NULL;
        *head = target -> next;
    }
    else if(target -> next == NULL){
        target -> prev -> next = NULL;
    }
    else{
        target -> prev -> next = target -> next;
        target -> next -> prev = target -> prev;
    }

    free(target);
}

HotList *Search_HotList(HotList* list,unsigned int lpn){

    while(list){
        if(list -> lpn == lpn){
            //printf("Found\n");
            return list;
        }
        list = list -> next;
    }
    //printf("No Found\n");
    return NULL;
}

void print_HotList(HotList* list){

    if(list == NULL) printf("HotList Empty\n");
    else{
        printf("HotList : ");
        while(list){
            printf("%d ",list -> lpn);
            list = list -> next;
        }
        printf("\n");
    }
}

CandidateList *create_CandidateList(unsigned int lpn){
    CandidateList *temp;
    temp = (CandidateList*) malloc(sizeof(CandidateList));
    temp -> lpn = lpn;
    temp -> next = NULL;
    temp -> prev = NULL;
    //printf("Create CandidateList %d\n",temp->lpn);
	return temp;
}

CandidateList *insert_CandidateList(CandidateList *head, unsigned int lpn){

    CandidateList *temp = create_CandidateList(lpn);

    if(head == NULL){
        free(head);
        return temp;
    }

    temp -> next = head;
    head -> prev = temp;

    return temp;
}

void delete_CandidateList(CandidateList **head, unsigned int lpn){

    CandidateList *target = *head;

    while(target){
        if(target -> lpn == lpn) break;
        target = target -> next;
    }

    if(target -> prev == NULL && target -> next == NULL){
        *head = NULL;
    }
    else if(target -> prev == NULL){
        target -> next -> prev = NULL;
        *head = target -> next;
    }
    else if(target -> next == NULL){
        target -> prev -> next = NULL;
    }
    else{
        target -> prev -> next = target -> next;
        target -> next -> prev = target -> prev;
    }

    free(target);
}

CandidateList *Search_CandidateList(CandidateList* list,unsigned int lpn){
    while(list){
        if(list -> lpn == lpn){
            //printf("Found\n");
            return list;
        }
        list = list -> next;
    }
    //printf("No Found\n");
    return NULL;
}

void print_CandidateList(CandidateList* list){

    if(list == NULL) printf("CandidateList Empty\n");
    else{
        printf("CandidateList : ");
        while(list){
            printf("%d ",list -> lpn);
            list = list -> next;
        }
        printf("\n");
    }
}


