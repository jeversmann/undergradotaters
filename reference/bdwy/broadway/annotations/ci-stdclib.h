
int ci_sprintf(char *str, const char *format, ...);

#define sprintf ci_sprintf

int ci_snprintf(char *str, size_t size, const  char  *format, ...);

#define snprintf ci_snprintf

int ci_vsprintf(char *str, const char *format, va_list ap);

#define vsprintf ci_vsprintf

int ci_vsnprintf(char *str, size_t size, const char  *format, va_list ap);

#define vsnprintf ci_vsnprintf

int ci_sscanf( const char *str, const char *format, ...);

#define sscanf ci_sscanf

int ci_vsscanf( const char *str, const char *format, va_list ap);

#define vsscanf ci_vsscanf

char *ci_strcat(char *dest, const char *src);

#define strcat ci_strcat

char *ci_strncat(char *dest, const char *src, size_t n);

#define strncat ci_strncat

char *ci_strcpy(char *dest, const char *src);

#define strcpy ci_strcpy

char *ci_strncpy(char *dest, const char *src, size_t n);

#define strncpy ci_strncpy

char *ci_strdup(const char *s);

#define strdup ci_strdup

void *ci_memcpy(void *dest, const void *src, size_t n);

#define memcpy ci_memcpy

void *ci_memccpy(void *dest, const void *src, int c, size_t n);

#define memccpy ci_memccpy

void *ci_memmove(void *dest, const void *src, size_t n);

#define memmove ci_memmove

void ci_bcopy (const void *src, void *dest, size_t n);

