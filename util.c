#include "util.h"

List * getRandomElements(List * list, int n) {
    if(n >= getSize(list))
        return list;

    List *new = createList();
    int max_rand = getSize(list) / n;
    if((getSize(list) % n) == 0)
        max_rand -= 1;

    int *addedNodes = (int *) calloc(getSize(list), sizeof(int));

    while(getSize(new) < n) {
        void *data = firstList(list);
        int pos = 0;

        while(data && (getSize(new) < n)) {
            int r = rand() % max_rand;
            if(r == 0 && addedNodes[pos] == 0)
            {
                pushBack(new, data);
                addedNodes[pos] = 1;
            }
            data = nextList(list);
            pos++;
        }
    }

    free(addedNodes);
    return new;
}