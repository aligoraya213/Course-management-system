#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// Constants
const int MAX_COURSES = 100;
const int MAX_STUDENTS = 30;
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "password";
const string DATA_FILE = "courses_data.txt";

// Abstract Interface Class
class CourseManagementInterface {
public:
    virtual void addCourse() = 0;
    virtual void registerStudent() = 0;
    virtual void displayCoursesWithParticipants() const = 0;
    virtual void displayCoursesWithoutParticipants() const = 0;
    virtual void viewAllCourses() const = 0;
    virtual void saveData() const = 0;
    virtual void loadData() = 0;
};
bool adminLogin() {
    string username, password;
    while (true) {
        cout << "\t\t\t\t\tEnter username: ";
        cin >> username;
        cout << "\t\t\t\t\tEnter password: ";
        cin >> password;
        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            cout << "\n\t\t\t\t\tLogin successful!\n";
            return true;
        } else {
            cout << "\t\t\t\t\tInvalid username or password. Please try again.\n";
        }
    }
}
// Person class
class PERSON {
public:
    string surname, firstname, email;
    string getDetails() const {
        return firstname + " " + surname + " (" + email + ")";
    }
};

// Lecturer class
class Lecturer : public PERSON {
public:
    string academic_title;
};

// Student class
class Student : public PERSON {
public:
    int student_id;
    string getDetailsWithID() const {
        return to_string(student_id) + "   | " + firstname + " " + surname + "   | " + email;
    }
};

// Course class
class Course {
public:
    int course_id, participant_count = 0;
    string course_name;
    Lecturer lecturer;
    Student participants[MAX_STUDENTS];
    bool isFullyBooked() const {
        return participant_count >= MAX_STUDENTS;
    }
};

// Derived class implementing the interface
class CourseManagementSystem : public CourseManagementInterface {
private:
    Course allCourses[MAX_COURSES];
    int courseCount = 0;

    bool isDuplicateCourseID(int courseID) const {
        for (int i = 0; i < courseCount; ++i)
            if (allCourses[i].course_id == courseID) return true;
        return false;
    }

    bool isDuplicateStudentID(const Course& course, int studentID) const {
        for (int i = 0; i < course.participant_count; ++i)
            if (course.participants[i].student_id == studentID) return true;
        return false;
    }
   
    void displayCourseHeader() const {
        cout << "\t\t\t\t\t===============================================================\n";
        cout << " \t\t\t\t\tCourse ID | Course Name              | Lecturer               \n";
        cout << "\t\t\t\t\t===============================================================\n";
    }

    void displayCourseRow(const Course& course) const {
        cout << "\t\t\t\t\t"<<setw(10) << course.course_id << " | "
             << setw(23) << course.course_name << " | "
             << course.lecturer.getDetails() << "\n" << endl;
    }

    void displayStudentHeader() const {
        cout << "\t\t\t\t\t===============================================================\n";
        cout << " \t\t\t\t\tStudent ID | Name                   | Email                  \n";
        cout << "\t\t\t\t\t===============================================================\n";
    }

    void displayStudentRow(const Student& student) const {
        cout <<"\t\t\t\t\t"<< setw(11) << student.student_id << " | "
             << setw(23) << (student.firstname + " " + student.surname) << " | "
             << student.email << "\n" << endl;
    }

public:
    void addCourse() override {
        if (courseCount >= MAX_COURSES) {
            cout << "\t\t\t\t\tCannot add more courses. Maximum limit reached.\n";
            return;
        }
        int courseID;
        cout << "\t\t\t\t\tEnter a unique Course ID: ";
        cin >> courseID;
        if (isDuplicateCourseID(courseID)) {
            cout << "\t\t\t\t\tCourse ID already exists.\n";
            return;
        }
        Course& course = allCourses[courseCount];
        course.course_id = courseID;
        cout << "\t\t\t\t\tEnter course name: ";
        cin.ignore();
        getline(cin, course.course_name);
        cout << "\t\t\t\t\tEnter lecturer details:\n\t\t\t\t\tAcademic title: ";
        getline(cin, course.lecturer.academic_title);
        cout << "\t\t\t\t\tFirst name: ";
        getline(cin, course.lecturer.firstname);
        cout << "\t\t\t\t\tLast name: ";
        getline(cin, course.lecturer.surname);
        cout << "\t\t\t\t\tEmail: ";
        getline(cin, course.lecturer.email);
        courseCount++;
        cout << "\t\t\t\t\tCourse added successfully.\n";
    }

    void registerStudent() override {
        if (courseCount == 0) {
            cout << "\t\t\t\t\tNo courses registered yet.\n";
            return;
        }
        cout << "\t\t\t\t\tAvailable courses:\n";
        displayCourseHeader();
        for (int i = 0; i < courseCount; ++i)
            displayCourseRow(allCourses[i]);
        int courseID;
        cout << "\t\t\t\t\tEnter the course ID to register a student: ";
        cin >> courseID;
        Course* selectedCourse = nullptr;
        for (int i = 0; i < courseCount; ++i)
            if (allCourses[i].course_id == courseID) {
                selectedCourse = &allCourses[i];
                break;
            }
        if (!selectedCourse) {
            cout << "\t\t\t\t\tInvalid course ID.\n";
            return;
        }
        if (selectedCourse->isFullyBooked()) {
            cout << "\t\t\t\t\tCourse is fully booked.\n";
            return;
        }
        int studentID;
        cout << "\t\t\t\t\tEnter a unique Student ID: ";
        cin >> studentID;
        if (isDuplicateStudentID(*selectedCourse, studentID)) {
            cout << "\t\t\t\t\tStudent ID already exists.\n";
            return;
        }
        Student& student = selectedCourse->participants[selectedCourse->participant_count];
        student.student_id = studentID;
        cout << "\t\t\t\t\tEnter student's first name: ";
        cin >> student.firstname;
        cout << "\t\t\t\t\tEnter student's last name: ";
        cin >> student.surname;
        cout << "\t\t\t\t\tEnter student's email: ";
        cin >> student.email;
        selectedCourse->participant_count++;
        cout << "\t\t\t\t\tStudent registered successfully.\n";
    }
void checkCourseCancellationWarnings() const {
    cout << "\t\t\t\t\tChecking for courses with low enrollment...\n";
    bool warningsIssued = false;

    for (int i = 0; i < courseCount; ++i) {
        const Course& course = allCourses[i];

        if (course.participant_count < 3) {
            warningsIssued = true;
            cout << "\t\t\t\t\t===========================================\n";
            cout << "\t\t\t\t\tWarning for Course ID: " << course.course_id
                 << " - " << course.course_name << "\n";
            cout << "\t\t\t\t\tLecturer: " << course.lecturer.getDetails() << "\n";
            cout << "\t\t\t\t\tParticipants: " << course.participant_count << "\n";

            if (course.participant_count > 0) {
                displayStudentHeader();
                for (int j = 0; j < course.participant_count; ++j) {
                    displayStudentRow(course.participants[j]);
                }
            } else {
                cout << "\t\t\t\t\tNo participants in this course.\n";
            }
            cout << "\t\t\t\t\tThis course may be canceled if enrollment does not increase.\n";
        }
    }

    if (!warningsIssued) {
        cout << "\t\t\t\t\tAll courses have sufficient enrollment.\n";
    }
}

    void displayCoursesWithParticipants() const override {
    cout << "\t\t\t\t\tCourses with Participants:\n";
    for (int i = 0; i < courseCount; ++i) {
        if (allCourses[i].participant_count > 0) {
            displayCourseHeader();
            displayCourseRow(allCourses[i]);
            displayStudentHeader();
            for (int j = 0; j < allCourses[i].participant_count; ++j) {
                displayStudentRow(allCourses[i].participants[j]);
            }
        }
    }
}




    void displayCoursesWithoutParticipants() const override {
        cout << "\t\t\t\t\tCourses with No Participants:\n";
        displayCourseHeader();
        bool found = false;
        for (int i = 0; i < courseCount; ++i) {
            if (allCourses[i].participant_count == 0) {
                displayCourseRow(allCourses[i]);
                found = true;
            }
        }
        if (!found) cout << "\t\t\t\t\tAll courses have participants.\n";
    }

void viewAllCourses() const override {
    if (courseCount == 0) {
        cout << "\t\t\t\t\tNo courses available.\n";
        return;
    }
    cout << "\t\t\t\t\tAll Courses:\n";
    for (int i = 0; i < courseCount; ++i) {
        displayCourseHeader();
        displayCourseRow(allCourses[i]);

        if (allCourses[i].participant_count > 0) {
            displayStudentHeader();
            for (int j = 0; j < allCourses[i].participant_count; ++j) {
                displayStudentRow(allCourses[i].participants[j]);
            }
        } else {
            cout << "\t\t\t\t\tNo participants.\n";
        }

        // Add a separator between courses for better readability
        cout << "\t\t\t\t\t---------------------------------------------------------------\n";
    }
}



    void saveData() const override {
        ofstream outFile(DATA_FILE.c_str());
        if (outFile.is_open()) {
            for (int i = 0; i < courseCount; ++i) {
                const Course& course = allCourses[i];
                outFile << "\t\t\t\t\tCourse ID: " << course.course_id << ", Name: " << course.course_name << "\n";
                outFile << "\t\t\t\t\tLecturer: " << course.lecturer.academic_title << " " << course.lecturer.firstname
                        << " " << course.lecturer.surname << " (" << course.lecturer.email << ")\n";
                outFile << "\t\t\t\t\tParticipants:\n";
                for (int j = 0; j < course.participant_count; ++j) {
                    const Student& student = course.participants[j];
                    outFile << "\t\t\t\t\tID: " << student.student_id << ", Name: " << student.firstname
                            << " " << student.surname << "\n";
                }
                outFile << "\n";
            }
            outFile.close();
            cout << "\t\t\t\t\tData saved successfully.\n";
        }
    }

  void loadData() override {
    ifstream inFile(DATA_FILE.c_str());
    if (!inFile.is_open()) {
        cout << "\t\t\t\t\tNo existing data found. Starting fresh.\n";
        return;
    }

    courseCount = 0; // Reset course count
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        Course& course = allCourses[courseCount];
        stringstream ss(line);

        // Parse course ID and course name
        ss.ignore(10, ':'); // Ignore "Course ID:"
        ss >> course.course_id;
        ss.ignore(10, ','); // Ignore ", Name:"
        getline(ss, course.course_name);

        // Parse lecturer details
        getline(inFile, line); // Read lecturer line
        stringstream lecturerLine(line);
        lecturerLine.ignore(10, ':'); // Ignore "Lecturer:"
        lecturerLine >> course.lecturer.academic_title >> course.lecturer.firstname >> course.lecturer.surname;
        lecturerLine.ignore(10, '('); // Ignore up to the opening parenthesis
        getline(lecturerLine, course.lecturer.email, ')'); // Read email up to closing parenthesis

        // Skip the "Participants:" line
        getline(inFile, line);

        // Parse participant details
        course.participant_count = 0;
        while (getline(inFile, line) && !line.empty()) {
            Student& student = course.participants[course.participant_count];
            stringstream studentLine(line);
            studentLine.ignore(10, ':'); // Ignore "ID:"
            studentLine >> student.student_id;
            studentLine.ignore(10, ','); // Ignore ", Name:"
            studentLine >> student.firstname >> student.surname;
            studentLine.ignore(10, ':'); // Ignore ", Email:"
            studentLine >> student.email;
            course.participant_count++;
        }

        // Move to the next course
        courseCount++;
    }

    inFile.close();
    cout << "\t\t\t\t\tData loaded successfully.\n";
}


};

// Main function
int main() {
	if (!adminLogin()) return 0;
    CourseManagementSystem cms;
    cms.loadData();
    int choice;
    do {
        cout << "\t\t\t\t\t===========================================\n";
        cout << "\t\t\t\t\tCOURSE MANAGEMENT SYSTEM\n";
        cout << "\t\t\t\t\t===========================================\n";
           cout << "\t\t\t\t\t1. Add a new course\n"
     << "\t\t\t\t\t2. Register a student for a course\n"
     << "\t\t\t\t\t3. Display courses with participants\n"
     << "\t\t\t\t\t4. Display courses with no participants\n"
     << "\t\t\t\t\t5. Check for course cancellation warnings\n"
     << "\t\t\t\t\t6. View all Detail\n"
     
     << "\t\t\t\t\t7. Save data\n"
     << "\t\t\t\t\t8. Exit\n";

        cout << "\t\t\t\t\t===========================================\n";
        cout << "\t\t\t\t\tEnter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cms.addCourse();
            break;
        case 2:
            cms.registerStudent();
            break;
        case 3:
            cms.displayCoursesWithParticipants();
            break;
        case 4:
            cms.displayCoursesWithoutParticipants();
            break;
        
        case 5:
        	cms.checkCourseCancellationWarnings();
            
            break;
        case 6:
    cms.viewAllCourses();
    break;
case 7:
    cms.saveData();
    break;
case 8:
    cout << "\t\t\t\t\tExiting program. Goodbye!\n";
    break;

        default:
            cout << "\t\t\t\t\tInvalid choice. Please try again.\n";
        }
    } while (choice != 7);
    return 0;
}
