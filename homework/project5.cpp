// 6-4강에서는 NotificationCenter를 만들었습니다. 동영상 강좌에서 만든 코드는 통보센터에 등록된 순서대로 함수가 호출 됩니다.
// NotificationCenter 코드를 수정해서 함수 등록시 우선순위를 전달할 수 있고, 멀티 스레드가 지원되도록 수정해 보세요.

void foo(void* p, string name)
{
    cout << name << " : " << this_thread::get_id() << endl;
    this_thread::sleep_for(3s);
    cout << name << " end" << endl;
}
int main()
{
    NotificationCenter nc;

    // Register 의 3번째 인자는 우선순위, 4번째 인자는 스레드사용여부 입니다.
    // 우선순위 1일 제일 낮고, 3일 제일 높다가 가정합니다.
    // NEW_THREAD 지정시 해당 함수는 새로운 스레드로 동작해야 합니다.
    nc.Register("TEST_EVENT", bind(&foo, _1, "one"s), 1);
    nc.Register("TEST_EVENT", bind(&foo, _1, "two"s), 2, NEW_THREAD);
    nc.Register("TEST_EVENT", bind(&foo, _1, "three"s), 3);

    nc.Notify("TEST_EVENT", (void*)0); // 등록된 순서가 아닌 우선순위가
                // 높은 순서로 실행되게 해주세요
                // NEW_THREAD 옵션을 가진 경우. 새로운
                // 스레드로 해당 함수를 동작되게 해주세요.
    getchar();
}

