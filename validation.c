#include "patient_management.h"

int isDuplicate(Node *head, char id[]) {
    if (searchPatient(head, id) != NULL) return 1;
    return 0;
}