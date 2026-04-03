#include "patient_management.h"

// 2.5 addNew
void addPatient(Node **head, Patient p) {
    Node* newNode = createNode(p);
    
    if (newNode == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// 2.7 searchPatient
Node* searchPatient(Node *head, char id[]) {
    Node *temp = head;
    
    while (temp != NULL) {
        if (strcmp(temp->data.id, id) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    
    return NULL;
}

// 2.6 updatePatient
void updatePatient(Node *head, char id[]) {
    Node *target = searchPatient(head, id);
    
    if (target == NULL) {
        printf("Error: Patient with ID '%s' not found.\n", id);
        return;
    }

    printf("\n--- Updating Information for Patient: %s ---\n", target->data.name);

    printf("Enter new name (current: %s): ", target->data.name);
    scanf(" %49[^\n]", target->data.name);

    printf("Enter new age (current: %d): ", target->data.age);
    scanf("%d", &target->data.age);

    printf("Enter new gender (current: %s): ", target->data.gender);
    scanf(" %9[^\n]", target->data.gender);

    printf("Enter new disease/diagnosis (current: %s): ", target->data.disease);
    scanf(" %99[^\n]", target->data.disease);

    printf("Enter new admission date [DD/MM/YYYY] (current: %s): ", target->data.admissionDate);
    scanf(" %10[^\n]", target->data.admissionDate);

    printf("-> Success: Patient information updated successfully!\n");
}

// 2.8 deletePatient
void deletePatient(Node **head, char id[]) {
    if (*head == NULL) {
        printf("Error: The patient list is empty. Cannot delete.\n");
        return;
    }

    Node *temp = *head;
    Node *prev = NULL;

    if (temp != NULL && strcmp(temp->data.id, id) == 0) {
        *head = temp->next;
        free(temp);
        printf("Success: Patient with ID '%s' has been deleted.\n", id);
        return;
    }

    while (temp != NULL && strcmp(temp->data.id, id) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Error: Patient with ID '%s' not found.\n", id);
        return;
    }

    prev->next = temp->next; 
    
    free(temp);
    printf("Success: Patient with ID '%s' has been deleted.\n", id);
}