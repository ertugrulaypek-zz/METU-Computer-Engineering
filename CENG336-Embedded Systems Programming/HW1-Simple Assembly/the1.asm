; When you press and relase the RB0 then the four top most LEDs connected to PORTC will be turned on
; Then when you press the RB0 the other four LEDs connected to PORTC  will be turned on
; To execute this program The jumper J13 should be ground side

LIST    P=18F8722

#INCLUDE <p18f8722.inc> 
    
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

 re3_pressed        udata 0X20
 re3_pressed
 re3_stopped  udata 0X21
 re3_stopped
 re4_stopped  udata 0X22
 re4_stopped
 stopped	 udata 0X23
 stopped
 ra4_pressed udata 0x24
 ra4_pressed
 re4_pressed udata 0x25
 re4_pressed
 
 UDATA_ACS
  t1	res 1	; used in delay
  t2	res 1	; used in delay
  t3	res 1	; used in delay

ORG     0x00
goto    main

 
 
DELAY	; Time Delay Routine with 3 nested loops
    MOVLW 81	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0x9B  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0xA5	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return
		
init
 movlw d'0'
 movwf ra4_pressed
 movwf stopped
 movwf re3_pressed
 movwf re3_stopped
 movwf re4_pressed
 movwf re4_stopped
 
 MOVLW 0Fh ; Configure A/D
 MOVWF ADCON1
 movlw b'00010000'
 movwf TRISA
 clrf LATA
 clrf PORTA
 
 movlw d'0'
 movwf TRISB
 clrf LATB
 clrf PORTB
 
 movlw d'0'
 movwf TRISC
 clrf LATC
 clrf PORTC
 
 movlw d'0'
 movwf TRISD
 clrf LATD
 clrf PORTD
 
 movlw b'00011000'
 movwf TRISE
 clrf LATE
 clrf PORTE
 
return
 
ra4_pressed_func
 movlw d'1'
 movwf ra4_pressed
 return

 check_if_ra4_released
 BTFSS ra4_pressed,0
 return
 movlw d'1'
 movwf stopped
 clrf ra4_pressed
 return
 
 
 
 
DELAY_checkButton
 MOVLW 10	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop33:
	MOVLW 0xA0  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop22:
	    MOVLW 0x89	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop11:
		BTFSC PORTA,4 ; skip if ra4 is clear
		call ra4_pressed_func
		BTFSS PORTA,4 ; skip if ra4 is set
		call check_if_ra4_released
		BTFSC stopped,0
		return
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop11 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop22 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop33 ; ELSE Keep counting down
		return
state1
;lights on
 movlw h'0f' 
 movwf LATA
 movwf LATB
 movwf LATC
 movwf LATD
 
 ;delay goes here for 2 secs
 call DELAY
 call DELAY
 
 ;lights off
 clrf LATA
 clrf LATB
 clrf LATC
 clrf LATD
 
 ;delay goes here for 1 sec
 call DELAY
 goto state1_ra0
 
state1_ra0:
 movlw d'1'
 movwf LATA
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_ra1
 goto state2_ra0
 
 
state1_ra1:
 movlw b'00000011'
 movwf LATA
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_ra2
 goto state2_ra1
state1_ra2:
 movlw b'00000111'
 movwf LATA
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_ra3
 goto state2_ra2
 

state1_ra3:
 movlw b'00001111'
 movwf LATA
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rb0
 goto state2_ra3
 
 
state1_rb0:
 movlw b'00000001'
 movwf LATB
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rb1
 goto state2_rb0
 
 
state1_rb1:
 movlw b'00000011'
 movwf LATB
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rb2
 goto state2_rb1
 
 
state1_rb2:
 movlw b'00000111'
 movwf LATB
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rb3
 goto state2_rb2
 
 
state1_rb3:
 movlw b'00001111'
 movwf LATB
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rc0
 goto state2_rb3
 
 
state1_rc0:
 movlw b'00000001'
 movwf LATC
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rc1
 goto state2_rc0
 

 state1_rc1:
 movlw b'00000011'
 movwf LATC
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rc2
 goto state2_rc1
 
 
 state1_rc2:
 movlw b'00000111'
 movwf LATC
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rc3
 goto state2_rc2
 
 
 state1_rc3:
 movlw b'00001111'
 movwf LATC
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rd0
 goto state2_rc3
 
 
 state1_rd0:
 movlw b'00000001'
 movwf LATD
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rd1
 goto state2_rd0
 
 
 state1_rd1:
 movlw b'00000011'
 movwf LATD
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rd2
 goto state2_rd1
 
 
 state1_rd2:
 movlw b'00000111'
 movwf LATD
 call clean_re_vars
 call DELAY_checkButton
 BTFSS stopped, 0
 goto state1_rd3
 goto state2_rd2
 
 
 state1_rd3:
 movlw b'00001111'
 movwf LATD
 call clean_re_vars
 call DELAY_checkButton
 BTFSC stopped,0
 goto state2_rd3 ; SHOULD CLEAR VARS !!!
 state_one2two_trainsition:
    BTFSC PORTA,4
    call ra4_pressed_func
    BTFSS PORTA,4 ; skip if ra4 is set
    call check_if_ra4_released
    BTFSS stopped,0
    goto state_one2two_trainsition
    goto state2_rd3
    
 re4_pressed_func
 movlw d'1'
 movwf re4_pressed
 return
 
 check_if_re4_released
 BTFSS re4_pressed,0
 return
 movlw d'1'
 movwf re4_stopped
 clrf re4_pressed
 return
 
 re3_pressed_func
 movlw d'1'
 movwf re3_pressed
 return
 
 check_if_re3_released
 BTFSS re3_pressed,0
 return
 movlw d'1'
 movwf re3_stopped
 clrf re3_pressed
 return
 
re3_check
    BTFSC PORTE,3
    call re3_pressed_func
    BTFSS PORTE,3
    call check_if_re3_released
    return
re4_check
    BTFSC PORTE,4
    call re4_pressed_func
    BTFSS PORTE,4
    call check_if_re4_released
    return

clean_re_vars
    clrf re3_pressed
    clrf re4_pressed
    clrf re3_stopped
    clrf re4_stopped
    return
clean_ra_vars
    clrf ra4_pressed
    clrf stopped
    return

state2_ra0:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_ra0
    call re4_check
    BTFSC re4_stopped,0
    goto state3_ra0
    goto state2_ra0
state2_ra1:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_ra1
    call re4_check
    BTFSC re4_stopped,0
    goto state3_ra1
    goto state2_ra1
state2_ra2:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_ra2
    call re4_check
    BTFSC re4_stopped,0
    goto state3_ra2
    goto state2_ra2
state2_ra3:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_ra3
    call re4_check
    BTFSC re4_stopped,0
    goto state3_ra3
    goto state2_ra3
state2_rb0:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rb0
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rb0
    goto state2_rb0
state2_rb1:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rb1
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rb1
    goto state2_rb1
state2_rb2:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rb2
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rb2
    goto state2_rb2
state2_rb3:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rb3
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rb3
    goto state2_rb3
state2_rc0:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rc0
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rc0
    goto state2_rc0
state2_rc1:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rc1
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rc1
    goto state2_rc1
state2_rc2:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rc2
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rc2
    goto state2_rc2
state2_rc3:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rc3
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rc3
    goto state2_rc3
state2_rd0:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rd0
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rd0
    goto state2_rd0
state2_rd1:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rd1
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rd1
    goto state2_rd1
state2_rd2:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rd2
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rd2
    goto state2_rd2
state2_rd3:
    call clean_ra_vars
    call re3_check
    BTFSC re3_stopped,0
    goto state1_rd3
    call re4_check
    BTFSC re4_stopped,0
    goto state3_rd3
    goto state2_rd3
    
    
    
state3_ra0:
    movlw b'00000000'
    movwf LATA
    call clean_re_vars
    call DELAY_checkButton
    BTFSC stopped, 0
    goto state2_ra0
    state_three2two_transition:
	BTFSC PORTA,4
	call ra4_pressed_func
	BTFSS PORTA,4 ; skip if ra4 is set
	call check_if_ra4_released
	BTFSS stopped,0
	goto state_three2two_transition
	clrf stopped
	goto state2_ra0
state3_ra1:
    movlw b'00000001'
    movwf LATA
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_ra0
    clrf stopped
    goto state2_ra1
state3_ra2:
    movlw b'00000011'
    movwf LATA
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_ra1
    clrf stopped
    goto state2_ra2
state3_ra3:
    movlw b'00000111'
    movwf LATA
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_ra2
    clrf stopped
    goto state2_ra3
    
state3_rb0:
    movlw b'00000000'
    movwf LATB
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_ra3
    clrf stopped
    goto state2_rb1
state3_rb1:
    movlw b'00000001'
    movwf LATB
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rb0
    clrf stopped
    goto state2_rb1
state3_rb2:
    movlw b'00000011'
    movwf LATB
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rb1
    clrf stopped
    goto state2_rb2
state3_rb3:
    movlw b'00000111'
    movwf LATB
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rb2
    clrf stopped
    goto state2_rb3
   
state3_rc0:
    movlw b'00000000'
    movwf LATC
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rb3
    clrf stopped
    goto state2_rc0
state3_rc1:
    movlw b'00000001'
    movwf LATC
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rc0
    clrf stopped
    goto state2_rc1
state3_rc2:
    movlw b'00000011'
    movwf LATC
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rc1
    clrf stopped
    goto state2_rc2
state3_rc3:
    movlw b'00000111'
    movwf LATC
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rc2
    clrf stopped
    goto state2_rc3
    
state3_rd0:
    movlw b'00000000'
    movwf LATD
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rc3
    clrf stopped
    goto state2_rd0
state3_rd1:
    movlw b'00000001'
    movwf LATD
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rd0
    clrf stopped
    goto state2_rd1
state3_rd2:
    movlw b'00000011'
    movwf LATD
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rd1
    clrf stopped
    goto state2_rd2
state3_rd3:
    movlw b'00000111'
    movwf LATD
    call clean_re_vars
    call DELAY_checkButton
    BTFSS stopped, 0
    goto state3_rd2
    clrf stopped
    goto state2_rd3
    
 
 
 
 
 
 
 


main:
call init
call state1
end