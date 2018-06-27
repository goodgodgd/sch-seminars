#include <iostream>
#include <string>

#define STEP1
//#define STEP2

#ifdef STEP1

class Animal
{
public:
    Animal(const std::string _name)
    {
        name = _name;
        std::cout << name << " was created!" << std::endl;
        age = 0;
    }

    void grow()
    {
        age++;
        std::cout << name << " grew up to " << age << std::endl;
    }

private:
    // 객체 지향에서 보통 데이터는 private으로 숨긴다 -> 캡슐화, 데이터 은닉
    std::string name;
    int age;
};

int main()
{
    Animal dog("dog");
    dog.grow();
    dog.grow();
    std::cout << std::endl;

    Animal cat("cat");
    cat.grow();
    cat.grow();
    cat.grow();

    return 0;
}

#endif STEP1



#ifdef STEP2

class Animal
{
public:
    Animal(const std::string _name)
    {
        name = _name;
        std::cout << name << " was created!" << std::endl;
        age = 0;
    }

    void grow()
    {
        age++;
        std::cout << name << " grew up to " << age << std::endl;
    }

    virtual void sound() = 0;

private:
    std::string name;
    int age;
};


class Dog : public Animal
{
public:
    Dog() : Animal("Dog") { }
    void sound() { std::cout << "wal! wal!" << std::endl; }
};

class Cat : public Animal
{
public:
    Cat() : Animal("Cat") { }
    void sound() { std::cout << "nya~o~ng" << std::endl; }
};


int main()
{
    Dog dogdog;
    dogdog.grow();
    dogdog.sound();
    std::cout << std::endl;

    Cat catcat;
    catcat.grow();
    catcat.sound();

    return 0;
}

#endif STEP1
