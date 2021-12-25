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

void addNewStock (char* catalog, struct Product product) {
	FILE* cfp  = fopen(catalog, "rb+");
	header ();
	setColor (LIGHTBLUE);
	printf ("Add Product Stock\n");
	setColor (WHITE);
	line ();

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
		while(fread(&product, sizeof(struct Product), 1, cfp)> 0){
			if(tempID ==  product.id){
				setColor (LIGHTGREEN);
				printf ("Input how many stock that you want to add:\n> ");
				setColor (WHITE);
				size_t tempStock;
				scanf ("%zu", &tempStock);

				product.stock += tempStock;
				fwrite(&product, sizeof(struct Product), 1, cfp);

				setColor (LIGHTCYAN);
				printf ("%zu of %s with ID %zu successfully added to the catalog\n", tempStock, product.name, product.id);
				fclose(cfp);
				break;
			}
		}	

		fclose (cfp);
	}

}

void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc) {
	fwrite(&rc, sizeof(struct Receipt), 1, rfp);
	freopen(receipt, "ab", rfp);
}

void buy_product(char* catalog, char* receipt, struct Product product, struct Receipt rc) {
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

	int id;
	size_t items;
	size_t payout = 0;

	header();
	setColor (LIGHTBLUE);
	printf ("Buy Product\n");
	setColor(WHITE);
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

		printf("\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");
		printf("%-7zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);

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
			setColor(LIGHTGREEN);
			printf("%zu of %s successfully added to your cart with price %zu per unit.\n\n", items, product.name, product.price);

			product.stock -= items;

			addStock(cfp, product);

			payout += product.price * items;

			rc.id = id;
			strncpy(rc.name, product.name, 32);
			rc.totalPrice = payout;

			payout = 0;
			fill_receipt(rfp, receipt, rc);

		} else {
			setColor(LIGHTRED);
			printf("Purchase dismissed\n\n");
		}
	}

	fclose(cfp);
	fclose(rfp);
	printf("\n");
}

void print_receipt(char* receipt, struct Receipt rc) {
	static size_t printNum = 0;
	printf("\n    receipt:%s\n", receipt);

	FILE* rfp = fopen(receipt, "rb");
	if (rfp == NULL) {
		perror(receipt);
		exit(-2);
	}

	header ();
	printf ("                            CASH RECEIPT\n");
	line ();
	
	time_t t;
	time(&t);
	// time(rc.trxTime);

	setColor(LIGHTMAGENTA);
	printf("Date and time = %s\n", ctime(&t));
	printf("Receipt ID: %zu\n", printNum);
	line ();
	printNum++;
	setColor(LIGHTCYAN);
	printf("\n%-7s%-33s%10s\n", "ID", "Product Name", "Price");
	setColor(WHITE);
	size_t total = 0;
	size_t ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	while (ret) {
		printf("%-7zu%-33s%10zu\n", rc.id, rc.name, rc.totalPrice);
		total += rc.totalPrice;
		ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	}

	fclose(rfp);

	setColor(LIGHTGREEN);
	printf("--------------------------------------------------\nTOTAL: ");
	setColor(WHITE);
	printf("%43zu\n", total);
	
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
}

void report (){
	setColor (LIGHTCYAN);
	printf ("Capital	= ");
	printf ("Items sold = ");
	printf ("Income		= ");

	printf ("Profit		= ");
}

void history (char* history, struct Product product, struct Receipt rc, struct Data data)
{
	FILE* cfp = fopen (history, "rb");
	if (cfp == NULL){
		perror (history);
		exit (-3);
	}

	header ();
	setColor (LIGHTBLUE);
	printf ("Transaction Report\n\n");

	setColor(WHITE);
	printf ("If you have finished, input -1 in the start date\n");

	int minDay, minMonth, minYear, maxDay, maxMonth, maxYear;
	while (1)
	{
		setColor(LIGHTGREEN);
		printf ("Please input date range to see transaction report\n");
			setColor(LIGHTCYAN);
			printf ("Start date\n> ");
				setColor(WHITE);
				scanf ("%d %d %d", &minDay, &minMonth, &minYear);
					if (minDay == -1) break;
		setColor(LIGHTCYAN);
		printf ("End date\n> ");
			setColor(WHITE);
			scanf ("%d %d %d", &maxDay, &maxMonth, &maxYear);	
		
		setColor (LIGHTMAGENTA);
		printf( "\n%-6s%-26s%-10s%-11s%-10s%-10s\n","ID", "Product", "Amount", "Price", "Total", "Time" );

		while (!feof( cfp ) ) 
		{ 
				if(fread( &data, sizeof( struct Data ), 1, cfp )){
					if ( rc.id != -1 ) {
						if(data.day >= minDay && data.day <= maxDay && data.month >= minMonth && data.month <= maxMonth && data.year >= minYear && data.year <= maxYear){
						setColor (WHITE);
						printf("%-6zu%-26s%-10zu%-11zu%-10zu%-10s\n", product.id, product.name, product.items, product.price, rc.totalPrice, data.time);
						}

						}
	}
		report ();
		}
	}

	fclose (cfp);
}