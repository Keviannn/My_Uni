#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Second version: increased functionality but still no error treatment
typedef struct time
{
    char date[6];
    char hour[6];
} Time;

typedef struct exam
{
    char name[31];
    int time_num;
    Time times[2];
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
    Subject *temp = (Subject *)malloc(sizeof(Subject));

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
    int num2;
    int i;
    char confirm;

    printf("Write subject's name (máx 30): ");
    scanf("%s", subject_name);
    strcpy(temp->name, subject_name);

    strcpy(command, "echo > ");
    strcat(command, subject_name);
    system(command);

    printf("Write subject's ETCS: ");
    scanf("%i", &ETCS);
    temp->ETCS = ETCS;

    printf("Write subject's evaluation inputs (máx 20): ");
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
            num2 = 0;
            printf("How many? (máx 2): ");
            scanf("%i", &num2);
            temp->exams[i].time_num = num2;

            int j = 0;
            while (j < temp->exams[i].time_num)
            {
                printf("Write date (dd/mm): ");
                scanf("%s", date);
                strcpy(temp->exams[i].times[j].date, date);

                printf("Write hour (00:00): ");
                scanf("%s", hour);
                strcpy(temp->exams[i].times[j].hour, hour);

                j++;
            }
        }

        if (temp->exams[i].mark >= temp->exams[i].min_mark)
            temp->global_mark += temp->exams[i].mark * (percentaje / 100);

        i++;
    }

    printf("-----ALL DATA REGISTERED-----\n\n");

    FILE *file = fopen(subject_name, "w");
    fwrite(temp, sizeof(Subject), 1, file);
    fclose(file);

    free(temp);
}

void to_string(Subject *in)
{
    printf("\nName: %s\n", in->name);
    printf("ETCS: %i\n", in->ETCS);
    printf("Global Mark: %f\n", in->global_mark);
    printf("Exam Number: %i\n\n{\n\n", in->exam_num);

    int i = 0;

    while (i < in->exam_num)
    {
        printf("    Name: %s\n", in->exams[i].name);
        printf("     - Times %i:\n", in->exams[i].time_num);

        int j = 0;

        while (j < in->exams[i].time_num)
        {
            printf("       - Date: %s  Hour: %s\n", in->exams[i].times[j].date, in->exams[i].times[j].hour);
            j++;
        }

        printf("     - Percentaje: %f\n", in->exams[i].percentaje);
        printf("     - Min Mark: %f\n", in->exams[i].min_mark);
        printf("     - Mark: %f\n\n", in->exams[i].mark);

        i++;
    }

    printf("}\n");
}

void to_string_ex(Exam *in)
{
    printf("\n     Name: %s\n", in->name);

    int i = 0;

    while (i < in->time_num)
    {
        printf("        Date: %s\n", in->times[i].date);
        printf("        Hour: %s\n\n", in->times[i].hour);

        i++;
    }

    printf("     Percentaje: %f\n", in->percentaje);
    printf("     Min Mark: %f\n", in->min_mark);
    printf("     Mark: %f\n\n", in->mark);
}

void show_subject(char *name)
{
    Subject *temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(name, "r+");
    fread(temp, sizeof(Subject), 1, file);
    fclose(file);

    to_string(temp);

    free(temp);
}

void edit_subject(char *name)
{
    int ex_num;
    int select;
    char option;
    int i = 0;

    Subject *temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(name, "r+");
    fread(temp, sizeof(Subject), 1, file);

    printf("\n     You are cheking %s which exam do you want to change? (0,1,2...): ", temp->name);
    scanf("%i", &ex_num);

    if (ex_num >= temp->exam_num)
        temp->exam_num++;

    to_string_ex(&temp->exams[ex_num]);
    printf("     What do you want to change?\n\n     - Name (n)\n     - Times (t)\n     - Percentaje (p)\n     - Mark (m)\n     - Min Mark (i)\n\n     Write it here: ");
    scanf(" %c", &option);

    switch (option)
    {
    case 'n':
        printf("\n     Write new name: ");
        scanf("%s", temp->exams[ex_num].name);

        rewind(file);
        fwrite(temp, sizeof(Subject), 1, file);
        fclose(file);

        show_subject(name);

        free(temp);
        break;

    case 't':
        printf("\n     You are cheking %s which date do you want to change? (0/1): ", temp->exams[ex_num].name);
        scanf("%i", &select);
        printf("\n     Write new date: ");
        scanf("%s", temp->exams[ex_num].times[select].date);
        printf("     Write new hour: ");
        scanf("%s", temp->exams[ex_num].times[select].hour);

        if (temp->exams[ex_num].time_num == 0 || temp->exams[ex_num].time_num == 1)
            temp->exams[ex_num].time_num++;

        rewind(file);
        fwrite(temp, sizeof(Subject), 1, file);
        fclose(file);

        show_subject(name);
        free(temp);
        break;

    case 'p':
        printf("\n     Write new percentaje: ");
        scanf("%f", &temp->exams[ex_num].percentaje);

        temp->global_mark = 0.0;

        while (i < temp->exam_num)
        {
            if (temp->exams[i].mark >= temp->exams[i].min_mark)
                temp->global_mark += temp->exams[i].mark * (temp->exams[i].percentaje / 100);
            i++;
        }

        rewind(file);
        fwrite(temp, sizeof(Subject), 1, file);
        fclose(file);

        show_subject(name);

        free(temp);
        break;

    case 'm':
        printf("\n     Add your new mark: ");
        scanf("%f", &temp->exams[ex_num].mark);
        printf("\n     New mark is %f\n", temp->exams[ex_num].mark);

        temp->global_mark = 0.0;

        while (i < temp->exam_num)
        {
            if (temp->exams[i].mark >= temp->exams[i].min_mark)
                temp->global_mark += temp->exams[i].mark * (temp->exams[i].percentaje / 100);
            i++;
        }

        printf("     New global mark is %f\n", temp->global_mark);

        rewind(file);
        fwrite(temp, sizeof(Subject), 1, file);
        fclose(file);

        show_subject(name);

        free(temp);
        break;

    case 'i':
        printf("\n     Write new min mark: ");
        scanf("%f", &temp->exams[ex_num].min_mark);

        temp->global_mark = 0.0;

        while (i < temp->exam_num)
        {
            if (temp->exams[i].mark >= temp->exams[i].min_mark)
                temp->global_mark += temp->exams[i].mark * (temp->exams[i].percentaje / 100);
            i++;
        }

        rewind(file);
        fwrite(temp, sizeof(Subject), 1, file);
        fclose(file);

        show_subject(name);

        free(temp);
        break;

    default:
        printf("\n     Input not valid, try again.\n");
        break;
    }
}

void program()
{
    int selection;

    char file[31];

    int i;
    i = 0;

    while (!i)
    {
        system("cat head.txt");
        printf("\n\n        Welcome to My_Uni!!\n\n    -----Select Your Option-----\n\n");
        printf("      1 - Create New Subject\n      2 - See a Subject\n      3 - Edit Subject\n      4 - Exit\n\n     Write your selection here: ");
        scanf("%i", &selection);
        printf("\n");

        switch (selection)
        {
        case 1:
            new_subject();
            break;

        case 2:
            printf("     Write subject's name: ");
            scanf("%s", file);
            show_subject(file);
            printf("\nPress [Enter] key to continue.\n ");
            while (getchar() != '\n')
                ;
            getchar();
            break;

        case 3:
            printf("     Write subject's name: ");
            scanf("%s", file);
            edit_subject(file);
            printf("\nPress [Enter] key to continue.\n ");
            while (getchar() != '\n')
                ;
            getchar();
            break;

        case 4:
            i = 1;
            break;

        default:
            printf("     Input not valid, try again.\n");
            printf("     Press [Enter] key to continue.\n ");
            while (getchar() != '\n');
            getchar();
            break;
        }
    }
}

int main(void)
{
    program();
    return 0;
}
