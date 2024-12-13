#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struktur data
typedef struct patient
{
    int id;
    char name[50];
    int age;
    char gender[10];
    char status[20];
    int priority;
    struct patient *next;
    struct patient *left;
    struct patient *right;
    char diagnosis[100];
    char treatment[100];
}patient;

// Fungsi untuk memindahkan pasien dari Queue ke Double Linked List berdasarkan prioritas
void moveToActiveQueue(Queue *queue, DoubleLinkedList *activeQueue)
{
    Patient *nextPatient = findHighestPriorityPatient(queue);
    if (nextPatient == NULL)
    {
        printf("Tidak ada pasien dalam antrian registrasi.\n");
        return;
    }

    nextPatient->left = NULL;
    nextPatient->right = NULL;

    // Menambahkan pasien ke antrian aktif berdasarkan prioritas
    if (activeQueue->left == NULL)
    {
        activeQueue->left = activeQueue->right = nextPatient;
    }
    else
    {
        Patient *current = activeQueue->left;
        // Cari posisi yang tepat berdasarkan prioritas
        while (current != NULL && current->priority <= nextPatient->priority)
        {
            current = current->right;
        }

        if (current == NULL)
        {
            // Tambahkan di akhir
            activeQueue->right->right = nextPatient;
            nextPatient->left = activeQueue->right;
            activeQueue->right = nextPatient;
        }
        else if (current == activeQueue->left)
        {
            // Tambahkan di awal
            nextPatient->right = activeQueue->left;
            activeQueue->left->left = nextPatient;
            activeQueue->left = nextPatient;
        }
        else
        {
            // Tambahkan di tengah
            nextPatient->right = current;
            nextPatient->left = current->left;
            current->left->right = nextPatient;
            current->left = nextPatient;
        }
    }

    printf("Pasien %s (ID: %d) dipindahkan ke antrian aktif.\n", nextPatient->name, nextPatient->id);
}