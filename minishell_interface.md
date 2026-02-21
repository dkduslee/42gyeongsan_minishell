# Minishell 파싱부 ↔ 실행부 인터페이스 명세서

> 작성 목적: 파싱부와 실행부 간의 데이터 교환 방식, 구조체 정의, 함수 인터페이스, 에러 처리 규칙을 명확히 공유하기 위함.

---

## 1. 개요

```
사용자 입력
    │
    ▼
[파싱부] ──── t_cmd 리스트 ────▶ [실행부]
                                     │
                                     ▼
                               프로세스 실행 / 결과 반환
```

파싱부는 입력 문자열을 분석하여 `t_cmd` 구조체 연결 리스트를 만들어 실행부에 넘긴다.  
실행부는 이 리스트를 순서대로 실행하며, 실행 후 메모리 해제까지 담당한다.

---

## 2. 구조체 / 데이터 타입 정의

### 2.1 리다이렉션 타입

```c
typedef enum e_redir_type
{
    REDIR_IN,       // <   (입력 리다이렉션)
    REDIR_OUT,      // >   (출력 리다이렉션, 덮어쓰기)
    REDIR_APPEND,   // >>  (출력 리다이렉션, 이어쓰기)
    REDIR_HEREDOC   // <<  (히어독)
    REDIR_PIPE	    // |   (파이프)
} t_redir_type;
```

### 2.2 리다이렉션 노드

```c
typedef struct s_redir
{
    t_redir_type    type;       // 리다이렉션 종류
    char            *file;      // 대상 파일명 or 히어독 delimiter
    struct s_redir  *next;      // 다음 리다이렉션 (같은 명령어 내 복수 가능)
} t_redir;
```

### 2.3 명령어 노드 (핵심 구조체)

```c
typedef struct s_cmd
{
    char            *cmd;     // 명령어
    char	    **argv       // 옵션과 매개들 연결리스트. 예: ["-la", "/tmp", NULL]
    t_redir         *redir;     // 리다이렉션 연결 리스트 (없으면 NULL)
    struct s_cmd    *next;      // 파이프로 연결된 다음 명령어 (없으면 NULL)
} t_cmd;
```

### 2.4 셸 전역 상태

```c
typedef struct s_data
{
	t_env	*env; //env 따온거.
	int		exit_status; //$?에 사용할 거.
}	t_data;
```

---

## 3. 파싱부 → 실행부 전달 규칙

### 3.1 `argv` 배열 규칙

| 조건 | 값 |
|---|---|
| 명령어가 없을 때 (빈 입력) | `NULL` 포인터 반환 |
| 마지막 요소 | 반드시 `NULL` |
| 따옴표 처리 | 파싱부에서 완료 후 전달 (따옴표 문자 제거됨) |          //여기서 있었는지 없었는지 판단을 해서 주기.
| 환경변수 확장 (`$VAR`) | 파싱부에서 완료 후 전달 |
| `$?` 확장 | 파싱부에서 `s_shell.last_exit_status` 값으로 치환 후 전달 |

### 3.2 `redir` 리스트 규칙

- 입력에 나타난 순서대로 연결한다.
- 히어독(`<<`)의 경우 `file` 필드에 **delimiter 문자열**을 저장한다. 히어독 내용(stdin pipe)은 **실행부**에서 처리한다.
- 리다이렉션이 없으면 `redir = NULL`.

### 3.3 파이프 연결 규칙

- `cmd->next`가 `NULL`이면 마지막 명령어.
- 파이프 수 = `t_cmd` 노드 수 - 1.

### 3.4 예시

입력: `cat < infile.txt | grep "hello" >> outfile.txt`

```
t_cmd [0]
  cmd  = ["cat"]
  argv = NULL
  redir = { REDIR_IN, "infile.txt", NULL }
  next  = t_cmd [1]

t_cmd [1]
  cmd  = "grep"
  argv = ["hello", NULL]
  redir = { REDIR_APPEND, "outfile.txt", NULL }
  next  = NULL
```

---

## 4. 함수 인터페이스 명세

### 4.1 파싱부가 실행부에 제공하는 함수

```c
/*
 * 입력 문자열을 파싱하여 t_cmd 연결 리스트를 반환한다.
 *
 * @param input  readline 등으로 받은 원본 입력 문자열
 * @param shell  현재 셸 상태 (환경변수, $? 확장에 사용)
 * @return       성공 시 t_cmd* (파이프라인 첫 번째 노드)
 *               입력이 비어있거나 공백만 있으면 NULL
 *               문법 오류 시 NULL (에러 메시지는 파싱부 내부에서 출력)
 */
t_cmd   *parse(const char *input, t_shell *shell);

/*
 * parse()가 반환한 t_cmd 리스트 전체를 해제한다.
 * 실행부는 실행 완료 후 반드시 이 함수를 호출한다.
 *
 * @param cmd  해제할 리스트의 첫 번째 노드
 */
void    free_cmd_list(t_cmd *cmd);
```

### 4.2 실행부가 파싱부에 제공하는 함수

```c
/*
 * t_cmd 리스트를 실행한다 (파이프라인 포함).
 *
 * @param cmd    파싱 결과 t_cmd 연결 리스트
 * @param shell  셸 상태 (last_exit_status 업데이트됨)
 * @return       마지막 명령어의 종료 상태 (0 = 성공)
 */
int     execute(t_cmd *cmd, t_shell *shell);
```

---

## 5. 파이프 · 리다이렉션 처리 방식

### 5.1 파이프 처리 (실행부 담당)

```
cmd[0] stdout  ──▶  pipe[0]  ──▶  cmd[1] stdin
```

- 파이프 수 = 노드 수 - 1개의 `pipe()`를 생성한다.
- 각 자식 프로세스는 `fork()` 후 적절한 fd를 `dup2()`로 연결한다.
- 부모 프로세스는 모든 파이프 fd를 닫고 `waitpid()`로 대기한다.
- `last_exit_status`는 **마지막 명령어**의 종료 코드로 설정한다.

### 5.2 리다이렉션 처리 (실행부 담당)

| 타입 | 처리 |
|---|---|
| `REDIR_IN` | `open(file, O_RDONLY)` → `dup2(fd, STDIN_FILENO)` |
| `REDIR_OUT` | `open(file, O_WRONLY \| O_CREAT \| O_TRUNC, 0644)` → `dup2(fd, STDOUT_FILENO)` |
| `REDIR_APPEND` | `open(file, O_WRONLY \| O_CREAT \| O_APPEND, 0644)` → `dup2(fd, STDOUT_FILENO)` |
| `REDIR_HEREDOC` | `pipe()` 생성 → 부모가 write 쪽에 내용 입력 → 자식은 read 쪽을 `dup2(fd, STDIN_FILENO)` |

- 리다이렉션은 `redir` 리스트 **순서대로** 적용한다. 같은 방향에 여러 개가 있을 경우 마지막 것이 유효하지만, 앞의 파일들도 **열고 닫아야** 한다 (bash 동작 기준).
- 파일 오픈 실패 시 해당 명령어를 실행하지 않고 에러를 출력한 뒤 `last_exit_status = 1`로 설정한다.

### 5.3 히어독 처리 흐름

```
1. 파싱부: redir->type = REDIR_HEREDOC, redir->file = "delimiter"
2. 실행부: pipe() 생성
3. 부모 프로세스: delimiter를 만날 때까지 readline()으로 읽어 pipe write 쪽에 씀
4. fork() 후 자식: pipe read 쪽을 dup2(STDIN_FILENO), write 쪽 닫음
5. 부모: write 쪽 닫음
```

> **주의:** 히어독 내부에서도 환경변수 확장(`$VAR`)이 발생한다. 단, delimiter가 따옴표로 감싸인 경우(`<<"EOF"`) 확장하지 않는다. 이 판단은 **파싱부**에서 flag로 표시하거나 delimiter 문자열에서 따옴표 제거 여부로 구분한다. *(별도 협의 필요)*

///이 부분은 파싱부가 맡기도 해서 협의 필요. 확장을 미리 해서 보내주면 실행부에서 출력하는 방식. <= 이게 덜 꼬임.

---

## 6. 에러 처리 규칙

### 6.1 사용 가능한 에러 함수

에러 출력은 **`perror`와 `strerror`만** 사용한다.

| 함수 | 특징 | 사용 시점 |
|---|---|---|
| `perror(const char *s)` | `s: strerror(errno)` 형식으로 stderr에 자동 출력 | 시스템 콜 실패 직후 (`errno`가 설정된 상황) |
| `strerror(int errnum)` | errno에 대응하는 에러 문자열 반환 | 직접 포맷을 구성해야 할 때 (`write` + `strerror` 조합) |

### 6.2 에러 출력 형식

```
minishell: [context]: [strerror 메시지]
```

**`perror` 사용 시** — prefix 문자열을 직접 구성해서 넘긴다.

```c
// 예: open() 실패
// 출력: "minishell: infile.txt: No such file or directory"
perror("minishell: infile.txt");

// 일반화
char buf[256];
ft_snprintf(buf, sizeof(buf), "minishell: %s", filename);
perror(buf);
```

**`strerror` 사용 시** — `write(2, ...)`와 조합해서 형식을 맞춘다.

```c
// errno가 설정되지 않는 에러 (문법 오류 등)
// 출력: "minishell: syntax error near unexpected token `|'"
write(2, "minishell: syntax error near unexpected token `|'\n", 50);

// errno 기반이지만 직접 포맷 구성이 필요한 경우
write(2, "minishell: ", 11);
write(2, filename, ft_strlen(filename));
write(2, ": ", 2);
write(2, strerror(errno), ft_strlen(strerror(errno)));
write(2, "\n", 1);
```

> **주의:** `perror`는 호출 즉시 현재 `errno`를 읽는다. 시스템 콜 실패 후 **다른 함수를 호출하기 전에** 바로 사용해야 `errno`가 덮어써지지 않는다.

### 6.3 에러별 처리 방법 정리

| 에러 상황 | 함수 | 예시 출력 |
|---|---|---|
| 파일 열기 실패 (`open`) | `perror` | `minishell: infile.txt: No such file or directory` |
| `fork()` 실패 | `perror` | `minishell: fork: Resource temporarily unavailable` |
| `pipe()` 실패 | `perror` | `minishell: pipe: Too many open files` |
| `malloc()` 실패 | `perror` 또는 `strerror(ENOMEM)` | `minishell: malloc: Cannot allocate memory` |
| 명령어 not found | `strerror` + `write` | `minishell: ls: command not found` (errno 없음 → 직접 문자열) |
| 문법 오류 | `write` 직접 | `minishell: syntax error near unexpected token \`\|'` |
| 권한 없음 (`execve`) | `perror` | `minishell: ./script.sh: Permission denied` |

---

### 6.2 종료 코드 규칙

| 상황 | `last_exit_status` |
|---|---|
| 정상 실행 | 명령어 종료 코드 그대로 |
| 명령어를 찾을 수 없음 | `127` |
| 권한 없음 (Permission denied) | `126` |
| 파일 리다이렉션 실패 | `1` |
| 시그널로 종료 (SIGINT 등) | `128 + signal_number` |
| 문법 오류 | `2` |

### 6.3 에러 처리 책임 분리

| 에러 종류 | 처리 주체 |
|---|---|
| 문법 오류 (따옴표 미닫힘, 잘못된 `\|` 위치 등) | **파싱부** |
| 환경변수 확장 오류 | **파싱부** |
| 파일 열기 실패 | **실행부** |
| 명령어 not found | **실행부** |
| `fork()` / `pipe()` 실패 | **실행부** |
| 빌트인 명령어 오류 | **실행부** |

### 6.4 메모리 해제 책임

- `parse()`가 반환한 `t_cmd` 리스트는 **실행부**가 `free_cmd_list()`로 해제한다.
- `parse()` 내부에서 문법 오류 발생 시 파싱부가 직접 부분 할당된 메모리를 해제하고 `NULL`을 반환한다.
- `shell->envp`는 셸 초기화 시 `main()`에서 할당하며, 종료 시 `main()`에서 해제한다.

---

## 7. 협의 필요 사항 (미결)

- [ 플래그를 추가 ] 히어독 내 따옴표 감싸인 delimiter 처리: 파싱부에서 flag 추가할지, 실행부에서 직접 판단할지
- [ 포인터 ] `t_shell` 구조체를 전역으로 둘지, 포인터로 넘기기. (보통 전역은 시그널로 해둠)
- [ 옵션 에러처리 내용 같음. ] 빌트인 명령어 목록 (`cd`, `echo`, `env`, `exit`, `export`, `unset`, `pwd`) 파싱부에서 특별 처리 필요 여부
- [ 전역 (아마 파싱) ] `SIGINT` / `SIGQUIT` 시그널 핸들러는 어느 쪽에서 등록할지

---

*최종 수정: 협의 후 업데이트 예정*
