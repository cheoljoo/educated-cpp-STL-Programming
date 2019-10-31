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


# STL 소개 와 STL Preview
- c++ 20 : 2020 : www.isocpp.org 참고
- http://www.cppreference.com 
- www.boost.org - c++20/23등에 추가될 라이브러리를 미리 경험
- 소스 위치 : https://github.com/codenuri/STL.git

# 1-2 : string & complex
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
  
# 1-3 : bitset & pair
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
  
