.thumb
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
.equ LiquidOozeID, SkillTester+4
.equ d100Result, 0x802a52c
@ r0 is attacker, r1 is defender, r2 is current buffer, r3 is battle data
push {r4-r7,lr}
mov r4, r0 @attacker
mov r5, r1 @defender
mov r6, r2 @battle buffer
mov r7, r3 @battle data

@check for miss
ldr     r0,[r2]           @r0 = battle buffer                @ 0802B40A 6800     
lsl     r0,r0,#0xD                @ 0802B40C 0340     
lsr     r0,r0,#0xD        @Without damage data                @ 0802B40E 0B40     
mov	r1,#0x82 @miss + devil
and	r0,r1
cmp	r0,#2
beq	End

@check for draining weapon
mov	r0,#0x4A
ldrh	r0,[r4,r0]	@equipped item
mov	r1,#0xFF
and	r0,r1		@only item id
mov	r1,#36		@size of entry
mul	r0,r1
add	r0,#31		@offset of weapon effect byte
ldr	r1,=0x080177C0	@has table pointer
ldr	r1,[r1]
ldrb	r0,[r1,r0]	@weapon effect
cmp	r0,#2		@steal hp effect
bne	End

@make sure damage > 0
mov	r0,#4
ldrsh	r0,[r7,r0]
cmp	r0,#0
ble	End

@check for devil
ldr     r0,[r2]           @r0 = battle buffer                @ 0802B40A 6800     
lsl     r0,r0,#0xD                @ 0802B40C 0340     
lsr     r0,r0,#0xD        @Without damage data                @ 0802B40E 0B40     
mov	r1,#0x82 @miss + devil
and	r0,r1
cmp	r0,#0x80
beq	noDamage

@if we proc, set the hp update flag
ldr     r2,[r6]    
lsl     r1,r2,#0xD                @ 0802B42C 0351     
lsr     r1,r1,#0xD                @ 0802B42E 0B49     
mov     r0, #0x1
lsl     r0, #8           @0x100, hp drain/update
orr     r1, r0

ldr     r0,=#0xFFF80000                @ 0802B434 4804     
and     r0,r2                @ 0802B436 4010     
orr     r0,r1                @ 0802B438 4308     
str     r0,[r6]                @ 0802B43A 6018

@check for liquid ooze
ldr r0, SkillTester
mov lr, r0
mov r0, r5 @defender data
ldr r1, LiquidOozeID
.short 0xf800
mov	  r1, #4
ldsh	r1, [r7, r1]    @ damage
ldrb  r2, [r5, #0x13] @ defender's curr hp
cmp   r1, r2
ble   defLives        @ Damage taken / HP healed by attacker.
  mov   r1, r2        @ can't exceed damage dealt to defender.
defLives:
cmp r0, #0
beq noOoze
  neg   r1, r1
noOoze:
mov   r2, #0x5
ldsb	r2,[r6,r2]	@hp change
add   r2, r1
strb	r2,[r6,#5]	@hp change

End:
pop {r4-r7}
pop {r15}

noDamage:
@unset reversal
ldr     r2,[r6]    
lsl     r1,r2,#0xD                @ 0802B42C 0351     
lsr     r1,r1,#0xD                @ 0802B42E 0B49     
mov     r0,#0x80
mvn	r0,r0
and     r1,r0

ldr     r0,=#0xFFF80000                @ 0802B434 4804     
and     r0,r2                @ 0802B436 4010     
orr     r0,r1                @ 0802B438 4308     
str     r0,[r6]                @ 0802B43A 6018

@mov	r0,#0x13
@ldsb	r0,[r4,r0]	@remaining hp
@mov	r2,#4
@ldsb	r2,[r7,r2]	@damage
@add	r0,r2
@strb	r0,[r4,#0x13]	@remaining hp
mov	r0,#0
strb	r0,[r7,#4]
strb	r0,[r7,#5]

mov	r0,#0xFF	@no animation!
strb	r0,[r6,#4]

b	End

.align
.ltorg
SkillTester:
@POIN SkillTester
@WORD LiquidOozeID
