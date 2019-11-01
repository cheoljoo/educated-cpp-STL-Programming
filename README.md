# educated-cpp-STL-Programming

## git sub module (**no use** this function in this project)
- git submodule add https://github.com/codenuri/STL.git

### get sub module when you clone git.
- git clone --recurse-submodules https://github.com/codenuri/STL.git

### When you clone git only, you run the following command.
- git submodule init
- git submodule update 
    - Then you can show the file in STL

### run it when you want to sync-up with recent remote repository.
- git submodule update --remote STL


### reference
- git submodules : https://git-scm.com/book/en/v2/Git-Tools-Submodules


# 1. STL 소개 와 STL Preview
- c++ 20 : 2020 : www.isocpp.org 참고
- http://www.cppreference.com 
- www.boost.org - c++20/23등에 추가될 라이브러리를 미리 경험
- 소스 위치 : https://github.com/codenuri/STL.git

## 1-2 : string & complex
- std::string
  - <string> 헤더
  - strcpy / strcmp() 대신 사용
  - char* c1.c_str() : const char* 을 반환
  - data() : C++11이상은 null 문자를 끝나는 것을 보장.  c_str()과 같다.
  - <string.h> <cstring>  -> C 문자열 함수를 위한 헤더 
  - stod() : string to double
  - to_string(5.4)  : string으로 변환
- literals (사용자 정의 literal)
  - foo("hello"s);   이 문자열을 string 객체로 봐 달라. <- user define literal (C++11)
  - "hello"s : string  =>  별도의 using namespace std::string_literals;   필요
  - 3s : seconds
  - 3i : complex<double>
  - 모든 literal을 다 쓰고 싶으면 : using namespace std::literals;  
- std::complex
  - 복소수를 나타내는 클래스 
  - <complex> 헤더
  - .real() .imag() member함수
  - **Literals : complex<double> 3.2i , complex<float> 3.2if , complex<long double> 3.2il 형태로 사용**
  
## 1-3 : bitset & pair
- std::bitset
  - bit를 관리하는 클래스
  - <bitset> 헤더
  - bitset<8> b1 = 0xf0;    0b11110000; 
  - 변환함수 : to_string() / to_ulong() / to_ullong()
  - set() : 모두 1로
  - reset() : 모두 0으로
  - b1[0].flip()
  - 조사 함수 : test , all , none , any , count
  - 비트 연산 가능 : `& , | , ^`
- std::pair
  - ```
    template<typename T1 , typename T2> struct pair {
      T1 first;
      T2 second;
    };
    ```
  - 서로 다른 타입의 객체를 2개를 보관하는 타입
  - <utility> 헤더   -> iostream을 쓰면 utility도 알아서 부름
  - first,second멤버를 통해서 요소를 접근
  - 함수가 2개의 값을 리턴하고 싶을때 pair로 리턴
    - (예제) set::insert 멤버 함수 
  - tuple
    - pair보다 일반화
    - 서로 다른 타입의 객체를 N개 보관
  - make_pair
    - template<typename T> void foo(T p)
    - pair<int,double> p1(1,2.1); foo(p1);
    - foo( pair<int,int>(1,1) );   여기서는 꼭 <int,int>를 꼭 적어주어야함.
    - foo( make_pair<int,int>(1,1) );  or foo( make_pair(1,1) ); 1,1을 보고 알아서 int,int로 유추
    - pair를 만드는 helper함수
    - 클래스 템플릿은 템플릿 인자를 생략할수 없기 때문에 항상 복잡하다.
    - 함수 템플릿은 템플릿 인자를 생략할수 있기 때문에 사용하기 쉽니다. 
    - c++17 부터는 클래스 템플릿도 인자를 생략할수 있다.    foo( pair(1,1) );  

# 2. STL의 설계 원리와 특징
## 2-1 : 일반화 알고르즘의 개념 : generic algorithm #1
- generic algorithm
    - find algorithm 만들기 #1 : 부분문자열도 검색이 가능하도록 개선
        - 시작 주소와 마지막 요소의 주소를 전달 : STL 이 사용하는 방식  (시작 시점 + offset도 있는데 STL은 주소,주소 방식을 사용)
        - STL은 보통 시작 주소는 검색에 포함되지만, 마지막 주소는 검색에 포함하지 않는다. 반개행구간 (half open range : [first,last) )
        - template 사용하여 일반화
        - ```template <typename T> T* efind(T* first, T* last , T value) { ... T ... } ```
        - double x[10];  efind(x,x+10,5); 에서는 5가 double이 아니므로 문제.
            - 구간의 타입과 찾는 요소의 타입이 연관되어있다. 
            - 구간의 표현은 반드시 포인터만 사용해야 한다.
        - ```template <typename T1,typename T2> T1 efind(T1 first, T1 last , T2 value) { ... T ... } ```
            - 구간의 타입과 찾는 요소의 타입을 다르게 가지면 문제 해결된다.
            - T1 , T2를 바로 사용하므로 pointer뿐만 아니라 객체도 가능.  단, ++ , * , != , == 연산이 가능해서 search가 된다. 
        - STL : 검색 실패시 NULL pointer보다는 last를 리턴.
        - C++ 표준 STL 의 find 함수 : <algorithm> 헤더
 
## 2-2 : Container & Iterator의 개념
- STL Container
    - Node의 모양 :  data , *next
- Container와 algorithm의 결합
    - 포인터가 아닌 객체를 이용하자.   efind(T1 first...)
    - ++p , *p 을 이용할수 있으면 iterator이다.
- 반복자(iterator)를 만들자
    - 내부적으로 slist안의 Node를 가리키는 멤버가 있어야 한다.  Node<T>* current=0 멤버 데이터
    - efind로 전달하려면 3가지 연산자가 재정의 되어야 한다. (4가지)
        - ++ , * , != , == 제공
        - slist_iterator& operator++()   return *this;
        - T& operator*()  return current->data;
        - bool operator ==(const slist_iterator& it) return current == it.current;
        - bool operator!=(const slist+iterator& it)  return current != it.current;
    - slist안에 자신의 시작과 마지막 다음을 가리키는 반복자를 리턴하는 멤버 함수 제공
        - begin : 처음 노드를 가리키는 iterator 리턴  return slist_iterator<T>(head);
        - end : 마지막 다음노드(0)를 가리키는 iterator 리턴 return slist_iterator<T>(0);
- class slist 안에 약속된 형태로 typedef or using으로 추가 한다. using iterator = slist_iterator<T>;
    - begin :   return iterator(head);
    - slist<int>::iterator p = s.begin();
    - 반복자의 이름을 container안에 iterator라는 이름으로 약속 : typedef or using 으로 제공
        - 컨테이너이름::iterator로 접근
- 반복자 덕분에 efind는 배열 뿐만 아니라 모든 선형 자료 구조에서 "선형 검색"을 수행할수 있다.
- STL 3대 요소
    - container / iterator / algorithm
- STL example
```cpp
    #include <list>
    #include <vector>
    reverse(s.begin()  , s.end());
    
    vector<int>::iterator p = s.begin();
    while(p != s.end()){
        cout << *p << endl;
        ++p;
    }
    
    for(auto& n: s) cout << n << endl;
    
```

- STL 컨테이너의 모든 요소를 열거하는 방법
    - [] 연산자 사용 - vector등 일부 컨테이너만 가능. list는 안됨
    - 반복자 사용 - 대부분 컨테이너 가능
    - range for문 사용   ->   for(auto& n : s)
    - 스트림 반복자 - Section 3 참조
    
# 2-3 : STL의 특징
- member type
    - 변수를 모든 container를 인수로 받자
    ```cpp
    template <typename T>
        void print(T& c){
            auto n = c.front();  // c++ 11에서는 auto로 쓰면 됨.
            //T::value_type p = c.front();   // 이렇게 value_type 이용  
                // g++ 에서는 ::value_type을 static variable로 보므로 error
                // 앞에 typename이라고 적어주어 해결
                // typename을 적으면 : value_type을 타입의 이름으로 해석 **
            typename T::value_type p = c.front();   // 이렇게 value_type 이용  
        }

    template<typename T> class list {
        public:
        typedef T value_type;  // 핵심
        using value_type = T;  // C++11
    };
        list<int>::value_type n = s.front();  // n은 int

        list<double> v = {1,2,3};
        print(v);    //v가 값이든 container든 처리 가능하게 하려면 위와 같이 선언 필요
    ```
    - STL에는 멤버 테이터와 멤버 함수 외에도 "멤버 타입"이라는 개념 제공.
        - typedef or using (C++11) 이용
    - 템플릿 의존 타입 (template dependent type)의 경우 typename을 표기해야 한다.
        - typename T::value_type
    - 템플릿이 아닌 경우는 typename을 표기하지 않는다.
        - list<int>::value_type
    - 반복자에서의 member type
        - 컨테이너가 저장하는 타입
        - 반복자가 가리키는 타입
    ```cpp
    list<int> s = {1,2,3};
    sum( s.begin() , s.end() );

    template<typename T> void sum(T first,T last){
        typename T::value_type s = 0;  // 합을 s에 보관하기 위해서  T::value_type s{}=0;으로 해주어야 좋은 코드
    }
    ```
    - value_type , size_type , iterator , iterator_category
        - `list<int>::size_type n = s.size();`

- C++17과 STL
    - list<int> s1 = {1,2,3};
    - list s1 = {1,2,3};    // C++17 style

- concept
    - std::min   <algorithm>
    - min( { 1,2,3,4,5} ) ; // ok   C++11 initialization 개념
    - min은 < 연산자를 만들어야 한다.
    - concept
        - 특정 함수 (클래스)를 사용하기 위해 타입이 가져야 하는 조건 (type requirement)
        - min함수에 전달되는 인자는 LessThanComparable 을 만족해야 한다.
        - 코드를 사용해서 concept을 정의할수 있다. - C++20부터
    - STL 템플릿이지만 "모든 타입"에 적용되는 것이 아니라 "조건을 만족"하는 타입에 대해서 동작한다.

# 3. 반복자 (Iterator)
## 3-1 : 반복자 기본
- 
