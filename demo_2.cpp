#include <string>
#include <utility>
#include <iostream>
#include <list>

using namespace std;

class Subject {
    private:
        string password;
        string subjectName;
        unsigned int weekHours;
        bool season; // 0 = Winter, 1 = Summer

    public:
        // Generic constructor, to be used in Student class.
        Subject() {
            password = "needsSetting";
            subjectName = "needsNaming";
            weekHours = 0;
            season = false;
        }
};

//list <Subject> subjectDB; // Assuming this is already filled out with the available courses.

class Student {
    private:
        char * registerNumber;
        string fullName;
        unsigned int semester;
        list <pair<Subject*, int>> subjectList; // int Grade, pointer to subject in subjectDB

    public:
        /* Setters */
        void setRegisterNumber(char * registerNumber) {
            this->registerNumber = registerNumber;
        }

        void setFullName(string fullName) {
            this->fullName = std::move(fullName);
        }

        void setSemester(unsigned int semester) {
            this->semester = semester;
        }

        /* Getters */
        char * getRegisterNumber() {
            return registerNumber;
        }

        string getFullName() {
            return fullName;
        }

        [[nodiscard]] unsigned int getSemester() const {
            return semester;
        }

        /* Constructors */
        Student(char* registerNumber, string fullName) {
            this->registerNumber = registerNumber;
            this->fullName = std::move(fullName);
            semester = 1;
        }

        Student(char* registerNumber, string fullName, unsigned int semester) {
            this->registerNumber = registerNumber;
            this->fullName = std::move(fullName);
            this->semester = semester;
        }

        Student(Student &that) {
            registerNumber = that.registerNumber;
            fullName  = that.fullName;
            semester = that.semester;
        }

        /* Class Functions */
        void attributePrint() {
            std::cout << "Register Number: " << registerNumber << " | Full Name: " << fullName << " | Semester: "<< semester;
        }

        void operator++() {
            semester ++;
        }

        void operator--() {
            semester --;
        }

        void operator+=(unsigned int subjectNumber) {
            for(int i = 0; i < subjectNumber; i++) {
                subjectList.emplace_front();
            }
        }

        bool operator==(unsigned int semesterNumber) const {
            if (semester == semesterNumber) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator!=(unsigned int semesterNumber) const {
            if (semester != semesterNumber) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator<(unsigned int semesterNumber) const {
            if (semester < semesterNumber) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator>(unsigned int semesterNumber) const {
            if (semester > semesterNumber) {
                return true;
            }
            else {
                return false;
            }
        }


        bool operator<=(unsigned int semesterNumber) const {
            if (semester <= semesterNumber) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator>=(unsigned int semesterNumber) const {
            if (semester >= semesterNumber) {
                return true;
            } else {
                return false;
            }
        }
};

int main () {
    Subject OOP;
    list <Subject> subjectDB;
    subjectDB.emplace_front(OOP);
}


