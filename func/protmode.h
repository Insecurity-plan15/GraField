#ifndef _FUNC_PROTMODE
#define _FUNC_PROTMODE

typedef PROTECTMODE_DESCRIPTOR struct {
    unsigned short int limitlow;
    unsigned short int baselow;
    unsigned char      basemid;
    unsigned char      type;
    unsigned char      limithi;
    unsigned char      basehi;
};

typedef PROTECTMODE_GATE struct {
    unsigned short int offsetlo;
    unsigned short int selector;
    unsigned short int attr;
    unsigned short int offsethi;
};

/* Macro to declare a descriptor */
/* Usage: PROTECTMODE_DESCRIPTOR gdt[] = (Descriptor(...), Descriptor(...), ...); */
#define Descriptor(Base, Limit, Attr) ( \
    /* limitlow */ (unsigned short int) (Limit), \
    /* baselow  */ (unsigned short int) (Base), \
    /* basemid  */ (unsigned char) ((unsigned long int) (Base)>>16), \
    /* type     */ (unsigned char) (Attr), \
    /* limithi  */ (unsigned char) ((unsigned long int) (Limit)>>16) & 0xf | ((unsigned char) (Attr)>>8) & 0xf0, \
    /* basehi   */ (unsigned char) ((unsigned long int) (Base)>>24) \
)

/* Macro to declare a gate */
#define Gate(Selector, Offset, DCount, Attr) ( \
    /* offsetlo */ (unsigned short int) (Offset), \
    /* selector */ (unsigned short int) (SElector), \
    /* attr     */ (unsigned short int) (((unsigned char) (DCount)) & 0x1f | ((unsigned char) (Attr))), \
    /* offsethi */ (unsigned short int) ((unsigned long int) (Offset)>>16) \
)

/* Macros to parse information from a descriptor */
/* Usage: gdt1base = DescriptorBase(gdt[1]); etc */
#define DescriptorBase(Desc) ((unsigned long int) (Desc).baselow | ((unsigned long int) (Desc).basemid<<16) | ((unsigned long int) (Desc).basehi<<24))
#define DescriptorLimit(Desc) ((unsigned long int) (Desc).limitlow | (((unsigned long int) (Desc).limithi & 0xf)<<16))
#define DescriptorAttr(Desc) ((unsigned short int) (Desc).type | (((unsigned short int) (Desc).limithi & 0xf0)<<16))

/* Macros to parse information from a gate */
#define GateSelector(Desc) ((Desc).Selector)
#define GateOffset(Desc) ((unsigned long int) (Desc).offsethi<<16 | (unsigned long int) (Desc).offsetlo)
#define GateDCount(Desc) ((unsigned char) ((Desc).attr & 0x1f))
#define GateAttr(Desc) ((unsigned char) ((Desc).attr>>8))

/* Descriptor attribute */
#define DA_32   0x4000
#define DA_DPL0 0x0
#define DA_DPL1 0x20
#define DA_DPL2 0x40
#define DA_DPL3 0x60
/* Storage segment */
#define DA_DR   0x90
#define DA_DRW  0x92
#define DA_DRWA 0x93
#define DA_C    0x98
#define DA_CR   0x9a
#define DA_CCO  0x9c
#define DA_CCOR 0x9e
/* System segment */
#define DA_LDT      0x82
#define DA_TaskGate 0x85
#define DA_386TSS   0x89
#define DA_386CGate 0x8c
#define DA_386IGate 0x8e
#define DA_386TGate 0x8f

/* Selector attribute */
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3
#define SA_TIG 0
#define SA_TIL 4

#endif
