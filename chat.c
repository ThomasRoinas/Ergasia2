#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PRODUCTS 20
#define NUM_CLIENTS 5
#define ORDERS_PER_CLIENT 10

// Δομή προϊόντος
typedef struct {
    char description[50];
    float price;
    int item_count;
} Product;

Product catalog[NUM_PRODUCTS]; // Κατάλογος προϊόντων

// Αρχικοποίηση καταλόγου
void initialize_catalog() {
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        sprintf(catalog[i].description, "Product_%d", i);
        catalog[i].price = (float)(rand() % 100 + 1); // Τυχαία τιμή
        catalog[i].item_count = 2; // Διαθέσιμα κομμάτια
    }
}

// Πατρική διεργασία (eshop) - Επεξεργασία παραγγελιών
void eshop_process(int client_pipes[NUM_CLIENTS][2][2]) {
    char buffer[100];
    for (int i = 0; i < NUM_CLIENTS; i++) {
        close(client_pipes[i][0][1]); // Κλείσιμο write στο pipe πελάτη->eshop
        close(client_pipes[i][1][0]); // Κλείσιμο read στο pipe eshop->πελάτη
    }

    while (1) {
        for (int i = 0; i < NUM_CLIENTS; i++) {
            if (read(client_pipes[i][0][0], buffer, sizeof(buffer)) > 0) {
                int product_id = atoi(buffer);

                // Έλεγχος διαθεσιμότητας
                if (product_id >= 0 && product_id < NUM_PRODUCTS && catalog[product_id].item_count > 0) {
                    catalog[product_id].item_count--;
                    sprintf(buffer, "Order for Product_%d successful. Remaining: %d", product_id, catalog[product_id].item_count);
                } else {
                    sprintf(buffer, "Order for Product_%d failed. Out of stock.", product_id);
                }

                // Αποστολή αποτελέσματος στον πελάτη
                write(client_pipes[i][1][1], buffer, strlen(buffer) + 1);
                sleep(1); // Προσομοίωση χρόνου διεκπεραίωσης
            }
        }
    }
}

// Θυγατρική διεργασία (Πελάτης)
void client_process(int client_id, int pipes[2][2]) {
    close(pipes[0][0]); // Κλείσιμο read στο pipe πελάτη->eshop
    close(pipes[1][1]); // Κλείσιμο write στο pipe eshop->πελάτη

    char buffer[100];
    for (int i = 0; i < ORDERS_PER_CLIENT; i++) {
        int product_id = rand() % NUM_PRODUCTS;
        sprintf(buffer, "%d", product_id);

        // Αποστολή παραγγελίας
        write(pipes[0][1], buffer, strlen(buffer) + 1);

        // Αναμονή αποτελέσματος
        read(pipes[1][0], buffer, sizeof(buffer));
        printf("Client %d received: %s\n", client_id, buffer);
        sleep(1); // Αναμονή πριν την επόμενη παραγγελία
    }

    exit(0);
}

int main() {
    srand(time(NULL));
    initialize_catalog();

    int client_pipes[NUM_CLIENTS][2][2]; // [client_id][0=to_eshop/1=from_eshop][0=read/1=write]

    // Δημιουργία pipes για κάθε πελάτη
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pipe(client_pipes[i][0]); // Pipe πελάτη->eshop
        pipe(client_pipes[i][1]); // Pipe eshop->πελάτη
    }

    // Δημιουργία πελατών
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            client_process(i, client_pipes[i]);
        }
    }

    // Πατρική διεργασία
    eshop_process(client_pipes);

    // Αναμονή για όλες τις θυγατρικές
    for (int i = 0; i < NUM_CLIENTS; i++) {
        wait(NULL);
    }

    return 0;
}

