#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "products.h"

#define ADMIN_PASS "jmikec2004"

static int userChoice;  
static Product shoppingCart[100];
static int cartCount = 0;

int main() {
    do {
        printf("--------------------------------------------\n");
        printf("1. Korisnik\n");
        printf("2. Administrator\n");
        printf("3. Izlaz\n");
        printf("--------------------------------------------\n");
        printf("Unesite vas izbor: ");
        scanf("%d", &userChoice);
        while (getchar() != '\n');

        switch (userChoice) {
        case User:
            userMenu();
            break;
        case Admin: {
            char passInput[50];
            printf("--------------------------------------------\n");
            getStringInput("Unesite administratorsku lozinku: ", passInput, 1);
            if (strcmp(passInput, ADMIN_PASS) == 0) {
                adminMenu();
            }
            else {
                printf("--------------------------------------------\n");
                printf("Neispravna lozinka!\n");
            }
            break;
        }
        case Exit:
            printf("Izlaz iz programa...\n");
            break;
        default:
            printf("Nepostojeća opcija, unesite ponovno!\n");
        }
    } while (userChoice != Exit);

    return 0;
}

void userMenu() {
    int choice;
    do {
        printf("--------------------------------------------\n");
        printf("1. Pregledaj proizvode\n");
        printf("2. Dodaj proizvod u kosaricu\n");
        printf("3. Pregledaj kosaricu\n");
        printf("4. Povratak na glavni meni\n");
        printf("--------------------------------------------\n");
        printf("Unesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            listProducts();
            break;
        case 2:
            addToCart(shoppingCart, &cartCount);
            break;
        case 3:
            viewCart(shoppingCart, cartCount);
            break;
        case 4:
            break;
        default:
            printf("--------------------------------------------\n");
            printf("Nepostojeća opcija, unesite ponovno!\n");
        }
    } while (choice != 4);
}

void adminMenu() {
    int choice;
    Product prod;
    int prodID;

    do {
        displayMenu();
        printf("--------------------------------------------\n");
        printf("Unesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            printf("--------------------------------------------\n");
            prod.id = getIntInput("Unesite ID proizvoda: ");
            getStringInput("Unesite ime proizvoda: ", prod.name, 3);
            prod.price = getFloatInput("Unesite cijenu proizvoda: ");
            prod.quantity = getIntInput("Unesite kolicinu proizvoda: ");
            prod.instore = getIntInput("Unesite broj dostupnih primjeraka: ");
            printf("--------------------------------------------\n");
            addProduct(&prod);
            break;
        case 2:
            listProducts();
            break;
        case 3:
            printf("--------------------------------------------\n");
            prodID = getIntInput("Unesite ID proizvoda kojeg zelite azurirati: ");
            getStringInput("Unesite novo ime proizvoda: ", prod.name, 3);
            prod.price = getFloatInput("Unesite novu cijenu proizvoda: ");
            prod.quantity = getIntInput("Unesite novu kolicinu proizvoda: ");
            prod.instore = getIntInput("Unesite novu dostupnu kolicinu primjeraka: ");
            printf("--------------------------------------------\n");
            updateProduct(prodID, &prod);
            break;
        case 4:
            printf("--------------------------------------------\n");
            prodID = getIntInput("Unesite ID proizvoda kojeg zelite izbrisati: ");
            printf("--------------------------------------------\n");
            deleteProduct(prodID);
            break;
        case 5:
            printf("Izlaz iz administratorskog menija...\n");
            break;
        default:
            printf("Nepostojeća opcija, unesite ponovno!\n");
        }
    } while (choice != 5);
}

void addToCart(Product* cart, int* cartCount) {
    printf("--------------------------------------------\n");
    int id = getIntInput("Unesite ID proizvoda kojeg zelite dodati u kosaricu: ");
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }
    Product* found = recursiveProductSearch(file, id);
    if (found != NULL) {
        cart[*cartCount] = *found;
        (*cartCount)++;
        printf("--------------------------------------------\n");
        printf("Proizvod dodan u kosaricu.\n");
        free(found);
    }
    else {
        printf("--------------------------------------------\n");
        printf("Proizvod s ID-om %d nije pronaden.\n", id);
    }
    fclose(file);
}

Product* recursiveProductSearch(FILE* file, int targetID) {
    Product* prod = (Product*)malloc(sizeof(Product));
    if (!prod) {
        perror("Greska pri alokaciji memorije");
        exit(EXIT_FAILURE);
    }

    long pos = ftell(file);

    if (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &prod->id, prod->name, &prod->price, &prod->quantity, &prod->instore) == EOF) {
        free(prod);
        return NULL;
    }

    if (prod->id == targetID) {
        return prod;
    }
    else {
        Product* res = recursiveProductSearch(file, targetID);
        fseek(file, pos, SEEK_SET);
        free(prod);
        return res;
    }
}

void viewCart(Product* cart, int cartCount) {
    float total = 0;
    printf("\nKosarica:\n");
    for (int i = 0; i < cartCount; i++) {
        printf("ID: %d, Ime: %s, Cijena: %.2f Eur\n", cart[i].id, cart[i].name, cart[i].price);
        total += cart[i].price;
    }
    printf("Ukupna cijena: %.2f Eur\n", total);
}

int getIntInput(const char* prompt) {
    int val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &val) == 1) {
                return val;
            }
            else {
                printf("Neispravan unos. Molimo unesite broj.\n");
            }
        }
    }
}

float getFloatInput(const char* prompt) {
    float val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &val) == 1) {
                return val;
            }
            else {
                printf("Neispravan unos. Molimo unesite broj.\n");
            }
        }
    }
}

void getStringInput(const char* prompt, char* input, int minLen) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if ((int)strlen(buffer) >= minLen) {
                strcpy(input, buffer);
                return;
            }
            else {
                printf("Unos nije validan. Molimo unesite barem %d znakova.\n", minLen);
            }
        }
    }
}
