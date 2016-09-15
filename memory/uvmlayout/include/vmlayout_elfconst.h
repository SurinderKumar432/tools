#ifndef VMLAYOUT_ELF_CONST_H__
#define VMLAYOUT_ELF_CONST_H__

extern unsigned long etext, edata, end;
extern unsigned long _start, __data_start, __bss_start;

#define DATA_BSS_END            &end
#define DATA_BSS_START          &__bss_start

#define DATA_END        &edata
#define DATA_START      &__data_start

#define TEXT_END        &etext
#define TEXT_START      &_start

#endif /* ! VMLAYOUT_ELF_CONST_H__ */
