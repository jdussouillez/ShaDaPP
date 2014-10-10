#ifndef TEST_H
#define TEST_H

namespace shadapp {

    class Test {
    private:
        int x;

    public:
        Test();

        void exec(std::string const& str) const;
    };
}

#endif // TEST_H
