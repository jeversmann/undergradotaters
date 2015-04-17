#!/lusr/bin/perl -w
# $Id: add_copyright.pl,v 1.1 2003/08/07 23:01:26 pnav Exp $
#
# adds copyright statement to missing files
# a file with the filenames to be processed should be given as input
# one can be generated with a shell command similar to this:
#        find . -name "*.h" -exec grep -Li 'Copyright' {} \; > files
#        find . -name "*.cc" -exec grep -Li 'Copyright' {} \; >> files

while (<>) {

    chomp $_;
    `mv $_ $_.pnav`;
    $OUTFILE = ">$_";

    open(OUTFILE);

    print OUTFILE "// ----------------------------------------------------------------------\n";
    print OUTFILE "//\n";
    print OUTFILE "//  C-Breeze\n";
    print OUTFILE "//  C Compiler Framework\n";
    print OUTFILE "// \n";
    print OUTFILE "//  Copyright (c) 2003 University of Texas at Austin\n";
    print OUTFILE "// \n";
    print OUTFILE "//  Samuel Z. Guyer\n";
    print OUTFILE "//  Adam Brown\n";
    print OUTFILE "//  Teck Bok Tok\n";
    print OUTFILE "//  Paul Arthur Navratil\n";
    print OUTFILE "//  Calvin Lin\n";
    print OUTFILE "// \n";
    print OUTFILE "//  Permission is hereby granted, free of charge, to any person\n";
    print OUTFILE "//  obtaining a copy of this software and associated documentation\n";
    print OUTFILE "//  files (the \"Software\"), to deal in the Software without\n";
    print OUTFILE "//  restriction, including without limitation the rights to use, copy,\n";
    print OUTFILE "//  modify, merge, publish, distribute, sublicense, and/or sell copies\n";
    print OUTFILE "//  of the Software, and to permit persons to whom the Software is\n";
    print OUTFILE "//  furnished to do so, subject to the following conditions:\n";
    print OUTFILE "//  \n";
    print OUTFILE "//  The above copyright notice and this permission notice shall be\n";
    print OUTFILE "//  included in all copies or substantial portions of the Software.\n";
    print OUTFILE "//  \n";
    print OUTFILE "//  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n";
    print OUTFILE "//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n";
    print OUTFILE "//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n";
    print OUTFILE "//  NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF TEXAS AT\n";
    print OUTFILE "//  AUSTIN BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER\n";
    print OUTFILE "//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF\n";
    print OUTFILE "//  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n";
    print OUTFILE "//  THE SOFTWARE.\n";
    print OUTFILE "//\n";
    print OUTFILE "//  We acknowledge the C-to-C Translator from MIT Laboratory for\n";
    print OUTFILE "//  Computer Science for inspiring parts of the C-Breeze design.\n";
    print OUTFILE "//\n";
    print OUTFILE "// ----------------------------------------------------------------------\n";

    close( OUTFILE );

    `cat $_.pnav >$OUTFILE`;
    `rm -f $_.pnav`
}
