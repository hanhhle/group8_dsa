#ifndef PATIENT_MANAGEMENT_H
#define PATIENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

struct Doctor {
    char doctorId[10];
    char name[50];
    char specialization[50];
};

struct Medicine {
    char medicineId[10];
    char medicineName[50];
    double price;
};

struct Treatment {
    char diagnosis[100];
    char method[150];
    char prescribedMedicineId[10];
};

struct VisitRecord {
    char arrivalDate[11];
    char symptoms[100];
    struct Doctor assignedDoctor;
    struct Treatment currentTreatment;
};

typedef struct {
    char id[10];
    char name[50];
    int age;
    char gender[10];
    struct VisitRecord* history;
    int visitCount;
    int capacity;
} Patient;

typedef struct Node {
    Patient data;
    struct Node* next;
} Node;

// CRUD
Node* createNode(Patient p);
void addPatient(Node **head, Patient p);
Node* searchPatient(Node *head, char id[]);
void deletePatient(Node **head, char id[]);
void updatePatient(Node *head, char id[]);

// File
void saveToFile(Node *head);
void loadFromFile(Node **head);
void freeList(Node *head);

// Utility
void displayAllPatients(Node *head);
void mergeSort(Node** headRef);

int isDuplicate(Node *head, char id[]);
int readLine(char *buffer, size_t size);
int isValidDate(const char *date);
int readInt(const char *prompt, int *outValue);

#endif // PATIENT_MANAGEMENT_H