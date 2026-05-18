#ifndef PATIENT_MANAGEMENT_H
#define PATIENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h> 

struct Doctor {
    char doctorId[10];          // Doctor ID
    char name[50];              // Doctor name
    char specialization[50];    // Medical specialization
};

struct Medicine {
    char medicineId[10];        // Medicine ID
    char medicineName[50];      // Medicine name
    double price;               // Medicine price
};


struct Treatment {
    char diagnosis[100];            // Detailed diagnosis
    char method[150];               // Treatment method
    char prescribedMedicineId[10];  // Prescribed medicine ID
};

struct VisitRecord {
    char arrivalDate[11];
    char symptoms[100];
    struct Doctor assignedDoctor;
    struct Treatment currentTreatment;
};

struct Patient {
    char id[10];
    char name[50];
    int age;
    char gender[10];
    struct VisitRecord* history;
    int visitCount;
    int capacity;
};

typedef struct Node {
    struct Patient data;
    struct Node* next;
} Node;

// 1. PATIENT MANAGEMENT MODULE
Node* createNode(struct Patient p);
void addPatient(Node **head, struct Patient p);
Node* searchPatient(Node *head, char id[]);
void deletePatient(Node **head, char id[]);
void displayPatients(Node *head);
void updatePatient(Node *head, char id[]);
void saveToFile(Node *head);
void loadFromFile(Node **head);
void freeList(Node *head);

// 2. VISIT HISTORY MANAGEMENT MODULE
void addVisitRecord(struct Patient *p,
                    struct VisitRecord newVisit);
void displayVisitHistory(struct Patient *p);
void expandVisitHistory(struct Patient *p);
