SECTIONS
{
  . = 0x402F0400;
  .text : {*(reset)}
  .text : {*(text)}
}
