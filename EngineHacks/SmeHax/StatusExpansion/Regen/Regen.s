.thumb
.align

.global RegenFunc
.type RegenFunc, %function

RegenFunc:
push {r14}

//r0 = unit, r1 = current heal %
//return new heal % in r0

@check for status
add r0,#0x30
ldrb r0,[r0]
mov r2,#0x1F
and r0,r2
ldr r2,=RegenStatusIDLink
ldrb r2,[r2]
cmp r0,r2
bne NoRegenStatus

@add 25% regen
add r1,#25

NoRegenStatus:
mov r0, r1 @return the amount healed.
pop {r1}
bx r1

.ltorg
.align
