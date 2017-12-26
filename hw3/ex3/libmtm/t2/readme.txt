If you're using Linux, and are getting an error similar to the following:

/usr/bin/ld: ./libmtm.a(list.o): relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC

then try using the "libmtm_PIC.a" file, which was created by using the "-fPIC" flag.
In any other case, use the "libmtm.a" file.
