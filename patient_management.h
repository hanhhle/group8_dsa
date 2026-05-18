#ifndef PATIENT_MANAGEMENT_H
#define PATIENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>  

struct VisitRecord {
    char arrivalDate[11];
    char symptoms[100];
    char doctorName[50];
    char treatment[256];
    char prescribedMedicine[100];
};

typedef struct {
    char id[10];
    char phoneNumber[10];
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

Node* createNode(Patient p);
void addPatient(Node **head, Patient p);
Node* searchPatient(Node *head, char id[]);
Node* searchPatientByPhone(Node *head, char phone[]); // Hàm mới tìm bằng SĐT

void deletePatient(Node **head, char id[]);
void updatePatient(Node *head, char id[]);

void saveToFile(Node *head);
void loadFromFile(Node **head);
void freeList(Node *head);

void displayAllPatients(Node *head);
void mergeSort(Node** headRef);

int isDuplicate(Node *head, char id[]);
int readLine(char *buffer, size_t size);
int isValidDate(const char *date);
int readInt(const char *prompt, int *outValue);

#endif // PATIENT_MANAGEMENT_H