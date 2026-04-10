#include "patient_management.h"

static Node* sortedMerge(Node* a, Node* b) {
Node* result = NULL;
    if (a == NULL) return b;
    if (b == NULL) return a;

    /* Sort by Patient ID alphabetically */
    if (strcmp(a->data.id, b->data.id) <= 0) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result;
}

static void splitList(Node* source, Node** frontRef, Node** backRef) {
    Node* fast;
    Node* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void mergeSort(Node** headRef) {
    Node* head = *headRef;
    Node* a;
    Node* b;

    if ((head == NULL) || (head->next == NULL)) return;

    splitList(head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);

    *headRef = sortedMerge(a, b);
}