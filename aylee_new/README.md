# Minishell - Built-in Commands Test

컨트롤 cd/
$? //대강
환경변수 확장
파이프 //ㅇㅇ
리다이렉션 //대강
히어독 (히스토리 만들 필요 없음.)
더블 쿼트 "" $를 인식해야함
싱글 쿼트 ''
/, ;는 구현 안 해도 됨.
전역변수 1개만 사용 
//구현해야하는것.

## 컴파일 방법

```bash
make
```

## 실행 방법

```bash
./minishell
```

## 구현된 기능

### Built-in 명령어
1. **echo** - 텍스트 출력 (`-n` 옵션 지원)
2. **cd** - 디렉토리 변경 (인자 없으면 HOME으로 이동)
3. **pwd** - 현재 디렉토리 출력
4. **export** - 환경 변수 설정/출력
5. **unset** - 환경 변수 삭제
6. **env** - 환경 변수 출력 (value가 있는 것만)
7. **exit** - 프로그램 종료

## 사용 예시

```bash
# pwd 테스트
minishell$ pwd
/home/claude

# echo 테스트
minishell$ echo Hello World
Hello World

minishell$ echo -n No newline
No newlineminishell$ 

# 환경 변수 테스트
minishell$ export MYVAR=test123
minishell$ env
...
MYVAR=test123

minishell$ export
declare -x MYVAR="test123"
...

minishell$ unset MYVAR
minishell$ env
(MYVAR가 사라진 것을 확인)

# cd 테스트
minishell$ cd /tmp
minishell$ pwd
/tmp

minishell$ cd
minishell$ pwd
/root

# 종료
minishell$ exit
```

## 주요 구조체

### t_env
환경 변수를 연결 리스트로 관리
```c
typedef struct s_env
{
    char            *key;      // 환경 변수 이름
    char            *value;    // 환경 변수 값 (NULL 가능)
    struct s_env    *next;     // 다음 노드
}   t_env;
```

### t_data
전역 데이터 관리
```c
typedef struct s_data
{
    t_env   *env;          // 환경 변수 리스트
    int     exit_status;   // 마지막 명령어 종료 상태
}   t_data;
```

## 파일 구조

- `minishell.h` - 헤더 파일
- `main.c` - 메인 루프
- `built_in.c` - Built-in 명령어 구현
- `env.c` - 환경 변수 관리 함수
- `utils.c` - 유틸리티 함수
- `Makefile` - 빌드 파일

## 주요 수정 사항

1. **export 수정**: `KEY=VALUE` 형태로 파싱하도록 수정
2. **env 수정**: value가 NULL인 변수는 출력하지 않음
3. **delete_env 수정**: head 포인터를 올바르게 업데이트
4. **메인 루프**: fgets로 입력을 받고 공백으로 split하여 처리

## 참고사항

- 현재 버전은 readline 없이 fgets를 사용합니다
- 실제 readline을 사용하려면 `libreadline-dev` 설치 후 Makefile 수정 필요
- 파이프, 리다이렉션 등은 아직 미구현








//aylee@c1r7s1:~/Downloads/files$ export ec="ho asdf"
aylee@c1r7s1:~/Downloads/files$ ec$ec
asdf
aylee@c1r7s1:~/Downloads/files$ echo asdf
asdf
aylee@c1r7s1:~/Downloads/files$ 

^^^^^^^^^^^^이거 구현되도록 만들기. (파싱부 내용)

exit 문자열
bash: exit: hello: numeric argument required
exit 상태 2

cat << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16