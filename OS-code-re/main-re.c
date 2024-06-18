// main.c

// 필요한 헤더 파일을 포함합니다.
#include "function.h"
#include "interrupt.h"
#include "data.h"
#include "shell.h"

// 쉘 함수와 쉘 명령어를 처리하는 함수를 선언합니다.
void shell();
void translate_shell();

// 메인 함수입니다. 프로그램의 진입점입니다.
void main()
{
    // 화면에 "We are now in C!"라는 메시지를 (10, 10) 위치에 출력합니다.
    kprintf("We are now in C!", 10, 10);
    
    // 인터럽트 디스크립터를 초기화합니다.
    init_intdesc();

    // 화면을 모두 지웁니다.
    kprintf_clear_all();
    
    // 쉘을 실행합니다.
    shell();
}

// 쉘 함수입니다. 명령어 입력과 처리 루프를 담당합니다.
void shell()
{
    // 프롬프트 문자열을 설정합니다.
    char path[] = "HoGoS>>";

    // 현재 줄과 컬럼을 초기화합니다.
    curline = 0;
    curcol = kstrlen(path);

    // 무한 루프를 돌면서 사용자 입력을 처리합니다.
    while (1)
    {
        // 인터럽트를 비활성화합니다.
        __asm__ __volatile__("cli");

        // 사용자가 ENTER 키를 눌렀을 때 입력된 명령어를 처리합니다.
        if (kindex != 0 && keyboard[kindex - 1] == ENTER)
        {
            // 현재 줄의 프롬프트 뒤에 있는 입력 문자열을 지웁니다.
            kprintf_line_clear(curline, curcol + kindex - 1);
            
            // 입력 버퍼에서 ENTER 키를 지웁니다.
            keyboard[kindex - 1] = 0;

            // 입력된 명령어를 처리합니다.
            translate_shell();

            // 비디오 메모리의 최대 열 수만큼 반복하여 키보드 입력을 초기화합니다.
            for (int i = 0; i < videomaxcol; i++)
                keyboard[i] = 0;

            // 다음 줄로 이동합니다.
            curline++;
            kindex = 0;
        }

        // 프롬프트를 현재 줄과 컬럼에 출력합니다.
        kprintf(path, curline, 0);

        // 현재 줄의 프롬프트 뒤의 입력 문자열을 지웁니다.
        kprintf_line_clear(curline, curcol + kindex);

        // 현재 줄의 프롬프트 뒤에 입력된 문자열을 출력합니다.
        kprintf(keyboard, curline, curcol);

        // 인터럽트를 활성화합니다.
        __asm__ __volatile__("sti");
    }
}

// 쉘 명령어를 처리하는 함수입니다.
void translate_shell()
{
    // 입력 버퍼가 비어있으면 아무 작업도 하지 않고 반환합니다.
    if (keyboard[0] == 0) { return; }

    // 'clear' 명령어가 입력되었을 때 화면을 지웁니다.
    if (kstrcmp(keyboard, "clear")) { sh_clear(); return; }

    // 'version' 명령어가 입력되었을 때 버전 정보를 출력합니다.
    if (kstrcmp(keyboard, "version")) { sh_version(); return; }

    // 정의되지 않은 명령어일 경우 오류 메시지를 출력합니다.
    kprintf("There is no such command.", ++curline, 0);
}
