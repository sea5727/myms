#include <iostream>
#include <typeinfo>
 
struct Base { virtual ~Base() = default; };
struct Derived : Base {};
 
enum class testenum : int
{
    t1 = 0,
    t2 = 1,
    t3 = 2,
};
int main() {
	Base b1;
	Derived d1;
 
	const Base *pb = &b1;
	std::cout << typeid(*pb).name() << '\n';
	pb = &d1;
	std::cout << typeid(*pb).name() << '\n';

    testenum t = testenum::t1;
    testenum t2 = testenum::t2;
    testenum t3 = testenum::t3;

    std::cout << typeid(t).name() << '\n';
    std::cout << typeid(t2).name() << '\n';
    std::cout << typeid(t3).name() << '\n';
    std::cout << typeid(testenum::t1).name() << '\n';
}