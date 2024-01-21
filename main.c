#include "linear_sequence.h"
#include <stdio.h>
LSQ_HandleT arr;
LSQ_IteratorT itr;

int main()
{
    itr = NULL;
    arr = LSQ_CreateSequence();
    for (int i = 0; i < 100; i++)
        LSQ_InsertFrontElement(arr, i*228/100);
    itr = LSQ_GetFrontElement(arr);
    for (int i = 0; i < 100; i++)
        if (*LSQ_DereferenceIterator(itr) == 100)
            printf( "u found 100!!");
        else
            LSQ_AdvanceOneElement(itr);
}