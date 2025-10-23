#include <iostream>
#include <stdlib.h>

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

struct user_answers
{
    char question[50];
    char option[20];
};

void read_seq_q(char[], int&);

void read_seq_a(char[], int, int&, int&);

void add_a_question(char[], char[]);

bool is_file_empty(FILE*);

void return_to_the_menu();

int main()
{
    user_answers current_answers[20];
    bool return_f=false;
    int choice = -1; // Initialize choice
    while (choice != 0) { // Loop until the user chooses to exit
        FILE* fp_q;
        FILE* fp_a;
        int last_q_id=-1, last_a_id=-1, last_a=-1;
        char questions_file_name[30]="../questions.txt";
        char answers_file_name[30]="../answers.txt";
        cout<<"Poll database. Please, look below at what choices you have and choose one of them.\nAfter you chose one, type in the corresponding number.\n\n";
        cout<<"1  -  choose the question and answer it.\n";
        cout<<"2  -  add a new question in the file.\n";            //done
        cout<<"3  -  delete the question/answer from the file.\n";
        cout<<"4  -  delete all content from files.\n";             //done
        cout<<"0  -  exit.\n\n";
        cin >> choice;
        switch(choice)
        {
            case 1:
                fp_q=fopen(questions_file_name, "rb");
                if(is_file_empty(fp_q))
                {
                    fclose(fp_q);
                    cout<<"The file is empty.";
                        break;
                }
                else
                {
                    cout<<"\tThe list of available questions:\n\n";
                    read_seq_q(questions_file_name, last_q_id);
                    cout<<"\nType in the number of the question you want to answer.\n\n";
                    int num;
                    cin>>num;
                    cout<<"\tThe list of available answers. Please, choose one:\n\n";
                    read_seq_a(answers_file_name, num, last_a_id);
                    break;
                }
            case 2:
                while (true) { // Loop for adding questions
                    cin.ignore();
                    add_a_question(questions_file_name, answers_file_name);
                    cout<<"\n\tWhat will be your next step?\n\n";
                    cout<<"1  -  continue adding questions.\n2  -  return to the main menu\n0  -  exit.\n\n";
                    int next_step;
                    cin>>next_step;
                    switch(next_step)
                    {
                        case 1:
                            cout<<endl;
                            break; // Continue adding questions
                        case 2:
                            system("CLS");
                            break; // Return to main menu
                        case 0:
                            return 0; // Exit program
                        default:
                            cout<<"\nerror. unknown request. please, try again.\n\n";
                            break;
                    }
                }
                break;
            case 3:
                cout<<"\n\tDo you wanna delete the question or the answer?\n\n";
                cout<<"1  -  question.\n";
                cout<<"2  -  answer.\n\n";
                int q_or_a;
                cin>>q_or_a;
                switch(q_or_a)
                {
                    case 1:
                        break;
                    case 2:
                        break;
                    default:
                        cout<<"\nerror. unknown request. try again.\n\n";
                        break;
                }
                break;
            case 4:
                fp_q=fopen(questions_file_name, "wb");
                fp_a=fopen(answers_file_name, "wb");
                fclose(fp_a);
                fclose(fp_q);
                fp_q=fopen(questions_file_name, "rb");
                fp_a=fopen(answers_file_name, "rb");
                if(is_file_empty(fp_q) && is_file_empty(fp_a)) cout<<"Removal went successfully.\n";
                else cout<<"Something went wrong.\n";
                fclose(fp_a);
                fclose(fp_q);
                cout<<"\n\tDo you want to continue?\n\n";
                cout<<"1  -  yes. Return to the main menu.\n0  -  no. Exit.\n\n";
                int next_step;
                cin>>next_step;
                switch(next_step)
                {
                    case 1:
                        system("CLS");
                        break; // Return to main menu
                    case 0:
                        return 0; // Exit program
                    default:
                        cout<<"\nerror. unknown request. please, try again.\n\n";
                        break;
                }
            case 0:
                return 0; // Exit program
            default:
                cout<<"\nerror. unknown request. please, try again.\n\n";
                break;
        }
    }
    return 0;
}

void read_seq_q(char file_name[], int &last_q_id)
{
    questions_info part;
    FILE *fp=fopen(file_name,"rb");
    while (!feof(fp))
    {
        fread(&part, sizeof(questions_info), 1, fp);
        if(!feof(fp))
        {
            last_q_id=part.id+1;
            cout<<part.id+1<<". "<<part.question<<endl;
        }
    }
    fclose(fp);
}

void read_seq_a(char file_name[], int num, int &last_a_id)
{
    answers_info part;
    FILE *fp=fopen(file_name,"rb");
    int i=1;
    while (!feof(fp))
    {
        fread(&part, sizeof(answers_info), 1, fp);
        if(!feof(fp) && part.question_id==num-1)
        {
            last_a_id=i;
            cout<<i<<". "<<part.option<<endl;
            i++;
        }
    }
    fclose(fp);
}

void add_a_question(char file_name_q[], char file_name_a[])
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
    new_question.id=id_q;
    cout<<"enter the question\n";
    cin.getline(new_question.question, sizeof(new_question.question));
    fwrite(&new_question,sizeof(new_question),1,fp_q);
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
    fclose(fp_q);
    fclose(fp_a);
}

bool is_file_empty(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
        return true;
    else return false;
}
