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
   LIT1921426629 db "I can do if:", 0
   LIT3323870820 db "i is ", 0
   LIT2134342519 db "You just broke Euclidean math, good job!", 0
   LIT223699673 db "Hypotenuse is indeed 5", 0
   LIT1722247484 db "math", 0
   LIT558051034 db "I am a simple program, let me introduce myself:", 0
   LIT3506387599 db "I can do for:", 0

data ends

text segment

hyp_int_2 = -8
i_int_2 = -12

print PROC
   enter 0, 0
   mov ecx, [ebp + 8]
   mov eax, [ebp + 12]
   invoke crt_printf, ecx, eax
   leave
   ret 4
print ENDP

__start:

   enter 12, 0
   mov ecx, 0
   push offset LIT558051034
   push offset print_3
   call print
   push offset print_nl
   call print
   mov ecx, 0
   push offset LIT3506387599
   push offset print_3
   call print
   push offset print_nl
   call print
   push 0
   pop eax
   mov i_int_2[ebp], eax
LOOPSTART_96531:
   push i_int_2[ebp]
   pop ecx
   push 10
   pop edx
   cmp ecx, edx
   jge CMPNE_21044
   push 1
   jmp CMPEND_21044
CMPNE_21044:
   push 0
CMPEND_21044:
   pop eax
   cmp eax, 0
   je LOOPEND_96531
   mov ecx, 0
   push offset LIT3323870820
   push offset print_3
   call print
   push i_int_2[ebp]
   push offset print_2
   call print
   push offset print_nl
   call print
LOOPINC_96531:
   push i_int_2[ebp]
   pop eax
   inc eax
   mov i_int_2[ebp], eax
   jmp LOOPSTART_96531
LOOPEND_96531:
   mov ecx, 0
   push offset LIT1921426629
   push offset print_3
   call print
   push offset print_nl
   call print
   push 5.
   pop eax
   mov hyp_int_2[ebp], eax
   push hyp_int_2[ebp]
   pop ecx
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
   pop edx
   cmp ecx, edx
   jne CMPNE_41883
   push 1
   jmp CMPEND_41883
CMPNE_41883:
   push 0
CMPEND_41883:
   pop eax
   cmp eax, 0
   je IFELSE_378250
   mov ecx, 0
   push offset LIT223699673
   push offset print_3
   call print
   push offset print_nl
   call print
   jmp IFEND_37825
IFELSE_378250:
   mov ecx, 0
   push offset LIT2134342519
   push offset print_3
   call print
   push offset print_nl
   call print
IFEND_37825:

   leave

   ret

text ends

end __start

