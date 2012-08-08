#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

#define al (*((unsigned char*)&eax))
#define bl (*((unsigned char*)&ebx))
#define cl (*((unsigned char*)&ecx))
#define dl (*((unsigned char*)&edx))
#define si (*((unsigned char*)&esi))
#define di (*((unsigned char*)&edi))

#define ah (*((unsigned char*)(&eax + 1)))
#define bh (*((unsigned char*)(&ebx + 1)))
#define ch (*((unsigned char*)(&ecx + 1)))
#define dh (*((unsigned char*)(&edx + 1)))

#define ax (*((unsigned short*)&eax))
#define bx (*((unsigned short*)&ebx))
#define cx (*((unsigned short*)&ecx))
#define dx (*((unsigned short*)&edx))

#endif