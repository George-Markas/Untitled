.data
	digitCount: .word 0 # Variable to store digitCount
	intInput: .space 4 # reserving 4 bytes
	message: .asciiz "Enter a number:\n"
	messageEven: .asciiz "Number is even \n"
	messageOdd: .asciiz "Number is odd.\n"
	messageIsMultipleOf4: .asciiz "Number is a multiple of 4\n"
	messageIsNotMultipleOf4: .asciiz "Number is not a multiple of 4\n"
	bitMask: .word 15 # 0xf in decimal, for isolating the last digit of a number

.text
	main:
		# print message
		li $v0, 4
    		la $a0, message
	    syscall

		# read int
		li $v0, 5
		la $a0, intInput
		syscall
		
		la $t2, ($v0) # copying contents of intInput to $t2
		la $t9, ($v0) # copying contents of intInput to $t9  
		li $t1, 0 # initialize digitCount to 0
		
		# determining the number of digits of intInput
		loop:
			beqz $t2, exit_loop # exit loop when temp reaches 0
			addi $t1, $t1, 1 # increment digitCount
			srl $t2, $t2, 4 # bitshift $a1 right by 4
			j loop
		exit_loop:
		
		lw $t4, bitMask
		li $t8, 0 # initialize $t8 to 0
		and $t8, $t9, $t4
		bnez $t8, if
		
		else:
			li $v0, 4
    			la $a0, messageEven
	    		syscall
			j exit_if
		if:
			li $v0, 4
    			la $a0, messageOdd
	    		syscall
	    	exit_if:
	    	
	    	li $t4, 3 # initialize bitMask to 0011
	    	li $t8, 0 # initialize $t8 to 0
	    	and $t8, $t9, $t4
	    	bnez $t8, if_2
	    	
	    	else_2:
			li $v0, 4
    			la $a0, messageIsMultipleOf4
	    		syscall
			j exit_if_2
		if_2:
			li $v0, 4
    			la $a0, messageIsNotMultipleOf4
	    		syscall
	    	exit_if_2:
	    	
		# printing the number of digits
		li $v0, 1             
        move $a0, $t1
        syscall
        
		# program exit
		li $v0, 10
		syscall