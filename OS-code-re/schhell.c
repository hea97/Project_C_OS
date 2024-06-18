// sh_clear와 sh_version 함수 정의를 위한 필요한 헤더 파일을 포함합니다.
#include "data.h"
#include "shell.h"
#include "function.h"

// 화면을 지우는 함수입니다.
void sh_clear()
{
    // 화면의 모든 내용을 지웁니다.
    kprintf_clear_all();
    
    // 현재 줄을 초기화합니다. -1로 설정하여 다음 명령어가 첫 번째 줄에 출력되도록 합니다.
    curline = -1;
}

// 버전 정보를 출력하는 함수입니다.
void sh_version()
{
    // 새로운 줄로 이동하여 버전 정보를 출력합니다.
    kprintf("HoGoS [version 0.0.1]", ++curline, 0);

    // 그 다음 줄에 저작권 정보를 출력합니다.
    kprintf("Copyright <c> 2018 HoGuSoft. All rights reserved", ++curline, 0);
}
