
.= torg + 1000 ;pc will start at address 1000
tks = 177560
tkb = 177562
tps = 177564
tpb = 177566
lcs = 177546
main:
    ; Interrupt Vector Initialization
    mov #inp, @#60
    mov #340, @#62 ; program priority set to 7: no interrupts expected  while 
                   ; running inp. 
    mov #outp, @#64
    mov #0, @#66
    mov #clock, @#100
    mov #300, @#102 ; program priority 6

    mov #100, @#tps
    mov #101, @#tks

    ;initialize stack
    mov pc, sp
    tst -(sp)


    mov #str_welcome, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter


    loop_program_running:
    jsr pc, clear_input_labels_and_args
    jsr pc, clearRegisters ; clearing registers
    mov #0, @#temp_word; clearing tmp helper- words
    mov #0, @#tmp_wrd  ; clearing tmp helper- words
    mov #0, @#tm_wrd ; clearing tmp helper- words
    mov #0, tmpWrd ; clearing tmp helper- words
    mov #before_command, -(sp) ; printing before-command string($)
    jsr pc, printStr
    tst (sp)+
    checkCrack: cmpb @#crackflag, #1
    beq crack
    jsr pc, inputUserInput
    mainContinue:
    cmp #1, @#isCommandRunning
    beq checkCrack
    mov #0, @#flag2
    br loop_program_running
    mov r1, r2
    halt
    

    doRti:
        mov @#saveR0, r0
        mov @#saveR1, r1
        mov @#saveR2, r2 ; return register values of the cracking
        mov @#saveR3, r3
        mov @#saveR4, r4
        mov @#saveR5, r5
        mov #101, @#tks
        rti

    crack:
    cmp @#endedCracking, #1
    bne cnt70
    loop11: mov #101, @#tks
    wait  ; wait for input
    br loop11

    cnt70:
    cmpb @#startedCracking, #1
    beq doRti
                    ;else havent started cracking yet
    mov #101, @#tks ; turning on tks; from this point input is enabled
    mov #1, @#startedCracking
    ;STARTED CRACKING USING SYSTEM, A FUNCTION FROM HW3
    mov #IMG2, -(sp)
    mov #IMG3, -(sp)
    mov @#crkRows, -(sp)
    mov @#crkCols, -(sp)
    mov #Hash, -(sp)

    ;this function receives an original image and an encoded image and finds the
;key needed to convert the original image to the encoded image
;this function uses all registers r0,r1,r2,r3,r4,r5
;this function receives 5 parameters
;1. the address of the original image array located in 12(sp)
;1. the address of the encoded image array located in 10(sp)
;2. the number of rows in the image located in 6(sp)
;3. the number of cols in the image located in 4(sp)
;4. the address of the Hash array located in 2(sp)
;this function uses the variable "Key" as in param
    mov #0, @#flag2 ; enabling commands input
    mov #10, -(sp)  ; simulating return pc
    loop:
        jsr pc, clearRegisters ;clear all registers just for safety

        mov #crackey, r1 ;push address of Key to stack
        jsr pc, nextKey ;set the Key to the next key in order

        jsr pc, clearRegisters ;clear all registers just for safety

        ;encode image using the key
        mov 12(sp), -(sp) ;push address of OrigImg to stack
        mov 10(sp), -(sp) ;push number of rows to stack
        mov 10(sp), -(sp) ;push number of cols to stack
        mov 10(sp), -(sp) ;push address of Hash to stack
        mov #crackey, -(sp) ;push address of Key to stack
        jsr pc, encode
        add #12, sp ;clear local variables

        jsr pc, clearRegisters ;clear all registers just for safety

        ;check if the appropriate key was found
        tst -(sp) ;create local variable for the output of "cmpImg"
        mov 14(sp), -(sp) ;push address of OrigImg to stack
        mov 14(sp), -(sp) ;push address of EncImg to stack
        mov 14(sp), -(sp) ;push number of rows to stack
        mov 14(sp), -(sp) ;push number of cols to stack
        jsr pc, cmpImg
        add #10, sp ;clear local variables
        tst (sp)+ ;check cmpImg output and then clear local variable
        beq end20 ;if images are equal then return to caller

        jsr pc, clearRegisters ;clear all registers just for safety

        ;decode the image and return it to its original state(before the encode)
        mov 12(sp), -(sp) ;push address of OrigImg to stack
        mov 10(sp), -(sp) ;push number of rows to stack
        mov 10(sp), -(sp) ;push number of cols to stack
        mov 10(sp), -(sp) ;push address of Hash to stack
        mov #crackey, -(sp) ;push address of Key to stack
        jsr pc, decode
        add #12, sp ;clear local variables

        br loop ;loop until valid key is found

    halt  ; shouldnt get here

    end20:
    tst (sp)+ ; popping ret pc simulator
    jmp CrackFoundKey

printNoKey: ;if no key was found
    tst (sp)+ ; popping ret pc from nextKey func
    jsr pc, printEnter
    mov #NoFoundKey, -(sp)
    jsr pc, printStr
    tst (sp)+
    jmp doneKey



CrackFoundKey: ; the right key was found! 1st step: printing the key.
; we will transfer the key to arg3, which shouldnt be in use now - we use it 
; only while in pre_crack.
    jsr pc, chSevenPrio ; program priority changed to 7; no interrupts should 
    ; accur while running this func
    jsr pc, printEnter
    mov #crack1Completed, -(sp)
    jsr pc, printStr
    tst (sp)+
    mov #arg3, r2
    mov #0, r1 ; key size counter
    mov #crackey, r0
    loop30: cmp (r0), #-1
    beq doneKey
                            ; else
    jsr pc, Keyprint_Char; uses r0,prints the current node
    mov 2(r0), r0 ; moves to next node
    br loop30

    doneKey: ; the key was printed! we would like to nullify it
    mov #crackey, r0
        nullLoop:
            mov #-1, (r0)
            add #2, r0
            cmp (r0), #0
            beq cnt30
            sub #2, r0
            mov 2(r0), r0
            br nullLoop

    cnt30:
    mov r0, r5
    add #2, r5 ; r5 points to the next node
    ; we will check if r5 is indeed a node(it may be out of the nodes range)
    cmp (r5), #-1
    bne cnt35
    mov r5, (r0)
    cnt35: ; need to print times
        jsr pc, printEnter
        mov #stCrack3, -(sp)
        jsr pc, printStr
        tst (sp)+
        mov @#timer_counter, r1 ;r1 is the original time
        mov @#crack_timer, r2 ; r2 is the original crack time
        mov r2, @#timer_counter ; timer counter contains the cracking time
        ; in order to print it in the relevant func
        mov r1, @#crack_timer ; crack_timer is the original time; its stil 
        ; incrementing.
        mov #1, @#crackprint
        jsr pc, print_Time ; crack time printed
        mov #0, @#crackprint
        mov @#crack_timer, @#timer_counter ; timer counter got its original
        ; value, plus the added time, back
        mov #0, @#crack_timer



        ; this function may have interrupted the user when he was typing 
        ; something. we should return exactly to the same point
        jsr pc, ch_prio ; change prio back to 0
        cmp @#user_input, #0 ; if 0 then user is not in the middle of typing
        ;a command and\or parameters.
        beq cnt64
                                                    ; printing the input line
        mov #0, @#flag2
        mov #user_input, -(sp)
        jsr pc, PrntCrack_inputStr
        tst (sp)+
    
        cnt64:
        cmp @#IMG1, #0
        beq cnt61
                                ; printing the IMG(where the user stopped)
        mov #1, @#flag2
        mov r0, #0
        mov #IMG1, r1
        loop60: 
            cmpb (r1), #0
            beq cnt62
                            ; else
            mov r1, r0
            jsr pc, print_Char
            add #1, r1
            br loop60
            
    cnt61:
        mov #1, @#endedCracking
        jmp afterCracking

    cnt62:
    mov #1, @#endedCracking
    jmp afterCracking


    PrntCrack_inputStr:; ret pc, address of string OF WORDS
    mov 2(sp), r0 ; r0 is the address of the first char of the string
    prnt2Str_loop: 
        cmpb (r0), #0
        beq finPrint_str ; if the char is 0, then we printed the entire str.
        jsr pc, print_Char ; printing the char
        add #2, r0 ; incrementing r0
        br prnt2Str_loop
    finPrint_str:
    rts pc
        




    




    Keyprint_Char:
    cmp (r0), #9
    bhi cnt40
    add #60, (r0)
    jsr pc, print_Char
    rts pc

    cnt40:
    add #127, (r0)
    jsr pc, print_Char
    rts pc


    

    chSevenPrio:
    mov (sp), -(sp) 
    mov #340, 2(sp) ; new priority, 7 
    rti


;*inputImage*   

inputImage: ; stack: pc return, cols, rows, img address
    jsr pc, clearRegisters ;clear all registers just for safety
    mov #101, @#tks
    mov 2(sp), r1 ; number of cols
    mov 4(sp), r3 ; number of rows
    mov 6(sp), r5 ; image address
    mul r3, r1 ; r1 is the number of cells
    mov r1,r2 ; r2 is the number of cells
    mov #1, @#flag2 ; 1 for img input.
    w_inputImage:
        wait
        br w_inputImage
    end_inputImage:
    tst (sp)+ ; pop pc(from the interrupt)
    tst (sp)+ ; pop psw(from the interrupt)
    jsr pc, convertImgHexToOct
    jsr pc, ch_prio ; we returned from the interrupt using br, not 
                              ; rti. Therefore we need to manually change the
                              ; the program priority. Also, that`s the reason
                              ; for popping the pc and PSW. usually rti does
                              ; the job.
    rts pc


getTwoParameters: ; r0 is the address of the first char AFTER the command
; teh input label is of WORDS
    mov #arg1, r1 ; r1 is the address of the first parameter label(WORDS)
    mov #arg2, r2 ; r2 is the address of the second parameter label(WORDS)
    cmpb (r0), #40 ; if the char is space
    bne not_space
    loop_while_space:
    add #2, r0 ; increment to the next char
    cmpb (r0), #40; if the char is space
    beq loop_while_space
    cmpb(r0), #0 ; if the 50 chars limit ended, halt. We shouldnt get here
    bne not_space
    halt

    not_space: ; (r0) is the first char of the parameter(OCTAL!)
    mov (r0), (r1)
    add #2, r0 ; next char
    add #2, r1 ; next char
    cmpb(r0), #0 ; if the 50 chars limit ended, halt. We shouldnt get here
    bne ok
    halt

    ok:
    cmpb (r0), #40 ; if the next char is space, then the copying of the first
    ; parameter has ended.
    beq second_parameter
    br not_space ; continue looping through the parameter.


    second_parameter:
    cmpb (r0), #40 ; if the char is space
    bne notSpace2
    loopWhile_space2:
    add #2, r0 ; increment to the next char
    cmpb (r0), #40; if the char is space
    beq loopWhile_space2
    cmpb(r0), #0 ; if the 50 chars limit ended, halt. We shouldnt get here
    bne notSpace2
    halt

    notSpace2:; (r0) is the first char of the SECOND parameter(OCTAL!)
    mov (r0), (r2)
    add #2, r0 ; next char
    add #2, r2 ; next char
    cmpb (r0), #0 ; end of the input line. parameters done
    beq ret
    cmpb(r0), #40 ; next char is space then parameter copying ended
    beq ret
    ;else the next char will be the continuation of the parameter
    br notSpace2

    ret: ; we would like to convert the ascii codes of parameters to numeric
    ;parameters
    mov r0, @#tmpWrd ;tmpWrd is the address of the first char after the second
    ;parameter
    mov #arg1, r0 ; the address of the string we would like to convert to a num
    jsr pc, OctalStringToOctalNum
    mov #arg2, r0 ; the address of the string we would like to convert to a num
    jsr pc, OctalStringToOctalNum
    rts pc


pre_crack:
    mov #IMG2, -(sp)
    jsr pc, ClearImage ; clearing the img
    tst (sp)+
    mov #IMG3, -(sp)
    jsr pc, ClearImage ; clearing the img
    tst (sp)+
    jsr pc, printEnter ; print new line
    mov #strCrack1, -(sp) ; relevant message printing
    jsr pc, printStr
    tst (sp)+ ; popping message`s address
    jsr pc, printEnter ; enter printing
    mov @#tm_wrd, r0 ; r0 is the address of the first char AFTER the command
    jsr pc, getTwoParameters ; tmpWrd is the address of the FIRST char after the
    ;second parameter
    mov #crkRows, r1
    mov #crkCols, r2
    mov @#arg1, (r1) ; arg1 is the number of rows
    mov @#arg2, (r2) ; arg2 is the number of cols
    mov @#tmpWrd, r0
    jsr pc, getThirdParameter
    mov #crkMaxKeyLength, r1
    mov @#arg3, (r1) ; arg3 is the max key length
    mov #IMG2, -(sp)    ; pushing the address of the image to whom we insert the
                        ; pixels the user types
    mov @#crkRows, -(sp) ;rows
    mov @#crkCols, -(sp) ;cols
    jsr pc, inputImage
    tst (sp)+ ; pop rows
    tst (sp)+ ; pop cols
    tst (sp)+ ; pop img2 address
    mov #sCrack2, -(sp) ; relevant message printing
    jsr pc, printStr
    tst (sp)+ ; popping message`s address
    mov #IMG3, -(sp)    ; pushing the address of the image to whom we insert the
                        ; pixels the user types
    mov @#crkRows, -(sp) ;rows
    mov @#crkCols, -(sp) ;cols
    jsr pc, printEnter ; print new line
    jsr pc, inputImage
    tst (sp)+ ; pop rows
    tst (sp)+ ; pop cols
    tst (sp)+ ; pop img3 address
    jsr pc, UpdateCrackKeySize
                        
    jsr pc, clear_input_labels_and_args
    mov @#timer_counter, @#crack_timer
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #1, @#crackflag
    mov #0, @#isCommandRunning

    rts pc


pre_setkey: ;stack: return pc
    mov @#tm_wrd, r0 ; address of the first char of input AFTER the command
    mov #0, r4 ; counter
    mov #arg1, r1 ; r1 is the address of the parameter label(WORDS)
    cmpb (r0), #40 ; if the char is space
    bne nSpace
    lWhile_space:
    add #2, r0 ; increment to the next char
    cmpb (r0), #40; if the char is space
    beq lWhile_space

    nSpace:  ;(r0) is the first char of the parameter(HEXADICIMAL ASCII!)
    mov #key, r2 ; address of the first key node
    loop10:
        cmpb (r0), #0
        beq end9
        cmpb (r0), #40
        beq end9
        add #1,r4
        cmpb (r0), #71
        blos minSixty
        sub #127, (r0)
        br set_key

        minSixty:
        sub #60, (r0)

        set_key:
        movb (r0), (r1)
        add #2, r1
        mov (r0), (r2)
        mov 2(r2), r2
        add #2, r0
        br loop10

        end9:
        jsr pc, printEnter
        mov #strKey, -(sp)
        jsr pc, printStr
        tst (sp)+
        mov r4, -(sp) ; number of elements
        mov #arg1, -(sp)
        jsr pc, cnvrt_hex_nums_to_hex_asciiPrint
        tst (sp)+
        tst (sp)+
        jsr pc, printEnter
        mov #0, @#lcs
        jsr pc, print_Time
        mov #0, @#timer_counter
        mov #0, @#isFirstNotSpace
        mov #0, isCommandRunning
        rts pc

pre_encode:
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    jsr pc, printEnter ; print new line
    mov #str_encode_message, -(sp) ; relevant message printing
    jsr pc, printStr
    tst (sp)+ ; popping message`s address
    jsr pc, printEnter ; enter printing
    mov @#tm_wrd, r0 ; r0 is the address of the first char AFTER the command
    jsr pc, getTwoParameters
    mov #lastImgRows, r1
    mov #last_ImgCols, r2
    mov @#arg1, (r1)
    mov @#arg2, (r2)
    mov #IMG1, -(sp)    ; pushing the address of the image to whom we insert the
                        ; pixels the user types
    mov @#arg1, -(sp) ;rows
    mov @#arg2, -(sp) ;cols
                        ; cleaning i
        mov #user_input,r1 ; nullify user_input; used in the final cracking func
        loop54:
            cmpb (r1), #0
            beq cnt54
            mov #0, (r1)
            add #2, r1
            br loop54

    cnt54:
    cmp @#crackflag, #1 ; if we are cracking right now
    beq EncImgInput
    jsr pc, inputImage
    tst (sp)+ ; pop rows
    tst (sp)+ ; pop cols
    tst (sp)+ ; pop img1 address
    ; PREPATRING STACK FOR ENCODE FUNCTION:
    mov #IMG1, -(sp)
    mov @#arg1, -(sp)
    mov @#arg2, -(sp)
    mov #Hash, -(sp)
    mov #key, -(sp)
    jsr pc, encode
                            ;now we woudl like to print the image
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #str_done_encode, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter
    mov @#arg2, -(sp)
    mov @#arg1, -(sp)
    mov #IMG1, -(sp)
    jsr pc, printImg
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #0, @#lcs
    jsr pc, print_Time
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #0, @#isCommandRunning
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    rts pc

EncImgInput:

    mov #1, @#isCommandRunning
    mov @#arg1, @#tempRows ; arg1 will be destroyed in main
    mov @#arg2, @#tempCols ; arg2 will be destroyed in main


    jsr pc, clearRegisters ;clear all registers just for safety
    mov #1, @#EncDec ; flag for recognizing img input for enc purposes or dec.
    mov (sp), r1 ; number of cols
    mov 2(sp), r3 ; number of rows
    mov 4(sp), r5 ; image address
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mul r3, r1 ; r1 is the number of cells
    mov r1,r2 ; r2 is the number of cells
    mov #1, @#flag2 ; 1 for img input.
    mov r2, @#numCells ; global var.
    mov r5, ImgAddress ; global var.

    rts pc ; to main.

    EncDone: ; will get here after all img was filled
    ; PREPATRING STACK FOR ENCODE FUNCTION:
    jsr pc, ch_prio ; eturn the prio back to 0
    mov #IMG1, -(sp)
    mov @#tempRows, -(sp)
    mov @#tempCols, -(sp)
    mov #1, -(sp)  ; simulating second ret pc
    jsr pc, convertImgHexToOct
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #IMG1, -(sp)
    mov @#tempRows, -(sp)
    mov @#tempCols, -(sp)
    mov #Hash, -(sp)
    mov #key, -(sp)
    jsr pc, encode
                            ;now we woudl like to print the image
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #str_done_encode, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter
    mov @#tempCols, -(sp)
    mov @#tempRows, -(sp)
    mov #IMG1, -(sp)
    jsr pc, printImg
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    mov #0, @#lcs
    jsr pc, print_Time
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #0, @#isCommandRunning
    cmp #1, @#endedCracking
    bne cnt80
    mov #0, @#crackflag
    mov #0, @#startedCracking
    mov #0, endedCracking

    cnt80:
    jmp mainContinue


;*printImg* stack parameters(from top to bottom): return pc, img address, 
; number of rows, number of cols.
; the function uses the following registers: R0 R1 R3 R5(in helper funcs)
; the function uses the following helper-functions: handleChar,printEnter
; the function prints the img that its address was sent as a parameter to the 
; func. The function prints in hexadecimal base(NOTE: the img is written in
; octal base in the memory). The img elements` range should be 0-17(octal).
printImg: 
    mov 2(sp), r0 ;  r0 is the address of the first element of the img
    movb 4(sp), r1 ; r1 is the number of rows
    movb 6(sp), r3 ; r3 is the number of cols

    loopRows: ; moves through the rows, r1 times
        loopCols: ; in each row, moves through the cols, r3 times
            jsr pc, handleChar ; the address of the img is still on the top of 
                              ; the stack
            add #1, r0
            sob r3, loopCols
        jsr pc, printEnter
        movb 6(sp), r3 ; r3 is the number of cols
        sob r1, loopRows
rts pc

;*inputUserInput* stack parameters: return pc
; the function uses the followign registers: r1,r2,
; the function is the most important one. It uses most of our functions.
; the function gets the user`s input line, proccesses it through the input 
; interrupt(proccessUserInput) and then sends it to handleCommand.

inputUserInput:
    mov #0, @#flag2 ; 0 'flag' is for user input(used in input interupt)
    afterCracking: mov #101, @#tks
    mov #user_input, r2 ; in order to increment the address of the input
                        ; label, we`ve chosen to keep the address in r2.
    w_input: wait ; waiting for input interupt
    br w_input 
end_of_input: jsr pc, ch_prio ; we returned from the interrupt using br, not 
                              ; rti. Therefore we need to manually change the
                              ; the program priority. Also, that`s the reason
                              ; for popping the pc and PSW. usually rti does
                              ; the job.
              cmp @#startedCracking, #1
              beq EndOfInputWhileCracking
              tst(sp)+ ; popping pc
              tst(sp)+ ; popping PSW
              mov #user_input, r2 ; r2 is the address of the input label
              jsr pc, handleCommand
              rts pc
EndOfInputWhileCracking: ; we WONT(!) pop PSW and pc because we need them in
; order to return to the same point in which we stopped at cracking
    mov #user_input, r2
    jsr pc, handleCommand
    jmp mainContinue    



ch_prio:
    mov (sp), -(sp) 
    mov #0, 2(sp) ; new priority
    rti

clear_input_labels_and_args: ;stack: return pc
    mov #user_input, r0
    mov #user_command, r1
    mov #arg1, r2
    mov #arg2, r3
    mov #arg3, r4
    clearUserInput:
        cmpb (r0), #0
        beq clear_UserCommand
        mov #0, (r0)
        add #2, r0
        br clearUserInput
    clear_UserCommand:
        cmpb (r1), #0
        beq clr_arg1
        mov #0, (r1)
        add #2, r1
        br clear_UserCommand

    clr_arg1:
        cmpb (r2), #0
        beq clr_arg2
        mov #0, (r2)
        add #2, r2
        br clr_arg1

    clr_arg2:
        cmpb (r3), #0
        beq clr_arg3
        mov #0, (r3)
        add #2, r3
        br clr_arg2

    clr_arg3:
        cmpb (r4), #0
        beq end_clear_input_labels_and_args
        mov #0, (r4)
        add #2, r4
        br clr_arg3

        end_clear_input_labels_and_args:
        rts pc

ClearImage: ; stack: pc return, img address
    mov 2(sp), r0
    l_clearImage:
        cmpb (r0), #0
        beq endClearImage
        mov #0, (r0)
        add #2, r0
        br l_clearImage

    endClearImage:
    rts pc

;INPUT INTERRUPT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;*inp* this is the INTERRUPT for input. stack parameters from top to bottom:
;return pc, psw
;registers parameters: r1,r2, r5(the last two used in ProccessImgInput)
;uses the following registers: r1, r0, r2, r5(the last two used in 
                                                            ;ProccessImgInput)
; In order to proccess different kinds of inputs, r1 is used as a 'flag':
; 0 for user command line input, 1 for user img input.
; sub funcs:
; *proccessUserInput*: registers parameters: r2 - input line-label`s address
; the function proccesses the input. It inserts it to the relevant label.
; when ENTER pressed, it ends the input.
; *ProccessImgInput*: registers parameters: r2 - number of cells needed to the
; the img. r5- the address of the IMG.
; the function proccesses the img input according to the instructions.


inp: ; stack(top to bottom): pc, psw
    mov r0, @#saveR0
    mov r1, @#saveR1
    mov r2, @#saveR2
    mov r3, @#saveR3
    mov r4, @#saveR4
    mov r5, @#saveR5
    mov #1, @#isCommandRunning

    cmpb @#flag2, #0 ; 0 for command line input
    beq proccessUserInput
    cmpb @#flag2, #1
    beq ProccessImgInput

proccessUserInput:
    cmpb @#tkb, #15 ; if the char is ENTER then the input has ended
    beq end_of_input
    cmpb #40, @#tkb
    bne nt_Space
    mov @#isFirstNotSpace, r5
    cmpb r5, #0
    bne CommandTypingEnded

    cont8: ; we would liek to put the char in the input label at the first place
    ; that IS 0
    mov #user_input, r2
    lpSpace:
        cmp (r2), #0
        beq cont9
        add #2, r2
        br lpSpace
    cont9:
    movb @#tkb, (r2) ; moving the char(ascii code) to the input label through
                     ; r2.
    mov r2, r0       ; r0 is used in the print_Char func as the address of the
                     ; char.
    jsr pc, print_Char
    cmpb @#tkb, #10  ; if the char is BACKSPACE 
    beq input_Backspace_Words ; then branch to the special label
    add #2, r2 ; else, increment to the next word
inc0_tks:
    inc @#tks ; RE-->1, 
    mov @#saveR0, r0
    mov @#saveR1, r1
    mov @#saveR2, r2 ; return register values of the cracking
    mov @#saveR3, r3
    mov @#saveR4, r4
    mov @#saveR5, r5
    rti

    nt_Space:
    mov #1, r5
    mov r5, @#isFirstNotSpace
    br cont8

    CommandTypingEnded: ; turn on clock IE; starting to count.
    mov #100, @#lcs 
    br cont8


    input_Backspace_Words:
    mov #0, (r2) ; nullify the latest char(which is BACKSPACE)
    sub #2, r2   ; move to the previous word
    mov #0, (r2) ; nullify the previous word
    br inc0_tks

    inc1_tks:
    inc @#tks ; RE-->1,
    rti

    is_DecOrEnc:
    jmp isEncOrDec

ProccessImgInput:
    cmp @#crackflag, #1
    beq is_DecOrEnc
    cmp r2, #0 ; if no more cells needed
    bne cnt_func
    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq input_backspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER
    bne continue
    jsr pc, printEnter ; print ENTER
    jmp input_img_end ; end of Img input.

    continue:
    mov @#tkb, @#temp_word
    mov #temp_word, r0 ; r0 is used in printing func
    jsr pc, print_Char ; r0 is used here
    br inc1_tks

    cnt_function:
    movb @#tkb, @#temp_word ; address of the char we woudl like to print
    mov #temp_word, r0 ; r0 is used in printing func
    mov r5, tmp_wrd ; print_char may change r5.
    jsr pc, print_Char ; r0 is used here
    mov tmp_wrd, r5 ; giving the r5 value back.

    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq input_backspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER then ignore
    beq inc1_tks
    cmpb @#tkb, #40 ; if the char is SPACE then ignore
    beq inc1_tks
    cmpb @#tkb, #60 ; if ASCII code less than 60, ignore
    blo inc1_tks
    cmpb @#tkb, #146 ; if ASCII code more than 146, ignore
    bhi inc1_tks
                    ; the char is between [60,146]
    cmpb @#tkb, #71
    blos inputImg_legal_char ; char <=67, legal
    cmpb @#tkb, #141
    bhis inputImg_legal_char ; char >=141, legal
    jmp inc1_tks ;else illegal. ignore


    inputImg_legal_char:
    dec r2 ; one less cell is needed
    movb @#tkb, (r5) ; moving the char to the relevant img. 
    add #1, r5 ; incrementing r5 to the next byte
    jmp inc1_tks

    input_backspace_bytes:
    sub #1, r5   ; move to the previous byte
    add #1, r2 ;  1 MORE cell needed to get.
    movb #0, (r5) ; nullify the previous byte
    jmp inc1_tks

    input_img_end:
    jmp end_inputImage



    isEncOrDec:
        cmp @#EncDec, #1
        beq EncInput  
        jmp DecInput                                            ;else decode

    EncInput: ;~~~~ENC
    cmp @#numCells, #0 
    bne cntFunction 
    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq inputBackspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER
    bne cnt5
    jsr pc, printEnter ; print ENTER. END OF INPUT. we should start encrypt func
    jmp EncDone
    
    cnt5:
    mov @#tkb, @#temp_word
    mov #temp_word, r0 ; r0 is used in printing func
    jsr pc, print_Char ; r0 is used here
    jmp mainContinue

    cntFunction:
    movb @#tkb, @#temp_word ; address of the char we woudl like to print
    mov #temp_word, r0 ; r0 is used in printing func
    mov r5, tmp_wrd ; print_char may change r5.
    jsr pc, print_Char ; r0 is used here
    mov tmp_wrd, r5 ; giving the r5 value back.

    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq inputBackspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER then ignore
    beq goMainContinue
    cmpb @#tkb, #40 ; if the char is SPACE then ignore
    beq goMainContinue
    cmpb @#tkb, #60 ; if ASCII code less than 60, ignore
    blo goMainContinue
    cmpb @#tkb, #146 ; if ASCII code more than 146, ignore
    bhi goMainContinue
                    ; the char is between [60,146]
    cmpb @#tkb, #71
    blos LegalChar ; char <=67, legal
    cmpb @#tkb, #141
    bhis LegalChar ; char >=141, legal
    jmp goMainContinue ;else illegal. ignore


    inputBackspace_bytes:
    sub #1, @#ImgAddress     ; move to the previous byte
    add #1, @#numCells ;  1 MORE cell needed to get.
    mov @#ImgAddress, r5
    movb #0, (r5) ; nullify the previous byte
    jmp goMainContinue

    LegalChar:
    sub #1, @#numCells  ; one less cell is needed
    mov @#ImgAddress, r5
    movb @#tkb, (r5) ; moving the char to the relevant img. 
    add #1, @#ImgAddress     ; move to the next byte
    jmp goMainContinue

    goMainContinue:
    inc @#tks
    jmp mainContinue

DecInput: ;DEC
    cmp @#numCells, #0 
    bne cnt2Function 
    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq inputBackspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER
    bne cnt6
    jsr pc, printEnter ; print ENTER. END OF INPUT. we should start decrypt func
    jmp DecDone

    cnt6:
    mov @#tkb, @#temp_word
    mov #temp_word, r0 ; r0 is used in printing func
    jsr pc, print_Char ; r0 is used here
    jmp mainContinue

    cnt2Function:
    movb @#tkb, @#temp_word ; address of the char we woudl like to print
    mov #temp_word, r0 ; r0 is used in printing func
    mov r5, tmp_wrd ; print_char may change r5.
    jsr pc, print_Char ; r0 is used here
    mov tmp_wrd, r5 ; giving the r5 value back.

    cmpb @#tkb, #10 ; if the char is BACKSPACE
    beq inputBackspace_bytes
    cmpb @#tkb, #15 ; if the char is ENTER then ignore
    beq goMainContinue
    cmpb @#tkb, #40 ; if the char is SPACE then ignore
    beq goMainContinue
    cmpb @#tkb, #60 ; if ASCII code less than 60, ignore
    blo goMainContinue
    cmpb @#tkb, #146 ; if ASCII code more than 146, ignore
    bhi goMainContinue
                    ; the char is between [60,146]
    cmpb @#tkb, #71
    blos LegalChar ; char <=67, legal
    cmpb @#tkb, #141
    bhis LegalChar ; char >=141, legal
    jmp goMainContinue ;else illegal. ignore









;END OF INPUT INTERRUPT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;*handleCommand*

    char_is_space: ; then ignore it and move to the next char   <<<<<<<
        add #2, r2                                          ;      ^
        br handleCommand                                    ;      ^        
                            ;this is a helper func.                ^
                            ;written above the main func           ^
                            ;because of branch restrictions.       ^
handleCommand:                                              ;      |
    ;tst (r2);if the current char is 0(ascii) then the input label has finished
             ; and no legal command was found.
    ;beq input_error
    cmpb (r2), #40 ; if the char is space
    beq char_is_space
    mov r2, -(sp) ; push the address of the first char of the input line that
                  ; is NOT space. It should be the first char of the command
    mov #user_command, -(sp) ; pushing the address of the command label. The
                             ; command will be copied here.
    jsr pc, copyString 
    tst (sp)+   ; popping address that was used in the func
    tst (sp)+   ; popping address that was used in the func
    mov #user_command, r2 ; r2 is the address of the first char of the command
    ; r1 is the address of the first char AFTER the command.
    mov r1, @#tm_wrd 

    ;check_encode:
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_encode, -(sp) ; the address of the encode string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne checkDecode
    tst (sp)+ ; popping output
    jsr pc, pre_encode
    rts pc


    checkDecode:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_decode, -(sp) ; the address of the decode string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne check_crack
    tst (sp)+ ; popping output
    jsr pc, pre_decode
    rts pc


    check_crack:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_crack, -(sp) ; the address of the sethash string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne check_sethash
    tst (sp)+
    jsr pc, pre_crack
    rts pc


    check_sethash:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_sethash, -(sp) ; the address of the sethash string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne checkSetkey
    tst (sp)+ ; popping output
    jsr pc, PreSet_hash
    rts pc


    checkSetkey:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #stringSetkey, -(sp) ; the address of the setkey string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne check_print
    tst (sp)+ ; popping output
    jsr pc, pre_setkey
    rts pc


    check_print:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_print, -(sp) ; the address of the print string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    bne check_quit
    tst (sp)+ ; popping output
    jsr pc, pre_print
    rts pc


    check_quit:
    tst (sp)+ ; popping output
    mov #0, -(sp) ; the output of the compare function will be written here
    mov #user_command, -(sp) ; the address of the command label
    mov #string_quit, -(sp) ; the address of the setkey string
    jsr pc, compareStrings ; comparing strings
    tst (sp)+ ; popping address
    tst (sp)+ ; popping address
    cmpb (sp), #1 ; if the output is 1
    beq quit
    br illegal_input

    quit:
    jsr pc, printEnter
    mov #str_bye, -(sp)
    jsr pc, printStr
    tst (sp)+
    halt

    illegal_input:
    jsr pc, printEnter
    mov #strError, -(sp)
    jsr pc, printStr
    tst (sp)+
    mov #user_command, -(sp)
    jsr pc, PrntStr_words
    tst (sp)+
    mov #strDot, -(sp)
    jsr pc, printStr
    tst (sp)+
    tst (sp)+  ;pulling the output
    jsr pc, printEnter
    mov #0, isCommandRunning
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    rts pc


    PrntStr_words:; ret pc, address of string
    mov 2(sp), r0 ; r0 is the address of the first char of the string
    prntStr_loop: 
        cmpb (r0), #0
        beq finishPrint_str ; if the char is 0, then we printed the entire str.
        cmpb (r0), #40 ; if space then ignore and move on
        beq cnt
        jsr pc, print_Char ; printing the char
        cnt: add #2, r0 ; incrementing r0
        br prntStr_loop
    finishPrint_str:
    rts pc  


PreSet_hash:
    mov @#tm_wrd, r0 ; address of the first char of input AFTER the command
    mov #arg1, r1 ; r1 is the address of the parameter label(WORDS)

    cmpb (r0), #40 ; if the char is space
    bne ntSpace
    lpWhile_space:
    add #2, r0 ; increment to the next char
    cmpb (r0), #40; if the char is space
    beq lpWhile_space
    cmpb(r0), #0 ; if the 50 chars limit ended, halt. We shouldnt get here
    bne ntSpace
    halt

    ntSpace: ; (r0) is the first char of the parameter(HEXADICIMAL ASCII!)
    mov #20, r2 ; 16 decimal. this is the length of hash. this supposed to be
                ; the length of the parameter.
    loop9:
    mov (r0), (r1)
    add #2, r0 ; next char
    add #2, r1 ; next char
    sob r2, loop9

    prm_end: ; arg1 contains the parameter. it contains hex chars. we will cast
    ; it to octal values
    mov #arg1, r0
    mov #20, r2

    loop5:
        cmpb (r0), #71 ; ascii code is [60,71]
        blos minuSixty
        ;else ascii code is [141,146]
        sub #127,(r0) ; now the val [10,15]
        br endLoop5

        minuSixty:
        sub #60,(r0); now the val is [0,9]
        endLoop5: 
        add #2, r0
        sob r2, loop5
        

    end3:
    jsr pc, setHash1
    jsr pc, print_Time
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #0, @#isCommandRunning
    rts pc

setHash1: ; arg1 contains the new hash values.
    mov #Hash, r0
    mov #arg1, r1
    mov #20, r2 ;20 oct is 16 decimal. hash array contains 16 elements, 
    ; and therefore we will loop 16 times.
    loop6:
        movb (r1), (r0)
        add #2, r1 ; WORDS
        add #1, r0 ; BYTES
        sob r2, loop6 ;;;;;;~~~~~~~ perfect up to this point.
        ; at that point Hash contains the new elements. for the relevant 
        ; printings, we will return the arg1 values back to hex ascii.
    mov #20, -(sp)
    mov #arg1, -(sp)
    jsr pc, printEnter
    mov #strHash, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, cnvrt_hex_nums_to_hex_asciiPrint
    tst (sp)+
    tst (sp)+
    jsr pc, printEnter
    rts pc


cnvrt_hex_nums_to_hex_asciiPrint: ; stack: ret pc, address of the first num to be
                        ; converted, number of nums to be casted and printed
                        ; NOTE: WORDS ONLY

    mov 2(sp), r0 ; first num address
    mov 4(sp), r1 ; num of iterations
    loop7:
        cmpb (r0), #11 ; num [0,11]
        blos plusSixty
                       ; else num [12,17]
        add #127, (r0)
        lp7_prnt: jsr pc, print_Char
        add #2, r0
        sob r1, loop7
    rts pc



    plusSixty:
    add #60, (r0)
    br lp7_prnt




    



UpdateCrackKeySize: ;stack: return pc
    mov @#crkMaxKeyLength, r0
    mov #crackey, r1
    loop21: mov #-1, (r1)
            mov 2(r1), r1 ; move to next node

        sob r0, loop21
    ; the current node is the LAST ONE. We will initialize the NEXT NODE as 0,
    ; meaning that this is the last node.
    sub #2, r1
    mov #0, (r1)
    rts pc

    
    


    getThirdParameter: ; r0 is the address of the first char AFTER the second
    ; parameter.
    mov #arg3, r3 ; r3 is the address of the third parameter label(WORDS)
    cmpb (r0), #40 ; if the char is space
    bne nt_space
    lP_while_space:
    add #2, r0 ; increment to the next char
    cmpb (r0), #40; if the char is space
    beq lP_while_space
    cmpb(r0), #0 ; if the 50 chars limit ended, halt. We shouldnt get here
    bne nt_space
    halt

    nt_space: ; (r0) is the first char of the parameter(OCTAL!)
    mov (r0), (r3)
    add #2, r0 ; next char
    add #2, r3 ; next char
    cmpb(r0), #0 ; end of parameter
    beq done20

    ok20:
    cmpb (r0), #40 ; if the next char is space, then the copying of the first
    ; parameter has ended.
    beq done20
    br nt_space ; continue looping through the parameter.


    done20: ;arg3 is the third parameter in OCTAL asciis. we would like to cast
    ; it to number
    mov #arg3, r0 ; the address of the string we would like to convert to a num
    jsr pc, OctalStringToOctalNum
    ;now the first word of arg3 should be the number.
    rts pc



convertImgHexToOct: ; stack: return pc,return pc, cols, rows, img address
                    ; TREAT AS BYTES
    mov 4(sp), r1 ; number of cols
    mov 6(sp), r3 ; number of rows
    mov 10(sp), r5 ; image address
    mul r3, r1 ; r1 is the number of cells
    loop_img:
        cmpb (r5), #71 ; ascii code is [60,71]
        blos minusSixty
        ;else ascii code is [141,146]
        mov #0, r3
        movb (r5), r3 ; r3 is a helper register to handle bytes correctly.
        sub #127,r3 ; now the byte is [12,17]
        movb r3, (r5)
        br itr_end

        minusSixty:
        mov #0, r3
        movb (r5), r3 ; r3 is a helper register to handle bytes correctly.
        sub #60,r3; now the byte is [0,11]
        movb r3, (r5)

        itr_end: 
        add #1,r5 ; next byte

        sob r1, loop_img
    rts pc

;*compareStrings*


compareStrings: ; stack: pc return, string1, string2, answer
               ; string1 is from BYTES, string2 is from WORDS
    mov 2(sp), r1 ; r1 is the address of the first string.
    mov 4(sp), r2 ; r2 is the address of the second string.
    ;IMPORTANT NOTE: THE FIRST STRING IS OF **BYTES**
    ;                THE SECOND STRING IS OF **WORDS**
    compare_loop:
        cmpb (r1), #40 ; if the char is space, then the string has ended
        beq first_string_ended ; checking if the second string has ended as well
        cmpb (r2), #40 ; if the char is space, then the string has ended
        beq second_string_ended ;checking if the second string has ended as well
        cmpb (r1), (r2) ; if both strings aren`t sapce yet, then comparing them
        bne different_strings ; if different chars were found, then the strings
                                ; are different
        add #1, r1 ; incrementing r1
        add #2, r2 ; incrementing r2
        br compare_loop ; looping

    first_string_ended:
    cmp (r2), #40   ; when the first string ended, checking if the second ended
                    ; as well
    beq same_strings 
    br different_strings

    second_string_ended:
    cmpb (r1), #40  ; when the second string ended, checking if the first ended
                    ; as well
    beq same_strings
    br different_strings

    different_strings:
    movb #0, 6(sp) ; if the strings are different, then the output is 0
    rts pc

    same_strings:
    movb #1, 6(sp) ; if the strings are equal, the nthe output is 1
    rts pc


;1. the address of an image array located in 12(sp)
;2. the number of rows in the image located in 10(sp)
;3. the number of cols in the image located in 6(sp)
;4. the address of the Hash array located in 4(sp)
;5. the address of a Key list located in 2(sp)
;this function makes use of all the registers r0,r1,r2,r3,r4,r5
;this function updates the image located in 12(sp)



pre_decode:
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    jsr pc, printEnter ; print new line
    mov #str_decode_message, -(sp) ; relevant message printing
    jsr pc, printStr
    tst (sp)+ ; popping message`s address
    jsr pc, printEnter ; enter printing
    mov @#tm_wrd, r0 ; r0 is the address of the first char AFTER the command
    jsr pc, getTwoParameters
    mov #lastImgRows, r1
    mov #last_ImgCols, r2
    mov @#arg1, (r1)
    mov @#arg2, (r2)
    mov #IMG1, -(sp)    ; pushing the address of the image to whom we insert the
                        ; pixels the user types
    mov @#arg1, -(sp) ;rows
    mov @#arg2, -(sp) ;cols
        mov #user_input,r1 ; nullify user_input; used in the final cracking func
        loop55:
            cmpb (r1), #0
            beq cnt55
            mov #0, (r1)
            add #2, r1
            br loop55

    cnt55:
    cmp @#crackflag, #1 ; if we are cracking right now
    beq DecImgInput
    jsr pc, inputImage
    tst (sp)+ ; pop rows
    tst (sp)+ ; pop cols
    tst (sp)+ ; pop img1 address
    ; PREPATRING STACK FOR DECODE FUNCTION:
    mov #IMG1, -(sp)
    mov @#arg1, -(sp)
    mov @#arg2, -(sp)
    mov #Hash, -(sp)
    mov #key, -(sp)
    jsr pc, decode
                            ;now we woudl like to print the image
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #strDone_decode, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter
    mov @#arg2, -(sp)
    mov @#arg1, -(sp)
    mov #IMG1, -(sp)
    jsr pc, printImg
    tst (sp)+
    tst (sp)+
    tst (sp)+
    jsr pc, print_Time
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #0, @#isCommandRunning
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    rts pc



DecImgInput:
    mov #1, @#isCommandRunning
    mov @#arg1, @#tempRows ; arg1 will be destroyed in main
    mov @#arg2, @#tempCols ; arg2 will be destroyed in main


    jsr pc, clearRegisters ;clear all registers just for safety
    mov #0, @#EncDec ; flag for recognizing img input for enc purposes or dec.
    mov (sp), r1 ; number of cols
    mov 2(sp), r3 ; number of rows
    mov 4(sp), r5 ; image address
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mul r3, r1 ; r1 is the number of cells
    mov r1,r2 ; r2 is the number of cells
    mov #1, @#flag2 ; 1 for img input.
    mov r2, @#numCells ; global var.
    mov r5, ImgAddress ; global var.

    rts pc ; to main.

    DecDone: ; will get here after all img was filled
    ; PREPATRING STACK FOR ENCODE FUNCTION:
    jsr pc, ch_prio
    mov #IMG1, -(sp)
    mov @#tempRows, -(sp)
    mov @#tempCols, -(sp)
    mov #1, -(sp)  ; simulating second ret pc
    jsr pc, convertImgHexToOct
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #IMG1, -(sp)
    mov @#tempRows, -(sp)
    mov @#tempCols, -(sp)
    mov #Hash, -(sp)
    mov #key, -(sp)
    jsr pc, decode
                            ;now we woudl like to print the image
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    tst (sp)+
    mov #strDone_decode, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter
    mov @#tempCols, -(sp)
    mov @#tempRows, -(sp)
    mov #IMG1, -(sp)
    jsr pc, printImg
    tst (sp)+
    tst (sp)+
    tst (sp)+
    jsr pc, print_Time
    mov #0, @#timer_counter
    mov #0, @#isFirstNotSpace
    mov #0, isCommandRunning
    mov #IMG1, -(sp)
    jsr pc, ClearImage
    tst (sp)+
    mov #0, @#crackflag
    jmp mainContinue















print_Time: ; stack: ret pc, 
    clr r1
    clr r2
    clr r4
    clr r3
    mov @#timer_counter, r3
    div @#rate, r2 ;r2 has the quotient. r3 has the remainder
    mov r3, @#tmp_wrd ; r3 will be stored here
    mov #chars_counter, -(sp)
    mov #printing_time_chars, -(sp)
    mov r2, -(sp) ; first we cast the quotient to ascii decimals.
    jsr pc, castOctalNumToDecimalAscii ; after that, temp_word will contain
    ; the address of the last ascii decimal added to the array of asciis.
    tst (sp)+
    tst (sp)+
    tst (sp)+
    cmp @#crackprint, #1
    beq cnt51
    mov #user_command, -(sp)
    jsr pc, PrntStr_words
    tst (sp)+
    cnt51:
    mov #strTime, -(sp)
    jsr pc, printStr
    tst (sp)+
    mov @#chars_counter, r5             ; quantity of digits to print
    mov @#temp_word, r4  ;the address of the last ascii decimal added to the 
    ;array of asciis.   
    loop18:
    mov r4, r0
    jsr pc, print_Char
    sub #2, r4
        sob r5, loop18

    mov #strDot, -(sp)
    jsr pc, printStr
    tst (sp)+

            ;the quotient was printed. now, printing the decimal remainder

    mov #0, @#chars_counter
    mov #chars_counter, -(sp)
    mov #printing_time_chars, -(sp)
    mov @#tmp_wrd, r2
    mov r2, -(sp) ; the remainder we want to print
    jsr pc, castOctalNumToDecimalAscii
    tst (sp)+
    tst (sp)+
    tst (sp)+
    ; we are supposed to print just the two first chars of the remainder. 
    ; therefore, we will perform: number of printings = MAX{2,@#chars_counter}
    mov #2, r1
    mov @#temp_word, r4 ; last char added to teh array
    cmp r1, @#chars_counter
    bgt  ok1 ; if 2>chars_counter
    ;else
    mov #2, r1
    br loop20

    ok1: mov @#chars_counter, r1
    br loop20
    loop20:
    mov r4, r0
    jsr pc, print_Char
    sub #2, r4
    sob r1, loop20
    mov #strSec, -(sp)
    jsr pc, printStr
    tst (sp)+
    jsr pc, printEnter
    rts pc
    
castOctalNumToDecimalAscii: ; stack: pc return, octal num,words array to which
                              ; the asciis will be copied, address to word in 
                              ; which we will write the length of the array.
    mov 2(sp), r3 ; octal num
    mov 4(sp), r1 ; array
    mov 6(sp), r5 ; array length counter address
    clr r2

    loop14:
    div #12, r2 ; the ascii value of the decimal digit is in r3.
                ; r2 is the rest of the number.
    mov r3, (r1) ; moving the decimal digit to the array
    add  #60, (r1) ; adding 60 to the digit: now it is the ascii value.
    add #1, (r5); incrementing the array length counter
    cmp r2, #0 ; if the rest of the number is 0
    beq endlp14
    add #2, r1 ; moving to the next word of the array
    clr r3 ; neccecary for future div operations
    mov r2,r3 ; r3 has the rest of the number
    clr r2
    br loop14

    endlp14:; get here when r2 is 0, meaning that the number has been fully 
            ; casted to ascii decimals.
    mov r1, @#temp_word ; temp_word contains the address of the last ascii 
    ; decimal address that was added to the array.
    rts pc


pre_print: ; stack: return pc. IMG1 is what we would liek to print.
    jsr pc, printEnter
    mov @#lastImgRows, r1 ; num of rows
    mov @#last_ImgCols, r2 ; num of cols
    mov #IMG1, r3 ; img address

    looRows: ; moves through the rows, r1 times
        looCols: ; in each row, moves through the cols, r2 times
            cmpb (r3), #10
            bhis prntAstrix
                            ; else print space
            mov #tmp_wrd, r0
            mov #40, (r0)
            jsr pc, print_Char
            br iter_end

            prntAstrix:
            mov #tmp_wrd, r0
            mov #52, (r0)
            jsr pc, print_Char

            iter_end:
            add #1, r3 ; next pixel
            sob r2, looCols
        jsr pc, printEnter
        mov @#last_ImgCols, r2 ; num of cols
        sob r1, looRows
        jsr pc, print_Time
        mov #0, @#timer_counter
        mov #0, @#isFirstNotSpace
        mov #0, isCommandRunning
        rts pc



;*handleChar* stack parameters(from top to bottom): return pc.
; other parameters: r0 is the address of the char that the func is printing.
; the function uses the following registers: R0,R5
; the function prints the char that "sits" in R0 address, in hexadecimal base

handleCharOcToHex: ; stack: return pc. ; r0 is the address of the char 
                   ; to print(octal)
    cmpb (r0), #10
    blt charBetweenZeroAndSeven ; the element is between [0,7]
                                ; else, the element is between [10,17]
    cmpb (r0), #12
    blt char_Between_TenAndEleven ; the element is between [10,11]
                                ; else, the element is between [12,17]
    movb (r0), r5 ; the char we want to print is in r5. We dont want to 
                  ; change it`s acutal value(r0).
    add #127, r5  ; r5+127 = [141,146](octal), which is ASCII code of a-f
    br printCharOcToHex

    charBetweenZeroAndSeven:
        movb (r0), r5 ; the char we want to print is in r5. We dont want to 
                     ; change it`s acutal value(r0).
                     ;;NOTE;; the [0,7] value should be changed to the relevant
                     ; ASCII value. Therefore:
        add #60, r5  ; 60 is the ASCII value of 0. 67 is the ASCII value of 7.
                     ; the ASCII values are octal.
                     ; at that point we want to print the char that its ASCII
                     ; octal value sits in r5
        br printCharOcToHex

    char_Between_TenAndEleven:
        movb (r0), r5 ; the char we want to print is in r5. We dont want to 
                      ; change it`s acutal value(r0).
                      ; we could`ve managed these Elements in 
                      ; charBetweenZeroAndSeven. But in our way it makes more 
                      ; sense when working with octal
        add #60, r5   ; 70 is the ASCII value of 8. 71 is the ASCII value of 9
                      ; the ASCII values are octal.
                      ; at that point we want to print the char that its ASCII
                      ; octal value sits in r5
    printCharOcToHex: 
        movb r5, @#tpb ; the desired char to print moves to tpb
        w_ready: tstb @#tps 
        bpl w_ready ; if tps is positive, i.e. N flag=0, i.e. ready flag is 0, i.e.
                    ; the printing hasn`t finished, we will loop.
    
            rts pc

;*printEnter* stack parameters(from top to bottom): return pc.
; the function uses the following registers: R5
; the function prints enter(moves to the next line)

printEnter:
    movb #12, r5 ; 12 is the ASCII value of LF, first needed for enter printing
    movb r5, @#tpb ; printing LF
    w_ready1: tstb @#tps 
    bpl w_ready1 ; if tps is positive, i.e. N flag=0, i.e. ready flag is 0, i.e.
                ; the printing hasn`t finished, we will loop.
    movb #15, r5 ; 15 is the ASCII value of CR, second needed fo enter printing
    movb r5, @#tpb ; printing CR
    w_ready2: tstb @#tps 
    bpl w_ready2 ; if tps is positive, i.e. N flag=0, i.e. ready flag is 0, i.e.
                ; the printing hasn`t finished, we will loop.
    rts pc

;*printStr* stack parameters(from top to bottom): return pc, str address
;the func uses the following registers: R0(in helper func)
;the func uses the following helper funcs: print_Char
;the func prints a string that it`s address is in the bottom of the stack.
;the string should have 0 as the last char.
;NOTE: STRING OF BYTES

printStr: ; stack: return pc, str address
    mov 2(sp), r0 ; r0 is the address of the first char of the string
    print_str_loop: 
        tstb (r0)
        beq finish_print_str ; if the char is 0, then we printed the entire str. 
        jsr pc, print_Char ; printing the char
        add #1, r0 ; incrementing r0
        br print_str_loop
    finish_print_str:
    rts pc

;*print_Char* stack parameters(from top to bottom): return pc
;other parameters: R0 - address of the char we would like to print
;the func uses the following registers: R0
;the func prints the char that sits in R0(it`s address)

print_Char: ; stack: return pc. r0 - address of the char we print
    cmpb (r0), #15
    beq char_isEnter
    movb (r0), @#tpb ; printing the char
    w_ready3: tstb @#tps 
    bpl w_ready3 ; if tps is positive, i.e. N flag=0, i.e. ready flag is 0, i.e.
                ; the printing hasn`t finished, we will loop.
    r_t_s: rts pc
    char_isEnter:  ; if the char is ENTER print enter.
    ;NOTE: CAN GET HERE ONLY FROM  ProccessImgInput func.
        jsr pc, printEnter
        br r_t_s




OctalStringToOctalNum: ; r0 the address to the first char of the string
    mov r0, tmp_wrd ; the address of first char is saved here
    l_till_zero:
        cmpb (r0), #0 ; if the char is 0
        beq end_of_string
        add #2, r0 ; else move to the next char
        br l_till_zero

    end_of_string: ; r0 is the address of one char after the end of string
    sub #2, r0 ; now r0 is the address of the last char of the string.
    mov #0, r4 ; the number will be kept here
    mov #1, r3; the multiplier is here
    mov #0, r1 ; helper operand
    sub #60, (r0)
    add (r0), r4
    l_Till_first:
        cmp r0, tmp_wrd ; if true then it was the last char
        beq fin_func
        sub #2, r0 ; previous char
        mul #10, r3
        sub #60, (r0)
        mov (r0), r1
        mul r3, r1   ; mul wouldnt allow (r0). therefore we used r1 as a helper 
                        ;register
        mov r1, (r0)
        add (r0), r4
        br l_Till_first

    fin_func:
    mov #tmp_wrd, r1
    mov r4, @0(r1) ; r1 is used as a helper register because addressing mode 7
                   ; is allowed only while using registers.(@0(#tmp_word)
                                                                ; is illegal)
    rts pc

;*copyString*


copyString: ;stack: return pc, copy to address, copy from address
    ; BOTH STRINGS ARE OF WORDS
    mov 2(sp), r0 ; copy to address
    mov 4(sp), r1 ; copy from address
    loop_until_space_or_zero: ; its promised that the "copy from" string isn`t
    ; NULL or space
        movb (r1), (r0) ; copying the first char
        add #2, r0 ; incrementing(WORD) 
        add #2, r1 ; incrementing(WORD)
        cmp (r1), #40 ; if the "copy from" string`s char is space, then the 
        ; command has ended
        beq exit ;if so, the copying has finished
        tst (r1) ; if the "copy from" string` char is 0, then the string has 
        ; NOTE - that shouldn`t happen
        beq exit ; then exit
        br loop_until_space_or_zero ; looping


    exit:
    mov #40, (r0) ; when finished, adding space to the "copy to" string.
    rts pc

rti
outp:
rti

clock:
    cmpb #0, @#isFirstNotSpace
    beq ok30
    add #1, @#timer_counter
    ok30: 
    cmp @#crackflag , #1
    beq crackClock
    rti
    crackClock:
    add #1, crack_timer
    rti



;this function clears all the registers(r0,r1,r2,r3,r4,r5)
clearRegisters:
    clr r0
    clr r1
    clr r2
    clr r3
    clr r4
    clr r5
    rts pc ;return to caller

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~HW3 FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; The function checks if the img is legal. A legal image is an image that the range
; of its pixels is: [0,17]. The parameters should be transfered, using the inline
; technique, by the following order: image address, number of rows, number of cols.
; The output will be saved right after the number of cols. The output is 0 or 1: 1
; for legal image, 0 for illegal. R5 is also a parameter - it contains the address
; of the first input line - image address.
; The function uses the following registers: R1,R3,R4,R5
isLegal:
    mov (r5)+, r4 ; r4 is the ADDRESS of the first element of the img
    mov (r5)+, r3 ; r3 is the ADDRESS of numRows
    movb (r3), -(sp) ;move the val of numRows to the stack
    mov (sp)+, r3;mov the val of numRows to r3 and clear local stack var
    mov (r5)+, r1 ; r1 is the ADDRESS of numCols
    movb (r1), -(sp) ;move the val of numCols to the stack
    mov (sp)+, r1;mov the val of numCols to r1 and clear local stack var
    mul r3, r1 ;r1 is the number of cells
    loopImg: ;loops r1(number of cells in OrigImg) times
        cmpb (r4), #17
        bgt notOk ; if higher than 17
        cmpb (r4), #0
        blt notOk ; if lower than 0
        inc r4 ;go to next byte in img
        sob r1, loopImg 
    mov #1, @(r5) ; output '1' - ∀i,∀j : orgImg[i][j]∈[0,17].
    add #2, r5  ;move to the appropriate line after the function call
    rts r5 ;return to the caller

    notOk: 
        mov #0, @(r5) ; output '0' - ∃i,∃j : orgImg[i][j]∉[0,17].
        add #2, r5 ;move to the appropriate line after the function call
    rts r5 ;return to the caller



; The function receives a key and returns the next key in cyclical order. 
; The parameters in the stack should appear in the following order(from top to bottom):
; Return address. R1 should contain the address to the key(first node).
; The new key is returned by address in R1.
; The function uses the following registers: R1,R2.
nextKey:                     
    ; r1 is the address to the first node of the key.
    isLastNode:  
        tst 2(r1) ; is it the last node?
        beq lastNode
    
    nextKeyLoop: 
        tst (r1) ; if node value<0
        blt nodeLessThenZero
        cmp (r1), #17 ; if node value=17
        beq nodeIsSeventeen
        br incNode ;  nodeValue ∈[0,17), we need to increment its value.

    lastNode: 
        cmp (r1), #17
        beq noKey
        inc (r1) ; if the value of the last node is -1, it will be 0.
        rts pc   ; if the value of the last node ∈[0,17), it will increment.
                 ; the value of the last node would never be 17 because it
                 ; is promised that the correct key exists. So if the value 
                 ; IS 17, then we have a problem.

        noKey:
        jmp printNoKey ; if no key was found

    nodeLessThenZero: 
        mov #0, (r1) ; the value of the node becomes 0.
        rts pc

    nodeIsSeventeen:
        mov #0, (r1) ; the value of the node becomes 0.
        mov 2(r1), r2 ; r2 is the address of the next node.
        mov r2, r1 ; r1 is the address of the next node.
        br isLastNode

    incNode: 
        inc (r1) ; the value of node1 is incremented.
        rts pc


; The function compares img1 and img2. The parameters should be transfered in the stack 
; in the following order(from top to bottom): return address, number of cols, number
; of rows, address to img1, address to img2. The function compares each and every cell
; of the two-dimensional arrays pixel-by pixel. The images should have the same number
; of cols and rows. The output will be saved in the stack right after the last parameter,
; address to img2. The output is 0 or 1: 0 if the images are the same, Otherwise, if a 
; cell that isn`t identical in both images was found, the output is 1.
; The function uses the following registers: R1,R3,R5
cmpImg:
    mov 2(sp), r5 ;num of cols
    mov 4(sp), r3 ;num of rows
    mul r5, r3  ;number of cells
    loopBytes:
        cmpb @6(sp), @8(sp) ;comparing pixels
        bne notEqual        ;if they are different
        mov 6(sp), r1
        inc r1
        mov r1, 6(sp) ; moving to the next pixel of img 2
        mov 8(sp), r1
        inc r1
        mov r1, 8(sp) ; moving to the next pixel of img 1
        sob r3, loopBytes ;loop as many times as the number of cells in each image.

    indeedEqual:
        mov #0, 12(sp) ; 0 is the output for same images
        rts pc

    notEqual:
        mov #1, 12(sp) ; 1 is the output for different images
        rts pc

;this function copies a block row(4 bytes) from Output to a block row in OrigImg
;this function receives 2 variables from the register
;1. the address of the current location in the OrigImg array located in r2
;2. the address of the Output array located in r3
;this function makes use of the register r2,r3,r5
copyBlockRow:
    mov #4, r5 ;the width of a block is 4
    loopBlock:
        movb (r3), (r2) ;copy byte from output to OrigImg
        inc r3  ;go to next byte in Output
        inc r2  ;go to next byte in OrigImg
        sob r5, loopBlock ;loop 4 times(the width of a block)
    sub #4, r2  ;return r2 to its value before the function call
    sub #4, r3  ;return r3 to its value before the function call    
    rts pc ;return to caller

;this function move the value of the address in r3 directly to r3
;NOTE:this function is placed before the function "out" on purpose the 
;function will return to the caller
moveKey:
    mov (r3), r3 ;move key value to r3

;this function returns to the caller using "rts pc"
out:
    rts pc ;return to the caller

;this function receives a key between 0 and 17(octal) (in r3)
;and an address of an element(in r2) and does "xor r3, (r2)"
;if r3 is negative then the function does nothing
encryptByKey:
    tst r3 ;check if r3 is negative
    bmi out ;if r3 is negative exit the function
    ;apply the key(r3) on the element(r2)
    movb (r2), -(sp) ;moving to stack to avoid an ODD WORD ADDRESS error
    xor r3, (sp) ;apply the key using xor
    movb (sp), (r2) ;move the xor result to r2
    add #2, sp ;clear local variable

    rts pc ;return to caller

;this function calculates the appropriate key for an element in OrigImg
;and stores the key value into r3
;this function receives 3 variables from the stack
;1. the current row in the OrigImg located in 6(sp)
;2. the current col in the OrigImg located in 4(sp)
;3. an address of the Key list located in 2(sp)
;this function makes use of the registers r0,r1,r3
GetKey:
    mov 2(sp), r3 ;r3 will contain the key index

    clr r0 ;clear r0 for safety
    clr r1 ;clear r1 for safety
    mov 4(sp), r1 ;the col number is stored in r1
    div #4, r0 ;divide r1 by 4(the block width) and store the remainder in r1
    mov r1, -(sp) ;store the current column in the current block location

    clr r0 ;clear r0 for safety
    clr r1 ;clear r1 for safety
    mov 10(sp), r1 ;the row number is stored in r1
    div #4, r0 ;divide r1 by 4(the block width) and store the remainder in r1
    mul #4, r1 ;multiply the current row number by 4(the block width)
    add r1, (sp) ;(sp) will equal the current position in the block

    mov (sp), r0 ;store (sp) in r0
    add #2, sp ;clear local variable

    ;if r0=0 the first key will be activated on the current element in the img
    tst r0
    beq moveKey 

    ;loop over the key list until the appropriate key is found for the element
    loopKey:
        mov 2(r3), r3 ;move to next node in the key list
        ;resetKey if at last node
        mov 2(sp), -(sp) ;send the key address to "resetKey"
        jsr pc, resetKey ;resetKey if the end of the key list was reached
        add #2, sp ;clear local variable
        sob r0, loopKey ;loop unti r0 is 0

    br moveKey ;set the appropriate key in r3

;this function receives an address of a node in the Key list in r3
;this function receives an address of the first node in the key list in 2(sp)
;if r3=0 or (r3)=-1 the r3 will be set to the address of the first node in the
;key list else this function does nothing
resetKey:
    tst r3 ;check if r3 is 0
    beq keyStart ;if r3 is set to an invalid node address(0) then reset the key
    tstb (r3) ;check if (r3) is smaller then 0
    blt keyStart ;if the val of the word in the node is invalid(-1) then reset the key
    rts pc ;return to caller
    keyStart:
    mov 2(sp), r3 ;reset r3 to the first node in the key list
    rts pc ;return to caller

;this function moves r2 from the rightmost column in a row in a block to the
;leftmost column in the row below in the block
;this function makes use of the registers(r0,r2,r5)
;this function receives 1 parameter from the stack
;1. the number of cols in OrigImg located in 2(sp)
moveRowDownInBlock:
    cmpb r0, r5 ;if in last row of the block then r0=r5
    beq out ;no need to move row down if in last row
    clr r5 ;clear r5 for safety
    movb 2(sp), r5 ;move nCols to r5
    sub #4, r5 ;subtract 4(the block width) from r5(nCols)
    add r5, r2 ;move r2 to the next row in OrigImg
    rts pc ;return to caller

;this function moves r2 from the end of the block(the right bottom corner)
;to the beginning of the next block(the top right corner)
;the next block is the next block from right
;this function makes use of the registers(r2,r5)
;this function receives 2 parameters from the stack
;1. the number of cols in OrigImg located in 2(sp)
;2. the current block col located in 4(sp)
moveToNextBlock:
    cmpb 2(sp), 4(sp) ;if in the last block of the row of blocks
    beq out;no need to move to the next block if in last block
    clr r5
    movb 2(sp), r5
    mul #3, r5
    sub r5, r2
    rts pc

;this function uses a Key list to encrypt an image
;this function receives 4 parameters
;1. the address of an image array located in 10(sp)
;2. the number of rows in the image located in 6(sp)
;3. the number of cols in the image located in 4(sp)
;4. the address of a Key list located in 2(sp)
;this function makes use of all the registers r0,r1,r2,r3,r4,r5
keyCypher:
    jsr pc, clearRegisters ;clear all registers just for safety
    mov 10(sp), r2 ;r2 will contain an address of an element in OrigImg
    ;r3 will contain the key value
    mov 6(sp), r4 ;nRows
    mov 4(sp), r5 ;nCols
    
    loopRow: ;loops over all the blocks row by row
        mov 4(sp), r5 ;nCols
        loopCol: ;loops over all the blocks column by column
            mov 6(sp), -(sp) ;nRows
            sub r4, (sp) ;current row
            mov 6(sp), -(sp) ;nCols
            sub r5, (sp) ;current col
            mov 6(sp), -(sp) ;first key
            jsr pc, GetKey
            add #6, sp ;clear the local variables
            jsr pc, encryptByKey
            inc r2
        sob r5, loopCol ;if looped over all cols in the row then exit loop
    sob r4, loopRow ;if looped over all rows in the image then exit loop
    rts pc ;return to caller

;this function encodes an image(in 12(sp)) using a Key list and a Hash array
;this function receives 5 parameters
;1. the address of an image array located in 12(sp)
;2. the number of rows in the image located in 10(sp)
;3. the number of cols in the image located in 6(sp)
;4. the address of the Hash array located in 4(sp)
;5. the address of a Key list located in 2(sp)
;this function makes use of all the registers r0,r1,r2,r3,r4,r5
;this function updates the image located in 12(sp)
encode:
    mov 12(sp), -(sp) ;move OrigImg into the stack
    mov 12(sp), -(sp) ;move nRows into the stack
    mov 12(sp), -(sp) ;move nCols into the stack
    mov 10(sp), -(sp) ;move the Key into the stack
    jsr pc, keyCypher ;encrypt the image using the key
    add #10, sp ;clear all the parameters

    jsr pc, clearRegisters ;clear all registers just for safety
    mov #0, -(sp) ;the row in the current block
    mov #0, -(sp) ;the column in the current block
    mov #4, -(sp) ;number of columns in a block
    mov #4, -(sp) ;number of rows in a block
    mov 22(sp), r2 ;r2 will always contain an address of an element in OrigImg
    mov #Output, r3 ;r3 will contain an address of an element in Output
    mov 14(sp), r4 ;r4 will always contain an address of an element in Hash
    loop1: ;loops over all the blocks row by row
        clr 4(sp) ;clear the current block column
        loop2: ;loops over all the blocks column by column
            movb 6(sp), r0; set r0 to the first row in the block
            loop3: ;loops over all the rows in the block
                movb 4(sp), r1 ;set r1 to the first column in the block
                loop4: ;loops over all the columns in the block
                    ;prepare parameters in stack for the function shiftAndCypher
                    mov 14(sp), -(sp) ;mov the Hash address to stack
                    mov 10(sp), -(sp) ;mov the row in the current block to stack
                    mov 10(sp), -(sp) ;mov the col in the current block to stack
                    jsr pc, shiftAndCypherEncode
                    add #6, sp;clear local parameters created for shiftAndCypher
                    ;increment r1, r2, r3
                    inc r3 ;move to the next element in the "Output" array
                    inc r2 ;move to the next element in the "OrigImg" array
                    inc r1 ;move to the next column in the block

                    ;check if last column was reached
                    mov 2(sp), r5 ;move the block height(4) to r5
                    add 4(sp), r5 ;add the current block column to r5
                    cmpb r1, r5 ;if last col in the block was reached then r1=r5
                    bne loop4 ;if the last col in the block was reached end the loop 

                ;copy the row in Output to the current block row in OrigImg
                mov #Output, r3 ;r3 will contain an address of an element in Output
                sub #4, r2 ;go to the first element in the current block row in OrigImg
                jsr pc, copyBlockRow ;copy the block row in Output to OrigImg
                add #4, r2 ;go back to the current location in OrigImg

                add #1, r0 ;increment r0 and move to the next row in the block 
                mov #4, r5 ;add 4(the block width) to r5
                add 6(sp), r5 ;add the current block row to r5
                ;if not in the last row of the block then move one row down
                mov r5, -(sp) ;save r5 because it will change in "moveRowDownInBlock"
                mov 20(sp), -(sp) ;move nCols to stack
                jsr pc, moveRowDownInBlock
                add #2, sp ;clear local variable created for "moveRowDownInBlock"
                mov (sp)+, r5 ;restor r5 to its val before "moveRowDownInBlock"
                cmpb r0, r5 ;if in last row of the block then r0=r5
                bne loop3 ;if the last row in the block was reached end the loop

            add #4, 4(sp) ;add 4 to the current block column
            ;if not in the last block of the row then move to the next right block
            mov 4(sp), -(sp) ;add the current block col to stack
            mov 20(sp), -(sp) ;move nCols to stack
            jsr pc, moveToNextBlock
            add #4, sp ;clear local variable created for moveRowDownInBlock
            cmp 16(sp), 4(sp) ;if in the last block of the row of blocks
            bne loop2 ;if the last block in the row of blocks was reached end loop

        add #4, 6(sp) ;add 4(the block height) to the current block row
        ;if the last block was reached(bottom right block) then end loop
        cmp 20(sp), 6(sp) ;compare nRows to current block row
        bne loop1
    
    add #10, sp ;clear all local variables used in the loops
    mov 12(sp), -(sp) ;move OrigImg into the stack
    mov 12(sp), -(sp) ;move nRows into the stack
    mov 12(sp), -(sp) ;move nCols into the stack
    mov 10(sp), -(sp) ;move the Key into the stack
    jsr pc, keyCypher
    add #10, sp ;clear all the parameters used in "keyCypher"
    rts pc ;return to caller

;this function moves an element from the array OrigImg(located in r2)
;to the array Output(located in r3)
;the function will move the element to them same block in the
;array Output and will shift it to the right r0%4 times(when r0 is the number
;of the row that the elemnt is located in). if the element is shifted out of
;the block then the element will just cycle through the row in the block
;the element will be then encrypted according to the Hash array
;this function makes use of all registers(r0-r5)
;this function receives 3 parameters from the stack
;1. the Hash address located in 6(sp)
;2. the current row in the block located in 4(sp)
;3. the current col in the block located in 2(sp)
shiftAndCypherEncode:
    mov 4(sp), -(sp) ;mov the row in the current block to stack
    mov 4(sp), -(sp) ;mov the col in the current block to stack
    jsr pc, shiftRight ;shift the element to the right
    add #4, sp ;clear the local parameters created for shiftRight
    tst -(sp) ;temp variable that will be used
    mov r5, (sp) ;mov r5 to temp because r5 will change in the cypher functions
    mov 10(sp), -(sp) ;mov the Hash address to stack
    jsr pc, encrypt ;encrypt the element
    sub 2(sp), r3 ;change r3 to its original value at the start of this function
    add #4, sp    ;clean the local variables used in the encrypt function
    rts pc ;return to caller

;this function is called by shiftRight when an elemnet was shifted out of the
;block to the right. this function will fix that shift by subtracting 4 from
;the elements shift(r5) so the element will cycle in the block row
;this function makes use of r5
circleRight:
    sub #4, r5 ;subtract 4(the block width) from r5
    br right_done

;this function moves an element from the array OrigImg(located in r2)
;to the array Output(located in r3) to the same location plus a shift to the
;right. The shift will equal r0%4(r0 is the number of the row that the element
;is located in). if the element is shifted out of the block then the
;element will just cycle through the row in the block
;this function makes use of the registers(r0,r1,r2,r3,r5)
;this function receives 2 parameters from the stack
;1. the current row in the block located in 4(sp)
;2. the current col in the block located in 2(sp)
shiftRight:
    ;r5 is the amount needed to shift right
    clr r5 ;set r5 to 0
    ;these two lines will add r0%4(the number of the row in the block) to r5
    add r0, r5
    sub 4(sp), r5 ;move block row to r5
    ;these two lines will add r1%4(the number of the column in the block) to r5
    add r1, r5
    sub 2(sp), r5 ;move block col to r5

    ;if the element will shift out of the block then cycle the element
    cmp #4, r5
    ble circleRight
    right_done: ;return label for the function circleRight

    ;these two lines will remove r1%4(the number of the col in the block) to r5
    add 2(sp), r5 ;move block col to r5
    sub r1, r5

    add r5, r3 ;add the shift to the address of the element in r3
    movb (r2), (r3) ;move the element from r2 to its shifted place in r3 
    rts pc ;return to caller

;this function is called by shiftLeft when an elemnet was shifted out of the
;block to the left. this function will fix that shift by adding 4 to
;the elements shift(r5) so the element will cycle in the block row
;this function makes use of r5
circleLeft:
    add #4, r5 ;add 4(the block width) from r5
    br left_done

;this function moves an element from the array OrigImg(located in r2)
;to the array Output(located in r3) to the same location plus a shift to the
;left. The shift will equal r0%4(r0 is the number of the row that the element
;is located in). if the element is shifted out of the block then the
;element will just cycle through the row in the block
;this function makes use of the registers(r0,r1,r2,r3,r5)
;this function receives 2 parameters from the stack
;1. the current row in the block located in 4(sp)
;2. the current col in the block located in 2(sp)
shiftLeft:
    ;r5 is the amount needed to shift left
    clr r5 ;set r5 to 0
    ;these two lines will add r0%4(the number of the row in the block) to r5
    sub r0, r5
    add 4(sp), r5 ;move block row to r5
    ;these two lines will add r1%4(the number of the column in the block) to r5
    add r1, r5
    sub 2(sp), r5 ;move block col to r5

    ;if the element will shift out of the block then cycle the element
    tst r5
    blt circleLeft
    left_done: ;return label for the function circleLeft

    ;these two lines will remove r1%4(the number of the col in the block) to r5
    add 2(sp), r5 ;move block col to r5
    sub r1, r5

    add r5, r3 ;add the shift to the address of the element in r3
    movb (r2), (r3) ;move the element from r2 to its shifted place in r3 
    rts pc ;return to caller

;this function decodes an image(in 12(sp)) using a Key list and a Hash array
;this function receives 5 parameters
;1. the address of an image array located in 12(sp)
;2. the number of rows in the image located in 10(sp)
;3. the number of cols in the image located in 6(sp)
;4. the address of the Hash array located in 4(sp)
;5. the address of a Key list located in 2(sp)
;this function makes use of all the registers r0,r1,r2,r3,r4,r5
;this function updates the image located in 12(sp)
decode:
    mov 12(sp), -(sp) ;move OrigImg into the stack
    mov 12(sp), -(sp) ;move nRows into the stack
    mov 12(sp), -(sp) ;move nCols into the stack
    mov 10(sp), -(sp) ;move the Key into the stack
    jsr pc, keyCypher ;encrypt the image using the key
    add #10, sp ;clear all the parameters

    jsr pc, clearRegisters ;clear all registers just for safety
    mov #0, -(sp) ;the row in the current block
    mov #0, -(sp) ;the column in the current block
    mov #4, -(sp) ;number of columns in a block
    mov #4, -(sp) ;number of rows in a block
    mov 22(sp), r2 ;r2 will always contain an address of an element in OrigImg
    mov #Output, r3 ;r3 will contain an address of an element in Output
    mov 14(sp), r4 ;r4 will always contain an address of an element in Hash
    loop1Decode: ;loops over all the blocks row by row
        clr 4(sp) ;clear the current block column
        loop2Decode: ;loops over all the blocks column by column
            movb 6(sp), r0; set r0 to the first row in the block
            loop3Decode: ;loops over all the rows in the block
                movb 4(sp), r1 ;set r1 to the first column in the block
                loop4Decode: ;loops over all the columns in the block
                    ;prepare parameters in stack for the function shiftAndCypher
                    mov 14(sp), -(sp) ;mov the Hash address to stack
                    mov 10(sp), -(sp) ;mov the row in the current block to stack
                    mov 10(sp), -(sp) ;mov the col in the current block to stack
                    jsr pc, shift_and_cypher_decode
                    add #6, sp;clear local parameters created for shiftAndCypher
                    ;increment r1, r2, r3
                    inc r3 ;move to the next element in the "Output" array
                    inc r2 ;move to the next element in the "OrigImg" array
                    inc r1 ;move to the next column in the block

                    ;check if last column was reached
                    mov 2(sp), r5 ;move the block height(4) to r5
                    add 4(sp), r5 ;add the current block column to r5
                    cmpb r1, r5 ;if last col in the block was reached then r1=r5
                    bne loop4Decode ;if the last col in the block was reached end the loop 

                ;copy the row in Output to the current block row in OrigImg
                mov #Output, r3 ;r3 will contain an address of an element in Output
                sub #4, r2 ;go to the first element in the current block row in OrigImg
                jsr pc, copyBlockRow ;copy the block row in Output to OrigImg
                add #4, r2 ;go back to the current location in OrigImg

                add #1, r0 ;increment r0 and move to the next row in the block 
                mov #4, r5 ;add 4(the block width) to r5
                add 6(sp), r5 ;add the current block row to r5
                ;if not in the last row of the block then move one row down
                mov r5, -(sp) ;save r5 because it will change in "moveRowDownInBlock"
                mov 20(sp), -(sp) ;move nCols to stack
                jsr pc, moveRowDownInBlock
                add #2, sp ;clear local variable created for "moveRowDownInBlock"
                mov (sp)+, r5 ;restor r5 to its val before "moveRowDownInBlock"
                cmpb r0, r5 ;if in last row of the block then r0=r5
                bne loop3Decode ;if the last row in the block was reached end the loop

            add #4, 4(sp) ;add 4 to the current block column
            ;if not in the last block of the row then move to the next right block
            mov 4(sp), -(sp) ;add the current block col to stack
            mov 20(sp), -(sp) ;move nCols to stack
            jsr pc, moveToNextBlock
            add #4, sp ;clear local variable created for moveRowDownInBlock
            cmp 16(sp), 4(sp) ;if in the last block of the row of blocks
            bne loop2Decode ;if the last block in the row of blocks was reached end loop

        add #4, 6(sp) ;add 4(the block height) to the current block row
        ;if the last block was reached(bottom right block) then end loop
        cmp 20(sp), 6(sp)
        bne loop1Decode
    
    add #10, sp ;clear all local variables used in the loops
    mov 12(sp), -(sp) ;move OrigImg into the stack
    mov 12(sp), -(sp) ;move nRows into the stack
    mov 12(sp), -(sp) ;move nCols into the stack
    mov 10(sp), -(sp) ;move the Key into the stack
    jsr pc, keyCypher
    add #10, sp ;clear all the parameters used in "keyCypher"
    rts pc ;return to caller

;this function moves an element from the array OrigImg(located in r2)
;to the array Output(located in r3)
;the function will move the element to them same block in the
;array Output and will shift it to the left r0%4 times(when r0 is the number
;of the row that the elemnt is located in). if the element is shifted out of
;the block then the element will just cycle through the row in the block
;the element will be then decrypted according to the Hash array
;this function makes use of all registers(r0-r5)
;this function receives 3 parameters from the stack
;1. the Hash address located in 6(sp)
;2. the current row in the block located in 4(sp)
;3. the current col in the block located in 2(sp)
shift_and_cypher_decode:
    mov 4(sp), -(sp) ;mov the row in the current block to stack
    mov 4(sp), -(sp) ;mov the col in the current block to stack
    jsr pc, shiftLeft ;shift the elemnt to the right
    add #4, sp ;clear the local parameters created for shiftRight
    tst -(sp) ;temp variable that will be used
    mov r5, (sp) ;mov r5 to temp because r5 will change in the cypher functions
    mov 10(sp), -(sp) ;mov the Hash address to stack
    jsr pc, decrypt ;encrypt the element
    sub 2(sp), r3 ;change r3 to its original value at the start of this function
    add #4, sp    ;clean the local variables used in the encrypt function
    rts pc ;return to caller

;this function decrypts an element in r3 to the value in r5
;this function makes use of the registers(r3,r4,r5)
;this function receives 1 parameter from the stack
;1. the address of the hash array located in 2(sp)
decryptNum:
    movb r5, (r3) ;decrypt r3
    mov 2(sp), r4 ;reset r4 to point to the "Hash" array
    rts pc

;this function encrpyts an element in r3 according to the "Hash" array
;for example: if the element equals 2 and the element with index 2 in the "Hash"
;array equals 7, then the element in r3 will be encrypted to 7
;this function makes use of the registers(r3,r4,r5)
;this function receives 1 parameter from the stack
;1. the address of the hash array located in 2(sp)
encrypt:
    clr r5
    ;using movb to avoid an ODD WORD ADDRESS error when we add
    movb (r3), r5
    add r5, r4

    movb (r4), (r3) ;moves the encrypted value to r3
    mov 2(sp), r4 ;reset r4 to point to the "Hash" array
    rts pc

;this function decrypts an element in r3 according to the "Hash" array
;for example: if the element equals 2 and the element that equals 2 in the
;"Hash" array has an index that equals 7, then the element in r3 will be
;encrypted to the value of the 7th element in the "Hash" array
;this function makes use of the registers(r3,r4,r5)
;this function receives 1 parameter from the stack
;1. the address of the hash array located in 2(sp)
decrypt:
    clr r5
    loopDecrypt:
        cmpb (r3), (r4) ;is r3 equal to r4
        beq decryptNum ;insert the decrypted element into r3
        inc r4 ;go to the next array element
        inc r5 ;increment r5
        br loopDecrypt







.= torg + 26000
user_input: .blkw 62 ; 50(decimal) words for 50 char inputs
            .byte 0
            .even
user_command: .blkw 62 ; 50(decimal) words for 50 char inputs
            .byte 0
str_welcome: .ascii<Welcome!>
             .byte 0
             .even
str_encode_message: .ascii<Please enter image to encode>
            .byte 0
            .even
strCrack1: .ascii<Please enter image>
            .byte 0
            .even
sCrack2: .ascii<Please enter encoded image>
            .byte 0
            .even
str_decode_message: .ascii<Please enter image to decode>
            .byte 0
            .even
endedCracking: .word 0
before_command: .ascii<$ >
                .byte 0
                .even
strSec: .ascii<sec>
.byte 0
.even
string_encode: .ascii<encode >
.even
string_decode: .ascii<decode > 
.even
string_sethash: .ascii<setHash >
.even
stringSetkey: .ascii<setKey >
.even
string_print: .ascii<print >
.even
string_crack: .ascii<crack >
.even
string_quit: .ascii<quit >
.even
str_bye: .ascii<Goodbye!>
.byte 0
.even
NoFoundKey: .ascii<The key is: -1>
.byte 0
.even
str_done_encode: .ascii<The encoded image is:>
.byte 0
.even
strDone_decode: .ascii<The decoded image is:>
.byte 0
.even
strError: .ascii<Unknown command >
.byte 0
.even
strHash: .ascii<Hash function set to >
.byte 0
.even
strKey: .ascii<Encryption key set to >
.byte 0
.even
strTime: .ascii< completed in >
.byte 0
.even
stCrack3: .ascii<crack>
.byte 0 ; for the final cracking func
.even
crack_timer: .word 0
saveR0: .word 0
saveR1: .word 0
saveR2: .word 0
saveR3: .word 0
saveR4: .word 0
saveR5: .word 0
strDot: .ascii<.>
.byte 0
.even
Hash: .byte 0,1,2,3,4,5,6,7,10,11,12,13,14,15,16,17
.even
startedCracking: .word 0
temp_word: .word 0
tmp_wrd: .word 0
tm_wrd: .word 0
tmpWrd: .word 0
arg1: .blkw 62
      .byte 0
      .even
arg2: .blkw 62
      .byte 0
      .even
arg3: .blkw 62
      .byte 0
.even
crackflag: .word 0
flag2: .word 0
printing_time_chars: .blkw 20
chars_counter: .word 0
lastImgRows: .word 0
last_ImgCols: .word 0
tempTimer: .word 0
isFirstNotSpace: .word 0
timer_counter: .word 0
crkRows: .word 0
crkMaxKeyLength: .word 0
crkCols: .word 0
Output:         .blkw 2;this static parameter will be used in encode/decode
rate:   .word   1000.
numCells: .word 0
EncDec: .word 0
tempRows: .word 0
crackprint: .word 0
tempCols: .word 0
ImgAddress: .word 0
crack1Completed: .ascii<The key is:>
.byte 0
.even
crack2Completed: .ascii<crack completed in >
.byte 0
.even

key:
node1:      .word 0,  node2
node2:      .word -1, node3
node3:      .word -1, node4
node4:      .word -1, node5
node5:      .word -1, node6
node6:      .word -1, node7
node7:      .word -1, node8
node8:      .word -1, node9
node9:      .word -1, node10
node10:     .word -1, node11
node11:     .word -1, node12
node12:     .word -1, node13
node13:     .word -1, node14
node14:     .word -1, node15
node15:     .word -1, node16
node16:     .word -1, 0
.even
.word 0
isCommandRunning: .word 0

crackey:
cnode1:     .word 0,  cnode2
cnode2:     .word -1, cnode3
cnode3:     .word -1, cnode4
cnode4:     .word -1, cnode5
cnode5:     .word -1, cnode6
cnode6:     .word -1, cnode7
cnode7:     .word -1, cnode8
cnode8:     .word -1, cnode9
cnode9:     .word -1, cnode10
cnode10:    .word -1, cnode11
cnode11:    .word -1, cnode12
cnode12:    .word -1, cnode13
cnode13:    .word -1, cnode14
cnode14:    .word -1, cnode15
cnode15:    .word -1, cnode16
cnode16:    .word -1, 0


.even
IMG1:.blkw  1000
IMG2:.blkw  1000
IMG3:.blkw  1000