ASSUME CS:CODE, DS:DATA

DATA SEGMENT
    M1 DB 10,13, "ENTER FIRST NUMBER: $"
    M2 DB 10,13, "ENTER SECOND NUMBER: $"
    M3 DB 10,13, "SUM: $"
    PROD DB 04
DATA ENDS

PRTMSG MACRO MESSAGE
    LEA DX,MESSAGE
    MOV AH,09H
    INT 21H
ENDM

GETDCM MACRO
    MOV AH,01H
    INT 21H
    SUB AL,30H
ENDM

PRTDCM MACRO
    MOV DL,[SI]
    ADD DL,30H
    MOV AH,02H
    INT 21H
ENDM

CODE SEGMENT
START: MOV AX,DATA
    MOV DS,AX
    PRTMSG M1
    GETDCM
    MOV BH,AL
    GETDCM
    MOV BL,AL

    PRTMSG M2
    GETDCM
    MOV CH,AL
    GETDCM
    MOV CL,AL

    MOV AX,CX
    MUL BX
    MOV CX,AX
    MOV AX,DX
    MOV BX,AX
    MOV [SI],CL
    INC SI
    MOV [SI],CX
    INC SI
    MOV [SI],BL
    INC SI
    MOV [SI],BX
    
    PRTMSG M3
     PRTDCM
     DEC SI
     PRTDCM
     DEC SI
     PRTDCM
     DEC SI
     PRTDCM 
     MOV AH,4CH
     INT 21H

    MOV AH,4CH
    INT 21H


CODE ENDS
END START