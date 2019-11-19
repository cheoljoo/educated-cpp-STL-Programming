#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
using namespace std;

struct People
{
    string name;
    int age;
    People(string n, int a) : name(n), age(a) {}
    void Dump() const { cout << name << "(" << age << ")" << endl; }
};

bool operator==(
    std::shared_ptr<People> lhs,
    std::shared_ptr<People> rhs
){
    return true; 
}

bool sortByName(
    std::shared_ptr<People> lhs,
    std::shared_ptr<People> rhs
)  {
    return lhs->name < rhs->name;
}

bool sortByAge(
    std::shared_ptr<People> lhs,
    std::shared_ptr<People> rhs
)  {
    return lhs->age < rhs->age;
}

int main()
{
    vector<shared_ptr<People>> v;
    v.push_back(make_shared<People>("kim",10));
    v.push_back(make_shared<People>("lee",15));
    v.push_back(make_shared<People>("park",5));
    v.push_back(make_shared<People>("jung",20));
    v.push_back(make_shared<People>("chio",12));
    // ("kim", 10), ("lee", 15), ("park", 5), ("jung", 20), ("choi", 12) 를 v에 넣어 주세요.
        // 이름순으로 정렬해서 출력해 보세요
        cout << "== sort by name ==" << endl;
        sort(v.begin(),v.end(),sortByName);
        for(auto n : v)
            n->Dump();

        // 나이순으로 정렬해서 출력해 보세요.
        cout << "== sort by age ==" << endl;
        sort(v.begin(),v.end(),sortByAge);
        for(auto n : v)
            n->Dump();
}

//비교 정책을 전달하는 방법은 어떠한 방법을 사용해도 상관없습니다.
// shared_ptr 객체를 만들 때 는 make_shared를 사용해 주세요.
