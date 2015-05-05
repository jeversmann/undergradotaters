%{

#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>

#include <signal.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <utime.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/uio.h>

#include <stdarg.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#undef vscanf
#undef vfscanf
#undef vsscanf

extern int vscanf(const char * format, va_list args);
extern int vfscanf(FILE * f, const char * format, va_list args);
extern int vsscanf(char * str, const char * format, va_list args);

#include <errno.h>
#undef errno

#include <sys/utsname.h>
#include <netdb.h>

#ifdef CI_STDLIB

/* Set this flag to make the following functions context-insensitive. Don't
forget to also provide the implementation file ci-stdlib.c */

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

#endif

}%

#define SHOW_ERRORS

// --- Debugging options ----------------------------------------------------------

#ifdef SHOW

#define SHOW_WRITE(buffer, IOHandle) \
  report if (FDKind : buffer is-atleast Unknown) \
    "WRITE: at " ++ @context ++ " data from " ++ FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ") " ++ \
    " sent to " ++ FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")\n";

#define SHOW_WRITE_SYSLOG(buffer) \
  report if (FDKind : buffer is-atleast Unknown) \
    "WRITE: at " ++ @context ++ " data from " ++ FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ") " ++ \
    " sent to system log\n";

#define SHOW_READ(IOHandle) \
  report "READ: at " ++ @context ++ ": read data from " ++ FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")" ++ \
    " trust level is " ++ Trust : IOHandle ++ " (" ++ Trust : IOHandle @ever ++ ")\n";

#else

#define SHOW_WRITE(buffer, IOHandle)
#define SHOW_WRITE_SYSLOG(buffer)
#define SHOW_READ(IOHandle)

#endif

// --- Error options --------------------------------------------------------------

#ifdef SHOW_ERRORS

#define ERROR_TAINTED_ARGUMENT(format, format_string) \
  report if (Taint : format_string is-exactly Tainted) \
    "MAYBE-TAINTED-ERROR at " ++ @context ++ ": Tainted format string " ++ [ format ] ++ \
    " -- Source is " ++ Trust : format_string ++ " (" ++ Trust : format_string @ever ++ ") " ++ \
    " " ++ FDKind : format_string ++ " (" ++ FDKind : format_string @ever ++ ")" ++ "\n"; \
  report if (Taint : format_string is-exactly Tainted && \
	     Trust : format_string is-atleast LocallyTrusted) \
    "REAL-TAINTED-ERROR at " ++ @context ++ ": Locally exploitable format string " ++ [ format ] ++ \
    " -- Source is " ++ Trust : format_string ++ " (" ++ Trust : format_string @ever ++ ") " ++ \
    " " ++ FDKind : format_string ++ " (" ++ FDKind : format_string @ever ++ ")" ++ "\n"; \
  report if (Taint : format_string is-exactly Tainted && \
	     ((Trust : format_string is-atmost Untrusted && \
	       FDKind : format_string is-atleast File) || \
	      FDKind : format_string is-atleast Socket)) \
    "REAL-TAINTED-ERROR at " ++ @context ++ ": Remotely exploitable format string " ++ [ format ] ++ \
    " -- Source is " ++ Trust : format_string ++ " (" ++ Trust : format_string @ever ++ ") " ++ \
    " " ++ FDKind : format_string ++ " (" ++ FDKind : format_string @ever ++ ")" ++ "\n";


#define ERROR_FILE_NOT_OPEN(IOHandle, var) \
  report if ( ! FDState : IOHandle is-atleast Open) \
    "MAYBE-CLOSED-ERROR at " ++ @context ++ ": file descriptor or stream " ++ [var] ++ \
      " may not be open -- state is " ++ FDState : IOHandle ++ " (" ++ FDState : IOHandle @ever ++ ")\n"; \
  report if (FDState : IOHandle is-exactly Closed) \
    "REAL-OPEN-ERROR at " ++ @context ++ ": file descriptor or stream " ++ [var] ++ \
      " is not open -- state is " ++ FDState : IOHandle ++ " (" ++ FDState : IOHandle @ever ++ ")\n"; \
  report if (FDState : IOHandle is-atleast Open) \
    "OK-OPEN at " ++ @context ++ ": file descriptor or stream " ++ [var] ++ \
      " is open.\n";

#define ERROR_FTP_BEHAVIOR(buffer, IOHandle) \
  report if (Trust : buffer is-atmost Untrusted && \
	     FDKind : buffer is-atleast File && \
	     FDKind : IOHandle is-atleast Socket) \
    "REAL-FTPGET-ERROR at " ++ @context ++ ": Allows ftp-get behavior: data from " ++ \
    Trust : buffer ++ " (" ++ Trust : buffer @ever ++ ") " ++ \
    FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ")" ++ \
    " sent to " ++ \
    Trust : IOHandle ++ " (" ++ Trust : IOHandle @ever ++ ") " ++ \
    FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")" ++ "\n"; \
  report if (FDKind : buffer is-atleast Socket && \
	     Trust : IOHandle is-atmost Untrusted && \
	     FDKind : IOHandle is-atleast File) \
    "REAL-FTPPUT-ERROR at " ++ @context ++ ": Allows ftp-put behavior: data from " ++ \
    Trust : buffer ++ " (" ++ Trust : buffer @ever ++ ") " ++ \
    FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ")" ++ \
    " sent to " ++ \
    Trust : IOHandle ++ " (" ++ Trust : IOHandle @ever ++ ") " ++ \
    FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")" ++ "\n"; \
  report if (Trust : buffer is-atmost Untrusted && \
	     FDKind : buffer is-atmost File && \
	     (FDKind : IOHandle is-atleast Socket || \
	      FDKind : IOHandle is-exactly Unknown)) \
    "MAYBE-FTPGET-ERROR at " ++ @context ++ ": May allow ftp-get behavior: data from " ++ \
    Trust : buffer ++ " (" ++ Trust : buffer @ever ++ ") " ++ \
    FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ")" ++ \
    " sent to " ++ \
    Trust : IOHandle ++ " (" ++ Trust : IOHandle @ever ++ ") " ++ \
    FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")" ++ "\n"; \
  report if (Trust : IOHandle is-atmost Untrusted && \
	     (FDKind : buffer is-atleast Socket || \
	      FDKind : buffer is-exactly Unknown) && \
	     FDKind : IOHandle is-atmost File) \
    "MAYBE-FTPPUT-ERROR at " ++ @context ++ ": May allow ftp-put behavior: data from " ++ \
    Trust : buffer ++ " (" ++ Trust : buffer @ever ++ ") " ++ \
    FDKind : buffer ++ " (" ++ FDKind : buffer @ever ++ ")" ++ \
    " sent to " ++ \
    Trust : IOHandle ++ " (" ++ Trust : IOHandle @ever ++ ") " ++ \
    FDKind : IOHandle ++ " (" ++ FDKind : IOHandle @ever ++ ")" ++ "\n";

#else

#define ERROR_TAINTED_ARGUMENT(format, format_string)

#define ERROR_FILE_NOT_OPEN(IOHandle, stream)

#define ERROR_FTP_BEHAVIOR(buffer, IOHandle)

#endif

// --- Warning options ------------------------------------------------------------

#ifdef SHOW_WARNINGS

#define WARNING_UNTRUSTED_SOURCE(name, name_string) \
  report if (Trust : name_string is-atmost Untrusted) \
    "WARNING at " ++ @context ++ ": Opening a file specified by " ++ Trust : name_string ++ \
    " source from " ++ FDKind : name_string ++ " (" ++ FDKind : name_string @ever ++ ")\n";

#else

#define WARNING_UNTRUSTED_SOURCE(name, name_string)

#endif

// ------------------------------------------------------------
//  Globals
// ------------------------------------------------------------

global { I/O Disk }

global { asctime_static_buffer,
	 strerror_static_buffer
       }

global { errno }

// ------------------------------------------------------------
// Analysis properties
// ------------------------------------------------------------

property Taint : { Tainted { Untainted } }
                 initially Untainted

property FDKind : { Unknown { File,
			      Socket { Unix { StandardIO },
				       Inet
				     },
			      Environment
			    }
		      }

property Trust : { Untrusted { LocallyTrusted { Trusted }}}
            initially Trusted

property FDState : { Unknown { Open { Read, Write },
			       Closed
			     }
		   }
            initially Closed

property SocketKind : { Unknown { Stream,
				  Datagram,
				  Raw }
		      }

// -- Socket state: this lattice is optimistic; it assumes that library calls succeed.

property SocketState :
    { Connected { Listening { Bound { New { Unused }}}}}
  initially Unused

// --- Standard streams

global {

  stdin --> stdin_file_structure --> stdin_iohandle
  stdout --> stdout_file_structure --> stdout_iohandle
  stderr --> stderr_file_structure --> stderr_iohandle
}

analyze FDKind {
  stdin_iohandle <- StandardIO
  stdout_iohandle <- StandardIO
  stderr_iohandle <- StandardIO
}

analyze Trust {
  stdin_iohandle <- Untrusted
  stdout_iohandle <- Untrusted
  stderr_iohandle <- Untrusted
}

analyze FDState {
  stdin_iohandle <- Read
  stdout_iohandle <- Write
  stderr_iohandle <- Write
}

// ------------------------------------------------------------
//  Error handling
// ------------------------------------------------------------

procedure __errno_location()
{
  on_exit {
    return --> errno
  }
}

procedure strerror(num)
{
  modify { strerror_static_buffer }
  on_exit {
    return --> strerror_static_buffer
  }
}

procedure perror(message)
{
  on_entry {
    message --> message_string
  }
  access { message_string }
  modify { Disk }
}

// ------------------------------------------------------------
// Environment
// ------------------------------------------------------------

procedure uname(buf)
{
  on_entry {
    buf --> struct_utsname { sysname --> sysname_contents
                             nodename --> nodename_contents
                             release --> release_contents
                             version --> version_contents
                             machine --> machine_contents
                             domainname --> domainname_contents }
  }

  modify { sysname_contents,
	     nodename_contents,
	     release_contents,
	     version_contents,
	     machine_contents,
	     domainname_contents }
}

procedure getpid() {}

procedure getppid() {}

procedure getuid() {}

procedure geteuid() {}

procedure getenv(name)
{
  on_entry { name --> name_string }

  access name_string
  modify value_string

  analyze Taint { value_string <- Tainted }

  analyze FDKind { value_string <- Environment }

  analyze Trust { value_string <- name_string }

  on_exit { return --> new value_string }

  WARNING_UNTRUSTED_SOURCE(name, name_string)
}

procedure putenv(name)
{
  on_entry {
    name --> name_string }

  access name_string
}


procedure gettimeofday(tv, tz)
{
  on_entry {
    tv --> struct_timeval { tv_sec, tv_usec }
    tz --> struct_timezone { tz_minuteswest, tz_dsttime }
  }

  modify { struct_timeval.tv_sec,
	     struct_timeval.tv_usec,
	     struct_timezone.tz_minuteswest,
	     struct_timezone.tz_dsttime }
}


procedure time(timep)
{
  on_entry {
    timep --> the_time
  }

  modify { the_time }
}

procedure asctime(timeptr)
{
  on_entry {
    timeptr --> struct_tm { tm_sec         // seconds
                          tm_min         // minutes
                          tm_hour        // hours 
                          tm_mday        // day of the month
                          tm_mon         // month
                          tm_year        // year
                          tm_wday        // day of the week
                          tm_yday        // day in the year
                          tm_isdst       // daylight saving time
			}
  }

  access { struct_tm.tm_sec,
	     struct_tm.tm_min,
	     struct_tm.tm_hour,
	     struct_tm.tm_mday,
	     struct_tm.tm_mon,
	     struct_tm.tm_year,
	     struct_tm.tm_wday,
	     struct_tm.tm_yday,
	     struct_tm.tm_isdst }

  modify { asctime_static_buffer }

  on_exit {
    return --> asctime_static_buffer
  }
}

procedure ctime(timep)
{
  on_entry {
    timep --> the_time
  }

  access the_time
  modify time_string

  on_exit {
    return --> new time_string
  }
}

global { struct_tm_static_buffer }


procedure localtime(timep)
{
  on_entry {
    timep --> the_time
  }

  access { the_time }

  modify { struct_tm_static_buffer }

  on_exit {
    return --> struct_tm_static_buffer
  }

  /* This isn't right: there is only one buffer
  modify {struct_tm.tm_sec,
	    struct_tm.tm_min,
	    struct_tm.tm_hour,
	    struct_tm.tm_mday,
	    struct_tm.tm_mon,
	    struct_tm.tm_year,
	    struct_tm.tm_wday,
	    struct_tm.tm_yday,
	    struct_tm.tm_isdst }

  on_exit {
    return --> new struct_tm { tm_sec         // seconds
                               tm_min         // minutes
                               tm_hour        // hours 
                               tm_mday        // day of the month
                               tm_mon         // month
                               tm_year        // year
                               tm_wday        // day of the week
                               tm_yday        // day in the year
                               tm_isdst       // daylight saving time
			     }
  }
  */
}

procedure gmtime(timep)
{
  on_entry {
    timep --> the_time
  }

  access { the_time }

  modify { struct_tm_static_buffer }

  on_exit {
    return --> struct_tm_static_buffer
  }

  /* See note above
  modify {struct_tm.tm_sec,
	    struct_tm.tm_min,
	    struct_tm.tm_hour,
	    struct_tm.tm_mday,
	    struct_tm.tm_mon,
	    struct_tm.tm_year,
	    struct_tm.tm_wday,
	    struct_tm.tm_yday,
	    struct_tm.tm_isdst }

  on_exit {
    return --> new struct_tm { tm_sec         // seconds
                               tm_min         // minutes
                               tm_hour        // hours 
                               tm_mday        // day of the month
                               tm_mon         // month
                               tm_year        // year
                               tm_wday        // day of the week
                               tm_yday        // day in the year
                               tm_isdst       // daylight saving time
			     }
  }
  */
}

procedure mktime(timeptr)
{
  on_entry {
    timeptr --> struct_tm { tm_sec         // seconds
                          tm_min         // minutes
                          tm_hour        // hours 
                          tm_mday        // day of the month
                          tm_mon         // month
                          tm_year        // year
                          tm_wday        // day of the week
                          tm_yday        // day in the year
                          tm_isdst       // daylight saving time
			}
  }

  access { struct_tm.tm_sec,
	     struct_tm.tm_min,
	     struct_tm.tm_hour,
	     struct_tm.tm_mday,
	     struct_tm.tm_mon,
	     struct_tm.tm_year,
	     struct_tm.tm_wday,
	     struct_tm.tm_yday,
	     struct_tm.tm_isdst }
}

// ------------------------------------------------------------
//  Misc
// ------------------------------------------------------------

/*
procedure exit(status)
{
  modify { Disk }
}
*/

procedure atexit(function_ptr)
{
  on_entry { function_ptr --> function }
  access { function }
}

/*
procedure abort() {}
*/

procedure ferror(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { IOHandle }
}

procedure feof(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { IOHandle }
}

procedure fflush(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

procedure truncate(path, length)
{
  on_entry { path --> path_string }

  access { path_string }
  modify { Disk }
}

procedure ftruncate(fd, length)
{
  modify { Disk }
}

%{
#define _GNU_SOURCE
#include <getopt.h>
}%

global { optarg, optind, opterr, optopt }

procedure getopt(argc, argv, optstring)
{
  on_entry {
    argv --> argv_array --> arg
    optstring --> optstring_contents
  }

  access { arg, optstring_contents }
}

procedure getopt_long(argc, argv, optstring, longopts, longindex)
{
  on_entry {
    argv --> argv_array --> arg
    optstring --> optstring_contents
    longopts --> struct_option { name --> name_string,
				 has_arg,
				 flag --> flag_var,
				 val }
  }

  access { arg, optstring_contents, name_string, struct_option.has_arg,
	     flag_var, struct_option.val }

  modify { flag_var }
}

procedure getopt_long_only(argc, argv, optstring, longopts, longindex)
{
  on_entry {
    argv --> argv_array --> arg
    optstring --> optstring_contents
    longopts --> struct_option { name --> name_string,
				 has_arg,
				 flag --> flag_var,
				 val }
  }

  access { arg, optstring_contents, name_string, struct_option.has_arg,
	     flag_var, struct_option.val }

  modify { flag_var }
}

// ------------------------------------------------------------
//  Memory map
// ------------------------------------------------------------

%{

#include <sys/mman.h>

}%

procedure mmap(start, length, prot, flags, fd, offset)
{
  access { Disk }

  analyze Taint { file_buffer <- Tainted }

  analyze FDKind { file_buffer <- File }

  on_exit { return --> new file_buffer }
}

procedure munmap(buffer_ptr, length)
{
  on_entry { buffer_ptr --> buffer }
  access { Disk }
}

// ------------------------------------------------------------
//  File position
// ------------------------------------------------------------

procedure lseek(fd, offset, whence)
{
  on_entry { fd --> IOHandle }
  modify { IOHandle, Disk }
}

procedure fseek( stream, offset, whence)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

procedure ftell( stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { IOHandle }
}

procedure rewind( stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

procedure fgetpos( stream, pos_ptr)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { IOHandle }
}

procedure fsetpos( stream, pos_ptr)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

// ------------------------------------------------------------
//  File meta-information
// ------------------------------------------------------------

%{

#include <unistd.h>
#include <dirent.h>

}%

procedure readdir(dir)
{
  on_entry { dir --> dirent }

  analyze Taint { name_string <- Tainted } 

  analyze FDKind { name_string <- File }

  on_exit {
    return --> new struct_dirent { d_ino, d_off, d_reclen,
				   d_name --> new name_string }
  }

  modify { struct_dirent.d_ino, struct_dirent.d_off,
	     struct_dirent.d_reclen, name_string }
}

procedure opendir(name)
{
  on_entry { name --> name_string }
  access { name_string }
  modify { dir_entry }

  on_exit { return --> new dir_entry }
}

procedure closedir(dir)
{
  on_entry { dir --> dirent }
  access { dirent }
}


procedure stat(file_name, buf)
{
  on_entry {
    file_name --> file_name_string
    buf --> struct_stat { st_dev,
			    st_ino,
			    st_mode,
			    st_nlink,
			    st_uid,
			    st_gid,
			    st_rdev,
			    st_size,
			    st_blksize,
			    st_blocks,
			    st_atime,
			    st_mtime,
			    st_ctime }
  }

  access { file_name_string, Disk }
  modify { struct_stat.st_dev,
	     struct_stat.st_ino,
	     struct_stat.st_mode,
	     struct_stat.st_nlink,
	     struct_stat.st_uid,
	     struct_stat.st_gid,
	     struct_stat.st_rdev,
	     struct_stat.st_size,
	     struct_stat.st_blksize,
	     struct_stat.st_blocks,
	     struct_stat.st_atime,
	     struct_stat.st_mtime,
	     struct_stat.st_ctime }
}

procedure fstat(fd, buf)
{
  on_entry {
    buf --> struct_stat { st_dev,
			    st_ino,
			    st_mode,
			    st_nlink,
			    st_uid,
			    st_gid,
			    st_rdev,
			    st_size,
			    st_blksize,
			    st_blocks,
			    st_atime,
			    st_mtime,
			    st_ctime }
  }

  access { Disk }
  modify { struct_stat.st_dev,
	     struct_stat.st_ino,
	     struct_stat.st_mode,
	     struct_stat.st_nlink,
	     struct_stat.st_uid,
	     struct_stat.st_gid,
	     struct_stat.st_rdev,
	     struct_stat.st_size,
	     struct_stat.st_blksize,
	     struct_stat.st_blocks,
	     struct_stat.st_atime,
	     struct_stat.st_mtime,
	     struct_stat.st_ctime }
}

procedure lstat(file_name, buf)
{
  on_entry {
    file_name --> file_name_string
    buf --> struct_stat { st_dev,
			    st_ino,
			    st_mode,
			    st_nlink,
			    st_uid,
			    st_gid,
			    st_rdev,
			    st_size,
			    st_blksize,
			    st_blocks,
			    st_atime,
			    st_mtime,
			    st_ctime }
  }

  access { file_name_string, Disk }
  modify { struct_stat.st_dev,
	     struct_stat.st_ino,
	     struct_stat.st_mode,
	     struct_stat.st_nlink,
	     struct_stat.st_uid,
	     struct_stat.st_gid,
	     struct_stat.st_rdev,
	     struct_stat.st_size,
	     struct_stat.st_blksize,
	     struct_stat.st_blocks,
	     struct_stat.st_atime,
	     struct_stat.st_mtime,
	     struct_stat.st_ctime }
}

procedure access(pathname, mode)
{
  on_entry { pathname --> pathname_string }

  access { pathname_string }
}

procedure chmod(path, mode)
{
  on_entry {
    path --> path_string
    mode --> mode_string
  }
  access { path_string, mode_string }
  modify { Disk }
}

procedure fchmod(fd, mode)
{
  modify { Disk }
}

procedure utime(filename, buf)
{
  on_entry {
    filename --> filename_string
    buf --> struct_utimebuf
  }
  access { filename_string, Disk }
  modify { struct_utimebuf }
}

procedure chown(path, owner, group)
{
  on_entry {
    path --> path_string
  }
  access { path_string }
  modify { Disk }
}

procedure fchown(fd, owner, group)
{
  modify { Disk }
}

procedure chdir(path)
{
  on_entry { path --> path_string }

  access { path_string }
  modify { Disk }
}

procedure chroot(path)
{
  on_entry { path --> path_string }

  access { path_string }
  modify { Disk }
}

procedure fchdir(fd)
{
  on_entry { fd --> IOHandle }

  access { IOHandle }
  modify { Disk }
}

procedure isatty(fd)
{
  on_entry { fd --> IOHandle }
  access { IOHandle }
}

// ------------------------------------------------------------
//  File descriptor manipulation
// ------------------------------------------------------------

procedure fcntl(fd, cmd, arg)
{
  on_entry { fd --> IOHandle }

  modify { IOHandle }
}

%{

#include <sys/ioctl.h>

}%

procedure ioctl(fd, request, arg)
{
  on_entry { fd --> IOHandle }

  modify { IOHandle }
}


// ------------------------------------------------------------
// Open and close
// ------------------------------------------------------------

procedure fopen(path, mode)
{
  on_entry { path --> path_string
             mode --> mode_string  }
  access { path_string, mode_string }
  modify { Disk }
  on_exit { return --> new file_structure --> new IOHandle }

  analyze FDKind { IOHandle <- File }

  analyze FDState { IOHandle <- Open }

  analyze Trust {
    if (path_string is-atmost LocallyTrusted) IOHandle <- path_string
    default IOHandle <- LocallyTrusted
  }

  WARNING_UNTRUSTED_SOURCE(path, path_string)
}

procedure open(path, flags, mode)
{
  on_entry {  path --> path_string  }
  access { path_string }
  modify { Disk }
  on_exit { return --> new IOHandle }

  analyze FDKind { IOHandle <- File }

  analyze FDState { IOHandle <- Open }

  analyze Trust {
    if (path_string is-atmost LocallyTrusted) IOHandle <- path_string
    default IOHandle <- LocallyTrusted
  }

  WARNING_UNTRUSTED_SOURCE(path, path_string)
}


procedure fdopen(fd, mode)
{
  on_entry { fd --> IOHandle
             mode --> mode_string  }
  access { mode_string }
  modify { Disk }
  on_exit { return --> new file_structure --> IOHandle }

  analyze FDState { IOHandle <- Open }
}

procedure freopen(path, mode, stream)
{
  on_entry { path --> path_string
             mode --> mode_string
	     stream --> old_file_structure --> IOHandle }
  access { path_string, mode_string }
  modify { Disk }
  on_exit { return --> new file_structure --> IOHandle
	    delete old_file_structure }

  analyze FDKind { IOHandle <- File }

  analyze FDState { IOHandle <- Open }

  analyze Trust {
    if (path_string is-atmost LocallyTrusted) IOHandle <- path_string
    default IOHandle <- LocallyTrusted
  }

  WARNING_UNTRUSTED_SOURCE(path, path_string)
}

procedure umask(mask) {}

procedure creat(path, mode)
{
  on_entry {
    path --> path_string
  }
  access { path_string }
  modify { Disk }
}

procedure fclose(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { Disk }
  on_exit { delete IOHandle
	    delete file_structure
	  }

  analyze FDState { IOHandle <- Closed }
  analyze SocketState { IOHandle <- Unused }
}

procedure close( fd )
{
  on_entry { fd --> IOHandle }
  modify { Disk }
  on_exit { delete IOHandle }

  analyze FDState { IOHandle <- Closed }
  analyze SocketState { IOHandle <- Unused }
}

procedure rename(old_path, new_path)
{
  on_entry {
    old_path --> old_path_string
    new_path --> new_path_string
  }

  access { old_path_string, new_path_string }
  modify { Disk }
}

procedure remove(path)
{
  on_entry {
    path --> path_string
  }
  access { path_string }
  modify { Disk }
}


procedure unlink(path)
{
  on_entry {
    path --> path_string
  }
  access { path_string }
  modify { Disk }
}

procedure fileno(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { Disk }
  on_exit { return --> IOHandle }
}

// ------------------------------------------------------------
//  printf
// ------------------------------------------------------------

procedure printf(format, args)
{
  on_entry {
    format --> format_string
    args --> arg_contents
  }
  access { format_string, arg_contents }
  modify { stdout_iohandle, Disk }

  SHOW_WRITE(arg_contents, stdout_iohandle)

  ERROR_FTP_BEHAVIOR(arg_contents, stdout_iohandle)

  ERROR_TAINTED_ARGUMENT(format, format_string)

  ERROR_FILE_NOT_OPEN(stdout_iohandle, stdout)
}

procedure fprintf(stream, format, args)
{
  on_entry {
    stream --> file_structure --> IOHandle
    format --> format_string
    args --> arg_contents
  }
  access { format_string, arg_contents }
  modify { IOHandle, Disk }

  SHOW_WRITE(arg_contents, IOHandle)

  ERROR_FTP_BEHAVIOR(arg_contents, IOHandle)

  ERROR_TAINTED_ARGUMENT(format, format_string)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure sprintf(str, format, args)
{
  on_entry { str --> output_string
	     format --> format_string
	     args --> arg_contents }

  access { output_string, format_string, arg_contents }
  modify { output_string }

  analyze Taint  { output_string <-+ arg_contents }
  analyze FDKind { output_string <-+ arg_contents }
  analyze Trust  { output_string <-+ arg_contents }

  ERROR_TAINTED_ARGUMENT(format, format_string)

}

procedure snprintf(str, size, format, args)
{
  on_entry { str --> output_string
	     format --> format_string
	     args --> arg_contents }

  access { output_string, format_string, arg_contents }
  modify { output_string }

  analyze Taint  { output_string <-+ arg_contents }
  analyze FDKind { output_string <-+ arg_contents }
  analyze Trust  { output_string <-+ arg_contents }

  ERROR_TAINTED_ARGUMENT(format, format_string)

    /*
  report 
    "SNPRINTF at " ++ @context ++ ": Format string " ++ [ format ] ++
    " -- Tainted value is " ++ Taint : format_string ++ " (" ++ Taint : format_string @ever ++ ") " ++
    " -- Source is " ++ Trust : format_string ++ " (" ++ Trust : format_string @ever ++ ") " ++
    " " ++ FDKind : format_string ++ " (" ++ FDKind : format_string @ever ++ ")" ++
    ", buffer is " ++ [ format_string ] ++ 
    " -- Trace: \n" ++ FDKind : format_string @trace ++ "\n";
*/
}

procedure vprintf(format, args)
{
  on_entry { format --> format_string
	     args --> arg_contents  }
  access { format_string, arg_contents }
  modify { stdout_iohandle, Disk }

  ERROR_TAINTED_ARGUMENT(format, format_string)

  SHOW_WRITE(arg_contents, stdout_iohandle)

  ERROR_FTP_BEHAVIOR(arg_contents, stdout_iohandle)

  ERROR_FILE_NOT_OPEN(stdout_iohandle, stdout)
}

procedure vfprintf(stream, format, args)
{
  on_entry {
    stream --> file_structure --> IOHandle
    format --> format_string
    args --> arg_contents
  }
  access { format_string, arg_contents }
  modify { IOHandle, Disk }

  SHOW_WRITE(arg_contents, IOHandle)

  ERROR_FTP_BEHAVIOR(arg_contents, IOHandle)

  ERROR_TAINTED_ARGUMENT(format, format_string)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure vsprintf(str, format, args)
{
  on_entry {
    str --> output_string
    format --> format_string
    args --> arg_contents
  }
  access { output_string, format_string, arg_contents }
  modify { output_string }

  analyze Taint  { output_string <-+ arg_contents }
  analyze FDKind { output_string <-+ arg_contents }
  analyze Trust  { output_string <-+ arg_contents }

  ERROR_TAINTED_ARGUMENT(format, format_string)

}

procedure vsnprintf(str, size, format, args)
{
  on_entry {
    str --> output_string
    format --> format_string
    args --> arg_contents
  }
  access { output_string, format_string, arg_contents }
  modify { output_string }

  analyze Taint  { output_string <-+ arg_contents }
  analyze FDKind { output_string <-+ arg_contents }
  analyze Trust  { output_string <-+ arg_contents }

  ERROR_TAINTED_ARGUMENT(format, format_string)
}

// ------------------------------------------------------------
//  scanf
// ------------------------------------------------------------

procedure scanf(format, args)
{
  on_entry {
    format --> format_string
    args --> arg_contents --> arg_refs
  }
  access { format_string, Disk }
  modify { arg_contents, arg_refs, stdin_iohandle }

  analyze Taint  { arg_refs <-+ Tainted }
  analyze FDKind { arg_refs <-+ StandardIO }
  analyze Trust  { arg_refs <-+ Untrusted }

  ERROR_TAINTED_ARGUMENT(format, format_string)

  SHOW_READ(stdin_iohandle)

  ERROR_FILE_NOT_OPEN(stdin_iohandle, stdin)
}

procedure fscanf(stream, format, args)
{
  on_entry {
    stream --> file_structure --> IOHandle
    format --> format_string
    args --> arg_contents --> arg_refs
  }

  access { format_string, Disk }
  modify { arg_contents, arg_refs, IOHandle }

  analyze Taint  { arg_refs <-+ Tainted }
  analyze FDKind { arg_refs <-+ IOHandle }
  analyze Trust  { arg_refs <-+ IOHandle }

  ERROR_TAINTED_ARGUMENT(format, format_string)

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure sscanf(str, format, args)
{
  on_entry {
    str --> input_string
    format --> format_string
    args --> arg_contents --> arg_refs
  }
  access { input_string, format_string }
  modify { arg_contents, arg_refs }

  analyze Taint {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  analyze FDKind {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  analyze Trust {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  ERROR_TAINTED_ARGUMENT(format, format_string)
}

procedure vscanf(format, args)
{
  on_entry {
    format --> format_string
    args --> arg_contents --> arg_refs
  }
  access { format_string, Disk }
  modify { arg_contents, arg_refs, stdin_iohandle }

  analyze Taint  { arg_refs <-+ Tainted }
  analyze FDKind { arg_refs <-+ StandardIO }
  analyze Trust  { arg_refs <-+ Untrusted }

  ERROR_TAINTED_ARGUMENT(format, format_string)

  SHOW_READ(stdin_iohandle)

  ERROR_FILE_NOT_OPEN(stdin_iohandle, stdin)
}

procedure vfscanf(stream, format, args)
{
  on_entry {
    stream --> file_structure --> IOHandle
    format --> format_string
    args --> arg_contents --> arg_refs  }

  access { format_string, Disk }
  modify { arg_contents, arg_refs, IOHandle }

  analyze Taint  { arg_refs <-+ Tainted }
  analyze FDKind { arg_refs <-+ IOHandle }
  analyze Trust  { arg_refs <-+ IOHandle }

  ERROR_TAINTED_ARGUMENT(format, format_string)

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure vsscanf(str, format, args)
{
  on_entry {
    str --> input_string
    format --> format_string
    args --> arg_contents --> arg_refs }

  access { input_string, format_string }
  modify { arg_contents, arg_refs }

  analyze Taint {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  analyze FDKind {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  analyze Trust {
    arg_contents <-+ input_string
    arg_refs <-+ input_string
  }

  ERROR_TAINTED_ARGUMENT(format, format_string)
}

// ------------------------------------------------------------
//  get
// ------------------------------------------------------------

procedure fgetc(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { Disk }
  modify { IOHandle }

  analyze Taint  { return <- Tainted }
  analyze FDKind { return <- IOHandle }
  analyze Trust  { return <- IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure fgets(str, size, stream)
{
  on_entry { str --> string
	     stream --> file_structure --> IOHandle }

  access { Disk }
  modify { IOHandle, string }

  analyze Taint  { string <-+ Tainted }
  analyze FDKind { string <-+ IOHandle }
  analyze Trust  { string <-+ IOHandle }

  on_exit { return --> string }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure getc(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  access { Disk }
  modify { IOHandle }

  analyze Taint  { return <- Tainted }
  analyze FDKind { return <- IOHandle }
  analyze Trust  { return <- IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure getchar()
{
  access { Disk }
  modify { stdin_iohandle }

  analyze Taint  { return <- Tainted }
  analyze FDKind { return <- StandardIO }
  analyze Trust  { return <- Untrusted }

  SHOW_READ(stdin_iohandle)

  ERROR_FILE_NOT_OPEN(stdin_iohandle, stdin)
}

procedure gets(str)
{
  on_entry { str --> string }
  access { Disk }
  modify { string, stdin_iohandle }

  analyze Taint  { string <-+ Tainted }
  analyze FDKind { string <-+ StandardIO }
  analyze Trust  { string <-+ Untrusted }

  on_exit { return --> string }

  SHOW_READ(stdin_iohandle)

  ERROR_FILE_NOT_OPEN(stdin_iohandle, stdin)
}

procedure ungetc(c, stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { stream, IOHandle, Disk }
}

// ------------------------------------------------------------
//  put
// ------------------------------------------------------------

procedure fputc(c, stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }

  SHOW_WRITE(c, IOHandle)

  ERROR_FTP_BEHAVIOR(c, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure fputs(str, stream)
{
  on_entry {
    str --> string
    stream --> file_structure --> IOHandle
  }
  access { string }
  modify { IOHandle, Disk }  

  SHOW_WRITE(string, IOHandle)

  ERROR_FTP_BEHAVIOR(string, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure putc(c, stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }

  SHOW_WRITE(c, IOHandle)

  ERROR_FTP_BEHAVIOR(c, IOHandle)

  ERROR_FILE_NOT_OPEN(stdout_iohandle, stdout)
}

procedure putchar(c)
{
  modify { stdout_iohandle, Disk }  

  SHOW_WRITE(c, stdout_iohandle)

  ERROR_FTP_BEHAVIOR(c, stdout_iohandle)

  ERROR_FILE_NOT_OPEN(stdout_iohandle, stdout)
}

procedure puts(str)
{
  on_entry { str --> string }
  access { string }
  modify { stdout_iohandle, Disk }  

  SHOW_WRITE(string, stdout_iohandle)

  ERROR_FTP_BEHAVIOR(string, stdout_iohandle)

  ERROR_FILE_NOT_OPEN(stdout_iohandle, stdout)
}

// ------------------------------------------------------------
//  read
// ------------------------------------------------------------

procedure fread(buf_ptr, size, num, stream)
{
  on_entry {
    stream --> file_structure --> IOHandle
    buf_ptr --> buffer
  }
  access { Disk }
  modify { IOHandle, buffer }

  analyze Taint  { buffer <-+ Tainted }
  analyze FDKind { buffer <-+ IOHandle }
  analyze Trust  { buffer <-+ IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream) 
}

procedure read(fd, buf_ptr, size)
{
  on_entry {
    fd --> IOHandle
    buf_ptr --> buffer
  }

  access { Disk }
  modify { IOHandle, buffer }

  analyze Taint  { buffer <-+ Tainted }
  analyze FDKind { buffer <-+ IOHandle }
  analyze Trust  { buffer <-+ IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, fd)
}

procedure readv(fd, vector, count)
{
  on_entry {
    fd --> IOHandle
    vector --> struct_iovec { iov_base --> buffer,
			      iov_len }
  }

  access { Disk }
  modify { IOHandle, buffer }

  analyze Taint  { buffer <-+ Tainted }
  analyze FDKind { buffer <-+ IOHandle }
  analyze Trust  { buffer <-+ IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, fd)
}

// ------------------------------------------------------------
//  write
// ------------------------------------------------------------

procedure fwrite(buf_ptr, size, num, stream)
{
  on_entry {
    stream --> file_structure --> IOHandle
    buf_ptr --> buffer
  }
  access { buffer }
  modify { IOHandle, Disk }

  SHOW_WRITE(buffer, IOHandle)

  ERROR_FTP_BEHAVIOR(buffer, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, stream)
}

procedure write(fd, buf_ptr, size)
{
  on_entry {
    fd --> IOHandle
    buf_ptr --> buffer
  }
  access { buffer }
  modify { IOHandle, Disk }

  SHOW_WRITE(buffer, IOHandle)

  ERROR_FTP_BEHAVIOR(buffer, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, fd)
}

procedure writev(fd, iovec, count)
{
  on_entry {
    fd --> IOHandle
    iovec --> struct_iovec { iov_base --> buffer,
			     iov_len }
  }
  access { buffer }
  modify { IOHandle, Disk }

  SHOW_WRITE(buffer, IOHandle)

  ERROR_FTP_BEHAVIOR(buffer, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, fd)
}

// ------------------------------------------------------------
//  strings
// ------------------------------------------------------------

procedure strcat(dest, src)
{
  on_entry {
    dest --> dest_string
    src --> src_string
  }
  access { src_string }
  modify { dest_string }

  analyze Taint     { dest_string <-+ src_string }
  analyze FDKind    { dest_string <-+ src_string }
  analyze Trust     { dest_string <-+ src_string }

  on_exit { return --> dest_string }
}

procedure strncat(dest, src, size)
{
  on_entry {
    dest --> dest_string
    src --> src_string
  }
  access { src_string }
  modify { dest_string }

  analyze Taint     { dest_string <-+ src_string }
  analyze FDKind    { dest_string <-+ src_string }
  analyze Trust     { dest_string <-+ src_string }

  on_exit {
    return --> dest_string
  }
}

procedure strcmp(s1, s2)
{
  on_entry {
    s1 --> s1_string
    s2 --> s2_string
  }
  access { s1_string }
  modify { s2_string }
}

procedure strncmp(s1, s2, size)
{
  on_entry {
    s1 --> s1_string
    s2 --> s2_string
  }
  access { s1_string }
  modify { s2_string }
}

procedure strcasecmp(s1, s2)
{
  on_entry {
    s1 --> s1_string
    s2 --> s2_string
  }
  access { s1_string }
  modify { s2_string }
}

procedure strncasecmp(s1, s2, size)
{
  on_entry {
    s1 --> s1_string
    s2 --> s2_string
  }
  access { s1_string }
  modify { s2_string }
}

procedure strcpy(dest, src)
{
  on_entry {
    dest --> dest_string
    src --> src_string
  }
  access { src_string }
  modify { dest_string }

  analyze Taint     { dest_string <-+ src_string }
  analyze FDKind    { dest_string <-+ src_string }
  analyze Trust     { dest_string <-+ src_string }

  on_exit { return --> dest_string }
}

procedure stpcpy(dest, src)
{
  on_entry {
    dest --> dest_string
    src --> src_string
  }
  access { src_string }
  modify { dest_string }

  analyze Taint     { dest_string <-+ src_string }
  analyze FDKind    { dest_string <-+ src_string }
  analyze Trust     { dest_string <-+ src_string }

  on_exit { return --> dest_string }
}

procedure strncpy(dest, src, size)
{
  on_entry {
    dest --> dest_string
    src --> src_string
  }
  access { src_string }
  modify { dest_string }

  analyze Taint     { dest_string <-+ src_string }
  analyze FDKind    { dest_string <-+ src_string }
  analyze Trust     { dest_string <-+ src_string }

  on_exit { return --> dest_string }
}

procedure strlen(s)
{
  on_entry { s --> s_string }
  access { s_string }
}

procedure strdup(src)
{
  on_entry { src --> src_string }

  access { src_string }
  modify { dup_string }

  analyze Taint     { dup_string <- src_string }
  analyze FDKind    { dup_string <- src_string }
  analyze Trust     { dup_string <- src_string }

  on_exit { return --> new dup_string }
}

procedure strpbrk(str, find_str)
{
  on_entry {
    str --> string
    find_str --> find_string
  }

  access { string, find_string }

  on_exit { return --> string }
}

procedure strstr(str, find_str)
{
  on_entry {
    str --> string
    find_str --> find_string
  }

  access { string, find_string }

  on_exit { return --> string }
}

global { strtok_static_pointer }

procedure strtok(str, find_str)
{
  on_entry {
    str --> string
    find_str --> find_string
    strtok_static_pointer --> previous_string
  }

  access { string, find_string }

  on_exit {
    if (str == 0) {
      strtok_static_pointer --> previous_string
      return --> previous_string
    }

    default {
      strtok_static_pointer --> string
      return --> string
    }
  }
}

procedure strchr(str, find_char)
{
  on_entry { str --> string }

  access { string }

  on_exit { return --> string }
}

procedure strrchr(str, find_char)
{
  on_entry { str --> string }

  access { string }

  on_exit { return --> string }
}

procedure tolower(c) {}

procedure toupper(c) {}

// ------------------------------------------------------------
//  Memory operations
// ------------------------------------------------------------

procedure memcmp(ptr1, ptr2, size)
{
  on_entry {
    ptr1 --> buffer_1
    ptr2 --> buffer_2
  }
  access { buffer_1, buffer_2 }
}

procedure memcpy(dest, src, size)
{
  on_entry {
    dest --> dest_buffer
    src --> src_buffer
  }

  access { src_buffer }
  modify { dest_buffer }

  analyze Taint     { dest_buffer <-+ src_buffer }
  analyze FDKind    { dest_buffer <-+ src_buffer }
  analyze Trust     { dest_buffer <-+ src_buffer }

  on_exit { return --> dest_buffer }
}

procedure memccpy(dest, src, c, size)
{
  on_entry {
    dest --> dest_buffer
    src --> src_buffer
  }

  access { src_buffer }
  modify { dest_buffer }

  analyze Taint     { dest_buffer <-+ src_buffer }
  analyze FDKind    { dest_buffer <-+ src_buffer }
  analyze Trust     { dest_buffer <-+ src_buffer }

  on_exit { return --> dest_buffer }
}

procedure memmove(dest, src, size)
{
  on_entry {
    dest --> dest_buffer
    src --> src_buffer
  }

  access { src_buffer }
  modify { dest_buffer }

  analyze Taint     { dest_buffer <-+ src_buffer }
  analyze FDKind    { dest_buffer <-+ src_buffer }
  analyze Trust     { dest_buffer <-+ src_buffer }

  on_exit { return --> dest_buffer }
}

procedure memset(dest, c, size)
{
  on_entry { dest --> dest_buffer }

  modify { dest_buffer }

  analyze Taint { dest_buffer <- c }

  on_exit { return --> dest_buffer }
}

procedure bzero(dest, size)
{
  on_entry { dest --> dest_buffer }

  modify { dest_buffer }
}

procedure bcopy(src, dest, size)
{
  on_entry {
    dest --> dest_buffer
    src --> src_buffer
  }

  access { src_buffer }
  modify { dest_buffer }

  analyze Taint     { dest_buffer <-+ src_buffer }
  analyze FDKind    { dest_buffer <-+ src_buffer }
  analyze Trust     { dest_buffer <-+ src_buffer }
}

// ------------------------------------------------------------
//  Network information
// ------------------------------------------------------------

procedure gethostname(name, size)
{
  on_entry {
    name --> name_string
  }
  modify { name_string }
}

global { struct_hostent_global { h_name --> h_name_string_global
                                 h_aliases --> h_alias_array_global --> h_alias_entry_global
				 h_addrtype
                                 h_length
                                 h_addr_list --> h_addr_array_global --> h_addr_entry_global }
       }

procedure gethostbyname(name)
{
  on_entry { name --> name_string }

  access { name_string }

  modify { h_name_string_global,
	   h_alias_entry_global,
	   struct_hostent_global.h_addrtype,
	   struct_hostent_global.h_length,
	   h_addr_entry_global }

  analyze Taint     { h_name_string_global <- Tainted
		      h_alias_entry_global <- Tainted
		    }

  analyze FDKind    { h_name_string_global <- File
		      h_alias_entry_global <- File
		    }

  analyze Trust     { h_name_string_global <- Untrusted
		      h_alias_entry_global <- Untrusted
		    }

  on_exit { 
    /* struct_hostent_global.h_name --> h_name_string_global */
    return --> struct_hostent_global
  }
}

procedure gethostbyaddr(addr, len, type)
{
  on_entry { addr --> addr_string }

  access { addr_string }

  modify { h_name_string_global,
	   h_alias_entry_global,
	   struct_hostent_global.h_addrtype,
	   struct_hostent_global.h_length,
	   h_addr_entry_global }

  analyze Taint     { h_name_string_global <- Tainted
		      h_alias_entry_global <- Tainted
		    }

  analyze FDKind    { h_name_string_global <- File
		      h_alias_entry_global <- File
		    }

  analyze Trust     { h_name_string_global <- Untrusted
		      h_alias_entry_global <- Untrusted
		    }


  on_exit { return --> struct_hostent_global  }
}

// These are declared in netdb.h

procedure setnetgrent(item)
{
  on_entry {
    item --> item_string
  }

  access { item_string }
}

procedure getnetgrent(machine_p, user_p, domain_p)
{
  on_entry {
    machine_p --> machine
    user_p --> user
    domain_p --> domain
  }

  modify { machine_string, user_string, domain_string }

  on_exit {
    machine --> new machine_string
    user --> new user_string
    domain --> new domain_string
  }
}

procedure endnetgrent() {}

%{
#include <pwd.h>
}%

global { struct_passwd_global,
	   pw_name_string_global,
	   pw_passwd_string_global,
	   pw_gecos_string_global,
	   pw_dir_string_global,
	   pw_shell_string_global }

procedure getpwnam(name)
{
  on_entry {
    name --> name_string
    struct_passwd_global { pw_name, pw_passwd, pw_uid, pw_gid, pw_gecos, pw_dir, pw_shell }
  }

  access { name_string }

  modify { pw_name_string_global,
	     pw_passwd_string_global,
	     struct_passwd_global.pw_uid,
	     struct_passwd_global.pw_gid,
	     pw_gecos_string_global,
	     pw_dir_string_global,
	     pw_shell_string_global }

  on_exit {
    return --> struct_passwd_global { pw_name --> pw_name_string_global
                                    pw_passwd --> pw_passwd_string_global
                                    pw_uid,
                                    pw_gid,
                                    pw_gecos --> pw_gecos_string_global
                                    pw_dir --> pw_dir_string_global
                                    pw_shell --> pw_shell_string_global }
  }
}

// ------------------------------------------------------------
//  System log
// ------------------------------------------------------------

procedure openlog( ident, option, facility)
{
  on_entry {
    ident --> ident_string
  }
  access { ident_string }
  modify { Disk }
}

procedure syslog( priority, format, args)
{
  on_entry {
    format --> format_string
    args --> arg_contents
  }
  access { format_string, arg_contents }
  modify { Disk }

  SHOW_WRITE_SYSLOG(arg_contents)

  ERROR_TAINTED_ARGUMENT(format, format_string)
}

procedure closelog( )
{
  modify { Disk }
}

// ------------------------------------------------------------
//  Reg exp
// ------------------------------------------------------------

#ifndef SKIP_REGEXP

%{

#include <regex.h>

}%

procedure regcomp(preg, regex, cflags)
{
  on_entry {
    preg --> regex_buffer
    regex --> regex_string
  }

  access { regex_string }
  modify { regex_buffer }
}

procedure regexec(preg,  string, nmatch,pmatch, eflags)
{
  on_entry {
    preg --> regex_buffer
    string --> string_contents
    pmatch --> pmatch_array --> regmatch_struct { rm_so, rm_eo }
  }

  access { regex_buffer, string_contents }
  modify { regmatch_struct.rm_so, regmatch_struct.rm_eo }
}

procedure regerror(errcode, preg, errbuf, errbuf_size)
{
  on_entry {
    preg --> regex_buffer
    errbuf --> errbuf_contents
  }

  access { regex_buffer }
  modify { errbuf_contents }
}

procedure regfree(preg)
{
  on_entry {
    preg --> regex_buffer
  }

  access { regex_buffer }
}

#endif

// ------------------------------------------------------------
//  Signals
// ------------------------------------------------------------

procedure signal(signum, function)
{
  modify { Disk }
}

// ------------------------------------------------------------
// Sockets
// ------------------------------------------------------------

procedure socket(domain, type, protocol)
{
  analyze FDKind {
    if (domain == AF_UNIX)
      IOHandle <- Unix

    if ((domain == AF_INET) ||
	(domain == AF_INET6))
      IOHandle <- Inet

    default
      IOHandle <- Socket
  }

  analyze Trust {
    if (domain == AF_UNIX)
      IOHandle <- LocallyTrusted

    if ((domain == AF_INET) ||
	(domain == AF_INET6))
      IOHandle <- Untrusted

    default
      IOHandle <- Untrusted
  }

  analyze SocketKind {
    if (type == SOCK_STREAM)
      IOHandle <- Stream
    if (type == SOCK_DGRAM)
      IOHandle <- Datagram
    if (type == SOCK_RAW)
      IOHandle <- Raw
    default
      IOHandle <- Unknown
  }

  analyze SocketState {
    IOHandle <- New
  }

  on_exit { return --> new IOHandle }
}

procedure bind(s, addr_ptr, add_len)
{
  on_entry { s --> IOHandle 
             addr_ptr --> addr { // --- INET addresses
                                 sin_family,
			         sin_port,
			         sin_addr { s_addr },
				 // --- UNIX addresses
				 sun_family,
				 sun_path --> path
			       }
	   }

  access { addr.sin_family,
	   addr.sin_port,
	   addr.sin_addr.s_addr,
	   addr.sun_family,
	   path }

  analyze SocketState { IOHandle <- Bound }

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost New)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot be bound (state is " ++
      SocketState : IOHandle ++ ")\n";

  report if ((addr.sin_family == AF_INET) &&
	     ! FDKind : IOHandle is-exactly Inet)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot be bound to INET address" ++
      " (kind is " ++ FDKind : IOHandle ++ ")\n";

  report if ((addr.sun_family == AF_UNIX) &&
	     ! FDKind : IOHandle is-exactly Unix)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot be bound to UNIX address" ++
      " (kind is " ++ FDKind : IOHandle ++ ")\n";
#endif
}  

procedure listen(s, backlog)
{
  on_entry { s --> IOHandle }

  analyze SocketState { IOHandle <- Listening }

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost Bound)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot listen (state is " ++
      SocketState : IOHandle ++ ")\n";
#endif
}


procedure accept(s, addr_ptr, addr_len_ptr)
{
  on_entry { s --> listen_IOHandle
             addr_ptr --> addr { // --- INET addresses
                                 sin_family,
			         sin_port,
			         sin_addr { s_addr },
				 // --- UNIX addresses
				 sun_family,
				 sun_path --> path
			       }
	     addr_len_ptr --> add_len
	   }

  modify { addr.sin_family,
	   addr.sin_port,
	   addr.sin_addr.s_addr,
	   addr.sun_family,
	   path }

  on_exit { return --> new IOHandle }

  analyze FDKind      { IOHandle <- listen_IOHandle }
  analyze Trust       { IOHandle <- listen_IOHandle }
  analyze FDState     { IOHandle <- Open }
  analyze SocketKind  { IOHandle <- listen_IOHandle }
  analyze SocketState { IOHandle <- Connected }

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost Listening)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot accept (state is " ++
      SocketState : IOHandle ++ ")\n";
#endif
}

procedure connect(s, addr_ptr, add_len)
{
  on_entry { s --> IOHandle
             addr_ptr --> addr { // --- INET addresses
                                 sin_family,
			         sin_port,
			         sin_addr { s_addr },
				 // --- UNIX addresses
				 sun_family,
				 sun_path --> path
			       }
	   }

  access { addr.sin_family,
	   addr.sin_port,
	   addr.sin_addr.s_addr,
	   addr.sun_family,
	   path }

  analyze FDState     { IOHandle <- Open }
  analyze SocketState { IOHandle <- Connected }

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost New)
    "ERROR at " ++ @context ++ ": socket " ++ [ s ] ++ " cannot be connected (state is " ++
      SocketState : IOHandle ++ ")\n";
#endif
}

procedure socketpair(domain, type, protocol, sv)
{
  on_entry { sv --> s }

  on_exit { s --> new IOHandle }

  analyze FDKind {
    if (domain == AF_UNIX)
      IOHandle <- Unix

    if ((domain == AF_INET) ||
	(domain == AF_INET6))
      IOHandle <- Inet

    default
      IOHandle <- Socket
  }

  analyze Trust {
    if (domain == AF_UNIX)
      IOHandle <- LocallyTrusted

    if ((domain == AF_INET) ||
	(domain == AF_INET6))
      IOHandle <- Untrusted

    default
      IOHandle <- Untrusted
  }

  analyze SocketKind {
    if (type == SOCK_STREAM)
      IOHandle <- Stream
    if (type == SOCK_DGRAM)
      IOHandle <- Datagram
    if (type == SOCK_RAW)
      IOHandle <- Raw
    default
      IOHandle <- Unknown
  }

  analyze FDState     { IOHandle <- Open }
  analyze SocketState { IOHandle <- Connected }
}

procedure pipe(sv)
{
  on_entry { sv --> s }

  on_exit { s --> new IOHandle }

  analyze FDKind      { IOHandle <- Unix }
  analyze Trust       { IOHandle <- LocallyTrusted }
  analyze FDState     { IOHandle <- Open }
  analyze SocketKind  { IOHandle <- Stream }
  analyze SocketState { IOHandle <- Connected }
}

procedure dup(oldfd)
{
  on_entry { oldfd --> IOHandle }
  on_exit { return --> IOHandle }
}

procedure dup2(oldfd, newfd)
{
  on_entry { oldfd --> old_IOHandle
	     newfd --> new_IOHandle }

  analyze FDKind
    {
      if (newfd == 0) { stdin_iohandle <- old_IOHandle }
      if (newfd == 1) { stdout_iohandle <- old_IOHandle }
      if (newfd == 2) { stderr_iohandle <- old_IOHandle }
      default { new_IOHandle <- Unknown }
    }

  analyze Trust
    {
      if (newfd == 0) { stdin_iohandle <- old_IOHandle }
      if (newfd == 1) { stdout_iohandle <- old_IOHandle }
      if (newfd == 2) { stderr_iohandle <- old_IOHandle }
      default { new_IOHandle <- Untrusted }
    }

  analyze FDState
    {
      if (newfd == 0) { stdin_iohandle <- old_IOHandle }
      if (newfd == 1) { stdout_iohandle <- old_IOHandle }
      if (newfd == 2) { stderr_iohandle <- old_IOHandle }
      default { new_IOHandle <- Closed }
    }

  analyze SocketState
    {
      if (newfd == 0) { stdin_iohandle <- old_IOHandle }
      if (newfd == 1) { stdout_iohandle <- old_IOHandle }
      if (newfd == 2) { stderr_iohandle <- old_IOHandle }
      default { new_IOHandle <- New }
    }

  on_exit { newfd --> old_IOHandle
	    return --> old_IOHandle }
}

procedure shutdown(s, how)
{
  on_entry { s --> IOHandle }

  modify { IOHandle }

  analyze SocketState {
    IOHandle <- New
  }
}

procedure setsockopt(s, level, optname, optval, optlen)
{
  on_entry { s --> IOHandle
	     optval --> optval_contents }

  access { optval_contents }
  modify { IOHandle }
}

procedure getsockopt(s, level, optname, optval, optlen)
{
  on_entry { s --> IOHandle
	     optval --> optval_contents }

  access { IOHandle }
  modify { optval_contents }
}

procedure getsockname(s, addr_ptr, add_len_ptr)
{
  on_entry { s --> IOHandle 
             addr_ptr --> addr { // --- INET addresses
                                 sin_family,
			         sin_port,
			         sin_addr { s_addr },
				 // --- UNIX addresses
				 sun_family,
				 sun_path --> path
			       }
	     add_len_ptr --> add_len
	   }

  access { IOHandle }

  modify { addr.sin_family,
	   addr.sin_port,
	   addr.sin_addr.s_addr,
	   addr.sun_family,
	   path,
	   add_len
	 }
}

procedure send(s, msg, len, flags)
{
  on_entry {
    s --> IOHandle
    msg --> msg_buffer
  }

  access { msg_buffer }
  modify { IOHandle, Disk }

  SHOW_WRITE(msg_buffer, IOHandle)

  ERROR_FTP_BEHAVIOR(msg_buffer, IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, s)

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost Connected)
    "ERROR at " ++ @context ++ ": Socket " ++ [ s ] ++ " may not be connected.\n";
#endif
}

procedure sendto(s, msg, len, flags, to, tolen)
{
  on_entry {
    s --> IOHandle
    msg --> msg_buffer
    to --> struct_sockaddr
  }

  access { msg_buffer, struct_sockaddr }
  modify { IOHandle, Disk }

  SHOW_WRITE(msg_buffer, IOHandle)

  ERROR_FTP_BEHAVIOR(msg_buffer, IOHandle)
}

procedure sendmsg(s, msg, flags)
{
  on_entry {
    s --> IOHandle
    msg --> struct_msghdr {
      msg_name --> msg_name_contents     /* optional address */
      msg_namelen                        /* size of address */
      msg_iov --> iovec { iov_base --> data, iov_len }       /* scatter/gather array */
      msg_iovlen                         /* # elements in msg_iov */
      msg_control --> msg_control_contents  /* ancillary data, see below */
      msg_controllen                     /* ancillary data buffer len */
      msg_flags                          /* flags on received message */
    }
  }

  access { msg_name_contents, struct_msghdr.msg_namelen,
	     iovec.iov_base, iovec.iov_len,
	     struct_msghdr.msg_iovlen, 
	     msg_control_contents, struct_msghdr.msg_controllen,
	     struct_msghdr.msg_flags,
	     data
	 }

  modify { IOHandle, Disk }

  SHOW_WRITE(data, IOHandle)

  ERROR_FTP_BEHAVIOR(data, IOHandle)
}


procedure recv(s, buf, len, flags)
{
  on_entry { s --> IOHandle
             buf --> buffer }

  modify { buffer, IOHandle, Disk }

  analyze FDKind { buffer <- IOHandle }
  analyze Trust  { buffer <- IOHandle }

  SHOW_READ(IOHandle)

  ERROR_FILE_NOT_OPEN(IOHandle, s)

#ifdef SHOW_ERRORS

  report if ( ! SocketState : IOHandle is-atmost Connected)
    "ERROR at " ++ @context ++ ": Socket " ++ [ s ] ++ " may not be connected.\n";
#endif
}

procedure recvfrom(s, buf,  len, flags, from, fromlen)
{
  on_entry { s --> IOHandle
             buf --> buffer
             from --> struct_sockaddr }

  access { struct_sockaddr }
  modify { buffer, IOHandle, Disk }

  analyze FDKind { buffer <- IOHandle }
  analyze Trust  { buffer <- IOHandle }

  SHOW_READ(IOHandle)
}

procedure select(n, readfds, writefds, exceptfds, timeout)
{
  on_entry { readfds --> read_fdset
	     writefds --> write_fdset
	     exceptfds --> except_fdset }

  access { read_fdset, write_fdset, except_fdset }
  modify { read_fdset, write_fdset, except_fdset }
}

// ------------------------------------------------------------
//  Locking
// ------------------------------------------------------------

%{

#include <sys/file.h>

}%

procedure flock(fd, operation)
{
  on_entry { fd --> IOHandle }
  modify { IOHandle }
}

// ------------------------------------------------------------
//  String conversions
// ------------------------------------------------------------

procedure atoi(string)
{
  on_entry { string --> string_contents }
  access { string_contents }
}

procedure atof(string)
{
  on_entry { string --> string_contents }
  access { string_contents }
}

procedure atol(string)
{
  on_entry { string --> string_contents }
  access { string_contents }
}

procedure strtod(string, end_ptr)
{
  on_entry {
    string --> string_contents
    end_ptr --> end
  }

  access { string_contents }

  on_exit { end --> string_contents }
}

procedure strtol(string, end_ptr, base)
{
  on_entry {
    string --> string_contents
    end_ptr --> end
  }

  access { string_contents }

  on_exit { end --> string_contents }
}

procedure strtoul(string, end_ptr, base)
{
  on_entry {
    string --> string_contents
    end_ptr --> end
  }

  access { string_contents }

  on_exit { end --> string_contents }
}

// ----------------------------------------------------------------------
//  Network
// ----------------------------------------------------------------------

%{

#include <netinet/in.h>
#include <arpa/inet.h>

}%

procedure htonl(hostlong)
{}

procedure  htons(hostshort)
{}

procedure  ntohl(netlong)
{}

procedure ntohs(netshort)
{}

procedure inet_aton(cp, inp)
{
  on_entry { cp --> cp_string
	     inp --> in_addr { s_addr }
	   }
  access { cp_string }
  modify { in_addr.s_addr }
}

procedure inet_addr(cp)
{
  on_entry { cp --> cp_string }
  access { cp_string }

  // report "ERROR at " ++ @context ++ ": function is obsolete -- use inet_aton().\n";
}

procedure inet_network(cp)
{
  on_entry { cp --> cp_string }
  access { cp_string }
}

global { inet_ntoa_buffer }

procedure inet_ntoa(in)
{
  on_entry { in { s_addr }}
  access { in.s_addr }
  modify { inet_ntoa_buffer }
  on_exit { return --> inet_ntoa_buffer }
}

procedure inet_makeaddr(net, host)
{
  
}

procedure inet_lnaof(in)
{

}

procedure inet_netof(in)
{

}


// ----------------------------------------------------------------------
//  Algorithms
// ----------------------------------------------------------------------

procedure qsort(base, num, size, compare)
{
  on_entry { base --> base_elements }

  access { base_elements }
}

// ----------------------------------------------------------------------
//  Processes
// ----------------------------------------------------------------------

procedure popen(command, type)
{
  on_entry { command --> command_string
             type --> type_string  }
  access { command_string, type_string }
  modify { Disk }
  on_exit { return --> new file_structure --> new IOHandle }

  analyze FDKind { IOHandle <- File }

  analyze FDState { IOHandle <- Open }

  analyze Trust {
    if (command_string is-atmost LocallyTrusted) IOHandle <- command_string
    default IOHandle <- LocallyTrusted
  }

  WARNING_UNTRUSTED_SOURCE(command, command_string)
}

procedure pclose(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { Disk }
  on_exit { delete IOHandle
	    delete file_structure
	  }

  analyze FDState { IOHandle <- Closed }
  analyze SocketState { IOHandle <- Unused }
}

// ----------------------------------------------------------------------
//  Crypt
// ----------------------------------------------------------------------

%{

#include <crypt.h>

}%

global { static_crypt_buffer }

procedure crypt(key, salt)
{
  on_entry { key --> key_data
	     salt --> salt_data }

  access { key_data, salt_data }
  modify { static_crypt_buffer }

  on_exit { return --> static_crypt_buffer }
}

// ----------------------------------------------------------------------
//  Name resolution
// ----------------------------------------------------------------------

%{

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

}%

global { static_res_struct }

procedure res_init()
{
  access { Disk }
}

procedure res_query(dname, class, type,
		    answer, anslen)
{
}

procedure res_search(dname, class, type,
		    answer, anslen)
{

}

procedure res_querydomain(name, domain, class, type,
			  answer, anslen)
{

}

procedure res_mkquery(op, dname, class, type,
		      data, datalen,
		      newrr, buf, buflen)
{

}

procedure res_send(msg, msglen,
		   answer, anslen)
{

}

procedure  dn_comp(exp_dn, comp_dn,
              length, dnptrs, exp_dn,
              lastdnptr)
{
}

procedure dn_expand(msg, eomorig,
              comp_dn, exp_dn,
              length)
{
}

// ------------------------------------------------------------
//  Stream Buffering
// ------------------------------------------------------------

procedure setbuf(stream, buf)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}
  
procedure setbuffer(stream, buf, size)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

procedure setlinebuf(stream)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

procedure setvbuf(stream, buf, mode, size)
{
  on_entry { stream --> file_structure --> IOHandle }
  modify { IOHandle, Disk }
}

