#include "patient_management.h"
int readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

int isValidDate(const char *date) {
    if (strlen(date) != 10) {
        return 0;
    }

    if (date[4] != '-' || date[7] != '-') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }

        if (!isdigit((unsigned char)date[i])) {
            return 0;
        }
    }

    return 1;
}

int readInt(const char *prompt, int *outValue) {
    char line[32];
    char *endPtr;
    long value;

    printf("%s", prompt);
    if (!readLine(line, sizeof(line))) {
        return 0;
    }

    value = strtol(line, &endPtr, 10);
    if (endPtr == line || *endPtr != '\0') {
        return 0;
    }

    *outValue = (int)value;
    return 1;
}

static void displayPatientDetail(const Node *node) {
    if (node == NULL) {
        return;
    }

    printf("\n--- Patient Details ---\n");
    printf("ID: %s\n", node->data.id);
    printf("Name: %s\n", node->data.name);
    printf("Age: %d\n", node->data.age);
    printf("Gender: %s\n", node->data.gender);
    printf("Disease/Diagnosis: %s\n", node->data.disease);
    printf("Admission Date: %s\n", node->data.admissionDate);
}

static int inputNewPatient(Node *head, Patient *p) {
    int age;

    printf("Enter patient ID (max 9 chars): ");
    if (!readLine(p->id, sizeof(p->id)) || strlen(p->id) == 0) {
        printf("Error: Invalid ID input.\n");
        return 0;
    }

    if (isDuplicate(head, p->id)) {
        printf("Error: Patient ID '%s' already exists.\n", p->id);
        return 0;
    }

    printf("Enter patient name: ");
    if (!readLine(p->name, sizeof(p->name)) || strlen(p->name) == 0) {
        printf("Error: Invalid name input.\n");
        return 0;
    }

    if (!readInt("Enter age: ", &age) || age <= 0) {
        printf("Error: Age must be a positive number.\n");
        return 0;
    }
    p->age = age;

    printf("Enter gender: ");
    if (!readLine(p->gender, sizeof(p->gender)) || strlen(p->gender) == 0) {
        printf("Error: Invalid gender input.\n");
        return 0;
    }

    printf("Enter disease/diagnosis: ");
    if (!readLine(p->disease, sizeof(p->disease)) || strlen(p->disease) == 0) {
        printf("Error: Invalid diagnosis input.\n");
        return 0;
    }

    printf("Enter admission date [YYYY-MM-DD]: ");
    if (!readLine(p->admissionDate, sizeof(p->admissionDate)) || !isValidDate(p->admissionDate)) {
        printf("Error: Invalid date format. Use YYYY-MM-DD.\n");
        return 0;
    }

    return 1;
}

static int menu(void) {
    int choice;

    printf("\n========== Patient Management Menu ==========\n");
    printf("1. Add new patient\n");
    printf("2. Search patient by ID\n");
    printf("3. Update patient information\n");
    printf("4. Delete patient\n");
    printf("5. Display all patients\n");
    printf("6. Sort patients by ID\n");
    printf("7. Save data to file\n");
    printf("8. Exit\n");

    if (!readInt("Choose an option (1-8): ", &choice)) {
        return -1;
    }

    return choice;
}

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
// 2.3 LOAD DATA FROM FILE
// =======================
void loadFromFile(Node **head) {
    FILE *f = fopen("patients.txt", "r");

    if (f == NULL) {
        printf("Cannot open file\n");
        return;
    }

    Patient p;

    while (fscanf(f, "%9[^,],%49[^,],%d,%9[^,],%99[^,],%10[^\n]\n",
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
void displayAllPatients(Node *head) {
    if (head == NULL) {
        printf("The patient list is empty.\n");
        return;
    }
    printf("\n%-10s %-20s %-5s %-10s %-20s %-15s\n",
           "ID", "Name", "Age", "Gender", "Disease", "Admission");
    printf("--------------------------------------------------------------------------------\n");
    Node *temp = head;
    while (temp != NULL) {
        printf("%-10s %-20s %-5d %-10s %-20s %-15s\n",
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
    int choice;
    char id[10];
    Patient p;
    Node *found;

    loadFromFile(&head);

    while (1) {
        choice = menu();

        switch (choice) {
            case 1:
                if (inputNewPatient(head, &p)) {
                    addPatient(&head, p);
                    printf("Success: Patient added successfully.\n");
                }
                break;

            case 2:
                printf("Enter patient ID to search: ");
                if (!readLine(id, sizeof(id)) || strlen(id) == 0) {
                    printf("Error: Invalid ID input.\n");
                    break;
                }
                found = searchPatient(head, id);
                if (found == NULL) {
                    printf("Error: Patient with ID '%s' not found.\n", id);
                } else {
                    displayPatientDetail(found);
                }
                break;

            case 3:
                printf("Enter patient ID to update: ");
                if (!readLine(id, sizeof(id)) || strlen(id) == 0) {
                    printf("Error: Invalid ID input.\n");
                    break;
                }
                updatePatient(head, id);
                break;

            case 4:
                printf("Enter patient ID to delete: ");
                if (!readLine(id, sizeof(id)) || strlen(id) == 0) {
                    printf("Error: Invalid ID input.\n");
                    break;
                }
                deletePatient(&head, id);
                break;

            case 5:
                displayAllPatients(head);
                break;

            case 6:
                mergeSort(&head);
                printf("Success: Patients have been sorted by ID.\n");
                break;

            case 7:
                saveToFile(head);
                printf("Success: Data saved to patients.txt.\n");
                break;

            case 8:
                saveToFile(head);
                freeList(head);
                printf("Program terminated. Data saved successfully.\n");
                return 0;

            default:
                printf("Error: Please choose a valid option from 1 to 8.\n");
                break;
        }
    }

    return 0;
}
