#include <iostream>
#include <string>
#include <tuple>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::placeholders;


#define NEW_THREAD 1

using HANDLER = function<void(void*)>;

bool sortByPriority(const tuple<int, int, HANDLER>& a,
               const tuple<int, int, HANDLER>& b)
{
    return (get<0>(a) > get<0>(b));
}

void foo(void* p, string name)
{
    cout << name << " : " << this_thread::get_id() << endl;
    this_thread::sleep_for(3s);
    cout << name << " end" << endl;
}

class NotificationCenter
{
    map<string, vector<tuple<int,int,HANDLER>>> notif_map;
public:
    void Register(string key, HANDLER h,int priority,int thd=0)
    {
        notif_map[key].push_back(make_tuple(priority,thd,h));
    }
    void Notify( string key, void* param)
    {
        vector<tuple<int,int,HANDLER>>& v = notif_map[key];

        for( tuple<int,int,HANDLER> f : v){
            cout << "priority:" << get<0>(f) <<  " , thread:" << get<1>(f) << endl;
            if(get<1>(f) == NEW_THREAD){
                thread t(get<2>(f),param);
                        //thread t(&foo,param,"PPP");
                t.join();
            } else {
                get<2>(f)(param);
            }
        }
    }
    void Sort(string key)
    {
        vector<tuple<int,int,HANDLER>>& v = notif_map[key];
        sort(v.begin(),v.end(),sortByPriority);
    }
};

int main()
{
    NotificationCenter nc;

    // Register 의 3번째 인자는 우선순위, 4번째 인자는 스레드사용여부 입니다.
    // 우선순위 1일 제일 낮고, 3일 제일 높다가 가정합니다.
    // NEW_THREAD 지정시 해당 함수는 새로운 스레드로 동작해야 합니다.
    nc.Register("TEST_EVENT", bind(&foo, _1, "one"s), 1);
    nc.Register("TEST_EVENT", bind(&foo, _1, "two"s), 2, NEW_THREAD);
    nc.Register("TEST_EVENT", bind(&foo, _1, "three"s), 3);

    nc.Sort("TEST_EVENT");
    nc.Notify("TEST_EVENT", (void*)0); // 등록된 순서가 아닌 우선순위가
                // 높은 순서로 실행되게 해주세요
                // NEW_THREAD 옵션을 가진 경우. 새로운
                // 스레드로 해당 함수를 동작되게 해주세요.
    getchar();
}

