#include <iostream>
#include <shadapp/Test.h>

namespace shadapp {

    Test::Test()
    : x(2) {
    }

    void Test::exec(std::string const& str) const {
        std::cout << str << std::endl;
    }
}
