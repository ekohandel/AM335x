/* A dumby file that doesn't really do anything
 * just to test the boot process
 */
.text
.global _start
_start:
	MOV R0, #0x00
	MOV R1, #0x01
	MOV R2, #0x02
	MOV R3, #0x03
	MOV R4, #0x04
	B   _start
