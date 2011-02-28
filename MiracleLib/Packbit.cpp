// packbit.cpp : Packbit Compression & Decompression Algorithms by Hakan OZGUR 27/02/2001
//

#include "stdafx.h"
#include <memory.h>
#include <conio.h>
#include "miracleobj.h"
typedef unsigned char byte;

#define MAXLEN 128

MBASEAPI long MIRACLEEXPORT packbitcompress(unsigned char *dest,long dwidth,const unsigned char*source,long width)
{
	unsigned char* bp = (unsigned char*) source;
	unsigned char *op, *ep, *lastliteral;
	long n, slop;
	int b;
	enum { BASE, LITERAL, RUN, LITERAL_RUN } state;
	int tifrawcc =0;
	op = dest;
	ep = op+dwidth;
	state = BASE;
	lastliteral = 0;
	int cc = width;
	while (cc > 0) {
		/*
		 * Find the longest string of identical bytes.
		 */
		b = *bp++, cc--, n = 1;
		for (; cc > 0 && b == *bp; cc--, bp++)
			n++;
	again:
		if (op + 2 >= ep) {		/* insure space for new data */
			/*
			 * Be careful about writing the last
			 * literal.  Must write up to that point
			 * and then copy the remainder to the
			 * front of the buffer.
			 */
			if (state == LITERAL || state == LITERAL_RUN) {
				slop = op - lastliteral;
				tifrawcc += lastliteral - dest;				
				op = dest;
				while (slop-- > 0)
					*op++ = *lastliteral++;
				lastliteral = dest;
			} else {
				tifrawcc += op - dest;				
				op = dest;
			}
		}
		switch (state) {
		case BASE:		/* initial state, set run/literal */
			if (n > 1) {
				state = RUN;
				if (n > 128) {
					*op++ =  -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (-(n-1));
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL:		/* last object was literal string */
			if (n > 1) {
				state = LITERAL_RUN;
				if (n > 128) {
					*op++ =  -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (-(n-1));	/* encode run */
				*op++ = b;
			} else {			/* extend literal */
				if (++(*lastliteral) == 127)
					state = BASE;
				*op++ = b;
			}
			break;
		case RUN:		/* last object was run */
			if (n > 1) {
				if (n > 128) {
					*op++ =  -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (-(n-1));
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL_RUN:	/* literal followed by a run */
			/*
			 * Check to see if previous run should
			 * be converted to a literal, in which
			 * case we convert literal-run-literal
			 * to a single literal.
			 */
			if (n == 1 && op[-2] ==  -1 &&
			    *lastliteral < 126) {
				state = (((*lastliteral) += 2) == 127 ?
				    BASE : LITERAL);
				op[-2] = op[-1];	/* replicate */
			} else
				state = RUN;
			goto again;
		}
	}
	return op-dest;
}
/*
long packbitcompress(char *dest,long dwidth,const char*source,long width)
{
	int spos,dpos,tpos;
	int n;
	int count,val;
	spos = dpos = 0;
	int rawcount,repcount,rawcount2;
	bool reread = true;
	
	while (spos<width)
	{
		n = width-spos; if (n>MAXLEN) n = MAXLEN; tpos = spos;
		if (n!=0)
		{
			if (reread)
			{
				count = tpos;
				val = source[count++];
				while (val != source[count])
				{
					if (count-tpos == n) break;
					val = source[count++];
		
				}
				if (count-tpos == n) rawcount = count-tpos; else rawcount=count-tpos-1;				
				reread=false;
			}
			else
			{
				rawcount = rawcount2;
				reread=false;
			}
		}
		tpos += rawcount; n = width-tpos; if (n>MAXLEN) n = MAXLEN;

		count = tpos;
		val = source[count++];

		while (val == source[count])
		{
			if (count-tpos == n) break;
			val = source[count++];
		
		}

		if (count-tpos ==1) repcount = 0; else repcount = count-tpos;

		tpos +=repcount; n = width-tpos; if (n>MAXLEN) n = MAXLEN;

		count = tpos;
		val = source[count++];

		while (val != source[count])
		{
			if (count-tpos == n) break;
			val = source[count++];
		}
		if (count-tpos == n) rawcount2 = count-tpos; else rawcount2=count-tpos-1;				

		if ((repcount == 2) && ((rawcount != 0) || (rawcount2 !=0) ))
		{
			if ((rawcount+rawcount2) <MAXLEN-2)
			{
				rawcount = rawcount+rawcount2+2;
				repcount=0;				
				reread = true;
				tpos +=rawcount2;
			}
			else
				if (rawcount < MAXLEN-2)
				{
					rawcount+=2;
					repcount=0;
				}
			
		}
	
		if (rawcount!=0)
		{
			
			
			dest[dpos++] = rawcount-1;
			
			if(rawcount == 1)
			{
				dest[dpos] = source[spos];				
			}
			else
			{
				if((dpos + rawcount) >dwidth)
				{
					Debug.Format("Rawcount Error = %d",dpos+rawcount+1-dwidth);
					MObj.DebugMessage(Debug);				
					rawcount = dwidth-dpos;
				}
				memcpy(dest+dpos,source+spos,rawcount);
			}
						
			dpos+= rawcount;
		}
		if (repcount!=0)
		{
			if((dpos + 2) >dwidth)
			{
				Debug.Format("Repcount Error = %d",dpos+2-dwidth);
				MObj.DebugMessage(Debug);
				return dpos;
			}
			dest[dpos] = -(repcount-1);
			dest[dpos+1] = source[spos+rawcount];
			dpos+=2;
		}
		spos = tpos;		
	}


	return dpos;
}


long calcpackbitcompress(const char *source,long width)
{
	long spos,dpos,tpos;
	int n;
	int count,val;
	spos = dpos = 0;
	int rawcount,repcount,rawcount2;
	bool reread = true;

	while (spos<width)
	{
		n = width-spos; if (n>MAXLEN) n = MAXLEN; tpos = spos;
		if (n!=0)
		{
			if (reread)
			{
				count = tpos;
				val = source[count++];
				while (val != source[count])
				{
					if (count-tpos == n) break;
					val = source[count++];
		
				}
				if (count-tpos == n) rawcount = count-tpos; else rawcount=count-tpos-1;				
				reread=false;
			}
			else
			{
				rawcount = rawcount2;
				reread=false;
			}
		}

		tpos += rawcount; n = width-tpos; if (n>MAXLEN) n = MAXLEN;

		count = tpos;
		val = source[count++];

		while (val == source[count])
		{
			if (count-tpos == n) break;
			val = source[count++];
		
		}

		if (count-tpos ==1) repcount = 0; else repcount = count-tpos;

		tpos +=repcount; n = width-tpos; if (n>MAXLEN) n = MAXLEN;

		count = tpos;
		val = source[count++];

		while (val != source[count])
		{
			if (count-tpos == n) break;
			val = source[count++];
		}
		if (count-tpos == n) rawcount2 = count-tpos; else rawcount2=count-tpos-1;				

		if ((repcount == 2) && ((rawcount != 0) || (rawcount2 !=0) ))
		{
			if ((rawcount+rawcount2) <MAXLEN-2)
			{
				rawcount = rawcount+rawcount2+2;
				repcount=0;				
				reread = true;
				tpos +=rawcount2;
			}
			else
				if (rawcount < MAXLEN-2)
				{
					rawcount+=2;
					repcount=0;
				}
			
		}
		if (rawcount!=0)
		{			
			dpos+= rawcount +1;
		}
		if (repcount!=0)
		{			
			dpos+=2;
		}
		spos = tpos;		
	}

	return dpos;
}
*/
MBASEAPI void MIRACLEEXPORT packbitdecompress(char *dest,unsigned long swidth,const char *source,unsigned long sourcewidth)
{
	unsigned long pos = 0;	// Source Position
	short len;	
	unsigned long dp = 0;	// Destination Position
	while (pos<sourcewidth)
	{
		len = source[pos++];
		if (len <0)
		{
			len = -len;			
			len++;
			ASSERT(dp+len <=swidth);
			memset(dest+dp,source[pos++],len);
			dp+=len;			
		}
		else
		{		
			len++;			
			ASSERT(dp+len <=swidth);
			memcpy(dest+dp,source+pos,len);			
			dp+=len;
			pos+=len;			
		}	
	}	
}
