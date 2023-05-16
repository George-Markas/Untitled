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
        Subject(string password, string subjectName, unsigned int weekHours, bool season) {
            this->password = std::move(password);
            this->subjectName = std::move(subjectName);
            this->weekHours = weekHours;
            this->season = season;
        }
};

class Student {
    private:
        char * registerNumber;
        string fullName;
        unsigned int semester;
        list<Subject> subjectList;

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

        void operator+=(unsigned int increment) {
            semester += increment;
        }
        void operator-=(unsigned int decrement) {
            semester -= decrement;
        }

        void operator+=(::nullptr_t) {
            subjectList.push_front(Subject()) {
        }
};

int main () {

}
