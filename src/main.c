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

	user_dashboard:
	header ();
	setColor (LIGHTBLUE);
	printf ("User Dashboard\n");
	setColor (WHITE);
	single_line ();
	setColor (LIGHTCYAN);
    printf ("Please choose this option:\n");
    printf ("1. Admin\n");
    printf ("2. Customer\n");
	int choose;
	int menu = 1;
	while (menu) 
	{
		user_option:
		setColor (LIGHTMAGENTA);
		printf ("Enter option 1 or 2 = ");
		scanf ("%d", &choose);
		line ();

		if (choose == 1 ){
			admin_dashboard:
			header ();
			setColor (LIGHTBLUE);
			printf ("Admin Dashboard\n");
			setColor (WHITE);
			single_line ();
			printf ("What do you want to do?\n");
			printf ("1. Add new product\n");
			printf ("2. Add product stock\n");
			printf ("3. Show transaction history and report\n");
			printf ("4. Back\n");
			printf ("5. Exit\n");
			
			int choose1;
			setColor (LIGHTMAGENTA);
			printf ("Enter option 1, 2. 3 or 4 = ");
			scanf ("%d", &choose1);	
			
			if (choose1 == 1){
				setColor (WHITE);
				addNewProduct(argv[1], product);

				adding_option:
				setColor (LIGHTCYAN);
				printf ("\n\nDo you want to go back to the admin dashboard? [y/n]\n> ");
				setColor (WHITE);
				char cs_dash;
				scanf (" %c", &cs_dash);
					if (cs_dash == 'y' ){
						goto admin_dashboard;
					}

					else if (cs_dash == 'n'){
						setColor (LIGHTCYAN);
						printf ("\nGood bye! Thanks for visiting our shop.\n");
						break;
					}
				
					else {
						setColor (LIGHTRED);
						printf ("Invalid choice!\n");
						goto adding_option;
					}
			}

			else if (choose1 == 2){
				updateStock (argv[1], product);

				stock_option:
				setColor (LIGHTCYAN);
				printf ("\n\nDo you want to go back to the admin dashboard? [y/n]\n> ");
				setColor (WHITE);
				char cs_dash;
				scanf (" %c", &cs_dash);
					if (cs_dash == 'y' ){
						goto admin_dashboard;
					}

					else if (cs_dash == 'n'){
						setColor (LIGHTCYAN);
						printf ("\nGood bye! Thanks for visiting our shop.\n");
						break;
					}
				
					else {
						setColor (LIGHTRED);
						printf ("Invalid choice!\n");
						goto stock_option;
					}
			}

			else if (choose1 == 3){

			char _minDate [40];
			char _maxDate [40];
			setColor(LIGHTGREEN);
			printf ("Please input date range to see transaction report\n");
			printf ("Format input (YYYY MM DD HH:MM:SS) \n\n");

			setColor(WHITE);
			printf ("If you have finished, input -1 in the start date to go back to admin dashboard\n");
			
			setColor(LIGHTCYAN);
			printf ("Start date\n> ");
			setColor(WHITE);
			scanf(" %[^\n]", _minDate);

			if (strcmp( _minDate, "-1") == 0) {
				goto admin_dashboard;
			}

			setColor(LIGHTCYAN);
			printf ("End date\n> ");
			setColor(WHITE);	
			scanf(" %[^\n]", _maxDate);

				struct ReportReceipt _aggregate = history(_minDate, _maxDate, argv[3], rc);
				report (_minDate, _maxDate, argv[1], product, _aggregate);

				history_option:
				setColor (LIGHTCYAN);
				printf ("\n\nDo you want to go back to the admin dashboard? [y/n]\n> ");
				setColor (WHITE);
				char cs_dash;
				scanf (" %c", &cs_dash);
					if (cs_dash == 'y' ){
						goto admin_dashboard;
					}

					else if (cs_dash == 'n'){
						setColor (LIGHTCYAN);
						printf ("\nGood bye! Thanks for visiting our shop.\n");
						break;
					}
				
					else {
						setColor (LIGHTRED);
						printf ("Invalid choice!\n");
						goto history_option;
					}
			}

			else if (choose1 ==4){
				goto user_dashboard;
			}

			else if (choose1 == 5){
				setColor (LIGHTRED);
				printf ("Exiting...\n\n");
				menu = 0;
				break;
			}

			else {
			setColor (LIGHTRED);
			printf ("You entered the wrong option, please input only 1, 2, 3, 4 or 5\n");
			goto admin_dashboard;
			}
		}  

		else if (choose == 2){
			customer_dashboard:
			header ();
			setColor (LIGHTBLUE);
			printf ("Customer Dashboard\n");
			setColor (WHITE);
			printf ("What do you want to do?\n");
			printf ("1. Show product catalog\n");
			printf ("2. Buy product\n");
			printf ("3. Back\n");
			printf ("4. Exit\n");

			int choose2;
			setColor (LIGHTMAGENTA);
			printf ("Enter option 1, 2, 3 or 4 = ");
			scanf ("%d", &choose2);

			if (choose2 == 1){
				show_catalog(argv[1], product);
				catalog_option:
				setColor (LIGHTCYAN);
				printf ("\n\nDo you want to go back to the customer dashboard? [y/n]\n> ");
				setColor (WHITE);
				char cs_dash;
				scanf (" %c", &cs_dash);
					if (cs_dash == 'y' ){
						goto customer_dashboard;
					}

					else if (cs_dash == 'n'){
						setColor (LIGHTCYAN);
						printf ("\nGood bye! Thanks for visiting our shop.\n");
						break;
					}
				
					else {
						setColor (LIGHTRED);
						printf ("Invalid choice!\n");
						goto catalog_option;
					}
			}

			else if (choose2 == 2){
				buy_product(argv[1], argv[2], argv[3],  product, rc);
				print_receipt(argv[2], rc);

				buying_option:
				setColor (LIGHTCYAN);
				printf ("\n\nDo you want to go back to the customer dashboard? [y/n]\n> ");
				setColor (WHITE);
				char cs_dash;
				scanf (" %c", &cs_dash);
					if (cs_dash == 'y' ){
						goto customer_dashboard;
					}

					else if (cs_dash == 'n'){
						setColor (LIGHTCYAN);
						printf ("\nGood bye! Thanks for visiting our shop.\n");
						break;
					}
				
					else {
						setColor (LIGHTRED);
						printf ("Invalid choice!\n");
						goto buying_option;
					}
				break;
			}

			else if (choose2 == 3){
				goto user_dashboard;
			}

			else if (choose2 == 4){
				setColor (LIGHTRED);
				printf ("Exiting...\n\n");
				menu = 0;
				break;
			}

			else {
				setColor (LIGHTRED);
				printf ("You entered the wrong option, please input only 1, 2, 3 or 4\n");
				goto customer_dashboard;
			}
		}

		else {
			setColor (LIGHTRED);
			printf ("You entered the wrong option, please input only 1 or 2\n");
			goto user_option;
		}
	}
	return 0;
}