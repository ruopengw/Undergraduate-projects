	lw	21	0	op1
	lw	22	0	op2
	lw	23	0	op3
	beq	21	21	label
	addi	21	22	50
label	sll	24	21	23
	beq	21	21	8
	addi	21	22	50
	sll	24	21	23
	srl	25	21	23
	and	26	21	22
	or	27	21	22
	addi	28	22	10
	sw	29	0	answer
done	halt
op1	.fill	50			
op2	.fill	30
op3	.fill	4
answer	.fill	0