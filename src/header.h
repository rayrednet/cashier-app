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
	size_t acquisitionPrice;
};

struct ReportReceipt {
	size_t soldItem;
	size_t income;
	size_t profit;
};

void line ();
void single_line ();
void header ();
void show_catalog(char* catalog, struct Product product);
void addNewProduct(char* catalog, struct Product product);
void addStock(FILE* cfp, struct Product product);
void updateStock (char* catalog, struct Product product);
void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc);
void remove_receipt ();
void fill_history (FILE* hfp, char* history, struct Receipt rc);
void buy_product(char* catalog, char* receipt, char* history, struct Product product, struct Receipt rc);
void print_receipt(char* receipt, struct Receipt rc);
size_t getCurrentCapital(char * catalog, struct Product product);
void report (char* _mindate, char* _maxDate, char * catalog, struct Product product, struct ReportReceipt _aggregate);
struct ReportReceipt history(char* _mindate, char* _maxDate, char * history, struct Receipt rc);

#endif
