#include <stdio.h>
#include <conio.h>
#include <iostream.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>
#include <direct.h>
////////////////////
int    ItemsCount,RecordsCount,IterativeItemsCount,DatabaseRecordsCount;;
float  Min_Sup;
double Max_Approximate;
float  PrimaryItemsValues[100][100];// Primary Values
float  StaticItemsValues[100][100];// Static Normalization Values
float  NCItemsValues[100][100];// NC Normalization Values
float  DivideMaxItemsValues[100][100];// DivideMax Normalization Values
char   IterativeItemsList[100];
float  IterativeItemsValues[100][100];// Iterative Values
char   D[100][3][100];// Database 
int	   N;//Transactions Count
char   BitTable[1001][1001][1000];//BitTable
int    FrequentItemsCount;//Count of The Frequent Items
FILE   *Out;
struct IndexArrayStructure
{
	char Item;
	char SubSume[1000];
};
IndexArrayStructure IndexArray[1000];
////////////////////
void AddCharToString(char Str[100],char ch)
{
	int Length=strlen(Str);
	Str[Length]=ch;
	Str[Length+1]='\0';	
}
////////////////////
void ReadDatabaseFromFile(float ItemsValues[100][100])
{	
    FILE *In;
    char ch;
    char Str[100];
	if ((In=fopen("input.txt","r"))==NULL)
	{		
        cout<<"Can not Open From File \n";
	}
    else
	{				            
		for(int i=1;i<=RecordsCount;i++)
		{
			for(int j=1;j<=ItemsCount;j++)
			{					
				strcpy(Str,"");
				while ((ch = getc(In))!=' ')
				{
					AddCharToString(Str,ch);
				}
				ItemsValues[i-1][j-1]=atof(Str);
			}
		}
		fclose(In);
	}
}
////////////////////
void ReadDatabaseFromInput(float ItemsValues[100][100])
{	
	cout << "*************************\n";
	cout << "Enter Values of the Items \n";
	char ch='A';
	for(int j=1;j<=ItemsCount;j++)
	{
		for(int i=1;i<=RecordsCount;i++)
		{
			cout << "[" << ch << "," << i << "] : ";
			cin >> ItemsValues[i-1][j-1];
		}
		ch++;
	}
}
////////////////////
float MinMaxRange(float ItemsValues[100][100],int Type,int ItemIndex)
{
	int i;
	float Min,Max;
	switch (Type)
	{		
		case 0: //For Min		
			Min=ItemsValues[0][ItemIndex-1];
			for(i=2;i<=RecordsCount;i++)
				if (ItemsValues[i-1][ItemIndex-1]<Min)
					Min=ItemsValues[i-1][ItemIndex-1];
			return Min;
			break;
		case 1: //For Max
			Max=ItemsValues[0][ItemIndex-1];
			for(i=2;i<=RecordsCount;i++)
				if (ItemsValues[i-1][ItemIndex-1]>Max)
					Max=ItemsValues[i-1][ItemIndex-1];
            return Max;
	}
}
////////////////////
float ItemAverage(float ItemsValues[100][100],int ItemIndex)
{
	float Sum=0;
	for (int i=1;i<=RecordsCount;i++)
		Sum+=ItemsValues[i-1][ItemIndex-1];          
	return ( ( (float)1/RecordsCount )*Sum );   
}
////////////////////
double ItemStandardDeviation(float ItemsValues[100][100],int ItemIndex)
{
	double Sum=0;
	for (int i=1;i<=RecordsCount;i++)
		Sum+=pow((ItemsValues[i-1][ItemIndex-1]-ItemAverage(ItemsValues,ItemIndex)),2);
	return sqrt( ( (double)1/(RecordsCount-1) )*Sum );
}
////////////////////
float NC(float ItemsValues[100][100],int ItemIndex)
{
	int i,Count=0;
	float Sum=0;
	//Calculate EVF
	for (i=1;i<=RecordsCount;i++)
		Sum+=ItemsValues[i-1][ItemIndex-1];
	for (i=1;i<=RecordsCount;i++)
		if (ItemsValues[i-1][ItemIndex-1]!=0)
			Count++;
	Sum=(float)(Sum/Count);
	//Calculate and Retuen NC
		return (float)( 1/(RecordsCount*Sum) );
}
////////////////////
float ItemSupportValue(float ItemsValues[100][100],int ItemIndex)
{
	int i;
	float Count=0;
	for(i=1;i<=RecordsCount;i++)
	{
		if ((ItemsValues[i-1][ItemIndex-1]>=MinMaxRange(ItemsValues,0,ItemIndex))&&(ItemsValues[i-1][ItemIndex-1]<=MinMaxRange(ItemsValues,1,ItemIndex))&&(ItemsValues[i-1][ItemIndex-1]!=0))				
			Count++;
	}
        
	return (float)(Count/RecordsCount);
}
////////////////////
double EuclideanSpaceValue(float Value1,float Value2)
{       
	return (double)(sqrt(pow((Value1-Value2),2)));
}
////////////////////
double CityBlockSpaceValue(float Value1,float Value2)
{       
	return fabs(Value1-Value2);
}
////////////////////
void OutputDatabase(char* FileName)
{	
    FILE *Out;
    char Str[100];
	if ((Out=fopen(FileName,"w"))==NULL)
	{		
        cout<<"Can not Open File " << FileName << "\n";
	}
    else
	{				            
		for(int i=1;i<=DatabaseRecordsCount;i++)
		{					
			strcpy(Str,"");
			strcat(Str,D[i-1][0]);
			strcat(Str," ");
			strcat(Str,D[i-1][1]);
			strcat(Str,",");
			strcat(Str,D[i-1][2]);
			strcat(Str,"\n");
			fputs(Str,Out);
		}
		fclose(Out);
	}
}
////////////////////
int FindCharInStr(char* Str,char ch)
{
	for (int i=0;i<strlen(Str);i++)
		if (Str[i]==ch)
			return i;
	return -1;
}
////////////////////
void OrderItemset()
{
	char ItemsList[1000],ch;
	int ItemsCount[1000],Count=-1,i,j,k,m;

	//Fill ItemsList And ItemsCount From Database 
    for (i=0;i<N;i++)	
	{
		for (j=0;j<strlen(D[i][1]);j++)
		{
			if (FindCharInStr(ItemsList,D[i][1][j])==-1)
			{
				ItemsList[++Count]=D[i][1][j];
                ItemsList[Count+1]='\0';
				//
                ItemsCount[Count]=1;
			}
			else
			{
				ItemsCount[FindCharInStr(ItemsList,D[i][1][j])]+=1;
			}
		}
	}
	//Delete InFrequent Items
	for (i=0;i<N;i++)
		strcpy(D[i][2],D[i][1]);
	for (i=0;i<N;i++)
	{
		for (j=0;j<strlen(D[i][2]);j++)
			if (ItemsCount[FindCharInStr(ItemsList,D[i][2][j])]<Min_Sup)
			{
				strcpy(&D[i][2][j],&D[i][2][j+1]);
				j--;
			}
	}
	//Order Itemset
	for (i=0;i<N;i++)
	{
		for (j=0;j<strlen(D[i][2]);j++)
			for (k=j;k<strlen(D[i][2]);k++)
				if (ItemsCount[FindCharInStr(ItemsList,D[i][2][j])]>ItemsCount[FindCharInStr(ItemsList,D[i][2][k])])
				{
					ch = D[i][2][k];
					for (m=k;m>j;m--)
					{
						D[i][2][m] = D[i][2][m-1];
					}
					D[i][2][j] = ch;
				}

	}
	//Cout Orderd Itemset
	cout << "\nTID  Ordered Items \n";
	cout << "---  ------------- \n";
	for (i=0;i<N;i++) 
	{
		cout<< D[i][0];
		switch (strlen(D[i][0]))
		{			
		    case 1: cout << "    "; break;
            case 2: cout << "   " ; break;
			default:cout << "  "  ;
		}
		cout<< D[i][2] << "\n";
	}
		
}
////////////////////
int ItemCountInDatabase(char ch)
{
	int Count=0;
	for (int i=0;i<N;i++)
	{
		for (int j=0;j<strlen(D[i][2]);j++)
		{
			if (D[i][2][j]==ch)
				Count++;
		}
	}
	return Count; 
}
////////////////////
void BitTableGenerate()
{
	char ItemsList[1000],ch;
	int ItemsCount[1000],Count,i,j,k,m,z,Value,Number;

	Count=0;
	//Fill and Sort ItemsList And ItemsCount From Ordered Items of The Database 
    ItemsList[0]='\0';
	for (i=0;i<N;i++)	
	{
		for (j=0;j<strlen(D[i][2]);j++)
		{			
			if (FindCharInStr(ItemsList,D[i][2][j])==-1)
			{
				Number=ItemCountInDatabase(D[i][2][j]);
				for (k=0;(k<Count)&&(Number>ItemsCount[k]);k++);
				if (Number==ItemsCount[k])
					for ( ;(k<Count)&&(D[i][2][j]>ItemsList[k]);k++);
				for (z=Count+1;z>k;z--)
				{
					ItemsList[z]=ItemsList[z-1];
					ItemsCount[z]=ItemsCount[z-1];
				}
				ItemsList[z]=D[i][2][j];
				ItemsCount[z]=Number;
                Count++;
			}
		}
	}//cout<<"********"<<N<<"*************";
	//Fill Horizontal Labels of The BitTable 
    for (i=0;i<N;i++)
	{
		strcpy(BitTable[i+1][0],D[i][0]);
	}
	//Fill Vertical Labels of The BitTable
    char S[2];
	S[1]='\0';
	for (i=0;i<strlen(ItemsList);i++)
	{
		S[0]=ItemsList[i];
		strcpy(BitTable[0][i+1],S);
	}
	//Fill Content of The BitTable
    for (i=1;i<=N;i++)
	{
		for (j=1;j<=strlen(ItemsList);j++)
		{
			if (FindCharInStr(D[i-1][2],ItemsList[j-1])!=-1)
				strcpy(BitTable[i][j],"1");
			else
				strcpy(BitTable[i][j],"0");
		}
	}
	//cout BitTable
	cout << "\n\n" << "BitTable\n" << "--------\n";
	for (i=0;i<=N;i++)
	{
		for (j=0;j<=strlen(ItemsList);j++)
		{
			cout << BitTable[i][j];
			if (j==0)
			{
				switch (strlen(BitTable[i][j]))
				{					
				    case 0: cout << "      "; break; 
					case 1: cout << "     "; break; 
					case 2: cout << "    "; break;
					case 3: cout << "   "; break;
					default:cout << "  "; 
				}
			}
			else
			{
				cout << "   ";
			}			
		}
		cout << "\n";
	}
	//Count of The Frequent Items
	FrequentItemsCount=strlen(ItemsList);	
	
}
////////////////////
char ConvertSingleStringToChar(char Str[2])
{
	return Str[0];
}
////////////////////
void IndexArrayGenerate()
{
	int i,j,k;
	bool Flag;
	for (i=0;i<FrequentItemsCount;i++)
	{
		IndexArray[i].Item=ConvertSingleStringToChar(BitTable[0][i+1]);
		//		
		for (j=i+1;j<FrequentItemsCount;j++)
		{
			Flag=false;
			for (k=0;k<N;k++)
			{
				if (strcmp(BitTable[k+1][i+1],"1")==0)
					if (strcmp(BitTable[k+1][i+1],BitTable[k+1][j+1])==0)
					{
						Flag=true;
					}
					else
					{
						Flag=false;
						break;
					}
			}
			if (Flag==true)
				strcat(IndexArray[i].SubSume,BitTable[0][j+1]);
			
		}
		if (strcmp(IndexArray[i].SubSume,"")==0)
			strcat(IndexArray[i].SubSume,"0");
		
	}
	cout << "\n\n" << "Index Array" << "\n" <<"-----------" << "\n";
	for (i=0;i<FrequentItemsCount;i++)
		cout << "(" << IndexArray[i].Item << "," << IndexArray[i].SubSume << ")" << "\n";
	
}
////////////////////
int StringOccurrenceCountInString(char *Str1,char *Str2)
{
	//Str2 Occurrence Count In Str1
	int Count=0;
	while ((Str1=strstr(Str1,Str2))!=NULL)
	{
		Count++;
		Str1=Str1+1;
	}
	return Count;
}
////////////////////
int StringOccurrenceCountInDatabase(char Str[1000])
{
	//Str Occurrence Count In Database
	int Count=0;
	for (int i=0;i<N;i++)
	{
		Count += StringOccurrenceCountInString(D[i][2],Str);
	}
	return Count;
}
////////////////////
int StringOccurrenceCountInBitTable(char Str[1000])
{
	bool Flag;
	int i,j,k,Pos,TotalCount=0;
    char S[2];

	for (i=1;i<=N;i++)
	{// 1th
		Flag=true;
		for (j=0;j<strlen(Str);j++)
		{// 2th
			for (k=1;k<=FrequentItemsCount;k++)
			{// 3th
				S[0]=Str[j];
				S[1]='\0';
				if (strcmp(S,BitTable[0][k])==0)
					if (strcmp(BitTable[i][k],"0")==0)
					{// 4th
						Flag=false;
						break;
					}// 4th							
			}// 3th
		}// 2th
		if (Flag)
			TotalCount++;
	}// 1th
	return TotalCount;
}
////////////////////
void Depth_First(char ItemSet[1000],char Tail[1000])
{
	int i;
	char F_ItemSet[1000],FileStr1[1000],FileStr2[1000];
	if (strlen(Tail)==0)
		return;
	else
	{
		for (i=0;i<strlen(Tail);i++)
		{
			char Str[2];
			Str[0]=Tail[i];
            Str[1]='\0';
			strcat(ItemSet,Str);
			strcpy(F_ItemSet,ItemSet);
			if (StringOccurrenceCountInBitTable(F_ItemSet)>=Min_Sup)
			{
				cout << "\n" << F_ItemSet << ":" << StringOccurrenceCountInBitTable(F_ItemSet);
				//
				strcpy(FileStr1,F_ItemSet);
				strcat(FileStr1,":");
				itoa(StringOccurrenceCountInBitTable(F_ItemSet),FileStr2,10);
				strcat(FileStr1,FileStr2);
				strcat(FileStr1,"\n"); 
				fputs(FileStr1,Out);
			}
			strcpy(&Tail[i],&Tail[i+1]);
			Depth_First(F_ItemSet,Tail);
		}
	}	
}
////////////////////
void Index_BitTableFI()
{
	int i,j,k,z,w,x,Count,SubSumeLen,Len,StrLen,Number;
	char Str[1000],Str1[1000],Str2[1000],Str3[2],Str4[1000],FileStr1[1000],FileStr2[1000];
	if ((Out=fopen("output.txt","w"))==NULL)
	{		
        cout<<"Can not Write To File \n";		
	}
	//
	cout << "\n\n" << "Index-BitTableFI" << "\n" << "----------------" << "\n";
	for (i=0;i<FrequentItemsCount;i++)
	{// 1th
		cout << "\n" << IndexArray[i].Item << ":" << ItemCountInDatabase(IndexArray[i].Item);
		//
		if (strcmp(IndexArray[i].SubSume,"0")==0)
		{// 2th
			if (ItemCountInDatabase(IndexArray[i].Item)>=Min_Sup)
			{
				// t(IndexArray[i].Item) : ItemSet After IndexArray[i].Item
				strcpy(Str,"");
				for (j=0;j<N;j++)
				{
					for (k=0;k<strlen(D[j][2]);k++)
					{
						if (D[j][2][k]==IndexArray[i].Item)
							for (z=k+1;z<strlen(D[j][2]);z++)
							{
								if (FindCharInStr(Str,D[j][2][z])==-1)
								{
									Len=strlen(Str);
									Str[Len]=D[j][2][z];
									Str[Len+1]='\0';
								}													    							    
							}
					}
				}
				// Call Depth_First				 
				Str3[0]=IndexArray[i].Item;
				Str3[1]='\0';
				Depth_First(Str3,Str);
			}
				
		}// 2th
		else
		{// 3th					
			SubSumeLen=strlen(IndexArray[i].SubSume);
			for (w=1;w<pow(2,SubSumeLen);w++)
			{
				Count=0;
				strcpy(Str2,"");
				Number=w;
				while (Number>0)
				{
					Count++;
					if (Number%2==1)
					{
						StrLen=strlen(Str2);
						Str2[StrLen]=IndexArray[i].SubSume[SubSumeLen-Count];
                        Str2[StrLen+1]='\0';						
					}
					Number=Number/2;
				}
				strrev(Str2);
				Str4[0]=IndexArray[i].Item;
				Str4[1]='\0';
                strcat(Str4,Str2);
				cout << "\n" << IndexArray[i].Item << Str2 << ":" << StringOccurrenceCountInBitTable(Str4);
				//
				FileStr1[0]=IndexArray[i].Item;
				FileStr1[1]='\0';
				strcat(FileStr1,Str2);
				strcat(FileStr1,":");
				itoa(StringOccurrenceCountInBitTable(Str4),FileStr2,10);
				strcat(FileStr1,FileStr2);
                strcat(FileStr1,"\n"); 
				fputs(FileStr1,Out);
			}
            if (ItemCountInDatabase(IndexArray[i].Item)>Min_Sup)
			{// 4th				
				strcpy(Str,"");
				for (j=0;j<N;j++)
				{
					for (k=0;k<strlen(D[j][2]);k++)
					{
						if (D[j][2][k]==IndexArray[i].Item)
							for (z=k+1;z<strlen(D[j][2]);z++)
							{
								if (FindCharInStr(Str,D[j][2][z])==-1)
								{
									Len=strlen(Str);
									Str[Len]=D[j][2][z];
									Str[Len+1]='\0';
								}													    							    
							}
					}
				}
				// Delete Items Included by IndexArray[i].SubSume From t(IndexArray[i].Item)
				for (x=0;x<strlen(Str);x++)
				{
					if (FindCharInStr(IndexArray[i].SubSume,Str[x])!=-1)
					{
						strcpy(&Str[x],&Str[x+1]);
						x--;
					}
				}
				// Call Depth_First				 
				Str3[0]=IndexArray[i].Item;
				Str3[1]='\0';
				strcpy(Str4,Str);
				Depth_First(Str3,Str4);
				//
			    SubSumeLen=strlen(IndexArray[i].SubSume);
			    for (w=1;w<pow(2,SubSumeLen);w++)
				{
				    Count=0;
				    strcpy(Str2,"");
				    Number=w;
				    while (Number>0)
					{
					    Count++;
					    if (Number%2==1)
						{
						    StrLen=strlen(Str2);
						    Str2[StrLen]=IndexArray[i].SubSume[SubSumeLen-Count];
                            Str2[StrLen+1]='\0';						    
						}
						Number=Number/2;
					}
					strrev(Str2);
				    strcpy(Str1,"");
				    Str1[0]=IndexArray[i].Item;
                    Str1[1]='\0';
				    strcat(Str1,Str2);
					strcpy(Str4,Str);
					Depth_First(Str1,Str4);
				}
			 }// 4th
		}// 3th
		cout << "\n**********";
		//        
		strcpy(FileStr1,"**********\n"); 
		fputs(FileStr1,Out);
	}// 1th
	fclose(Out);
}
////////////////////
void main()
{
	int i,j,k,l,Option;
	char ch,Str[100];
	float Sum,Count,Temp,Min,Max,Digit1,Digit2;
	double Value;
    cout << "Enter Items Count: ";
	cin >> ItemsCount;
	cout << "Enter Records Count: ";
	cin >> RecordsCount;
	//Primary values of the Items
	cout << "what is your option for input Database : \n" << "1.Input From File (Input.txt) \n" << "2.Input From Enter Data \n" << "Enter 1 or 2 : ";
    cin >> Option;
	switch (Option)
	{
		case 1 : ReadDatabaseFromFile(PrimaryItemsValues); break;
		case 2 : ReadDatabaseFromInput(PrimaryItemsValues); break;
		default: cout << "The entered option is wrong.";
	}
	//
	cout << "Enter The Min_Sup Value: ";
	cin >> Min_Sup;
	//
	cout << "Enter The Max_Approximate Value: ";
	cin >> Max_Approximate;
	//Calculate and Output Min and Max Values of the Primary Values
	cout << "*************************\n";
	cout << "Min and Max Values of the Primary Values is:\n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		cout << ch << " : " << "Min=" << MinMaxRange(PrimaryItemsValues,0,j) << " , Max=" << MinMaxRange(PrimaryItemsValues,1,j) << "\n";
		ch++;
	}
	//Static Normalization
	for(j=1;j<=ItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			StaticItemsValues[i-1][j-1]=(float)( (PrimaryItemsValues[i-1][j-1]-ItemAverage(PrimaryItemsValues,j))/ItemStandardDeviation(PrimaryItemsValues,j) );
		}

	}
	//Output Static Items Values and Sum of Them
	cout << "*************************\n";
	cout << "Static Items Values is : \n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		Sum=0;
		for(i=1;i<=RecordsCount;i++)
		{
			Sum+=StaticItemsValues[i-1][j-1];
			cout << "[" << ch << ":" << i << "] = " << StaticItemsValues[i-1][j-1] << "\n";
		}
        cout << "Total = " << Sum << "\n";
		cout << "+++++\n";
		ch++;
	}
	//Calculate and Output Min and Max Values of the Static Values
	cout << "*************************\n";
	cout << "Min and Max Values of the Static Values is:\n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		cout << ch << " : " << "Min=" << MinMaxRange(StaticItemsValues,0,j) << " , Max=" << MinMaxRange(StaticItemsValues,1,j) << "\n";
		ch++;
	}
	//Calculate NC Values of the Primary Values
	for(j=1;j<=ItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			NCItemsValues[i-1][j-1]=PrimaryItemsValues[i-1][j-1]*NC(PrimaryItemsValues,j);			
		}
	}
	//Output NC Items Values and Sum of Them
	cout << "*************************\n";
	cout << "NC Items Values is : \n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		Sum=0;
		for(i=1;i<=RecordsCount;i++)
		{
			Sum+=NCItemsValues[i-1][j-1];
			cout << "[" << ch << ":" << i << "] = " << NCItemsValues[i-1][j-1] << "\n";
		}
        cout << "Total = " << Sum << "\n";
		cout << "+++++\n";
		ch++;
	}
	//Calculate and Output Min and Max Values of the NC Values
	cout << "*************************\n";
	cout << "Min and Max Values of the NC Values is:\n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		cout << ch << " : " << "Min=" << MinMaxRange(NCItemsValues,0,j) << " , Max=" << MinMaxRange(NCItemsValues,1,j) << "\n";
		ch++;
	}
	//Calculate DivideMax Values of the Primary Values
	for(j=1;j<=ItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			DivideMaxItemsValues[i-1][j-1]=(float)(PrimaryItemsValues[i-1][j-1]/MinMaxRange(PrimaryItemsValues,1,j));			
		}
	}
	//Output DivideMax Items Values and Sum of Them
	cout << "*************************\n";
	cout << "DivideMax Items Values is : \n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		Sum=0;
		for(i=1;i<=RecordsCount;i++)
		{
			Sum+=DivideMaxItemsValues[i-1][j-1];
			cout << "[" << ch << ":" << i << "] = " << DivideMaxItemsValues[i-1][j-1] << "\n";
		}
        cout << "Total = " << Sum << "\n";
		cout << "+++++\n";
		ch++;
	}
	//Calculate and Output Min and Max Values of the DivideMax Values
	cout << "*************************\n";
	cout << "Min and Max Values of the DivideMax Values is:\n";
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
		cout << ch << " : " << "Min=" << MinMaxRange(DivideMaxItemsValues,0,j) << " , Max=" << MinMaxRange(DivideMaxItemsValues,1,j) << "\n";
		ch++;
	}	
	//Calculate and Output Sup Degree Values of the Static Items
	cout << "*************************\n";
	cout << "Sup Degree Values of the Static Items Values is:\n";    
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
        cout << ch << ":" << ItemSupportValue(StaticItemsValues,j) << "\n";
		cout << "+++++\n";
		ch++;
	}
	//Calculate and Output Sup Degree Values of the NC Items
	cout << "*************************\n";
	cout << "Sup Degree Values of the NC Items Values is:\n";    
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{
        cout << ch << ":" << ItemSupportValue(NCItemsValues,j) << "\n";
		cout << "+++++\n";
		ch++;
	}
	//Calculate and Output Sup Degree Values of the DivideMax Items
	cout << "*************************\n";
	cout << "Sup Degree Values of the DivideMax Items Values is:\n";    
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{        
		cout << ch << ":" << ItemSupportValue(DivideMaxItemsValues,j) << "\n";
		cout << "+++++\n";
		ch++;
	}



	//StaticItemsValues
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	cout << "Operations on the Static Items Values :\n"; 
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	//Fill IterativeItemsList,IterativeItemsValues and Delete Non Iterative Items
	IterativeItemsCount=0;	
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{		        
		if (ItemSupportValue(StaticItemsValues,j)>=Min_Sup)
		{
			IterativeItemsCount++;
			IterativeItemsList[IterativeItemsCount-1]=ch;
			for (i=1;i<=RecordsCount;i++)
			{
				IterativeItemsValues[i-1][IterativeItemsCount-1]=StaticItemsValues[i-1][j-1];
			}

		}
		ch++;
	}
	//Sort Items of the IterativeItemsValues
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(k=j+1;k<=IterativeItemsCount;k++)
		{			
			Digit1=ItemSupportValue(IterativeItemsValues,j);
			Digit2=ItemSupportValue(IterativeItemsValues,k);
			if (Digit1>Digit2)
			{                             
				ch=IterativeItemsList[j-1];
				IterativeItemsList[j-1]=IterativeItemsList[k-1];
				IterativeItemsList[k-1]=ch;
				//
				for(i=1;i<=RecordsCount;i++)
				{
					Temp=IterativeItemsValues[i-1][j-1];
					IterativeItemsValues[i-1][j-1]=IterativeItemsValues[i-1][k-1];
					IterativeItemsValues[i-1][k-1]=Temp;
				}
			}
		}

	}
	//Output Items of The IterativeItemsList and IterativeItemsValues
	cout << "Iterative Items Values is : \n";
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			cout << "[" << IterativeItemsList[j-1] << ":" << i << "] = " << IterativeItemsValues[i-1][j-1] << "\n";
		}
		cout << "+++++\n";
	}
	//Generate Index Array
	DatabaseRecordsCount=0;
	//
	for (j=1;j<=IterativeItemsCount;j++)
	{//1   
		//Find The First Record That Is Not Equal With Zero
		for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
		//
		Min=IterativeItemsValues[l-1][j-1];                                        
		Max=IterativeItemsValues[l-1][j-1];
		//Find Min and Max Value In Item
		for(i=l+1;i<=RecordsCount;i++)
		{                                                         
			if (IterativeItemsValues[i-1][j-1]!=0)
			{                                                                                                                
				if (IterativeItemsValues[i-1][j-1]<Min)
					Min=IterativeItemsValues[i-1][j-1];
				if (IterativeItemsValues[i-1][j-1]>Max)
					Max=IterativeItemsValues[i-1][j-1];
			}
		}
		//
		if ((Min>0)&&(Max>0))
			Value=CityBlockSpaceValue(Max,Min);
		else
			Value=EuclideanSpaceValue(Max,Min);                                       
		if (Value<=Max_Approximate)
		{//2    
			DatabaseRecordsCount++;	        		
			sprintf(Str,"%d",DatabaseRecordsCount);//Convert i to Str
			strcpy(D[DatabaseRecordsCount-1][0],Str);
			strcpy(D[DatabaseRecordsCount-1][1],"");
            AddCharToString(D[DatabaseRecordsCount-1][1],IterativeItemsList[j-1]);
            strcpy(D[DatabaseRecordsCount-1][2],"");
			//
			for (k=j+1;k<=IterativeItemsCount;k++)
			{//3
				//Find The First Record That Is Not Equal With Zero
				for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
				//
				Min=IterativeItemsValues[l-1][k-1];
				Max=IterativeItemsValues[l-1][k-1];
				//Find Min and Max Value In Item
				for(i=l+1;i<=RecordsCount;i++)
				{
					if (IterativeItemsValues[i-1][j-1]!=0)
					{
						if (IterativeItemsValues[i-1][k-1]<Min)
							Min=IterativeItemsValues[i-1][k-1];
						if (IterativeItemsValues[i-1][k-1]>Max)
							Max=IterativeItemsValues[i-1][k-1];
					}
				}
				//
				if ((Min>0)&&(Max>0))
					Value=CityBlockSpaceValue(Max,Min);
				else
					Value=EuclideanSpaceValue(Max,Min);                                                          
				if (Value<=Max_Approximate)
				{
					AddCharToString(D[DatabaseRecordsCount-1][2],IterativeItemsList[k-1]);
				}
				
			}//3
			if (strcmp(D[DatabaseRecordsCount-1][2],"")==0)
				strcpy(D[DatabaseRecordsCount-1][2],"0");
		}//2

	}//1
	//Output Values of The Index Array 
	cout << "Values of The Index Array Is:\n";
	cout << "*************************\n";
	for (i=1;i<=DatabaseRecordsCount;i++)
	{
		cout << D[i-1][0] << ": (" << D[i-1][1] << "," << D[i-1][2] << ")\n";  
	}
	//Output D
	OutputDatabase("Output_Database_Static.txt");
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";



	//NCItemsValues
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	cout << "Operations on the NC Items Values :\n"; 
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	//Fill IterativeItemsList,IterativeItemsValues and Delete Non Iterative Items
	IterativeItemsCount=0;	
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{		        
		if (ItemSupportValue(NCItemsValues,j)>=Min_Sup)
		{
			IterativeItemsCount++;
			IterativeItemsList[IterativeItemsCount-1]=ch;
			for (i=1;i<=RecordsCount;i++)
			{
				IterativeItemsValues[i-1][IterativeItemsCount-1]=NCItemsValues[i-1][j-1];
			}

		}
		ch++;
	}
	//Sort Items of the IterativeItemsValues
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(k=j+1;k<=IterativeItemsCount;k++)
		{			
			Digit1=ItemSupportValue(IterativeItemsValues,j);
			Digit2=ItemSupportValue(IterativeItemsValues,k);
			if (Digit1>Digit2)
			{                             
				ch=IterativeItemsList[j-1];
				IterativeItemsList[j-1]=IterativeItemsList[k-1];
				IterativeItemsList[k-1]=ch;
				//
				for(i=1;i<=RecordsCount;i++)
				{
					Temp=IterativeItemsValues[i-1][j-1];
					IterativeItemsValues[i-1][j-1]=IterativeItemsValues[i-1][k-1];
					IterativeItemsValues[i-1][k-1]=Temp;
				}
			}
		}

	}
	//Output Items of The IterativeItemsList and IterativeItemsValues
	cout << "Iterative Items Values is : \n";
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			cout << "[" << IterativeItemsList[j-1] << ":" << i << "] = " << IterativeItemsValues[i-1][j-1] << "\n";
		}
		cout << "+++++\n";
	}
	//Generate Index Array
	DatabaseRecordsCount=0;
	//
	for (j=1;j<=IterativeItemsCount;j++)
	{//1   
		//Find The First Record That Is Not Equal With Zero
		for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
		//
		Min=IterativeItemsValues[l-1][j-1];                                        
		Max=IterativeItemsValues[l-1][j-1];
		//Find Min and Max Value In Item
		for(i=l+1;i<=RecordsCount;i++)
		{                                                         
			if (IterativeItemsValues[i-1][j-1]!=0)
			{                                                                                                                
				if (IterativeItemsValues[i-1][j-1]<Min)
					Min=IterativeItemsValues[i-1][j-1];
				if (IterativeItemsValues[i-1][j-1]>Max)
					Max=IterativeItemsValues[i-1][j-1];
			}
		}
		//
		if ((Min>0)&&(Max>0))
			Value=CityBlockSpaceValue(Max,Min);
		else
			Value=EuclideanSpaceValue(Max,Min);                                       
		if (Value<=Max_Approximate)
		{//2    
			DatabaseRecordsCount++;	        		
			sprintf(Str,"%d",DatabaseRecordsCount);//Convert i to Str
			strcpy(D[DatabaseRecordsCount-1][0],Str);
			strcpy(D[DatabaseRecordsCount-1][1],"");
            AddCharToString(D[DatabaseRecordsCount-1][1],IterativeItemsList[j-1]);
            strcpy(D[DatabaseRecordsCount-1][2],"");
			//
			for (k=j+1;k<=IterativeItemsCount;k++)
			{//3
				//Find The First Record That Is Not Equal With Zero
				for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
				//
				Min=IterativeItemsValues[l-1][k-1];
				Max=IterativeItemsValues[l-1][k-1];
				//Find Min and Max Value In Item
				for(i=l+1;i<=RecordsCount;i++)
				{
					if (IterativeItemsValues[i-1][j-1]!=0)
					{
						if (IterativeItemsValues[i-1][k-1]<Min)
							Min=IterativeItemsValues[i-1][k-1];
						if (IterativeItemsValues[i-1][k-1]>Max)
							Max=IterativeItemsValues[i-1][k-1];
					}
				}
				//
				if ((Min>0)&&(Max>0))
					Value=CityBlockSpaceValue(Max,Min);
				else
					Value=EuclideanSpaceValue(Max,Min);                                                          
				if (Value<=Max_Approximate)
				{
					AddCharToString(D[DatabaseRecordsCount-1][2],IterativeItemsList[k-1]);
				}
				
			}//3
			if (strcmp(D[DatabaseRecordsCount-1][2],"")==0)
				strcpy(D[DatabaseRecordsCount-1][2],"0");
		}//2

	}//1
	//Output Values of The Index Array 
	cout << "Values of The Index Array Is:\n";
	cout << "*************************\n";
	for (i=1;i<=DatabaseRecordsCount;i++)
	{
		cout << D[i-1][0] << ": (" << D[i-1][1] << "," << D[i-1][2] << ")\n";  
	}
	//Output D
	OutputDatabase("Output_Database_NC.txt");
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";



	//DivideMaxItemsValues
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	cout << "Operations on the DivideMax Items Values :\n"; 
	cout << "**************************************************\n";
	cout << "**************************************************\n";
	//Fill IterativeItemsList,IterativeItemsValues and Delete Non Iterative Items
	IterativeItemsCount=0;	
	ch='A';
	for(j=1;j<=ItemsCount;j++)
	{		        
		if (ItemSupportValue(DivideMaxItemsValues,j)>=Min_Sup)
		{
			IterativeItemsCount++;
			IterativeItemsList[IterativeItemsCount-1]=ch;
			for (i=1;i<=RecordsCount;i++)
			{
				IterativeItemsValues[i-1][IterativeItemsCount-1]=DivideMaxItemsValues[i-1][j-1];
			}

		}
		ch++;
	}
	//Sort Items of the IterativeItemsValues
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(k=j+1;k<=IterativeItemsCount;k++)
		{			
			Digit1=ItemSupportValue(IterativeItemsValues,j);
			Digit2=ItemSupportValue(IterativeItemsValues,k);
			if (Digit1>Digit2)
			{                             
				ch=IterativeItemsList[j-1];
				IterativeItemsList[j-1]=IterativeItemsList[k-1];
				IterativeItemsList[k-1]=ch;
				//
				for(i=1;i<=RecordsCount;i++)
				{
					Temp=IterativeItemsValues[i-1][j-1];
					IterativeItemsValues[i-1][j-1]=IterativeItemsValues[i-1][k-1];
					IterativeItemsValues[i-1][k-1]=Temp;
				}
			}
		}

	}
	//Output Items of The IterativeItemsList and IterativeItemsValues
	cout << "Iterative Items Values is : \n";
	for(j=1;j<=IterativeItemsCount;j++)
	{
		for(i=1;i<=RecordsCount;i++)
		{
			cout << "[" << IterativeItemsList[j-1] << ":" << i << "] = " << IterativeItemsValues[i-1][j-1] << "\n";
		}
		cout << "+++++\n";
	}
	//Generate Index Array
	DatabaseRecordsCount=0;
	//
	for (j=1;j<=IterativeItemsCount;j++)
	{//1   
		//Find The First Record That Is Not Equal With Zero
		for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
		//
		Min=IterativeItemsValues[l-1][j-1];                                        
		Max=IterativeItemsValues[l-1][j-1];
		//Find Min and Max Value In Item
		for(i=l+1;i<=RecordsCount;i++)
		{                                                         
			if (IterativeItemsValues[i-1][j-1]!=0)
			{                                                                                                                
				if (IterativeItemsValues[i-1][j-1]<Min)
					Min=IterativeItemsValues[i-1][j-1];
				if (IterativeItemsValues[i-1][j-1]>Max)
					Max=IterativeItemsValues[i-1][j-1];
			}
		}
		//
		if ((Min>0)&&(Max>0))
			Value=CityBlockSpaceValue(Max,Min);
		else
			Value=EuclideanSpaceValue(Max,Min);                                       
		if (Value<=Max_Approximate)
		{//2    
			DatabaseRecordsCount++;	        		
			sprintf(Str,"%d",DatabaseRecordsCount);//Convert i to Str
			strcpy(D[DatabaseRecordsCount-1][0],Str);
			strcpy(D[DatabaseRecordsCount-1][1],"");
            AddCharToString(D[DatabaseRecordsCount-1][1],IterativeItemsList[j-1]);
            strcpy(D[DatabaseRecordsCount-1][2],"");
			//
			for (k=j+1;k<=IterativeItemsCount;k++)
			{//3
				//Find The First Record That Is Not Equal With Zero
				for(l=1;(l<=RecordsCount)&&(IterativeItemsValues[l-1][j-1]==0);l++);
				//
				Min=IterativeItemsValues[l-1][k-1];
				Max=IterativeItemsValues[l-1][k-1];
				//Find Min and Max Value In Item
				for(i=l+1;i<=RecordsCount;i++)
				{
					if (IterativeItemsValues[i-1][j-1]!=0)
					{
						if (IterativeItemsValues[i-1][k-1]<Min)
							Min=IterativeItemsValues[i-1][k-1];
						if (IterativeItemsValues[i-1][k-1]>Max)
							Max=IterativeItemsValues[i-1][k-1];
					}
				}
				//
				if ((Min>0)&&(Max>0))
					Value=CityBlockSpaceValue(Max,Min);
				else
					Value=EuclideanSpaceValue(Max,Min);                                                          
				if (Value<=Max_Approximate)
				{
					AddCharToString(D[DatabaseRecordsCount-1][2],IterativeItemsList[k-1]);
				}
				
			}//3
			if (strcmp(D[DatabaseRecordsCount-1][2],"")==0)
				strcpy(D[DatabaseRecordsCount-1][2],"0");
		}//2

	}//1
	//Output Values of The Index Array 
	cout << "Values of The Index Array Is:\n";
	cout << "*************************\n";
	for (i=1;i<=DatabaseRecordsCount;i++)
	{
		cout << D[i-1][0] << ": (" << D[i-1][1] << "," << D[i-1][2] << ")\n";  
	}
	//Output D
	OutputDatabase("Output_Database_DivideMax.txt");
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

	//Index Bit Table FI
	for (i=1;i<=DatabaseRecordsCount;i++)
	{
		if (strcmp(D[i-1][2],"0")!=0)
			strcat(D[i-1][1],D[i-1][2]);
		strcpy(D[i-1][2],""); 
	}
	N=DatabaseRecordsCount;
	OrderItemset();
	BitTableGenerate();
	IndexArrayGenerate();
    Index_BitTableFI();

getch();
}