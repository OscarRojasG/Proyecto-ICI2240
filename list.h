#ifndef List_h
#define List_h

typedef struct List List;

typedef List Stack;

// Operaciones TDA Lista

List * createList(void);

List * copyList(List *);

void * firstList(List * list);

void * nextList(List * list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list, void * data);

void pushBack(List * list, void * data);

void pushCurrent(List * list, void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void cleanList(List * list);

int getSize(List * list);


// Operaciones TDA Pila

Stack* createStack();

void pop(Stack* s);

void* top(Stack* s);

void push(Stack* s, void* data);


#endif