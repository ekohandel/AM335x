echo.img: echo.bin
	../../tools/mkAM335xImg/mkAM335xImg -s 0x402F0400 -r -o echo.img echo.bin

echo.bin: echo.elf
	arm-none-eabi-objcopy -O binary echo.elf echo.bin

echo.elf: *.s *.ld
	arm-none-eabi-gcc -o echo.elf -nostdlib -T./echo.ld echo.s

clean:
	rm -f echo.elf echo.bin
