CodeSegment equ 0x08        ; 코드 세그먼트 셀렉터를 0x08로 정의.
DataSegment equ 0x10        ; 데이터 세그먼트 셀렉터를 0x10로 정의.
VideoSegment equ 0x18       ; 비디오 세그먼트 셀렉터를 0x18로 정의.

[org 0x10000]               ; 코드의 원점 주소를 0x10000으로 설정.
[bits 32]                   ; 32비트 코드 작성.

START:
    mov bx, DataSegment     ; 데이터 세그먼트 셀렉터 값을 BX에 로드.
    mov ds, bx              ; DS 레지스터에 데이터 세그먼트 셀렉터 값을 설정.
    mov es, bx              ; ES 레지스터에 데이터 세그먼트 셀렉터 값을 설정.
    mov fs, bx              ; FS 레지스터에 데이터 세그먼트 셀렉터 값을 설정.
    mov gs, bx              ; GS 레지스터에 데이터 세그먼트 셀렉터 값을 설정.
    mov ss, bx              ; 스택 세그먼트(SS) 레지스터에 데이터 세그먼트 셀렉터 값을 설정.
    lea esp, [START]        ; ESP 레지스터에 START의 주소를 로드 (스택 포인터 초기화).

    mov edi, 0              ; EDI 레지스터를 0으로 초기화 (비디오 메모리의 시작 주소).
    mov esi, msg            ; ESI 레지스터에 msg의 주소를 로드.
    call printf             ; printf 함수를 호출.

    jmp dword CodeSegment:0x10200 ; 원거리 점프를 통해 코드 세그먼트의 0x10200 주소로 점프.

printf:
    push eax                ; EAX 레지스터의 값을 스택에 저장.
    push es                 ; ES 레지스터의 값을 스택에 저장.
    mov ax, VideoSegment    ; 비디오 세그먼트 셀렉터 값을 AX에 로드.
    mov es, ax              ; ES 레지스터에 비디오 세그먼트 셀렉터 값을 설정.

printf_loop:
    mov al, byte [esi]      ; ESI가 가리키는 메모리 위치의 바이트 값을 AL에 로드.
    mov byte [es:edi], al   ; AL의 값을 비디오 메모리(ES:EDI)에 저장.
    inc edi                 ; EDI를 증가시켜 다음 비디오 메모리 위치로 이동.
    mov byte [es:edi], 0x09 ; 다음 비디오 메모리 위치에 탭 문자를 저장.
    inc esi                 ; ESI를 증가시켜 다음 문자열 문자로 이동.
    inc edi                 ; EDI를 증가시켜 비디오 메모리의 다음 위치로 이동.
    or al, al               ; AL과 AL을 OR 연산하여 0 플래그를 설정.
    jz printf_end           ; AL이 0이면 printf_end로 점프 (문자열 끝).
    jmp printf_loop         ; 다음 문자를 처리하기 위해 printf_loop로 점프.

printf_end:
    pop es                  ; 스택에서 ES 레지스터의 값을 복원.
    pop eax                 ; 스택에서 EAX 레지스터의 값을 복원.
    ret                     ; 호출자에게 복귀.

msg db 'Call printf', 0     ; 출력할 메시지 'Call printf'를 정의하고 null로 종료.

times 512-($-$$) db 0       ; 현재 위치부터 512바이트까지 0으로 채웁니다 (부트 섹터 크기 맞춤).
