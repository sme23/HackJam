; 080160d0 ���炱�̃R�[�h�֔�΂�.(FE8J)
; 080160C4 ���炱�̃R�[�h�֔�΂�.(FE8U)

@thumb
	cmp  r0, #0x10		;BGM:�����߂�
	bne  exit			;�Ⴄ�Ȃ�I��
	
	                    ;�}�b�v�ݒ肩��擾
;	ldr  r0, =$0202BCEC		;(FE8J)
	ldr  r0, =$0202BCF0		;(FE8U)

	ldrb r0, [r0, #14]
	
	;//�}�b�v�ԍ�����A�}�b�v�ݒ�̃A�h���X��Ԃ��֐� r0:�}�b�v�ݒ�̃A�h���X r0:���ׂ����}�b�vID:MAPCHAPTER
;	ldr  r1, =$08034520		;(FE8J)
	ldr  r1, =$08034618		;(FE8U)

	mov  lr, r1
	@dcw $F800
	add  r0, #0x28
	ldrh r0, [r0]
exit
;	ldr r1,=$08016fdc		;(FE8J)
	ldr r1,=$08017234		;(FE8U)

	mov pc,r1
