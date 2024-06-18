#include "interrupt.h"
#include "function.h"
#include "data.h"

// 인터럽트 디스크립터 테이블 (IDT) 구조체와 IDTR 구조체 선언
struct IDT inttable[3]; // 3개의 인터럽트 디스크립터를 위한 배열
struct IDTR idtr = { 256 * 8 - 1, 0 }; // IDT 레지스터 구조체

unsigned char keyt[2] = { 'A', 0 }; // 키 입력 버퍼 초기화
unsigned char keybuf; // 키 입력 버퍼

// 인터럽트 디스크립터 초기화 함수
void init_intdesc() {
    int i;
    unsigned int ptr;
    unsigned short *isr;

    // 0x00: isr_ignore
    {
        ptr = (unsigned int)idt_ignore;
        inttable[0].selector = (unsigned short)0x08;
        inttable[0].type = (unsigned short)0x8E00;
        inttable[0].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[0].offseth = (unsigned short)(ptr >> 16);
    }

    // 0x01: isr_timer
    {
        ptr = (unsigned int)idt_timer;
        inttable[1].selector = (unsigned short)0x08;
        inttable[1].type = (unsigned short)0x8E00;
        inttable[1].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[1].offseth = (unsigned short)(ptr >> 16);
    }

    // 0x02: isr_keyboard
    {
        ptr = (unsigned int)idt_keyboard;
        inttable[2].selector = (unsigned short)0x08;
        inttable[2].type = (unsigned short)0x8E00;
        inttable[2].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[2].offseth = (unsigned short)(ptr >> 16);
    }

    // 모든 ISR을 기본 ignore 핸들러로 초기화
    for (i = 0; i < 256; i++) {
        isr = (unsigned short*)(0x0 + i * 8);
        *isr = inttable[0].offsetl;
        *(isr + 1) = inttable[0].selector;
        *(isr + 2) = inttable[0].type;
        *(isr + 3) = inttable[0].offseth;
    }

    // 타이머 ISR 등록
    {
        isr = (unsigned short*)(0x0 + 8 * 0x20);
        *isr = inttable[1].offsetl;
        *(isr + 1) = inttable[1].selector;
        *(isr + 2) = inttable[1].type;
        *(isr + 3) = inttable[1].offseth;
    }

    // 키보드 ISR 등록
    {
        isr = (unsigned short*)(0x0 + 8 * 0x21);
        *isr = inttable[2].offsetl;
        *(isr + 1) = inttable[2].selector;
        *(isr + 2) = inttable[2].type;
        *(isr + 3) = inttable[2].offseth;

        kindex = 0;
        for (int i = 0; i < videomaxcol; i++)
            keyboard[i] = 0;
    }

    // 키보드 활성화
    __asm__ __volatile__ (
        "mov al, 0xAE;"
        "out 0x64, al;"
    );

    // 인터럽트 시작
    __asm__ __volatile__("mov eax, %0"::"r"(&idtr));
    __asm__ __volatile__("lidt [eax]");
    __asm__ __volatile__("mov al,0xFC");
    __asm__ __volatile__("out 0x21,al");
    __asm__ __volatile__("sti");

    return;
}

// 기본 인터럽트 핸들러 (무시)
void idt_ignore() {
    __asm__ __volatile__ (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "mov al, 0x20;"
        "out 0x20, al;"
    );

    kprintf("idt_ignore", 5, 40);

    __asm__ __volatile__ (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}

// 타이머 인터럽트 핸들러
void idt_timer() {
    __asm__ __volatile__ (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "mov al, 0x20;"
        "out 0x20, al;"
    );

    kprintf(keyt, videomaxline-1, videomaxcol-1);
    keyt[0]++;

    __asm__ __volatile__ (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}

// 키보드 인터럽트 핸들러
void idt_keyboard() {
    __asm__ __volatile__ (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "xor al, al;"
        "in al, 0x60;"
    );

    __asm__ __volatile__("mov %0, al;" :"=r"(keybuf));

    keybuf = transScan(keybuf);

    if (keybuf == BACKSPACE && kindex != 0) // 백스페이스 입력
        keyboard[--kindex] = 0;
    else if (keybuf != 0xFF && keybuf != BACKSPACE)
        keyboard[kindex++] = keybuf;

    __asm__ __volatile__ (
        "mov al, 0x20;"
        "out 0x20, al;"
    );

    __asm__ __volatile__ (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}

// 스캔 코드를 ASCII 문자로 변환하는 함수
unsigned char transScan(unsigned char target) {
    unsigned char result;

    switch (target) { // 스캔 코드 세트 1 기준
        case 0x1E: result = 'a'; break;
        case 0x30: result = 'b'; break;
        case 0x2E: result = 'c'; break;
        case 0x20: result = 'd'; break;
        case 0x12: result = 'e'; break;
        case 0x21: result = 'f'; break;
        case 0x22: result = 'g'; break;
        case 0x23: result = 'h'; break;
        case 0x17: result = 'i'; break;
        case 0x24: result = 'j'; break;
        case 0x25: result = 'k'; break;
        case 0x26: result = 'l'; break;
        case 0x32: result = 'm'; break;
        case 0x31: result = 'n'; break;
        case 0x18: result = 'o'; break;
        case 0x19: result = 'p'; break;
        case 0x10: result = 'q'; break;
        case 0x13: result = 'r'; break;
        case 0x1F: result = 's'; break;
        case 0x14: result = 't'; break;
        case 0x16: result = 'u'; break;
        case 0x2F: result = 'v'; break;
        case 0x11: result = 'w'; break;
        case 0x2D: result = 'x'; break;
        case 0x15: result = 'y'; break;
        case 0x2C: result = 'z'; break;
        case 0x39: result = ' '; break; // 스페이스
        case 0x0E: result = BACKSPACE; break; // 백스페이스 아스키 코드 = 8
        case 0x1C: result = ENTER; break; // Enter 키
        default: result = 0xFF; break; // 구현되지 않은 것은 무시, 구분자는 0xFF
    }

    return result;
}
