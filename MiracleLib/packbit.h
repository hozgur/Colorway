// packbit.h : Header File Packbit Compression & Decompression Algorithms by Hakan OZGUR 27/02/2001
//


MBASEAPI long MIRACLEEXPORT packbitcompress(unsigned char *dest,long dwidth,const unsigned char*source,long width);
//long calcpackbitcompress(const char*source,long width);
MBASEAPI void MIRACLEEXPORT packbitdecompress(char *dest,unsigned long swidth,const char *source,unsigned long sourcewidth);
