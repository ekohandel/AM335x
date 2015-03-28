MLO: MLO.bin
	../../tools/mkAM335xImg/mkAM335xImg -s 0x402F0400 -o MLO MLO.bin

MLO.bin: MLO.elf
	arm-none-eabi-objcopy -O binary MLO.elf MLO.bin

MLO.elf: MLO.s
	arm-none-eabi-gcc -o MLO.elf MLO.s -nostdlib -Ttext=0x402F0400

clean:
	rm MLO.elf MLO.bin
