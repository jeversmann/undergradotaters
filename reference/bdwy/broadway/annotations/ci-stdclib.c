
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int sprintf(char *str, const char *format, ...)
{
  va_list	va;
  int result;

  va_start( va, format );
  result = ci_vsprintf(str, format, va);
  va_end( va );

  return result;
}

int snprintf(char *str, size_t size, const  char  *format, ...)
{
  va_list	va;
  int result;

  va_start( va, format );
  result = ci_vsnprintf(str, size, format, va);
  va_end( va );

  return result;
}

int vsprintf(char *str, const char *format, va_list ap)
{
  return ci_vsprintf(str, format, ap);
}

int vsnprintf(char *str, size_t size, const char  *format, va_list ap)
{
  return ci_vsnprintf(str, size, format, ap);
}

int sscanf( const char *str, const char *format, ...)
{
  va_list	va;
  int result;

  va_start( va, format );
  result = ci_vsscanf(str, format, va);
  va_end( va );

  return result;
}

int vsscanf( const char *str, const char *format, va_list ap)
{
  return ci_vsscanf(str, format, ap);
}

char *strcat(char *dest, const char *src)
{
  return ci_strcat(dest, src);
}

char *strncat(char *dest, const char *src, size_t n)
{
  return ci_strncat(dest, src, n);
}

char *strcpy(char *dest, const char *src)
{
  return ci_strcpy(dest, src);
}

char *strncpy(char *dest, const char *src, size_t n)
{
  return ci_strncpy(dest, src, n);
}

char *strdup(const char *s)
{
  return ci_strdup(s);
}

void *memcpy(void *dest, const void *src, size_t n)
{
  return ci_memcpy(dest, src, n);
}

void *memccpy(void *dest, const void *src, int c, size_t n)
{
  return ci_memccpy(dest, src, c, n);
}

void *memmove(void *dest, const void *src, size_t n)
{
  return ci_memmove(dest, src, n);
}

void bcopy (const void *src, void *dest, size_t n)
{
  ci_bcopy(src, dest, n);
}

