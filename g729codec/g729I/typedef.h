/* ITU-T G.729 Software Package Release 2 (November 2006) */
/* Version 3.3    Last modified: December 26, 1995 */

/* WARNING: Make sure that the proper flags are defined for your system */

/*
   Types definitions
*/

#if defined(__BORLANDC__) || defined (__WATCOMC__) || defined(_MSC_VER) || defined(__ZTC__) || defined(__HIGHC__) || defined (__CYGWIN32__)
typedef  long  int   Word32   ;
typedef  short int   Word16   ;
typedef  short int   Flag  ;
#elif defined( __sun)
typedef short  Word16;
typedef long  Word32;
typedef int   Flag;
#elif defined(__unix__) || defined(__unix)
typedef short Word16;
typedef int   Word32;
typedef int   Flag;
#elif defined(VMS) || defined(__VMS)
typedef short  Word16;
typedef long  Word32;
typedef int   Flag;
#else
typedef short  Word16;
typedef int  Word32;
typedef int   Flag;
#endif

