#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define NUM_SLOTS 5
#define NUM_ROWS 4
#define SEATS_PER_BENCH 3

typedef struct Student
{
    char name[50];
    char enrollment_number[20];
    char courses[10][20];
    int slots[NUM_SLOTS];
    struct Student *next;
} Student;

typedef struct Course
{
    char name[20];
    int slot;
    int num_students;
    char courses_room[10];
    struct Student *students;
} Course;

Course courses[] = {
    {"P", 1},
    {"C", 2},
    {"M", 3},
    {"DM", 3},
    {"B", 4},
    {"CS", 5},
    {"DSPD1", 6}
   
    };

int num_courses = sizeof(courses) / sizeof(Course);

typedef struct classroom
{
    char room_number[10];
    int capacity;
    int benches_per_row;
    int slots[NUM_SLOTS];
    Student *students;
} classroom;

classroom classrooms[] = {
    {"C101", 5},
    {"C102", 2},
    {"C103", 4},
    {"C104", 20},
    {"C105", 3},
};

Student *student_list = NULL;
 
int get_slot(char *course_name)
{
    for (int i = 0; i < num_courses; i++)
    {
        if (strcmp(course_name, courses[i].name) == 0)
        {
            return courses[i].slot;
        }
    }
    return -1;
}

int check_slot(int slots[], int slot)
{
    for (int i = 0; i < 5; i++)
    {
        if (slots[i] == slot)
        {
            return 1;
        }
    }
    return 0;
}

void add_student()
{
    
    FILE* ptr = fopen("student_read.txt", "r");

    char name[50];
    char enrol[20];
    char s1[20];
    char s2[20];
    char s3[20];
    char s4[20];
    char s5[20];
    while(fscanf(ptr,"%s %s %s %s %s %s %s\n",name,enrol,s1,s2,s3,s4,s5) != EOF)
    {
    Student *new_Student = (Student *)malloc(sizeof(Student));
    memset(new_Student->name, 0, sizeof(new_Student->name));
    memset(new_Student->enrollment_number, 0, sizeof(new_Student->enrollment_number));
    for (int i = 0; i < num_courses; i++)
    {
        memset(new_Student->courses[i], 0, sizeof(new_Student->courses[i]));
    }

    strcpy(new_Student->name, name);
    strcpy(new_Student->enrollment_number, enrol);
    strcpy(new_Student->courses[0], s1);
    strcpy(new_Student->courses[1], s2);
    strcpy(new_Student->courses[2], s3);
    strcpy(new_Student->courses[3], s4);
    strcpy(new_Student->courses[4], s5);
    
   static int count2=1;
    Student *current = student_list;
    while (current != NULL)
    {
        if (strcmp(current->enrollment_number, new_Student->enrollment_number) == 0)
        {
            printf("Error: Enrollment No. %s already exists.\n", new_Student->enrollment_number);
            return;
        }
        current = current->next;
    }

   
    for (int i = 0; i < 5; i++)
    {
        int slot = get_slot(new_Student->courses[i]);
        if (slot == -1)
        {
            printf("Error: Invalid course name.\n");
            return;
        }
        if (check_slot(new_Student->slots, slot))
        {
            printf("Error: Two courses cannot be assigned the same slot.\n");
            return;
        }
        new_Student->slots[i] = slot;
    }
    new_Student->next = NULL;

    Student *ptr = student_list;
    Student *prev = NULL;
    while (ptr != NULL && strcmp(ptr->enrollment_number, new_Student->enrollment_number) < 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if (prev == NULL)
    {
        new_Student->next = student_list;
        student_list = new_Student;
    }
    else
    {
        new_Student->next = ptr;
        prev->next = new_Student;
    }
    
    if(count2==1)
    {
        printf("Student added successfully!\n");     
    }
    count2++;
    }
}

void add_student_to_txt()
{
    FILE *fptr;
    fptr = fopen("student_print_list.txt", "w");
    if (fptr == NULL)
    {
        printf("Error opening file.");
        return;
    }

    Student *current = student_list;
    fprintf(fptr, "Name\tEnrollment Number\tCourses\t\tSlots\n");
    while (current != NULL)
    {
        fprintf(fptr, "%s\t%s\t\t", current->name, current->enrollment_number);
        for (int i = 0; i < num_courses; i++)
        {
            fprintf(fptr, "%s ", current->courses[i]);
        }
        fprintf(fptr, "\t");
        for (int i = 0; i < NUM_SLOTS; i++)
        {
            fprintf(fptr, "%d ", current->slots[i]);
        }
        fprintf(fptr, "\n");
        current = current->next;
    }

    fclose(fptr);
}

void print_student_list()
{
    Student *current = student_list;
    printf("Name\tEnrollment Number\tCourses\t\tSlots\n");
    while (current != NULL)
    {
        printf("%s\t%s\t\t", current->name, current->enrollment_number);
        for (int i = 0; i < num_courses; i++)
        {
            printf("%s ", current->courses[i]);
        }
        printf("\t");
        for (int i = 0; i < NUM_SLOTS; i++)
        {
            printf("%d ", current->slots[i]);
        }
        printf("\n");
        current = current->next;
    }
}

void delete_student()
{
    char enrollment_number[20];
    printf("Enter enrollment number of student to delete: ");
    scanf("%s", enrollment_number);

    Student *ptr = student_list;
    Student *prev = NULL;

    while (ptr != NULL && strcmp(ptr->enrollment_number, enrollment_number) != 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL)
    {
        printf("Error: Student with enrollment number %s not found.\n", enrollment_number);
        return;
    }

    if (prev == NULL)
    {
        student_list = ptr->next;
    }
    else
    {
        prev->next = ptr->next;
    }

    for (int i = 0; i < 5; i++)
    {
        Course *course = &courses[ptr->slots[i] - 1];
        Student *current = course->students;
        Student *prev = NULL;
        while (current != NULL && strcmp(current->enrollment_number, enrollment_number) != 0)
        {
            prev = current;
            current = current->next;
        }
        if (current != NULL)
        {
            if (prev == NULL)
            {
                course->students = current->next;
            }
            else
            {
                prev->next = current->next;
            }
        }
    }

    free(ptr);

    printf("Student with enrollment number %s deleted successfully!\n", enrollment_number);
}

void add_course()
{
    char name[20];
    int slot;

    printf("Enter course name: ");
    scanf("%s", name);

    printf("Enter slot number: ");
    scanf("%d", &slot);

    for (int i = 0; i < num_courses; i++)
    {
        if (strcmp(name, courses[i].name) == 0)
        {
            printf("Error: Course already exists.\n");
            return;
        }
    }
    num_courses++;

    strcpy(courses[num_courses].name, name);
    courses[num_courses].slot = slot;
    courses[num_courses].students = NULL;

    printf("Course added successfully!\n");
}

void delete_course()
{
    char name[20];

    printf("Enter course name: ");
    scanf("%s", name);

    int course_index = -1;
    for (int i = 0; i < num_courses; i++)
    {
        if (strcmp(name, courses[i].name) == 0)
        {
            course_index = i;
            break;
        }
    }

    if (course_index == -1)
    {
        printf("Error: Course not found.\n");
        return;
    }

    Student *current = student_list;
    while (current != NULL)
    {
        for (int i = 0; i < num_courses; i++)
        {
            if (strcmp(current->courses[i], name) == 0)
            {
                printf("Error: A student has taken this course.\n");
                return;
            }
        }
        current = current->next;
    }

    for (int i = course_index; i < num_courses - 1; i++)
    {
        strcpy(courses[i].name, courses[i + 1].name);
        courses[i].slot = courses[i + 1].slot;
        courses[i].num_students = courses[i + 1].num_students;
        strcpy(courses[i].courses_room, courses[i + 1].courses_room);
        courses[i].students = courses[i + 1].students;
    }
    num_courses--;

    printf("Course deleted successfully!\n");
}

void print_course_list()
{
    printf("Course List:\n");
    for (int i = 0; i < num_courses; i++)
    {
        // if (courses[i].slot != 0)
            printf("%s (slot %d)\n", courses[i].name, courses[i].slot);
    }
}

int count_students_in_course(char *course_name)
{
    int count = 0;
    for (int i = 0; i < num_courses; i++)
    {
        if (strcmp(course_name, courses[i].name) == 0)
        {
            Student *current_student = student_list;
            while (current_student != NULL)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (strcmp(current_student->courses[j], course_name) == 0)
                    {
                        count++;
                        break;
                    }
                }
                current_student = current_student->next;
            }
            break;
        }
    }
    return count;
}

void allot_classrooms()
{
    for (int i = 0; i < num_courses - 1; i++)
    {
        courses[i].num_students = count_students_in_course(courses[i].name);
        for (int j = i + 1; j < num_courses; j++)
        {
            if (courses[i].slot > courses[j].slot ||
                (courses[i].slot == courses[j].slot &&
                 courses[i].num_students > courses[j].num_students) ||
                (courses[i].slot == courses[j].slot &&
                 courses[i].num_students == courses[j].num_students &&
                 strcmp(courses[i].name, courses[j].name) > 0))
            {
                Course temp = courses[i];
                courses[i] = courses[j];
                courses[j] = temp;
            }
        }
    }

    int num_classrooms = sizeof(classrooms) / sizeof(classroom);
    for (int i = 0; i < num_classrooms - 1; i++)
    {
        for (int j = i + 1; j < num_classrooms; j++)
        {
            if (classrooms[j].capacity > classrooms[i].capacity ||
                (classrooms[j].capacity == classrooms[i].capacity &&
                 strcmp(classrooms[j].room_number, classrooms[i].room_number) < 0))
            {
                classroom temp = classrooms[j];
                classrooms[j] = classrooms[i];
                classrooms[i] = temp;
            }
        }
    }

    for (int i = 0; i < num_courses; i++)
    {
        courses[i].num_students = count_students_in_course(courses[i].name);

        if (courses[i].num_students == 0)
        {
            printf("Cannot allot classroom for %s (slot %d) as no one registered for it\n", courses[i].name, courses[i].slot);
            continue;
        }

        for (int j = 0; j < num_classrooms; j++)
        {
            if (classrooms[j].capacity >= courses[i].num_students)
            {
                for (int k = i - 1; k >= 0 && courses[k].slot == courses[i].slot; k--)
                {
                    if (strcmp(courses[k].name, courses[i].name) != 0)
                    {
                        j++;
                        break;
                    }
                }

                strcpy(courses[i].courses_room, classrooms[j].room_number);
                classrooms[j].slots[i] = courses[i].slot;
                printf("Room alloted %s for %s(Slot %d)\n", classrooms[j].room_number, courses[i].name, courses[i].slot);
               
                break;
            }
        }
    }

    for (int slot = 1; slot <= NUM_SLOTS; slot++) 
    {
        int num_courses_in_slot = 0;                   
        int total_students_in_slot = 0;                
        int min_num_courses_without_room = INT_MAX;    
        int min_total_students_without_room = INT_MAX; 

        for (int i = 0; i < num_courses; i++)
        {
            if (courses[i].slot == slot)
            {
                courses[i].num_students = count_students_in_course(courses[i].name);
                total_students_in_slot += courses[i].num_students;
                num_courses_in_slot++;

                if (strcmp(courses[i].courses_room, "") == 0) 
                {
                    min_num_courses_without_room = fmin(min_num_courses_without_room, num_courses_in_slot);
                    min_total_students_without_room = fmin(min_total_students_without_room, total_students_in_slot);
                }
            }
        }

        if (min_num_courses_without_room == INT_MAX) 
        {
            continue;
        }

    
        for (int j = num_classrooms - 1; j >= 0; j--)
        {
            if (classrooms[j].capacity < min_total_students_without_room) // room is too small
            {
                break;
            }

   
            for (int i = 0; i < num_courses; i++)
            {
                if (courses[i].slot == slot && strcmp(courses[i].courses_room, "") == 0)
                {
                    if (classrooms[j].capacity >= courses[i].num_students)
                    {
                        strcpy(courses[i].courses_room, classrooms[j].room_number);
                        classrooms[j].slots[i] = courses[i].slot;
                        printf("Room allotted %s for %s (Slot %d)\n", classrooms[j].room_number, courses[i].name, courses[i].slot);
                        min_num_courses_without_room--;
                        min_total_students_without_room -= courses[i].num_students;

                        if (min_num_courses_without_room == 0 || min_total_students_without_room == 0)
                        {
                            break;
                        }
                    }
                }
            }

            if (min_num_courses_without_room == 0 || min_total_students_without_room == 0)
            {
                break; 
            }
        }

        if (min_num_courses_without_room > 0 || min_total_students_without_room > 0)
        {
            printf("Unable to allocate room for some courses in Slot %d with minimum number of courses without a room = %d and minimum total number of students without a room = %d\n", slot, min_num_courses_without_room, min_total_students_without_room);
        }
    }
}

void allot_classrooms_for_exams()
{
    int slot = 1;
    
    int num_courses_with_slot = 0;
    Course *courses_with_slot[num_courses];
    for (int i = 0; i < num_courses; i++)
    {
        if (courses[i].slot == slot)
        {
            courses_with_slot[num_courses_with_slot] = &courses[i];
            num_courses_with_slot++;
        }
    }

    for (int i = 0; i < num_courses_with_slot; i++)
    {
        Course *course = courses_with_slot[i];
        int num_students = count_students_in_course(course->name);
        if (num_students == 0)
        {
            printf("No students enrolled in course %s\n", course->name);
            continue;
        }
        int num_rows = 4;
        int benches_per_row = (int)ceil((float)num_students / (SEATS_PER_BENCH * num_rows));
        int total_benches = benches_per_row * num_rows;

        classroom *room = NULL;
        for (int j = 0; j < sizeof(classrooms) / sizeof(classroom); j++)
        {
            if (classrooms[j].capacity >= total_benches)
            {
                room = &classrooms[j];
                break;
            }
        }
        if (room == NULL)
        {
            printf("Error: No classroom available for course %s with %d students\n", course->name, num_students);
            continue;
        }

        printf("Assigning seats for course %s in room %s:\n", course->name, room->room_number);

        int assigned_students = 0;
        Student *current_student = student_list;
         int count =1;
        while (current_student != NULL && assigned_students < num_students)
        {   
            for (int j = 0; j < 5; j++)

            {
                   
                if (strcmp(current_student->courses[j], course->name) == 0)
                {
                    int row = (assigned_students / (SEATS_PER_BENCH * benches_per_row)) + 1;
                    int bench = ((assigned_students % (SEATS_PER_BENCH * benches_per_row)) / SEATS_PER_BENCH) + 1;
                    int seat = (assigned_students % SEATS_PER_BENCH) + 1;
                    printf("%s  %s : Room %s, Row %d, Col %d, SEAT_N0- %d\n", current_student->name,current_student->enrollment_number, room->room_number, row, seat, count++);
                    assigned_students++;
                    break;
                }
            }
            current_student = current_student->next;
        }

        room->capacity -= total_benches;
    }
}
int main()
{     
    int choice;
    do
    {
        printf("\n1. Add student\n2. Delete Student\n3. Add Course\n4. Delete Course\n5. Print Student List\n6. Print Course List\n7. Allot Classroom\n8. Allot Classroom For Exams\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            add_student();
            add_student_to_txt();
            break;
        case 2:
            delete_student();
            add_student_to_txt();
            break;
        case 3:
            add_course();
            break;
        case 4:
            delete_course();
            break;
        case 5:
            print_student_list();
            break;
        case 6:
            print_course_list();
            break;
        case 7:
            allot_classrooms();
            break;
        case 8:
            allot_classrooms_for_exams();
            break;
        case 9:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);
    return 0;
}
