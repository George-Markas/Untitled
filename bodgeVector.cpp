#include <iostream>

template <typename T> class MyVector {
    private:
        T *vectorPtr = nullptr;
        unsigned int size = 0;
        unsigned filledSlots;

    public:
         explicit MyVector(unsigned int size) {
            if(size != 0) {
                vectorPtr = new T(size);
            }
            this->size = size;
            filledSlots = 0;
        }

        void back_push(T toBeInserted) {
             // If the vector's full, backup contents and remake it with a size increase of 1, transfer contents back.
            if(filledSlots == size) {
                T *bufferPtr = new T(size + 1);
                memcpy(bufferPtr, vectorPtr, filledSlots*sizeof(T));
                delete [] vectorPtr;
                vectorPtr = bufferPtr;
                size++;
            }
            vectorPtr[filledSlots] = toBeInserted;
            filledSlots++;
        }

        T operator[](unsigned int pos) {
            return vectorPtr[pos];
        }

        void print() {
             for (int i = 0; i < size; i++) {
                 std::cout << vectorPtr[i] << std::endl;
             }
             std::cout << std::endl;
        }

};

// Driver Code
int main() {
    MyVector<int> intVector (4);

    // Pushing in integers demo.
    intVector.back_push(1);
    intVector.back_push(2);
    intVector.back_push(3);
    intVector.back_push(4);

    // Exceeding size at this point, dynamic increase.
    intVector.back_push(5);
    intVector.print();

    MyVector <bool> boolVector (2);
    boolVector.back_push(true);
    boolVector.back_push(true);
    boolVector.back_push(false);

    boolVector.print();
}