
proc Tank.Draw uses di es si dx cx ax,\
     X, Y, orientation, tankColor: BYTE
  ;   push  $A000
  ;   pop   es
     mov     ax, cs
     add     ax, 1000h
     mov     es, ax

     mov        ax, 320
     mul        [Y]
     add        ax, [X]
     mov        di, ax

     mov   al, [tankColor]

     cmp   [orientation], UpOrientation
     je    .UpTank
     cmp   [orientation], DownOrientation
     je    .DownTank
     cmp   [orientation], LeftOrientation
     je    .LeftTank
     cmp   [orientation], RightOrientation
     je    .RightTank
;=======================================================
.UpTank:
     mov   cx, 16
     sub   di, 10
.UpBody:
     push  cx
     mov   cx, 20
     rep   stosb
     add   di, 300
     pop   cx
     loop  .UpBody

     sub   di, 320 * 24
     add   di, 8
     mov   cx, 8
.UpMuzzle:
     push  cx
     mov   cx, 4
     rep   stosb
     add   di, 316
     pop   cx
     loop  .UpMuzzle

     mov   bx, UpOrientation

     jmp   .End
;=========================================================
.DownTank:
     sub   di, 10
     mov   cx, 16
.DownBody:
     push  cx
     mov   cx, 20
     rep   stosb
     add   di, 300
     pop   cx
     loop  .DownBody

     add   di, 8
     mov   cx, 8
.DownMuzzle:
     push  cx
     mov   cx, 4
     rep   stosb
     add   di, 316
     pop   cx
     loop  .DownMuzzle

     mov   bx, DownOrientation

     jmp   .End
;==========================================================
.LeftTank:
     mov   cx, 20
     sub   di, 8
.LeftBody:
     push  cx
     mov   cx, 16
     rep   stosb
     add   di, 304
     pop   cx
     loop  .LeftBody

     sub   di, 8
     sub   di, 320 * 12
     mov   cx, 4
.LeftMuzzle:
     push  cx
     mov   cx, 8
     rep   stosb
     add   di, 312
     pop   cx
     loop  .LeftMuzzle

     mov   bx, LeftOrientation

     jmp   .End
;==========================================================
.RightTank:
     mov   cx, 20
     sub   di, 8
.RightBody:
     push  cx
     mov   cx, 16
     rep   stosb
     add   di, 304
     pop   cx
     loop  .RightBody

     sub   di, 320 * 12
     add   di, 16
     mov   cx, 4
.RightMuzzle:
     push  cx
     mov   cx, 8
     rep   stosb
     add   di, 312
     pop   cx
     loop  .RightMuzzle

     mov   bx, RightOrientation
;===========================================================
.End:
     ret
endp

proc Tank.Bullet uses es di ax cx si dx,\             ;если в uses не прописать si, dx, то пуля будет лететь с начала карты, хотя si и dx не задействованы...
     X, Y
    ; push  $A000
    ; pop   es
     mov     ax, cs
     add     ax, 1000h
     mov     es, ax

     mov     ax, 320
     mul     [Y]
     add     ax, [X]
     mov     di, ax
     mov     al, $29

.Draw:
     mov     cx, 4
.DrawBullet:
     push    cx
     mov     cx, 4
     rep     stosb
     add     di, 316
     pop     cx
     loop    .DrawBullet


     ret
endp


proc Tank.DrawHeart uses es di ax cx si dx,\
     color: BYTE, X, Y
     mov     ax, cs
     add     ax, 1000h
     mov     es, ax

     mov     ax, 320
     mul     [Y]
     add     ax, [X]
     mov     di, ax
     mov     al, [color]

     mov     cx, 2
     rep     stosb

     inc     di

     mov     cx, 2
     rep     stosb

     add     di, 314
     mov     cx, 2
.Loo:
     push    cx
     mov     cx, 7
     rep     stosb
     add     di, 313
     pop     cx
     loop    .Loo

     inc     di
     mov     cx, 5
     rep     stosb

     add     di, 316
     mov     cx, 3
     rep     stosb

     add     di, 318
     mov     cx, 1
     stosb


     ret
endp

