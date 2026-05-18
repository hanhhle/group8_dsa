#include "patient_management.h"
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
    if (node == NULL) return;

    printf("\n============================================\n");
    printf("              PATIENT DETAILS               \n");
    printf("============================================\n");
    printf("ID: %s | Name: %s\n", node->data.id, node->data.name);
    printf("Age: %d | Gender: %s\n", node->data.age, node->data.gender);
    printf("Total Visits: %d\n", node->data.visitCount);
    printf("--------------------------------------------\n");
    
    for (int i = 0; i < node->data.visitCount; i++) {
        printf("[Visit %d] - Date: %s\n", i + 1, node->data.history[i].arrivalDate);
        printf(" * Symptoms : %s\n", node->data.history[i].symptoms);
        printf(" * Doctor   : %s\n", node->data.history[i].assignedDoctor.name);
        printf(" * Diagnosis: %s\n", node->data.history[i].currentTreatment.diagnosis);
        printf("- - - - - - - - - - - - - - - - - - - - - - \n");
    }
}

static struct VisitRecord tempVisit; // Biến tạm để lưu dữ liệu lần khám đầu

static int inputNewPatient(Node *head, Patient *p) {
    int age;

    printf("Enter patient ID (max 9 chars): ");
    if (!readLine(p->id, sizeof(p->id)) || strlen(p->id) == 0) return 0;

    // Chỉ bắt nhập Tên, Tuổi nếu là BỆNH NHÂN MỚI TOANH
    if (!isDuplicate(head, p->id)) {
        printf("Enter patient name: ");
        if (!readLine(p->name, sizeof(p->name))) return 0;

        if (!readInt("Enter age: ", &age) || age <= 0) return 0;
        p->age = age;

        printf("Enter gender: ");
        if (!readLine(p->gender, sizeof(p->gender))) return 0;
    } else {
        printf("-> Patient exists! Please enter new visit details.\n");
    }

    // --- NHẬP THÔNG TIN CHO ĐỢT KHÁM NÀY ---
    printf("Enter arrival date [YYYY-MM-DD]: ");
    if (!readLine(tempVisit.arrivalDate, sizeof(tempVisit.arrivalDate)) || !isValidDate(tempVisit.arrivalDate)) {
        printf("Error: Invalid date format.\n");
        return 0;
    }

    printf("Enter symptoms: ");
    readLine(tempVisit.symptoms, sizeof(tempVisit.symptoms));

    printf("Enter Doctor's name: ");
    readLine(tempVisit.assignedDoctor.name, sizeof(tempVisit.assignedDoctor.name));

    printf("Enter Diagnosis: ");
    readLine(tempVisit.currentTreatment.diagnosis, sizeof(tempVisit.currentTreatment.diagnosis));

    // Móc con trỏ history của p vào biến tạm để truyền sang hàm addPatient
    p->history = &tempVisit; 

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
        printf("No existing data file found. Starting fresh.\n");
        return;
    }

    char line[2048]; // Buffer lớn để chứa nguyên 1 dòng dài
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0; // Xóa ký tự xuống dòng
        if(strlen(line) < 5) continue;

        Patient p;
        int vCount = 0;
        
        // Cắt khúc đầu tiên (trước dấu |) để lấy thông tin cơ bản
        char *token = strtok(line, "|");
        if(token == NULL) continue;
        
        sscanf(token, "%[^,],%[^,],%d,%[^,],%d", p.id, p.name, &p.age, p.gender, &vCount);
        
        p.visitCount = vCount;
        p.capacity = (vCount > 0) ? vCount : 2;
        // Cấp phát mảng động
        p.history = (struct VisitRecord*)malloc(p.capacity * sizeof(struct VisitRecord));

        // Cắt các khúc tiếp theo (sau dấu |) để nạp lịch sử khám
        for (int i = 0; i < vCount; i++) {
            token = strtok(NULL, "|");
            if (token != NULL) {
                // Parse chuỗi con: Date,Symptoms,Doctor,Diagnosis
                char *sub = token;
                char *comma;
                
                comma = strchr(sub, ','); if(comma){ *comma=0; strcpy(p.history[i].arrivalDate, sub); sub=comma+1; }
                comma = strchr(sub, ','); if(comma){ *comma=0; strcpy(p.history[i].symptoms, sub); sub=comma+1; }
                comma = strchr(sub, ','); if(comma){ *comma=0; strcpy(p.history[i].assignedDoctor.name, sub); sub=comma+1; }
                strcpy(p.history[i].currentTreatment.diagnosis, sub);
            }
        }

        // Tạo Node và nối thẳng vào đuôi danh sách (không gọi hàm addPatient để tránh lỗi malloc kép)
        Node* newNode = createNode(p);
        if (*head == NULL) {
            *head = newNode;
        } else {
            Node* temp = *head;
            while(temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
    }
    fclose(f);
}

// 2.12 SAVE DATA TO FILE
// Format: ID,Name,Age,Gender,VisitCount|Date,Symptoms,Doctor,Diagnosis|Date,...
void saveToFile(Node *head) {
    FILE *f = fopen("patients.txt", "w");
    if (f == NULL) {
        printf("Cannot open file for writing\n");
        return;
    }

    Node *temp = head;
    while (temp != NULL) {
        // 1. Ghi thông tin tĩnh
        fprintf(f, "%s,%s,%d,%s,%d", 
                temp->data.id, temp->data.name, temp->data.age, temp->data.gender, temp->data.visitCount);
        
        // 2. Ghi mảng lịch sử (dùng dấu | để ngăn cách các lần khám)
        for(int i = 0; i < temp->data.visitCount; i++) {
            fprintf(f, "|%s,%s,%s,%s", 
                    temp->data.history[i].arrivalDate,
                    temp->data.history[i].symptoms,
                    temp->data.history[i].assignedDoctor.name,
                    temp->data.history[i].currentTreatment.diagnosis);
        }
        fprintf(f, "\n");
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
        free(temp->data.history);
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
    printf("\n%-10s %-20s %-5s %-10s %-15s\n", "ID", "Name", "Age", "Gender", "Total Visits");
    printf("-----------------------------------------------------------------\n");
    Node *temp = head;
    while (temp != NULL) {
        printf("%-10s %-20s %-5d %-10s %-15d\n",
               temp->data.id, temp->data.name, temp->data.age,
               temp->data.gender, temp->data.visitCount);
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
