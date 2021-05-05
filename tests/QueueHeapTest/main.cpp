#include <OmegaWTK.h>

int main(int argc,char *argv[]){

    using namespace OmegaWTK;
    int num = 0;
    std::string str = "Test";

    Core::QueueHeap<std::string> test(5);


    while(!test.full()){
        test.push(str + std::to_string(++num));
        printf("New Element: %s \n",test.last().c_str());
    };

    printf("This Queue Heap is now full with %i number of elements\n",test.length());

    while(!test.empty()){
        auto ele = test.first();
        test.pop();
        printf("Element: %s \n",ele.c_str());
    };

     printf("This Queue Heap is now empty with %i number of elements\n",test.length());


    std::cout << "Hello World" << std::endl;
    return 0;
};