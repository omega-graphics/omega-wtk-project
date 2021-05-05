#include <OmegaWTK.h>

int main(int argc,char *argv[]){

    using namespace OmegaWTK;
    int _max = 10;
    std::string str = "Test";

    Core::PriorityQueueHeap<int,std::greater<int>> test(10);

    while(!test.full()){
        auto el = 1 + std::rand() % _max;
        test.push(el);
        printf("New Element: %i \n",el);
    };

    printf("This Queue Heap is now full with %i number of elements\n",test.length());

    while(!test.empty()){
        auto ele = test.first();
        test.pop();
        printf("Element: %i \n",ele);
    };

     printf("This Queue Heap is now empty with %i number of elements\n",test.length());


    std::cout << "Hello World" << std::endl;
    return 0;
};