
#define COURSES 3
#define LINE 320
#define NAME 128

// courses codes
enum Courses { c_lang, comp_net, cs_f };
char *courses_names[] = {[c_lang] = "C language",[comp_net] = "Computer Networks",[cs_f] = "CS Fundamentals"};

typedef struct Student {
	unsigned long ID : 30;
	unsigned short marks_avrage : 7;
	unsigned short marks[COURSES];
	char* first_name;
	char* last_name;
} Student;

typedef struct StudentList {
	Student* head_of_the_list;
	Student* cursor;
} StudentList;


void print_error_and_exit(const char* error, int error_code);
void print_student(Student*);
void print_all_students(StudentList*);



int find_cours_code(char* course_name);
int is_number(char* txt);
//// get state of a single bit
//int check_bit(short num, short bit);
//// change state of a single bit
//int set_bit(short num, short bit);

// check if all the chars is 'a-z, A-Z, spaces..' (not digits or special signs)
int check_text_for_name(char* txt);
// validate request to server
int check_request(char* req);
// check details string for student structure

void crate_student(char* details, Student* student, char* error);

void sort_students_list(StudentList*);
void add_student_in_order(StudentList*, Student*);
void delete_student(StudentList*, long ID);

void help();// print documentation for the requests server
void play_requests_server();// for data requests