/* TN5250
 * Copyright (C) 2001 Scott Klement
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 * 
 * As a special exception, the copyright holder gives permission
 * for additional uses of the text contained in its release of TN5250.
 * 
 * The exception is that, if you link the TN5250 library with other files
 * to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the TN5250 library code into it.
 * 
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 * 
 * If you write modifications of your own for TN5250, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAXBUFSIZE 2048
#define SMALLBUFSIZE 32

#define POSIX_CONFIG_IN      "..\\configure.in"
#define LIB5250_MAKEFILE_AM "..\\src\\Makefile.am"
#define TN5250_CONFIG_H_IN  "..\\src\\tn5250-config.h.in"
#define AC_INIT_AUTOMAKE    "AM_INIT_AUTOMAKE("

struct _replace_undef {
   char *from;
   char *to;
};
typedef struct _replace_undef replace_undef;

replace_undef redefs[] = 
{
  {"SOCKET_TYPE",      "SOCKET_TYPE SOCKET"},
  {"HAVE_LIBM",        "HAVE_LIBM 1"},
  {"HAVE_LOCALE_H",    "HAVE_LOCALE_H 1"},
  {"HAVE_MEMORY_H",    "HAVE_MEMORY_H 1"},
  {"HAVE_SOCKET",      "HAVE_SOCKET 1"},
  {"HAVE_SELECT",      "HAVE_SELECT 1"},
  {"HAVE_UCHAR",       "HAVE_UCHAR 1"},
  {"HAVE_STDLIB_H",    "HAVE_STDLIB_H 1"},
  {"HAVE_STRERROR",    "HAVE_STRERROR 1"},
  {"HAVE_STRING_H",    "HAVE_STRING_H 1"},
  {"HAVE_SYS_STAT_H",  "HAVE_SYS_STAT_H 1"},
  {"HAVE_SYS_TIME_H",  "HAVE_SYS_TIME_H 1"},
  {"HAVE_SYS_TYPES_H", "HAVE_SYS_TYPES_H 1"},
  {"HAVE_UNISTD_H",    "HAVE_UNISTD_H 1"},
  {"HAVE_FCNTL_H",     "HAVE_FCNTL_H 1"},
  {"RETSIGTYPE",       "RETSIGTYPE void"},
  {"STDC_HEADERS",     "STDC_HEADERS 1"},
  {"X_DISPLAY_MISSING","X_DISPLAY_MISSING 1"},
  {NULL, NULL},
};

#define DEF_HAVE_LIBCRYPTO 1
#define DEF_HAVE_LIBSSL    2
#define DEF_SIZEOF_INT     3
#define DEF_SIZEOF_LONG    4
#define DEF_SIZEOF_SHORT   5
#define DEF_PACKAGE        6
#define DEF_VERSION        7

struct _manual_def {
     char *from;
     int  def;
};
typedef struct _manual_def manual_def;

manual_def mredefs[] = 
{
  {"HAVE_LIBCRYPTO", DEF_HAVE_LIBCRYPTO},
  {"HAVE_LIBSSL",    DEF_HAVE_LIBSSL},
  {"SIZEOF_INT",     DEF_SIZEOF_INT},
  {"SIZEOF_LONG",    DEF_SIZEOF_LONG},
  {"SIZEOF_SHORT",   DEF_SIZEOF_SHORT},
  {"PACKAGE",        DEF_PACKAGE},
  {"VERSION",        DEF_VERSION},
  {NULL, -1},
};

char my_redef[128];
  
int get_package_version(char *version, char *package, int maxlen);
int get_makefile_var(const char *fn, const char *var, char *value, int maxlen);
void replaceall(char *str, const char *from, const char *to);
void strtrim(char *str);
void get_path(const char *prompt, const char *file, char *path, int maxlen);
int create_tn5250_config_h(const char *infile, const char *outfile, 
                   char with_openssl, const char *package, const char *version);
int create_config_h(const char *infile, const char *outfile, 
                        const char *version);
int create_makefile(const char *infile, const char *outfile,
                     const char *openssl_lib, const char *openssl_inc,
                     const char *glib_lib, const char *glib_include,
                     const char *lib5250_objs, const char *installdir,
                     const char *debug, const char *cursor);
void replacedata(const char *from, const char *to, char *line, int maxlen);
int make_root_makefile(const char *fn);
char *manual_redef(int redefnum, char with_openssl, const char *package, 
                  const char *version);
int get_glib_info(const char *args, char *value, int maxlen);



int main(unsigned argc, char **argv) {

   char lib5250_sources[MAXBUFSIZE+1];
   char lib5250_objs[MAXBUFSIZE+1];
   char openssl_lib[MAXBUFSIZE+1];
   char openssl_include[MAXBUFSIZE+1];
   char glib_libs[MAXBUFSIZE+1];
   char glib_include[MAXBUFSIZE+1];
   char temp[MAXBUFSIZE+1];
   char installdir[MAXBUFSIZE+1];
   char package[SMALLBUFSIZE+1], version[SMALLBUFSIZE+1];
   char cursor[SMALLBUFSIZE+1], debug[SMALLBUFSIZE+1];
   char with_openssl;
   int ch, rc;



/* Get the PACKAGE and VERSION info from configure.in */

   if (get_package_version(version, package, SMALLBUFSIZE)<0) {
       fprintf(stderr, "Unable to find package info in %s\n", POSIX_CONFIG_IN);
       exit(1);
   }

   fprintf(stdout, "package = %s, version = %s\n\n", package, version);


/* Find out which source files are needed to compile LIB5250 */

   if (get_makefile_var(LIB5250_MAKEFILE_AM, "lib5250_la_SOURCES", 
          lib5250_sources, MAXBUFSIZE) < 1) {
        fprintf(stderr, "Unable to find lib5250_la_SOURCES in %s\n", 
                LIB5250_MAKEFILE_AM );
        exit(1);
   }

   strcpy(lib5250_objs, lib5250_sources);
   replaceall(lib5250_objs, ".c", ".o");

 /* find out about GLIB support */

   rc = get_glib_info("--libs glib-2.0", glib_libs, MAXBUFSIZE);
   if (rc < 0) {
       exit(1);
   }
   rc = get_glib_info("--cflags glib-2.0", glib_include, MAXBUFSIZE);
   if (rc < 0) {
       exit(1);
   }


 /* find out if OpenSSL is around, and if we should use it: */

   do {
        printf("Do you wish to compile in OpenSSL support? (Y/N) ");
        fgets(temp, MAXBUFSIZE, stdin);
        with_openssl = *temp;
        with_openssl = tolower(with_openssl);
   } while (with_openssl!='y' && with_openssl!='n');
   
   if (with_openssl=='y') {
        get_path("Enter the path where the openssl libraries are:", 
                 "\\libssl.a", temp, MAXBUFSIZE);
        _snprintf(openssl_lib, MAXBUFSIZE, "-L\"%s\" -lssl -lcrypto", temp);
        get_path("Enter the path where the openssl header files are:", 
                 "\\openssl\\ssl.h", temp, MAXBUFSIZE);
        _snprintf(openssl_include, MAXBUFSIZE, "-I\"%s\"", temp);
   }
   else {
        openssl_lib[0] = '\0';
        openssl_include[0] = '\0';
   }

   fprintf(stdout, "\nNOTE: You must pre-create the following directory!\n");
   get_path("Where would you like to put the compiled programs?",
            "\\.", installdir, MAXBUFSIZE);

  /* Make a tn5250-configure.h file */

   if (create_tn5250_config_h(TN5250_CONFIG_H_IN, "tn5250-config.h", 
           with_openssl, package, version) < 0) {
        exit(1);
   }

   if (create_config_h("tn5250-config.h", "config.h", version) < 0) {
        exit(1);
   }

   /* Create a makefile */

   debug[0] = '\0';
   strcpy(cursor, "-DNOBLINK");

   if (create_makefile("Makefile.win.in", "Makefile", openssl_lib,
         openssl_include, glib_libs, glib_include, lib5250_objs, 
         installdir, debug, cursor)<0) {
         exit(1);
   }

   make_root_makefile("..\\Makefile");

   return 0;
}


/****f* get_package_version
 * NAME
 *    get_package_version
 * SYNOPSIS
 *    ret = get_package_version (ver, pkg, sizeof(ver));
 * INPUTS
 *    char      *        version -
 *    char      *        package -
 *    int                maxlen  -
 * DESCRIPTION
 *    Scan the ..\configure.in file for the TN5250 package name
 *    and version level so we can output them later to our config.h
 *****/
int get_package_version(char *version, char *package, int maxlen) {

   FILE *uconf;
   char buf[MAXBUFSIZE+1];
   char *p, *p2;

   uconf = fopen(POSIX_CONFIG_IN, "r");
   if (uconf == NULL) {
        fprintf(stderr, "Error opening ");
        perror(POSIX_CONFIG_IN);
        exit(1);
   }

   package[0] = '\0';
   version[0] = '\0';

   while (fgets(buf, MAXBUFSIZE, uconf)!=NULL) {
        if ((p=strstr(buf, AC_INIT_AUTOMAKE))!=NULL) {
            p += strlen(AC_INIT_AUTOMAKE);
            p2 = strchr(p, ',');
            if (p2!=NULL) {
                 int len = (p2 - p);
                 if (len>maxlen) len = maxlen;
                 strncpy(package, p, len);
                 package[len] = '\0';
 
                 p+=len+1; 
                 while ((*p)==' ') p++;
                 p2 = strchr(p, ')');
                 if (p2!=NULL) {
                    len = (p2 - p);
                    if (len>maxlen) len = maxlen;
                    strncpy(version, p, len);
                    version[len] = '\0';
                 }
            }
            if (*version && *package) break;
        }
   }
   fclose(uconf);

}


/****f* get_makefile_var
 * NAME
 *    get_makefile_var
 * SYNOPSIS
 *    ret = get_makefile_var ("Makefile.am", "tn5250_SOURCES", value, 1024);
 * INPUTS
 *    const char  *      fn     -
 *    const char  *      var    -
 *    char        *      value  -
 *    int                maxlen -
 * DESCRIPTION
 *    Searches a Makefile.am for a variable, and loads the value into
 *    the "value" variable.
 *    We use this to get the names of the source files needed to build
 *    lib5250.dll.
 *****/
int get_makefile_var(const char *fn, const char *var, char *value, int maxlen) {

     FILE *f;
     int pos, len, ch, state;
     int lastspace;
     int x;
     int savestate;

     f = fopen(fn, "r");
     if (f==NULL) {
         fprintf(stderr, "error opening %s: %s\n", fn, strerror(errno));
         return -1;
     }

     lastspace=0;
     state = 0;
     pos = 0;
     len = -1;
     while ((ch=fgetc(f))!=EOF) {
         if (ch=='#' && state!=3 && state>0) { savestate=state; state=-1;}
         switch (state) {
           case -1:
             if (ch=='\n') state=savestate;
             break;
           case 0:
             if (ch==var[pos]) pos++; else pos = 0;
             if (var[pos]=='\0') {
                 state = 1;
             }
             break;
           case 1:
             if (ch=='=') state = 2;
             break;
           case 2:
             switch (ch) {
                case '\\':
                   state = 3;
                   break;
                case '\n':
                   state = 4;
                   break;
                case '\r':
                   break;
                case '\t':
                case ' ':
                   if (!lastspace) {
                       len++;
                       if (len<maxlen) value[len] = ' ';
                       lastspace = 1;
                   }
                   break;
                default:
                   len++;
                   if (len<maxlen) 
                      value[len] = ch;
                   lastspace=0;
                   break;
             } 
             break;
           case 3:
             switch (ch) {
               case '\r':
                  break;
               case '\t':
               case ' ':
               case '\n':
                  ch=' ';
                  len++;
                  if (len<maxlen)
                      value[len] = ch;
                  lastspace = 1;
                  state = 2;
                  break;
               default:
                  len++;
                  if (len<maxlen)
                      value[len] = ch;
                  state = 2;
                  lastspace = 0;
                  break;
             }
             break;
         }
         if (state==4) break;
     }

     fclose(f);

     len++;
     value[len] = '\0';

     if (len>0) {
          strtrim(value);
          len = strlen(value);
     }

     return len;
}


/****f* replaceall
 * NAME
 *    replaceall
 * SYNOPSIS
 *    replaceall (value, ".c", ".o");
 * INPUTS
 *    char        *      str    -
 *    const char  *      from   -
 *    const char  *      to     -
 * DESCRIPTION
 *    Replaces all occurrances of "from" in string "str" with
 *    the contents of "to".   Note that "from" and "to" must
 *    be the same length.
 *****/
void replaceall(char *str, const char *from, const char *to) {

      char *p;
      if (strlen(from)!=strlen(to)) {
         return;
      }
      while ((p=strstr(str, from))!=NULL) {
          memcpy(p, to, strlen(to));
      }
      return;
}


/****f* strtrim
 * NAME
 *    strtrim
 * SYNOPSIS
 *    strtrim (string);
 * INPUTS
 *    char        *      str    -
 * DESCRIPTION
 *    Trims off all leading blanks, all trailing blanks
 *    and trailing end-of-line characters from a string.
 *****/
void strtrim(char *str) {

     int len;
     int x;

     len = strlen(str);

     while (str[0]==' ') {
        for (x=0; x<len; x++) 
          str[x] = str[x+1];
        len = strlen(str);
     }

     while ((len>0) &&
            (str[len]==' '||str[len]=='\0'||str[len]=='\n'||str[len]=='\r'))
         { str[len]='\0'; len--; }

     return;
}

/****f* get_path
 * NAME
 *    get_path
 * SYNOPSIS
 *    get_path ("Path to foo?", "\\foo", my_path, 640);
 * INPUTS
 *    const char  *      prompt -
 *    const char  *      file   -
 *    char        *      path   -
 *    int                maxlen -
 * DESCRIPTION
 *    Prompt the user for a directory name, and ensure that "file" is in
 *    in the returned dir.
 *****/
void get_path(const char *prompt, const char *file, char *path, int maxlen) {

    struct stat st;
    char *f;
    int found=0;
    

     do {

           printf("\n%s\n", prompt);

           fgets(path, maxlen, stdin);
           strtrim(path);

           f = malloc(strlen(path)+strlen(file)+1);

           strcpy(f, path);
           strcat(f, file);

           if (stat(f, &st)<0) {
               found = 0;
               fprintf(stderr, "%s: %s\n", f, strerror(errno));
           } else {
               found = 1;
           }

           free(f);

   } while (!found);

   return;
}


/****f* create_tn5250_config_h
 * NAME
 *    create_tn5250_config_h
 * SYNOPSIS
 *    x = create_tn5250_config_h ("config.h.in", "config.h", 'y', pkg, ver);
 * INPUTS
 *    const char  *      infile       -
 *    const char  *      outfile      -
 *    char               with_openssl -
 *    const char  *      package      -
 *    const char  *      version      -
 * DESCRIPTION
 *    Create a create a config.h file.  This is really a quick,
 *    minimal routine to allow us to get by without having autoconf
 *    under Windows, it defines the macros that enable the appropriate
 *    options for a Win32 compile.
 *****/
int create_tn5250_config_h(const char *infile, const char *outfile, 
               char with_openssl, const char *package, const char *version) {

      FILE *in;
      FILE *out;
      char rec[MAXBUFSIZE+1];
      char *s;
      int changed, i;
      struct stat st;

      if (stat(infile, &st)==0) {       /* tn5250-config.h.in found */

           in = fopen(infile, "r");
           if (in==NULL) {
                fprintf(stderr, "%s: %s\n", infile, strerror(errno));
                return -1;
           }

           out = fopen(outfile, "w");
           if (out==NULL) {
                fprintf(stderr, "%s: %s\n", outfile, strerror(errno));
                fclose(in);
                return -1;
           }

           while (fgets(rec, MAXBUFSIZE, in)!=NULL) {
             changed = 0;
             if (strstr(rec, "#undef")!=NULL) {
                 i = 0; 
                 while (redefs[i].from!=NULL) {
                    if (strstr(rec, redefs[i].from)!=NULL) {
                         fprintf(out, "#define %s\n", redefs[i].to);
                         changed=1;
                         break;
                    }
                    i++;
                 }
                 if (!changed) {
                    i = 0;
                    while (mredefs[i].from!=NULL) {
                       if (strstr(rec, mredefs[i].from)!=NULL) {
                           s = manual_redef(mredefs[i].def, with_openssl, 
                                 package, version);
                           if (s[0]!='\0') {
                               changed = 1;
                               fprintf(out, "%s\n", s);
                           }
                       }
                       i++;
                    }
                 }
             }
             if (!changed)  {
                 fprintf(out, "%s", rec);
             }
          }
     
          fclose(in);
          fclose(out);

     }
     else {       /* tn5250-config.h.in not found */

           out = fopen(outfile, "w");
           if (out==NULL) {
                fprintf(stderr, "%s: %s\n", outfile, strerror(errno));
                fclose(in);
                return -1;
           }

           i = 0; 
           while (redefs[i].from!=NULL) {
                fprintf(out, "#define %s\n", redefs[i].to);
		i++;
           }
      
           i = 0;
           while (mredefs[i].from!=NULL) {
                s = manual_redef(mredefs[i].def, with_openssl, 
                           package, version);
                if (s[0]!='\0') {
                      fprintf(out, "%s\n", s);
                }
		i++;
           }

           fclose(out);
     }
     return 0;
}     


/****f* create_config_h
 * NAME
 *    create_config_h
 * SYNOPSIS
 *    x = create_config_h ("tn5250-config.h", "config.h", ver);
 * INPUTS
 *    const char  *      infile       -
 *    const char  *      outfile      -
 *    const char  *      version      -
 * DESCRIPTION
 *    The UN*X Makefile will use a simple sed command to convert
 *    the string "VERSION" to "TN5250_LIB_VERSION".  We do the same
 *    thing here, manually, so that Windows users don't need sed.
 *****/
int create_config_h(const char *infile, const char *outfile, 
                        const char *version) {

     FILE *in, *out;
     char rec[MAXBUFSIZE+1];

     in = fopen(infile, "r");
     if (in==NULL) {
        fprintf(stderr, "%s: %s\n", infile, strerror(errno));
        return -1;
     }

     out = fopen(outfile, "w");
     if (out==NULL) {
        fprintf(stderr, "%s: %s\n", outfile, strerror(errno));
        fclose(in);
        return -1;
     }

     while (fgets(rec, MAXBUFSIZE, in)!=NULL) {
          if (strstr(rec, "VERSION")!=NULL) {
               fprintf(out, "#define TN5250_LIB_VERSION \"%s\"\n", version);
          } else {
               fprintf(out, "%s", rec);
          }
     }
     fclose(in);
     fclose(out);

     return 0;
}


/****f* create_makefile
 * NAME
 *    create_makefile
 * SYNOPSIS
 *    x = create_makefile ("Makefile.win.in", "Makefile", "C:\openssl\lib",
 *                         "C:\openssl\include", value, "C:\tn5250", 
 *                         "-DNDEBUG", "-DLINE_CURSOR");
 * INPUTS
 *    const char  *      infile       -
 *    const char  *      outfile      -
 *    const char  *      openssl_lib  -
 *    const char  *      openssl_inc  -
 *    const char  *      lib5250_objs -
 *    const char  *      installdir   -
 *    const char  *      debug        -
 *    const char  *      cursor       -
 * DESCRIPTION
 *    This replaces the variables in the Makefile.win.in file
 *    with values calculated in this program.  
 *    For example, "@lib5250_objs@" in the makefile is replaced
 *    with the contents of the lib5250_objs arg to this function.
 *
 *    Again, we're not trying to fully implement everything that
 *    autoconf can do, here -- just the minimum amount that we
 *    need in order to make Windows users able to get by without
 *    the autoconf/automake tools.
 *****/
int create_makefile(const char *infile, const char *outfile,
                     const char *openssl_lib, const char *openssl_inc,
                     const char *glib_libs, const char *glib_include,
                     const char *lib5250_objs, const char *installdir,
                     const char *debug, const char *cursor) {

     FILE *in,*out;
     char rec[MAXBUFSIZE+1];

     in = fopen(infile, "r");
     if (in==NULL) {
         fprintf(stderr, "%s: %s\n", infile, strerror(errno));
         return -1;
     }

     out = fopen(outfile, "w");
     if (out==NULL) {
         fprintf(stderr, "%s: %s\n", outfile, strerror(errno));
         fclose(in);
         return -1;
     }

     while (fgets(rec, MAXBUFSIZE, in)!=NULL) {
         replacedata("@installdir@",      installdir,      rec, MAXBUFSIZE);
         replacedata("@openssl_lib@",     openssl_lib,     rec, MAXBUFSIZE);
         replacedata("@openssl_include@", openssl_inc,     rec, MAXBUFSIZE);
         replacedata("@lib5250_objs@",    lib5250_objs,    rec, MAXBUFSIZE);
         replacedata("@debug@",           debug,           rec, MAXBUFSIZE);
         replacedata("@cursor@",          cursor,          rec, MAXBUFSIZE);
         replacedata("@GLIB_LIBS@",	  glib_libs,       rec, MAXBUFSIZE);
         replacedata("@GLIB_INCLUDE@",	  glib_include,    rec, MAXBUFSIZE);
         fputs(rec, out);
     }

     fclose(in);
     fclose(out);
}

/****f* replacedata
 * NAME
 *    replacedata
 * SYNOPSIS
 *    replacedata ("@cursor@", "-DLINE_CURSOR", line, sizeof(line));
 * INPUTS
 *    const char  *      from         -
 *    const char  *      to           -
 *    char        *      line         -
 *    int                maxlen       -
 * DESCRIPTION
 *    This will replace the first occurrance of the "from" string with 
 *    the "to" string in a line of text.  The from and to do not have to 
 *    be the same length.  This differs from replaceall() above in that
 *    it only replaces one occurrance, and the strings do not have to be the
 *    same length.  Unfortunately, it does not perform as well as replaceall().
 *****/
void replacedata(const char *from, const char *to, char *line, int maxlen) {

     char *p;
     int len;
     char *before, *after;

     if ((p=strstr(line, from))!=NULL) {
          if (p<=line) {
             before=malloc(1);
             *before = '\0';
          } else {
             len = p - line;
             before = malloc(len+1);
             memcpy(before, line, len);
             before[len] = '\0';
          }
          p += strlen(from);
          if (strlen(p)<1) {
             after = malloc(1);
             *after = '\0';
          } else {
             len = strlen(p);
             after = malloc(len+1);
             memcpy(after, p, len);
             after[len] = '\0';
          }
           _snprintf(line, maxlen-1, "%s%s%s", before, to, after);
          free(before);
          free(after);
     }

}


/****f* make_root_makefile
 * NAME
 *    make_root_makefile
 * SYNOPSIS
 *    make_root_makefile ("..\\Makefile");
 * INPUTS
 *    const char  *      fn           -
 * DESCRIPTION
 *    This generates a very simple Makefile that
 *    just switches to the Win32 directory to do
 *    a make or make install.
 *****/
int make_root_makefile(const char *fn) {

   FILE *out;

   out = fopen(fn, "w");
   if (out==NULL) {
       fprintf(stderr, "%s: %s\n", fn, strerror(errno));
       return -1;
   }

   fprintf(out, "all:\n");
   fprintf(out, "\tmake -f win32\\Makefile all\n");
   fprintf(out, "\n");
   fprintf(out, "install:\n");
   fprintf(out, "\tmake -f win32\\Makefile install\n");

   fclose(out);
}


/****f* manual_redef
 * NAME
 *    manual_redef
 * SYNOPSIS
 *    manual_redef (DEF_HAVE_LIBCRYPTO, 1, "tn5250","0.16.3");
 * INPUTS
 *    int                redefnum     -
 *    int                with_openssl -
 *    const char    *    package      -
 *    const char    *    version      -
 * DESCRIPTION
 *    Certain #define's that need to be put in config.h
 *    require some calculations to determine what the macro
 *    should be.   This calculates these "manual redefinitions"
 *    and returns the string to put in the config.h file.
 *****/
char *manual_redef(int redefnum, char with_openssl, 
                   const char *package, const char *version) {

     switch (redefnum) {
       case DEF_HAVE_LIBCRYPTO:
         if (with_openssl=='y') {
              strcpy(my_redef, "#define HAVE_LIBCRYPTO 1");
         }
         break;
       case DEF_HAVE_LIBSSL:
         if (with_openssl=='y') {
              strcpy(my_redef, "#define HAVE_LIBSSL 1");
         }
         break;
       case DEF_SIZEOF_INT:
         sprintf(my_redef, "#define SIZEOF_INT %d\n", sizeof(int));
         break;
       case DEF_SIZEOF_LONG:
         sprintf(my_redef, "#define SIZEOF_LONG %d\n", sizeof(long));
         break;
       case DEF_SIZEOF_SHORT:
         sprintf(my_redef,"#define SIZEOF_SHORT %d\n",sizeof(short));
         break;
       case DEF_PACKAGE:
         sprintf(my_redef,"#define PACKAGE \"%s\"\n", package);
         break;
       case DEF_VERSION:
         sprintf(my_redef,"#define VERSION \"%s\"\n", version);
         break;
     }

     return my_redef;
}


/****f* get_glib_info
 * NAME
 *    get_glib_info
 * SYNOPSIS
 *    ret = get_glib_info ("--libs glib-2.0", value, 1024);
 * INPUTS
 *    const char  *      args   -
 *    char        *      value  -
 *    int                maxlen -
 * DESCRIPTION
 *     Executes the pkg-config command with the given args,
 *     and returns the result as a string value.
 *****/
int get_glib_info(const char *args, char *value, int maxlen) {

     char *pkgconf = "pkg-config";
     char *workfile = "glibinfo.txt";
     char *cmd;
     struct stat st;
     int rc;
     FILE *f;

     cmd = malloc(strlen(args) + strlen(pkgconf) + strlen(workfile) + 6);
     if (cmd == NULL) {
           fprintf(stderr, "mkconfig: get_glib_info: Out of memory!\n");
           return -1;
     }

     sprintf(cmd, "%s %s > %s", pkgconf, args, workfile);
     if (system(cmd) < 0) {
         fprintf(stderr, "mkconfig: get_glib_info: error running pkg-config.\n"
                         "          Maybe glib isn't installed? or isn't in "
                         "your path?\n");
        free(cmd);
        return -1;
     }
     free(cmd);

     if (stat(workfile, &st) < 0) {
         fprintf(stderr, "mkconfig: get_glib_info: stat: %s\n", 
               strerror(errno));
         return -1;
     }

     f = fopen(workfile, "r");
     if (f == NULL) {
         fprintf(stderr, "mkconfig: get_glib_info: fopen: %s\n", 
               strerror(errno));
     }
   
     if (st.st_size > maxlen) 
           st.st_size = maxlen;
   
     fread(value, st.st_size, 1, f);

     fclose(f);

     strtrim(value);

     return 0;
}

      
