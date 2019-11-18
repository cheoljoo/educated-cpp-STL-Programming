// “preprocessor” 라는 실행파일을 만드세요.
// input.txt 파일이 아래와 같을 때
// 
// define MAX 10
// define MIN 5
// n1 = MAX
// n2 = MIN
// 
// preprocessor input.txt result.txt
// 처럼 실행 하면 다음과 같은 result.txt 가 생성되어야 합니다.
// 
// n1 = 10
// n2 = 5
// 
// unordered_map 을 사용해 주세요.
// 과제 제출시 실행파일이 아닌 preprocessor 의 소스를 제출하시면 됩니다.


#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <algorithm>
using namespace std;


int 
main(int argc, char * argv[])
{
    unordered_map<string, string> u;
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    string s;
    while(getline(in,s)) // 파일에서 한줄씩 읽기
    {
        istringstream iss(s);
        string n1,n2,n3;
        iss >> n1 >> n2 >> n3;
        cout << n1 << ":" << n2 << ":" << n3 << endl;
        if(n1 == "define"){
            u[n2] = n3;
        } else {
            out << n1 << " = " << u[n3] << endl;
        }
    }

    //for(auto n : u){
        //cout << "first:" << n.first << " , second:" << n.second << endl;
    //}
}

