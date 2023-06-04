#include <iostream>

template <typename T> class MyVector {
    private:
        T *vectorPtr = nullptr;
        unsigned int size{};
        unsigned filledSlots;

    public:
        explicit MyVector(unsigned int size) {
            this->size = size;
            filledSlots = -1;
            vectorPtr = new T(size);
        }

        void back_push(T toBeInserted) {
            filledSlots++;

            // If the vector's full, backup contents and remake it with a size increase of 1, transfer contents back.
            if(filledSlots >= size) {
                T *bufferPtr = new T(size);
                memcpy(bufferPtr, vectorPtr, filledSlots*sizeof(T));
                delete [] vectorPtr;
                vectorPtr = new T(size + 1);
                memcpy(vectorPtr, bufferPtr, filledSlots*sizeof(T));
                delete bufferPtr;
                size++;
            }
            vectorPtr[filledSlots] = toBeInserted;
        }

        T operator[](unsigned int pos) {
            return vectorPtr[pos];
        }

};

// Driver Code
int main() {
    MyVector <int> sampleVector (1);

    // Pushing in an integer demo.
    sampleVector.back_push(1337);

    // Dynamic reallocation demo.
    sampleVector.back_push(256);

    // Overloaded operator demo
    std::cout << sampleVector[1];
}