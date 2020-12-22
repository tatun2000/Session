#include "SourceCode.h"
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Student
{
public:
    Student(char* first_param, char* second_param, char* third_param, char* fouth_param):
        id_stud(atoi(first_param)), fam_stud(second_param), group_id(atoi(third_param)), skill(atoi(fouth_param)) {};
    ~Student();

private:
    unsigned int id_stud, group_id, skill;
    string fam_stud;
};
Student::~Student()
{
}

class Teacher
{
public:
    Teacher(char* first_param, char* second_param, char* third_param, char* fouth_param):
        id_teach(atoi(first_param)), id_subject(atoi(second_param)), fam_teach(third_param), power(atoi(fouth_param)) {};
    ~Teacher();
    
private:
    unsigned int id_teach, id_subject;
    string subject_name, fam_teach;
    float power;
};


Teacher::~Teacher()
{
}

void Teacher::Inizialisation_Fam_Teachers()
{

}



int main()
{
    setlocale(LC_ALL, "rus");
    MYSQL* conn; // указатель на соединение (Дескриптор) (id конкретного соединения для операции)
    MYSQL_ROW row = 0; // Переменная, из которой будем читать результат
    MYSQL_RES* res = 0; // Указатель на результат запроса

    conn = mysql_init(0); // Инициализируем новый объект MySQL
    conn = mysql_real_connect(conn, "localhost", "root", "Dima10910000", "Session", 3306, NULL, 0); // Подключаем объект к БД

    if (conn)
    {
        cout << "Successful connection to database!\n"; // Подключились к БД
    }
    else
    {
        cout << "Connection to database has failed!\n";
    }
    return 0;
}

