
DATA SEGMENT 
	STRING1 DB 11H,22H,33H,44H,55H
	MSG1 DB 10,13, "FOUND$"
	MSG2 DB 10,13, "NOT FOUND$"
	SE DB 10H
DATA ENDS

PRINT MACRO MSG
	MOV AH,09H
	LEA DX,MSG
	INT 21H
	INT 3
	ENDM

CODE SEGMENT
ASSUME CS:CODE, DS: DATA
	START:
	MOV AX, DATA
	MOV DS,AX
	MOV AL,SE
	LEA SI, STRING1
	MOV CX,04H

	UP: MOV BL,[SI]
	CMP AL, BL
	JZ FO
	INC SI
	DEC CX
	JNZ UP
	PRINT MSG2
	JMP END1

	FO: PRINT MSG1
	END1: 
	MOV AH,4CH
	INT 21H
CODE ENDS	
	END START

