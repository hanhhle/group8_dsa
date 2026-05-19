#include "patient_management.h"

// VALIDATION & INPUT
int readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    if (strchr(buffer, '\n') != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 1;
}

int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)date[i])) return 0;
    }
    return 1;
}

int readInt(const char *prompt, int *outValue) {
    char line[32];
    char *endPtr;
    long value;

    printf("%s", prompt);
    if (!readLine(line, sizeof(line))) return 0;

    value = strtol(line, &endPtr, 10);
    if (endPtr == line || *endPtr != '\0') return 0;

    *outValue = (int)value;
    return 1;
}

// UI
static void displayPatientDetail(const Node *node) {
    if (node == NULL) return;

    printf("\n============================================\n");
    printf("              PATIENT DETAILS               \n");
    printf("============================================\n");
    printf("ID: %s | Name: %s\n", node->data.id, node->data.name);
    printf("Phone: %s | Age: %d | Gender: %s\n", node->data.phoneNumber, node->data.age, node->data.gender);
    printf("Total Visits: %d\n", node->data.visitCount);
    printf("--------------------------------------------\n");

    for (int i = 0; i < node->data.visitCount; i++) {
        printf("[Visit %d] - Date: %s\n", i + 1, node->data.history[i].arrivalDate);
        printf(" * Symptoms : %s\n", node->data.history[i].symptoms);
        printf(" * Doctor   : %s\n", node->data.history[i].doctorName);
        printf(" * Treatment: %s\n", node->data.history[i].treatment);
        printf(" * Medicine : %s\n", node->data.history[i].prescribedMedicine);
        printf("- - - - - - - - - - - - - - - - - - - - - - \n");
    }
}

// Hàm tự sinh số thứ tự tiếp theo (1, 2, 3...)
static int getNextIdNumber(Node *head) {
    int maxNum = 0;
    Node *temp = head;
    while (temp != NULL) {
        int num = atoi(temp->data.id);
        if (num > maxNum) maxNum = num;
        temp = temp->next;
    }
    return maxNum + 1;
}

static struct VisitRecord tempVisit; 

// Hàm nghiệp vụ Tiếp nhận bệnh nhân
static int inputNewPatient(Node *head, Patient *p) {
    char buffer[128]; 
    int age;

    printf("\n--- PATIENT RECEPTION ---\n");
    printf("Enter Patient's Phone Number: ");
    
    if (!readLine(buffer, sizeof(buffer)) || strlen(buffer) == 0) return 0;
    
    snprintf(p->phoneNumber, sizeof(p->phoneNumber), "%s", buffer);
    
    Node *existing = searchPatientByPhone(head, p->phoneNumber);
    
    if (existing != NULL) {
        printf("-> System Alert: Phone number found in database!\n");
        printf("   [ ID: %s | Name: %s | Age: %d ]\n", existing->data.id, existing->data.name, existing->data.age);
        
        snprintf(p->id, sizeof(p->id), "%s", existing->data.id);
        snprintf(p->name, sizeof(p->name), "%s", existing->data.name);
        p->age = existing->data.age;
        snprintf(p->gender, sizeof(p->gender), "%s", existing->data.gender);
    } else {
        printf("-> System Alert: Phone number NOT found! Registering a new profile...\n");
        
        int nextNum = getNextIdNumber(head);
        snprintf(p->id, sizeof(p->id), "%d", nextNum);
        
        printf("-> Assigned New ID: %s\n", p->id);

        printf("Enter patient name: ");
        if (!readLine(buffer, sizeof(buffer))) return 0;
        snprintf(p->name, sizeof(p->name), "%s", buffer);

        if (!readInt("Enter age: ", &age) || age <= 0) return 0;
        p->age = age;

        printf("Enter gender: ");
        if (!readLine(buffer, sizeof(buffer))) return 0;
        snprintf(p->gender, sizeof(p->gender), "%s", buffer);
    }

    // NHẬP LỊCH SỬ KHÁM 
    printf("\n--- Enter Visit Details ---\n");
    printf("Enter arrival date [YYYY-MM-DD]: ");
    if (!readLine(buffer, sizeof(buffer)) || !isValidDate(buffer)) {
        printf("Error: Invalid date format.\n");
        return 0;
    }
    snprintf(tempVisit.arrivalDate, sizeof(tempVisit.arrivalDate), "%s", buffer);

    printf("Enter symptoms: ");
    if (readLine(buffer, sizeof(buffer))) {
        snprintf(tempVisit.symptoms, sizeof(tempVisit.symptoms), "%s", buffer);
    }

    printf("Enter Doctor's name: ");
    if (readLine(buffer, sizeof(buffer))) {
        snprintf(tempVisit.doctorName, sizeof(tempVisit.doctorName), "%s", buffer);
    }

    printf("Enter Treatment Details: ");
    if (readLine(buffer, sizeof(buffer))) {
        snprintf(tempVisit.treatment, sizeof(tempVisit.treatment), "%s", buffer);
    }

    printf("Enter Prescribed Medicine: ");
    if (readLine(buffer, sizeof(buffer))) {
        snprintf(tempVisit.prescribedMedicine, sizeof(tempVisit.prescribedMedicine), "%s", buffer);
    }

    p->history = &tempVisit;
    return 1;
}

static int menu(void) {
    int choice;
    printf("\n========== Patient Management Menu ==========\n");
    printf("1. Patient Reception / Registration\n");
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
// TẠO NODE & QUẢN LÝ FILE
// =======================
Node *createNode(Patient p) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newNode->data = p;
    newNode->next = NULL;
    return newNode;
}

void loadFromFile(Node **head) {
    FILE *f = fopen("patients.txt", "r");
    if (f == NULL) {
        printf("No existing data file found. Starting fresh.\n");
        return;
    }

    char line[2048]; 
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0; 
        if (strlen(line) < 5) continue;

        Patient p;
        int vCount = 0;

        char *token = strtok(line, "|");
        if (token == NULL) continue;

        sscanf(token, "%[^,],%[^,],%[^,],%d,%[^,],%d", 
               p.id, p.phoneNumber, p.name, &p.age, p.gender, &vCount);

        p.visitCount = vCount;
        p.capacity = (vCount > 0) ? vCount : 2;
        p.history = (struct VisitRecord *)malloc(p.capacity * sizeof(struct VisitRecord));

        for (int i = 0; i < vCount; i++) {
            token = strtok(NULL, "|");
            if (token != NULL) {
                char *sub = token;
                char *comma;

                comma = strchr(sub, ','); if (comma) { *comma = 0; snprintf(p.history[i].arrivalDate, sizeof(p.history[i].arrivalDate), "%s", sub); sub = comma + 1; }
                comma = strchr(sub, ','); if (comma) { *comma = 0; snprintf(p.history[i].symptoms, sizeof(p.history[i].symptoms), "%s", sub); sub = comma + 1; }
                comma = strchr(sub, ','); if (comma) { *comma = 0; snprintf(p.history[i].doctorName, sizeof(p.history[i].doctorName), "%s", sub); sub = comma + 1; }
                comma = strchr(sub, ','); if (comma) { *comma = 0; snprintf(p.history[i].treatment, sizeof(p.history[i].treatment), "%s", sub); sub = comma + 1; }
                snprintf(p.history[i].prescribedMedicine, sizeof(p.history[i].prescribedMedicine), "%s", sub);
            }
        }

        Node *newNode = createNode(p);
        if (*head == NULL) {
            *head = newNode;
        } else {
            Node *temp = *head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
    }
    fclose(f);
}

void saveToFile(Node *head) {
    FILE *f = fopen("patients.txt", "w");
    if (f == NULL) {
        printf("Cannot open file for writing\n");
        return;
    }

    Node *temp = head;
    while (temp != NULL) {
        fprintf(f, "%s,%s,%s,%d,%s,%d",
                temp->data.id, temp->data.phoneNumber, temp->data.name, 
                temp->data.age, temp->data.gender, temp->data.visitCount);

        for (int i = 0; i < temp->data.visitCount; i++) {
            fprintf(f, "|%s,%s,%s,%s,%s",
                    temp->data.history[i].arrivalDate,
                    temp->data.history[i].symptoms,
                    temp->data.history[i].doctorName,
                    temp->data.history[i].treatment,
                    temp->data.history[i].prescribedMedicine);
        }
        fprintf(f, "\n");
        temp = temp->next;
    }
    fclose(f);
}

void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data.history);
        free(temp);
    }
}

void displayAllPatients(Node *head) {
    if (head == NULL) {
        printf("The patient list is empty.\n");
        return;
    }
    printf("\n%-10s %-15s %-20s %-5s %-10s %-15s\n", "ID", "Phone", "Name", "Age", "Gender", "Total Visits");
    printf("--------------------------------------------------------------------------------\n");
    Node *temp = head;
    while (temp != NULL) {
        printf("%-10s %-15s %-20s %-5d %-10s %-15d\n",
               temp->data.id, temp->data.phoneNumber, temp->data.name, 
               temp->data.age, temp->data.gender, temp->data.visitCount);
        temp = temp->next;
    }
}

// =======================
// MAIN FLOW
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