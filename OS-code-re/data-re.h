# C언어 헤더파일

#pragma once                    // 중복 포함을 방지하는 전처리기 지시문입니다.

#define videomaxline 25         // 비디오 화면의 최대 라인 수를 25로 정의합니다.
#define videomaxcol 80          // 비디오 화면의 최대 열 수를 80으로 정의합니다.

#define BACKSPACE 0x08          // 백스페이스 키 코드를 0x08로 정의합니다.
#define ENTER 0x13              // 엔터 키 코드를 0x13으로 정의합니다.

#define TRUE 1                  // TRUE를 1로 정의합니다.
#define FALSE 0                 // FALSE를 0으로 정의합니다.

#define DEBUG 24                // 디버그 모드를 위한 상수를 24로 정의합니다.

unsigned char keyboard[videomaxcol]; // 키보드 입력을 저장할 배열을 비디오 최대 열 수로 정의합니다.
unsigned short kindex;              // 키보드 입력의 현재 인덱스를 저장할 변수입니다.

unsigned short curline;             // 쉘의 현재 라인을 저장할 변수입니다.
unsigned short curcol;              // 쉘의 현재 열을 저장할 변수입니다.
