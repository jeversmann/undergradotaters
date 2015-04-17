// $Id: cbztimer.h,v 1.4 2003/08/07 23:14:01 pnav Exp $
  /*
    High-resolution timer code for Linux.

    Copyright (C) 2002 by Emery Berger.
    Copyright (C) 2002 by The University of Massachusetts.
    All rights reserved.
  */

#ifndef CBZ_TIMER_H
#define CBZ_TIMER_H

#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class cbzTimer
{
 public:

  static void getTime (unsigned long& tlo, unsigned long& thi) {
#ifdef __linux__
    asm volatile ("rdtsc"
		  : "=a"(tlo),
		  "=d" (thi));
#endif
  }

  
  static double getFrequency (void) {
    static double freq = 0.0;
#ifdef __linux__
    unsigned long LTime0, LTime1, HTime0, HTime1;
    if (freq == 0.0) {

      // Parse out the CPU frequency from the /proc/cpuinfo file.

      enum { MAX_PROCFILE_SIZE = 32768 };
      const char searchStr[] = "cpu MHz		: ";
      static char line[MAX_PROCFILE_SIZE];
      int fd = open ("/proc/cpuinfo", O_RDONLY);
      read (fd, line, MAX_PROCFILE_SIZE);
      char * pos = strstr (line, searchStr);
      if (pos == NULL) {

	// Compute MHz directly.
	// Wait for approximately one second.

	getTime (LTime0, HTime0);
	sleep (1);
	getTime (LTime1, HTime1);

	freq = (double)(LTime1 - LTime0) +
	  (double)(UINT_MAX)*(double)(HTime1 - HTime0);
	if(LTime1 < LTime0) { freq -= (double)UINT_MAX; }

	cout << "THIS IS BAD" << endl;

      } else {
	// Move the pointer over to the MHz number itself.
	pos += strlen(searchStr);
	float f;
	sscanf (pos, "%f", &f);
	freq = ((double) f) * 1000000.0;
      }
    }
#endif
    return freq;
  }


  cbzTimer (void)
    : timeElapsed (0.0),
      currentLo(0),
      currentHi(0)
  {}

  void start (void) {
    getTime (currentLo, currentHi);
  }

  void stop (void) {
    unsigned long lo, hi;
    getTime (lo, hi);
    double d = (double) (lo - currentLo) + (double) (UINT_MAX)*(double)(hi - currentHi);
    if (lo < currentLo) {
      d -= (double) UINT_MAX;
    }
    timeElapsed += d;
  }

  operator double (void) {
    static double freq = getFrequency ();
    return timeElapsed / freq;
  }

  double time() {
    static double freq = getFrequency ();
    return timeElapsed / freq;
  }
  
private:
  double timeElapsed;
  unsigned long currentLo, currentHi;
};

#endif
