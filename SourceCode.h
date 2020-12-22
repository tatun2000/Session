#pragma once
#include "SourceCode.h"
#include "Timer.h"
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

const unsigned int KOL_GROUP = 3;
const unsigned int KOL_TEACH_IN_GROUP = 4;

class Student
{
public:
    Student(char* first_param, char* second_param, char* third_param, char* fouth_param) :
        id_stud(atoi(first_param)), fam_stud(second_param), group_id(atoi(third_param)), skill(atoi(fouth_param)) {};
    unsigned int Get_Id_Group_Of_Stud();
    unsigned int Get_Skill_Of_Stud();
    unsigned int Get_Id_Of_Stud();
    string Get_Fam_Of_Stud();
private:
    unsigned int id_stud, group_id, skill;
    string fam_stud;
};

class Teacher
{
public:
    Teacher();
    Teacher(char* first_param, char* second_param, char* third_param, char* fouth_param, string fifth_param) :
        id_teach(atoi(first_param)), id_subject(atoi(second_param)), subject_name(third_param),
        fam_teach(fouth_param), power(stof(fifth_param)) {};
    float Get_Power();
    unsigned int Get_Id();
    string Get_Fam_Teacher();
private:
    unsigned int id_teach, id_subject;
    string subject_name, fam_teach;
    float power;
};

class Group_Teachers
{
public:
    Group_Teachers(char* first_param) :id_group(atoi(first_param))
    {
        for (int i = 0; i < KOL_TEACH_IN_GROUP; ++i)
        {
            this->id_teach[i] = 0;
        }
    };
    void Inizialisation_Id_Teach_In_One_Group(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res);
    float Get_Power_On_Id_Teach(vector<Teacher*> my_teach, const size_t& I);
    string Get_Fam_Teach_In_Group(vector<Teacher*> my_teach, const size_t& I);
private:
    unsigned int id_group;
    unsigned int id_teach[KOL_TEACH_IN_GROUP];
};


vector<Student*> Create_Vector_Of_Students(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res); // Creating a vector of students
vector<Teacher*> Create_Vector_Of_Teachers(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res); // Creating a vector of teachers
vector<Group_Teachers*> Create_Vector_Of_Groups(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res); // Creating a vector from teacher groups
void Main_Session(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res); //Main Function

// Creating a vector with minimal results for each group
vector<float> Calculate_Min_Res_In_One_Group(const vector<Teacher*>& my_teacher, 
    const vector<Group_Teachers*>& my_group_teacher);

// Function for calculating the power factor of a teacher in a group
float Calculate_One_Koeff_In_One_Group(const size_t& i, const size_t& j, const vector<Teacher*>& my_teacher, 
    const vector<Group_Teachers*>& my_group_teacher);

// Session
void Session(const vector<Student*>& my_stud, const vector<Teacher*>& my_teacher, 
    const vector<Group_Teachers*>& my_group_teacher, const vector<float>& min_res);
