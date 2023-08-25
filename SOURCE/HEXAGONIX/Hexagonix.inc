; flat assembler interface for Linux
; Copyright (c) 1999-2021, Tomasz Grysztar.
; All rights reserved.

; Hexagonix system interface for flat assembler
; Copyright (c) 2015-2023, Felipe Miguel Nery Lunkes
; All rights reserved.

;;************************************************************************************
;;
;; Adaptação para execução no Hexagonix®
;;
;;************************************************************************************

;; Entrada:
;;
;; [linha_comando] = Aponta para a linha fornecida pelo sistema
;; 
;; Saída:
;;
;; [input_file]  - Nome do código fonte
;; [output_file] - Nome do binário a ser gerado
;;
;;	CF definido em caso de erro

obterParametros:

	mov esi, [linha_comando]
	mov [input_file], esi
		
	cmp byte[esi], 0
	je .parametrosInvalidos
	
	mov [symbols_file], 0
	mov [memory_setting], 0
	
	mov [passes_limit], 100
	
	mov al, ' '

	hx.syscall encontrarCaractere

	jc .unicoArgumento

	mov al, ' '

	call encontrarString
	
	mov [output_file], esi
	jmp .pronto
	
.unicoArgumento:

	mov [output_file], 0

.pronto:

	clc

	ret
	
.parametrosInvalidos:

	stc

	ret

;;************************************************************************************

;; Procurar um caractere específico em uma string
;;
;; Entrada:
;;
;; ESI - String
;; AL  - Caractere para pesquisar
;; 
;; Saída:
;; 
;; ESI - Posição do caractere

encontrarString:

	lodsb
	
	cmp al, ' '
	je .pronto
	
	jmp encontrarString

.pronto:

	mov byte[esi-1], 0

	ret

;;************************************************************************************

finalizarProcesso:

	mov es, [regES]
	
	mov ebx, 00h
	
	hx.syscall encerrarProcesso

	ret

;;************************************************************************************

;; Entrada:
;;
;; ESI - String

exibirString:

	push ebx

    imprimirString

	pop ebx

	ret

;;************************************************************************************

;; Entrada:
;;
;; AL - Caractere

exibirCaractere:

	push ebx

	mov al, dl

	hx.syscall imprimirCaractere

	pop ebx

	ret

;;************************************************************************************

;; Entrada:
;;
;; EAX - Número

exibirNumero:

	push ebx

	imprimirInteiro

	pop ebx

	ret

;;************************************************************************************

;; Entrada:
;;
;; ESI - String
;; ECX - Caracteres para exibir

exibirBloco:

	push ebx

.loop:

	lodsb

	hx.syscall imprimirCaractere

	loop .loop
	
	pop ebx

	ret

;;************************************************************************************

;; Entrada:
;;
;; EDX - Ponteiro para o nome de arquivo
;; EBX - Manipulador do arquivo (ignorado)
;;
;; Saída:
;;
;; CF definido quando o arquivo não existe

abrirArquivo:

	pushad
	
	mov esi, edx

	hx.syscall tamanhoString

	inc eax
	
	mov ecx, eax
	mov esi, edx
	mov edi, nomeArquivo

	rep movsb
	
	popad
	pushad
		
	mov esi, nomeArquivo
	mov edi, endBuffer + 1024*10000 + 1024*3000

	hx.syscall abrir

	jc .erro

	mov [fileSize], eax
	
	clc

	popad

	mov ebx, 3

	ret

.erro:

	stc

	popad

	ret

;;************************************************************************************

;; Entrada:
;;
;; EDX - Ponteiro para o nome do arquivo
;; EBX - Manipulador do arquivo (ignorado)
;;
;; Saída:
;;
;; CF definido em caso de erro

criarArquivo:

	pushad
	
	mov esi, edx

	hx.syscall tamanhoString

	inc eax
	
	mov ecx, eax
	mov esi, edx
	mov edi, fileNameW

	rep movsb
	
	mov esi, fileNameW

	hx.syscall deletarArquivo
	
	popad
	pushad
	
	mov eax, 0
	mov esi, fileNameW
	mov edi, fileNameW

	hx.syscall salvarArquivo		

	jnc .successo		
	
.errro:

	mov al, 'C'

	hx.syscall imprimirCaractere

	stc

	jmp .fim
	
.successo:

	clc

.fim:	

	popad

	mov ebx, 3

	ret

;;************************************************************************************

;; Entrada:
;; 
;; EBX - Manipulador do arquivo (ignorado)
;; ECX - Número de bytes para ler 
;; EDX - Buffer para ler
;;
;; Saída:
;;
;; CF definido em caso de erro

lerArquivo:

	pushad
	
	mov esi, endBuffer + 1024*10000 + 1024*3000	

	add esi, [filePosition]

	mov edi, edx

	rep movsb
	
	popad

	clc

	ret

;;************************************************************************************

;; Entrada:
;;
;; EBX - Manipulador do arquivo (ignorado)
;; ECX - Bytes para ler
;; EDX - Dados para ler
;;
;; Saída:
;;
;; CF definido em caso de erro

escreverArquivo:

	mov esi, fileNameW

	hx.syscall deletarArquivo
	
	mov eax, ecx
	mov edi, edx
	mov esi, fileNameW

	hx.syscall salvarArquivo

	jc .erro

	clc

	ret

.erro:

	ret

;;************************************************************************************

;; Mover para determinada posição no arquivo
;;
;; Entrada:
;;
;; EDX - Bytes para pular
;; AL  - Origem (0 = início, 1 = posição atual, 2 = fim)
;;
;; Saída:
;;
;; EAX - Posição no arquivo

moverPosicao:

	pushad

	cmp al, 0	
	je .doInicio

	cmp al, 1	
	je .posicaoAtual

	cmp al, 2
	je .doFim

	jmp .fim
	
.doInicio:	

	mov [filePosition], edx

	jmp .fim

.posicaoAtual:

	add [filePosition], edx

	jmp .fim
	
.doFim:

	mov eax, [fileSize]
	mov [filePosition], eax

	add [filePosition], edx

.fim:

	popad

	mov eax, [filePosition]
	
	ret

;;************************************************************************************

;;************************************************************************************
;;
;; Fim da implementação específica para o Hexagonix®
;;
;;************************************************************************************

; flat assembler
; Copyright (c) 1999-2022, Tomasz Grysztar.
; All rights reserved.

; Hexagonix system interface for flat assembler
; Copyright (c) 2015-2023, Felipe Miguel Nery Lunkes
; All rights reserved.

;; Redirecionamento de funções e adaptações para execução no Hexagonix®

get_params:

	jmp obterParametros

;;************************************************************************************

findChar:

	jmp encontrarString

;;************************************************************************************

get_environment_variable:

	ret

make_timestamp:

	ret

;;************************************************************************************

exit_program:

	jmp finalizarProcesso

;;************************************************************************************

print_string:

	jmp exibirString

;;************************************************************************************

display_character:

	jmp exibirCaractere

;;************************************************************************************

display_number:

	jmp exibirNumero

;;************************************************************************************

display_block:

	jmp exibirBloco

;;************************************************************************************

;________________________________________________
;Initialize memory
;IN:	nothing
;OUT:	[memory_start] 	= Start of free main memory block
;	[memory_end] 	= End of free main memory block
;	[additional_memory] 	= Start of additional memory
;	[additional_memory_end] = End of additional memory
;

init_memory:

	mov eax, esp
	add eax, 1000h-10000h
	
	mov [stack_limit], eax
	
	mov [memory_start], endBuffer
	mov [memory_end], endBuffer + 1024*10000
	
	mov [additional_memory], endBuffer + 1024*10000
	mov [additional_memory_end], endBuffer + 1024*10000 + 1024*3000
	
	ret

;;************************************************************************************

open:

	jmp abrirArquivo

;;************************************************************************************

create:

	jmp criarArquivo

;;************************************************************************************

close:

	stc

	ret

;;************************************************************************************

read:

	jmp lerArquivo

;;************************************************************************************

write:

	jmp escreverArquivo

;;************************************************************************************

lseek:

	jmp moverPosicao

;;************************************************************************************

;________________________________________________
;Display error and sair
;IN:	Pointer to error message on stack
;OUT:	nothing
;

fatal_error:

	mov esi, error_prefix
	call print_string
	
	pop esi			;Error message 
	call print_string
	
	mov esi, error_suffix
	call print_string
	
	jmp exit_program

;________________________________________________
;Display user messages
;IN/OUT: nothing
;	
display_user_messages:
	mov [displayed_count],0
	call show_display_buffer
	
	cmp [displayed_count],0
	je .line_break_ok
	
	cmp [last_displayed],0Ah
	je .line_break_ok
	
	mov dl,0Ah
	call display_character
	
.line_break_ok:
	ret
	
;________________________________________________
;Display assembler error and sair
;IN/OUT: see fasmguide.txt
;	
assembler_error:
	call	display_user_messages
	push	dword 0
	mov	ebx,[current_line]
      get_error_lines:
	mov	eax,[ebx]
	cmp	byte [eax],0
	je	get_next_error_line
	push	ebx
	test	byte [ebx+7],80h
	jz	display_error_line
	mov	edx,ebx
      find_definition_origin:
	mov	edx,[edx+12]
	test	byte [edx+7],80h
	jnz	find_definition_origin
	push	edx
      get_next_error_line:
	mov	ebx,[ebx+8]
	jmp	get_error_lines
      display_error_line:
	mov	esi,[ebx]
	call	print_string
	mov	esi,line_number_start
	call	print_string
	mov	eax,[ebx+4]
	and	eax,7FFFFFFFh
	call	display_number
	mov	dl,']'
	call	display_character
	pop	esi
	cmp	ebx,esi
	je	line_number_ok
	mov	dl,20h
	call	display_character
	push	esi
	mov	esi,[esi]
	movzx	ecx,byte [esi]
	inc	esi
	call	display_block
	mov	esi,line_number_start
	call	print_string
	pop	esi
	mov	eax,[esi+4]
	and	eax,7FFFFFFFh
	call	display_number
	mov	dl,']'
	call	display_character
      line_number_ok:
	mov	esi,line_data_start
	call	print_string
	mov	esi,ebx
	mov	edx,[esi]
	call	open
	mov	al,2
	xor	edx,edx
	call	lseek
	mov	edx,[esi+8]
	sub	eax,edx
	push	eax
	xor	al,al
	call	lseek
	mov	ecx,[esp]
	mov	edx,[additional_memory]
	lea	eax,[edx+ecx]
	cmp	eax,[additional_memory_end]
	ja	out_of_memory
	call	read
	call	close
	pop	ecx
	mov	esi,[additional_memory]
      get_line_data:
	mov	al,[esi]
	cmp	al,0Ah
	je	display_line_data
	cmp	al,0Dh
	je	display_line_data
	cmp	al,1Ah
	je	display_line_data
	or	al,al
	jz	display_line_data
	inc	esi
	loop	get_line_data
      display_line_data:
	mov	ecx,esi
	mov	esi,[additional_memory]
	sub	ecx,esi
	call	display_block
	mov	esi,lf
	call	print_string
	pop	ebx
	or	ebx,ebx
	jnz	display_error_line
	mov	esi,error_prefix
	call	print_string
	pop	esi
	call	print_string
	mov	esi,error_suffix
	call	print_string
	mov	al,2
	jmp	exit_program
;______________________________________________________________________________

error_prefix db 'Error: ',0
error_suffix db '.'
lf db 0xA,0
line_number_start db ' [',0
line_data_start db ':',0xA,0