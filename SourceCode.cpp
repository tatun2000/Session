#include "SourceCode.h"



//Main Function
void Main_Session(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res)
{
    vector<Student*> my_vector_of_students;
    vector<Teacher*> my_vector_of_teachers;
    vector<Group_Teachers*> my_vector_of_groups;
    vector<float> my_vector_of_min_res;
    my_vector_of_students = Create_Vector_Of_Students(conn, row, res); // Filling in the vector of students
    my_vector_of_teachers = Create_Vector_Of_Teachers(conn, row, res); // Filling in the vector of teachers
    my_vector_of_groups = Create_Vector_Of_Groups(conn, row, res); // Filling in the vector of teacher groups

    // Filling in vector with minimal results for each group
    my_vector_of_min_res = Calculate_Min_Res_In_One_Group(my_vector_of_teachers, my_vector_of_groups); 

    // Session function
    Session(my_vector_of_students, my_vector_of_teachers, my_vector_of_groups, my_vector_of_min_res);
};

// Creating a vector of students
vector<Student*> Create_Vector_Of_Students(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res)
{
    string select_all_stud = "SELECT * FROM Stud;";
    mysql_query(conn, select_all_stud.c_str()); // Executing an SQL query specified as a c string
    res = mysql_store_result(conn); // Extracting the full result set for the connection
    vector<Student*> local_vector_of_student; // Declaring a vector of pointers to objects of the Student class
    if (mysql_num_rows(res) > 0) // If the number of rows in the result set is > 0
    {
        int i = 0;
        local_vector_of_student.resize(mysql_num_rows(res));
        while (row = mysql_fetch_row(res)) // As long as there is the next row in the resulting dataset
        {
            local_vector_of_student[i] = new Student(row[0], row[1], row[2], row[3]);
            ++i;
        }
    }
    else
    {
        cout << "Error to connect\n";
    }
    mysql_free_result(res); // Freeing up the memory used for the result set
    return local_vector_of_student;
}

// Creating a vector of teachers
vector<Teacher*> Create_Vector_Of_Teachers(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res)
{
    string string_1 = "SELECT a.teacher_id, a.subject_id, b.name_of_subject, a.fam_teach, a.power";
    string string_2 = " FROM Teacher AS a INNER JOIN Subject AS b ON b.sub_id = a.subject_id ORDER BY a.teacher_id;";
    string select_all_teach = string_1 + string_2;
    mysql_query(conn, select_all_teach.c_str()); // Executing an SQL query specified as a c string
    res = mysql_store_result(conn); // Extracting the full result set for the connection
    vector<Teacher*> local_vector_of_teacher; // Declaring a vector of pointers to objects of the Teacher class
    if (mysql_num_rows(res) > 0) // If the number of rows in the result set is > 0
    {
        int i = 0;
        local_vector_of_teacher.resize(mysql_num_rows(res));
        string temp = "";
        while (row = mysql_fetch_row(res)) // As long as there is the next row in the resulting dataset
        {
            temp += row[4];
            temp[1] = ',';

            local_vector_of_teacher[i] = new Teacher(row[0], row[1], row[2], row[3], temp);
            ++i;

            temp = "";
        }
        mysql_free_result(res); // Freeing up the memory used for the result set
    }
    else
    {
        cout << "Error to connect\n";
    }
    return local_vector_of_teacher;
}

// Creating a vector from teacher groups
vector<Group_Teachers*> Create_Vector_Of_Groups(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res)
{
    string select_all_groups = "SELECT * FROM Group_Teachers;";

    mysql_query(conn, select_all_groups.c_str()); // Executing an SQL query specified as a c string
    res = mysql_store_result(conn); // Extracting the full result set for the connection
    vector<Group_Teachers*> local_vector_of_groups; // Declaring a vector of pointers to objects of the Group_Teacher class
    if (mysql_num_rows(res) > 0) // If the number of rows in the result set is > 0
    {
        local_vector_of_groups.resize(KOL_GROUP);
        string arr[KOL_TEACH_IN_GROUP];

        int i = 0;
        while (row = mysql_fetch_row(res)) // As long as there is the next row in the resulting dataset
        {
            local_vector_of_groups[i] = new Group_Teachers(row[0]);
            local_vector_of_groups[i]->Inizialisation_Id_Teach_In_One_Group(conn, row, res);
            ++i;
        }
        mysql_free_result(res); // Freeing up the memory used for the result set
    }
    else
    {
        cout << "Error to connect\n";
    }
    return local_vector_of_groups;
}

// Creating a vector with minimal results for each group
vector<float> Calculate_Min_Res_In_One_Group(const vector<Teacher*>& my_teacher,
    const vector<Group_Teachers*>& my_group_teacher)
{
    vector<float> local_result;
    float res_in_one_group = 0;

    local_result.resize(KOL_GROUP);
    for (size_t i = 0; i < KOL_GROUP; ++i)
    {
        res_in_one_group = 0;
        for (size_t j = 0; j < KOL_TEACH_IN_GROUP; ++j)
        {
            res_in_one_group += Calculate_One_Koeff_In_One_Group(i, j, my_teacher, my_group_teacher);
        }
        local_result[i] += res_in_one_group;
    }
    return local_result;
}


// Function for calculating the power factor of a teacher in a group
float Calculate_One_Koeff_In_One_Group(const size_t& i, const size_t& j, const vector<Teacher*>& my_teacher,
    const vector<Group_Teachers*>& my_group_teacher)
{
    float koeff[KOL_GROUP] = { 0.78, 0.72, 0.75 };
    return  my_group_teacher[i]->Get_Power_On_Id_Teach(my_teacher, j) * koeff[i];
}

// Session
void Session(const vector<Student*>& my_stud, const vector<Teacher*>& my_teacher,
    const vector<Group_Teachers*>& my_group_teacher, const vector<float>& min_res)
{
    float koeff[KOL_GROUP] = { 0.78, 0.72, 0.75 };
    map<int, float> my_map;
    for (size_t i = 0; i < my_stud.size(); ++i)
    {
        my_map.emplace(i, my_stud[i]->Get_Skill_Of_Stud());
        my_map[i] /= 100;
    }
    unsigned int local_group = 0;
    for (size_t i = 0; i < my_stud.size(); ++i)
    {
        local_group = my_stud[i]->Get_Id_Group_Of_Stud();


        cout << "\n----------\n " << my_stud[i]->Get_Id_Of_Stud() << ". " << my_stud[i]->Get_Fam_Of_Stud()
            << "\n The student belongs to " << local_group << " group\n";
        cout << " Student teachers : ";
        for (size_t i = 0; i < KOL_TEACH_IN_GROUP; ++i)
        {
            cout << my_group_teacher[local_group - 1]->Get_Fam_Teach_In_Group(my_teacher, i) << " ";
        }
        cout << "\n The skills of the student = " << my_map[i] * 100 << "\n"
            << " Minimum threshold for a group = " << koeff[local_group - 1] * 100;
        if (my_map[i] * 100 >= koeff[local_group - 1] * 100)
        {
            cout << "\t The session was successfully completed!\n----------";
        }
        else
        {
            cout << "\t Session is swamped!\n----------";
        }
    }
    cout << "\n\n";
}

unsigned int Student::Get_Id_Group_Of_Stud()
{
    return this->group_id;
}

unsigned int Student::Get_Skill_Of_Stud()
{
    return this->skill;
}

unsigned int Student::Get_Id_Of_Stud()
{
    return this->id_stud;
}

string Student::Get_Fam_Of_Stud()
{
    return this->fam_stud;
}

Teacher::Teacher()
{

}

float Teacher::Get_Power()
{
    return this->power;;
}

unsigned int Teacher::Get_Id()
{
    return this->id_teach;
}

string Teacher::Get_Fam_Teacher()
{
    return this->fam_teach;
}

void Group_Teachers::Inizialisation_Id_Teach_In_One_Group(MYSQL* conn, MYSQL_ROW row, MYSQL_RES* res)
{
    for (size_t i = 0; i < KOL_TEACH_IN_GROUP; ++i)
    {
        id_teach[i] = atoi(row[1]);
        if (i == KOL_TEACH_IN_GROUP - 1)
        {
            break;
        }
        row = mysql_fetch_row(res);
    }
}

float Group_Teachers::Get_Power_On_Id_Teach(vector<Teacher*> my_teach, const size_t& I)
{
    for (size_t j = 0; j < my_teach.size(); ++j)
    {
        if (id_teach[I] == my_teach[j]->Get_Id())
        {
            return my_teach[j]->Get_Power();
        }
    }
}

string Group_Teachers::Get_Fam_Teach_In_Group(vector<Teacher*> my_teach, const size_t& I)
{
    for (size_t j = 0; j < my_teach.size(); ++j)
    {
        if (id_teach[I] == my_teach[j]->Get_Id())
        {
            return my_teach[j]->Get_Fam_Teacher();
        }
    }
}
