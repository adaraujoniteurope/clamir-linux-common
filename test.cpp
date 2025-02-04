#include <iostream>

class abstract_base
{
    public:
    virtual void proc() = 0;
};

class concrete : public abstract_base
{
    public:
    virtual void proc() override {
        std::cout << "hello from concetre" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    abstract_base* generic;
    concrete impl;
    generic = &impl;

    generic->proc();
    return 0;
}
