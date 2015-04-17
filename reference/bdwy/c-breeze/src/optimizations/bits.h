// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF TEXAS AT
//  AUSTIN BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  We acknowledge the C-to-C Translator from MIT Laboratory for
//  Computer Science for inspiring parts of the C-Breeze design.
//
// ----------------------------------------------------------------------

#ifndef CBZ_BITS_H
#define CBZ_BITS_H

#define BITS_PER_INT (8*sizeof(int))

class Bits {
private:
	unsigned int *ints;
	int	n, nints;

public:
	void reset_all (void) {
		for (int i=0; i<nints; i++) ints[i] = 0;
	}

	Bits (int nn) {
		n = nn;
		nints = nn / BITS_PER_INT;
		if (nn % BITS_PER_INT) nints++;
		ints = new unsigned int[nints];
		reset_all ();
	}

	~Bits (void) {
		delete [] ints;
	}

	void set (int pos, bool val) {
		int i = pos / BITS_PER_INT;
		int j = pos % BITS_PER_INT;
		if (val) {
			ints[i] |= (1<<j);
		} else {
			ints[i] &= ~(1<<j);
		}
	}

	void set (int pos) {
		set (pos, true);
	}

	void copy (Bits *b) {
		for (int i=0; i<nints; i++) ints[i] = b->ints[i];
	}

	void reset (int pos) {
		set (pos, false);
	}

	bool value (int pos) {
		int i = pos / BITS_PER_INT;
		int j = pos % BITS_PER_INT;
		if (ints[i] & (1<<j)) 
			return true;
		return false;
	}

	void And (Bits *other) {
		for (int i=0; i<nints; i++) 
			ints[i] &= other->ints[i];
	}

	void Or (Bits *other) {
		for (int i=0; i<nints; i++) 
			ints[i] |= other->ints[i];
	}

	void Not (void) {
		for (int i=0; i<nints; i++)
			ints[i] = ~ints[i];
	}

	Bits *clone (void) {
		Bits *b = new Bits (n);
		for (int i=0; i<nints; i++) b->ints[i] = ints[i];
		return b;
	}
};

#endif // CBZ_BITS_H
