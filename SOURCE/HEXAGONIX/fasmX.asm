
; flat assembler interface for Linux
; Copyright (c) 1999-2021, Tomasz Grysztar.
; All rights reserved.

;;************************************************************************************

use32

;; Agora vamos criar um cabeçalho para a imagem HAPP final do aplicativo. Anteriormente,
;; o cabeçalho era criado em cada imagem e poderia diferir de uma para outra. Caso algum
;; campo da especificação HAPP mudasse, os cabeçalhos de todos os aplicativos deveriam ser
;; alterados manualmente. Com uma estrutura padronizada, basta alterar um arquivo que deve
;; ser incluído e montar novamente o aplicativo, sem a necessidade de alterar manualmente
;; arquivo por arquivo. O arquivo contém uma estrutura instanciável com definição de 
;; parâmetros no momento da instância, tornando o cabeçalho tão personalizável quanto antes.

include "../../../../LibAPP/HAPP.s" ;; Aqui está uma estrutura para o cabeçalho HAPP

;; Instância | Estrutura | Arquitetura | Versão | Subversão | Entrada | Tipo  
cabecalhoAPP cabecalhoHAPP HAPP.Arquiteturas.i386, 8, 57, fasmX, 01h

;;************************************************************************************

include "../../../../LibAPP/andrmda.s"

;;************************************************************************************

fasmX:

;; Capturar os parâmetros fornecidos pelo Hexagonix®

	mov [regES], es
	
	push ds
	pop es			
	
	mov	[linha_comando], edi
	
	novaLinha
	
;;	Início do código do FASM

	mov	esi,_logo
	call	print_string
	
	call	get_params
	jc	information

	call	init_memory

	mov	esi,_memory_prefix
	call	print_string
	mov	eax,[memory_end]
	sub	eax,[memory_start]
	add	eax,[additional_memory_end]
	sub	eax,[additional_memory]
	shr	eax,10
	call	display_number
	mov	esi,_memory_suffix
	call	print_string

	mov	eax,78
	mov	ebx,buffer
	xor	ecx,ecx
	int	0x80
	mov	eax,dword [buffer]
	mov	ecx,1000
	mul	ecx
	mov	ebx,eax
	mov	eax,dword [buffer+4]
	div	ecx
	add	eax,ebx
	mov	[start_time],eax

	and	[preprocessing_done],0
	call	preprocessor
	or	[preprocessing_done],-1
	call	parser
	call	assembler
	call	formatter

	call	display_user_messages
	movzx	eax,[current_pass]
	inc	eax
	call	display_number
	mov	esi,_passes_suffix
	call	print_string
	mov	eax,78
	mov	ebx,buffer
	xor	ecx,ecx
	int	0x80
	mov	eax,dword [buffer]
	mov	ecx,1000
	mul	ecx
	mov	ebx,eax
	mov	eax,dword [buffer+4]
	div	ecx
	add	eax,ebx
	sub	eax,[start_time]
	jnc	time_ok
	add	eax,3600000
      time_ok:
	xor	edx,edx
	mov	ebx,100
	div	ebx
	or	eax,eax
	jz	display_bytes_count
	xor	edx,edx
	mov	ebx,10
	div	ebx
	push	edx
	call	display_number
	mov	dl,'.'
	call	display_character
	pop	eax
	call	display_number
	mov	esi,_seconds_suffix
	call	print_string
      display_bytes_count:
	mov	eax,[written_size]
	call	display_number
	mov	esi,_bytes_suffix
	call	print_string
	xor	al,al
	jmp	exit_program

information:
	mov	esi,_usage
	call	print_string
	mov	al,1
	jmp	exit_program

fileSize:	dd 0
filePosition:	dd 0
nomeArquivo: 	times 50 db 0
fileNameW: 	times 50 db 0	;File name to write

include 'Hexagonix.inc' ;; Interface para o Hexagonix

include '../VERSION.INC'

_copyright db 'Copyright (c) 1999-2021, Tomasz Grysztar',0xA,0

_logo db 10, 'flat assembler para Hexagonix(R) versao ',VERSION_STRING,0
_usage db 0xA
       db 'Uso: fasmX <fonte> [saida]',0xA
       db 0
_memory_prefix db '  (',0
_memory_suffix db ' Kbytes de memoria)',0xA,0
_passes_suffix db ' passadas, ',0
_seconds_suffix db ' segundos, ',0
_bytes_suffix db ' bytes.',0xA,0

include '../ERRORS.INC'
include '../SYMBDUMP.INC'
include '../PREPROCE.INC'
include '../PARSER.INC'
include '../EXPRPARS.INC'
include '../ASSEMBLE.INC'
include '../EXPRCALC.INC'
include '../FORMATS.INC'
include '../X86_64.INC'
include '../AVX.INC'

include '../TABLES.INC'
include '../MESSAGES.INC'

align 4

include '../VARIABLE.INC'

linha_comando dd ?
memory_setting dd ?
definitions_pointer dd ?
environment dd ?
timestamp dq ?
start_time dd ?
con_handle dd ?
displayed_count dd ?
last_displayed db ?
character db ?
preprocessing_done db ?

predefinitions rb 1000h
buffer rb 1000h


regES:	dw 0

endBuffer:
