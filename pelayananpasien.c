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
}Patient;

// Queue: Antrian registrasi
typedef struct
{
    Patient *front;
    Patient *rear;
} Queue;

// Double Linked List: Antrian pasien aktif
typedef struct
{
    Patient *left;  
    Patient *right; 
} DoubleLinkedList;

// Single Linked List: Riwayat pasien
typedef struct
{
    Patient *head;
} SingleLinkedList;

// Fungsi untuk menemukan pasien dengan prioritas tertinggi dalam antrian registrasi
Patient *findHighestPriorityPatient(Queue *queue)
{
    if(queue->front== NULL)
        return NULL;
    Patient *current = queue->front;
    Patient *highestPriorityPatient = current;

    //Mencari pasien dengan prioritas tertinggi
    while(current != NULL)
    {
        if (current->priority < highestPriorityPatient->priority)
        {
            highestPriorityPatient = current;
        }
        current = current->next;
    }

    //Menghapus pasien dari queue
    if (highestPriorityPatient == queue->front)
    {
        queue->front = queue->front->next;
        if (queue->front == NULL)
        {
            queue->rear = NULL;
        }
    }
    else{
        current = queue->front;
        while(current->next != highestPriorityPatient)
        {
            current = current->next;
        }
        current->next = highestPriorityPatient;
        if (highestPriorityPatient == queue->rear)
        {
            queue-> rear = current;
        }
    }

    highestPriorityPatient->next = NULL;
    return highestPriorityPatient;
}

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

// Fungsi untuk mencetak antrian aktif
void printActiveQueue(DoubleLinkedList *activeQueue)
{
    Patient *current = activeQueue->left;
    print("Antrian Pasien AKtif:\n");
    while (current != NULL)
    {
        printf("ID: %d, Nama: %s, Usia: %d, Jenis Kelamin: %s, Kepentingan: %s\n",
        current->id, current->name, current->age, current->gender, current->status);
        current = current->right;
    }
    if (activeQueue->left == NULL)
    {
        printf("Kosong.\n");
    }
}

//main programm
int main() {
    Queue waitingQueue ={NULL, NULL};
    DoubleLinkedList activeQueue = {NULL, NULL};
    SingleLinkedList history = {NULL};
    int choice, id, age, priority;
    char name[50], gender[10], status[20];

    while (1)
    {
        printf("\n ------ Menu ------\n");
        printf("1. Tambahkan pasien ke registrasi\n");
        printf("2. Pindahkan pasien ke antrian aktif\n");
        printf("3. Cetak antrian registrasi\n");
        printf("4. Cetak antrian pasien aktif\n");
        printf("5. Catat histori pasien setelah pemeriksaan\n");
        printf("6. Tampilkan riwayat pasien hari ini\n");
        printf("7. Cari pasien berdasarkan ID\n");
        printf("8. Keluar program\n");
        printf("9. Hapus semua data\n");
        printf("Pilih opsi: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Masukkan ID Pasien: ");
            scanf("%d, &id");
            printf("Masukkan nama pasien: ");
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Masukkan usia pasien: ");
            scanf("%d", &age);
            printf("Masukkan jenis kelamin pasien: ");
            scanf("%s", gender);
            printf("Masukkan kepentingan pasien (IGD, Kontrol, Konsultasi): ");
            scanf("%s", status);

            if (strcmp(status, "IGD")==0)
            {
                priority = 1;
            }
            else if (strcmp(status, "Kontrol")==0)
            {
                priority = 2;
            }
            else
            {
                priority = 3;
            }
            
            enqueue(&waitingQueue, createPatient(id, name, age, gender, status, priority));
            printf("Pasien berhasil ditambahkan ke antrian registrasi.\n");
            break;

        case 2:
            moveToActiveQueue(&waitingQueue, &activeQueue);
            break;

        case 3:
            printQueue(&waitingQueue);
            break;

        case 4:
            printActiveQueue(&activeQueue);
            break;

        case 5:
            moveToHistory(&activeQueue, &history);
            break;

        case 6:
            printHistory(&history);
            break;

        case 7:
            printf("Masukkan ID pasien yang ingin dicari: ");
            scanf("%d", &id);
            searchPatienById(&waitingQueue, &activeQueue, &history, id);
            break;

        case 8:
            exit(0);
            break;

        case 9:
            clearQueue(&waitingQueue);
            clearActiveQueue(&activeQueue);
            clearHistory(&history);
            break;
        default:
            printf("Opsi tidak valid!!\n");
        }
    }
    
    return 0;
}