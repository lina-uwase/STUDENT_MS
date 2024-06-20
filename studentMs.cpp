#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <limits>

#define FILE_NAME "students.csv"

using namespace std;

class Course {
public:
    int id;
    string name;
    Course* next;

    Course() : id(0), name(""), next(nullptr) {}
    Course(int id, string name) : id(id), name(name), next(nullptr) {}

    int getId() const { return id; }
    string getName() const { return name; }
};

class Student {
public:
    int id;
    string first_name;
    string last_name;
    Course* courseList;
    Student* next;

    Student(int id, string first_name, string last_name) :
            id(id), first_name(first_name),
            last_name(last_name), courseList(nullptr), next(nullptr) {}

    int getId() const { return id; }
    string getFirstName() const { return first_name; }
    string getLastName() const { return last_name; }
    string getFullName() const { return first_name + " " + last_name; }

    void enrollInCourse(Course* course) {
        Course* newCourse = new Course(course->id, course->name);
        if (courseList == nullptr) {
            courseList = newCourse;
        } else {
            Course* temp = courseList;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newCourse;
        }
    }

    void printCourses(const map<int, Course>& courseMap) const {
        cout << "Courses: ";
        Course* temp = courseList;
        while (temp != nullptr) {
            cout << courseMap.at(temp->id).getName();
            if (temp->next != nullptr) cout << ", ";
            temp = temp->next;
        }
        cout << endl;
    }
};

class StudentList {
public:
    Student* head;

    StudentList() : head(nullptr) {}

    void addStudent(Student* student) {
        if (head == nullptr) {
            head = student;
        } else {
            Student* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = student;
        }
    }

    Student* findStudentById(int id) {
        Student* temp = head;
        while (temp != nullptr) {
            if (temp->getId() == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Student* findStudentByName(const string& full_name) {
        Student* temp = head;
        while (temp != nullptr) {
            if (temp->getFullName() == full_name) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void printStudents(const map<int, Course>& courseMap) const {
        cout << "Students" << endl;
        cout << "---------------------" << endl;
        Student* temp = head;
        int i = 1;
        while (temp != nullptr) {
            cout << "(" << i << ") : " << temp->getId() << " " << temp->getFullName() << endl;
            temp->printCourses(courseMap);
            temp = temp->next;
            i++;
        }
        cout << "---------------------" << endl;
    }

    void saveToFile() {
        ofstream tempFile("temp.csv");
        Student* temp = head;
        while (temp != nullptr) {
            tempFile << temp->getId() << "," << temp->getFirstName() << "," << temp->getLastName();
            Course* courseTemp = temp->courseList;
            while (courseTemp != nullptr) {
                tempFile << "," << courseTemp->getId();
                courseTemp = courseTemp->next;
            }
            tempFile << endl;
            temp = temp->next;
        }
        tempFile.close();
        remove(FILE_NAME);
        rename("temp.csv", FILE_NAME);
    }
};

void saveStudent(const Student* student) {
    ofstream file(FILE_NAME, ios::app); // Open file in append mode

    if (file.is_open()) {
        file << student->getId() << "," << student->getFirstName() << "," << student->getLastName();

        Course* temp = student->courseList;
        while (temp != nullptr) {
            file << "," << temp->getId();
            temp = temp->next;
        }

        file << endl;
        cout << "Student " << student->getFullName() << " saved successfully!" << endl;
    } else {
        cerr << "Failed to open file " << FILE_NAME << endl;
    }
}

void listStudents(StudentList& studentList, const map<int, Course>& courseMap) {
    ifstream file(FILE_NAME);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream ss(line);
            string id, first_name, last_name;
            getline(ss, id, ',');
            getline(ss, first_name, ',');
            getline(ss, last_name, ',');

            Student* student = new Student(stoi(id), first_name, last_name);

            string course_id;
            while (getline(ss, course_id, ',')) {
                student->enrollInCourse(new Course(stoi(course_id), courseMap.at(stoi(course_id)).getName()));
            }

            studentList.addStudent(student);
        }

        studentList.printStudents(courseMap);

    } else {
        cerr << "Failed to open file " << FILE_NAME << " check if it exists and try again " << endl;
    }
}

void displayMenu() {
    cout << "Welcome to the Student Management System!" << endl;
    cout << "Please enter a command number from the following options:" << endl;
    cout << "1. List all students" << endl;
    cout << "2. Add new students" << endl;
    cout << "3. Enroll a student in a course" << endl;
    cout << "4. Search for a student by ID or name" << endl;
    cout << "5. Exit the program" << endl;
    cout << "----------------------------" << endl;
}

bool validateStudentInput(const string& input, int& id, string& first_name, string& last_name) {
    istringstream student_ss(input);
    if (student_ss >> id >> first_name >> last_name) {
        return true;
    }
    return false;
}

int main() {
    map<int, Course> courseMap;
    courseMap[1] = Course(1, "Mathematics");
    courseMap[2] = Course(2, "Physics");
    courseMap[3] = Course(3, "Chemistry");
    courseMap[4] = Course(4, "Java");
    courseMap[5] = Course(5, "DSA");
    courseMap[6] = Course(6, "Networking");
    courseMap[7] = Course(7, "PHP");

    StudentList studentList;
    int command;
    displayMenu();
    while (true) {
        cout << "Enter command number: " << endl;
        cin >> command;
        cin.ignore(); // ignore the newline character left in the buffer

        switch (command) {
            case 1:
                studentList = StudentList(); // Reset the student list
                listStudents(studentList, courseMap);
                break;
            case 2: {
                // Get number of students the user wants to add
                int number_of_students;
                cout << "How many students do you want to add?" << endl;
                cin >> number_of_students;
                cin.ignore(); // Ignore the newline character left in the buffer

                cout << "Enter students data in format: <id> <first_name> <last_name>" << endl;

                // Get students data
                for (int i = 0; i < number_of_students; i++) {
                    string input;
                    int id;
                    string first_name, last_name;
                    cout << "Enter the data for student (" << i + 1 << ")" << endl;
                    getline(cin, input);
                    if (validateStudentInput(input, id, first_name, last_name)) {
                        Student* student = new Student(id, first_name, last_name);
                        studentList.addStudent(student);
                        saveStudent(student);
                    } else {
                        cerr << "Invalid input format" << endl;
                    }
                }

                cout << "Finished adding students" << endl;
                break;
            }
            case 3: {
                int student_id, course_id;

                cout << "Enter student id: " << endl;
                cin >> student_id;
                cin.ignore();

                Student* student = studentList.findStudentById(student_id);

                if (student == nullptr) {
                    cout << "Student not found" << endl;
                    break;
                }

                cout << "Available courses:" << endl;
                for (const auto &course : courseMap) {
                    cout << course.second.getId() << ". " << course.second.getName() << endl;
                }

                while (true) {
                    cout << "Enter course id to enroll student in (enter 0 to finish): " << endl;
                    cin >> course_id;
                    cin.ignore();

                    if (course_id == 0) {
                        break;
                    }

                    if (courseMap.find(course_id) != courseMap.end()) {
                        student->enrollInCourse(new Course(course_id, courseMap.at(course_id).getName()));
                        cout << "Student enrolled in " << courseMap.at(course_id).getName() << endl;
                    } else {
                        cout << "Invalid course id" << endl;
                    }
                }

                // Update the file with the new course enrollment
                studentList.saveToFile();

                cout << "Student enrolled in courses successfully!" << endl;

                // Reload the students from the file
                studentList = StudentList(); // Reset the student list
                listStudents(studentList, courseMap);
                break;
            }
            case 4: {
                string searchTerm;
                cout << "Enter the ID or full name of the student to search: ";
                getline(cin, searchTerm);

                Student* student = nullptr;
                if (isdigit(searchTerm[0])) {
                    student = studentList.findStudentById(stoi(searchTerm));
                } else {
                    student = studentList.findStudentByName(searchTerm);
                }

                if (student != nullptr) {
                    cout << "Student found:" << endl;
                    cout << student->getId() << " " << student->getFullName() << endl;
                    student->printCourses(courseMap);
                } else {
                    cout << "Student not found" << endl;
                }
                break;
            }
            case 5:
                cout << "Exiting the program..." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid command" << endl;
                displayMenu();
                break;
        }
    }
    return 0;
}
