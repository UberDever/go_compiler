.586
.model flat, stdcall

include <\masm32\include\msvcrt.inc>
include <\masm32\include\kernel32.inc>
includelib <\masm32\lib\msvcrt.lib>
includelib <\masm32\lib\kernel32.lib>

data segment

   sqrtOp dd 0
   print_0 db "%d", 0
   print_1 db "%s", 0
   print_2 db "%d ", 0
   print_3 db "%s ", 0
   print_nl db 10, 0
   LIT3534916879 db "fmt", 0
   LIT2971646047 db "It will not be printed", 0
   LIT1722247484 db "math", 0
   LIT4057494348 db "It will be printed eventually, gamma:", 0
   LIT3068837375 db "And c:", 0
   LIT2212957155 db "I can print like anything!", 0
   LIT2791304878 db "It is b:", 0

data ends

text segment

gamma_int_1 = -8
j_int_1 = -12
k5_int_11 = -16
k1_int_3 = -20
k2_int_5 = -24
b_int_1 = -28
k3_int_7 = -32
c_int_4 = -36
a_int_1 = -40
beta_int_1 = -44
alpha_int_1 = -48
i_int_2 = -52
alpha_int_3 = -56
t_int_1 = -60
k4_int_9 = -64

print PROC
   enter 0, 0
   mov ecx, [ebp + 8]
   mov eax, [ebp + 12]
   invoke crt_printf, ecx, eax
   leave
   ret 4
print ENDP

__start:

   enter 64, 0
   push 10
   pop eax
   mov a_int_1[ebp], eax
   push 2
   pop eax
   imul eax, -1
   push eax
   push 4
   pop eax
   pop ebx
   imul eax, ebx
   push eax
   push 9
   pop eax
   pop ebx
   add eax, ebx
   push eax
   pop eax
   mov t_int_1[ebp], eax
   push 5
   pop eax
   mov j_int_1[ebp], eax
   push 400
   pop eax
   imul eax, -1
   push eax
   push 1000
   pop eax
   pop ebx
   add eax, ebx
   push eax
   push 32
   mov edx, 0
   pop ebx
   cdq
   pop eax
   idiv ebx
   push eax
   pop eax
   mov alpha_int_1[ebp], eax
   push 8400
   pop eax
   mov beta_int_1[ebp], eax
   push 7
   pop eax
   imul eax, -1
   push eax
   push 3
   mov edx, 0
   pop ebx
   cdq
   pop eax
   idiv ebx
   push eax
   pop eax
   mov gamma_int_1[ebp], eax
   push 25
   pop sqrtOp
   finit
   fild sqrtOp
   fsqrt
   fistp sqrtOp
   push sqrtOp
   pop eax
   mov b_int_1[ebp], eax
   push 25
   pop sqrtOp
   finit
   fild sqrtOp
   fsqrt
   fistp sqrtOp
   push sqrtOp
   mov ecx, 0
   push alpha_int_1[ebp]
   push offset print_2
   call print
   push beta_int_1[ebp]
   push offset print_2
   call print
   push gamma_int_1[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   mov ecx, 0
   push offset LIT2212957155
   push offset print_3
   call print
   push 1
   push offset print_2
   call print
   push 2
   push offset print_2
   call print
   push 3
   push 8
   push 2
   mov edx, 0
   pop ebx
   cdq
   pop eax
   idiv ebx
   push eax
   push 4
   pop eax
   pop ebx
   imul eax, ebx
   push eax
   push 4
   pop ebx
   pop eax
   sub eax, ebx
   push eax
   pop eax
   pop ebx
   add eax, ebx
   push eax
   push offset print_2
   call print
   push gamma_int_1[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   push 0
   pop eax
   mov i_int_2[ebp], eax
LOOPSTART_26168:
   push i_int_2[ebp]
   pop ecx
   push a_int_1[ebp]
   pop edx
   cmp ecx, edx
   jge CMPNE_19763
   push 1
   jmp CMPEND_19763
CMPNE_19763:
   push 0
CMPEND_19763:
   pop eax
   cmp eax, 0
   je LOOPEND_26168
   push 10
   pop eax
   mov alpha_int_1[ebp], eax
   push 3
   push 3
   pop eax
   pop ebx
   imul eax, ebx
   push eax
   push 4
   push 4
   pop eax
   pop ebx
   imul eax, ebx
   push eax
   pop eax
   pop ebx
   add eax, ebx
   push eax
   pop sqrtOp
   finit
   fild sqrtOp
   fsqrt
   fistp sqrtOp
   push sqrtOp
   pop eax
   mov c_int_4[ebp], eax
   push b_int_1[ebp]
   pop ecx
   push 5
   pop edx
   cmp ecx, edx
   jne CMPNE_27482
   push 1
   jmp CMPEND_27482
CMPNE_27482:
   push 0
CMPEND_27482:
   pop eax
   cmp eax, 0
   je IFELSE_434450
   mov ecx, 0
   push offset LIT2791304878
   push offset print_3
   call print
   push b_int_1[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   mov ecx, 0
   push offset LIT3068837375
   push offset print_3
   call print
   push c_int_4[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   push 8
   pop eax
   mov b_int_1[ebp], eax
   jmp IFEND_43445
IFELSE_434450:
   push 3
   pop eax
   mov c_int_4[ebp], eax
   push a_int_1[ebp]
   pop ecx
   push 5
   pop edx
   cmp ecx, edx
   jge CMPNE_3217
   push 1
   jmp CMPEND_3217
CMPNE_3217:
   push 0
CMPEND_3217:
   pop eax
   cmp eax, 0
   je IFELSE_434451
   mov ecx, 0
   push offset LIT2971646047
   push offset print_3
   call print
   push offset print_nl
   call print
   jmp IFEND_43445
IFELSE_434451:
   push gamma_int_1[ebp]
   pop ecx
   push 0
   pop edx
   cmp ecx, edx
   jge CMPNE_46505
   push 1
   jmp CMPEND_46505
CMPNE_46505:
   push 0
CMPEND_46505:
   pop eax
   cmp eax, 0
   je IFELSE_434452
   mov ecx, 0
   push offset LIT4057494348
   push offset print_3
   call print
   push gamma_int_1[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   mov ecx, 0
   push offset LIT3068837375
   push offset print_3
   call print
   push c_int_4[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
   jmp LOOPEND_26168
   jmp IFEND_43445
IFELSE_434452:
IFEND_43445:
LOOPINC_26168:
   push i_int_2[ebp]
   pop eax
   inc eax
   mov i_int_2[ebp], eax
   jmp LOOPSTART_26168
LOOPEND_26168:
LOOPSTART_61308:
   push 0
   pop eax
   mov k1_int_3[ebp], eax
   jmp LOOPEND_61308
LOOPSTART_99955:
   push 0
   pop eax
   mov k2_int_5[ebp], eax
   jmp LOOPEND_99955
LOOPSTART_82496:
   push 0
   pop eax
   mov k3_int_7[ebp], eax
   jmp LOOPEND_82496
LOOPSTART_63414:
   push 0
   pop eax
   mov k4_int_9[ebp], eax
   jmp LOOPEND_63414
LOOPSTART_83070:
   push 0
   pop eax
   mov k5_int_11[ebp], eax
   jmp LOOPEND_83070
   jmp LOOPSTART_83070
LOOPEND_83070:
   jmp LOOPSTART_63414
LOOPEND_63414:
   jmp LOOPSTART_82496
LOOPEND_82496:
   jmp LOOPSTART_99955
LOOPEND_99955:
   jmp LOOPSTART_61308
LOOPEND_61308:

   leave

   ret

text ends

end __start

