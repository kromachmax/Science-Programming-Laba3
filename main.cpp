#include "wrapper.h"
#include "engine.h"


class A
{
public:
    A(int base = 0) : m_base(base) { ; };
    int foo(int a) { return a + m_base; };
    int sum(int a, int b) { return a + b; };
    int delta(int a, int b) { return a - b; };

private:
    int m_base;
};


int main()
{

    A b1(10);
    Wrapper a_foo(&b1, &A::foo, { {"arg1", 2} });
    Wrapper b_sum(&b1, &A::sum, { {"arg1", 0},  {"arg2", 0} });
    Wrapper c_delta(&b1, &A::delta, { {"arg1", 0},  {"arg2", 0} });

    Engine engine;

    engine.register_command(&a_foo, "command1");
    engine.register_command(&b_sum, "command2");
    engine.register_command(&c_delta, "command3");

    std::cout << engine.execute("command1", { {"arg1", 4} }) << std::endl;
    std::cout << engine.execute("command2", { {"arg1", 4}, {"arg2", 5} }) << std::endl;
    std::cout << engine.execute("command3", { {"arg2", 5}, {"arg1", 7} }) << std::endl; 
        
    return 0;
}
