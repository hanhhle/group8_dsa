#include "patient_management.h"

// =======================
// 2.1 CREATE NODE (FROM PATIENT)
// =======================
Node* createNode(Patient p) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    newNode->data = p;
    newNode->next = NULL;

    return newNode;
}


// =======================
// 2.2 ADD PATIENT TO LIST
// =======================
void addPatient(Node **head, Patient p) {
    Node* newNode = createNode(p);

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


// =======================
// 2.3 LOAD DATA FROM FILE
// =======================
void loadFromFile(Node **head) {
    FILE *f = fopen("patients.txt", "r");

    if (f == NULL) {
        printf("Cannot open file\n");
        return;
    }

    Patient p;

    while (fscanf(f, "%9[^,],%49[^,],%d,%9[^,],%99[^,],%10[^\n]",
                  p.id, p.name, &p.age, p.gender, p.disease, p.admissionDate) == 6) {

        addPatient(head, p);
    }

    fclose(f);
}


// =======================
// 2.12 SAVE DATA TO FILE
// =======================
void saveToFile(Node *head) {
    FILE *f = fopen("patients.txt", "w");

    if (f == NULL) {
        printf("Cannot open file for writing\n");
        return;
    }

    Node *temp = head;

    while (temp != NULL) {
        fprintf(f, "%s,%s,%d,%s,%s,%s\n",
                temp->data.id,
                temp->data.name,
                temp->data.age,
                temp->data.gender,
                temp->data.disease,
                temp->data.admissionDate);

        temp = temp->next;
    }

    fclose(f);
}


// =======================
// 2.13 FREE MEMORY
// =======================
void freeList(Node *head) {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}


// =======================
// DISPLAY (TEST ONLY)
// =======================
void displayPatients(Node *head) {
    Node *temp = head;

    while (temp != NULL) {
        printf("%s | %s | %d | %s | %s | %s\n",
               temp->data.id,
               temp->data.name,
               temp->data.age,
               temp->data.gender,
               temp->data.disease,
               temp->data.admissionDate);
        temp = temp->next;
    }
}


// =======================
// MAIN (TEST FLOW)
// =======================
int main() {
    Node *head = NULL;

    loadFromFile(&head);

    printf("=== Patient List ===\n");
    displayPatients(head);

    saveToFile(head);

    freeList(head);

    printf("Program executed successfully!\n");

    return 0;
}
