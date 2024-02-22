#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Third version: even more functionality and initial error treatment
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
    float percentage;
    float mark;
    float min_mark;
} Exam;

typedef struct subject
{
    char name[31];
    int ECTS;
    float global_mark;
    int exam_num;
    Exam exams[20];
} Subject;

//Checks if the float resulting from doing strtof() of a char is a number.
int is_input_num(char input, float num)
{
    if (input > 0 && input != '0' && num == 0)
    {
        printf("----- Input is not a number -----\n");
        return 0;
    }
    return 1;
}

//Empties stdin so it doesn't add extra inputs.
void clean_stdin()
{
    while (getchar() != '\n');
}

//Sets any type of variable to 0.
void clean_arr(void* input)
{
    memset(input, 0, sizeof(*input));
}

//Rewinds, writes a subject and closes a file.
void rw_wr_file(Subject* temp, FILE* file)
{
    rewind(file);
    fwrite(temp, sizeof(Subject), 1, file);
    fclose(file);
}

//Asks the user to press Enter key to continue.
void wait_enter()
{
    printf("\n     Press [Enter] key to continue. ");
    clean_stdin();
    getchar();
}

//Receives a string, cleans it, asks the user
//for the name to write on it and returns it.
char* get_name(char* name)
{
    clean_arr(name);
    clean_stdin();
    printf("     Write subject's name: ");
    scanf("%30[^\n]", name);
    return name;
}

//Prints as text the struct exam.
void to_string_ex(Exam *in)
{
    printf("     Name: %s\n", in->name);

    int i = 0;

    while (i < in->time_num)
    {
        printf("       - Date: %s  Hour: %s\n", in->times[i].date, in->times[i].hour);
        i++;
    }

    printf("     Percentage: %f\n", in->percentage);
    printf("     Min Mark: %f\n", in->min_mark);
    printf("     Mark: %f\n\n", in->mark);
}

//Prints as text the struct subject.
void to_string(Subject *in)
{
    printf("\nName: %s\n", in->name);
    printf("ECTS: %i\n", in->ECTS);
    printf("Global Mark: %f\n", in->global_mark);
    printf("Exam Number: %i\n\n{\n\n", in->exam_num);

    int i = 0;

    while (i < in->exam_num)
    {
        to_string_ex(&in->exams[i]);
        i++;
    }

    printf("}\n");
}

//Attempts to open a file by its name and displays it as a subject.
void show_subject(char *name)
{
    Subject *temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(name, "r+");

    if(file)
    {
        fread(temp, sizeof(Subject), 1, file);
        fclose(file);
        to_string(temp);
    }
    else
        printf("\n----- Specified file does not exist, please try again -----\n");

    free(temp);
}

//Calculates the global mark of a subject based on
//the percentage and minimum mark of every exam.
void calc_global_mark(Subject* temp)
{
    temp->global_mark = 0;

    int i = 0;
    while (i < temp->exam_num)
    {
        if (temp->exams[i].mark >= temp->exams[i].min_mark)
            temp->global_mark += temp->exams[i].mark * (temp->exams[i].percentage / 100);
        i++;
    }

    printf("     New global mark is %f\n", temp->global_mark);
}

//Edits the float indicated by mark based on users input, asks for the float and
//has an error msg if the input is not in [0, max_num] or not a number,
//restarting the petition.
void edit_float(char* msg, char* errmsg, float max_num, float* mark)
{
    int i = 0;
    float dummy;
    char input[31];

    while (!i)
    {
        printf("     %s ", msg);
        scanf("%3[^\n]", input);
        clean_stdin();
        dummy = strtof(input, NULL);
        if (strtof(input, NULL) <= max_num && strtof(input, NULL) >= 0 && is_input_num(input[0], dummy))
        {
            *mark = dummy;
            i = 1;
        }
        else
            printf("\n%s\n", errmsg);
    }
}

//Edits the int indicated by mark based on users input, asks for the int with msg
//and has an error msg if the input is not in [0, max_num] or not a number,
//restarting the petition.
void edit_int(char* msg, char* errmsg, int max_num, int* mark)
{
    int i = 0;
    int dummy;
    char input[31];

    while (!i)
    {
        printf("     %s ", msg);
        scanf("%3[^\n]", input);
        clean_stdin();
        dummy = (int) strtol(input, NULL, 0);
        if ((int) strtol(input, NULL, 0) <= max_num && (int) strtol(input, NULL, 0) >= 0 && is_input_num(input[0], (float) dummy))
        {
            *mark = dummy;
            i = 1;
        }
        else
            printf("\n%s\n", errmsg);
    }
}

//Receives inputs from the user and writes them in the
//date and hour of a time after cleaning them.
void edit_time(Time* time)
{
    clean_arr(time->date);
    printf("     Write date (dd/mm): ");
    scanf("%5[^\n]", time->date);
    clean_stdin();

    clean_arr(time->hour);
    printf("     Write hour (00:00): ");
    scanf("%5[^\n]", time->hour);
    clean_stdin();
}

//Starts the process to add information to an exam.
void new_exam(Subject * temp, int i)
{
    char input[2];

    //Asks the user for the different information of the exam
    printf("     Write the exam %i name (máx 30): ", i);
    scanf("%30[^\n]", temp->exams[i].name);
    clean_stdin();

    edit_float("Write the percentage:", "----- Number should be between 0 and 100 -----", 100, &temp->exams[i].percentage);
    edit_float("Write minimum mark out of ten:", "----- Number should be between 0 and 10 -----", 10, &temp->exams[i].min_mark);
    edit_float("Write your mark out of ten (0 if you do not have): ", "----- Number should be between 0 and 10 -----", 10, &temp->exams[i].mark);

    //Asks user for the possible dates of the exam
    printf("     Does it have a date? y/n: ");
    scanf(" %1c", input);
    clean_stdin();

    if (input[0] == 'y')
    {
        //If the answer is 'y' starts a loop asking for the information, restarting if the format is not correct
        int k = 0;
        while (!k)
        {
            clean_arr(input);
            printf("     How many? (máx 2): ");
            scanf("%1[^\n]", input);
            clean_stdin();

            //TODO: puedo mirar a ver si input[1] == \n y sacar un nuevo error de ahí
            if(is_input_num(input[0], strtof(input, NULL)))
                temp->exams[i].time_num = (int) strtol(input, NULL, 0);
            else
                continue;

            clean_arr(input);
            if (temp->exams[i].time_num < 3 && temp->exams[i].time_num >= 0)
            {
                int j = 0;
                while (j < temp->exams[i].time_num)
                {
                    edit_time(&temp->exams[i].times[j]);
                    j++;
                }
                k = 1;
            }
            else
            {
                printf("\n----- Number is not valid -----\n\n");
                clean_arr(input);
            }
        }
    }
}

//Starts the process to add information to a new subject.
void new_subject()
{
    Subject *temp = (Subject *)malloc(sizeof(Subject));
    clean_stdin();

    //Asks the user for the information of the subject
    printf("     Write subject's name (máx 30): ");
    scanf("%30[^\n]", temp->name);
    clean_stdin();

    edit_int("Write subject's ECTS:", "----- Number should be between 0 and 100 -----", 100, &temp->ECTS);
    edit_int("Write subject's evaluation inputs (máx 20):", "----- Number should be between 0 and 20 -----", 20, &temp->exam_num);

    //Starts a loop of creating exams for the subject based on exam_num
    int i = 0;
    while (i < temp->exam_num)
    {
        new_exam(temp, i);
        i++;
    }

    //Calculates the global mark
    calc_global_mark(temp);

    printf("----- All subject data registered -----\n\n");

    //Opens or creates a file and writes the subject
    FILE *file = fopen(temp->name, "w");
    fwrite(temp, sizeof(Subject), 1, file);
    fclose(file);

    //Shows it and finishes the process
    show_subject(temp->name);

    free(temp);
}

//Adds a new exam following the last exam of the subject
//based on the name of the file that stores it and has an
//error msg if the file does not exist, restarting the petition.
void add_exam(char* input)
{
    //Creates a temporarily subject and attempts to open de file
    Subject* temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(input, "r+");

    //If the file exists reads it and adds the exam indicating the position
    if(file)
    {
        fread(temp, sizeof(Subject), 1, file);
        printf("\n     You are adding an exam in last position (%i)...\n\n", temp->exam_num);

        new_exam(temp, temp->exam_num);
        temp->exam_num ++;
        calc_global_mark(temp);
        rw_wr_file(temp, file);
        show_subject(input);
    }
    else
        printf("\n----- Specified file does not exist, please try again -----\n\n");
    free(temp);
}

//Cleans the exam values of the last exam of a subject and
//reduces exam_num by 1 based on the name of the file that stores it and has an
//error msg if the file does not exist, restarting the petition.
void remove_exam(char* name)
{
    //Creates a temporarily subject and attempts to open de file
    Subject *temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(name, "r+");

    //If the file exists reads it and removes the last exam
    if(file)
    {
        fread(temp, sizeof(Subject), 1, file);

        int exam = temp->exam_num - 1;

        clean_arr(temp->exams[exam].name);

        temp->exams[exam].percentage = 0;
        temp->exams[exam].mark = 0;
        temp->exams[exam].min_mark = 0;

        int i = 0;

        while (i < temp->exams[exam].time_num)
        {
            clean_arr(temp->exams[exam].times[i].date);
            clean_arr(temp->exams[exam].times[i].hour);
            i++;
        }

        temp->exams[exam].time_num = 0;

        temp->exam_num -= 1;

        calc_global_mark(temp);
        rw_wr_file(temp, file);
        show_subject(name);
    }
    else
        printf("\n----- Specified file does not exist, please try again -----\n\n");
    free(temp);
}

//Starts the process of editing the different values of a subject
//showing a menu for the user to choose the different options.
void edit_exam(char *name)
{
    int select;
    int t_select;
    char input[31];

    //Creates a temporarily subject and attempts to open de file
    Subject *temp = (Subject *)malloc(sizeof(Subject));
    FILE *file = fopen(name, "r+");

    //If the file exists reads it and starts the meno
    if(file)
    {
        fread(temp, sizeof(Subject), 1, file);
        show_subject(name);

        //The menu will show until k = 1 which occurs after one successful edition
        int k = 0;
        while (!k)
        {
            //Receives the input of the user referring the exam that wants to edit and stores it in select
            printf("\n     You are checking %s which exam do you want to change? (0,1,2...): ", temp->name);
            scanf("%2[^\n]", input);
            //clean_stdin();

            //If the input is a number adds it to select
            if(is_input_num(input[0], strtof(input, NULL)))
                select = (int) strtol(input, NULL, 0);
            //If not, prints the error and restarts the loop
            else
                continue;

            //If select is between [0, exam_num) starts the edition menu of the selected exam
            if(select < temp->exam_num && select >= 0)
            {
                //Cleans the input string and asks the user
                clean_arr(input);
                to_string_ex(&temp->exams[select]);
                printf("     What do you want to change?\n\n     - Name (n)\n     - Times (t)\n     - Percentage (p)\n     - Mark (m)\n     - Min Mark (i)\n     - Nothing (x)\n\n     Write it here: ");
                scanf(" %1c", &input[0]);

                //Starts a switch that has a process for every input
                switch (input[0])
                {
                case 'n':
                    printf("\n     Write new name: ");
                    scanf("%30[^\n]", temp->exams[select].name);
                    clean_stdin();
                    rw_wr_file(temp, file);
                    show_subject(name);
                    free(temp);
                    break;

                case 't':
                    printf("\n     You are checking %s which date do you want to change? (0/1): ", temp->exams[select].name);
                    scanf("%1[^\n]", input);
                    t_select = (int) strtol(input, NULL, 0);
                    clean_stdin();

                    //Receives the int selected and does the process if it is between [0,2)
                    if (t_select < temp->exams[select].time_num && t_select >= 0)
                        edit_time(&temp->exams[select].times[t_select]);
                    //If not, stops showing a message
                    else
                        printf("----- The date introduced does not exist -----");

                    rw_wr_file(temp, file);
                    show_subject(name);
                    free(temp);
                    break;

                case 'p':
                    edit_float("Write new percentage:", "----- Number should be between 0 and 100 -----", 100, &temp->exams[select].percentage);
                    calc_global_mark(temp);
                    rw_wr_file(temp, file);
                    show_subject(name);
                    free(temp);
                    break;

                case 'm':
                    edit_float("Add your new mark:", "----- Number should be between 0 and 10 -----", 10, &temp->exams[select].mark);
                    calc_global_mark(temp);
                    rw_wr_file(temp, file);
                    show_subject(name);
                    free(temp);
                    break;

                case 'i':
                    edit_float("Write new min mark:", "----- Number should be between 0 and 10 -----", 10, &temp->exams[select].min_mark);
                    calc_global_mark(temp);
                    rw_wr_file(temp, file);
                    show_subject(name);
                    free(temp);
                    break;

                case 'x':
                    break;

                default:
                    printf("----- Input not valid, try again -----\n\n");
                    break;
                }
                k = 1;
            }
            //If not, shows a message and restarts the loop
            else
            {
                printf("\n----- Number not valid -----\n\n");
                wait_enter();
            }
        }
    }
    //If not, stops showing a message
    else
        printf("\n----- Specified file does not exist, please try again -----\n");

}

//Starts the main process showing a menu for the user to choose the different options.
void program()
{
    int selection;
    char input[31];
    int k = 0;

    //The menu will show until k = 1 which occurs after the user selects exit
    while (!k)
    {
        //Shows the menu and receives users input
        printf("                                _ \n _ __ ___  _   _    _   _ _ __ (_)\n| '_ ` _ \\| | | |  | | | | '_ \\| |\n| | | | | | |_| |  | |_| | | | | |\n|_| |_| |_|\\__, |___\\__,_|_| |_|_|\n           |___/_____|            ");
        printf("\n\n        Welcome to My_Uni!!\n\n    -----Select Your Option-----\n\n");
        printf("      1 - Create New Subject\n      2 - See a Subject\n      3 - Add Exam\n      4 - Edit Exam\n      5 - Remove Last Exam\n      6 - Exit\n\n     Write your selection here: ");

        scanf("%2[^\n]", input);
        selection = (int) strtol(input, NULL, 0);

        //Starts a switch that has a process for every input
        switch (selection)
        {
        case 1:
            //TODO: funciona distinto wait_enter() por alguna razón
            new_subject();
            wait_enter();
            break;

        case 2:
            show_subject(get_name(input));
            wait_enter();
            break;

        case 3:
            add_exam(get_name(input));
            wait_enter();
            break;

        case 4:
            edit_exam(get_name(input));
            wait_enter();
            break;

        case 5:
            remove_exam(get_name(input));
            wait_enter();
            break;

        case 6:
            k = 1;
            printf("\n     See you later!!");
            break;

        default:
            printf("\n----- Input not valid, try again -----\n");
            wait_enter();
            break;
        }
    }
}

int main(void)
{
    program();
    return 0;
}
