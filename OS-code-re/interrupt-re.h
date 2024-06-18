#pragma once

// 함수 프로토타입 선언
void init_intdesc(); // 인터럽트 디스크립터 초기화 함수
void idt_ignore(); // 기본 인터럽트 핸들러 함수
void idt_timer(); // 타이머 인터럽트 핸들러 함수
void idt_keyboard(); // 키보드 인터럽트 핸들러 함수
unsigned char transScan(unsigned char); // 스캔 코드를 ASCII 문자로 변환하는 함수

// IDT (Interrupt Descriptor Table) 구조체 정의
struct IDT {
    unsigned short offsetl; // ISR의 하위 16비트 오프셋
    unsigned short selector; // 코드 세그먼트 셀렉터
    unsigned short type; // 디스크립터 타입 및 속성
    unsigned short offseth; // ISR의 상위 16비트 오프셋
} __attribute__((packed)); // 패킹된 구조체 (구조체 멤버 사이에 패딩 없음)

// IDTR (Interrupt Descriptor Table Register) 구조체 정의
struct IDTR {
    unsigned short size; // IDT 크기
    unsigned int addr; // IDT 시작 주소
} __attribute__((packed)); // 패킹된 구조체 (구조체 멤버 사이에 패딩 없음)
