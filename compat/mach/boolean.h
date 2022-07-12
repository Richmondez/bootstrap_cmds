#ifndef	_MACH_BOOLEAN_H_
#define	_MACH_BOOLEAN_H_

#if defined(__x86_64__)
typedef unsigned int	boolean_t;
#else
typedef int		boolean_t;
#endif

#ifndef	TRUE
#define TRUE	1
#endif	/* TRUE */

#ifndef	FALSE
#define FALSE	0
#endif	/* FALSE */

#endif