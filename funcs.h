
typedef struct Course {
	char* name;
	unsigned short mark;
} Course;

typedef struct Student {
	long ID;
	Course* courses;
	unsigned short marks_avrage;
	char* first_name;
	char* last_name;
} Student;

typedef struct StudentList {
	Student* head_of_the_list;
	Student* cursor;
} StudentList;


void print_error_and_exit(const char* error, int error_code);
void print_all_students(StudentList*);

int check_text(char* txt);// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_request(char* req); // validate request
int chck_student_details_types(char* details);// check details string for student structure
Student* crate_student(long ID, char* name, Course* courses);
Course* create_course(char* name, unsigned short mark);

void sort_students_list(StudentList*);
void add_student_in_order(StudentList*, Student*);
void delete_student(StudentList*, long ID);

void help();// print documentation for the requests server
void play_requests_server();// for data requests