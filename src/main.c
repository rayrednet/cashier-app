#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "rlutil.h"
#include "func.c"
#include "header.h"

int main(int argc, char* argv[argc + 1]) 
{
	if (argc != 4) 
	{
		fprintf(stderr, "Usage: %s <catalog_file> <receipt_file> <history_file>\n", argv[0]);
		return -1;
	}

	struct Product product = {0};
	struct Receipt rc = {0};
	struct Data data = {0};

	header ();
	setColor (LIGHTCYAN);
    printf ("Please choose this option:\n");
    printf ("1. Admin\n");
    printf ("2. Customer\n");
	int choose;
	int menu = 1;
	while (menu) 
	{
		setColor (LIGHTMAGENTA);
		printf ("Enter option 1 or 2 = ");
		scanf ("%d", &choose);
		line ();

		if (choose == 1 ){
			header ();
			setColor (LIGHTBLUE);
			printf ("Admin Dashboard\n");
			setColor (WHITE);
			printf ("What do you want to do?\n");
			printf ("1. Add new product\n");
			printf ("2. Add product stock\n");
			printf ("3. Show transaction report\n");
			printf ("4. Exit\n");
			int choose1;
			setColor (LIGHTMAGENTA);
			printf ("Enter option 1, 2. 3 or 4 = ");
			scanf ("%d", &choose1);	
			
			if (choose1 == 1){
				setColor (WHITE);
				addNewProduct(argv[1], product);
					break;
			}

			if (choose1 == 2){
				addNewStock (argv[1], product);
				break;
			}

			if (choose1 == 3){
				// history (argv[1], product, rc, data);
			}

			if (choose1 == 4){
				setColor (LIGHTRED);
				printf ("Exiting...\n\n");
				menu = 0;
				break;
			}

			else {
			setColor (LIGHTRED);
			printf ("You entered the wrong option, please input only 1, 2, 3 or 4\n");
			}
		}  

		if (choose = 2){
			header ();
			setColor (LIGHTBLUE);
			printf ("Customer Dashboard\n");
			setColor (WHITE);
			printf ("What do you want to do?\n");
			printf ("1. Show product catalog\n");
			printf ("2. Add product to cart\n");
			printf ("3. Exit\n");

			int choose2;
			setColor (LIGHTMAGENTA);
			printf ("Enter option 1, 2, 3 or 4 = ");
			scanf ("%d", &choose2);

			if (choose2 == 1){
				show_catalog(argv[1], product);
				break;
			}

			if (choose2 == 2){
				buy_product(argv[1], argv[3], product, rc);
				print_receipt(argv[3], rc);
				break;
			}

			if (choose2 == 3){
				setColor (LIGHTRED);
				printf ("Exiting...\n\n");
				menu = 0;
				break;
				break;
			}

			else {
				setColor (LIGHTRED);
				printf ("You entered the wrong option, please input only 1, 2 or 3\n");
			}
		}

		else {
			setColor (LIGHTRED);
			printf ("You entered the wrong option, please input only 1 or 2\n");
		}
	}
	return 0;
}