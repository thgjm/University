#include <iostream>

#define UNDERLINE "\033[4m"

#define CLOSEUNDERLINE "\033[0m"

using namespace std;

struct answer_options
{
    unsigned int id;
    unsigned int question_id;
    char option[20];
    bool deleted;
};

struct poll_info
{
    unsigned int id;
    char question[50];
    bool deleted;
};

void read_seq(char file_name[])
{
    poll_info part;
    FILE *fp=fopen("../questions.txt","rb");
    while (!feof(fp)) {
        fread(&part, sizeof(poll_info), 1, fp);
        if(!feof(fp)&& part.id==0) {cout<<"ID: "<<part.id<<" Question: "<<part.question<<endl; break; }
    }
    fclose(fp);
}

void read_all_q(char file_name[])
{
    FILE *fp_q=fopen(file_name,"rb");
    fseek(fp_q,0,SEEK_END);
    long pos=ftell(fp_q);
    fseek(fp_q,0,SEEK_SET);
    int count=pos/sizeof(poll_info);
    poll_info* poll=new poll_info[count];
    fread(poll, sizeof(poll_info), count, fp_q);
    for (int i=0; i<count; i++)
        cout<<"ID: "<<poll[i].id<<" Question: "<<poll[i].question<<" Deleted: "<<poll[i].deleted<<endl;
    delete[] poll;
    fclose(fp_q);
}

void read_all_a(char file_name[])
{
    FILE *fp_a=fopen(file_name,"rb");
    fseek(fp_a,0,SEEK_END);
    long pos=ftell(fp_a);
    fseek(fp_a,0,SEEK_SET);
    int count=pos/sizeof(answer_options);
    answer_options* part=new answer_options[count];
    fread(part, sizeof(answer_options), count, fp_a);
    for (int i=0; i<count; i++)
        cout<<"ID: "<<part[i].id<<" Question ID: "<<part[i].question_id<<" Option: "<<part[i].option<<" Deleted: "<<part[i].deleted<<endl;
    delete[] part;
    fclose(fp_a);
}


int main()
{
    char questions_file_name[100]="../questions.txt";
    char answers_file_name[100]="../answers.txt";
    //read_seq(questions_file_name);
    read_all_q(questions_file_name);
    cout<<endl<<"-------------------"<<endl<<endl;
    read_all_a(answers_file_name);
    rename("../temp_q", "../questions.txt");
    rename("../temp_a", "../answers.txt");
    return 0;
}
