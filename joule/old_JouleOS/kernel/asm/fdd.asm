;=========================================================;
; Fdd                                            11/12/03 ;
;---------------------------------------------------------;
; DOS EXTREME OS V0.01                                    ;
; by Craig Bamford.                                       ;
;                                                         ;
; Pmode floppy disk driver.                               ;                          
;=========================================================;

_DorReg            EQU   0x3f2                              ; digital output register (write only).
_MsReg             EQU   0x3f4                              ; main status register (read only).
_DtReg             EQU   0x3f5                              ; data register (read/write).
_CcrReg            EQU   0x3f7                              ; configuration control register.

_Sr0_AbTerm        EQU   6                                  ; test if aborted
_Sr0Seek           EQU   5                                  ; test if seek ok


_FddErrorSeekFail  EQU   0x40                               ; error code for seek command has failed.
_FddError_TimeOut  EQU   0x80                               ; error code for time out.

_FddMotorTimer0n db 0                                     ; is the time on,to turn the floppy motor off 1= yes,0 =no.
_FddMTimer       db 0                                     ; how many clock ticks, before turning floppy motor off
_MotorOn         db 0                                     ; is the motor on yes "MotorOn" = 1, no = 0
_Head            db 0                                     ; store the head/side.
_DriveHead       db 0                                     ; store head + drive.
_cTrack          db 0                                     ; store track/cylinder
_Sector          db 0                                     ; store Sector_

_FddErrorStatus  db 0                                     ; this the error code 00 = no error
_CommandTry      db 0                                     ; number of times to try command,before error.

_ResultST0	db 0
_ResultST1	db 0	                                 ; or pcn
_ResultST2	db 0
_ResultC	        db 0
_ResultH	        db 0
_ResultR	        db 0
_ResultN	        db 0
_ResultExtra	db 0

_ResultST3	db 0


 ;----------------------------------------------------;
 ; Fdd motor off         ;TURNS MOTOR OFF WITH DELAY. ;
 ;----------------------------------------------------;

_Fdd_motor_off:
        mov   dx,_DorReg                                 ; DorReg = 0x3F2
        mov   al,0
        out   dx,al
        mov   [_MotorOn],0                               ; mov the on bit 0,this means motor off.
        ret

 ;----------------------------------------------------;
 ; Fdd Motor On           ;TURNS MOTOR ON WITH DELAY. ;
 ;----------------------------------------------------;

_FddMotorOn:                                             
        mov   dx,DorReg                                 ; DorReg = 0x3F2
        mov   al,00011100b                              ; motor 0 on, DMA enabled, no reset, drive A:(00011100b)
        out   dx,al
        mov   [_Timer],20                                ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer
        mov   [_MotorOn],1                               ; mov the on bit 1
        ret



 ;----------------------------------------------------;
 ; FddReSet                         ;ResetController. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          none.                                     ;
 ;  Output:                                           ;
 ;                                                    ;
 ;                                             (100%) ;
 ;....................................................;


_FddReSet:                                                  
                                                        ;  **** RESET ****
        mov   dx,_DorReg                                 ; DorReg = 0x3F2
        mov   al,00001000b                              ; no motors enabled, DMA Enabled, held FDC at reset
        out   dx,al                                     ; drive A selected.

        mov   [_Timer],5	                                ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer
                                                        ;  **** PROGRAM DATE VIA CCR ****
        mov   dx,_CcrReg                                 ; CcrReg = 0x3f7
        mov   al,00000000b                              ; 500Kb/sec mode
        out   dx,al

        mov   dx,_DorReg                                 ; DorReg = 0x3F2
        or    al,00001100b                              ; FDC completed reset, al =0x0c.
        out   dx,al
                                                        ;  **** WAIT FOR INTERRUPT ****
        mov   [_done],0                                  ; we need to wait
        call  _WaitDone                                  ; for floppy int.
        jc    _ResetControllerError                      ; jump to error exit,if timeout.
        
        mov   cx,0x04                                   ; number of loops.
                                                        ;  **** ISSUE SENSE INTERRUPT ****
_SenseStatusLoop:                                        ; After a reset, the interrupts must be 
                                                        ; cleared by (four) dummy-reads.
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x08                                   ; Sense Interrupt Status Command
        out   dx,al

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read ST0 ,(DtReg = 0x3f5 )
        in    al,dx

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read PCN ,(DtReg = 0x3f5 )
        in    al,dx
       
        loop  _SenseStatusLoop                           ; Is cx,0 no then loop to lable.

                                                        ;  **** PARAMETERS DIFFERENT FROM DEFAULT ****
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer
                                                        ;  **** SPECIFY COMMAND ****
        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x03                                   ; Specify Command (includes next two bytes sent)
        out   dx,al 

        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov  dx,_DtReg                                   ; DtReg = 0x3f5 
        mov  al,0xDF                                    ; SRT = 3ms, HUT = 240ms
        out  dx,al 

        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _ResetControllerError                      ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov  dx,_DtReg                                   ; DtReg = 0x3f5 
        mov  al,0x02                                    ; HLT = 16ms, ND = 0
        out  dx,al

_ResetControllerSuccsess:
        clc
        ret

_ResetControllerError:
        stc
        ret

 ;----------------------------------------------------;
 ; Fdd seek                                           ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          none.                                     ;
 ;  Output:                                           ;
 ;                                                    ;
 ;                                             (100%) ;
 ;....................................................;

_FddReadWriteSeek:
        pushad
        mov   al,[_ResultC]                              ; put what track/cylinder we are at in al
        cmp   [_cTrack],al                               ; is it the same,as we want
        je    _FddSeekControllerSuccsess                 ; yes leave.
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer
                                                        ;  **** ISSUE A SEEK COMMAND ****
        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x0F                                   ; Seek Command 
        out   dx,al

        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov  dx,_DtReg                                   ; DtReg = 0x3f5 
        mov  al,[_DriveHead]                             ; Drive # (00 = A)
        out  dx,al 

        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc   _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov  dx,_DtReg                                   ; DtReg = 0x3f5 
        mov  al,[_cTrack]                                ; Cylinder #                
        out  dx,al
                                                        ;  **** WAIT FOR INTERRUPT ****
        mov   [_done],0                                  ; we need to wait
        call  _WaitDone                                  ; for floppy int.
        jc    _FddSeekControllerError                    ; jump to error exit,if timeout.
       
                                                        ;  **** ISSUE SENSE INTERRUPT STATUS COMMAND ****
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x08                                   ; Sense Interrupt Status Command
        out   dx,al

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   dx,DtReg                                  ; Should read ST0 ,(DtReg = 0x3f5 )
        in    al,dx
 
        mov   ah,al                                     ; save ST0 in ah

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddSeekControllerError                    ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read PCN ,(DtReg = 0x3f5 )
        in    al,dx

       ; mov   al,FddErrorSeekFail

        test  ah,00100000b                              ; test sr0 is 0x20
        jz    _FddSeekControllerError                    ; if not we have a error :-(.
   
        test  ah,10000000b                              ; test sr0 is 0x80
        jnz    _FddSeekControllerError                   ; if not we have a error :-(.
       

_FddSeekControllerSuccsess:                              ; succsess we hope :-)
        popad
        clc
        ret

_FddSeekControllerError:                                 ; we have a error (blame M$)
        popad
        stc
        ret


 ;----------------------------------------------------;
 ; Fdd recalibrate                                    ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          none.                                     ;
 ;  Output:                                           ;
 ;                                                    ;
 ;                                             (100%) ;
 ;....................................................;

FddRecalibrate:
        test  [_MotorOn],1
        jnz   @f
        call  _FddMotorOn                                ;  **** ENABLE MOTOR VIA DOR ****
@@:
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddRecalibrateError                       ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer
                                                        ;  **** ISSUE A RECALIBRATE COMMAND ****
        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x07                                   ; recalibrate Command 
        out   dx,al

        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddRecalibrateError                       ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov  dx,_DtReg                                   ; DtReg = 0x3f5 
        mov  al,00h                                     ; Selects drive A:
        out  dx,al 
                                                        ;  **** WAIT FOR INTERRUPT ****
        mov   [_done],0                                  ; we need to wait
        call  _WaitDone                                  ; for floppy int.
        jc    _FddRecalibrateError                       ; jump to error exit,if timeout.
       
                                                        ;  **** ISSUE SENSE INTERRUPT STATUS COMMAND ****
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddRecalibrateError                       ; Is CF 1, if so timeout error.

        mov   [_Timer],1                                 ; 5 = 250us
        mov   [_TimerOn],1                               ; start couting
        call  _WaitTimer                                 ; wait for timer

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x08                                   ; Sense Interrupt Status Command
        out   dx,al

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddRecalibrateError                       ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read ST0 ,(DtReg = 0x3f5 )
        in    al,dx
 
        mov   ah,al                                     ; save ST0 in ah

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddRecalibrateError                       ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read PCN ,(DtReg = 0x3f5 )
        in    al,dx

        test  ah,00100000b                              ; test sr0 is 0x20
        jz    _FddRecalibrateError                       ; if not we have a error :-(.
   
        test  ah,00010000b                              ; test sr0 is 0x80
        jnz   _FddRecalibrateError                       ; if not we have a error :-(.
       

_FddRecalibrateSuccsess:                                 ; succsess we hope :-)
        mov [ResultC],0                                 
        clc
        ret

__FddRecalibrateError:                                    ; we have a error (blame M$)
        stc
        ret



 ;----------------------------------------------------;
 ; Fdd read                                           ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          CH  = Track/cylinder                      ;
 ;          CL  = Sector                              ;
 ;          DH  = Head                                ;
 ;          DL  = Drive (only A: drive used 00 )      ;
 ;                                                    ;
 ;  Output:                                           ;
 ;          AH  = Status                              ;
 ;          AL  = Sector number read                  ;
 ;          CF  = 0 If successful                     ;
 ;              = 1 If error                          ;
 ;                                             (100%) ;
 ;....................................................;

__FddRead:
        pushad
        and   dh,00000001b                              ; is it set to head 0 or 1 ?.
        mov   [_Head],dh                                 ; store it.
        shl   dh,2                                      ; make the first 2 bits = 00 for A:.
        mov   [_DriveHead],dh                            ; store it.

        mov   [_FddErrorStatus],0x04                     ; put error code in ah,just incase
        cmp   ch,0x51                                   ; cmp if track number, is above 80 decimal (51 hex) ?.
        jae   _FddReadError                              ; if above jump error.
        mov   [_cTrack],ch                               ; if not above,store it.

        cmp   cl,0x13                                   ; cmp if sector number, is above 18 decimal (12 hex) ?. 
        jae   _FddReadError                              ; if above jump error.
        mov   [_Sector],cl                               ; if not above,store it.
                                                         ; Make shore fdd motor timer is off 
        mov   [_FddMTimer],0	                        ; 36 = 2 seconds
        and   [_FddMotorTimer0n],0                       ; mov floppy time switch off

        test  [_MotorOn],1                               ;  **** ENABLE MOTOR VIA DOR ****
        jnz   @f
        call  _FddMotorOn                               
@@:                                                     ;  **** PROGRAM DATE VIA CCR ****
        mov   dx,_CcrReg                                 ; CcrReg = 0x3f7
        mov   al,00000000b                              ; 500Kb/sec mode
        out   dx,al
        mov   [_FddErrorStatus],0x80                     ; put basic error code,just in case.
                                                        ;  **** RECALIBRATE ****
        ;call  FddRecalibrate                           ; we need to calibrate
        ;jc    FddReadError
        xor    ecx,ecx                                  ;  **** SEEK ****
        mov    cx,3                                     ; we want to try seek 3 times
@b:
        call  _FddReadWriteSeek                          ; we need to move to the, right track.
        jnc   @f                                        ; we should be on the right track.
        
        loop  @b
        jmp   _FddReadError                              ; FddReadError
;@@:
    
        mov   dx,MsReg                                  ; check status reg (to see if DMA bit set)
        in    al,dx 
        test  al,00100000b                              ; test sr0 is 0x80
        jnz   _FddReadError  
                                               
        Call	_Setup_DMA2_Read_fdd                         ;  **** SETUP DMA ****
                                                        ; set up to read, 512 bytes    
                                                        ;  **** ISSUE READ COMMAND ****
;******************************
; 1. Read Sector Command
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.
                                                        
        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0xe6                                   ; read sector Command 
        out   dx,al
;******************************
; 2. Drive
;******************************        
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.
 
        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,[_DriveHead]                            ; Head no. 0, Drive A: 
        out   dx,al
;******************************
; 3. Cylinder
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,DtReg                                  ; DtReg = 0x3f5 
        mov   al,[_cTrack]                               ; Cylinder  
        out   dx,al
;******************************
; 4. Head
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,[_Head]                                 ; Head/side 0 or 1
        out   dx,al
;******************************
; 5. Sector
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,[_Sector]                               ; Sector number,starts at 1
        out   dx,al
;******************************
; 6. Sector Size
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x02                                   ; Sector Size - 512 bytes
        out   dx,al
;******************************
; 7. Sectors to a track
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x12                                   ; 18 decimal sectors to a track. 
        out   dx,al
;******************************
; 8. Gap Length
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0x01B                                  ; 27 should be the Gap Length for a 3 1/2 inch 1.44Mb
        out   dx,al
;******************************
; 9. Data Length
;******************************
        call  _FdcSendByteReady                          ; can we send a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; DtReg = 0x3f5 
        mov   al,0xFF                                   ; not used Data Length, because Sector Size has been filled 
        out   dx,al 

;******************************                         ;  **** FLOPPT INTERRUPT ****
; Wait floppy int
;******************************
        mov   [_done],0                                  ; we need to wait
        call  _WaitDone                                  ; for floppy int.
        jc    _FddReadError                              ; jump to error exit,if timeout.
                                                        ;  **** READ RESULT BYTES ****
        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read ST0 ,(DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultST0],al                            ; save result of ST0 in var

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,DtReg                                  ; Should read PCN ,(DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultST1],al                            ; save result of ST1 in var

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; Should read ST2 ,(DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultST2],al                            ; save result of ST2 in var
                                       
        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; (DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultC],al                              ; save result of cylinder

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; (DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultH],al                              ; save result of head

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; (DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultR],al                              ; save result of sector number.

        call  _FdcGetByteReady                           ; can we get a byte ?.
        jc    _FddReadError                              ; Is CF 1, if so timeout error.

        mov   dx,_DtReg                                  ; (DtReg = 0x3f5 )
        in    al,dx
        mov   [_ResultN],al                              ; save result of sector size

        test  [_ResultST0],11000000b                     ; test sr0 is 0xC0
        jnz   _FddReadError                              ; if so, we have a error :-(.
                                                        
        mov   [_FddErrorStatus],0x00                     ;  **** STATUS PASS? ****
_FddReadSuccsess:                                        ; succsess we hope :-)
        popad
        mov   ah,[_FddErrorStatus]                       ; move error status into ah
        mov   al,[_ResultR]
        clc
        ret


_FddReadError:
        popad
        mov   ah,[_FddErrorStatus]                       ; move error status into ah
        stc
        ret


 ;----------------------------------------------------;
 ; Wait Timer.            ;waits for timer to finish. ;
 ;----------------------------------------------------;

_WaitTimer:
        push  eax
_WaitTimerLoop:
        mov   al,[_TimerOn]
	or    al,al
	jnz   _WaitTimerLoop
        pop   eax
	ret

 ;----------------------------------------------------;
 ; Wait Done                 ;waits for a floppy int. ;
 ;----------------------------------------------------;

_WaitDone:
        mov   [_Timer],30                                ; 20 = about 1 second,we use (1.5 seconds).
        mov   [_TimerOn],1                               ; start couting.
_WaitDoneLoop:
        mov   al,[_TimerOn]                              ; we test if 
	or    al,al                                     ; timeout is up yet?.  
        jz    _WaitDoneError                             ; if it is we exit,with error.
        mov   ax,[_done]                                 ; if not we check for floppt int.
	or    ax,ax
	jz    _WaitDoneLoop                              ; if not do another loop.
        clc                                             ; we end here if we have a int:-)
	ret

_WaitDoneError:                                          ;we end up here if we run out of time:-(.
        stc
        ret

 ;----------------------------------------------------;
 ; FdcSendByteReady   ;floppy control  sendbyte ready ;
 ;----------------------------------------------------;

_FdcSendByteReady:
        push  eax
        push  edx
        mov   [_Timer],30 	                        ; 20 = about 1 second,we use (1.5 seconds).
        mov   [_TimerOn],1                               ; start couting.
_FdcSendByteReadyLoop:
        mov   al,[_TimerOn]                              ; we test if 
	or    al,al                                     ; timeout is up yet?.  
        jz    _FdcSendByteReadyError                     ; if it is we exit,with error.
        mov   dx,_MsReg                                  ; check status reg
        in    al,dx
        and   al,11000000b
        cmp   al,10000000b                              ; are we ok to write
        jnz   _FdcSendByteReadyLoop                      ; if not do another loop.
        pop   edx
        pop   eax
        clc                                             ; we end here if we write:-)
	ret

_FdcSendByteReadyError:                                  ; we end up here if we run out of time:-(.
        pop   edx
        pop   eax
        stc
        ret


 ;----------------------------------------------------;
 ; FdcGetByteReady     ;floppy control  getbyte ready ;
 ;----------------------------------------------------;

_FdcGetByteReady:
        push  eax
        push  edx
        mov   [_Timer],30	                        ; 20 = about 1 second,we use (1.5 seconds).
        mov   [_TimerOn],1                               ; start couting.
_FdcGetByteReadyLoop:
        mov   al,[_TimerOn]                              ; we test if 
	or    al,al                                     ; timeout is up yet?.  
        jz    _FdcGetByteReadyError                      ; if it is we exit,with error.
        mov   dx,MsReg                                  ; check status reg
        in    al,dx
        and   al,11000000b
        cmp   al,11000000b                              ; are we ok to read
        jnz   _FdcGetByteReadyLoop                       ; if not do another loop.
        pop   edx
        pop   eax
        clc                                             ; we end here if we can getbyte:-)
	ret

_FdcGetByteReadyError:                                   ; we end up here if we run out of time:-(.
        pop   edx
        pop   eax
        stc
        ret
