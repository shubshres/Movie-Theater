//Shubhayu Shrestha 1001724804

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MovieTheaterLib.h"
#include "QueueLib.h"
#include "BSTLib.h"
#include "ListLib.h"
#include "StackLib.h"

void PrintReceipts(SNODE **StackTop)
{

	/* 
	   if the StackTop is empty, the print appropriate message - see example output - 
	   else print out the receipts.  A receipt is one node of the stack and each receipt
	   consists of a receipt number, a movie theater name and a TicketList (a linked list
	   of all seats sold to one customer).  Use ReturnAndFreeLinkedListNode() to traverse 
	   the linked list stored in each stack node and display the seats and free the 
	   linked list nodes. Each call to ReturnAndFreeLinkedListNode() brings back ONE ticket 
	   from the linked list in the Ticket variable.  Call pop() to remove the stack node.
	*/

	char ticket[10]; 
	
	if(StackTop == NULL)
	{
		printf("There are no receipts\n\n"); 
	}
	else
	{
		//use a while loop
		while(*StackTop != NULL)
		{
			//printing out the receipt number
			printf("Receipt Number: %d\n", (*StackTop)->ReceiptNumber); 

			//printing out the theater name
			printf("Theatre Name: %s\n", (*StackTop)->MovieTheaterName);
			
			//second while loop to go through the link list and print out the seats in order
			while((*StackTop)->TicketList != NULL)
			{
				ReturnAndFreeLinkedListNode(&((*StackTop)->TicketList), ticket); 
				printf("Seat: %s", ticket); 
			} 

			//pop the node
			pop(StackTop);
		}
	}
}

BNODE *PickAndDisplayTheater(BNODE *BSTRoot, char MovieTheaterMap[][MAXCOLS], int *MapRow, int *MapCol)
{
	BNODE *MyTheater = NULL;
	char zip[6] = {};
	char buffer[6] = {};

	//creating variables for row and column for when we utilize strcpy
	char row[3] = {}; 
	char col[3] = {};

	int rowInt; 
	int colInt; 
	char *token = NULL; 


	printf("\n\nPick a theater by entering the zipcode\n\n");
				
	// Traverse the BST in order and print out the theaters in zip code order - use InOrder()			
	InOrder(BSTRoot);

	// Prompt for a zip
	printf("Enter a zip code: ");
	fgets(zip, 6, stdin); 
	printf("\n"); 


	// Call SearchForBNODE()
	MyTheater = SearchForBNODE(BSTRoot, zip);

	// If theater is not found, then print message
	if(MyTheater == NULL)
	{
		printf("The theater could not be found from the zipcode provided"); 
	}
	// If theater is found, then tokenize the theater's dimensions and

	if(MyTheater != NULL)
	{
		
		strcpy(buffer, (MyTheater->Dimensions));
		
		token = strtok(buffer, "x");
		strcpy(row, token); 
		rowInt = atoi(row);
		
		token = strtok(NULL, "x");
		strcpy(col, token);
		colInt = atoi(col);  

		*MapRow = rowInt; 
		*MapCol = colInt; 

		int resultFile = ReadMovieTheaterFile(MovieTheaterMap, MyTheater->FileName, *MapRow, *MapCol);

		if(resultFile==0)
		{
			printf("Unable to print that seat map at this time.  Check back later.\n\n");
		}
		else
		{
			PrintSeatMap(MovieTheaterMap, *MapRow, *MapCol);
		}
	}

	// pass those dimensions and the MovieTheaterMap and the theater's filename
	// to ReadMovieTheaterFile()

	// If ReadMovieTheaterFile() returns FALSE, then print
	// "Unable to print that seat map at this time.  Check back later."
	// else call PrintSeatMap()

	


	// return the found theater

	return MyTheater; 

}	

void ReadFileIntoQueue(FILE *QueueFile, QNODE **QH, QNODE **QT)
{
	//	read the passed in file and calls enQueue for each name in the file

	char FileLine[200];
	
	while(fgets(FileLine, sizeof(FileLine)-1, QueueFile) != NULL)
	{
		enQueue(FileLine, QH, QT);
	}
}

void ReadFileIntoBST(FILE *BSTFile, BNODE **BSTnode)
{
	//	read the passed in file and tokenize each line and pass the information to AddBSTNode
	char FileLine[200];
	char *token = NULL; 
	char *tokenDimensions = NULL; 

	//creating temporary variables to hold each value
	char theaterName[40] = {};
	char zipcode[6]= {};
	char filename[200]= {};
	char dimensions[10] = {}; 

	

	while(fgets(FileLine, sizeof(FileLine)-1, BSTFile) != NULL)
	{
		token = strtok(FileLine, "|");
		strcpy(theaterName, token);

		token = strtok(NULL, "|");
		strcpy(zipcode, token);

		token = strtok(NULL, "|");
		strcpy(filename, token);

		token = strtok(NULL, "|");
		tokenDimensions = strtok(token,"\0");
		strcpy(dimensions, tokenDimensions);

		AddBSTNode(BSTnode, theaterName, zipcode, filename, dimensions); 
	}
}

void get_command_line_parameter(char *argv[], char ParamName[], char ParamValue[])
{
	int i = 0; 
	
	while (argv[++i] != NULL)
	{
		if (!strncmp(argv[i], ParamName, strlen(ParamName)))
		{
			strcpy(ParamValue, strchr(argv[i], '=') + 1);
		}
	}
}

int PrintMenu()
{
	int choice = 0;
	
	printf("\n\n1.	Sell tickets to next customer\n\n");
	printf("2.	See who's in line\n\n");
	printf("3.	See the seat map for a given theater\n\n");
	printf("4.	Print today's receipts\n\n");
	printf("Choice : ");
	scanf("%d", &choice);
	getchar(); 
	
	while (choice < 1 || choice > 4)
	{
		printf("Invalid choice.  Please reenter. ");
		scanf("%d", &choice);
		getchar(); 
	}
	
	return choice;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	FILE *queueFile = NULL;
	FILE *zipFile = NULL;
	char arg_value[20];
	char queuefilename[20];
	char zipfilename[20];
	int ReceiptNumber = 0;
	int choice = 0;
	int SeatNumber;
	char Row;
	char Ticket[5];
	int ArrayRow, ArrayCol;
	int MapRow, MapCol;
	char MovieTheaterMap[MAXROWS][MAXCOLS] = {};
	LNODE *TicketLinkedListHead = NULL;
	QNODE *QueueHead = NULL;
	QNODE *QueueTail = NULL;
	BNODE *BSTRoot = NULL;
	BNODE *MyTheater = NULL;
	SNODE *StackTop = NULL;
	int NumberOfTickets = 0;

	/*
    MyTheater = malloc(sizeof(BNODE));
    MyTheater->MovieTheaterName = malloc(13);
    strcpy(MyTheater->MovieTheaterName, "Test Theater");
    strcpy(MyTheater->ZipCode, "76109");
    MyTheater->FileName = malloc(sizeof("file1.txt"));
    strcpy(MyTheater->FileName, "file1.txt");
    MapRow = 5;
    MapCol = 5;
	

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			MovieTheaterMap[i][j] = 'O';
	*/
	
	if (argc != 4)
	{
		printf("%s QUEUE=xxxxxx ZIPFILE=xxxxx RECEIPTNUMBER=xxxxx\n", argv[0]);
		exit(0);
	}
	
	get_command_line_parameter(argv, "QUEUE=", queuefilename);
	get_command_line_parameter(argv, "ZIPFILE=", zipfilename);
	get_command_line_parameter(argv, "RECEIPTNUMBER=", arg_value);
	ReceiptNumber = atoi(arg_value);

	/* call function to open queuefilename - if it does not open, print message and exit */	
	queueFile = fopen(queuefilename, "r+");
	
	if(queueFile == NULL)
	{
		printf("\nQUEUEFILE could not be open\n");
		exit(0);  
	}


	/* call function to open zipfilename - if it does not open, print message and exit */
	zipFile = fopen(zipfilename, "r+");
	
	if(zipFile == NULL)
	{
		printf("\nZIPFILE could not be open\n");
		exit(0);  
	}

	ReadFileIntoQueue(queueFile, &QueueHead, &QueueTail);
	ReadFileIntoBST(zipFile, &BSTRoot);
	
	while (QueueHead != NULL)
	{
		choice = PrintMenu();
	
		switch (choice)
		{
			case 1 :
				printf("\n\nHello %s\n", QueueHead->name);	
				PrintSeatMap(MovieTheaterMap, MapRow, MapCol);			
				MyTheater = PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				if (MyTheater != NULL)
				{
					printf("\n\nHow many movie tickets do you want to buy? ");
					scanf("%d", &NumberOfTickets);
					for (i = 0; i < NumberOfTickets; i++)
					{
						do
						{
							printf("\nPick a seat (Row Seat) ");
							scanf(" %c%d", &Row, &SeatNumber);
							Row = toupper(Row);
							ArrayRow = (int)Row - 65;
							ArrayCol = SeatNumber - 1;
						
							if ((ArrayRow < 0 || ArrayRow >= MapRow) ||
								(ArrayCol < 0 || ArrayCol >= MapCol))
							{
								printf("\nThat is not a valid seat.  Please choose again\n\n");
							}		
						}
						while ((ArrayRow < 0 || ArrayRow >= MapRow) ||
							   (ArrayCol < 0 || ArrayCol >= MapCol));
						
						if (MovieTheaterMap[ArrayRow][ArrayCol] == 'O')
						{	
							MovieTheaterMap[ArrayRow][ArrayCol] = 'X';
							sprintf(Ticket, "%c%d", Row, SeatNumber); 
							InsertNode(&TicketLinkedListHead, Ticket);
						}
						else
						{
							printf("\nSeat %c%d is not available.\n\n", Row, SeatNumber);
							i--;
						}
						PrintSeatMap(MovieTheaterMap, MapRow, MapCol);
					}
					
					WriteSeatMap(MyTheater, MovieTheaterMap, MapRow, MapCol);
					push(&StackTop, TicketLinkedListHead, ReceiptNumber, MyTheater->MovieTheaterName);
					TicketLinkedListHead = NULL;
					ReceiptNumber++;
					printf("\nThank you %s - enjoy your movie!\n", QueueHead->name);
					deQueue(&QueueHead);
				}
				break;
			case 2 : 
				printf("\n\nCustomer Queue\n\n");
				DisplayQueue(QueueHead);
				printf("\n\n");
				break;
			case 3 :
				PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				break;
			case 4 : 
				PrintReceipts(&StackTop);
				break;
			default :
				printf("Bad menu choice");
		}
	}
	
	printf("Good job - you sold tickets to all of the customers in line.\n");
	PrintReceipts(&StackTop);
	
	return 0;
}
