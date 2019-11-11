
#include <iostream>
using namespace std;

template<class InputIt, class OutputIt, class UnaryPredicate>
OutputIt xcopy_if(InputIt first, InputIt last, 
                 OutputIt d_first, UnaryPredicate pred)
{
    while (first != last) {
        if (pred(*first))
            *d_first++ = *first;
        first++;
    }
    return d_first;
}

int main()
{
	int x[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int y[10] = { 0 };
	xcopy_if(x, x + 10, y, [](int a) { return a % 2 == 0; });
	for (auto n : y)
		cout << n << endl;
}
// 출력 결과 : 2, 4, 6, 8, 10, 0,0,0,0,0
