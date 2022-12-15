.thumb
.align

.global CallEventMenuASMC
.type CallEventMenuASMC, %function

.global prEventMenuCommandEffect
.type prEventMenuCommandEffect, %function


.macro blh to,reg=r3
	push {\reg}
	ldr \reg,=\to
	mov r14,\reg
	pop {\reg}
	.short 0xF800
.endm

.equ pLCDControlBuffer,0x3003080
.equ MemorySlot1,0x30004BC

.equ ClearBG0BG1,0x804E884
.equ SetFont,0x08003D38 @r0 = font struct, 0 for default
.equ SetupFontForUI,0x080043A8
.equ LoadNewUIGraphics,0x0804EB68
.equ NewMenu_DefaultChild,0x0804EBC8
.equ SetEventSlotC,0x0800D1F8


CallEventMenuASMC:
push {r4,r14}
mov r4,r0 @r4 = event engine proc

blh ClearBG0BG1

ldr r2,=pLCDControlBuffer
ldrb r0,[r2,#1]

mov r1,#0x1F 
orr r0,r1 @enable bg0-bg3+obj display

strb r0,[r2,#1]

@ Setting up relevant graphics 
@(Setting up text font and loading UI frame graphics)

mov r0, #0
blh SetFont
blh SetupFontForUI
blh LoadNewUIGraphics

@calling the actual lord split menu proc, with the event engine as parent

ldr r0,=MemorySlot1 @mem slot 1 contains the menu def
ldr r0,[r0]
mov r1,r4
blh NewMenu_DefaultChild

pop {r4}
pop {r0}
bx r0

.ltorg
.align




prEventMenuCommandEffect:

push {r14}

mov r0,#0x3C
ldrb r0,[r1,r0]

blh SetEventSlotC

mov r0,#0x17

pop {r1}
bx r1

.ltorg
.align



