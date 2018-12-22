#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Serial.h"

typedef struct DataNode 
{ 
        int dd,mm,yy,hh,nn; //nn = minute 
        float temp,hum; 
        struct DataNode *next; 
} TDataNode;
TDataNode *head; //head of linked list

void parseData(char* str, TDataNode *ret);

void loadLinkedList();

char *dataTempHum[] = "";

/* = {"01-12-2018-01-57;36.09;64.45",
"01-12-2018-16-38;23.27;66.40",
"02-12-2018-01-58;29.86;73.59",
"03-12-2018-02-24;29.59;59.40",
"03-12-2018-02-30;24.03;65.01",
"03-12-2018-20-24;37.27;64.96",
"03-12-2018-22-36;19.63;72.63",
"04-12-2018-16-19;30.53;60.93",
"04-12-2018-23-29;16.78;69.46",
"05-12-2018-02-02;33.89;64.63",
"05-12-2018-21-18;24.25;59.61",
"06-12-2018-10-25;27.98;61.75",
"06-12-2018-13-30;27.05;68.24",
"07-12-2018-09-41;36.63;74.54"};*/

int main(int argc, char* argv[])
{
	printf("Welcome to the Temperature and Humidity Chart Demo!\n\n"); 
    printf("This program will demonstrate the chart of temperature and humidity across a time.\n"); 
    printf("Data is got from a temperature and humidity sensors, connected to Arduino and saved into a SD Card.\n"); 
    printf("The data is then withdrawed through Serial port (RS-232) to the PC then plotted to the chart.\n"); 
    printf("Enjoy the Demo!\n"); 
   // system("pause");
    
    //loadLinkedList();
    
    //Show the linked list
    //TDataNode *node;
    /*for (node=head;node->next;node=node->next)
    {
     printf("%02d-%02d-%02d %02d:%02d \t%0.2f\t%0.2f\n",node->dd,node->mm,node->yy,node->hh,node->nn,node->temp,node->hum);
 	}*/
	
	
	SerialOpen("COM4");

	if (IsConnected())
		printf("We're connected\n\n");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;

	int j = 0; char *token;
	while(IsConnected())
	{
		readResult = ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
                incomingData[readResult] = 0;

        	printf("%s",incomingData);
		
		if(incomingData != NULL)
		{
			token = strtok(incomingData, "\n");
			while(token != NULL)
			{
				strcpy(dataTempHum[j], token);
				j++;
				strtok(NULL, "\n");
			}
		}
		
		Sleep(500);
	}
	
	/*int j = 0; char *token;
	token = strtok(incomingData, "\n");
	while(token != NULL)
	{
		strcpy(dataTempHum[j], token);
		j++;
		strtok(NULL, "\n");
	}*/
	
	return 0;
}


void parseData(char* str, TDataNode *ret) 
{ 
    char strLocal[50]; 
    char *token,sDate[16],sTemperature[5],sHum[5];
    if (strlen(str)<=2) //avoid invalid string to split 
    { 
        ret= NULL; 
        return; 
    }
    strcpy(strLocal, str); //save the parameter str, not to be cut by strtok()
        token = strtok(strLocal, ";"); 
        strcpy(sDate,token); //get date dd-mm-yyyy hh:nn 
        token = strtok(NULL, ";"); 
        strcpy(sTemperature,token); //get temperature data xx.x 
        token = strtok(NULL, ";"); 
        strcpy(sHum,token); //get humidity data xx.xx
        token= strtok(sDate,"-"); 
        ret->dd= atoi(token); 
        token= strtok(NULL,"-"); 
        ret->mm= atoi(token); 
        token= strtok(NULL,"-"); 
        ret->yy= atoi(token); 
        token= strtok(NULL,"-"); 
        ret->hh= atoi(token); 
        token= strtok(NULL,"-"); 
        ret->nn= atoi(token);
        ret->temp= atof(sTemperature); 
        ret->hum= atof(sHum); 
}

/*void loadLinkedList()
{
 char *datetime,*sTemp,*sHum;
 int i=0;
 int arrSize= (sizeof(dataTempHum)/sizeof(const char*));
 TDataNode *last=NULL;
 
 head= (TDataNode*) malloc(sizeof(TDataNode));
 last=head;
 
 for (i=0;i<arrSize;i++)
 {
  parseData(dataTempHum[i],last);
  last->next= (TDataNode*) malloc(sizeof(TDataNode));
  last= last->next;
 }
 last->next= NULL;
}
*/
