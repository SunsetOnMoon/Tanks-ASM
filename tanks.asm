       include 'MACRO\proc16.inc'

        include 'Units\Keyboard.h'
        include 'Units\Screen.h'
        include 'Units\Tank.h'

        org 100h

nil     =       -1

EntryPoint:
        mov     ax, $0013
        int     10h
       ; push    $A000
       ; pop     es

Start:
        mov     si, [Y1]
        mov     dx, [X1]
        mov     [prevOrient1], UpOrientation
        stdcall Tank.Draw, dx, si, UpOrientation, $14

        mov     si, [Y2]
        mov     dx, [X2]
        mov     [prevOrient2], DownOrientation
        stdcall Tank.Draw, dx, si, DownOrientation, $90


        stdcall SetIntVector, 09h, cs, KeyBoardISR
        mov      word [OldKeyboardISR], ax
        mov      word[OldKeyboardISR + 2], dx
        stdcall SetIntVector, 08h, cs, TimerISR
        mov      word [OldTimerISR], ax
        mov      word[OldTimerISR + 2], dx
.Loop:

.Key:
        mov     ax, cs
        add     ax, 1000h
        mov     es, ax

        bt      [Screen.UpdateState], 0
        jnc     .Key
;===========Обновление танка==============================
        stdcall Screen.Clear
        stdcall Screen.DrawBoarder

        mov     si, [Y1]
        mov     dx, [X1]
        stdcall Tank.Draw, dx, si, [prevOrient1], $14


        mov     si, [Y2]
        mov     dx, [X2]
        stdcall Tank.Draw, dx, si, [prevOrient2], $90
;==========================================================

;============Обновление пуль===============================
        ;mov     ax, [$046C]
        ;mov     [OldTime], ax
        ;mov     [StartTime], ax
        ;cmp     [OldTime], 5
        ;jns     .Checker

        bt      [Tank.IfBullet], 0
        jnc     .NextBull
        mov     si, [bull1 + 2]
        mov     dx, [bull1]
        stdcall Screen.MoveBullet, dx, si, [bullOrient1], 1
        mov     [bull1 + 2], si
        mov     [bull1], dx

.NextBull:
        bt      [Tank.IfBullet], 1
        jnc     .Checker
        mov     si, [bull2 + 2]
        mov     dx, [bull2]
        stdcall Screen.MoveBullet, dx, si, [bullOrient2], 2
        mov     [bull2 + 2], si
        mov     [bull2], dx

;==========================================================
.Checker:
        bt      [KeyBoard.KeyData], 11
        jc      .End

        bt      [KeyBoard.KeyData], 8
        jnc     .IfNextBull
        bt      [Tank.IfBullet], 0
        jc      .IfNextBull
        mov     dx, [X1]
        mov     si, [Y1]
        stdcall Screen.SpawnBullet,dx, si, [prevOrient1]
        mov     [bullOrient1], bx
        mov     [bull1], dx
        mov     [bull1 + 2], si
        bts     [Tank.IfBullet], 0
.IfNextBull:
        bt      [KeyBoard.KeyData], 10
        jnc     .After
        bt      [Tank.IfBullet], 1
        jc      .After
        mov     dx, [X2]
        mov     si, [Y2]
        stdcall Screen.SpawnBullet,dx, si, [prevOrient2]
        mov     [bullOrient2], bx
        mov     [bull2], dx
        mov     [bull2 + 2], si
        bts     [Tank.IfBullet], 1
.After:
        mov     di, 0
.Cycle:
        push    di
        bt      [KeyBoard.KeyData], di
        jnc      .EndCycle
       ; mov     bx, cx
        add     di, di
        mov     ax, [KeyBoard.TankRedraw + di]
        cmp     ax, nil
        jz      .EndCycle
        cmp     di, 2 * 4
        jns     .Tank2
        mov     si, [Y1]
        mov     dx, [X1]
        stdcall ax, dx, si, 1
        mov     [X1], dx
        mov     [Y1], si
        mov     [prevOrient1], bx
        jmp     .EndCycle
.Tank2:
        mov     si, [Y2]
        mov     dx, [X2]
        stdcall ax, dx, si, 2
        mov     [X2], dx
        mov     [Y2], si
        mov     [prevOrient2], bx
.EndCycle:
        pop     di
        inc     di
        cmp     di, 8
        jnz     .Cycle
        stdcall Screen.LifeLeft

.IfWin:
        cmp     [Tank.HeapPoints], 0
        jz      .End
        cmp     [Tank.HeapPoints + 1],0
        jz      .End
.ReadFromBuffer:
        push    es
        pop     ds
        push    $A000
        pop     es
        mov     cx, 32000

        xor     di, di
        xor     si, si
        rep     movsw

        push    cs
        pop     ds

        jmp     .Key
.End:
        stdcall   SetIntVector, $09, word[OldKeyboardISR + 2], word[OldKeyboardISR]
        stdcall   SetIntVector, $08, word[OldTimerISR + 2], word[OldTimerISR]
        mov     ah, 4Ch
        int     21h



KeyBoardISR:
        push    ds ax bx cx
        push    cs
        pop     ds

        in      al, 60h
        movzx   bx, al
        movzx   cx, [KeyBoard.ScanCodes.Push + bx]

        in      al, 61h
        mov     ah, al
        or      al, 80h
        out     61h, al
        xchg    ah, al
        out     61h, al

        mov     al, 20h
        out     20h, al

        cmp     cx, nil
        jz     .EndISR
        test    bl, $80
        jz      .SetKey

.ResetKey:
        btr     [KeyBoard.KeyData], cx
        jmp     .EndISR
.SetKey:
       ; mov     [KeyBoard.KeyData], 0
       ; mov     [KeyBoard.KeyData + 2], 0
        bts     [KeyBoard.KeyData], cx

.EndISR:
        pop     cx bx ax ds
        iret

TimerISR:
        push    ds ax bx cx
        push    cs
        pop     ds



        dec     [Timer.Counter]
        jnz     .NoUpdate

        bts     [Screen.UpdateState], 0
        mov     [Timer.Counter],3
        jmp     .EndISR

.NoUpdate:
        btr     [Screen.UpdateState], 0
.EndISR:
        mov     al, 20h
        out     20h, al
        pop     cx bx ax ds
        iret

proc SetIntVector uses es bx,\
     bIntNumber:BYTE, segISR, ofsISR

     xor        ax, ax
     mov        es, ax

     mov        dx, [segISR]
     mov        ax, [ofsISR]

     movzx      bx, [bIntNumber]
     shl        bx, 2

     pushf
     cli
     xchg       [es:bx], ax
     xchg       [es:bx + 2], dx
     popf
    ; sti
     ret
endp



        include 'Units\Tank.c'
        include 'Units\Screen.c'

        include 'Units\Keyboard.du'

        include 'Units\Keyboard.di'
        include 'Units\Screen.di'
        include 'Units\Tank.di'


X1       dw      50
Y1       dw      150
prevOrient1 dw   ?
bull1    dw      50, 50
bullOrient1 dw   ?
tankHeapPoints1  db     3

X2       dw      200
Y2       dw      50
prevOrient2 dw   ?
bull2    dw      0, 0
bullOrient2 dw   ?
tankHeapPoints2  db     3

OldKeyboardISR  dd      ?
OldTimerISR     dd      ?

Timer.Counter   dw      3

OldTime         dw      ?
NowTime         dw      ?
StartTime       dw      ?
