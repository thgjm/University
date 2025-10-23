#include <iostream>
#include <stdlib.h>

#define UNDERLINE "\033[4m"

#define CLOSEUNDERLINE "\033[0m"

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

struct user_answers
{
    unsigned int question_id;
    unsigned int answer_id;
};

void menu();

void check_questions(int[], int);

void read_seq_q(char[], int&);

int read_seq_a(char[], int, int&, int&);

void output_results(user_answers[], int, char[], char[]);

void add_a_question(char[], char[]);

void delete_the_question(char[], char[], int);

void delete_the_answer(char[], int);

void delete_the_content(char[], char[]);

bool is_file_empty(char[], char[]);

void delete_deleted_content(char[], char[]);

int main()
{
    user_answers current_answers[20];
    int answers_i=0, answered_questions[20], answered_i=0;
    FILE* fp_q;
    FILE* fp_a;
    int next_step;
    int max_q_id=-1, first_id=-1, last_id=-1;
    char questions_file_name[30]="../questions.txt";
    char answers_file_name[30]="../answers.txt";
    menu();
    int choice;
    while(true)
    {
        delete_deleted_content(questions_file_name, answers_file_name);
        cin>>choice;
        if(choice>0 && choice<5) cout<<"\n---------------------------------------\n\n";
        if(choice==1 || choice==3)
            if(is_file_empty(questions_file_name, answers_file_name))
            {
                delete_the_content(questions_file_name, answers_file_name);
                cout<<"The file is empty. Please, add the questions in.\n\n";
                cout<<"1  -  return to the main menu.\n0  -  exit.\n\n";
                while(true)
                {
                    cin>>next_step;
                    switch(next_step)
                    {
                    case 1:
                        system("CLS");
                        menu();
                        break;
                    case 0:
                        return 0;
                    default:
                        cout<<"\nerror. unknown request. please, try again.\n\n";
                        continue;
                    }
                    break;
                }
                continue;
            }
        switch(choice)
        {
        case 1:
            while(true)
            {
                check_questions(answered_questions, answered_i);
                cout<<"\t\tThe list of available questions:\n\n";
                read_seq_q(questions_file_name, max_q_id);
                cout<<"\nType in the index of the question you want to answer.\n\n";
                int num;
                while(true)
                {
                    cin>>num;
                    if(num>-1 && num < max_q_id+1)
                    {
                        current_answers[answers_i].question_id=num;
                        answered_questions[answered_i]=num;
                        answered_i++;
                        break;
                    }
                    else
                    {
                        cout<<"\nNo question with this index is found. Please, try again.\n\n";
                        continue;
                    }
                }
                cout<<"\tThe list of available answers. Please, choose one:\n\n";
                int iter = read_seq_a(answers_file_name, num, first_id, last_id);
                int answer_choice;
                cout<<endl;
                while(true)
                {
                    if(iter==0)
                    {
                        cout<<"\n\tError. No answers found.";
                        break;
                    }
                    cin>>answer_choice;
                    if(answer_choice>first_id-1 && answer_choice < last_id+1)
                    {
                        current_answers[answers_i].answer_id=answer_choice;
                        answers_i++;
                        break;
                    }
                    else
                    {
                        cout<<"\nNo answer with this index is found. Please, try again.\n\n";
                        continue;
                    }
                }
                cout<<"\n\nWhat will you do next?\n\n";
                cout<<"1  -  choose another question (results will be saved).\n2  -  see results.\n3  -  return to the main menu (results will be erased).\n0  -  exit.\n\n";
                while(true)
                {
                    cin>>next_step;
                    switch(next_step)
                    {
                    case 1:
                        break;
                    case 2:
                        cout<<"\n\n";
                        output_results(current_answers, answers_i, questions_file_name, answers_file_name);
                        answers_i=0;
                        answered_i=0;
                        system("pause");
                        cout<<"\n\n1  -  return to the main menu (this will clear your results).\n0  -  exit.\n\n";
                        int nnext_step;
                        while(true)
                        {
                            cin>>nnext_step;
                            switch(nnext_step)
                            {
                            case 1:
                                system("CLS");
                                menu();
                                break;
                            case 0:
                                return 0;
                            default:
                                cout<<"\nerror. unknown request. please, try again.\n\n";
                                continue;
                            }
                            break;
                        }
                        break;
                    case 3:
                        system("CLS");
                        menu();
                        break;
                    case 0:
                        return 0;
                    default:
                        cout<<"\nerror. unknown request. please, try again.\n\n";
                        continue;
                    }
                    break;
                }
                if(next_step==1) continue;
                break;
            }
            break;
        case 2:
            while(true)
            {
                cin.ignore();
                add_a_question(questions_file_name, answers_file_name);
                cout<<"\n\tWhat will be your next step?\n\n";
                cout<<"1  -  continue adding questions.\n2  -  return to the main menu\n0  -  exit.\n\n";
                while(true)
                {
                    cin>>next_step;
                    switch(next_step)
                    {
                    case 1:
                        cout<<"\n\n---------------------------------------\n\n";
                        break;
                    case 2:
                        system("CLS");
                        menu();
                        break;
                    case 0:
                        return 0;
                    default:
                        cout<<"\nerror. unknown request. please, try again.\n\n";
                        continue;
                    }
                    break;
                }
                if(next_step==1) continue;
                break;
            }
            if(next_step==2) continue;
        case 3:
            cout<<"\n\tDo you wanna delete the question or the answer?\n\n";
            cout<<"1  -  question.\n";
            cout<<"2  -  answer.\n\n";
            int q_or_a;
            while(true)
            {
                cin>>q_or_a;
                switch(q_or_a)
                {
                case 1:
                    cout<<"\tThe list of available questions:\n\n";
                    read_seq_q(questions_file_name, max_q_id);
                    cout<<"\nType in the index of the question you want to delete.\n\n";
                    int question_id;
                    while(true)
                    {
                        cin>>question_id;
                        if(question_id>-1 && question_id < max_q_id+1)
                        {
                            delete_the_question(questions_file_name, answers_file_name, question_id);
                            break;
                        }
                        else
                        {
                            cout<<"\nNo question with this index is found. Please, try again.\n\n";
                            continue;
                        }
                    }
                    break;
                case 2:
                    cout<<"\tThe list of available answers.\n\n";
                    read_seq_a(answers_file_name, -1, first_id, last_id);
                    cout<<"\nType in the index of the answer you want to delete.\n\n";
                    int answer_id;
                    cout<<endl;
                    while(true)
                    {

                        cin>>answer_id;
                        if(answer_id>first_id-1 && answer_id < last_id+1)
                        {
                            delete_the_answer(answers_file_name, answer_id);
                            break;
                        }
                        else
                        {
                            cout<<"\nNo answer with this index is found. Please, try again.\n\n";
                            continue;
                        }
                    }
                    delete_the_answer(answers_file_name, answer_id);
                    break;
                default:
                    cout<<"\nerror. unknown request. try again.\n\n";
                    continue;
                }
                break;
            }
            cout<<"\n\tDo you want to continue?\n\n";
            cout<<"1  -  yes. Return to the main menu.\n0  -  no. Exit.\n\n";
            while(true)
            {
                cin>>next_step;
                switch(next_step)
                {
                case 1:
                    system("CLS");
                    menu();
                    break;
                case 0:
                    return 0;
                default:
                    cout<<"\nerror. unknown request. please, try again.\n\n";
                    continue;
                }
                break;
            }
            break;
        case 4:
            delete_the_content(questions_file_name, answers_file_name);
            if(is_file_empty(questions_file_name, answers_file_name)) cout<<"Removal went successfully.\n";
            else cout<<"Something went wrong.\n";
            cout<<"\n\tDo you want to continue?\n\n";
            cout<<"1  -  yes. Return to the main menu.\n0  -  no. Exit.\n\n";
            while(true)
            {
                cin>>next_step;
                switch(next_step)
                {
                case 1:
                    system("CLS");
                    menu();
                    break;
                case 0:
                    return 0;
                default:
                    cout<<"\nerror. unknown request. please, try again.\n\n";
                    continue;
                }
                break;
            }
            continue;
        case 0:
            return 0;
        default:
            cout<<"\nerror. unknown request. please, try again.\n\n";
            system("pause");
            system("CLS");
            menu();
            continue;
        }
    }
    delete_deleted_content(questions_file_name, answers_file_name);
    return 0;
}

void check_questions(int answered[], int i)
{
    if(i==0) return;
    if(i==1) cout<<"\n\tYou have already answered on the question with index ";
    else cout<<"\n\tYou have already answered on the questions with index ";
    for(int j=0; j<i-1; j++)
        cout<<answered[j]<<", ";
    cout<<answered[i-1]<<".";
    cout<<"\n\n";
}

void menu()
{
    cout<<"\t\t\tPoll database.\nPlease, look below at what choices you have and choose one of them.\n\tAfter you chose one, type in the corresponding number.\n\n";
    cout<<"1  -  choose the question.\n";
    cout<<"2  -  add a new question in the file.\n";
    cout<<"3  -  delete the question/answer from the file.\n";
    cout<<"4  -  delete all content from files.\n";
    cout<<"0  -  exit.\n\n";
}

void read_seq_q(char file_name[], int &max_q_id)
{
    questions_info part;
    FILE *fp=fopen(file_name,"rb");
    while (!feof(fp))
    {
        fread(&part, sizeof(questions_info), 1, fp);
        if(!part.deleted)
            if(!feof(fp))
            {
                if(part.deleted) continue;
                max_q_id=part.id;
                cout<<"[ID: "<<part.id<<"]  The question: "<<UNDERLINE<<part.question<<CLOSEUNDERLINE<<endl;
            }
    }
    fclose(fp);
}

int read_seq_a(char file_name[], int num, int &first_id, int &last_id)
{
    answers_info part;
    int iter=0;
    FILE *fp=fopen(file_name,"rb");
    if(num==-1)
    {
        while (!feof(fp))
        {
            fread(&part, sizeof(answers_info), 1, fp);
            if(!part.deleted)
                if(!feof(fp))
                {
                    first_id=part.id;
                    break;
                }
        }
        fseek(fp, 0, SEEK_SET);
        while (!feof(fp))
        {
            fread(&part, sizeof(answers_info), 1, fp);
            if(!part.deleted)
                if(!feof(fp))
                {
                    last_id=part.id;
                    cout<<"[ID: "<<part.id<<"]  The option: "<<UNDERLINE<<part.option<<CLOSEUNDERLINE<<endl;
                }
        }
        fclose(fp);
        return 0;
    }

    while (!feof(fp))
    {
        fread(&part, sizeof(answers_info), 1, fp);
        if(!part.deleted)
            if(!feof(fp) && part.question_id==num)
            {
                first_id=part.id;
                break;
            }
    }
    fseek(fp, 0, SEEK_SET);
    while (!feof(fp))
    {
        fread(&part, sizeof(answers_info), 1, fp);
        if(!part.deleted)
            if(!feof(fp) && part.question_id==num)
            {

                last_id=part.id;
                iter++;
                cout<<"[ID: "<<part.id<<"]  The option: "<<UNDERLINE<<part.option<<CLOSEUNDERLINE<<endl;
            }
    }
    fclose(fp);
    return iter;
}

void output_results(user_answers current_answers[], int i, char file_name_q[], char file_name_a[])
{
    questions_info part_q;
    answers_info part_a;
    FILE* fp_q;
    FILE* fp_a;
    for(int j=0; j<i; j++)
    {
        fp_a=fopen(file_name_a,"rb");
        fp_q=fopen(file_name_q,"rb");
        while (!feof(fp_q))
        {
            fread(&part_q, sizeof(questions_info), 1, fp_q);
            if(!feof(fp_q) && part_q.id==current_answers[j].question_id)
            {
                cout<<"The question:\t"<<part_q.question;
                break;
            }
        }
        while (!feof(fp_a))
        {
            fread(&part_a, sizeof(answers_info), 1, fp_a);
            if(!feof(fp_a) && part_a.id==current_answers[j].answer_id)
            {
                cout<<"\nYour answer:\t"<<part_a.option<<"\n\n";
                break;
            }
        }
        fclose(fp_q);
        fclose(fp_a);
    }
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
    cout<<"Enter the question.\n";
    cin.getline(new_question.question, sizeof(new_question.question));
    new_question.deleted=false;
    if (new_question.question[0]==' ' || new_question.question[0]==NULL)
    {
        cout<<"\t\tIncorrect input.";
        return;
    }
    cout<<"Enter the options to the answer. ('enter' to stop)\n";
    while(true)
    {
        new_answer.id=id_a;
        new_answer.question_id=id_q;
        new_question.deleted=false;
        cin.getline(new_answer.option, sizeof(new_answer.option));
        if((new_answer.option[0]!=' ' && new_answer.option[0]!=NULL) && (new_question.question[0]!=' ' && new_question.question[0]!=NULL)) fwrite(&new_answer,sizeof(new_answer),1,fp_a);
        else break;
        id_a++;
    }
    fwrite(&new_question,sizeof(new_question),1,fp_q);
    fclose(fp_q);
    fclose(fp_a);
}

void delete_the_question(char file_name_q[], char file_name_a[], int id)
{
    FILE* fp_q=fopen(file_name_q, "r+b");
    FILE* fp_a=fopen(file_name_a, "r+b");
    if(!fp_q)
    {
        perror("Error");
        return;
    }
    questions_info part;
    answers_info part_a;
    fseek(fp_q,sizeof(questions_info)*id,SEEK_SET);
    fread(&part, sizeof(questions_info), 1, fp_q);
    part.deleted=true;
    fseek(fp_q, -sizeof(questions_info),SEEK_CUR);
    fwrite(&part, sizeof(questions_info), 1, fp_q);
    fseek(fp_a, 0, SEEK_SET);
    while(fread(&part_a, sizeof(answers_info), 1, fp_a) == 1)
    {
        if(part_a.question_id==id)
        {
            part_a.deleted=true;
            fseek(fp_a, -sizeof(answers_info),SEEK_CUR);
            fwrite(&part_a, sizeof(answers_info), 1, fp_a);
        }
        if(part_a.question_id>id) break;
        fseek(fp_a, 0, SEEK_CUR);
    }
    fclose(fp_q);
    fclose(fp_a);
}

void delete_the_answer(char file_name[], int id)
{
    FILE* fp=fopen(file_name, "rb+");
    if(!fp)
    {
        perror("Error");
        return;
    }
    answers_info part;
    fseek(fp, 0, SEEK_SET);
    while(fread(&part, sizeof(answers_info), 1, fp) == 1)
    {
        if(part.id==id)
        {
            part.deleted=true;
            fseek(fp, -sizeof(answers_info),SEEK_CUR);
            fwrite(&part, sizeof(answers_info), 1, fp);
            break;
        }
    }
    /*fseek(fp,sizeof(answers_info)*id,SEEK_SET);
    fread(&part, sizeof(answers_info), 1, fp);
    part.deleted=true;
    fseek(fp, -sizeof(answers_info),SEEK_CUR);
    fwrite(&part, sizeof(answers_info), 1, fp);*/
    fclose(fp);
}

void delete_the_content(char file_name_q[], char file_name_a[])
{
    FILE* fp_q=fopen(file_name_q, "wb");
    FILE* fp_a=fopen(file_name_a, "wb");
    fclose(fp_a);
    fclose(fp_q);
}

bool is_file_empty(char file_name_q[], char file_name_a[])
{
    FILE *fp_q=fopen(file_name_q, "rb");
    FILE *fp_a=fopen(file_name_a, "rb");
    fseek(fp_q, 0, SEEK_END);
    if (ftell(fp_q) == 0)
    {
        fclose(fp_q);
        return true;
    }
    fseek(fp_a, 0, SEEK_END);
    if (ftell(fp_a) == 0)
    {
        fclose(fp_a);
        return true;
    }
    questions_info part_q;
    answers_info part_a;
    fseek(fp_q, 0, SEEK_SET);
    fseek(fp_a, 0, SEEK_SET);
    bool deleted_el=true;
    while (!feof(fp_q))
    {
        fread(&part_q, sizeof(questions_info), 1, fp_q);
        if(!feof(fp_q) && part_q.deleted==false)
        {
            deleted_el=false;
            break;
        }
    }
    if(deleted_el==true) return true;
    fclose(fp_q);
    deleted_el=true;
    while (!feof(fp_a))
    {
        fread(&part_a, sizeof(answers_info), 1, fp_a);
        if(!feof(fp_a) && part_a.deleted==false)
        {
            deleted_el=false;
            break;
        }
    }
    if(deleted_el==true) return true;
    fclose(fp_a);
    return false;
}

void delete_deleted_content(char file_name_q[], char file_name_a[])
{
    questions_info part_q;
    answers_info part_a;
    FILE* temp_q=fopen("../temp_q.txt", "wb+");
    FILE* temp_a=fopen("../temp_a.txt", "wb+");
    FILE* f_q=fopen("../questions.txt", "rb");
    FILE* f_a=fopen("../answers.txt", "rb");
    while(!feof(f_q))
    {
        fread(&part_q, sizeof(questions_info), 1, f_q);
        if(!part_q.deleted && !feof(f_q))
            fwrite(&part_q, sizeof(questions_info), 1, temp_q);
    }
    while(!feof(f_a))
    {
        fread(&part_a, sizeof(answers_info), 1, f_a);
        if(!part_a.deleted && !feof(f_a))
            fwrite(&part_a, sizeof(answers_info), 1, temp_a);
    }
    fclose(temp_q);
    fclose(temp_a);
    fclose(f_a);
    fclose(f_q);
    remove(file_name_q);
    remove(file_name_a);
    rename("../temp_q.txt", "../questions.txt");
    rename("../temp_a.txt", "../answers.txt");
    return;
}
