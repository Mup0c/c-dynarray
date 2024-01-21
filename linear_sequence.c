#include <stdlib.h>
#include <mem.h>
#include "linear_sequence.h"

typedef struct {
    LSQ_BaseTypeT *LSQ_Elements;
    int RealSize, MemSize;
} LSQ_Type;

typedef struct {
    LSQ_Type *seq;
    LSQ_IntegerIndexT index;
} LSQ_TIterator;

LSQ_HandleT LSQ_CreateSequence(void) {
    LSQ_Type *s = (LSQ_Type*) malloc(sizeof(LSQ_Type)); //typecast
    s->MemSize = 1;
    s->RealSize = 0;
    s->LSQ_Elements = (LSQ_BaseTypeT*)malloc(sizeof(LSQ_BaseTypeT) * s->MemSize);
    return s;
}

void LSQ_DestroySequence(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid) return;
    LSQ_Type *a = (LSQ_Type*)handle;
    free(a->LSQ_Elements);
    free(a);
}

LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
    return handle == LSQ_HandleInvalid ? -1 : ((LSQ_Type*)handle)->RealSize;
}

int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    return ((!LSQ_IsIteratorPastRear(iterator)) && (!LSQ_IsIteratorBeforeFirst(iterator)) && (iterator != NULL));
}

int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    return((iterator != LSQ_HandleInvalid) && (((LSQ_TIterator*)iterator)->index >= ((LSQ_TIterator*)iterator)->seq->RealSize));
}

int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    return(iterator != LSQ_HandleInvalid && ((LSQ_TIterator*)iterator)->index < 0);
}

LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    return iterator == LSQ_HandleInvalid ? NULL :
           ((LSQ_TIterator*)iterator)->seq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index;//razimenovaniya (poluchenie znacheniya) net??
}

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
    if (handle == LSQ_HandleInvalid) return NULL;
    LSQ_TIterator *iterator = (LSQ_TIterator*)malloc(sizeof(LSQ_TIterator));
    iterator->index = index;
    iterator->seq = (LSQ_Type*)handle;
    return ((LSQ_IteratorT)iterator);
}

LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    return handle == LSQ_HandleInvalid ? NULL : LSQ_GetElementByIndex(handle, 0);
}

LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    return handle == LSQ_HandleInvalid ? NULL : LSQ_GetElementByIndex(handle, (LSQ_GetSize(handle))); //check
}

void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    if (iterator == LSQ_HandleInvalid) return;
    free((LSQ_TIterator*)iterator);
}

void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    if ((LSQ_TIterator*)iterator == NULL) return;
    ((LSQ_TIterator*)iterator)->index++;
}

void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
    if ((LSQ_TIterator*)iterator == NULL) return;
    ((LSQ_TIterator*)iterator)->index--;
}

void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
    if ((LSQ_TIterator*)iterator == NULL) return;
    ((LSQ_TIterator*)iterator)->index += shift;
}

void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
    if ((LSQ_TIterator*)iterator == NULL) return;
    ((LSQ_TIterator*)iterator)->index = pos;
}

void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    if (handle == LSQ_HandleInvalid) return;
    LSQ_TIterator *iterator = (LSQ_TIterator*)LSQ_GetFrontElement(handle);      //typecast mojet bit'
    if (iterator == NULL) return;
    LSQ_InsertElementBeforeGiven(iterator, element);
    LSQ_DestroyIterator(iterator);
}

void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    if (handle == LSQ_HandleInvalid) return;
    LSQ_TIterator *iterator = (LSQ_TIterator*)LSQ_GetPastRearElement(handle);      //typecast mojet bit'
    if (iterator == NULL) return;
    LSQ_InsertElementBeforeGiven(iterator, element);
    LSQ_DestroyIterator(iterator);
}

void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement) {
    if (iterator == NULL) return;
    LSQ_Type *ItSeq = ((LSQ_TIterator*)iterator)->seq;
    if (ItSeq->RealSize == ItSeq->MemSize){
        ItSeq->MemSize *= 2;
        ItSeq->LSQ_Elements = (LSQ_BaseTypeT*)realloc(ItSeq->LSQ_Elements, ItSeq->MemSize * sizeof(LSQ_BaseTypeT));
    }
    ItSeq->RealSize++;
    memmove(ItSeq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index + 1,
            ItSeq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index,
            sizeof(LSQ_BaseTypeT) * (ItSeq->RealSize - ((LSQ_TIterator*)iterator)->index - 1)); //ya ponyal
    *(ItSeq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index) = newElement; //razyimenovaniye
}

void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid) return;
    LSQ_TIterator *iterator = (LSQ_TIterator*)LSQ_GetFrontElement(handle);      //typecast mojet bit'
    if (iterator == NULL) return;
    LSQ_DeleteGivenElement(iterator);
    LSQ_DestroyIterator(iterator);
}

void LSQ_DeleteRearElement(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid) return;
    LSQ_TIterator *iterator = (LSQ_TIterator*)LSQ_GetPastRearElement(handle);       //typecast mojet bit'
    iterator->index--;
    if (iterator == NULL) return;
    LSQ_DeleteGivenElement(iterator);
    LSQ_DestroyIterator(iterator);
}

void LSQ_DeleteGivenElement(LSQ_IteratorT iterator) {
    if (!LSQ_IsIteratorDereferencable(iterator)) return;
    LSQ_Type *ItSeq = ((LSQ_TIterator*)iterator)->seq;
    ItSeq->RealSize--;
    memmove(ItSeq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index,
            ItSeq->LSQ_Elements + ((LSQ_TIterator*)iterator)->index + 1,
            sizeof(LSQ_BaseTypeT) * (ItSeq->RealSize - ((LSQ_TIterator*)iterator)->index));
    if (ItSeq->RealSize <= ItSeq->MemSize * 0.25) {
        ItSeq->MemSize /= 2;
        if (ItSeq->MemSize == 0) ItSeq->MemSize = 1;
        ItSeq->LSQ_Elements = (LSQ_BaseTypeT*)realloc(ItSeq->LSQ_Elements, ItSeq->MemSize * sizeof(LSQ_BaseTypeT));
    }
}