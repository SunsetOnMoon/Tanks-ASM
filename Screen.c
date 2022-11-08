proc Screen.Clear uses es di cx ax
   ;  push  $A000
   ;  pop   es
     mov     ax, cs
     add     ax, 1000h
     mov     es, ax

     mov   al, $00
     mov   cx, 320 * 200
     xor   di, di
     rep   stosb
     ret
endp


proc Screen.MoveTankUp uses cx ax,\
     X, Y, tankNumber
    ;  stdcall Screen.Clear
      mov     ax, cs
      add     ax, 1000h
      mov     es, ax

      mov     dx, [X]
      mov     si, [Y]
      sub     si, 1

      mov      ax, 320
      push     dx
      mul      si
      sub      ax, 320* 8
      pop      dx
      add      ax, dx
      mov      di, ax

      mov     al, $04
      cmp     [es:di], al

      jnz      .TankCollision
      inc      si
       ; jmp      .Move

.TankCollision:
        mov     bx, [tankNumber]
        dec     bx
        mov     al, [Tank.HitColor + bx]
        cmp     [es:di], al
        jnz     .Move
        inc      si

.Move:
      mov     cx, [tankNumber]
      cmp     cx, 1
      jz      .UpFirst
      stdcall Tank.Draw, dx, si, UpOrientation, $90
      jmp     .End
.UpFirst:
      stdcall Tank.Draw, dx, si, UpOrientation, $14
.End:
      ret
endp

proc Screen.MoveTankDown uses cx ax,\
     X, Y, tankNumber
     ;   stdcall Screen.Clear
        mov     ax, cs
        add     ax, 1000h
        mov     es, ax

        mov     dx, [X]
        mov     si, [Y]
        add     si, 1

        mov      ax, 320
        push     dx
        mul      si
        add      ax, 320* 24
        pop      dx
        add      ax, dx
        mov      di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz      .TankCollision
        dec      si
       ; jmp      .Move

.TankCollision:
        mov     bx, [tankNumber]
        dec     bx
        mov     al, [Tank.HitColor + bx]
        cmp     [es:di], al
        jnz     .Move
        dec     si
.Move:
        mov     cx, [tankNumber]
        cmp     cx, 1
        jz      .DownFirst
        stdcall Tank.Draw, dx, si, DownOrientation, $90
        jmp     .End
.DownFirst:
        stdcall Tank.Draw, dx, si, DownOrientation, $14
.End:
      ret
endp

proc Screen.MoveTankLeft uses cx,\
     X, Y, tankNumber
      ;  stdcall Screen.Clear
        mov     ax, cs
        add     ax, 1000h
        mov     es, ax

        mov     dx, [X]
        mov     si, [Y]
        sub     dx, 1

        mov      ax, 320
        push     dx
        mul      si
        add      ax, 320* 10
        pop      dx
        add      ax, dx
        sub      ax, 16
        mov      di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz      .TankCollision
        inc      dx
       ; jmp      .Move

.TankCollision:
        mov     bx, [tankNumber]
        dec     bx
        mov     al, [Tank.HitColor + bx]
        cmp     [es:di], al
        jnz     .Move
        inc      dx
.Move:
        mov     cx, [tankNumber]
        cmp     cx, 1
        jz      .LeftFirst
        stdcall Tank.Draw, dx, si, LeftOrientation, $90
        jmp     .End
.LeftFirst:
        stdcall Tank.Draw, dx, si, LeftOrientation, $14
.End:
      ret
endp

proc Screen.MoveTankRight uses cx,\
     X, Y, tankNumber
     ;   stdcall Screen.Clear
        mov     ax, cs
        add     ax, 1000h
        mov     es, ax

        mov     dx, [X]
        mov     si, [Y]
        add     dx, 1

        mov      ax, 320
        push     dx
        mul      si
        add      ax, 320* 10
        pop      dx
        add      ax, dx
        add      ax, 16
        mov      di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz      .TankCollision
        dec      dx
       ; jmp      .Move

.TankCollision:
        mov     bx, [tankNumber]
        dec     bx
        mov     al, [Tank.HitColor + bx]
        cmp     [es:di], al
        jnz     .Move
        dec      dx
.Move:
        mov     cx, [tankNumber]
        cmp     cx, 1
        jz      .RightFirst
        stdcall Tank.Draw, dx, si, RightOrientation, $90
        jmp     .End
.RightFirst:
        stdcall Tank.Draw, dx, si, RightOrientation, $14
.End:
      ret
endp


proc Screen.SpawnBullet,\
     X, Y, orientation
        mov     dx, [X]
        mov     si, [Y]
        cmp     [orientation], UpOrientation
        jz      .UpBullet
        cmp     [orientation], DownOrientation
        jz      .DownBullet
        cmp     [orientation], LeftOrientation
        jz      .LeftBullet
        cmp     [orientation], RightOrientation
        jz      .RightBullet
.UpBullet:
        sub     si, 12
        sub     dx, 2
        mov     bx, UpOrientation
        jmp     .DrawBullet
.DownBullet:
        add     si, 24
        sub     dx, 2
        mov     bx, DownOrientation
        jmp     .DrawBullet
.LeftBullet:
        add     si, 8
        sub     dx, 20
        mov     bx, LeftOrientation
        jmp     .DrawBullet
.RightBullet:
        add     si, 8
        add     dx, 16
        mov     bx, RightOrientation
.DrawBullet:
        stdcall Tank.Bullet, dx, si
     ret
endp

proc Screen.MoveBullet uses ax, \
     X, Y, orientation, tankNumber
        mov     dx, [X]
        mov     si, [Y]
        dec     [tankNumber]
        cmp     [orientation], UpOrientation
        jz      .UpBulletFly
        cmp     [orientation], DownOrientation
        jz      .DownBulletFly
        cmp     [orientation], LeftOrientation
        jz      .LeftBulletFly
        cmp     [orientation], RightOrientation
        jz      .RightBulletFly

.UpBulletFly:
        sub     si, 4
        ;cmp     si, 4
        mov      ax, 320
        push     dx
        mul      si
        sub      ax, 320
        pop      dx
        add      ax, dx
        mov      di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz     .ColorCheck

        ;jns     .Result
        mov     bx, [tankNumber]
        btr      [Tank.IfBullet], bx
        jmp     .Result
.DownBulletFly:
        add     si, 4
        ;cmp     si, 196
        mov      ax, 320
        push     dx
        mul      si
        add      ax, 320
        pop      dx
        add      ax, dx
        mov      di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz     .ColorCheck
        ;js     .Result
        mov     bx, [tankNumber]
        btr      [Tank.IfBullet], bx
        jmp     .Result
.LeftBulletFly:
        sub     dx, 4
        ;cmp     dx, 4
        mov     ax, 320
        push    dx
        mul     si
        sub     ax, 1
        pop     dx
        add     ax, dx
        mov     di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz     .ColorCheck
        ;jns     .Result
        mov     bx, [tankNumber]
        btr      [Tank.IfBullet], bx
        jmp     .Result
.RightBulletFly:
        add     dx, 4
        ;cmp     dx, 316
        mov     ax, 320
        push    dx
        mul     si
        add     ax, 1
        pop     dx
        add     ax, dx
        mov     di, ax

        mov     al, $04
        cmp     [es:di], al
        jnz     .ColorCheck
        ;js      .Result
        mov     bx, [tankNumber]
        btr      [Tank.IfBullet], bx
        jmp      .Result

.ColorCheck:
        mov     bx, [tankNumber]
        mov     al, [Tank.HitColor + bx]
        cmp     [es:di], al
        jnz     .Result
        dec     [Tank.HeapPoints + bx]
        btr      [Tank.IfBullet], bx
.Result:
        stdcall Tank.Bullet, dx, si
        ret
endp

proc Screen.DrawBoarder uses es di
       ; push    $A000
        ;pop     es
        mov     ax, cs
        add     ax, 1000h
        mov     es, ax

        xor     di, di
        mov     al, $04
        mov     cx, 3
.UpBoard:
        push    cx
        mov     cx, 320
        rep     stosb
        pop     cx
        loop    .UpBoard

        mov     cx, 195
.LeftBoard:
        push    cx
        mov     cx, 3
        rep     stosb
        add     di, 317
        pop     cx
        loop    .LeftBoard

        mov     di, 320 * 3
        add     di, 317
        mov     cx, 194
.RightBoard:
        push    cx
        mov     cx, 3
        rep     stosb
        add     di, 317
        pop     cx
        loop    .RightBoard

        mov     cx, 3
.DownBoard:
        push    cx
        mov     cx, 320
        rep     stosb
        pop     cx
        loop    .DownBoard

        mov     cx, 5
        mov     di, 320 * 40
.Map:
        push    cx
        mov     cx, 40
        rep     stosb
        add     di, 280
        pop     cx
        loop    .Map

;=============Защита для сердечек :D============

;=============Не нужна )))))===================
        ret
endp

proc Screen.LifeLeft uses es di cx dx ax
        movzx   cx, [Tank.HeapPoints]
        mov     dx, 270
.ForTank2:
        stdcall Tank.DrawHeart, $0F, dx, 5
        add     dx,10
        loop    .ForTank2

        movzx   cx, [Tank.HeapPoints + 1]
        mov     dx, 5
.ForTank1:
        stdcall Tank.DrawHeart, $2C, dx, 190
        add     dx,10
        loop    .ForTank1

        ret
endp

