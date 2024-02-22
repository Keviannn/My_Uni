#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//First version: small functionality and with no error treatment
typedef struct exam
{
    char name[31];
    char date[6];
    char hour[6];
    float percentaje;
    float mark;
    float min_mark;
} Exam;

typedef struct subject
{
    char name[31];
    int ETCS;
    float global_mark;
    int exam_num;
    Exam exams[20];
} Subject;

void new_subject()
{
    Subject* temp = (Subject*) malloc(sizeof(Subject));

    char command[42];

    char exam_name[31];
    char date[6];
    char hour[6];
    float percentaje;
    float mark;
    float min_mark;

    char subject_name[31];
    int ETCS;
    float global_mark;
    Exam exams[20];

    int num;
    int i;
    char confirm;

    printf("Write subjet's name (máx 30): ");
    scanf("%s", subject_name);
    strcpy(temp->name, subject_name);

    strcpy(command, "echo > ");
    strcat(command, subject_name);
    system(command);

    printf("Write subjet's ETCS: ");
    scanf("%i", &ETCS);
    temp->ETCS = ETCS;

    printf("Write subjet's evaluation inputs (máx 20): ");
    scanf("%d", &num);
    temp->exam_num = num;

    i = 0;

    while (i < num)
    {
        printf("Write the name without spaces: ");
        scanf("%s", exam_name);
        strcpy(temp->exams[i].name, exam_name);

        printf("Write the percentaje: ");
        scanf("%f", &percentaje);
        temp->exams[i].percentaje = percentaje;

        printf("Write minimun mark: ");
        scanf("%f", &min_mark);
        temp->exams[i].min_mark = min_mark;

        printf("Write your mark: ");
        scanf("%f", &mark);
        temp->exams[i].mark = mark;

        printf("Does it have a date? y/n: ");
        scanf(" %c", &confirm);

        if (confirm == 'y')
        {
            printf("Write date (dd/mm): ");
            scanf("%s", date);
            strcpy(temp->exams[i].date, date);

            printf("Write hour (00:00): ");
            scanf("%s", hour);
            strcpy(temp->exams[i].hour, hour);
        }

        if(temp->exams[i].mark >= temp->exams[i].min_mark)
            temp->global_mark += temp->exams[i].mark * (percentaje/100);

        i++;
    }


    printf("-----ALL DATA REGISTERED-----\n\n");

    FILE* file = fopen(subject_name, "w");
    fwrite(temp, sizeof(Subject), 1, file);
    fclose(file);

    free(temp);
}

void to_string(Subject* in)
{
    printf("\nName: %s\n", in->name);
    printf("ETCS: %i\n", in->ETCS);
    printf("Global Mark: %f\n", in->global_mark);
    printf("Exam Number: %i\n\n{\n\n", in->exam_num);

    int i = 0;

    while (i < in->exam_num)
    {
        printf("    Name: %s\n", in->exams[i].name);
        printf("    Date: %s\n", in->exams[i].date);
        printf("    Hour: %s\n", in->exams[i].hour);
        printf("    Percentaje: %f\n", in->exams[i].percentaje);
        printf("    Min Mark: %f\n", in->exams[i].min_mark);
        printf("    Mark: %f\n\n", in->exams[i].mark);

        i++;
    }

    printf("}\n");    

}

void show_subject(char* name)
{
    Subject* temp = (Subject*) malloc(sizeof(Subject));
    FILE* file = fopen(name, "r+");
    fread(temp, sizeof(Subject), 1, file);
    fclose(file);

    to_string(temp);

    free(temp);
}

void edit_subject(char* name)
{
    int i;
    char option;

    Subject* temp = (Subject*) malloc(sizeof(Subject));
    FILE* file = fopen(name, "r+");
    fread(temp, sizeof(Subject), 1, file);
    
    i = 0;

    temp->global_mark = 0.0;

    while(i < temp->exam_num)
    {
        printf("You are checking %s and the mark is %f\nDo you want to change it? y/n: ", temp->exams[i].name, temp->exams[i].mark);
        scanf(" %c", &option);

        if (option == 'y')
        {
            printf("Add your new mark: ");
            scanf("%f", &temp->exams[i].mark);
            printf("New mark is %f\n", temp->exams[i].mark);
        }

        if(temp->exams[i].mark >= temp->exams[i].min_mark)
            temp->global_mark += temp->exams[i].mark * (temp->exams[i].percentaje/100);
        printf("New global mark is %f\n", temp->global_mark);

        i++;
        printf("-------------- %i --------------\n", i);
    }
    
    
    rewind(file);
    fwrite(temp, sizeof(Subject), 1, file);
    fclose(file);
    show_subject(name);
    free(temp);
}

void program()
{
    system("cat head.txt");
    printf("\n\n        Wellcome to My_Uni!!\n\n    -----Select Your Option-----\n\n");
    printf("      1 - Create New Subject\n      2 - See a Subject\n      3 - Edit Marks\n      4 - Exit\n\n     Write your selecion here: ");

    int selection;
    scanf("%i", &selection);
    printf("\n");

    char file[31];

    switch (selection)
    {
    case 1:
        new_subject();
        program();
        break;
    case 2:
        printf("     Write subject's name: ");
        scanf("%s", file);
        show_subject(file);
        printf("\nPress [Enter] key to continue.\n ");
        while(getchar()!='\n');
        getchar();
        program();
        break;
    case 3:
        printf("     Write subject's name: ");
        scanf("%s", file);
        edit_subject(file);
        printf("\nPress [Enter] key to continue.\n ");
        while(getchar()!='\n');
        getchar();
        program();
        break;
    case 4:
        break;
    default:
        printf("     Input not valid, try again.\n");
        printf("     Press [Enter] key to continue.\n ");
        while(getchar()!='\n');
        getchar();
        program();
        break;
    }
}

int main(void)
{
    program();
    return 0;   
}


