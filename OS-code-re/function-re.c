#include "function.h"
#include "data.h"

// str 문자열을 주어진 line(줄)의 col(열) 위치에서 비디오 메모리에 출력하는 함수.
void kprintf(char* str, int line, int col)
{
	char *video = (char*)(0xB8000 + 160 * line + 2 * col); // 비디오 메모리의 시작 주소에 line과 col을 반영하여 위치를 설정.

	for (int i = 0; str[i] != 0; i++) // 문자열 끝(null)까지 반복.
	{
		*video++ = str[i]; // 문자열의 현재 문자를 비디오 메모리에 쓰기.
		*video++ = 0x03;   // 문자 속성을 설정 (배경색과 문자색).
	}

	return;
}

// 주어진 line(줄)의 col(열)부터 줄의 끝까지 비디오 메모리를 지우는 함수.
void kprintf_line_clear(int line, int col)
{
	char *video = (char*)(0xB8000 + 160 * line + 2 * col); // 비디오 메모리의 시작 주소에 line과 col을 반영하여 위치를 설정.
	for (int i = 0; i < 160 - 2 * col; i++) // 주어진 col부터 줄의 끝까지 반복.
	{
		*video++ = 0;   // 문자를 지우기 위해 0을 쓰기.
		*video++ = 0x03; // 속성 설정 (배경색과 문자색).
	}
}

// 전체 비디오 메모리를 지우는 함수.
void kprintf_clear_all()
{
	for (int i = 0; i < videomaxline; i++) // 모든 줄을 반복.
		kprintf_line_clear(i, 0); // 각 줄을 지웁니다.
}

// 두 문자열 str1과 str2를 비교하여 같으면 TRUE를, 다르면 FALSE를 반환하는 함수.
int kstrcmp(char* str1, char* str2)
{
	for (int i = 0; i < kstrlen(str1); i++) // str1의 길이만큼 반복.
		if (str1[i] != str2[i]) return FALSE; // 문자가 다르면 FALSE 반환.
	return TRUE; // 모두 같으면 TRUE 반환.
}

// 문자열 str1의 길이를 반환하는 함수.
int kstrlen(char* str1)
{
	int i = 0;
	while (str1[i] != 0) i++; // null 문자(문자열의 끝)를 만날 때까지 반복.
	return i; // 문자열의 길이 반환.
}
