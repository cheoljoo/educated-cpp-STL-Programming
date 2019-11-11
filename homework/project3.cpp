struct People
{
string name;
int age;
People(string n, int a) : name(n), age(a) {}
void Dump() const { cout << name << "(" << age << ")" << endl; }
};
int main()
{
vector<shared_ptr<People>> v;
// ("kim", 10), ("lee", 15), ("park", 5), ("jung", 20), ("choi", 12) 를 v에 넣어
주세요.
// 이름순으로 정렬해서 출력해 보세요
// 나이순으로 정렬해서 출력해 보세요.
}

//비교 정책을 전달하는 방법은 어떠한 방법을 사용해도 상관없습니다.
// shared_ptr 객체를 만들 때 는 make_shared를 사용해 주세요.
