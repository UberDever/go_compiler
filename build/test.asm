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
LOOPSTART_82716:
   push i_int_2[ebp]
   pop ecx
   push a_int_1[ebp]
   pop edx
   cmp ecx, edx
   jge CMPNE_2487
   push 1
   jmp CMPEND_2487
CMPNE_2487:
   push 0
CMPEND_2487:
   pop eax
   cmp eax, 0
   je LOOPEND_82716
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
   jne CMPNE_8408
   push 1
   jmp CMPEND_8408
CMPNE_8408:
   push 0
CMPEND_8408:
   pop eax
   cmp eax, 0
   je IFELSE_221700
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
   jmp IFEND_22170
IFELSE_221700:
   push 3
   pop eax
   mov c_int_4[ebp], eax
   push a_int_1[ebp]
   pop ecx
   push 5
   pop edx
   cmp ecx, edx
   jge CMPNE_73736
   push 1
   jmp CMPEND_73736
CMPNE_73736:
   push 0
CMPEND_73736:
   pop eax
   cmp eax, 0
   je IFELSE_221701
   mov ecx, 0
   push offset LIT2971646047
   push offset print_3
   call print
   push offset print_nl
   call print
   jmp IFEND_22170
IFELSE_221701:
   push gamma_int_1[ebp]
   pop ecx
   push 0
   pop edx
   cmp ecx, edx
   jge CMPNE_77514
   push 1
   jmp CMPEND_77514
CMPNE_77514:
   push 0
CMPEND_77514:
   pop eax
   cmp eax, 0
   je IFELSE_221702
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
   jmp LOOPEND_82716
   jmp IFEND_22170
IFELSE_221702:
IFEND_22170:
LOOPINC_82716:
   push i_int_2[ebp]
   pop eax
   inc eax
   mov i_int_2[ebp], eax
   jmp LOOPSTART_82716
LOOPEND_82716:
LOOPSTART_31328:
   push 0
   pop eax
   mov k1_int_3[ebp], eax
   jmp LOOPEND_31328
LOOPSTART_52425:
   push 0
   pop eax
   mov k2_int_5[ebp], eax
   jmp LOOPEND_52425
LOOPSTART_682:
   push 0
   pop eax
   mov k3_int_7[ebp], eax
   jmp LOOPEND_682
LOOPSTART_30622:
   push 0
   pop eax
   mov k4_int_9[ebp], eax
   jmp LOOPEND_30622
LOOPSTART_24918:
   push 0
   pop eax
   mov k5_int_11[ebp], eax
   jmp LOOPEND_24918
   jmp LOOPSTART_24918
LOOPEND_24918:
   jmp LOOPSTART_30622
LOOPEND_30622:
   jmp LOOPSTART_682
LOOPEND_682:
   jmp LOOPSTART_52425
LOOPEND_52425:
   jmp LOOPSTART_31328
LOOPEND_31328:

   leave

   ret

text ends

end __start

