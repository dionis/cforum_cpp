/*!
 * \file utils.c
 * \brief Utilities for the Classic Forum, making the life easier
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Utilities for the Classic Forum, such as getline(), make_path()
 * remove_path(), etc, pp making the life easier
 */

#include "defines.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <dirent.h>

#include <glib.h>

#include "memoryutils.h"

int cf_strcasecmp(const char *str1,const char *str2) {
  register const char *ptr1 = str1,*ptr2 = str2;

  while(*ptr1 && *ptr2) {
    if(g_unichar_toupper(g_utf8_get_char(ptr1)) != g_unichar_toupper(g_utf8_get_char(ptr2))) return -1;

    ptr1 = g_utf8_next_char(ptr1);
    ptr2 = g_utf8_next_char(ptr2);
  }

  return 0;
}

int cf_strncasecmp(const char *str1,const char *str2,size_t n) {
  register const char *ptr1 = str1,*ptr2 = str2;
  register size_t i;

  for(i=0;*ptr1 && *ptr2 && i < n && g_unichar_toupper(g_utf8_get_char(ptr1)) == g_unichar_toupper(g_utf8_get_char(ptr2));ptr1 = g_utf8_next_char(ptr1),ptr2 = g_utf8_next_char(ptr2),i++) {
    if(i == n - 1) return 0;
  }

  return -1;
}

#define ONEMASK ((size_t)(-1) / 0xFF)
size_t cf_strlen_utf8(const char * _s) {
  const char * s;
  size_t count = 0;
  size_t u;
  unsigned char b;

  /* Handle any initial misaligned bytes. */
  for(s = _s; (uintptr_t)(s) & (sizeof(size_t) - 1); s++) {
    b = *s;

    /* Exit if we hit a zero byte. */
    if(b == '\0') goto done;

    /* Is this byte NOT the first byte of a character? */
    count += (b >> 7) & ((~b) >> 6);
  }

  /* Handle complete blocks. */
  for(; ;s+=sizeof(size_t)) {
    /* Prefetch 256 bytes ahead. */
    __builtin_prefetch(&s[256], 0, 0);

    /* Grab 4 or 8 bytes of UTF-8 data. */
    u = *(size_t *)(s);

    /* Exit the loop if there are any zero bytes. */
    if((u - ONEMASK) & (~u) & (ONEMASK * 0x80)) break;
  
    /* Count bytes which are NOT the first byte of a character. */
    u = ((u & (ONEMASK * 0x80)) >> 7) & ((~u) >> 6);
    count += (u * ONEMASK) >> ((sizeof(size_t) - 1) * 8);
  }
  
  /* Take care of any left-over bytes. */
  for(; ;++s) {
    b = *s;

    /* Exit if we hit a zero byte. */
    if(b == '\0') break;
  
    /* Is this byte NOT the first byte of a character? */
    count += (b >> 7) & ((~b) >> 6);
  }

  done:
  return ((s - _s) - count);
}


#if __SSE2__
#include <emmintrin.h>

#ifndef WORDS_BIGENDIAN
static inline int count_bits_to_0(unsigned int x) { /* counting trailing zeroes, by Nazo, post: 2009/07/20 03:40, this is current winner for speed */
  static const unsigned char table[256] = {
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
  };
  if((unsigned char)x) return table[(unsigned char)x];
  return table[x >> 8] + 8; /* t[x / 256] + 8 */
}
#else
static inline int count_bits_to_0(unsigned int x) {  /* counting trailing zeroes
  http://www.hackersdelight.org/: nlz1() shortened for 16-bit mask */
  register int n = 0;
  if(x <= 0x000000FFU) {n = n + 8; x = x << 8;}
  if(x <= 0x00000FFFU) {n = n + 4; x = x << 4;}
  if(x <= 0x00003FFFU) {n = n + 2; x = x << 2;}
  if(x <= 0x00007FFFU) {n = n + 1;}
  return n;
}
#endif

size_t cf_strlen(const char *str) {
  register size_t len = 0;
  __m128i xmm0 = _mm_setzero_si128();
  __m128i xmm1;
  int mask = 0;

  /* align to 16 bytes */
  while((((intptr_t)str) & (sizeof(__m128i)-1)) != 0) {
    if(*str++ == 0) return len;
    ++len;
  }

  /* search for 0 */
  for(;;) {
    xmm1 = _mm_load_si128((__m128i *)str);
    xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);
    if((mask = _mm_movemask_epi8(xmm1)) != 0) {
      /* got 0 somewhere within 16 bytes in xmm1, or within 16 bits in mask
      find index of first set bit */

      #ifndef _DISABLE_ASM_BSF /* define it to disable ASM */
      #if(_MSC_VER >= 1300)   /* make sure <intrin.h> is included */
      unsigned long pos;
      _BitScanForward(&pos, mask);
      len += (size_t)pos;
      #elif defined(_MSC_VER)  /* earlier MSVC's do not have _BitScanForward, use inline asm */
      __asm bsf edx, mask ; edx = bsf(mask)
      __asm add edx, len  ; edx += len
      __asm mov len, edx  ; len = edx
      #elif((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4))) /* modern GCC has built-in __builtin_ctz */
      len += __builtin_ctz(mask);
      #elif defined(__GNUC__) /* older GCC shall use inline asm */
      unsigned int pos;
      asm("bsf %1, %0" : "=r" (pos) : "rm" (mask));
      len += (size_t)pos;
      #else                    /* none of choices exist, use local BSF implementation */
      len += count_bits_to_0(mask);
      #endif
      #else
      len += count_bits_to_0(mask);
      #endif

      break;
    }
  
    str += sizeof(__m128i);
    len += sizeof(__m128i);
  }

  return len;
}
#else
#define cf_strlen(s) strlen(s)
#endif


int cf_remove_path(const char *path) {
  DIR *dir;
  struct dirent *ent = NULL,*entry = NULL;
  char *buff;
  struct stat st;
  size_t len = cf_strlen(path);
  int ret;

  if(lstat(path, &st) < 0) {
    fprintf(stderr,"utils: cf_remove_path: could not lstat '%s': %s\n",path,strerror(errno));
    return -1;
  }

  if(S_ISDIR(st.st_mode)) {
    if((dir = opendir(path)) == NULL) {
      fprintf(stderr,"utils: cf_remove_path: could not opendir '%s': %s\n",path,strerror(errno));
      return -1;
    }

    while((readdir_r(dir,entry,&ent)) == 0) {
      /* take care of . and .. */
      if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) continue;

      /* Recursively call to remove the current entry */
      buff = cf_alloc(NULL,1,len + cf_strlen(ent->d_name) + 2,CF_ALLOC_MALLOC);
      sprintf(buff,"%s/%s",path,ent->d_name);
      ret = cf_remove_path(buff);
      free(buff);

      if(ret != 0) return -1;
    }

    if(rmdir(path) < 0) {
      fprintf(stderr,"utils: cf_remove_path: could not rmdir '%s': %s\n",path,strerror(errno));
      return -1;
    }
  }
  else {
    /* path is not a directory; just unlink() it */
    if(unlink(path) < 0) {
      fprintf(stderr,"utils: cf_remove_path: could not unlink '%s': %s\n",path,strerror(errno));
      return -1;
    }
  }

  return 0;
}


int cf_make_path(const char *path,mode_t mode) {
  char *mpath = strdup(path);
  register char *ptr;

  for(ptr=mpath+1;*ptr;++ptr) {
    if(*ptr == '/') {
      *ptr = '\0';

      if(mkdir(mpath,mode) != 0) {
        if(errno != EEXIST) return -1;
      }

      *ptr = '/';
    }
  }

  if(*(ptr-1) != '/') {
    if(mkdir(mpath,mode) != 0) {
      if(errno != EEXIST) return -1;
    }
  }

  return 0;
}

#ifdef HAS_NO_GETLINE
size_t getline(char **lineptr,size_t *n,FILE *stream) {
  return getdelim(lineptr,n,'\n',stream);
}
#endif

#ifdef HAS_NO_GETDELIM
size_t getdelim(char **lineptr,size_t *n,int delim,FILE *stream) {
  GString buf = {NULL,0,0};
  register char c;

  while(!feof(stream) && !ferror(stream)) {
    c = fgetc(stream);
    g_string_append_c(&buf,c);

    if(c == delim) break;
  }

  if(*lineptr) free(*lineptr);

  *lineptr = buf.str;
  *n       = buf.len;

  if(feof(stream) || ferror(stream)) return -1;

  return buf.len;
}
#endif

#ifdef NOSTRDUP
u_char *strdup(const char *str) {
  size_t len = strlen(str);
  char *buff = cf_alloc(NULL,1,len+1,CF_ALLOC_MALLOC);

  memcpy(buff,str,len+1);

  return buff;
}
#endif

#ifdef NOSTRNDUP
u_char *strndup(const char *str,size_t len) {
  char *buff = cf_alloc(NULL,1,len+1,CF_ALLOC_MALLOC);

  memcpy(buff,str,len);
  buff[len-1] = '\0';

  return buff;
}
#endif


/* eof */
