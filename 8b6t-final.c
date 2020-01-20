#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#define VAL_INDEX 6
#define W_INDEX 7
#define TOTAL_COMBINATION 256

int power(int x, int y){
	int i, res = 1;

	for(y; y > 0; y--)
		res *= x;

	return res;
}

void geNerate6t(int arr6t[]){
	int i;
	int sum = 0, weight = 0;

	for(i = 0; i < 6; i++){
		arr6t[i] = rand() % 3 - 1;
		weight = weight + arr6t[i];
		sum = sum + (power(3, 6 - i)) * (arr6t[i] + 1);
	}

	arr6t[VAL_INDEX] = sum;
	arr6t[W_INDEX] = weight;

	if(weight != 0 && weight != 1)
		geNerate6t(arr6t);
}

void generate_8b6t(int mappinG[][8], int iBytes){
	int i, j;
	int arr6t[8];

	for(i = 0; i < iBytes; i++){
		startAgain:
			geNerate6t(arr6t);

		if(i != 0)			
			for(j = 0; j < i; j++)
				if(mappinG[j][VAL_INDEX] == arr6t[VAL_INDEX])
					goto startAgain;

		for(j = 0; j < 8; j++)
			mappinG[i][j] = arr6t[j];
	}	
}

void fnDispSignalUnit(int signalUnit){
	if(signalUnit == -1)
		printf("-");
	else if(signalUnit == 0)
		printf("0");
	else	
		printf("+");
}

char* fnCheckInver(int check){
	if(!check)
		return "NO";
	else if(check == 1) 
		return "YES";
	return "NA";
}

void shOwmappinG(int map[][8], int iTotalBytes, int iInverted){
	int i, j;
	printf("-----------------------------------------------------------------\n");
	printf("|   Signal   |\t Value\t |   Data    |    Weight  |  Inverted   |\n");
	printf("-----------------------------------------------------------------\n");

	for(i = 0; i < iTotalBytes; i++){
		printf("|   ");
		for(j = 0; j < 6; j++)
			fnDispSignalUnit(map[i][j]);
		printf("   |\t  %d\t |    %-4d   |   %4d     |      %s     |", map[i][VAL_INDEX], i, map[i][W_INDEX], fnCheckInver(iInverted));  
		printf("\n");
	}
	printf("-----------------------------------------------------------------\n");
	printf("\n");
}

void fnShowSignalMapping(int iaSignal[][8], int iBytes){

	int i, j;
	int weight = 0;
	int iInversion = 0;
	
	system("cls");

	printf("----------------------------------\n");
	printf("|Signal\t| Data\t|Weight\t|Inverted|\n");
	printf("----------------------------------\n");
	
	for(i = 0; i < iBytes; i++){
		printf("|");
		if(weight == 1 && iaSignal[i][W_INDEX] == 1){
			for(j = 0; j < 6; j++)
				fnDispSignalUnit(iaSignal[i][j] * -1);
			weight = 0;
			iInversion = 1;
		}
		else{
			for(j = 0; j < 6; j++)
				fnDispSignalUnit(iaSignal[i][j]);
			weight += iaSignal[i][W_INDEX];
		}

		printf("\t|  %d\t|%4d\t|%6s\t |", iaSignal[i][VAL_INDEX], iaSignal[i][W_INDEX], fnCheckInver(iInversion));

		iInversion = 0;

		printf("\n");
	}

	printf("----------------------------------\n");
}

void fnDataToSignal(int ia8b61Mapping[][8], int iaSignal[][8], int iBytes){
	int i, j;

	for(i = 0; i < iBytes; i++){
		label3:
		printf("Enter the data byte %d in decimal:\t", i + 1);
		scanf("%d", &iaSignal[i][VAL_INDEX]);

		if(iaSignal[i][VAL_INDEX] < 0 || iaSignal[i][VAL_INDEX] > 255){
			printf("Invalid data. Try again");			
			goto label3;
		}

		for(j = 0; j < 6; j++){
			iaSignal[i][j] = ia8b61Mapping[iaSignal[i][VAL_INDEX]][j];
		}

		iaSignal[i][W_INDEX] = ia8b61Mapping[iaSignal[i][VAL_INDEX]][W_INDEX];
	}
}

void fnPlotSignalUnit(int y, int x){
	FILE *fp;
	fp = fopen("8b6tPlot.dat", "a");
	fprintf(fp, "%d\t%d\n", x, y);
	fprintf(fp, "%d\t%d\n", x + 1, y);
	fclose(fp);
}

void fnPlotSignal(int iaSignal[][8], int iBytes){
	int i, j;
	int weight = 0;
	int iInversion = 0;
	FILE *fp;

	fp = fopen("8b6tPlot.dat", "w");
	fprintf(fp, "%d\t%d\n", 0, 2);
	fprintf(fp, "%d\t%d\n", 0, -2);
	fclose(fp);
	
	for(i = 0; i < iBytes; i++){
		if(weight == 1 && iaSignal[i][W_INDEX] == 1){
			for(j = 0; j < 6; j++)
				fnPlotSignalUnit(iaSignal[i][j] * -1, i * 6 + j);
			weight = 0;
			iInversion = 1;
		}
		else{
			for(j = 0; j < 6; j++)
				fnPlotSignalUnit(iaSignal[i][j], i * 6 + j);
			weight += iaSignal[i][W_INDEX];
		}

		iInversion = 0;
	}
}
		
int main(void){

	int i, j;
	int mappinG[TOTAL_COMBINATION][8];
	int maxBytes, iBytes;
	
	generate_8b6t(mappinG, TOTAL_COMBINATION);

	printf("Enter the maximum bytes of data:\t");
	scanf("%d", &maxBytes);
	
	system("clear");

	int iaSignal[maxBytes][8];

	int iChoice;
	int iAssigned = 0;
	
	while(1){
		printf("\n1. Show 8b6t Mapping\n2. Enter New Input data\n3. Convert Data to Signal\n4. Plot cordinates.\n5. Exit\n");
		printf("Enter your choice:\t");
		scanf("%d", &iChoice);

		system("clear");
		
		switch(iChoice){
			case 1:
				shOwmappinG(mappinG, TOTAL_COMBINATION, 2);
				break;

			case 2:
				tryAgain:
				iAssigned = 1;
				printf("Enter the amount of data in bytes:\t");
				scanf("%d", &iBytes);
				if(iBytes > maxBytes){
					printf("Maximum data can be %d bytes. PLease try again.\n", maxBytes);
					goto tryAgain;
				}
				fnDataToSignal(mappinG, iaSignal, iBytes);

			case 3:
				if(!iAssigned){
					system("clear");
					printf("No previous data found. Enter data first.\n");
					goto tryAgain;				
				}
				fnShowSignalMapping(iaSignal, iBytes);

				break;
			case 4:
				if(!iAssigned){
					system("clear");
					printf("No previous data found. Enter data first.\n");
					goto tryAgain;				
				}
				
				fnPlotSignal(iaSignal, iBytes);
				printf("Coordinates has been plotted in 8b6tPlot.dat file\n");
				system("gnuplot 8b6tPlot.gpl");
				break;
			
			case 5:
				exit(0);
			default:
				printf("Invalid option. Try again.\n");
		}
	}

	return 0;
}
