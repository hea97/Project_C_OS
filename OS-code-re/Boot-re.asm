[org 0]          ; 원점 설정, 어셈블러에 시작 주소를 지정합니다.
[bits 16]        ; 16비트 코드 작성.

jmp 0x07C0:start ; 0x07C0 세그먼트의 start 레이블로 점프합니다.

start:
mov ax, cs       ; 코드 세그먼트 레지스터(cs) 값을 ax에 로드합니다.
mov ds, ax       ; 데이터 세그먼트(ds)를 코드 세그먼트(ax)와 동일하게 설정합니다.
mov es, ax       ; 추가 세그먼트(es)를 코드 세그먼트(ax)와 동일하게 설정합니다.

read:
    mov ax, 0x1000    ; ax를 0x1000으로 설정합니다.
    mov es, ax        ; 추가 세그먼트(es)를 0x1000으로 설정합니다.
    mov bx, 0         ; 기본 레지스터(bx)를 0으로 설정하여 es:bx가 0x10000을 가리키게 합니다.

    mov ah, 2         ; ah를 2로 설정하여 섹터 읽기 기능을 지정합니다.
    mov al, 20        ; al을 20으로 설정하여 읽을 섹터 수를 지정합니다 (최대 64).
    mov ch, 0         ; ch를 0으로 설정하여 0번째 실린더를 지정합니다 (최대 1024).
    mov cl, 2         ; cl을 2로 설정하여 2번째 섹터부터 시작합니다.
    mov dh, 0         ; dh를 0으로 설정하여 0번째 헤드를 지정합니다 (최대 16).
    mov dl, 0         ; dl을 0으로 설정하여 플로피 디스크 드라이브를 지정합니다.
    int 13h           ; 디스크 서비스 인터럽트 13h 호출.

    jc read           ; 오류가 발생하면 (캐리 플래그가 설정되면) 다시 읽습니다.

    mov dx, 0x3F2     ; 플로피 디스크 컨트롤러 포트를 dx에 설정합니다.
    xor al, al        ; al을 0으로 클리어하여 플로피 디스크 모터를 끕니다.
    out dx, al        ; 플로피 디스크 컨트롤러 포트로 출력합니다.

    cli               ; 인터럽트를 비활성화합니다.

    ; 인터럽트 설정 코드 시작

    mov al, 0x11      ; PIC 초기화 명령.
    out 0x20, al      ; 마스터 PIC 명령 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).
    out 0xA0, al      ; 슬레이브 PIC 명령 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).

    mov al, 0x20      ; 마스터 PIC의 인터럽트 벡터 오프셋 설정.
    out 0x21, al      ; 마스터 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).
    mov al, 0x28      ; 슬레이브 PIC의 인터럽트 벡터 오프셋 설정.
    out 0xA1, al      ; 슬레이브 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).

    mov al, 0x04      ; 마스터 PIC에 슬레이브 PIC가 IRQ2에 연결되어 있음을 설정.
    out 0x21, al      ; 마스터 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).
    mov al, 0x02      ; 슬레이브 PIC의 카스케이드 식별을 설정.
    out 0xA1, al      ; 슬레이브 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).

    mov al, 0x01      ; PIC를 8086 모드로 설정합니다.
    out 0x21, al      ; 마스터 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).
    out 0xA1, al      ; 슬레이브 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).

    mov al, 0xFF      ; 슬레이브 PIC의 모든 인터럽트를 비활성화합니다.
    out 0xA1, al      ; 슬레이브 PIC 데이터 포트로 전송.
    dw 0x00eb, 0x00eb ; jmp $+2 (두 개의 no-op 지연 명령어).
    mov al, 0xFB      ; 마스터 PIC의 IRQ2를 제외한 모든 인터럽트를 비활성화합니다.
    out 0x21, al      ; 마스터 PIC 데이터 포트로 전송.

    ; 인터럽트 설정 코드 끝

lgdt [gdtr]      ; 글로벌 디스크립터 테이블 레지스터를 gdtr의 주소로 로드합니다.

mov eax, cr0     ; 제어 레지스터 0(cr0)의 현재 값을 eax에 이동합니다.
or eax, 1        ; cr0의 PE(프로텍션 활성화) 비트를 설정하여 보호 모드를 활성화합니다.
mov cr0, eax     ; 수정된 값을 다시 cr0에 이동하여 보호 모드를 활성화합니다.

jmp $+2          ; 짧은 지연 (두 개의 no-op 명령어).
nop
nop

mov bx, 0x10     ; bx를 데이터 세그먼트 선택자로 설정합니다.
mov ds, bx       ; 데이터 세그먼트(ds)를 데이터 세그먼트 선택자로 설정합니다.
mov es, bx       ; 추가 세그먼트(es)를 데이터 세그먼트 선택자로 설정합니다.
mov fs, bx       ; fs 세그먼트를 데이터 세그먼트 선택자로 설정합니다.
mov gs, bx       ; gs 세그먼트를 데이터 세그먼트 선택자로 설정합니다.
mov ss, bx       ; 스택 세그먼트(ss)를 데이터 세그먼트 선택자로 설정합니다.

jmp dword CodeSegment:0x10000 ; 코드 세그먼트 선택자의 0x10000 오프셋으로 원거리 점프합니다.

gdtr:
dw gdt_end - gdt - 1 ; GDT의 한계(GDT의 크기 - 1).
dd gdt+0x7C00        ; GDT의 기본 주소(현재 코드 세그먼트에서 오프셋).

gdt:
    dd 0, 0           ; Null 디스크립터.
CodeSegment equ 0x08
    dd 0x0000FFFF, 0x00CF9A00 ; 코드 세그먼트 디스크립터 (기본 0x0000, 한계 0xFFFF, 접근 바이트 0x9A, 플래그 0xCF).
DataSegment equ 0x10
    dd 0x0000FFFF, 0x00CF9200 ; 데이터 세그먼트 디스크립터 (기본 0x0000, 한계 0xFFFF, 접근 바이트 0x92, 플래그 0xCF).
VideoSegment equ 0x18
    dd 0x8000FFFF, 0x0040920B ; 비디오 세그먼트 디스크립터 (기본 0x8000, 한계 0xFFFF, 접근 바이트 0x92, 플래그 0x40).

gdt_end:

times 510-($-$$) db 0 ; 부트 섹터의 나머지 부분을 510바이트까지 0으로 채웁니다.
dw 0xAA55             ; 부트 섹터 서명.
