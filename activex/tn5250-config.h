/* Modified version from src/tn5250-config.h which was generated 
 * automatically by configure.
 * Look at the end for usefull stuff */

/* Define to your system's socket descriptor type. */
/*#define SOCKET_TYPE int*/
#define SOCKET_TYPE SOCKET

/* Define this to use the -lncurses or -lcurses support. */
/*#define USE_CURSES 1*/

/* Define this to use the -lslang support. */
/* #undef USE_SLANG */

/* Define this to `int' if your curses doesn't typedef attr_t */
/* #undef attr_t */

/* Define this to use the new key handler. */
#define USE_OWN_KEY_PARSING 1

/* Define this to compile the system under WINE. */
/* #undef WINE */

/* Define this to the system config directory. */
/* #undef SYSCONFDIR */

/* Define this if your system already has a def for UCHAR. */
/* #undef HAVE_UCHAR */

/* define this when creating a binary distribution */
/* #undef BINARY_RELEASE */

/* Define if you have the <curses.h> header file. */
/* #undef HAVE_CURSES_H */

/* Define if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define if you have the `curses' library (-lcurses). */
/* #undef HAVE_LIBCURSES */

/* Define if you have the `dl' library (-ldl). */
/* #undef HAVE_LIBDL */

/* Define if you have the `m' library (-lm). */
/* #undef HAVE_LIBM */

/* Define if you have the `ncurses' library (-lncurses). */
#define HAVE_LIBNCURSES 1

/* Define if you have the `nsl' library (-lnsl). */
#define HAVE_LIBNSL 1

/* Define if you have the `slang' library (-lslang). */
/* #undef HAVE_LIBSLANG */

/* Define if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define if you have the `util' library (-lutil). */
/* #undef HAVE_LIBUTIL */

/* Define if you have the `X11' library (-lX11). */
/* #undef HAVE_LIBX11 */

/* Define if you have the `Xext' library (-lXext). */
/* #undef HAVE_LIBXEXT */

/* Define if you have the `Xpm' library (-lXpm). */
/* #undef HAVE_LIBXPM */

/* Define if you have the `Xxf86dga' library (-lXxf86dga). */
/* #undef HAVE_LIBXXF86DGA */

/* Define if you have the `Xxf86misc' library (-lXxf86misc). */
/* #undef HAVE_LIBXXF86MISC */

/* Define if you have the `Xxf86vm' library (-lXxf86vm). */
/* #undef HAVE_LIBXXF86VM */

/* Define if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if you have the <ncurses.h> header file. */
#define HAVE_NCURSES_H 1

/* Define if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <pwd.h> header file. */
/*#define HAVE_PWD_H 1*/

/* Define if you have the `resizeterm' function. */
#define HAVE_RESIZETERM 1

/* Define if you have the `select' function. */
#define HAVE_SELECT 1

/* Define if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define if you have the <slang.h> header file. */
/* #undef HAVE_SLANG_H */

/* Define if you have the <slang/slang.h> header file. */
#define HAVE_SLANG_SLANG_H 1

/* Define if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have the <syslog.h> header file. */
/*#define HAVE_SYSLOG_H 1*/

/* Define if you have the <sys/filio.h> header file. */
/* #undef HAVE_SYS_FILIO_H */

/* Define if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/time.h> header file. */
/*#define HAVE_SYS_TIME_H 1*/

/* Define if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <sys/wait.h> header file. */
/*#define HAVE_SYS_WAIT_H 1*/

/* Define if you have the <termcap.h> header file. */
#define HAVE_TERMCAP_H 1

/* Define if you have the <unistd.h> header file. */
/*#define HAVE_UNISTD_H 1*/

/* Name of package */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* Define if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */


// Usefull stuff now.

/* Define if you have the `ssl' library (-lssl). */
#define HAVE_LIBSSL 1

/* Version number of package */
#define TN5250_LIB_VERSION "0.17.x"
#define VERSION TN5250_LIB_VERSION
#define PACKAGE "EON"
#define SYSCONFDIR 
#define __WIN32__ 1


#ifdef _MSC_VER
#define strcasecmp strcmpi
// Pragmas for Warning level 4 for MSVC
#pragma warning(disable:4100) // '' : unreferenced formal parameter
#pragma warning(disable:4101) // '' : unreferenced local variable
#pragma warning(disable:4189) // '' : local variable is initialized but not referenced
#pragma warning(disable:4244) // 'function' : conversion from 'int' to 'unsigned char', possible loss of data
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4206) // nonstandard extension used : translation unit is empty
#pragma warning(disable:4701) // local variable '' may be used without having been initialized
#pragma warning(disable:4245) // 'initializing' : conversion from 'int' to 'COLORREF', signed/unsigned mismatch
__declspec(noreturn) void clean_up_and_exit(int error);

#ifdef HAVE_LIBSSL
// Replace with you own path or modify your make file.
#pragma comment(linker, "/defaultlib:../../openssl-0.9.7b/out32dll/libeay32.lib")
#pragma comment(linker, "/defaultlib:../../openssl-0.9.7b/out32dll/ssleay32.lib")
#endif

#endif // _MSC_VER

#include "Utils.h"

