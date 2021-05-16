#include <iostream>
#include <map>
#include <string>
#include <functional>

using namespace std;

map<string, function<void()>> vtable_base_class;
map<string, function<void()>> vtable_derived_class;


#define VIRTUAL_CALL(self_class_ptr, fun_name)\
if (((self_class_ptr)->vtable_ptr->find(#fun_name)) != ((self_class_ptr)->vtable_ptr->end())) { \
        (*((self_class_ptr)->vtable_ptr->find(#fun_name))).second();\
    } else { \
        throw;\
}


#define DECLARE_METHOD(class_name, fun_name, fun_code) { \
    auto self_method = [&] () {fun_code};\
    if (#class_name == base_class_name) { \
            vtable_base_class[#fun_name] = self_method; \
        } \
    vtable_derived_class[#fun_name] = self_method; \
}


#define END_BASE(class_name) \
    map<string, function<void()>> *vtable_ptr = &vtable_base_class; \
}; \

string base_class_name = #class_name;

#define END_DERIVED(class_name) \
    map<string, function<void()>> *vtable_ptr = &vtable_derived_class; \
};

struct Base {
END_BASE(Base)

struct Derived : public Base {
END_DERIVED(Derived)


int main() {
    DECLARE_METHOD(Base, Both, cout << "Base::Both"  << endl;)
    DECLARE_METHOD(Base, OnlyBase, cout << "Base::OnlyBase" << endl;)

    DECLARE_METHOD(Derived, Both, cout << "Derived::Both" << endl;)
    DECLARE_METHOD(Derived, OnlyDerived, cout << "Derived::OnlyDerived" << endl;)

    Base *base = new Base();
    Derived derived;
    Base* reallyDerived = reinterpret_cast<Base*>(&derived);

    VIRTUAL_CALL(base, Both);
    VIRTUAL_CALL(reallyDerived, Both);
    VIRTUAL_CALL(&derived, Both);
    VIRTUAL_CALL(base, OnlyBase);
    VIRTUAL_CALL(reallyDerived, OnlyBase);
    VIRTUAL_CALL(&derived, OnlyBase);
    VIRTUAL_CALL(base, Both);
    VIRTUAL_CALL(reallyDerived, Both);
    VIRTUAL_CALL(reallyDerived, OnlyBase);
    delete base;
    return 0;
}