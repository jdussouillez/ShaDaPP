#include <iostream>
#include <shadapp/Test.h>

#include "config.h"

int main(void) {
    shadapp::Test t;
    std::cout << "Version : " << VERSION << std::endl;
    t.exec("Hello World! ");
    return 0;
}
