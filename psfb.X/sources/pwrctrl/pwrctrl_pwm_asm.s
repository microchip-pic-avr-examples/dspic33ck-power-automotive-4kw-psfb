; **********************************************************************************
;  SDK Version: PowerSmart Digital Control Library Designer v1.9.15.709
;  CGS Version: Code Generator Script v3.0.11 (01/06/2022)
;  Author:      M70027
;  Date/Time:   12/13/2024 13:54:07
; **********************************************************************************
;  3P3Z Control Library File (Dual Bitshift-Scaling Mode)
; **********************************************************************************
    
;------------------------------------------------------------------------------
;file start
    .nolist                                 ; (no external dependencies)
    .list                                   ; list of all external dependencies
    
;------------------------------------------------------------------------------
;local inclusions.
    .section .data                          ; place constant data in the data section
    
;------------------------------------------------------------------------------
;include files and global constants.
    .equ PGxSTAT_UPDREQ,      3             ; bit position of the UPDREQ control bit within the PGxSTAT register
 
;data object member address offsets
    .equ PhaseShift, 0
    .equ ptrPeriod, 2
    .equ ptrPhaseShift, 4
    .equ ptrDCSRR, 6
    .equ ptrDCSRL, 8
    .equ ptrUpdateReg, 10
    
;------------------------------------------------------------------------------
;source code section.
    .section .text                          ; place code in the text section
    
;------------------------------------------------------------------------------
; Global function declaration
; This function calls the z-domain controller processing the latest data point input
;------------------------------------------------------------------------------

    .global _PwrCtrl_PWM_Update             ; provide global scope to routine
    _PwrCtrl_PWM_Update:                    ; local function label
    
    ; Save working registers
    push.s                                  ; save shadowed working registers (WREG0, WREG1, WREG2, WREG3)

    ; Distribute control output across PWM channel registers
    ; w0 already points to the PhaseShift data member
    mov [w0 + #ptrPeriod], w1               ; load pointer to period register into working register
    mov [w1], w2                            ; load value of period register into working register
    sub w2, w0, w1                          ; subtract phase shift value from period value
    mov [w0 + #ptrDCSRR], w2                ; load pointer address of right leg SR duty cycle register
    mov [w0 + #ptrDCSRL], w3                ; load pointer address of left leg SR duty cycle register
    mov w1, [w2]                            ; move SR on-time value to right SR duty cycle register
    mov w1, [w3]                            ; move SR on-time value to left SR duty cycle register
    mov [w0 + #ptrPhaseShift], w1           ; load pointer address to phase shift register
    mov w0, [w1]                            ; move value of phase shift to register
    
    mov [w0 + #ptrUpdateReg], w1            ; load pointer to PGxSTAT register into working register
    mov [w1], w2                            ; load value of PGxSTAT register into working register
    bset w2, #PGxSTAT_UPDREQ                ; manually setting the update bit
    mov w2, [w1]                            ; push value back to PGxSTAT register
    
    ; Restore working registers in reverse order
    pop.s                                   ; restore shadowed working registers (WREG0, WREG1, WREG2, WREG3)
    
    ; End of routine
    return                                  ; end of function; return to caller
;------------------------------------------------------------------------------
    
;------------------------------------------------------------------------------
; End of file
    .end                                    ; end of file VCOMP_asm.s
;------------------------------------------------------------------------------

     
; **********************************************************************************
;  Download latest version of this tool here: https://www.microchip.com/powersmart
; **********************************************************************************
    


