; ------------------------
; protected mode start
[org 0x7e00]
[bits 32]

pm_start:
	mov ax, 0x10        ; data segment selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x90000    ; stack somewhere safe

	; set up color
	mov al, [background_color]
	rol al, 4
	or al, [text_color]
	mov [color], al

	call clear

	mov edi, 0xb8000
	mov esi, welcomeMsg
	call print

	jmp inputloop

inputloop:
	call getkey

	mov al, byte [kbuf]
	call printchar

	jmp inputloop

; ------------------------
; functions

print_byte:
	call print_nibble
	call print_nibble
	ret
print_nibble:
	ror eax, 4
	mov bl, al
	and bl, 0b00001111

	cmp bl, 10
	jge nibble_is_letter
	; nibble is *not* letter
	add bl, 0x30
	push eax
	mov al, bl
	call printchar
	pop eax
	ret
nibble_is_letter:
	add bl, 0x37
	push eax
	mov al, bl
	call printchar
	pop eax
	ret

done:
	ret

printchar:
	cmp al, 0x08
	je .backspace
	cmp al, 0x0d
	je .carriageret
	cmp al, 0x0a
	je .newline
	mov ah, [color]
	stosw

	ret
.backspace:
	sub edi, 2
	mov ah, [color]
	mov al, 0x20
	stosw
	sub edi, 2

	ret
.newline:
	inc byte [line]
	add edi, 0xa0 ; move ahead 80 words = 1 line

	ret
.carriageret:
	movzx eax, byte [line]
	imul eax, 0xa0
	add eax, 0xb8000
	mov edi, eax

	ret

print:
	lodsb
	or al, al
	jz done
	call printchar
	jmp print

getkey:
	in al, 0x64
	test al, 1
	jz getkey
	cmp al, 0xe0
	je getkey
	cmp al, 0xe1
	je getkey

	in al, 0x60
	test al, 0b10000000
	jnz getkey

	push ebx
	movzx eax, al            ; zero-extend AL to EAX
	mov bl, [scancode_table + eax]
	mov byte [kbuf], bl
	mov byte [kbuf+1], 0
	pop ebx
	ret

clear:
	mov edi, 0xb8000        ; VGA mem start
	mov ecx, 80*25          ; total characters
	mov ah, [color]         ; color
	mov al, 0x20            ; space
	rep stosw               ; write ECX words
	ret

; ------------------------
; config - build after modifying to apply changes

; --- colors ---
; 0 = Black
; 1 = Blue
; 2 = Green
; 3 = Cyan
; 4 = Red
; 5 = Magenta
; 6 = Brown
; 7 = Light Gray
; 8 = Dark Gray
; 9 = Light Blue
; 10 = Light Green
; 11 = Light Cyan
; 12 = Light Red
; 13 = Light Magenta
; 14 = Yellow
; 15 = White

background_color db 8
text_color db 1
color db 0

; ------------------------
; data
welcomeMsg 	db "  /\----/\",0x0d,0x0a
		db " /  \  /  \",0x0d,0x0a
		db "| terminal |",0x0d,0x0a
		db " \  /OS\  /",0x0d,0x0a
		db "  \/----\/",0x0d,0x0a

kbuf db 0,0
line db 0

scancode_table:
	db "..............", 0x08, "." ; 0x
	db "qwertyuiop....as" ; 1x
	db "dfghjkl.....zxcv" ; 2x
	db "bnm.....@ ......" ; 3x
	db "................" ; 4x
	db "................" ; 5x
	db "................" ; 6x
	db "................" ; 7x
	db "................" ; 8x
	db "................" ; 9x
	db "................" ; ax
	db "................" ; bx
	db "................" ; cx
	db "................" ; dx
	db "................" ; ex
	db "................" ; fx

mbuf:
	times 2048 db 0

times 4096-($-$$) db 0