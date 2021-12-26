#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stddef.h>

struct Product {
	size_t id;
	char name[40];
	size_t purchasePricePerUnit;
	size_t stock;
	size_t purchasePrice;
	size_t price;
	size_t items;
};

struct Receipt {
	size_t id;
	size_t productID;
	size_t totalPrice;
	char name[40];
	size_t cash;
	size_t change;
	char  trxTime [26];
	size_t amount;
};

struct Data {
	int day;
	int month;
	int year;
	char time [50];
};

void show_catalog(char* catalog, struct Product product);
void modify_catalog(char* catalog, struct Product product);
void update_catalog(FILE* cfp, struct Product product);
void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc);
void buy_product(char* catalog, char* receipt, char* history, struct Product product, struct Receipt rc);
void print_receipt(char* receipt, struct Receipt rc);
void history(char * history, struct Receipt rc);
#endif
