#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// Struktur data
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
} Patient;

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

// Deklarasi Fungsi
void enqueue(Queue *queue, Patient *patient);
void moveToActiveQueue(Queue *queue, DoubleLinkedList *activeQueue);
void moveToHistory(DoubleLinkedList *activeQueue, SingleLinkedList *history);
void printQueue(Queue *queue);
void printActiveQueue(DoubleLinkedList *activeQueue);
void printHistory(SingleLinkedList *history);
void searchPatientById(Queue *queue, DoubleLinkedList *activeQueue, SingleLinkedList *history, int id);
void clearQueue(Queue *queue);
void clearActiveQueue(DoubleLinkedList *activeQueue);
void clearHistory(SingleLinkedList *history);
int isNumber(const char *str);
Patient *createPatient(int id, const char *name, int age, const char *gender, const char *status, int priority);

// Main programm
int main()
{
    Queue waitingQueue = {NULL, NULL};
    DoubleLinkedList activeQueue = {NULL, NULL};
    SingleLinkedList history = {NULL};
    int choice;
    char choiceInput[10];
    char id[10], age[10];
    char name[50], gender[10], status[20];

    while (1)
    {
        system("cls");
        printf("\n ------ Menu ------\n");
        printf("1. Tambahkan pasien ke registrasi\n");
        printf("2. Pindahkan pasien ke antrian aktif\n");
        printf("3. Cetak antrian registrasi\n");
        printf("4. Cetak antrian pasien aktif\n");
        printf("5. Catat histori pasien setelah pemeriksaan\n");
        printf("6. Tampilkan riwayat pasien hari ini\n");
        printf("7. Cari pasien berdasarkan ID\n");
        printf("8. Hapus semua data\n");
        printf("9. Keluar program\n");
        printf("Pilih opsi: ");
        scanf("%s", choiceInput);

        if (!isNumber(choiceInput))
        {
            printf("Input tidak valid! Harap masukkan angka.\n");
            continue;
        }

        choice = atoi(choiceInput);

        switch (choice)
        {
        case 1:
            printf("Masukkan ID pasien: ");
            scanf("%s", id);
            if (!isNumber(id))
            {
                printf("Input tidak valid! Harap masukkan angka untuk ID.\n");
                getch();
                break;
            }

            printf("Masukkan nama pasien: ");
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            printf("Masukkan usia pasien: ");
            scanf("%s", age);
            if (!isNumber(age))
            {
                printf("Input tidak valid! Harap masukkan angka untuk usia.\n");
                break;
            }

            printf("Masukkan jenis kelamin pasien (Pria/Wanita): ");
            scanf("%s", gender);
            printf("Masukkan kepentingan pasien (IGD, Kontrol, Konsultasi): ");
            scanf("%s", status);

            int priority;
            if (strcmp(status, "IGD") == 0)
            {
                priority = 1;
            }
            else if (strcmp(status, "Kontrol") == 0)
            {
                priority = 2;
            }
            else
            {
                priority = 3;
            }

            enqueue(&waitingQueue, createPatient(atoi(id), name, atoi(age), gender, status, priority));
            printf("Pasien %s dengan ID Pasien: %d berhasil ditambahkan ke antrian registrasi.\n");
            break;

        case 2:
            moveToActiveQueue(&waitingQueue, &activeQueue);
            getch();
            break;

        case 3:
            printQueue(&waitingQueue);
            getch();
            break;

        case 4:
            printActiveQueue(&activeQueue);
            getch();
            break;

        case 5:
            moveToHistory(&activeQueue, &history);
            getch();
            break;

        case 6:
            printHistory(&history);
            getch();
            break;

        case 7:
            printf("Masukkan ID pasien yang ingin dicari: ");
            scanf("%s", id);

            if (!isNumber(id))
            {
                printf("Input tidak valid! Harap masukkan angka untuk ID Pasien.\n");
                break;
            }

            searchPatientById(&waitingQueue, &activeQueue, &history, atoi(id));
            getch();
            break;

        case 8:
            clearQueue(&waitingQueue);
            clearActiveQueue(&activeQueue);
            clearHistory(&history);
            getch();
            break;

        case 9:
            exit(0);
            getch();
            break;
        default:
            printf("Opsi tidak valid!!\n");
        }
    }

    return 0;
}

// Fungsi untuk mengecek apakah input hanya angka
int isNumber(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0; // Bukan angka
        }
    }
    return 1; // Angka valid
}

// Fungsi untuk membuat pasien baru
Patient *createPatient(int id, const char *name, int age, const char *gender, const char *status, int priority)
{
    Patient *newPatient = (Patient *)malloc(sizeof(Patient));
    newPatient->id = id;
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->gender, gender);
    strcpy(newPatient->status, status);
    newPatient->priority = priority;
    newPatient->next = NULL;
    newPatient->left = NULL;
    newPatient->right = NULL;
    return newPatient;
}

// Fungsi untuk menabahkan pasien ke Queue (Registrasi)
void enqueue(Queue *queue, Patient *patient)
{
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = patient;
    }
    else
    {
        queue->rear->next = patient;
        queue->rear = patient;
    }
}

// Fungsi untuk menemukan pasien dengan prioritas tertinggi dalam antrian registrasi
Patient *findHighestPriorityPatient(Queue *queue)
{
    if (queue->front == NULL)
        return NULL;
    Patient *current = queue->front;
    Patient *highestPriorityPatient = current;

    // Mencari pasien dengan prioritas tertinggi
    while (current != NULL)
    {
        if (current->priority < highestPriorityPatient->priority)
        {
            highestPriorityPatient = current;
        }
        current = current->next;
    }

    // Menghapus pasien dari queue
    if (highestPriorityPatient == queue->front)
    {
        queue->front = queue->front->next;
        if (queue->front == NULL)
        {
            queue->rear = NULL;
        }
    }
    else
    {
        current = queue->front;
        while (current->next != highestPriorityPatient)
        {
            current = current->next;
        }
        current->next = highestPriorityPatient->next;
        if (highestPriorityPatient == queue->rear)
        {
            queue->rear = current;
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

    printf("Pasien %s dengan ID Pasien: %d berhasil dipindahkan ke antrian aktif.\n", nextPatient->name, nextPatient->id);
}

// Fungsi untuk memindahkan pasien dari antrian aktif ke histori
void moveToHistory(DoubleLinkedList *activeQueue, SingleLinkedList *history)
{
    if (activeQueue->left == NULL)
    {
        printf("Tidak ada pasien dalam antrian aktif.\n");
        return;
    }

    Patient *servedPatient = activeQueue->left;
    activeQueue->left = activeQueue->left->right;
    if (activeQueue->left != NULL)
    {
        activeQueue->left->left = NULL;
    }
    else
    {
        activeQueue->right = NULL;
    }

    servedPatient->right = NULL;
    servedPatient->left = NULL;

    // Tambahkan diagnosis dan tindakan
    printf("Masukkan diagnosis untuk pasien %s dengan ID Pasien: %d: ", servedPatient->name, servedPatient->id);
    getchar();
    fgets(servedPatient->diagnosis, sizeof(servedPatient->diagnosis), stdin);
    servedPatient->diagnosis[strcspn(servedPatient->diagnosis, "\n")] = '\0';

    printf("Masukkan tindakan untuk pasien %s dengan ID Pasien: %d: ", servedPatient->name, servedPatient->id);
    fgets(servedPatient->treatment, sizeof(servedPatient->treatment), stdin);
    servedPatient->treatment[strcspn(servedPatient->treatment, "\n")] = '\0';

    // Tambahkan ke histori
    servedPatient->next = history->head;
    history->head = servedPatient;

    printf("Pasien %s dengan ID Pasien: %d berhasil ditambahkan ke histori.\n", servedPatient->name, servedPatient->id);
}
// Fungsi untuk mencetak antrian registrasi
void printQueue(Queue *queue)
{
    Patient *current = queue->front;
    
    int console_width = 80;
    char title[] = "Antrian Registrasi";
    int title_len = strlen(title);
    int padding = (console_width - title_len) / 2;

    printf("%*s%s\n", padding, "", title);

    printf("=========================================================================================\n");
    printf("| ID Pasien  | Nama                   | Usia  | Jenis Kelamin     | Kepentingan         |\n");
    printf("=========================================================================================\n");

    while (current != NULL)
    {
        printf("| %-10d | %-22s | %-5d | %-17s | %-19s |\n", current->id, current->name, current->age, current->gender, current->status);
        current = current->next;
    }

    if (queue->front == NULL)
    {
        printf("| %-10s | %-22s | %-5s | %-17s | %-19s |\n", "Kosong", "", "", "", "");    }
    
    printf("=========================================================================================\n");
}

// Fungsi untuk mencetak antrian aktif
void printActiveQueue(DoubleLinkedList *activeQueue)
{
    Patient *current = activeQueue->left;

    int console_width = 80;
    char title[] = "Antrian Pasien Aktif";
    int title_len = strlen(title);
    int padding = (console_width - title_len) / 2;

    printf("%*s%s\n", padding, "", title);

    printf("=========================================================================================\n");
    printf("| ID Pasien  | Nama                   | Usia  | Jenis Kelamin     | Kepentingan         |\n");
    printf("=========================================================================================\n");

    while (current != NULL)
    {
        printf("| %-10d | %-22s | %-5d | %-17s | %-19s |\n", 
            current->id, current->name, current->age, current->gender, current->status);
        current = current->right;
    }
    if (activeQueue->left == NULL)
    {
        printf("| %-10s | %-22s | %-5s | %-17s | %-19s |\n", "Kosong", "", "", "", "");
    }
    printf("=========================================================================================\n");

}

// Fungsi untuk mencetak histori pasien
void printHistory(SingleLinkedList *history)
{
    // Hitung jumlah pasien dlm riwayat
    Patient *current = history->head;
    int count = 0;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // Cetak riwayat pasien dari yang pertama diperiksa
    printf("Riwayat Pasien Hari Ini:\n");
    for (int i = count; i > 0; i--)
    {
        current = history->head;
        for (int j = 1; j < i; j++)
        {
            current = current->next;
        }
        printf("ID Pasien: %d, Nama: %s, Usia: %d, Jenis Kelamin: %s, Kepentingan: %s\n",
               current->id, current->name, current->age, current->gender, current->status);
        printf("Diagnosis: %s\n", current->diagnosis);
        printf("Tindakan: %s\n", current->treatment);
        printf("\n");
    }

    if (history->head == NULL)
    {
        printf("Kosong.\n");
    }
}

// Fungsi untuk mencari pasien berdasarkan ID dalam antrian (Queue, ActiveQueue, History)
void searchPatientById(Queue *queue, DoubleLinkedList *activeQueue, SingleLinkedList *history, int id)
{
    // Mnecari di antrian registrasi (queue)
    Patient *current = queue->front;
    while (current != NULL)
    {
        if (current->id == id)
        {
            printf("Pasien ditemukan diantrian registrasi:\n");
            printf("ID Pasien: %d, Nama: %s, Usia: %d, Jenis Kelamin: %s, Kepentingan: %s\n",
                   current->id, current->name, current->age, current->gender, current->status);
            return;
        }
        current = current->next;
    }

    // Mencari diantrian aktif (double linkedlist)
    current = activeQueue->left;
    while (current != NULL)
    {
        if (current->id == id)
        {
            printf("Pasien ditemukan diantrian aktif:\n");
            printf("ID Pasien: %d, Nama: %s, Usia: %d, Jenis Kelamin: %s, Kepentingan: %s\n",
                   current->id, current->name, current->age, current->gender, current->status);
            return;
        }
        current = current->right;
    }

    // Mencari di riwayat pasien (single linkedlist)
    current = history->head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            printf("Pasien ditemukan di riwayat pasien:\n");
            printf("ID Pasien: %d, Nama: %s, Usia: %d, Jenis Kelamin: %s, Kepentingan: %s\n",
                   current->id, current->name, current->age, current->gender, current->status);
            printf("Diagosis: %s\n", current->diagnosis);
            printf("Tindakan: %s\n", current->treatment);
            return;
        }
        current = current->next;
    }

    printf("Pasien dengan ID %d tidak ditemukan.\n", id);
}

// Fungsi untuk menghapus semua pasien dari Queue
void clearQueue(Queue *queue)
{
    Patient *current = queue->front;
    Patient *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    queue->front = queue->rear = NULL;
    printf("Semua data di antrian registrasi telah dihapus.\n");
}

// Fungsi untuk menghapus semua pasien dari Double Linked List (Antrian Aktif)
void clearActiveQueue(DoubleLinkedList *activeQueue)
{
    Patient *current = activeQueue->left;
    Patient *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->right;
        free(temp);
    }
    activeQueue->left = activeQueue->right = NULL;
    printf("Semua data di antrian pasien aktif telah dihapus.\n");
}

// Fungsi untuk menghapus semua pasien dari Single Linked List (Riwayat)
void clearHistory(SingleLinkedList *history)
{
    Patient *current = history->head;
    Patient *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    history->head = NULL;
    printf("Semua data di riwayat pasien telah dihapus.\n");
}
