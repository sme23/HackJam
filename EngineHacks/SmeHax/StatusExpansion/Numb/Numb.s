.thumb
.align


.global NumbFunc
.type NumbFunc, %function
NumbFunc: @return usability bool in r0

push {r4-r7,r14}
mov r4,r0 @character pointer
mov r5,r1 @item halfword
mov r6,r2 @character wrank.

mov r0,r4
add r0,#0x30
ldrb r0,[r0]
lsl r0,#27
lsr r0,#27
ldr r1,=NumbStatusIDLink
ldrb r1,[r1]
cmp r0,r1
bne RetTrue

@load weapon ability word @@@@@this is not an attacker struct we can't use the version in ram!!!
mov r1,#0xFF
and r1,r5
mov r0,#0x24
mul r1,r0
ldr r0,=ItemTable
add r2,r0,r1
mov r0,r2
add r0,#8
ldr r0,[r0]
mov r1,#0x46 @magic weapon or staff or magic sword
and r0,r1
cmp r0,#0
bne RetTrue
ldrb r0,[r2,#7] @weapon type
cmp r0,#4 @staff because apparently the staff bit doesn't count
beq RetTrue

RetFalse:
mov r0,#0
b GoBack

RetTrue:
mov r0,#1

GoBack:
pop {r4-r7}
pop {r1}
bx r1

.ltorg
.align
