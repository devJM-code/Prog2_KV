#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    int instore;
} Product;

void addProduct(Product* product);
void listProducts();
void updateProduct(int id, Product* updatedProduct);
void deleteProduct(int id);
void displayMenu();
void sortProductsByID(Product* products, size_t count);
Product* recursiveProductSearch(FILE* file, int targetID);

void userMenu();
void adminMenu();
void addToCart(Product* cart, int* cartCount);
void viewCart(Product* cart, int cartCount);
int getIntInput(const char* prompt);
float getFloatInput(const char* prompt);
void getStringInput(const char* prompt, char* input, int minLen);

enum MenuOption {
    User = 1,
    Admin,
    Exit
};

