#include <iostream>

using namespace std;

struct questions_info
{
    unsigned int id;
    char question[50];
};

struct answers_info
{
    unsigned int id;
    unsigned int question_id;
    char option[20];
};

/*int main()
{
    unsigned int id_q=0;
    unsigned int id_a=0;
    poll_info poll;
    answer_options answer;
    FILE *fp_q=fopen("../questions.txt","rb");
    FILE *fp_a=fopen("../answers.txt","rb");
    fseek(fp_q,0,SEEK_END);
    long pos_q=ftell(fp_q);
    if (pos_q>0)
    {
        fseek(fp_q,pos_q-sizeof(poll_info),SEEK_SET);
        fread(&poll,sizeof(poll),1,fp_q);
        id_q=poll.id+1;
    }

    fseek(fp_a,0,SEEK_END);
    long pos_a=ftell(fp_a);
    if (pos_a>0)
    {
        fseek(fp_a,pos_a-sizeof(answer_options),SEEK_SET);
        fread(&answer,sizeof(answer),1,fp_a);
        id_a=answer.id+1;
    }
    fclose(fp_q);
    fclose(fp_a);

    fp_q=fopen("../questions.txt","ab");
    fp_a=fopen("../answers.txt","ab");
    fseek(fp_q,0,SEEK_END);
    fseek(fp_a,0,SEEK_END);
    while (true)
    {
        poll.id=id_q;
        cout<<"enter the question (type ' ' to exit)\n";
        cin.getline(poll.question, sizeof(poll.question));
        if (poll.question[0]==' ') break;
        else fwrite(&poll,sizeof(poll),1,fp_q);
        cout<<"enter the options to the answer (type ' ' to exit)\n";
        while(true)
        {
            answer.id=id_a;
            answer.question_id=id_q;
            cin.getline(answer.option, sizeof(answer.option));
            if(answer.option[0]==' ') break;
            else fwrite(&answer,sizeof(answer),1,fp_a);
            id_a++;
        }
        id_q++;
    }
    fclose(fp_q);
    fclose(fp_a);
    return 0;
}*/


void write_in_a_file(char file_name_q[], char file_name_a[])
{
    unsigned int id_q=0;
    unsigned int id_a=0;
    questions_info new_question;
    answers_info new_answer;
    FILE *fp_q=fopen(file_name_q,"rb");
    FILE *fp_a=fopen(file_name_a,"rb");
    fseek(fp_q,0,SEEK_END);
    long pos_q=ftell(fp_q);
    if (pos_q>0)
    {
        fseek(fp_q,pos_q-sizeof(questions_info),SEEK_SET);
        fread(&new_question,sizeof(new_question),1,fp_q);
        id_q=new_question.id+1;
    }

    fseek(fp_a,0,SEEK_END);
    long pos_a=ftell(fp_a);
    if (pos_a>0)
    {
        fseek(fp_a,pos_a-sizeof(answers_info),SEEK_SET);
        fread(&new_answer,sizeof(new_answer),1,fp_a);
        id_a=new_answer.id+1;
    }
    fclose(fp_q);
    fclose(fp_a);

    fp_q=fopen(file_name_q,"ab");
    fp_a=fopen(file_name_a,"ab");
    fseek(fp_q,0,SEEK_END);
    fseek(fp_a,0,SEEK_END);
    while (true)
    {
        new_question.id=id_q;
        cout<<"enter the question (type ' ' to exit)\n";
        cin.getline(new_question.question, sizeof(new_question.question));
        if (new_question.question[0]==' ') break;
        else fwrite(&new_question,sizeof(new_question),1,fp_q);
        cout<<"enter the options to the answer (type ' ' to exit)\n";
        while(true)
        {
            new_answer.id=id_a;
            new_answer.question_id=id_q;
            cin.getline(new_answer.option, sizeof(new_answer.option));
            if(new_answer.option[0]==' ') break;
            else fwrite(&new_answer,sizeof(new_answer),1,fp_a);
            id_a++;
        }
        id_q++;
    }
    fclose(fp_q);
    fclose(fp_a);
}

int main()
{
    write_in_a_file("../questions.txt", "../answers.txt");
    return 0;
}
