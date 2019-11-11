“preprocessor” 라는 실행파일을 만드세요.
input.txt 파일이 아래와 같을 때

define MAX 10
define MIN 5
n1 = MAX
n2 = MIN

preprocessor input.txt result.txt
처럼 실행 하면 다음과 같은 result.txt 가 생성되어야 합니다.

n1 = 10
n2 = 5

unordered_map 을 사용해 주세요.
과제 제출시 실행파일이 아닌 preprocessor 의 소스를 제출하시면 됩니다.
