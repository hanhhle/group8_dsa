#include "patient_management.h"

// ==============================================================
// 1. TÌM KIẾM THEO ID
// ==============================================================
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

// ==============================================================
// 2. TÌM KIẾM THEO SĐT
// ==============================================================
Node* searchPatientByPhone(Node *head, char phone[]) {
    Node *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data.phoneNumber, phone) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// ==============================================================
// 3. THÊM BỆNH NHÂN / LƯỢT KHÁM MỚI
// ==============================================================
void addPatient(Node **head, Patient p) {
    Node* existing = searchPatient(*head, p.id);
    
    if (existing != NULL) {
        // --- BỆNH NHÂN CŨ: Nới rộng mảng lịch sử ---
        if (existing->data.visitCount >= existing->data.capacity) {
            int newCapacity = existing->data.capacity * 2;
            if (newCapacity == 0) newCapacity = 2; 
            
            struct VisitRecord* newHistory = (struct VisitRecord*)realloc(
                existing->data.history, 
                newCapacity * sizeof(struct VisitRecord)
            );
            
            if (newHistory == NULL) {
                printf("Error: Memory allocation failed during realloc!\n");
                return;
            }
            existing->data.history = newHistory;
            existing->data.capacity = newCapacity;
        }
        
        // Chép dữ liệu lần khám mới
        existing->data.history[existing->data.visitCount] = p.history[0];
        existing->data.visitCount++;
        
        printf("-> Success: New visit record added successfully!\n");
        return; 
    }

    // --- BỆNH NHÂN MỚI TOANH ---
    p.capacity = 2; 
    p.visitCount = 1;
    
    struct VisitRecord* initialHistory = (struct VisitRecord*)malloc(p.capacity * sizeof(struct VisitRecord));
    if (initialHistory == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    
    initialHistory[0] = p.history[0]; 
    p.history = initialHistory; 

    Node* newNode = createNode(p);
    if (newNode == NULL) {
        free(initialHistory); 
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
    printf("-> Success: New patient registered successfully!\n");
}

// ==============================================================
// 4. CẬP NHẬT THÔNG TIN (ĐÃ DÙNG SNPRINTF ĐỂ CHỐNG TRACE TRAP)
// ==============================================================
void updatePatient(Node *head, char id[]) {
    Node *target = searchPatient(head, id);
    
    if (target == NULL) {
        printf("Error: Patient with ID '%s' not found.\n", id);
        return;
    }

    Patient tempPatient = target->data;
    int lastIdx = tempPatient.visitCount - 1;
    struct VisitRecord tempVisit = target->data.history[lastIdx];
    
    char buffer[256]; 

    printf("\n--- Updating Information for Patient: %s ---\n", tempPatient.name);
    printf("(Press Enter to skip and keep current value)\n");

    // Sửa thông tin Tĩnh (Dùng snprintf giới hạn đúng kích thước struct, an toàn tuyệt đối)
    printf("Enter new name (current: %s): ", tempPatient.name);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempPatient.name, sizeof(tempPatient.name), "%s", buffer);

    printf("Enter new phone number (current: %s): ", tempPatient.phoneNumber);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempPatient.phoneNumber, sizeof(tempPatient.phoneNumber), "%s", buffer);

    printf("Enter new age (current: %d): ", tempPatient.age);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) {
        char *endPtr;
        long val = strtol(buffer, &endPtr, 10);
        if (endPtr == buffer || *endPtr != '\0' || val <= 0) {
            printf("Error: Invalid age input. Canceled.\n");
            return;
        }
        tempPatient.age = (int)val;
    }

    printf("Enter new gender (current: %s): ", tempPatient.gender);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempPatient.gender, sizeof(tempPatient.gender), "%s", buffer);

    // Sửa thông tin Động
    printf("Enter new arrival date [YYYY-MM-DD] (current: %s): ", tempVisit.arrivalDate);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) {
        if (!isValidDate(buffer)) {
            printf("Error: Invalid date format. Canceled.\n");
            return;
        }
        snprintf(tempVisit.arrivalDate, sizeof(tempVisit.arrivalDate), "%s", buffer);
    }
    
    printf("Enter new symptoms (current: %s): ", tempVisit.symptoms);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempVisit.symptoms, sizeof(tempVisit.symptoms), "%s", buffer);

    printf("Enter new Doctor name (current: %s): ", tempVisit.doctorName);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempVisit.doctorName, sizeof(tempVisit.doctorName), "%s", buffer);

    printf("Enter new Treatment (current: %s): ", tempVisit.treatment);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempVisit.treatment, sizeof(tempVisit.treatment), "%s", buffer);

    printf("Enter new Prescribed Medicine (current: %s): ", tempVisit.prescribedMedicine);
    if (!readLine(buffer, sizeof(buffer))) return;
    if (strlen(buffer) > 0) snprintf(tempVisit.prescribedMedicine, sizeof(tempVisit.prescribedMedicine), "%s", buffer);

    // Cập nhật lại vào dữ liệu gốc
    target->data = tempPatient; 
    target->data.history[lastIdx] = tempVisit; 

    printf("-> Success: Patient information updated successfully!\n");
}

// ==============================================================
// 5. XÓA BỆNH NHÂN
// ==============================================================
void deletePatient(Node **head, char id[]) {
    if (*head == NULL) {
        printf("Error: The patient list is empty. Cannot delete.\n");
        return;
    }

    Node *temp = *head;
    Node *prev = NULL;

    if (temp != NULL && strcmp(temp->data.id, id) == 0) {
        *head = temp->next;
        free(temp->data.history); 
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
    
    free(temp->data.history); 
    free(temp);               
    printf("Success: Patient with ID '%s' has been deleted.\n", id);
}