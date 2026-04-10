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

    // Create a temporary copy to avoid partial updates if the user cancels halfway
    Patient tempPatient = target->data;
    char buffer[128]; // Temporary buffer for reading inputs

    printf("\n--- Updating Information for Patient: %s ---\n", tempPatient.name);
    printf("(Press Enter to skip and keep current value)\n");

    printf("Enter new name (current: %s): ", tempPatient.name);
    if (!readLine(buffer, sizeof(tempPatient.name))) return;
    if (strlen(buffer) > 0) {
        strcpy(tempPatient.name, buffer);
    }

    printf("Enter new age (current: %d): ", tempPatient.age);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) {
        char *endPtr;
        long val = strtol(buffer, &endPtr, 10);
        if (endPtr == buffer || *endPtr != '\0' || val <= 0) {
            printf("Error: Invalid age input. Update canceled.\n");
            return;
        }
        tempPatient.age = (int)val;
    }

    printf("Enter new gender (current: %s): ", tempPatient.gender);
    if (!readLine(buffer, sizeof(tempPatient.gender))) return;
    if (strlen(buffer) > 0) {
        strcpy(tempPatient.gender, buffer);
    }

    printf("Enter new disease/diagnosis (current: %s): ", tempPatient.disease);
    if (!readLine(buffer, sizeof(tempPatient.disease))) return;
    if (strlen(buffer) > 0) {
        strcpy(tempPatient.disease, buffer);
    }

    printf("Enter new admission date [YYYY-MM-DD] (current: %s): ", tempPatient.admissionDate);
    if (!readLine(buffer, sizeof(tempPatient.admissionDate))) return;
    if (strlen(buffer) > 0) {
        if (!isValidDate(buffer)) {
            printf("Error: Invalid admission date format. Update canceled.\n");
            return;
        }
        strcpy(tempPatient.admissionDate, buffer);
    }

    // If we made it this far, all inputs were valid. Apply the changes!
    target->data = tempPatient;

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