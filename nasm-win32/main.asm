global _main
    extern  _GetStdHandle@4
    extern  _WriteFile@20
    extern  _ReadFile@20
    extern  _ExitProcess@4
    extern  _HeapCreate@12
    extern  _HeapAlloc@12
    extern  _CreateFileA@28
    extern  _CloseHandle@4
    extern  _GetFileSize@8
    extern  _HeapFree@12

section .data
    hStdOut: dd 0
    hStdIn: dd 0
    hHeap: dd 0
    program: dd 0
    programLength: dd 0
    hFile: dd 0
    filename: db "./program.bf",0
    inputString: dd 0
    inputPointer: dd 0

    programPointer: dd 0
    memory: times 4096 db 0 ; Max memory
    memory_end:
    memoryPointer: dd 2048  ; Initial memory pointer position
    tmpInput: db 0
    nestedLoopCount: dd 0

; ----------- DEBUGGING ----------------
    ;newLine: db 10,0


section .text
_main:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    mov     eax, [ebp+12] ; argv
    mov     ebx, [eax+4] ; ebx = argv[1]
    mov     dword [inputString], ebx

    ; hStdOut
    push    -11
    call    _GetStdHandle@4
    mov     [hStdOut], eax
    ; hStdIn
    push    -10
    call    _GetStdHandle@4
    mov     [hStdIn], eax
    ; hHeap
    push    0
    push    0
    push    0x00040000
    call    _HeapCreate@12
    mov     [hHeap], eax


    ; CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)
    push    0   ; hTemplateFile
    push    1   ; dwFlagsAndAttributes: 1 = read-only
    push    3   ; dwCreationDisposition: 3 = only open if file exists
    push    0   ; lpSecurityAttributes
    push    1   ; dwShareMode: 1 = file_share_read
    push    31  ; dwDesiredAccess: 31 = GENERIC_READ
    push    dword filename
    call    _CreateFileA@28
    mov     [hFile], eax

    ; GetFileSize(hFile, lpFileSizeHigh)
    push    0
    push    dword [hFile]
    call    _GetFileSize@8
    mov     [programLength], eax



    ; Alloc [programLength] bytes
    push    dword [programLength]
    push    0x00000008
    push    dword [hHeap]
    call    _HeapAlloc@12
    mov     [program], eax

    ; Read [programLength] bytes from file into abc
    push    0
    lea     eax, [ebp-4]
    push    eax
    push    dword [programLength]
    push    dword [program]
    push    dword [hFile]
    call    _ReadFile@20

    ; Close FileHandle
    push    dword [hFile]
    call    _CloseHandle@4


    mov     dword [programPointer], 0 ; program_pointer
programLoop:
    ; Load next char into al
    mov     eax, [program]
    add     eax, [programPointer]
    mov     al, [eax]

    cmp     al, '+'
    je      plus
    cmp     al, '-'
    je      minus
    cmp     al, '<'
    je      left
    cmp     al, '>'
    je      right
    cmp     al, '.'
    je      output
    cmp     al, ','
    je      input
    cmp     al, '['
    je      loopStart
    cmp     al, ']'
    je      loopEnd
    jmp     continue

    plus:
    mov     edx, [memoryPointer]
    mov     al, [memory + edx]
    cmp     al, 255
    je      setzero
    inc     byte [memory + edx]
    jmp     continue
    setzero:
    mov     byte [memory + edx], 0
    jmp     continue
    minus:
    mov     edx, [memoryPointer]
    mov     al, [memory + edx]
    cmp     al, 0
    je      setmax
    dec     byte [memory + edx]
    jmp     continue
    setmax:
    mov     byte [memory + edx], 255
    jmp     continue
    left:
    cmp     dword [memoryPointer], 0
    je      invalidLeftOperation
    dec     dword [memoryPointer]
    jmp continue
    invalidLeftOperation:
    push    4
    call    _ExitProcess@4
    right:
    lea     eax, [memory_end - memory]
    dec     eax
    cmp     dword [memoryPointer], eax
    je      invalidRightOperation
    inc     dword [memoryPointer]
    jmp continue
    invalidRightOperation:
    push    5
    call    _ExitProcess@4
    output:
    ; WriteFile( hstdOut, message, length(message), &bytes, 0);
    push    0
    lea     eax, [ebp-4]
    push    eax
    push    1
    mov     eax, memory
    add     eax, [memoryPointer]
    push    eax
    push    dword [hStdOut]
    call    _WriteFile@20
    jmp     continue
    input:
    mov     ebx, [inputString]
    add     ebx, [inputPointer]
    mov     bl, [ebx]
    cmp     bl, 0
    je      noInputPresent
    mov     ecx, memory
    add     ecx, [memoryPointer]
    mov     byte [ecx], bl
    inc     dword [inputPointer]
    jmp     continue
    noInputPresent:
    push    2
    call    _ExitProcess@4
    loopStart:
        mov     eax, memory
        add     eax, [memoryPointer]
        cmp     byte [eax], 0
        jne     continue
        ; Seek end of loop

        mov     dword [nestedLoopCount], 0
        mov     edx, [programPointer]
        seekForward:
        mov     eax, [program]
        mov     al, [eax + edx]
        cmp     al, '['
        je      seekForwardStartLoop
        cmp     al, ']'
        je      seekForwardEndLoop
        jmp     continueSeekForward
        seekForwardStartLoop:
            inc     dword [nestedLoopCount]
            jmp     continueSeekForward

        seekForwardEndLoop:
            cmp     dword [nestedLoopCount], 1
            je      foundEndLoop
            dec     dword [nestedLoopCount]
            jmp     continueSeekForward
            foundEndLoop:
            mov     [programPointer], edx
            jmp     continue

        continueSeekForward:
            mov     ecx, [programLength]
            sub     ecx, edx
            inc     edx
            loop    seekForward
            push    1
            call    _ExitProcess@4
    loopEnd:
            mov     eax, memory
            add     eax, [memoryPointer]
            cmp     byte [eax], 0
            je     continue
            ; Seek start of loop

            mov     dword [nestedLoopCount], 0
            mov     edx, [programPointer]
            seekBackwards:
            mov     eax, [program]
            mov     al, [eax + edx]
            cmp     al, ']'
            je      seekBackwardsStartLoop
            cmp     al, '['
            je      seekBackwardsEndLoop
            jmp     continueSeekBackwards
            seekBackwardsStartLoop:
                inc     dword [nestedLoopCount]
                jmp     continueSeekBackwards

            seekBackwardsEndLoop:
                cmp     dword [nestedLoopCount], 1
                je      foundStartLoop
                dec     dword [nestedLoopCount]
                jmp     continueSeekBackwards
                foundStartLoop:
                mov     [programPointer], edx
                jmp     continue

            continueSeekBackwards:
                mov     ecx, edx
                dec     edx
                loop    seekBackwards
                push    3
                call    _ExitProcess@4
    jmp     continue
    continue:
    mov     ecx, [programLength]
    sub     ecx, [programPointer]
    inc     dword [programPointer]
    dec     ecx
    jnz     programLoop




    ; HeapFree(hHeap, dwFlags, lpMem)
    push    dword [program]
    push    0
    push    dword [hHeap]
    call    _HeapFree@12

    ; ExitProcess(0)
    push    0
    call    _ExitProcess@4

; ----------- DEBUGGING ----------------

;printMemory:
;    ; Dump brainfuck memory in console
;    mov     edx, 0
;memoryDumpLoop:
;    push    edx
;    ; WriteFile( hstdOut, message, length(message), &bytes, 0);
;    push    0
;    lea     eax, [ebp-4]
;    push    eax
;    push    1
;    ;add     dword [memory + ecx], 48
;    mov     eax, memory
;    add     eax, edx
;    push    eax
;    push    dword [hStdOut]
;    call    _WriteFile@20
;    pop     edx
;    mov     ecx, 19
;    sub     ecx, edx
;    inc     edx
;    loop    memoryDumpLoop
;    call    println
;    ret
;
;println:
;    push    0
;    lea     eax, [ebp-4]
;    push    eax
;    push    1
;    push    newLine
;    push    dword [hStdOut]
;    call    _WriteFile@20
;    ret