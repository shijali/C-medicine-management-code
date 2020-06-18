#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>

#define ANS 15
#define ACS 4
COORD coord= {0,0};
void gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
/*declaration of checking functions*/
void c_code(char[]);
int check(char[]);

/*structure declaration*/
typedef struct
{
    char name[ANS],code[ACS];
    float rate;
    int quantity;
} rec;
rec item;

/*declaration of display functions*/
void curser(int);
void dbill();
void d_mainmenu();
void display(rec *,int,int);
void window(int,int,int,int);
void dis_con();
void highlight(int,int);

/*declaration of main menu functions*/
void bill() ;
void add();
void exit();

/*declaration of display submenu functions*/
void d_code();
void d_rate();
void d_quan();

/*start of main*/
int main()
{
    d_mainmenu();
    return 0;
}

void d_mainmenu()
{
    int i;
    char ch;
    const char *menu[]= {"   Calculate Bill","   Add Medicine","   Edit Medicine","   Display All ","   Search", "   Delete Medicine","   Exit"};
    system("cls");
    window(25,50,20,32);
    gotoxy(33,18);
    printf("MAIN MENU");
    for (i=0; i<=6; i++)
    {
        gotoxy(30,22+i+1);
        printf("%s\n\n\n",menu[i]);
    }
    curser(7);
}


/*function for cursor movement*/
void curser(int no)
{
    int count=1;
    char ch='0';
    gotoxy(30,23);
    while(1)
    {
        switch(ch)
        {
        case 80:
            count++;
            if (count==no+1) count=1;
            break;
        case 72:
            count--;
            if(count==0) count=no;
            break;
        }
        highlight(no,count);
        ch=getch();
        if(ch=='\r')
        {
            if(no==7)
            {
                if (count==1) bill() ;
                else if(count==2) add();
                else   exit(0);
            }

        }
    }
}

void highlight(int no,int count)
{
    if (no==4)
    {
        gotoxy(30,23);
        printf("   By Code          ");
        gotoxy(30,24);
        printf("   By Rate          ");
        gotoxy(30,25);
        printf("   By Quantity      ");
        gotoxy(30,26);
        printf("   Back to main menu");
        switch (count)
        {
        case 1:
            gotoxy(30,23);
            printf(" - By Code          ");
            break;
        case 2:
            gotoxy(30,24);
            printf(" - By Rate          ");
            break;
        case 3:
            gotoxy(30,25);
            printf(" - By Quantity      ");
            break;
        case 4:
            gotoxy(30,26);
            printf(" - Back to main menu");
            break;
        }
    }

    if(no==7)
    {
        gotoxy (30,23);
        printf("   Calculate Bill ");
        gotoxy (30,24);
        printf("   Add Medicine      ");
        switch(count)
        {
        case 1:
            gotoxy (30,23);
            printf(" - Calculate Bill ");
            break;
        case 2:
            gotoxy (30,24);
            printf(" - Add Medicine      ");
            break;
          }
    }
}

void bill()
{
    char x[4]= {0};
    int j=29,q=0,size=0,i=1;
    float total=0,gtotal=0;
    FILE *file;
    file=fopen("record.txt","r+b");
    rewind(file);
    system("cls");
    dbill();
    gotoxy(26,15);
    printf("enter  \"end\" to finish input");
    while(1)
    {
        gotoxy(25,18);
        printf("                    ");
        gotoxy(25,19);
        printf("                    ");
        gotoxy(25,18);
        printf("enter item code:");
        scanf("%s",x);
        if(strcmp(x,"end")==0)
            break;
        gotoxy(25,19);
        printf("enter quantity:");
        scanf("%d",&q);
        rewind(file);
        while(fread(&item,sizeof(item),1,file))
        {
            if((strcmp(item.code,x)==0))
            {
                total=item.rate*q;
                gotoxy(11,j);
                printf("%4d",i);
                printf("%9s",item.name);
                printf("%13d",q);
                printf("%15.2f",item.rate);
                printf("%13.2f",total);
                gtotal=gtotal+total;
                size=sizeof(item);
                item.quantity=item.quantity-q;
                j+=2;
                i++;
                fseek(file,-size,SEEK_CUR);
                fwrite(&item,sizeof(item),1,file);
                break;
            }
        }
    }
    if(gtotal!=0)
    {
        gotoxy(30,j+5);
        printf("TOTAL AMOUNT = NRs. %6.2f",gtotal);
    }
    fclose(file);
    getch();
    d_mainmenu();
}
/*function to display bill window*/
void dbill()
{
    int i;
    gotoxy(20,10);
    for (i=1; i<=10; i++)
        printf("*");
    printf(" * MEDICAPS * ");
    for (i=1; i<=10; i++)
        printf("*");
    printf("\n\n");
    gotoxy(30,11);
    printf("Medical Store");

    gotoxy(32,25);
    printf("CUSTOMER'S BILL") ;
    gotoxy(13,27);
    printf("SN.   Item Name     Quantity     Rate          Total");

}
/*function to add records*/
void add ()
{
    FILE *file;
    char y[ACS],x[12];
    system("cls");
    gotoxy(25,25);
    printf("Enter new record(Y/N)?");
    while(toupper(getche())=='Y')
    {
        system("cls");
        file=fopen("record.txt","ab");
        c_code(y);
        strcpy(item.code,y);
        gotoxy(22,28);
        printf("Enter rate of the item:");
        scanf("%f",&item.rate);
        gotoxy(22,30);
        printf("Enter quantity of the item:");
        scanf("%d",&item.quantity);
        gotoxy(22,32);
        printf("Enter name of the item:");
        scanf("%s",item.name);
        fseek(file,0,SEEK_END);
        fwrite(&item,sizeof(item),1,file);
        fclose(file);
        gotoxy(22,34);
        printf("Enter new record(Y/N)?");

    }
    d_mainmenu();
}

/*function to check availability of code*/
void c_code(char y[])
{
    int flag;
    FILE *file;
    file=fopen("record.txt","rb");
    while(1)
    {
        system("cls");
        window(20,58,23,36);
        gotoxy(32,18);
        printf(" ADD ARTICLES ")  ;
        flag=1;
        rewind(file);
        gotoxy(22,25);
        printf("Enter new code of the article:");
        scanf(" %[^\n]",y);
        while(fread(&item,sizeof(item),1,file)==1)
        {
            if (strcmp(y,item.code)==0)
            {
                flag=0;
                gotoxy(26,30);
                printf("code already exists");
                gotoxy(29,32);
                printf("enter again");
                getch();
                break;
            }
        }
        if (flag==1)
            break;
    }
}


/*function to display window for item display*/
void dis_con()
{
    int i;
    system("cls");
    gotoxy(20,10);
    ;
    for (i=1; i<=10; i++)
        printf("*");
    printf(" * MEDICAPS * ");
    for (i=1; i<=10; i++)
        printf("*");
    printf("\n\n");
    gotoxy(30,11);
    printf("Medical Store");

    gotoxy(32,17);
    printf("RECORDS") ;
    gotoxy(18,23);
    printf ("SN   Item Name   Item Code      Rate     Quantity");
}

/*function to display in screen*/
void display(rec *item,int i,int j)
{
    gotoxy(16,i);//textcolor(13);
    printf("%4d",j);
    printf("%9s",item->name);
    printf("%12s",item->code);
    printf("%14.2f",item->rate);
    printf("%11d",item->quantity);
}
/*function to display box*/
void window(int a,int b,int c,int d)
{
    int i;
    system("cls");
    gotoxy(20,10);
    for (i=1; i<=10; i++)
        printf("*");
    printf(" * MEDICAPS * ");
    for (i=1; i<=10; i++)
        printf("*");
    printf("\n\n");
    gotoxy(30,11);
    printf("Medical Store");
    for (i=a; i<=b; i++)
    {
        gotoxy(i,17);
        printf("\xcd");
        gotoxy(i,19);
        printf("\xcd");
        gotoxy(i,c);
        printf("\xcd");
        gotoxy(i,d);
        printf("\xcd");
    }

    gotoxy(a,17);
    printf("\xc9");
    gotoxy(a,18);
    printf("\xba");
    gotoxy(a,19);
    printf("\xc8");
    gotoxy(b,17);
    printf("\xbb");
    gotoxy(b,18);
    printf("\xba");
    gotoxy(b,19);
    printf("\xbc");
    for(i=c; i<=d; i++)
    {
        gotoxy(a,i);
        printf("\xba");
        gotoxy(b,i);
        printf("\xba");
    }
    gotoxy(a,c);
    printf("\xc9");
    gotoxy(a,d);
    printf("\xc8");
    gotoxy(b,c);
    printf("\xbb");
    gotoxy(b,d);
    printf("\xbc");
}


