#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "products.h"

void addProduct(Product* product) {
    FILE* file = fopen("products.txt", "a");
    if (!file) {
        perror("Greska pri otvaranju datoteke za dodavanje");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d,%s,%.2f,%d,%d\n", product->id, product->name, product->price, product->quantity, product->instore);
    fclose(file);
}

void listProducts() {
    FILE* file = fopen("products.txt", "r");
    if (!file) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }
    Product product;
    printf("--------------------------------------------\n");
    printf("Lista proizvoda:\n");
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        printf("ID: %d, Ime: %s, Cijena: %.2f, Kolicina: %d, Na skladistu: %d\n", product.id, product.name, product.price, product.quantity, product.instore);
    }
    fclose(file);
}

void updateProduct(int id, Product* updatedProduct) {
    FILE* file = fopen("products.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        perror("Greska pri otvaranju datoteke");
        exit(EXIT_FAILURE);
    }
    Product product;
    int found = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        if (product.id == id) {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", updatedProduct->id, updatedProduct->name, updatedProduct->price, updatedProduct->quantity, updatedProduct->instore);
            found = 1;
        }
        else {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (!found) {
        printf("Proizvod s ID-om %d nije pronaden.\n", id);
    }
}

void deleteProduct(int id) {
    FILE* file = fopen("products.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        perror("Greska pri otvaranju datoteke");
        exit(EXIT_FAILURE);
    }
    Product product;
    int found = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        if (product.id != id) {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
        }
        else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (!found) {
        printf("Proizvod s ID-om %d nije pronaden.\n", id);
    }
}

void displayMenu() {
    printf("--------------------------------------------\n");
    printf("1. Dodaj proizvod\n");
    printf("2. Lista proizvoda\n");
    printf("3. Azuriraj proizvod\n");
    printf("4. Izbrisi proizvod\n");
    printf("5. Povratak na glavni meni\n");
}

int compareProductsByID(const void* a, const void* b) {
    const Product* pA = (const Product*)a;
    const Product* pB = (const Product*)b;
    return pA->id - pB->id;
}

void sortProductsByID(Product* products, size_t count) {
    qsort(products, count, sizeof(Product), compareProductsByID);
}
