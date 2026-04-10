#ifndef PATIENT_MANAGEMENT_H
#define PATIENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char id[10];
    char name[50];
    int age;
    char gender[10];
    char disease[100];
    char admissionDate[11];
} Patient;

typedef struct Node {
    Patient data;
    struct Node* next;
} Node;

int readLine(char *buffer, size_t size);
int readInt(const char *prompt, int *outValue);
int isValidDate(const char *date);

Node* createNode(Patient p);
void addPatient(Node **head, Patient p);
int isDuplicate(Node *head, char id[]);
Node* searchPatient(Node *head, char id[]);
void updatePatient(Node *head, char id[]);
void deletePatient(Node **head, char id[]);

void loadFromFile(Node **head);
void saveToFile(Node *head);
void freeList(Node *head);

void displayAllPatients(Node *head);
void mergeSort(Node **headRef);

#endif // PATIENT_MANAGEMENT_H
