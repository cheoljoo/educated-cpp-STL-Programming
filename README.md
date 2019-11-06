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
- 반복자(Iterator) in GoF's design pattern
    - provice a way to access the elements of an aggregate object sequentially without exposing its underlying representation.
    - 복합 객체의 내부 구조에 상관없이 순차적으로 요소에 접근하는 방법을 제공하는 것
- 반복자 (iterator) in STL
    - ++ 연산자로 이동가능하고 , * 연산자로 요소에 접근 가능한 것 
    - 대표적인 것은 C의 pointer (STL 에서는 포인터도 반복자)
    - directory_iterator  ++p : 다음 하위 폴더  // C++17부터 제공  :: 재미난 모양의 반복자 예제
        - using namespace std::experimental::filesystem;
        - g++ iterator1.cpp -std=c++1z -lstdc++fs
- 반복자의 다양한 형태
    - Raw Pointer
    - container의 요소를 열거위한 객체 ( begin() )
    - stream iterator
    - insert iterator
    - directory iterator ...
- contailer에서 반복자를 꺼내는 다양한 방법들
    - list<int>::iterator p = s.begin();
    - auto p1 = s.begin;    // C++11
    - int s[5]; // 진짜 배열을 썼을 경우 
        - auto p1 = begin(s);  // 이렇게 일반 함수를 사용.  배열도 가능하게 함수 overloading 해 놓음. C++11
        - int n = size(s);  empty(s)  , end(s)  // 이렇게 일반함수 사용하는게 더 일반적이됨. 
        - auto p2 = end(s); // 이때 end일때는 마지막 다음을 가리키므로 dereferencing을 하면 안된다. 끝을 아는 용도로만
- 반복자의 무효화 현상 : auto p = begin(v);  v.resize(100); 일경우에 전에 *p 을 사용할수 없음.
    - vector등의 컨테이너의 내부 버퍼가 재할당될때
    - 반복자가 가리키던 요소가 제거(erase)될때
    - 컨테이너의 종류에 따라 무효화되는 조건이 다르다.  (vector는 무효화 가능할수 있지만, list는 무효화되지 않는다.)
- 반복자의 구간 (range)
    - [first,last) 
    - 유효한 구간 : first부터 시작해서 ++연산으로 last에 도달할수 있는 구간
    - empty 구간 : fist == last 인 경우,   빈 구간도 유효한 구간이다.
- copy 알고리즘
    - copy (x , x+5 , begin(y) );  출력 for(auto& n : y) { cout << n << endl; }
    - <algorithm>

## 3-2. 반복자 카테고리 (iterator category)
- vector<int> v ...  ; sort( begin(v) , end(v) );
- list<int> v 로 하면 에러가 나온다.   // ++p , --p 가능.    //이유는 list는 sort 사용할수 없다. 
- forward_list<int> s1 ;   ++p; 문제 없지만,  --p; 는 에러
- 반복자들도 연산이 모두 다르다. 
- 반복자는 적용할수 있는 연산에 따라 5가지(C++17부터는 6가지)로 분류
    - 입력 반복자 (input iterator) : 입력 (=*i) , ++
    - 출력 반복자 (output iterator) : 출력 (*i=) , ++
    - 전진형 반복자 (forward iterator) : 입력 , ++ , multiple pass                  <forward_list>
    - 양방향 반복자 (bidirectional iterator) : 입력 , ++ , -- , multiple pass       <list>
    - 임의 접근 반복자 (random access iterator) : 입력 , ++ , -- , + , - , []       <vector> <deque>
    - 인접한 반복자 (contiguous iterator)- C++17 : 입력 , ++ , -- , + , - , [] , *(i+n) == *(addressof(*a) + n)    <vector>
- 출력 가능한 반복자 : "mutable iterator"

- multipass guarantee 개념
    - i1 == i2 일때 *i1 == *i2   &&    i1==i2일때 ++i1 == ++i2 
    - 2개 이상의 반복자가 컨테이너의 요소에 동일하게 접근함을 보장
    - list의 반복자는 multipass를 보장한다. 
    - stream / insert iterator는 multipass를 보장하지 못한다.

- 다양한 알고리즘의 인자로 요구하는 반복자 카테고리가 무엇인지 알아야 한다. 
    - auto p = find(begin(v) , end(v) , 5);  // 1,2인자 최소 요구조건 - 입력 반복자
    - reverse(begin(v) , end(v));   // 양방향
    - sort(begin(v) , end(v));      // 임의 접근 반복자 : 반으로 갈라야 한다.
        - list<int> s2는 sort(s2); // error
        - s2.sort(); // ok   quick sort가 아닌 다른 알고리즘을 사용했을 것임
        - vector<int> v;  v.sort();  // 멤버함수 sort()가 있을까?  NO  -> 일반 함수 (알고리즘) sort()를 사용하면 된다.

- tag dispatching #1 (1. 함수 인자를 사용하는 overloading)
    - list<int> s;   // 반복자 p를 5칸 전진하고 싶다.
    - advance(p,5);   
        - <iterator> 헤더
        - 반복자를 N만큼 전진(후진)하는 함수
        - 임의반복자와 그렇지 않은 경우를 분리하여 구현 하면 성능도 좋아진다. vector<int> , list<int>는 다르게 처리
    - iterator category tag
        - <iterator> 헤더
        - 반복자의 5가지 category를 나타내는 타입. empty class로 제공
        - C++17에서 추가된 contiguous iterator는 별도의 tag가 제공되지 않음.
        - using iterator_category = bidirectional_iterator_tag;
        - template<typename T>  void advance_imp(p,n,  typename T::iterator_category() ); // 객체여야 함.
- tag dispatching #2 
    - 2. is_same< , > 사용  / <type_traists>  / C++17 if constexpr 사용 (함수 1개)
        - if constexpr (is_same< T::iterator_category , random_access_terator_tag>::value) { }
    - 3. enable_if 를 사용 (SFINAE 개념 사용)  C++11  (함수 2개)
        - template <typename T>  enable_if<is_same<typename T::iterator_category,random_access_iterator_tag>::value>::type advance(T& p,intn){ }
        - template <typename T>  enable_if<! is_same<typename T::iterator_category,random_access_iterator_tag>::value>::type advance(T& p,intn){ }

- 반복자와 member type
    - 1. 반복자를 만들때 반드시 member type을 제공해야 한다. 
        - using terator_category = forward_iterator_tag;
        - using value_type = T;
        - using pointer = T*;
        - using reference = T&;
        - using difference_type = std::ptrdiff_t;
    - 2. template<typename T> class slist_iterator : public iterator< forward_iterator_tag , T> { } 
        - <iterator> 헤더
        - C++17부터는 사용되지 않음.
- 알고리즘과 category
    - template 인자들도 category 이름으로 적어주면 좋음.
    
## 3-3. 반복자 특질 (iterator traits)
- 반복자 타입::value_type
    - template <typename T> typename T::value_type  sum(T f,T s){ auto s = $f; }
    - typename T::value_type s = 0;
- 반복자는 2가지 형태
    - user define type으로 만들어진 반복자 : value_type이 있다. 
    - raw pointer (array)는 value_type이 없다.
- array일때도 해결하는 방법
    - // 핵심 : template을 사용할때는 부분특수화 가능하다.
    - template<typename T> struct iterator_traits { using value_type=typename T::value_type; }
    - template<typename T> struct iterator_traits<T*> { using value_type = T; }
    - template <typename T> typename iterator_traits<T>::value_type  sum(T f,T s){ auto s = $f; }
- value_type을 사용하는 2가지 방식
    - T::value_type
    - iterator_traits<T>::value_type   <- **Raw pointer도 해결되므로 꼭 이것을 사용하자.**

- template<class T> struct iterator_traits<T*>{ using iterator_category = random_access_iterator_tag; ... }

- auto / decltype을 사용하는 경우 
    - decltype(*first) s = 0;   // 반복자의 역참조의 type에 담겠다. error
        - decltype( *포인터타입 ) 
    - typename remove_reference<decltype(*first)::type s = 0;   // ok.
    - type deduction 규칙을 정확히 알고 사용해야 한다.

## 3-4. 삽입 반복자 (insert iterator)
- 삽입 반복자(insert iterator)
    - s.push_back(10);
    - back_insert_iterator< list<int> > p(s);   *p = 20;  // s.push_ack(20);
        - 삽입 반복자 사용하여 추가 가능 (전방 삽입 , 후방 삽입 , 임의 삽입)
    - 기본 모양
        - back_insert_iterator<컨테이너 클래스 이름> p(컨테이너 객체);
    - int x[5] = {1,2,3,4,5};   for(int i=0;i<5;i++){ s.push_back(x[i]); }
        - copy(x,x+5,p);  // p는 위의 back_insert_iterator 이다. 이렇게 한번에 push가 가능한다.
    - copy와 같은 STL 알고리즘을 사용하여 컨테이너에 요소를 추가 할수 있다.
        - 꼭 이것 아니어도 s.end()를 copy argument로 넣어주어도 될 것으로 보임.  <- 이것은 덮어 쓴 것이라고함. push_back이 아닌 덮어 쓴 것
    - template<typename T> back_insert_iterator<T>  back_inserter(T& c){ return back_insert_iterator<T>(c); }
        - copy(x , x+5 , back_inserter(s) ); // **back_inserter() c++ 11에 이미 제공**

- 삽입 반복자의 종류
    - back_insert_iterator   : back_inserter()
    - front_insert_iterator  : front_inserter()   // 1,2,3,4,5를 앞쪽에 집어 넣으면 결과는 뒤집어져있다. 
    - insert_iterator        : inserter()
- 주의 사항
    - vector는 앞에 삽입할수 없다.  (push_front 멤버 함수가 없다.)
    - 임의 삽입의 경우 생성자 인자가 2개 이다.  (컨테이너와 삽입할 위치를 나타내는 반복자)
- 전방 삽입과 임의 삽입
    - 전방 삽입을 사용해서 컨테이너에 삽입하면 요소의 순서가 반대로 삽입되지만,
    - 임의 삽입을 사용해서 컨테이너의 앞에 삽입하면 요소의 순서대로 삽입된다. 

- 동작 원리
    - C* container
    - explicit back_insert_iterator(C& c) : container(std::addressof(c)){}   // c.operator&()  &c 대신 addressof() 사용하면 좋다.  C++표준 함수
        - explicit 로 생성자 만들어 변환을 막는다. 
    - *p = 30; // (p.operator*() ).operator=(30)
        - eback_insert_iterator& operator*(){ return *this;}
        - eback_insert_iterator& operator=(const typename C::value_type& a){ container->push_back(a); return *this;}
    - copy(x,x+2,p); // ++p 로 이동하므로 ++이 있어야함.
        - eback_insert_iterator& operator++(){ return *this;}      // 전형  ++p
        - eback_insert_iterator& operator++(int){ return *this;}   // 후형  p++
    - C++11일때는 move도 만들어야 한다. 
        - eback_insert_iterator& operator=(const typename C::value_type&& a){ container->push_back(move(a)); return *this;}
    - g++에서는 copy를 만들때 내부적으로 아래 것을 사용한다.
        - using iterator_category = output_iterator_tag;
        - using value_type = void;
        - using pointer = void;
        - using reference = void;
        - using differnce_type = void;
        - using container_type = C; // **삽입 반복자는 container_type을 가져야 한다.**

- 정리
    - 모든 반복자는 *로 요소에 접근 가능하고 ++ 로 이동가능해야 한다.   no-op
    - =연산 : push_back , push_front , insert
    
## 3-5. 스트림 반복자 (stream iterator)
- ostream_iterator<int> p(cout, ", ");  *p = 20;  // cout << 20 << ", "
- copy(begin(s) , end(s) , p);
- fill_n(p , 3, 0);  // 0, 0, 0,
- 스트림 반복자
    - ostream_iterator : basic_ostream
    - ostreambuf_iterator : basic_ostreambuf    CharT 출력
    - istream_iterator : basic_istream  
    - istreambuf_iterator : basic_istreambuf  CharT 입력
- ostream_iterator 
    - * , ++ 제공해야 햔다.  대신 ++p 는 no-op
    - ofstream f("a.txt);   ostream_iterator<int> p(f,",");  *p=30;  // file로 출력이 되는 것이다.
    - 구현
        - template<typename T , typename CharT=char , typename Traits = char_traits<CharT>> class ostream_iterator
        - using iterator_category = output_iterator_tag;  value_type , pointer,reference , difference_type (일반적)
        - using char_type = CharT;
        - using traits_type = Traits;
        - using ostream_type = basic_ostream<CharT,Traits>;
        
- ostreambuf_iterator
    - ostreambuf_iterator<char> p(cout);  *p = 65;  // buf는 char만 가능함  'A'
    - 내부 동작
        - rdbuf() : stream 객체가 사용하는 streambuf의 포인터를 변환하는 stream 멤버함수
        - basic_streambuf<>::sputc() :  streambuf에 한 문자를 출력하는 함수
- istream_iterator
    - sgetc() 'x' , snextc() '\t' , snextc() 'y'
    - 디폴트 생성자는 end of stream을 나타낸다.
    - istream_iterator<char> p1(f) , p2;   ostream_iterator<char> p3;
        - copy(p1,p2,p3);  // 공백이 모두 무시됨.
    - istreambuf_iterator<char> p1(f) , p2;   ostream_iterator<char> p3;
        - copy(p1,p2,p3);  // 공백까지 모두 출력됨.

## 3-6. 반복자 어답터 (iterator adapter)
- iterator adapter
    - reverse_iterator : 거꾸로
        - auto p1=begin(s);  auto p2=end(s);
        - reverse_iterator< list<int>::iterator > p3(p2);  // p3,p4는 독립적인 객체로 움직이는 것이다. 
        - reverse_iterator< list<int>::iterator > p4(p1);
        - ++p3;  // --p2처럼
        - find(p3,p4,3);
    - 구현
        - template <typename ITER> class reverse_iterator{ ITER iter;  reverse_iterator(ITER i){ iter = i; --iter; }
        - typename ITER::value_type operator*(){ return *iter; }
        - 그외는 일반 iterator와 같이...
    - GoF의 adapter 패턴이다. 기존 것의 동작을 바꿔주는 것이다.
- reverse_iterator를 만든느 4가지 방법
    1. 기존 반복자를 가지고 생성 (adapter) : reverse_iterator<> 클래스 사용
    1. make_reverse_iterator<>  함수 사용
    1. 컨테이너에서 직접 꺼내기 : .rbegin() , .rend() 멤버 함수 
    1. rbegin(),rend()  일반 함수  : find (rbegin(s) , rend(s) , 3)
- iterator 4가지 종류
    - list<int>::iterator p1 = begin(s);
    - list<int>::reverse_iterator p2 = rbegin(s);
    - list<int>::const_iterator p3 = cbegin(s);
    - list<int>::const_reverse_iterator p4 = crbegin(s);
- <forward_list>  : single list
    - 이때는 reverse 안됨.  rbegin(s2) 안됨.
- move_iterator adapter (C++11)
    - people.h  class People
    - People peo1 = *p1;  // 복사 생성자를 부름 (copy : constructor)
    - 방법 1. move_iterator< vector<People>::interator > p2(p1);   
        - People peo2 = *p2;  // move 생성자
        - People peo3 = move(*p1);  // move 생성자  rvalue 참조를 return
    - 방법 2. auto p3 = make_move_iterator(begin(v));
    - vector<People> v2(begin(v) , end(v));  // 복사 생성자
    - vector<People> v2(make_move_iterator(begin(v)) , make_move_iterator(end(v)) );  // move 생성자
        - C++11 부터 지원됨.
    
## 3-7. 반복자 보조 함수
- iterator 보조 함수 <iterator>
    - next 
        - int* foo()  -> 진짜 pointer를 return하는 foo()에 대해서 ++foo()은 에러 
        - but   p2 = next(foo()); 는 가능
    - prev
    - advance : 반복자를 N만큼 이동
    - distance : 2개 반복차의 차이
    - iter_swap : 2개의 반복자가 가리키는 요소 교환

# 4. 알고리즘 (Algorithm)
## 4-1. STL 알고리즘의 개념.
- <algorithm> <numeric> <memory>
- 알고리즘은 컨테이너를 알지 못한다.  어떤 컨테이너의 반복자인지 알수 없다.
    - remove는 ?을 제거하고 메모리를 당기는 역할을 한다.  return 값은 다음 요소를 받게 된다. p = remove(begin(v) , end(v) , 3)
    - 지우는 것은 일반 algorithm이 아닌 멤버 함수를 사용해야 한다. v.erase(p,end(v));
- 알고리즘보다 멤버함수가 좋은 경우가 있다.
    - v.remove(3);

## 4-2. 알고리즘과 함수
- 함수 객체(function object)
    - a + b  // a.operator+(b)
    - a();   // a.operator()()
    - a(1,2)  // a.operator()(1,2)
- 함수 객체 (function objet , Functor)
    - 최초에는 "() 연산자를 재정의해서 함수 처럼 사용가능한 객체"라는 의미
    - 요즘에는 "() 연산자를 사용해서 함수처럼 호출 가능한 모든 객체" : 함수 포인터 , () 재정의한 클래스 , 멤버 함수 포인터 등...
- 함수 객체의 장점
    - 알고리즘에 전달시 일반 함수는 인라인 치환이 안되니만 , 함수 객체는 인라인 치환 가능
    - 상태를 가질수 있다.
```cpp
#include <iostream>
using namespace std;
// ()를 사용해서 호출하는 것들
// 1. 함수
// 2. 함수 포인터
// 3. ()를 재정의한 클래스
// 4. 람다표현식...
Struct Plus {
    int operator()(int a, int b) const { return a+b; }
};

Plus p;
int n = p(1,2);
```

- #include <functional>   <- 여기 plus 같은 함수들이 template으로 정의되어져있다.
    - plus<int> p; p(1,2);
    - multiplies<int> m;  m(3,4);

- 알고리즘과 함수
    - for_each (begin(v) , end(v) , foo);
    - STL 알고리즘은 함수를 인자로 가지는 경우가 많이 있다.
        - 알고리즘의 활용도가 더욱 높여준다.
        - for_each , transform
        - for_each는 인자가 1개
    - 일반 함수 뿐 아니라, 함수 객체 , 람다 표현식을 사용할수 있다. - ()로 호출가능한 모든 객체를 사용 가능
        - Show s(cout, "\t");  for_each (begin(v),end(v) , s); 와 같이 함수 객체를 넘길수도 있다. Show는 operator()(int a)의 생성자가 있다.
- 람다 표현식
    - for_each 
        - 일반 함수 전달 : for_each (begin(v) , end(v) , foo);
        - 함수 객체 전달 : for_each (begin(v) , end(v) , s);
        - C++11 : 람다표현식 : for_each (begin(v) , end(v) , [](int a){cout << a << endl; }  );
            - []을 lambda introducer 라 부른다.
- transform 
    - 인자로 전달된 컨테이너의 모든요소에 연산을 적용후 다른 컨테이너에 복사하는 알고르즘
        - transform(begin(s1) , end(s1) , begin(s2) , foo) ;  s1 시작 ~ 끝까지는 foo에 인자로 넣어 s2에 return값을 넣는다.
    - 알고리즘이 2개의 구간을 받을때는 첫번째 구간은 양쪽을 받고 , 2번째 구간은 처음만 받는다. (유추가 가능하다고 한다.)
        - transform(begin(s1) , end(s1) , back_inserter(s2) , foo) ;  // s2 container가 empty일때도 죽지 않는다. 
    - 이항 함수 버젼
        - transform(begin(s1) , end(s1) , begin(s2) , begin(s3) , foo) ; // s1의 모든 내용을 s2와 함께 차례로 foo에 보내서 foo(1,2) 을 하고 결과를 s3에 넣는다.
        - transform(begin(s1) , end(s1) , begin(s2) , begin(s3) , multiplies<int>() ) ;
        - transform(begin(s1) , end(s1) , begin(s2) , begin(s3) , [](int a , int b){ return a+b; } ) ;

## 4-3. 알고리즘의 변형
- 조건자 (Predicator)
    - bool 타입을 리턴하는 함수 (함수 객체)
    - find_if(begin(v) , end(v) , 함수);   bool 함수(int a) 가 true를 return하는 첫번째 요소를 찾아라.
    - find_if(begin(v) , end(v) , [](int a){ return a%3 == 0;  } );
- 알고리즘의 4가지 형태
    - 기본 버젼 : remove_if(begin(v) , end(v) , 3);
    - 조건자 버젼 : remove_if(begin(v) , end(v) , [](int a){ return a%3 == 0; });
    - 복사 버젼. remove_copy(begin(v) , end(v) , begin(v2) , 3);   // 리턴 값 p는 v2의 반복자
    - 조건자 복사 버젼 : p = remove_copy_if(begin(v) , end(v) , begin(v2) , [](int a){ return a%3 == 0; });  v2.erase(p,end(v2)); // 3의 배수는 모두 제거 및 뒤의 0을 없앤다. 
- sort  : STL 설계 철학
    - 인자 2개는 default , 인자 3개는 뒤에 함수가 나온다.  뒤에 _if 를 안 붙여도 된다.
    - sort_copy( begin(v1) , end(v1) , begin(v2));  // 이 함수가 존재하는가?  STL에는 존재하지 않는다. 하나로 합쳐봤자 큰 성능 향상이 없다. 
        - remove_copy : copy 후에 remove하는 것보다 동시에 하는 것이 빠르다. (한번에 하면 성능향상이 된다.)
        - copy후에 sort하면 된다.  동시에 해도 성능 향상이 크지 않다.
- std::bind
    - modulus<int> m;   n = m(10,3);  // 10%3 => 1
    - find_if(x,x+5, 함수);   // 이미 함수가 있다면 재사용
        - find_if(x,x+5, m); // error , find_if는 단항함수 인데 modulus는 이항 함수
        - 이항을 단항으로 바꾸면 넘길수 있다.   --> bind
    - void foo(int a, int b , int c , int d)
        - foo(1,2,3,4); // 4항 함수
        - 2항으로 변경     bind(&foo, 10 , _2 , _1 , 20)(1,2)   // _2 뒤의 2번째 인자를 사용
            - // bind의 return값으로 나오는 것이 함수 객체
    - M항 함수 (함수객체)의 인자를 고정한 새로운 함수를 생성
    - C++11
    - <functional>
    - placeholder(_1,_2...)는 std::placeholders 이름 공간에는 있다.
        - // 4gkd gkatn => 3 항으로
        - bind(&foo , _3, _2, _1 , 100)(30,20,10);
        - bind(&foo,1,2,3,4)();
    - modulus<int> m ;  bind(m, 10 , 3)();
    - modulus<int> m ;  bind(m, _1 , 3)(7);
    - modulus<int> m ;  find_if(x, x+5, bind(m, _1 , 3) );

# 5. 컨테이너
## 5-1. STL Container의 특징
- container 종류
    - sequence container
        - 요소가 삽입된 순서대로 놓여있는 컨테이너
        - C++98 : list , vector , deque
        - C++11 : forward_list_array
    - associative container
        - 삽입된 순서와 상관없이 규칙을 가지고 요소를 보관
        - C++98 : tree -> set, multi-set , map , multi-map
        - C++11 : hash -> unordered_map , unordered_multimap
    - container adapter
        - stack , queue , priority_queue

- 공통적 특징
    - 값을 보관한다.
        - 복사본을 보관한다.
    - 멤버 함수 뿐 아니라 "Member Type"이 있다. 
        - ::value_type   
        - ::interator
    - 반복자를 가지고 있다. 
        - s.begin()
        - begin(s) <- it's better
    - 제거와 리턴을 동시에 하지 않는다.
        - s.back()을 하여 마지막을 출력하고 , 제거는 s.pop_back()을 추가적으로 해야 한다.
    - STL 자체는 예외가 거의 없다.
- 단위 전략 디자인  (Policy Base Design) 과 STL
    - STL은 내부적으로 어떻게 메모리 할당을 할까?  new , malloc , 공유 메모리 , pool , system call 등...
    - 사용자가 원하는 방식으로 바꿀수 있게 해준다. template인자로 Allocator를 받는다.
    - Allocator
        - 메모리 할당 정책을 담은 클래스 : allocate() , deallocate()
        - 표준 할당기 typename Allocator = allocator<T>
    - vector<int, MyAlloc> v; 로 자신의 것을 변경할수 있다. 
    - 단위 전략 디자인 
        - 클래스가 사용하는 다양한 정책을 "템플릿 인자"를 통해서 교체할수 있게 하는 디자인 기법
    - string class
        - using string = basic_string<char>; 
        - using wstring = basic_string<wchat_t>;
        - template < typename T , typename Traits = std::char_traits<T> , typename Allocator = std::allocator<T> > 
            - 2번째는 비교 정책 클래스
            - 대체하려면 char_traits 의 모든 member함수를 구현해야 한다.  cppreference.com
            - 다 구현이 힘들면 calss mytraits : public char_traits<char> {} 로 선언하면 된다.  이중에 변경할 것만 만들면 된다.
    
## 5-2. Sequence Container
- list
- vector
    - vector<int> v7(10,0); // 10개의 0으로 초기화
    - vector<int> v8{10,0}; // 2개의 10,0으로 초기화
    - v.push_back(10);  // v.push_front(10) 은 에러
    - v.insert(begin(v) +1 , 30);
    - v.front;
    - v[0]
    - v.assign(x,x+5);  
    - sort(begin(v) , end(v)) ;   // 이것과 같이 대부분은 algorithm에서 처리
    - v[100] = 10 ; // 예외 없이 runtime error
        - v.at(100) = 10;  // 예외 발생  (성능이 떨어짐)
    - 크기
        - v.resize(7); // 10개를 7개로 줄임 ->  v.capacity() 10 이며 , v.size()만 7로
        - v.shrink_to_fit(); // 쓸데 없는 메모리 삭제
        - 
- deque
- forward_list (C++11)
- array (C++11)










