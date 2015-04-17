// size_alignment.h
// July 31st, 2002
// for PowerPC

// file of definitions
// change this depending on target machine

#define CHRSIZE 1
#define INTSIZE 4
#define FLTSIZE 4
#define DBLSIZE 8
#define PTRSIZE 4

#define CMALIGN 1
#define IMALIGN 4
#define FMALIGN 4
#define DMALIGN 8
#define PMALIGN 4

// for natural embedded alignment in PPC
#ifdef __NATURAL
#define CEALIGN 1
#define IEALIGN 4
#define FEALIGN 4
#define DEALIGN 8
#define PEALIGN 4
#define CMPSITE 16	/* the embedding alignment of a composite is determined */
			/* by the largest alignment of its members. */
#endif // __NATURAL

// for packed embedded alignment in PPC
#ifdef __PACKED
#define IEALIGN 1
#define FEALIGN 1
#define CEALIGN 1
#define DEALIGN 1
#define PEALIGN 1
#define CMPSITE 1	/* the embedding alignment of a composite is determined */
			/* by the largest alignment of its members. */
#endif //__PACKED
