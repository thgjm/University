#include <iostream>
#include <stdlib.h>


using namespace std;

struct questions_info
{
    unsigned int id;
    char question[50];
    bool deleted;
};

struct answers_info
{
    unsigned int id;
    unsigned int question_id;
    char option[20];
    bool deleted;
};

int main()
{
    int id=0;
char file_name_q[20]="../questions.txt";
FILE* fp_q=fopen(file_name_q, "r+b");
    if(!fp_q)
    {
        perror("Error");
        return 0;
    }
    questions_info part;
    fseek(fp_q,sizeof(questions_info)*id,SEEK_SET);
    fread(&part, sizeof(questions_info), 1, fp_q);
    part.deleted=true;
    fseek(fp_q, -sizeof(questions_info),SEEK_CUR);
    fwrite(&part, sizeof(questions_info), 1, fp_q);
    fclose(fp_q);
}
