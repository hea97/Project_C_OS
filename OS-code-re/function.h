#pragma once // 이 파일의 내용이 한 번만 포함되도록 하는 전처리기 지시문입니다.

// 함수 선언
void kprintf(char*, int, int); // str : 출력할 문자열의 주소, int : 몇 번째 줄에 출력할 것인지, int : 몇 번째 열에 출력할 것인지
void kprintf_line_clear(int, int); // 주어진 줄(line)과 열(column)부터 그 줄의 끝까지 지우는 함수
void kprintf_clear_all(); // 화면 전체를 지우는 함수

int kstrcmp(char*, char*); // 두 문자열을 비교하는 함수, 같으면 TRUE, 다르면 FALSE 반환
int kstrlen(char*); // 문자열의 길이를 반환하는 함수
