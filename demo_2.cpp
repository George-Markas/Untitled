#include <string>
#include <utility>
#include <iostream>

using namespace std;

class Student {
    private:
        char * registerNumber;
        string fullName;
        unsigned int semester;

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
};

int main () {
    char sample_register[8] = "2239000";
    string sample_name = "John Doe";

    Student Sample_1(sample_register, sample_name);
    Sample_1.setFullName("Mpekos");
    Sample_1.attributePrint();
}