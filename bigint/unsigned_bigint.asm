; Copyright 2019 nzh63
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;   http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

.code
    align 16
    add_proc proc
        ; rcx  e
        ; rdx  &a
        ; r8 &b
        ; r9 &ans
        mov r10, 0
        mov r11, rcx
        and rcx, 3
        jz main_loop
            shr r11, 2
            jz only_once
                clc
                loop_pre:
                    mov rax, [rdx + r10 * 8]
                    adc rax, [r8 + r10 * 8]
                    mov [r9 + r10 * 8], rax
                    inc r10
                    loop loop_pre
                mov rcx, r11
                jmp loop1
        main_loop:
            shr r11, 2
            mov rcx, r11
            clc
        align 16
        loop1:
            mov rax, [rdx + r10 * 8]
            adc rax, [r8 + r10 * 8]
            mov [r9 + r10 * 8], rax

            mov rax, 8[rdx + r10 * 8]
            adc rax, 8[r8 + r10 * 8]
            mov 8[r9 + r10 * 8], rax

            mov rax, 16[rdx + r10 * 8]
            adc rax, 16[r8 + r10 * 8]
            mov 16[r9 + r10 * 8], rax

            mov rax, 24[rdx + r10 * 8]
            adc rax, 24[r8 + r10 * 8]
            mov 24[r9 + r10 * 8], rax

            lea r10, [r10 + 4]

            loop loop1
        ret
        only_once:
            clc
            loop_once:
                mov rax, [rdx + r10 * 8]
                adc rax, [r8 + r10 * 8]
                mov [r9 + r10 * 8], rax
                inc r10
                loop loop_once
        ret
    add_proc endp

    align 16
    add_mix proc
        ; rcx  e
        ; rdx  &a
        ; r8   b
        ; r9   &ans
        mov r10, 0
        mov rax, [rdx + r10 * 8]
        add rax, r8
        mov [r9 + r10 * 8], rax
        jnc end_loop1
        loop1:
            inc r10
            mov rax, [rdx + r10 * 8]
            adc rax, 0
            mov [r9 + r10 * 8], rax
            jc loop1
        end_loop1:
        ret
    add_mix endp

    align 16
    add_three_this proc
        ; rcx  e
        ; rdx  &ans
        ; r8 &a
        ; r9 &b
        push r12
        push r13
        push r14
        mov r10, 0
        mov r13, 4
        mov r14, 0ffffffffffffffffh
        mov r12, 0
        mov r11, rcx
        and rcx, 3
        jz main_loop
            shr r11, 2
            jz only_once
                clc
                loop_pre:
                    mov rax, [rdx + r10 * 8]
                    adcx rax, [r8 + r10 * 8]
                    adox r12, r14
                    adox rax, [r9 + r10 * 8]
                    mov [rdx + r10 * 8], rax
                    mov r12, 0
                    adox r12, r12
                    inc r10
                    loop loop_pre
                mov rcx, r11
                jmp loop1
        main_loop:
            shr r11, 2
            mov rcx, r11
            clc
        align 16
        loop1:
            mov rax, [rdx + r10 * 8]
            adcx rax, [r8 + r10 * 8]
            adox r12, r14
            adox rax, [r9 + r10 * 8]
            mov [rdx + r10 * 8], rax

            mov rax, 8[rdx + r10 * 8]
            adcx rax, 8[r8 + r10 * 8]
            adox rax, 8[r9 + r10 * 8]
            mov 8[rdx + r10 * 8], rax

            mov rax, 16[rdx + r10 * 8]
            adcx rax, 16[r8 + r10 * 8]
            adox rax, 16[r9 + r10 * 8]
            mov 16[rdx + r10 * 8], rax

            mov rax, 24[rdx + r10 * 8]
            adcx rax, 24[r8 + r10 * 8]
            adox rax, 24[r9 + r10 * 8]
            mov 24[rdx + r10 * 8], rax

            mov r12, 0
            adox r12, r12

            adox r10, r13

            loop loop1
        pop r14
        pop r13
        pop r12
        ret
        only_once:
            clc
            loop_once:
                mov rax, [rdx + r10 * 8]
                adcx rax, [r8 + r10 * 8]
                adox r12, r14
                adox rax, [r9 + r10 * 8]
                mov [rdx + r10 * 8], rax
                mov r12, 0
                adox r12, r12
                inc r10
                loop loop_once
        pop r14
        pop r13
        pop r12
        ret
    add_three_this endp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    align 16
    sub_proc proc
        ; rcx  e
        ; rdx  &a
        ; r8 &b
        ; r9 &ans
        mov r10, 0
        mov r11, rcx
        and rcx, 3
        jz main_loop
            shr r11, 2
            jz only_once
                clc
                loop_pre:
                    mov rax, [rdx + r10 * 8]
                    sbb rax, [r8 + r10 * 8]
                    mov [r9 + r10 * 8], rax
                    inc r10
                    loop loop_pre
                mov rcx, r11
                jmp loop1
        main_loop:
            shr r11, 2
            mov rcx, r11
            clc
        align 16
        loop1:
            mov rax, [rdx + r10 * 8]
            sbb rax, [r8 + r10 * 8]
            mov [r9 + r10 * 8], rax

            mov rax, 8[rdx + r10 * 8]
            sbb rax, 8[r8 + r10 * 8]
            mov 8[r9 + r10 * 8], rax

            mov rax, 16[rdx + r10 * 8]
            sbb rax, 16[r8 + r10 * 8]
            mov 16[r9 + r10 * 8], rax

            mov rax, 24[rdx + r10 * 8]
            sbb rax, 24[r8 + r10 * 8]
            mov 24[r9 + r10 * 8], rax

            lea r10, [r10 + 4]

            loop loop1
        ret
        only_once:
            clc
            loop_once:
                mov rax, [rdx + r10 * 8]
                sbb rax, [r8 + r10 * 8]
                mov [r9 + r10 * 8], rax
                inc r10
                loop loop_once
        ret
    sub_proc endp

    align 16
    sub_mix proc
        ; rcx  e
        ; rdx  &a
        ; r8   b
        ; r9   &ans
        mov r10, 0
        mov rax, [rdx + r10 * 8]
        sub rax, r8
        mov [r9 + r10 * 8], rax
        jnc end_loop1
        loop1:
            inc r10
            mov rax, [rdx + r10 * 8]
            sbb rax, 0
            mov [r9 + r10 * 8], rax
            jc loop1
        end_loop1:
        ret
    sub_mix endp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    align 16
    mul_mix proc
        ; rcx  e
        ; rdx  this
        ; r8   b
        ; r9   &ans
        push r12
        push r13
        mov r12, rdx   ;this
        mov r10, 0
        mov r11, 0
        shr rcx, 1
        jnc test2
            mov rdx, [r12 + r10 * 8]
            mulx r11, rax, r8
            mov [r9 + r10 * 8], rax
            inc r10
            test2:
                shr rcx, 1
                jz only_once
                jnc main_loop
                    mov rdx, [r12 + r10 * 8]
                    mulx r13, rax, r8
                    add rax, r11
                    mov [r9 + r10 * 8], rax

                    mov rdx, 8[r12 + r10 * 8]
                    mulx r11, rax, r8
                    adc rax, r13
                    mov 8[r9 + r10 * 8], rax
                    lea r10, [r10 + 2]

        align 16
        main_loop:
        loop1:
            mov rdx, [r12 + r10 * 8]
            mulx r13, rax, r8
            adc rax, r11
            mov [r9 + r10 * 8], rax

            mov rdx, 8[r12 + r10 * 8]
            mulx r11, rax, r8
            adc rax, r13
            mov 8[r9 + r10 * 8], rax

            mov rdx, 16[r12 + r10 * 8]
            mulx r13, rax, r8
            adc rax, r11
            mov 16[r9 + r10 * 8], rax

            mov rdx, 24[r12 + r10 * 8]
            mulx r11, rax, r8
            adc rax, r13
            mov 24[r9 + r10 * 8], rax

            lea r10, [r10 + 4]

            loop loop1
        return:
        pop r13
        pop r12
        ret
        only_once:
        jnc return
            mov rdx, [r12 + r10 * 8]
            mulx r13, rax, r8
            add rax, r11
            mov [r9 + r10 * 8], rax

            mov rdx, 8[r12 + r10 * 8]
            mulx r11, rax, r8
            adc rax, r13
            mov 8[r9 + r10 * 8], rax
        pop r13
        pop r12
        ret
    mul_mix endp

    align 16
    mul_mix_add proc
        ; rcx  e
        ; rdx  this
        ; r8   b
        ; r9   &ans
        push r12
        push r13
        push r14
        mov r12, rdx   ;this
        mov r14, rcx
        and r14, 3
        jz not_after
        shr rcx, 2
        jz only_once
        xor r10, r10
        xor r11, r11

        align 16
        main_loop:
        loop1:
            mov rdx, [r12 + r10 * 8]
            mulx r13, rax, r8
            adcx rax, r11
            adox rax, [r9 + r10 * 8]
            mov [r9 + r10 * 8], rax

            mov rdx, 8[r12 + r10 * 8]
            mulx r11, rax, r8
            adcx rax, r13
            adox rax, 8[r9 + r10 * 8]
            mov 8[r9 + r10 * 8], rax

            mov rdx, 16[r12 + r10 * 8]
            mulx r13, rax, r8
            adcx rax, r11
            adox rax, 16[r9 + r10 * 8]
            mov 16[r9 + r10 * 8], rax

            mov rdx, 24[r12 + r10 * 8]
            mulx r11, rax, r8
            adcx rax, r13
            adox rax, 24[r9 + r10 * 8]
            mov 24[r9 + r10 * 8], rax

            lea r10, [r10 + 4]

            loop loop1
        jmp loop_after_begin
        only_once:
            xor r10, r10
            xor r11, r11
        loop_after_begin:
            mov rcx, r14
        loop_after:
            mov rdx, [r12 + r10 * 8]
            mulx r13, rax, r8
            adcx rax, r11
            adox rax, [r9 + r10 * 8]
            mov [r9 + r10 * 8], rax
            mov r11, r13
            loop loop_after
        pop r14
        pop r13
        pop r12
        ret
        align 16
        not_after:
        shr rcx, 2
        xor r10, r10
        xor r11, r11
        loop2:
            mov rdx, [r12 + r10 * 8]
            mulx r13, rax, r8
            adcx rax, r11
            adox rax, [r9 + r10 * 8]
            mov [r9 + r10 * 8], rax

            mov rdx, 8[r12 + r10 * 8]
            mulx r11, rax, r8
            adcx rax, r13
            adox rax, 8[r9 + r10 * 8]
            mov 8[r9 + r10 * 8], rax

            mov rdx, 16[r12 + r10 * 8]
            mulx r13, rax, r8
            adcx rax, r11
            adox rax, 16[r9 + r10 * 8]
            mov 16[r9 + r10 * 8], rax

            mov rdx, 24[r12 + r10 * 8]
            mulx r11, rax, r8
            adcx rax, r13
            adox rax, 24[r9 + r10 * 8]
            mov 24[r9 + r10 * 8], rax

            lea r10, [r10 + 4]

            loop loop2
        pop r14
        pop r13
        pop r12
        ret
    mul_mix_add endp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    align 16
    mod_mix proc
        ; rcx  e
        ; rdx  this
        ; r8   m
        push rbx
        push r12
        mov r12, rdx   ;this
        mov r10, 0
        mov rbx, 0
        mov r11, 1 ;(2^64)^i % m
        loop1:
            mov rax, [r12 + r10 * 8]  ;第。。。位
            mov rdx, 0
            div r8
            mov rax, rdx    ;余数*2^(64*i)
            mul r11
            div r8
            add rdx, rbx
            mov rax, 0
            adc rax, 0
            xchg rax, rdx
            div r8
            mov rbx, rdx
            mov rdx, r11
            mov rax, 0
            div r8            ;*2^64
            mov r11, rdx
            inc r10
            loop loop1
        mov rax, rbx
        pop r12
        pop rbx
        ret
    mod_mix endp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    align 16
    div_128 proc
        ; rcx  ax
        ; rdx  dx
        ; r8   div
        ; r9   &ans
        mov rax, rcx
        div r8
        ret
    div_128 endp
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    align 16
    real_rand proc
        ; rcx  e
        ; rdx  this
        sub rdx, 8
        loop1:
            rdrand rax
            mov [rdx + rcx * 8], rax
            loop loop1
        ret
    real_rand endp
end
