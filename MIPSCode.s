.data
_glb_a:	.word -1, 134, 124, 64, 908, 1324, 5243, 2, 0, -341
_prompt: .asciiz "Enter an integer:"
.globl main
_ret: .asciiz "\n"
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
move $sp, $fp
jr $ra
write:
lw $a0, -4($fp)
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
move $sp, $fp
jr $ra
j L41
sort: 
li $t0, 0
sw $t0, -8($fp)
L5: 
lw $t0, -8($fp)
lw $t1, -4($fp)
sw $t0, -8($fp)
sw $t1, -4($fp)
blt $t0, $t1, L10
j L40
L7: 
lw $t0, -8($fp)
addi $t0, $t0, 1
sw $t0, -8($fp)
j L5
L10: 
li $t0, 0
sw $t0, -12($fp)
L11: 
lw $t0, -4($fp)
lw $t1, -8($fp)
sw $t0, -4($fp)
sw $t1, -8($fp)
sub $t0, $t0, $t1
sw $t0, -88($fp)
addi $t0, $t0, -1
lw $t1, -12($fp)
sw $t0, -84($fp)
sw $t1, -12($fp)
blt $t1, $t0, L18
j L39
L15: 
lw $t0, -12($fp)
addi $t0, $t0, 1
sw $t0, -12($fp)
j L11
L18: 
lw $t0, -12($fp)
li $a0, 4
mul $t1, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t1
lw $t2, 0($a0)
sw $t1, -76($fp)
sw $t0, -12($fp)
addi $t0, $t0, 1
sw $t0, -68($fp)
li $a0, 4
mul $t0, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t0
lw $t1, 0($a0)
sw $t0, -64($fp)
sw $t1, -60($fp)
sw $t2, -72($fp)
bgt $t2, $t1, L25
j L38
L25: 
lw $t0, -12($fp)
li $a0, 4
mul $t1, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t1
lw $t2, 0($a0)
sw $t1, -56($fp)
move $t1, $t2
li $a0, 4
mul $t3, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t3
lw $t4, 0($a0)
addi $t5, $t0, 1
sw $t5, -40($fp)
li $a0, 4
mul $t5, $t5, $a0
la $a0, _glb_a
add $a0, $a0, $t5
lw $t6, 0($a0)
sw $t5, -36($fp)
la $a0, _glb_a
add $a0, $a0, $t3
sw $t6, 0($a0)
sw $t3, -48($fp)
sw $t0, -12($fp)
addi $t0, $t0, 1
sw $t0, -28($fp)
li $a0, 4
mul $t0, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t0
lw $t3, 0($a0)
la $a0, _glb_a
add $a0, $a0, $t0
sw $t1, 0($a0)
sw $t0, -24($fp)
sw $t1, -16($fp)
sw $t2, -52($fp)
sw $t3, -20($fp)
sw $t4, -44($fp)
sw $t6, -32($fp)
L38: 
j L15
L39: 
j L7
L40: 
li $v0, 0
move $sp, $fp
jr $ra
L41: 
main: 
move $fp, $sp
addi $sp, $fp, -20
li $t0, 0
addi $sp, $sp, -4
sw $fp, 0($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
addi $sp, $sp, -4
li $t1, 10
sw $t1, 0($sp)
sw $t0, -4($fp)
addi $fp, $sp, 4
addi $sp, $fp, -92
jal sort
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $fp, 0($sp)
addi $sp, $sp, 4
move $t0, $v0
sw $t0, -20($fp)
li $t0, 0
sw $t0, -4($fp)
L46: 
lw $t0, -4($fp)
li $a0, 10
sw $t0, -4($fp)
blt $t0, $a0, L51
j L56
L48: 
lw $t0, -4($fp)
addi $t0, $t0, 1
sw $t0, -4($fp)
j L46
L51: 
lw $t0, -4($fp)
sw $t0, -4($fp)
li $a0, 4
mul $t0, $t0, $a0
la $a0, _glb_a
add $a0, $a0, $t0
lw $t1, 0($a0)
sw $t0, -12($fp)
addi $sp, $sp, -4
sw $fp, 0($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
addi $sp, $sp, -4
sw $t1, -8($fp)
sw $t1, 0($sp)
addi $fp, $sp, 4
addi $sp, $fp, -0
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $fp, 0($sp)
addi $sp, $sp, 4
j L48
L56: 
li $v0, 0
move $sp, $fp
jr $ra
