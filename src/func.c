#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "rlutil.h"
#include <time.h>

void line ()
{
    setColor (WHITE);
    printf ("========================================================================\n");
}

void single_line ()
{
	setColor (WHITE);
	printf ("------------------------------------------------------------------------\n");
}

void header ()
{
	line();
    setColor(YELLOW);
    printf("                           Emporium Store\n");
    setColor (WHITE);
	line();
}

void show_catalog(char* catalog, struct Product product) {
	FILE* cfp = fopen(catalog, "rb");
	if (cfp == NULL) {
		perror(catalog);
		exit(-2);
	}
	setColor (LIGHTBLUE);
	printf("\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");

	setColor (WHITE);
	size_t ret = fread(&product, sizeof(struct Product), 1, cfp);
	while (ret) {
		printf("%-7zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);
		ret = fread(&product, sizeof(struct Product), 1, cfp);
	}

	fclose(cfp);
	printf("\n");
}

void addNewProduct(char* catalog, struct Product product) {
	FILE* cfp = fopen(catalog, "rb+");
	if (cfp == NULL) {
		perror(catalog);
		exit(-3);
	}

	int id;

	header ();
	setColor (LIGHTBLUE);
		printf ("Add Product to Catalog\n");
		setColor (WHITE);
		single_line ();
	printf ("If you have finished adding new product, please enter -1 in the product ID\n\n");

	while (1) {
		setColor (LIGHTGREEN);
		printf("Insert product ID:\n> ");
			setColor (WHITE);
			scanf("%d", &id);

		if (id < 0) {
			break;
		}

		product.id = id;
		setColor (LIGHTGREEN);
		printf("Insert product name (max length 40):\n> ");
			setColor (WHITE);
			scanf(" %[^\n]", product.name);

		setColor (LIGHTGREEN);
		printf ("Insert product purchase price per item:\n> ");
			setColor (WHITE);
			printf ("Rp. ");
			scanf("%zu", &product.purchasePricePerUnit);

		setColor (LIGHTGREEN);
		printf("Insert product stock:\n> ");
			setColor (WHITE);
			scanf("%zu", &product.stock);

		setColor (LIGHTGREEN);
		printf ("Total product purchase price:\n> ");
			setColor (WHITE);
			printf ("Rp. ");
			product.purchasePrice = product.stock * product.purchasePricePerUnit;
			printf ("%zu\n", product.purchasePrice);

		size_t profit = (product.purchasePrice * 10)/100;
		setColor(LIGHTGREEN);
		printf ("Product selling price per item with 10%% profit:\n> ");
			setColor(WHITE);
			product.price = (profit + product.purchasePrice) / product.stock;
			setColor (WHITE);
			printf ("Rp. ");
			printf ("%zu\n", product.price);

		setColor(LIGHTCYAN);
		fseek (cfp, product.id * sizeof(struct Product), SEEK_SET);
		fwrite (&product, sizeof(struct Product), 1, cfp);

		printf ("%s has been successfully added with price per unit Rp. %zu\n\n", product.name, product.price);
	}

	fclose(cfp);
	printf("\n");
}

void addStock(FILE* cfp, struct Product product) {
	fseek(cfp, product.id * sizeof(struct Product), SEEK_SET);
	fwrite(&product, sizeof(struct Product), 1, cfp);
}

void updateStock (char* catalog, struct Product product) {
	FILE* cfp  = fopen(catalog, "rb+");
	header ();
	setColor (LIGHTBLUE);
	printf ("Add Product Stock\n");
	setColor (WHITE);
	single_line ();

	printf ("If you have finished adding product stock, enter -1 in the product's ID\n");
	
	while (1){
		setColor (LIGHTGREEN);
		printf ("Input product's ID that you want to add stock:\n> ");
		setColor (WHITE);
		int tempID;
		scanf ("%d", &tempID);

		if (tempID < 0){
			break;
		}

		fseek(cfp, tempID * sizeof(struct Product), SEEK_SET);

		int flag = 0;
		while(fread(&product, sizeof(struct Product), 1, cfp) > 0){
			if(tempID == product.id){
				setColor (LIGHTGREEN);
				printf ("Input how many stock that you want to add:\n> ");
				setColor (WHITE);
				size_t tempStock;
				scanf ("%zu", &tempStock);


				product.stock += tempStock;
				fseek (cfp, -sizeof (product), SEEK_CUR);
				fwrite(&product, sizeof(struct Product), 1, cfp);
				
				flag++;
				setColor (LIGHTCYAN);
				printf ("%zu of %s with ID %zu successfully added to the catalog\n", tempStock, product.name, product.id);
				fclose(cfp);
				break;

			}
				
				if (flag == 0)
				fclose (cfp);
		}	

	}
}

void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc) {
	fwrite(&rc, sizeof(struct Receipt), 1, rfp);
	freopen(receipt, "ab", rfp);
}

void removeReceipt(){
	system("rm receipt.txt");
	system("touch receipt.txt");

}

void fill_history(FILE* hfp, char* history, struct Receipt rc) {
	fwrite(&rc, sizeof(struct Receipt), 1, hfp);
	freopen(history, "ab", hfp);
}

void buy_product(char* catalog, char* receipt, char* history, struct Product product, struct Receipt rc) {
	FILE* cfp = fopen(catalog, "rb+");
	if (cfp == NULL) {
		perror(catalog);
		exit(-4);
	}

	FILE* rfp = fopen(receipt, "ab");
	if (rfp == NULL) {
		perror(receipt);
		exit(-5);
	}

	FILE* hfp = fopen(history, "ab");
	if (hfp == NULL) {
		perror(history);
		exit(-5);
	}

	//Generate transaction ID, could use current timestamp
	int _timestamp = (int) time(NULL);
	time_t _trxTime = time(0);
	char _timeStr [40];
	struct tm* _timeInfo = localtime(&_trxTime);
	strftime(_timeStr, 26, "%Y %m %d %H:%M:%S", _timeInfo);
	
	int id;
	size_t items;
	size_t payout = 0;

	header();
	setColor (LIGHTBLUE);
	printf ("Buy Product\n");
	setColor(WHITE);
	single_line ();
	printf("If you have finished buying product, please enter -1 in the product ID\n");

	while (1) {
		setColor(LIGHTGREEN);
		printf("Input product ID:\n> ");
			setColor(WHITE);
			scanf("%d", &id);

		if (id < 0) {
			break;
		}

		fseek(cfp, id * sizeof(struct Product), SEEK_SET);
		fread(&product, sizeof(struct Product), 1, cfp);

		setColor (LIGHTCYAN);
		printf("\n%-5s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");

		setColor(WHITE);
		printf("%-5zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);

		if (product.stock <= 0) {
			printf("Sorry, %s is currently out of stock\n\n", product.name);
			continue;
		}

		do {
			setColor(LIGHTGREEN);
			printf("Input how many %s that you want to buy:\n> ", product.name);
				setColor(WHITE);
				scanf("%zu", &items);

			if (items > product.stock) {
				printf("Sorry, you can't buy more than the available stock\n\n");
			}
		} while (items > product.stock);

		if (items != 0) {
			setColor(LIGHTBLUE);
			printf("%zu of %s successfully added to your cart with price %zu per unit.\n\n", items, product.name, product.price);

			product.stock -= items;

			addStock(cfp, product);

			payout += product.price * items;

			rc.productID = id;
			strncpy(rc.name, product.name, 32);
			rc.totalPrice = payout;
			rc.acquisitionPrice = product.purchasePricePerUnit;

			payout = 0;

			// set transaction id with _timestamp
			rc.id = _timestamp;
			strncpy(rc.trxTime, _timeStr, 19);
			rc.amount = items;

			fill_receipt(rfp, receipt, rc);
			fill_history(hfp, receipt, rc);

		} else {
			setColor(LIGHTRED);
			printf("Purchase dismissed\n\n");
		}
	}

	fclose(cfp);
	fclose(rfp);
	fclose(hfp);

	printf("\n");
}

void print_receipt(char* receipt, struct Receipt rc) {
	FILE* rfp = fopen(receipt, "rb");

	if (rfp == NULL) {
		perror(receipt);
		exit(-2);
	}

	size_t total = 0;
	size_t readReceipt = fread(&rc, sizeof(struct Receipt), 1, rfp);

	header ();
	printf ("                            CASH RECEIPT\n");
	line ();
	
	time_t t;
	time(&t);

	setColor(LIGHTMAGENTA);
	printf("Date and time [YYYY] [MM] [DD] [HH] [MM] [SS]\n> ");

	setColor (WHITE);
	printf ("%s\n", rc.trxTime);

	setColor (LIGHTMAGENTA);
	printf("Receipt ID\n> ");

	setColor(WHITE);
	printf ("%zu\n", rc.id);

	single_line ();

	setColor(LIGHTCYAN);
	printf("\n%-7s%-7s%-33s%10s%10s\n", "No", "ID", "Product Name", "Amount", "Price");
	
	setColor(WHITE);
	int _i =1;

	while (readReceipt) {
		printf("%-7d%-7zu%-33s%10zu%10zu\n", _i++, rc.productID, rc.name, rc.amount, rc.totalPrice);
		total += rc.totalPrice;
		readReceipt = fread(&rc, sizeof(struct Receipt), 1, rfp);
	}

	fclose(rfp);

	setColor(LIGHTGREEN);
	printf("------------------------------------------------------------------------\nTOTAL: ");
	setColor(WHITE);
	printf("%60zu\n", total);
	
	setColor(LIGHTGREEN);
	printf ("Cash   = Rp. ");
	setColor(WHITE);	
		scanf ("%zu", &rc.cash);
		if (rc.cash < total){
			printf ("Sorry your payment doesn't exceed for this transaction\n");
			exit(-4);
		}

		rc.change =  rc.cash - total;
		setColor(LIGHTGREEN);
		printf ("Change = Rp. ");
		setColor(WHITE);
		printf ("%zu", rc.change);
		
	printf("\n\n");
	setColor (LIGHTBLUE);
	printf ("Thank you for buying at Emporium Store. Have a nice day!\n");
	setColor (WHITE);
	
	line ();
	removeReceipt ();
}

size_t getCurrentInitialCapital (char* catalog, struct Product product) 
{
	FILE* cfp = fopen (catalog, "rb");

	if (cfp == NULL){
		perror(catalog);
		exit (-5);
	}
	size_t _capital = 0;

	while (fread (&product, sizeof(struct Product), 1, cfp)){
			size_t _stock = product.stock;
			size_t _price = product.purchasePricePerUnit;

			_capital += (_stock * _price) ;
	}
	fclose (cfp);
	return _capital;
}

void report (char* _minDate, char* _maxDate, char* catalog, struct Product product, struct ReportReceipt _report){
	setColor (LIGHTBLUE);
	printf ("\n\nInitial capital\n> ");
	setColor (WHITE);
	size_t _currentCapital = getCurrentInitialCapital(catalog, product);
	printf ("Rp. %zu\n", _currentCapital );

	setColor (LIGHTBLUE);
	printf ("\nItems sold\n> ");
	setColor (WHITE);
	printf ("%zu\n", _report.soldItem);

	setColor (LIGHTBLUE);
	printf ("\nIncome\n> ");
	setColor (WHITE);
	printf ("Rp. %zu\n", _report.income);

	setColor (LIGHTBLUE);
	printf ("\nProfit\n> ");
	setColor (WHITE);
	printf ("Rp. %zu\n", _report.profit);
}

struct ReportReceipt history (char* _minDate, char* _maxDate, char* history,  struct Receipt rc)
{	
	struct ReportReceipt result;
	int i=0;
	FILE* hfp = fopen(history, "rb");
	if (hfp == NULL){
		perror (history);
		exit (-4);
	}
	header ();
	setColor (LIGHTBLUE);
	printf ("Transaction history and report\n");
	setColor(WHITE);
	single_line ();
	setColor (LIGHTMAGENTA);
	printf( "\n%-13s%-30s%-10s%-11s%-20s\n","Receipt ID", "Product", "Amount", "Price", "Date and time" );

	int count = 0;

	while (fread(&rc, sizeof(struct Receipt), 1, hfp))
	{	
		if (strcmp(rc.trxTime, _minDate) >=0  && strcmp(rc.trxTime, _maxDate) <=0 ) 
		{
			setColor (WHITE);
			printf("%-13zu%-30s%-10zu%-11zu%-20s\n", rc.id, rc.name, rc.amount, rc.totalPrice, rc.trxTime);
			result.soldItem += rc.amount;
			result.income += rc.totalPrice;
			result.profit = result.income - rc.acquisitionPrice*rc.amount;
			count++;
		}
	}
	if (count == 0) {
		setColor (LIGHTRED);
		printf ("Sorry we couldn't find any matching transaction history\n");
	}
	fclose (hfp);

	return result ;
}