;2171270, Ertugrul AYPEK
;2171296, Mustafa BADILLI 
    
list P=18F8722

#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

var_tmr1h   udata 0x26
var_tmr1h ; to save value of tmr1 when RG0 is pressed.. high

var_tmr1l   udata 0x21	
var_tmr1l ; to save value of tmr1 when RG0 is pressed.. low


tmr1_temp   udata 0x22
tmr1_temp ; for operations in create_astreoid function, to control last 3 bits

create_counter  udata 0x23
create_counter ;; every 10 astreoid complement it
  
count_2_ms_interrupts udata 0x24
count_2_ms_interrupts ; for example, to count 500 ms we need it to be 250.
  
spaceship_location udata 0x25
spaceship_location ; keeps spaceship's location in a variable 
 
laser_beam_rb udata 0x27
laser_beam_rb

laser_beam_rc udata 0x28
laser_beam_rc

laser_beam_rd udata 0x29
laser_beam_rd

laser_beam_re udata 0x2a
laser_beam_re

laser_beam_rf udata 0x2b
laser_beam_rf

count_passed_intervals udata 0x2c
count_passed_intervals
 
astreoid_ra udata 0x2d
astreoid_ra
 
astreoid_rb udata 0x2e
astreoid_rb

astreoid_rc udata 0x2f
astreoid_rc

astreoid_rd udata 0x30
astreoid_rd

astreoid_re udata 0x31
astreoid_re

astreoid_rf udata 0x32
astreoid_rf

 
wreg_temp udata 0x35
wreg_temp
 
required_time udata 0x36
required_time
 
status_temp udata 0x37
status_temp
 
pclath_temp udata 0x38
pclath_temp
 
puan0 udata 0x34
puan0	;least significant bit of score
 
puan1 udata 0x39
puan1
 
puan2 udata 0x3A
puan2
 
puan3 udata 0x3B
puan3	; most significant bit of score (this program counts upto 9999)
 
L1 udata 0x3C
L1
 
L2 udata 0x3D
L2
 
org     0x00
goto    init

org     0x08
goto    isr             ;go to interrupt service routine

 table
    MOVF    PCL,    F  
    RLNCF   WREG,   W 
    ADDWF   PCL,    F  
    RETLW b'00111111' ;0
    RETLW b'00000110' ;1
    RETLW b'01011011' ;2
    RETLW b'01001111' ;3 
    RETLW b'01100110' ;4
    RETLW b'01101101' ;5
    RETLW b'01111101' ;6
    RETLW b'00000111' ;7 
    RETLW b'01111111' ;8 
    RETLW b'01100111' ;9 
DELAY                          
    movlw d'10'                      
    movwf L2                    

    LOOP2:
	movlw d'10'                 
	movwf L1                  

    LOOP1:
	decfsz L1,F                    
	goto LOOP1               
	decfsz L2,F                    
	goto LOOP2               
	return 
    return
    
incrPuan
    INCF puan0
    movlw d'9'
    cpfsgt puan0
    return
    
    clrf puan0
    INCF puan1
    
    movlw d'9'
    cpfsgt puan1
    return
    
    clrf puan1
    INCF puan2
    
    movlw d'9'
    cpfsgt puan2
    return
    
    clrf puan2
    INCF puan3
    
    return
    
display
    clrf LATH
    
    movf puan3,0
    call table
    movwf LATJ
    bsf LATH,0
    call DELAY
    
    movf puan2,0
    call table
    movwf LATJ
    bcf LATH,0
    bsf LATH,1
    call DELAY
    
    movf puan1,0
    call table
    movwf LATJ
     bcf LATH,1
    bsf LATH,2
    call DELAY
    
    movf puan0,0
    call table
    movwf LATJ
     bcf LATH,2
    bsf LATH,3
    call DELAY
    
    return
    
    
save_registers
    movwf 	wreg_temp          ;Copy W to TEMP register
    swapf 	STATUS, w       ;Swap status to be saved into W
    clrf 	STATUS          ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
    movwf 	status_temp     ;Save status to bank zero STATUS_TEMP register
    movf 	PCLATH, w       ;Only required if using pages 1, 2 and/or 3
    movwf 	pclath_temp     ;Save PCLATH into W
    clrf 	PCLATH          ;Page zero, regardless of current page
    return
restore_registers
    movf 	pclath_temp, w  ;Restore PCLATH
    movwf 	PCLATH          ;Move W into PCLATH
    swapf 	status_temp, w  ;Swap STATUS_TEMP register into W
    movwf 	STATUS          ;Move W into STATUS register
    swapf 	wreg_temp, f       ;Swap W_TEMP
    swapf 	wreg_temp, w       ;Swap W_TEMP into W
    return
isr:
    call save_registers
    btfsc INTCON, 2 ; if the cause of interrupt was timer0 then go to t0 interrupt
    call t0_interrupt
    call restore_registers
    RETFIE
    
t0_interrupt
    INCF count_2_ms_interrupts
    movlw d'178' ; 1.9968 ms
    movwf TMR0
    bcf INTCON, 2
    return    
    
init:
    ;Disable interrupts
    clrf INTCON
    
    ;Configure Output Ports
    MOVLW 0Fh ; Configure A/D
    MOVWF ADCON1
    
    clrf TRISA
    clrf LATA
    clrf TRISB
    clrf LATB
    
    clrf TRISC
    clrf LATC
    
    clrf TRISD
    clrf LATD
    
    clrf TRISE
    clrf LATE
    
    clrf TRISF
    clrf LATF
    
  
    ;Configure Input/Interrupt Ports
    movlw   b'00001111' 
    movwf   TRISG    
    clrf    LATG
   ;bcf     INTCON2, 7  ;Pull-ups are enabled - clear INTCON2<7>
    
   
    clrf    TRISJ
    clrf    LATJ
    
    clrf    TRISH
    clrf    LATH
    

    ;Initialize Timer0 and Timer1
    movlw   b'01000111'  
    movwf   T0CON ;
    
    movlw   b'11001001' 
    movwf   T1CON ; 
    
    ;Enable interrupts
    movlw   b'10100000' ;Enable Global, Timer0 interrupts 
    movwf   INTCON

    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1
    
    goto waitForRG0


waitForRG0:
    
    clrf LATJ
    clrf LATH
    movlw 0x00
    call table
    movwf LATJ
    bsf LATH,3
    bsf LATH,2
    bsf LATH,1
    bsf LATH,0
    call DELAY
    
	
    CLRF LATA
    movlw   b'11001101' 
    movwf   T1CON 
    movlw b'00001000'
    movwf LATA
    movwf spaceship_location
    clrf LATB
    clrf LATC
    clrf LATD
    clrf LATE
    clrf LATF
    
    waitRG0:
	btfss PORTG,0
	goto waitRG0
    
    movf  TMR1H,w
    movwf var_tmr1h
    movf  TMR1L,w
    movwf var_tmr1l
    clrf T1CON
    clrf laser_beam_rb
    clrf laser_beam_rc
    clrf laser_beam_rd
    clrf laser_beam_re
    clrf laser_beam_rf
    clrf astreoid_ra
    clrf astreoid_rb
    clrf astreoid_rc
    clrf astreoid_rd
    clrf astreoid_re
    clrf astreoid_rf
    clrf puan0 ; to save score
    clrf puan1 ; to save score
    clrf puan2 ; to save score
    clrf puan3 ; to save score
    clrf L1
    clrf L2
    clrf tmr1_temp
    clrf create_counter
    clrf count_2_ms_interrupts
    clrf count_passed_intervals
    movlw d'250'
    movwf required_time
    goto startTheGame
    

    

; main loop of the program
startTheGame:
    call space_ship_button
    btfsc PORTG, 1 ;if laser beam is pressed do next instruction
    call create_laser_beam
    movf required_time, 0
    cpfslt count_2_ms_interrupts
    call periodic_routine
    call check_for_end_game
    call display
    goto startTheGame

space_ship_button 
    btfsc PORTG, 3
    goto move_up
    btfsc PORTG, 2
    goto move_down
    goto exit
    
create_laser_beam
    btfsc spaceship_location, 0
    goto create_laser_beam_rb0
    btfsc spaceship_location, 1
    goto create_laser_beam_rb1
    btfsc spaceship_location, 2
    goto create_laser_beam_rb2
    btfsc spaceship_location, 3
    goto create_laser_beam_rb3
    btfsc spaceship_location, 4
    goto create_laser_beam_rb4
    btfsc spaceship_location, 5
    goto create_laser_beam_rb5

create_laser_beam_rb0:
    bsf laser_beam_rb,0
    bsf LATB,0
    goto exit
create_laser_beam_rb1:
    bsf laser_beam_rb,1
    bsf LATB,1
    goto exit
create_laser_beam_rb2:
    bsf laser_beam_rb,2
    bsf LATB,2
    goto exit
create_laser_beam_rb3:
    bsf laser_beam_rb,3
    bsf LATB,3
    goto exit
create_laser_beam_rb4:
    bsf laser_beam_rb,4
    bsf LATB,4
    goto exit
create_laser_beam_rb5:
    bsf laser_beam_rb,5
    bsf LATB,5
    goto exit
    
exit:
    return
    
    
; Moves spaceship up when pressed PORTG,3 is released
move_up:
    btfsc PORTG, 3
    goto move_up
    btfsc spaceship_location, 0
    return
    btfsc spaceship_location, 1
    goto move_up_spaceship_ra1
    btfsc spaceship_location, 2
    goto move_up_spaceship_ra2
    btfsc spaceship_location, 3
    goto move_up_spaceship_ra3
    btfsc spaceship_location, 4
    goto move_up_spaceship_ra4
    btfsc spaceship_location, 5
    goto move_up_spaceship_ra5    
move_up_spaceship_ra1:
    bcf spaceship_location, 1
    bcf LATA,		    1
    bsf spaceship_location, 0
    bsf LATA,		    0
    goto exit
move_up_spaceship_ra2:
    bcf spaceship_location, 2
    bcf LATA,		    2
    bsf spaceship_location, 1
    bsf LATA,		    1
    goto exit
move_up_spaceship_ra3:
    bcf spaceship_location, 3
    bcf LATA,		    3
    bsf spaceship_location, 2
    bsf LATA,		    2
    goto exit
move_up_spaceship_ra4:
    bcf spaceship_location, 4
    bcf LATA,		    4
    bsf spaceship_location, 3
    bsf LATA,		    3
    goto exit
move_up_spaceship_ra5:
    bcf spaceship_location, 5
    bcf LATA,		    5
    bsf spaceship_location, 4
    bsf LATA,		    4
    goto exit

    
; moves spaceship down when pressed PORTG,2 is released
move_down:
    btfsc PORTG, 2
    goto move_down
    btfsc spaceship_location, 5
    return
    btfsc spaceship_location, 0
    goto move_down_spaceship_ra0
    btfsc spaceship_location, 1
    goto move_down_spaceship_ra1
    btfsc spaceship_location, 2
    goto move_down_spaceship_ra2
    btfsc spaceship_location, 3
    goto move_down_spaceship_ra3
    btfsc spaceship_location, 4
    goto move_down_spaceship_ra4
move_down_spaceship_ra0:
    bcf spaceship_location, 0
    bcf LATA,		    0
    bsf spaceship_location, 1
    bsf LATA,		    1
    goto exit
move_down_spaceship_ra1:
    bcf spaceship_location, 1
    bcf LATA,		    1
    bsf spaceship_location, 2
    bsf LATA,		    2
    goto exit
move_down_spaceship_ra2:
    bcf spaceship_location, 2
    bcf LATA,		    2
    bsf spaceship_location, 3
    bsf LATA,		    3
    goto exit
move_down_spaceship_ra3:
    bcf spaceship_location, 3
    bcf LATA,		    3
    bsf spaceship_location, 4
    bsf LATA,		    4
    goto exit
move_down_spaceship_ra4:
    bcf spaceship_location, 4
    bcf LATA,		    4
    bsf spaceship_location, 5
    bsf LATA,		    5
    goto exit
    

; creates astreoid according to given algorithm
create_astreoid
movlw b'00000111'
andwf var_tmr1l,0 ; value of tmr1l at the beginning
movwf tmr1_temp ; last 3 bits
movlw d'6'
cpfseq tmr1_temp,0
goto not_equal ; greater or less
goto equal ; RF0

equal:
    bsf LATF,0  ;; RF0
    bsf astreoid_rf, 0
    goto finish
    
finish:
    incf create_counter
    movlw d'10'
    cpfseq create_counter
    goto rotate_var_tmr1
    comf var_tmr1l
    comf var_tmr1h
    clrf create_counter
    goto exit_create_astreoid
    
rotate_var_tmr1:
    rrncf var_tmr1l
    rrncf var_tmr1h
    btfss var_tmr1l, 7
    goto rotate_exchange0
    btfss var_tmr1h ,7 
    goto rotate_exchange10
    goto rotate_exchange11
        
rotate_exchange0:
    btfss var_tmr1h, 7
    goto rotate_exchange00
    bcf var_tmr1l, 7
    bsf var_tmr1h, 7
    goto exit_create_astreoid
rotate_exchange00:
    bcf var_tmr1l, 7
    bcf var_tmr1h, 7
    goto exit_create_astreoid
rotate_exchange10:  
    bsf var_tmr1l, 7
    bcf var_tmr1h, 7
    goto exit_create_astreoid
rotate_exchange11:    
    bsf var_tmr1l, 7
    bsf var_tmr1h, 7
    goto exit_create_astreoid
    
not_equal:
    movlw d'7'
    cpfseq tmr1_temp
    goto lt0
    goto gt
    ;goto lt0
  
gt:
    ;subwf tmr1_temp
    bsf LATF,1  ;;; RF1 den astreoid ..
    bsf astreoid_rf, 1
    goto finish


lt0:   
    movlw d'0'
    cpfseq tmr1_temp
    goto lt1
    bsf LATF,0
    bsf astreoid_rf, 0
    goto finish
lt1:
    movlw d'1'
    cpfseq tmr1_temp
    goto lt2
    bsf LATF,1
    bsf astreoid_rf, 1
    goto finish
lt2:
    movlw d'2'
    cpfseq tmr1_temp
    goto lt3
    bsf LATF,2
    bsf astreoid_rf, 2
    goto finish
lt3:
    movlw d'3'
    cpfseq tmr1_temp
    goto lt4
    bsf LATF,3
    bsf astreoid_rf, 3
    goto finish
lt4:
    movlw d'4'
    cpfseq tmr1_temp
    goto lt5
    bsf LATF,4
    bsf astreoid_rf, 4
    goto finish
lt5:    
    movlw d'5'
    cpfseq tmr1_temp
    nop
    bsf LATF,5
    bsf astreoid_rf, 5
    goto finish
    
exit_create_astreoid:
    return

    
periodic_routine ; every 500, 400, 300 or 200 ms do things here
    clrf count_2_ms_interrupts
    call collision_check
    call move_laser_beams
    call move_astreoids
    call create_astreoid        
    
    movlw d'50'
    cpfslt count_passed_intervals
    goto set_to_200ms
    
    movlw d'30'
    cpfslt count_passed_intervals
    goto set_to_300ms
    
    movlw d'10'
    cpfslt count_passed_intervals
    goto set_to_400ms
    
    INCF count_passed_intervals
    return

set_to_400ms:
    movlw d'200'
    movwf required_time
    INCF count_passed_intervals
    return
set_to_300ms:
    movlw d'150'
    movwf required_time
    INCF count_passed_intervals
    return
set_to_200ms:
    movlw d'100'
    movwf required_time
    INCF count_passed_intervals
    return
    
    
    
move_laser_beams
    btfsc laser_beam_rf, 5
    call move_laser_rf5
    btfsc laser_beam_rf, 4
    call move_laser_rf4
    btfsc laser_beam_rf, 3
    call move_laser_rf3
    btfsc laser_beam_rf, 2
    call move_laser_rf2
    btfsc laser_beam_rf, 1
    call move_laser_rf1
    btfsc laser_beam_rf, 0
    call move_laser_rf0
    
    
    btfsc laser_beam_re, 5
    call move_laser_re5
    btfsc laser_beam_re, 4
    call move_laser_re4
    btfsc laser_beam_re, 3
    call move_laser_re3
    btfsc laser_beam_re, 2
    call move_laser_re2
    btfsc laser_beam_re, 1
    call move_laser_re1
    btfsc laser_beam_re, 0
    call move_laser_re0
    
    btfsc laser_beam_rd, 5
    call move_laser_rd5
    btfsc laser_beam_rd, 4
    call move_laser_rd4
    btfsc laser_beam_rd, 3
    call move_laser_rd3
    btfsc laser_beam_rd, 2
    call move_laser_rd2
    btfsc laser_beam_rd, 1
    call move_laser_rd1
    btfsc laser_beam_rd, 0
    call move_laser_rd0
    
    btfsc laser_beam_rc, 5
    call move_laser_rc5
    btfsc laser_beam_rc, 4
    call move_laser_rc4
    btfsc laser_beam_rc, 3
    call move_laser_rc3
    btfsc laser_beam_rc, 2
    call move_laser_rc2
    btfsc laser_beam_rc, 1
    call move_laser_rc1
    btfsc laser_beam_rc, 0
    call move_laser_rc0
    
    btfsc laser_beam_rb, 5
    call move_laser_rb5
    btfsc laser_beam_rb, 4
    call move_laser_rb4
    btfsc laser_beam_rb, 3
    call move_laser_rb3
    btfsc laser_beam_rb, 2
    call move_laser_rb2
    btfsc laser_beam_rb, 1
    call move_laser_rb1
    btfsc laser_beam_rb, 0
    call move_laser_rb0
                                
    return
move_laser_rb0
    bcf laser_beam_rb,	0
    bcf LATB,		0
    bsf laser_beam_rc,	0
    bsf LATC,		0
    return
move_laser_rb1
    bcf laser_beam_rb,	1
    bcf LATB,		1
    bsf laser_beam_rc,	1
    bsf LATC,		1
    return
move_laser_rb2
    bcf laser_beam_rb,	2
    bcf LATB,		2
    bsf laser_beam_rc,	2
    bsf LATC,		2
    return
move_laser_rb3
    bcf laser_beam_rb,	3
    bcf LATB,		3
    bsf laser_beam_rc,	3
    bsf LATC,		3
    return
move_laser_rb4
    bcf laser_beam_rb,	4
    bcf LATB,		4
    bsf laser_beam_rc,	4
    bsf LATC,		4
    return    
move_laser_rb5
    bcf laser_beam_rb,	5
    bcf LATB,		5
    bsf laser_beam_rc,	5
    bsf LATC,		5
    return    
move_laser_rc0
    bcf laser_beam_rc,	0
    bcf LATC,		0
    bsf laser_beam_rd,	0
    bsf LATD,		0
    return
move_laser_rc1
    bcf laser_beam_rc,	1
    bcf LATC,		1
    bsf laser_beam_rd,	1
    bsf LATD,		1
    return
move_laser_rc2
    bcf laser_beam_rc,	2
    bcf LATC,		2
    bsf laser_beam_rd,	2
    bsf LATD,		2
    return
move_laser_rc3
    bcf laser_beam_rc,	3
    bcf LATC,		3
    bsf laser_beam_rd,	3
    bsf LATD,		3
    return
move_laser_rc4
    bcf laser_beam_rc,	4
    bcf LATC,		4
    bsf laser_beam_rd,	4
    bsf LATD,		4
    return    
move_laser_rc5
    bcf laser_beam_rc,	5
    bcf LATC,		5
    bsf laser_beam_rd,	5
    bsf LATD,		5
    return   
    
move_laser_rd0
    bcf laser_beam_rd,	0
    bcf LATD,		0
    bsf laser_beam_re,	0
    bsf LATE,		0
    return
move_laser_rd1
    bcf laser_beam_rd,	1
    bcf LATD,		1
    bsf laser_beam_re,	1
    bsf LATE,		1
    return
move_laser_rd2
    bcf laser_beam_rd,	2
    bcf LATD,		2
    bsf laser_beam_re,	2
    bsf LATE,		2
    return
move_laser_rd3
    bcf laser_beam_rd,	3
    bcf LATD,		3
    bsf laser_beam_re,	3
    bsf LATE,		3
    return
move_laser_rd4
    bcf laser_beam_rd,	4
    bcf LATD,		4
    bsf laser_beam_re,	4
    bsf LATE,		4
    return    
move_laser_rd5
    bcf laser_beam_rd,	5
    bcf LATD,		5
    bsf laser_beam_re,	5
    bsf LATE,		5
    return   
    
move_laser_re0
    bcf laser_beam_re,	0
    bcf LATE,		0
    bsf laser_beam_rf,	0
    bsf LATF,		0
    return
move_laser_re1
    bcf laser_beam_re,	1
    bcf LATE,		1
    bsf laser_beam_rf,	1
    bsf LATF,		1
    return
move_laser_re2
    bcf laser_beam_re,	2
    bcf LATE,		2
    bsf laser_beam_rf,	2
    bsf LATF,		2
    return
move_laser_re3
    bcf laser_beam_re,	3
    bcf LATE,		3
    bsf laser_beam_rf,	3
    bsf LATF,		3
    return
move_laser_re4
    bcf laser_beam_re,	4
    bcf LATE,		4
    bsf laser_beam_rf,	4
    bsf LATF,		4
    return    
move_laser_re5
    bcf laser_beam_re,	5
    bcf LATE,		5
    bsf laser_beam_rf,	5
    bsf LATF,		5
    return   
 
move_laser_rf0
    bcf laser_beam_rf,	0
    bcf LATF,		0
    return
move_laser_rf1
    bcf laser_beam_rf,	1
    bcf LATF,		1
    return
move_laser_rf2
    bcf laser_beam_rf,	2
    bcf LATF,		2
    return
move_laser_rf3
    bcf laser_beam_rf,	3
    bcf LATF,		3
    return
move_laser_rf4
    bcf laser_beam_rf,	4
    bcf LATF,		4
    return
move_laser_rf5
    bcf laser_beam_rf,	5
    bcf LATF,		5
    return
    

        	 

;; Movement of astreoids
    
move_astreoids ;; set a flag to check whether game finished or not.
    btfsc astreoid_ra, 5
    call move_astreoid_ra5
    btfsc astreoid_ra, 4
    call move_astreoid_ra4
    btfsc astreoid_ra, 3
    call move_astreoid_ra3
    btfsc astreoid_ra, 2
    call move_astreoid_ra2
    btfsc astreoid_ra, 1
    call move_astreoid_ra1
    btfsc astreoid_ra, 0
    call move_astreoid_ra0
    
    
    btfsc astreoid_rb, 5
    call move_astreoid_rb5
    btfsc astreoid_rb, 4
    call move_astreoid_rb4
    btfsc astreoid_rb, 3
    call move_astreoid_rb3
    btfsc astreoid_rb, 2
    call move_astreoid_rb2
    btfsc astreoid_rb, 1
    call move_astreoid_rb1
    btfsc astreoid_rb, 0
    call move_astreoid_rb0
    
    
    btfsc astreoid_rc, 5
    call move_astreoid_rc5
    btfsc astreoid_rc, 4
    call move_astreoid_rc4
    btfsc astreoid_rc, 3
    call move_astreoid_rc3
    btfsc astreoid_rc, 2
    call move_astreoid_rc2
    btfsc astreoid_rc, 1
    call move_astreoid_rc1
    btfsc astreoid_rc, 0
    call move_astreoid_rc0
    
    
    btfsc astreoid_rd, 5
    call move_astreoid_rd5
    btfsc astreoid_rd, 4
    call move_astreoid_rd4
    btfsc astreoid_rd, 3
    call move_astreoid_rd3
    btfsc astreoid_rd, 2
    call move_astreoid_rd2
    btfsc astreoid_rd, 1
    call move_astreoid_rd1
    btfsc astreoid_rd, 0
    call move_astreoid_rd0
    
    
    btfsc astreoid_re, 5
    call move_astreoid_re5
    btfsc astreoid_re, 4
    call move_astreoid_re4
    btfsc astreoid_re, 3
    call move_astreoid_re3
    btfsc astreoid_re, 2
    call move_astreoid_re2
    btfsc astreoid_re, 1
    call move_astreoid_re1
    btfsc astreoid_re, 0
    call move_astreoid_re0
    
    
    btfsc astreoid_rf, 5
    call move_astreoid_rf5
    btfsc astreoid_rf, 4
    call move_astreoid_rf4
    btfsc astreoid_rf, 3
    call move_astreoid_rf3
    btfsc astreoid_rf, 2
    call move_astreoid_rf2
    btfsc astreoid_rf, 1
    call move_astreoid_rf1
    btfsc astreoid_rf, 0
    call move_astreoid_rf0
                          
    return
    
move_astreoid_ra0 
    bcf astreoid_ra, 0
    bcf LATA,	0
    return
	
move_astreoid_ra1
    bcf astreoid_ra,	1
    bcf LATA,		1
    return


move_astreoid_ra2
    bcf astreoid_ra,	2
    bcf LATA,		2
    return

move_astreoid_ra3
    bcf astreoid_ra,	3
    bcf LATA,		3
    return

move_astreoid_ra4
    bcf astreoid_ra,	4
    bcf LATA,		4
    return

move_astreoid_ra5
    bcf astreoid_ra,	5
    bcf LATA,		5
    return
	  
move_astreoid_rb0
    btfsc laser_beam_rb, 0
    goto collisionB0
    bcf astreoid_rb,	0
    bcf LATB,		0
    bsf astreoid_ra,	0
    bsf LATA,		0
    return 
    collisionB0:
	bcf laser_beam_rb, 0
	bcf astreoid_rb, 0
	bcf LATB, 0
	call incrPuan	
	return

move_astreoid_rb1
    btfsc laser_beam_rb, 1
    goto collisionB1
    bcf astreoid_rb,	1
    bcf LATB,		1
    bsf astreoid_ra,	1
    bsf LATA,		1
    return 
    collisionB1:
	bcf laser_beam_rb, 1
	bcf astreoid_rb, 1
	bcf LATB, 1
	call incrPuan	
	return

move_astreoid_rb2
    btfsc laser_beam_rb, 2
    goto collisionB2
    bcf astreoid_rb,	2
    bcf LATB,		2
    bsf astreoid_ra,	2
    bsf LATA,		2
    return 
    collisionB2:
	bcf laser_beam_rb, 2
	bcf astreoid_rb, 2
	bcf LATB, 2
	call incrPuan	
	return

move_astreoid_rb3
    btfsc laser_beam_rb, 3
    goto collisionB3
    bcf astreoid_rb,	3
    bcf LATB,		3
    bsf astreoid_ra,	3
    bsf LATA,		3
    return 
    collisionB3:
	bcf laser_beam_rb, 3
	bcf astreoid_rb, 3
	bcf LATB, 3
	call incrPuan	
	return

move_astreoid_rb4
    btfsc laser_beam_rb, 4
    goto collisionB4
    bcf astreoid_rb,	4
    bcf LATB,		4
    bsf astreoid_ra,	4
    bsf LATA,		4
    return 
    collisionB4:
	bcf laser_beam_rb, 4
	bcf astreoid_rb, 4
	bcf LATB, 4
	call incrPuan	
	return

move_astreoid_rb5
    btfsc laser_beam_rb, 5
    goto collisionB5
    bcf astreoid_rb,	5
    bcf LATB,		5
    bsf astreoid_ra,	5
    bsf LATA,		5
    return 
    collisionB5:
	bcf laser_beam_rb, 5
	bcf astreoid_rb, 5
	bcf LATB, 5
	call incrPuan	
	return
    
move_astreoid_rc0
    btfsc laser_beam_rc, 0
    goto collisionC0
    bcf astreoid_rc,	0
    bcf LATC,		0
    bsf astreoid_rb,	0
    bsf LATB,		0
    return 
    collisionC0:
	bcf laser_beam_rc, 0
	bcf LATC, 0
	bcf astreoid_rc, 0
	bcf LATC, 0
	call incrPuan	
	return

move_astreoid_rc1
    btfsc laser_beam_rc, 1
    goto collisionC1
    bcf astreoid_rc,	1
    bcf LATC,		1
    bsf astreoid_rb,	1
    bsf LATB,		1
    return 
    collisionC1:
	bcf laser_beam_rc, 1
	bcf LATC, 1
	bcf astreoid_rc, 1
	bcf LATC, 1
	call incrPuan	
	return

move_astreoid_rc2
    btfsc laser_beam_rc, 2
    goto collisionC2
    bcf astreoid_rc,	2
    bcf LATC,		2
    bsf astreoid_rb,	2
    bsf LATB,		2
    return 
    collisionC2:
	bcf laser_beam_rc, 2
	bcf LATC, 2
	bcf astreoid_rc, 2
	bcf LATC, 2
	call incrPuan	
	return

move_astreoid_rc3
    btfsc laser_beam_rc, 3
    goto collisionC3
    bcf astreoid_rc,	3
    bcf LATC,		3
    bsf astreoid_rb,	3
    bsf LATB,		3
    return 
    collisionC3:
	bcf laser_beam_rc, 3
	bcf LATC, 3
	bcf astreoid_rc, 3
	bcf LATC, 3
	call incrPuan	
	return

move_astreoid_rc4
    btfsc laser_beam_rc, 4
    goto collisionC4
    bcf astreoid_rc,	4
    bcf LATC,		4
    bsf astreoid_rb,	4
    bsf LATB,		4
    return 
    collisionC4:
	bcf laser_beam_rc, 4
	bcf LATC, 4
	bcf astreoid_rc, 4
	bcf LATC, 4
	call incrPuan	
	return

move_astreoid_rc5
    btfsc laser_beam_rc, 5
    goto collisionC5
    bcf astreoid_rc,	5
    bcf LATC,		5
    bsf astreoid_rb,	5
    bsf LATB,		5
    return 
    collisionC5:
	bcf laser_beam_rc, 5
	bcf LATC, 5
	bcf astreoid_rc, 5
	bcf LATC, 5
	call incrPuan	
	return	

move_astreoid_rd0
    btfsc laser_beam_rd, 0
    goto collisionD0
    bcf astreoid_rd,	0
    bcf LATD,		0
    bsf astreoid_rc,	0
    bsf LATC,		0
    return 
    collisionD0:
	bcf laser_beam_rd, 0
	bcf LATD, 0
	bcf astreoid_rd, 0
	bcf LATD, 0
	call incrPuan	
	return

move_astreoid_rd1
    btfsc laser_beam_rd, 1
    goto collisionD1
    bcf astreoid_rd,	1
    bcf LATD,		1
    bsf astreoid_rc,	1
    bsf LATC,		1
    return 
    collisionD1:
	bcf laser_beam_rd, 1
	bcf LATD, 1
	bcf astreoid_rd, 1
	bcf LATD, 1
	call incrPuan	
	return

move_astreoid_rd2
    btfsc laser_beam_rd, 2
    goto collisionD2
    bcf astreoid_rd,	2
    bcf LATD,		2
    bsf astreoid_rc,	2
    bsf LATC,		2
    return 
    collisionD2:
	bcf laser_beam_rd, 2
	bcf LATD, 2
	bcf astreoid_rd, 2
	bcf LATD, 2
	call incrPuan	
	return

move_astreoid_rd3
    btfsc laser_beam_rd, 3
    goto collisionD3
    bcf astreoid_rd,	3
    bcf LATD,		3
    bsf astreoid_rc,	3
    bsf LATC,		3
    return 
    collisionD3:
	bcf laser_beam_rd, 3
	bcf LATD, 3
	bcf astreoid_rd, 3
	bcf LATD, 3
	call incrPuan	
	return

move_astreoid_rd4
    btfsc laser_beam_rd, 4
    goto collisionD4
    bcf astreoid_rd,	4
    bcf LATD,		4
    bsf astreoid_rc,	4
    bsf LATC,		4
    return 
    collisionD4:
	bcf laser_beam_rd, 4
	bcf LATD, 4
	bcf astreoid_rd, 4
	bcf LATD, 4
	call incrPuan	
	return

move_astreoid_rd5
    btfsc laser_beam_rd, 5
    goto collisionD5
    bcf astreoid_rd,	5
    bcf LATD,		5
    bsf astreoid_rc,	5
    bsf LATC,		5
    return 
    collisionD5:
	bcf laser_beam_rd, 5
	bcf LATD, 5
	bcf astreoid_rd, 5
	bcf LATD, 5
	call incrPuan	
	return
	
	
move_astreoid_re0
    btfsc laser_beam_re, 0
    goto collisionE0
    bcf astreoid_re,	0
    bcf LATE,		0
    bsf astreoid_rd,	0
    bsf LATD,		0
    return 
    collisionE0:
	bcf laser_beam_re, 0
	bcf astreoid_re, 0
	bcf LATE, 0
	call incrPuan	
	return

move_astreoid_re1
    btfsc laser_beam_re, 1
    goto collisionE1
    bcf astreoid_re,	1
    bcf LATE,		1
    bsf astreoid_rd,	1
    bsf LATD,		1
    return 
    collisionE1:
	bcf laser_beam_re, 1
	bcf astreoid_re, 1
	bcf LATE, 1
	call incrPuan	
	return

move_astreoid_re2
    btfsc laser_beam_re, 2
    goto collisionE2
    bcf astreoid_re,	2
    bcf LATE,		2
    bsf astreoid_rd,	2
    bsf LATD,		2
    return 
    collisionE2:
	bcf laser_beam_re, 2
	bcf astreoid_re, 2
	bcf LATE, 2
	call incrPuan	
	return

move_astreoid_re3
    btfsc laser_beam_re, 3
    goto collisionE3
    bcf astreoid_re,	3
    bcf LATE,		3
    bsf astreoid_rd,	3
    bsf LATD,		3
    return 
    collisionE3:
	bcf laser_beam_re, 3
	bcf astreoid_re, 3
	bcf LATE, 3
	call incrPuan	
	return

move_astreoid_re4
    btfsc laser_beam_re, 4
    goto collisionE4
    bcf astreoid_re,	4
    bcf LATE,		4
    bsf astreoid_rd,	4
    bsf LATD,		4
    return 
    collisionE4:
	bcf laser_beam_re, 4
	bcf astreoid_re, 4
	bcf LATE, 4
	call incrPuan	
	return

move_astreoid_re5
    btfsc laser_beam_re, 5
    goto collisionE5
    bcf astreoid_re,	5
    bcf LATE,		5
    bsf astreoid_rd,	5
    bsf LATD,		5
    return 
    collisionE5:
	bcf laser_beam_re, 5
	bcf astreoid_re, 5
	bcf LATE, 5
	call incrPuan	
	return

move_astreoid_rf0
    btfsc laser_beam_rf, 0
    goto collisionF0
    bcf astreoid_rf,	0
    bcf LATF,		0
    bsf astreoid_re,	0
    bsf LATE,		0
    return 
    collisionF0:
	bcf laser_beam_rf, 0
	bcf astreoid_rf, 0
	bcf LATF, 0
	call incrPuan	
	return

move_astreoid_rf1
    btfsc laser_beam_rf, 1
    goto collisionF1
    bcf astreoid_rf,	1
    bcf LATF,		1
    bsf astreoid_re,	1
    bsf LATE,		1
    return 
    collisionF1:
	bcf laser_beam_rf, 1
	bcf astreoid_rf, 1
	bcf LATF, 1
	call incrPuan
	return

move_astreoid_rf2
    btfsc laser_beam_rf, 2
    goto collisionF2
    bcf astreoid_rf,	2
    bcf LATF,		2
    bsf astreoid_re,	2
    bsf LATE,		2
    return 
    collisionF2:
	bcf laser_beam_rf, 2
	bcf astreoid_rf, 2
	bcf LATF, 2
	call incrPuan
	return

move_astreoid_rf3
    btfsc laser_beam_rf, 3
    goto collisionF3
    bcf astreoid_rf,	3
    bcf LATF,		3
    bsf astreoid_re,	3
    bsf LATE,		3
    return 
    collisionF3:
	bcf laser_beam_rf, 3
	bcf astreoid_rf, 3
	bcf LATF, 3
	call incrPuan
	return

move_astreoid_rf4
    btfsc laser_beam_rf, 4
    goto collisionF4
    bcf astreoid_rf,	4
    bcf LATF,		4
    bsf astreoid_re,	4
    bsf LATE,		4
    return 
    collisionF4:
	bcf laser_beam_rf, 4
	bcf astreoid_rf, 4
	bcf LATF, 4
	call incrPuan
	return

move_astreoid_rf5
    btfsc laser_beam_rf, 5
    goto collisionF5
    bcf astreoid_rf,	5
    bcf LATF,		5
    bsf astreoid_re,	5
    bsf LATE,		5
    return 
    collisionF5:
	bcf laser_beam_rf, 5
	bcf astreoid_rf, 5
	bcf LATF, 5
	call incrPuan
	return 
    

    
    
    
; Checks laser beam - astreoid collision
collision_check
    btfsc laser_beam_rb,0
    call collision_on_rb0
    btfsc laser_beam_rb,1
    call collision_on_rb1
    btfsc laser_beam_rb,2
    call collision_on_rb2
    btfsc laser_beam_rb,3
    call collision_on_rb3
    btfsc laser_beam_rb,4
    call collision_on_rb4
    btfsc laser_beam_rb,5
    call collision_on_rb5
    
    btfsc laser_beam_rc,0
    call collision_on_rc0
    btfsc laser_beam_rc,1
    call collision_on_rc1
    btfsc laser_beam_rc,2
    call collision_on_rc2
    btfsc laser_beam_rc,3
    call collision_on_rc3
    btfsc laser_beam_rc,4
    call collision_on_rc4
    btfsc laser_beam_rc,5
    call collision_on_rc5
    
    btfsc laser_beam_rd,0
    call collision_on_rd0
    btfsc laser_beam_rd,1
    call collision_on_rd1
    btfsc laser_beam_rd,2
    call collision_on_rd2
    btfsc laser_beam_rd,3
    call collision_on_rd3
    btfsc laser_beam_rd,4
    call collision_on_rd4
    btfsc laser_beam_rd,5
    call collision_on_rd5
    
    btfsc laser_beam_re,0
    call collision_on_re0
    btfsc laser_beam_re,1
    call collision_on_re1
    btfsc laser_beam_re,2
    call collision_on_re2
    btfsc laser_beam_re,3
    call collision_on_re3
    btfsc laser_beam_re,4
    call collision_on_re4
    btfsc laser_beam_re,5
    call collision_on_re5
    
    btfsc laser_beam_rf,0
    call collision_on_rf0
    btfsc laser_beam_rf,1
    call collision_on_rf1
    btfsc laser_beam_rf,2
    call collision_on_rf2
    btfsc laser_beam_rf,3
    call collision_on_rf3
    btfsc laser_beam_rf,4
    call collision_on_rf4
    btfsc laser_beam_rf,5
    call collision_on_rf5
    
    return
    
collision_on_rb0
    btfss astreoid_rb,	0
    return
    bcf astreoid_rb,	0
    bcf laser_beam_rb,	0
    bcf LATB,		0
    call incrPuan
    return
collision_on_rb1
    btfss astreoid_rb,	1
    return
    bcf astreoid_rb,	1
    bcf laser_beam_rb,	1
    bcf LATB,		1
    call incrPuan
    return
collision_on_rb2
    btfss astreoid_rb,	2
    return
    bcf astreoid_rb,	2
    bcf laser_beam_rb,	2
    bcf LATB,		2
    call incrPuan
    return
collision_on_rb3
    btfss astreoid_rb,	3
    return
    bcf astreoid_rb,	3
    bcf laser_beam_rb,	3
    bcf LATB,		3
    call incrPuan
    return
collision_on_rb4
    btfss astreoid_rb,	4
    return
    bcf astreoid_rb,	4
    bcf laser_beam_rb,	4
    bcf LATB,		4
    call incrPuan
    return
collision_on_rb5
    btfss astreoid_rb,	5
    return
    bcf astreoid_rb,	5
    bcf laser_beam_rb,	5
    bcf LATB,		5
    call incrPuan
    return
    
collision_on_rc0
    btfss astreoid_rc,	0
    return
    bcf astreoid_rc,	0
    bcf laser_beam_rc,	0
    bcf LATC,		0
    call incrPuan
    return
collision_on_rc1
    btfss astreoid_rc,	1
    return
    bcf astreoid_rc,	1
    bcf laser_beam_rc,	1
    bcf LATC,		1
    call incrPuan
    return
collision_on_rc2
    btfss astreoid_rc,	2
    return
    bcf astreoid_rc,	2
    bcf laser_beam_rc,	2
    bcf LATC,		2
    call incrPuan
    return
collision_on_rc3
    btfss astreoid_rc,	3
    return
    bcf astreoid_rc,	3
    bcf laser_beam_rc,	3
    bcf LATC,		3
    call incrPuan
    return
collision_on_rc4
    btfss astreoid_rc,	4
    return
    bcf astreoid_rc,	4
    bcf laser_beam_rc,	4
    bcf LATC,		4
    call incrPuan
    return
collision_on_rc5
    btfss astreoid_rc,	5
    return
    bcf astreoid_rc,	5
    bcf laser_beam_rc,	5
    bcf LATC,		5
    call incrPuan
    return
    
collision_on_rd0
    btfss astreoid_rd,	0
    return
    bcf astreoid_rd,	0
    bcf laser_beam_rd,	0
    bcf LATD,		0
    call incrPuan
    return
collision_on_rd1
    btfss astreoid_rd,	1
    return
    bcf astreoid_rd,	1
    bcf laser_beam_rd,	1
    bcf LATD,		1
    call incrPuan
    return
collision_on_rd2
    btfss astreoid_rd,	2
    return
    bcf astreoid_rd,	2
    bcf laser_beam_rd,	2
    bcf LATD,		2
    call incrPuan
    return
collision_on_rd3
    btfss astreoid_rd,	3
    return
    bcf astreoid_rd,	3
    bcf laser_beam_rd,	3
    bcf LATD,		3
    call incrPuan
    return
collision_on_rd4
    btfss astreoid_rd,	4
    return
    bcf astreoid_rd,	4
    bcf laser_beam_rd,	4
    bcf LATD,		4
    call incrPuan
    return
collision_on_rd5
    btfss astreoid_rd,	5
    return
    bcf astreoid_rd,	5
    bcf laser_beam_rd,	5
    bcf LATD,		5
    call incrPuan
    return
    
collision_on_re0
    btfss astreoid_re,	0
    return
    bcf astreoid_re,	0
    bcf laser_beam_re,	0
    bcf LATE,		0
    call incrPuan
    return
collision_on_re1
    btfss astreoid_re,	1
    return
    bcf astreoid_re,	1
    bcf laser_beam_re,	1
    bcf LATE,		1
    call incrPuan
    return
collision_on_re2
    btfss astreoid_re,	2
    return
    bcf astreoid_re,	2
    bcf laser_beam_re,	2
    bcf LATE,		2
    call incrPuan
    return
collision_on_re3
    btfss astreoid_re,	3
    return
    bcf astreoid_re,	3
    bcf laser_beam_re,	3
    bcf LATE,		3
    call incrPuan
    return
collision_on_re4
    btfss astreoid_re,	4
    return
    bcf astreoid_re,	4
    bcf laser_beam_re,	4
    bcf LATE,		4
    call incrPuan
    return
collision_on_re5
    btfss astreoid_re,	5
    return
    bcf astreoid_re,	5
    bcf laser_beam_re,	5
    bcf LATE,		5
    call incrPuan
    return
    
collision_on_rf0
    btfss astreoid_rf,	0
    return
    bcf astreoid_rf,	0
    bcf laser_beam_rf,	0
    bcf LATF,		0
    call incrPuan
    return
collision_on_rf1
    btfss astreoid_rf,	1
    return
    bcf astreoid_rf,	1
    bcf laser_beam_rf,	1
    bcf LATF,		1
    call incrPuan
    return
collision_on_rf2
    btfss astreoid_rf,	2
    return
    bcf astreoid_rf,	2
    bcf laser_beam_rf,	2
    bcf LATF,		2
    call incrPuan
    return
collision_on_rf3
    btfss astreoid_rf,	3
    return
    bcf astreoid_rf,	3
    bcf laser_beam_rf,	3
    bcf LATF,		3
    call incrPuan
    return
collision_on_rf4
    btfss astreoid_rf,	4
    return
    bcf astreoid_rf,	4
    bcf laser_beam_rf,	4
    bcf LATF,		4
    call incrPuan
    return
collision_on_rf5
    btfss astreoid_rf,	5
    return
    bcf astreoid_rf,	5
    bcf laser_beam_rf,	5
    bcf LATF,		5
    call incrPuan
    return


; Checks if there is an astreoid on the spaceship's location
check_for_end_game
    btfsc spaceship_location,0
    goto check_for_end_ra0
    btfsc spaceship_location,1
    goto check_for_end_ra1
    btfsc spaceship_location,2
    goto check_for_end_ra2
    btfsc spaceship_location,3
    goto check_for_end_ra3
    btfsc spaceship_location,4
    goto check_for_end_ra4
    btfsc spaceship_location,5
    goto check_for_end_ra5
    
check_for_end_ra0:
    btfss astreoid_ra,0
    return
    goto waitForRG0    
check_for_end_ra1:
    btfss astreoid_ra,1
    return
    goto waitForRG0
check_for_end_ra2:
    btfss astreoid_ra,2
    return
    goto waitForRG0
check_for_end_ra3:
    btfss astreoid_ra,3
    return
    goto waitForRG0
check_for_end_ra4:
    btfss astreoid_ra,4
    return
    goto waitForRG0
check_for_end_ra5:   
    btfss astreoid_ra,5
    return
    goto waitForRG0 

end
    
    


 