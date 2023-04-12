.thumb 
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
@ 0x801CA24 with callHackNew  
.equ GetUnitSelectionValueThing, 0x801D51C
.equ GetUnit, 0x8019430 
.global HookButtonPressA
.type HookButtonPressA, %function 
HookButtonPressA: 
push {r5, lr} 
ldrb r0, [r0] 
blh GetUnit 
mov r4, r0 
blh GetUnitSelectionValueThing
mov r5, r0 @ something to return 
cmp r4, #0 
bne GotoEnd 
mov r0, r6 @ proc 
bl Map_OnAPress
mov r5, r0 
GotoEnd: 
mov r0, r5
pop {r5} 
pop {r3} 
bx r3 
.ltorg 


@hook @ 801CAD4, normally checks start/select, now checks B and does our thing
.global HookButtonPressB
.type HookButtonPressB, %function
HookButtonPressB:
mov r0,r6 @proc
bl Map_CheckBPress
ldr r0,=#0x801CAE7
bx r0
.ltorg









