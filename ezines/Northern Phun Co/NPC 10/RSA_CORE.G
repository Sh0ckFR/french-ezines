Newsgroups: alt.sources
From: paasivir@jyu.fi (Risto Paasivirta)
Subject: mrsa, RSA cryptosystem core
Message-ID: <1993Feb20.193538.25392@jyu.fi>
Date: Sat, 20 Feb 1993 19:35:38 GMT

No warranty but I hope someone finds this useful.

	Risto

#!/bin/sh
# This is a shell archive (produced by shar 3.49)
# To extract the files from this archive, save it to a file, remove
# everything above the "!/bin/sh" line above, and type "sh file_name".
#
# made 02/20/1993 19:16 UTC by paasivir@tukki
# Source directory /home/tukki/opis/paasivir/crypt/rsa
#
# existing files will NOT be overwritten unless -c is specified
#
# This shar contains:
# length  mode       name
# ------ ---------- ------------------------------------------
#    289 -rw-r--r-- Makefile
#    454 -rw-r--r-- Makefile.amiga
#   7030 -rw-r--r-- README.mrsa
#  10725 -rw-r--r-- mrsa.c
#   2250 -rw-r--r-- mrsa.h
#   4890 -rw-r--r-- pmm68k.s
#    919 -rw-r--r-- rsatest.c
#
# ============= Makefile ==============
if test -f 'Makefile' -a X"$1" != X"-c"; then
	echo 'x - skipping Makefile (File already exists)'
else
echo 'x - extracting Makefile (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'Makefile' &&
#
# Makefile for gnu-c
#
X
CC= gcc
CFLAGS=  -O
X
OBJ=	rsatest.o mrsa.o
DOBJ=	destest.o mdes.o
X
all: rsatest
X	rsatest 6c2f3ab193fe851da0af3f1bed53c2f57 \
X		6e36cce2f76eda422742ecc4ed9a213
X
rsatest: $(OBJ)
X	$(CC) -o rsatest $(CFLAGS) $(OBJ)
X
rsatest.o: rsatest.c mrsa.h
mrsa.o: mrsa.c mrsa.h
X
SHAR_EOF
chmod 0644 Makefile ||
echo 'restore of Makefile failed'
Wc_c="`wc -c < 'Makefile'`"
test 289 -eq "$Wc_c" ||
	echo 'Makefile: original size 289, current size' "$Wc_c"
fi
# ============= Makefile.amiga ==============
if test -f 'Makefile.amiga' -a X"$1" != X"-c"; then
	echo 'x - skipping Makefile.amiga (File already exists)'
else
echo 'x - extracting Makefile.amiga (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'Makefile.amiga' &&
#
# Makefile for SAS/C 6.2 and pdmake
#
X
CC= sc
CFLAGS=  NOSTKCHK UNSCHAR NOWVRET STRMERGE OPTIMIZE PARMS=register \
X	ANSI DEFINE=_STRICT_ANSI DEFINE=AMIGA
# SAS asm is too weird for me
AS= 	a68k
AFLAGS=
LNK=	sc link $(CFLAGS)
X
.suffixes: .s .c .o .h .i
X
.c.o:
X	$(CC) $(CFLAGS) $*.c 
X
.s.o:
X	$(AS) $*.s 
X
rsatest: rsatest.o mrsa.o pmm68k.o
X	$(LNK) rsatest.o mrsa.o pmm68k.o TO rsatest
X
rsatest.o: rsatest.c mrsa.h
mrsa.o: mrsa.c mrsa.h
pmm68k:	pmm68k.s
X
SHAR_EOF
chmod 0644 Makefile.amiga ||
echo 'restore of Makefile.amiga failed'
Wc_c="`wc -c < 'Makefile.amiga'`"
test 454 -eq "$Wc_c" ||
	echo 'Makefile.amiga: original size 454, current size' "$Wc_c"
fi
# ============= README.mrsa ==============
if test -f 'README.mrsa' -a X"$1" != X"-c"; then
	echo 'x - skipping README.mrsa (File already exists)'
else
echo 'x - extracting README.mrsa (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'README.mrsa' &&
X
Files mrsa.c, mrsa.h, rsatest.c, pmm68k.s
X
X	Portable(?) math library and low level RSA code.
X
X	Public Domain, NO WARRANTY.
X
X	Note:
X
X	This code may be subject of U.S. Patent no 4,405,829. Read 'Frequently
X	Asked Questions Of Todays Cryptography', chapter 2.22 'Is RSA Patented'
X	for more info (ftp to rsa.com.) Use 'non-commercially for a personal,
X	academic or intellectual reason' and consult Public Key Partners of
X	Sunnyvale, California about other use in USA/Canada. RSA is not paten-
X	ted outside North America, and my implementation is free for any use.
X
X	This code may or may not be subject of silly US export controls. Ask
X	'State Department's Office of Defense Trade Controls' if in doubt...
X
Version 1.1
X
X	First version I posted to alt.sources (file mtest.c) was far too
X	slow for any real use, this one should be fast enough for some 
X	applications, expecially with assembler modmul (pmm(). Key generation
X	is still slow. Some testing needed.
X
X	I have tested this only with big-endian machines, but library
X	_should_ be portable to anything with ANSI-C compiler, 8-bit
X	and 8/16/32-bit unsigned types.
X
Author
X
X	Risto Paasivirta, paasivir@jyu.fi
X
==============================================================================
X
mrsa.h
X
X		typedef unsigned short UWORD;
X		typedef unsigned long ULONG;
X
X	Unsigned 16-bit unit type and 32-bit double unit type. For other
X	unit sizes, some hacking is required.
X
X		#define NSIZE xx
X
X	NSIZE defines max precision in UWORDs. For 16-bit UWORDS, xx should
X	be 33 for 512-bit precision, or 65 for max 1024-bit precision. Use
X	lowest possible precision for required application.
X
X		typedef UWORD *N, NN[NSIZE];
X
X	Multiprecison value pointer and storage type.
X
X		typedef struct rsa_key {
X			ULONG	b;	/* bits in modulus */
X			NN	pq;	/* public modulus, p * q */
X			NN	e;	/* public exponent */
X			NN	d;	/* private exponent */
X			NN	p;	/* secret prime 1 */
X			NN	q;	/* secret prime 2 */
X			NN	dp;	/* exponent 1, d mod p */
X			NN	dq;	/* exponent 2, d mod q */
X			NN	qp;	/* coefficient, q^{-1} mod p */
X		} rsa_key;
X
X	RSA key type. For encryption only, you need to initialize only
X	b, pq and e, for decryption you must initialize all values.
X
mrsa.c
X
X	Math library
X
X	For math library docs read the source. If you don't care,
X	define RSA_ONLY while compiling mrsa.c to get only RSA
X	functions extern. Error checking is minimal, if you give
X	garbage in, you'll get garbage out.
X
X
X	RSA library, prime generation and RSA functions
X
X		void	randomize(N n, ULONG bits);
X
X	Randomize n with pseudorandom generator and limit value to range 
X	2^bits to 2(bits-1)-1. For example If you need a strong random value 
X	n of 175 bits you first initialize a (175+7)/8 = 22-byte buffer with 
X	strong random bytes, convert random bytes to value n with b_to_n() and 
X	call randomize():
X	int i;
X	NN n;
X	unsigned char buf[sizeof(NN)];
X	...
X	for(i=0;i<22;i++)
X		buf[i] = your_strong_random_gen();
X	b_to_n(n, buf, 22);
X	randomize(n, buf, 175);
X	...
X
X		ULONG divisor = sieve_prime(N n);
X
X	Try to divide n with first 54 primes. Returns divisor if found,
X	0 if not. This routine is slower than it should because of
X	unoptimized and pessimal divide routine. 
X
X		int flag = prob_prime(N n);
X
X	Test if 2^(n-1) mod n == 1. Returns 0 if test failed, !0 if success.
X	Large n which passes this test is a probable prime. This test does
X	not work well with small value of n. Because this test is slow, you 
X	should first try sieving n. This routine is slightly optimized and
X	faster than using modexp (me()) directly.
X
X		void	next_prime(N n);
X
X	Find next prime >= n by sieve and probable prime test. This function
X	is slow and writes 'thinking indicator' to stdout. If you don't
X	need indicator feature, define THINK_SILENTLY when compiling
X	mrsa.c
X
X		ULONG bits = rsa_gen(rsa_key *);
X
X	Generate a RSA key from key->p and key->q. Initialize key->p and 
X	key->q either with primes or strong random integers of apporopriate
X	size (see randomize()) Returns number of bits in modulus key->pq
X	or 0 if key generation failed. If key generation fails, you may
X	recycle key->p and key->q by using randomize with same bit sizes.
X	Some testing for weak keys might be nice, possibly an encrypt/decrypt
X	test too.
X
X		void 	rsa_dec(N m,rsa_key *key);
X
X	RSA decrypt message m with private (fully initialized) key. Result
X	undefined (ie. wrong) if key is not private rsa key. Convert message
X	to/from internal representation with b_to_n() and n_to_b().
X	This routine uses chinese remainder theorem shortcut. You can decrypt
X	m with using only key->pq and key->d by using modexp function:
X	...
X	me(m, key->d, key->pq);
X	...
X	but that will be significantly slower. m should be < key->pq.
X
X		void 	rsa_enc(N m ,rsa_key *key);
X
X	RSA encrypt message m with public (at least key->p, key->q and key->e
X	initialized) key. Convert message to/from internal representation with
X	b_to_n() and n_to_b(). m should be < key->pq.
X
X		ULONG len = n_to_b(unsigned char *buf ,N n);
X
X	Convert value n from internal representation to bytes, most significant
X	byte first. Returns number of bytes written to buf. buf should be large
X	enough to hold sizeof(NN) bytes. Note that number is handled as 
X	unsigned, negative value converts to 2-complement representation of
X	sizeof(NN) bytes. For positive values, smallest number of significant
X	bytes is written (ie. 1 byte for value 0, 1 for 255, 2 bytes for 256,
X	etc.)
X
X		void 	b_to_n(N n ,unsigned char *buf,ULONG len);
X
X	Convert len bytes from buf to value n. Conversion is unsigned, most
X	significant byte first. Note: If len == sizeof(NN) and most significant
X	byte is >127, resulting value will be negative but if msb >127 and
X	len < sizeof(NN), positive...
X
rsatest.c
X
X	Example output from RSA test compiled with NSIZE = 16
X
X		rsatest 6c2f3ab193fe851da0af3f1bed53c2f57 \
X			6e36cce2f76eda422742ecc4ed9a213
X	Generating key...
X	Done, key components: pq,e,d,p,q,dp,dq,qp
X	bits = 254
X	2e9373c43c1451bf02e85011709d22961dce9cdcac504f060589838286f55da3
X	0000000000000000000000000000000000000000000000000000000000010001
X	0046ae7e80ceac0762588734fb426b34874e65d2e0581b7bf62b887f27ee6451
X	00000000000000000000000000000006c2f3ab193fe851da0af3f1bed53c2fb3
X	0000000000000000000000000000000006e36cce2f76eda422742ecc4ed9a251
X	00000000000000000000000000000004861b11b6eb133cf06db39962dcb85011
X	0000000000000000000000000000000004201cbff7a95075e6b4927a8616cda1
X	0000000000000000000000000000000449fdacb190021839485e90519f2bc9c9
X	testing, msg,cip,dec1,dec2
X	1e7e1f9b657ce171170a9917daa8d40d0bd6d3d3ca14ede9d8e2ebcf7fc06b05
X	18523970d34c42ad472ca6b2ce35f6dacd42c7624d765a700c7a954cb3616f2c
X	1e7e1f9b657ce171170a9917daa8d40d0bd6d3d3ca14ede9d8e2ebcf7fc06b05
X	1e7e1f9b657ce171170a9917daa8d40d0bd6d3d3ca14ede9d8e2ebcf7fc06b05
X
pmm68k.s
X
X	This is Amiga-spesific 68000 assembler modular multiply function.
X	Quick kludge, I borrowed some code from totally different library
X	with different unit size and endianess... but it seems to work.
X	(If you would like to use greater precision than NSIZE 65, you
X	need to modify this file...)
X
(eof)
SHAR_EOF
chmod 0644 README.mrsa ||
echo 'restore of README.mrsa failed'
Wc_c="`wc -c < 'README.mrsa'`"
test 7030 -eq "$Wc_c" ||
	echo 'README.mrsa: original size 7030, current size' "$Wc_c"
fi
# ============= mrsa.c ==============
if test -f 'mrsa.c' -a X"$1" != X"-c"; then
	echo 'x - skipping mrsa.c (File already exists)'
else
echo 'x - extracting mrsa.c (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'mrsa.c' &&
/*
X * mrsa.c -- portable multiprecision math and RSA library
X * 1993 Risto Paasivirta, paasivir@jyu.fi
X * Public Domain, no warranty. 
X */
X
#define MRSA_C 1
X
#include "mrsa.h"
X
/*
X * math library stuff
X */
X
/*
X * sign = ts(N a) -- test signed, returns 1, 0 or -1 
X */
X
PRIVATE int
ts(N a)
{
X	ULONG i = NSIZE;
X	if (a[NSIZE - 1] & SIGN_BIT)
X		return -1;
X	while (i--)
X		if (*a++)
X			return 1;
X	return 0;
}
X
/*
X * carry = ng(N a) -- negate, returns carry
X */
X
PRIVATE ULONG
ng(N a)
{
X	ULONG c = 0, i = NSIZE;
X	while (i--) {
X		c = 0 - *a - c;
X		*a++ = c;
X		c = (c >> UNIT_BITS) & 1;
X	} return c;
}
X
/*
X * cl(N a) -- clear value, a = 0
X */
X
PRIVATE void
cl(N a)
{
X	ULONG i = 0;
X	while (i++ < NSIZE)
X		*a++ = 0;
}
X
/*
X * cp(N a, N b) -- copy, a = b
X */
X
PRIVATE void
cp(N a, N b)
{
X	ULONG i = NSIZE;
X	while (i--)
X		*a++ = *b++;
}
X
/*
X * flag = cu(a, b) -- compare unsigned, returns <0 if a<b, 0 if a==b, >0 if a>b
X */
X 
PRIVATE int
cu(N a, N b)
{
X	ULONG i = NSIZE;
X	a += NSIZE;
X	b += NSIZE;
X	while (i--)
X		if (*--a - *--b)
X			return (int) *a - (int) *b;
X	return 0;
}
X
/*
X * carry = ad(N a, N b) -- add, a += b
X */
X
PRIVATE ULONG
ad(N a, N b)
{
X	ULONG c = 0, i = NSIZE;
X	while (i--) {
X		c = *b++ + *a + c;
X		*a++ = c;
X		c >>= UNIT_BITS;
X	} 
X	return c;
}
X
/*
X * carry = sb(N a, N b) -- substract, a -= b
X */
X
PRIVATE ULONG
sb(N a, N b)
{
X	ULONG c = 0, i = NSIZE;
X	while (i--) {
X		c = *a - *b++ - c;
X		*a++ = c;
X		c = (c >> UNIT_BITS) & 1;
X	}
X	return c;
}
X
/*
X * carry = sr(N a) -- shift right, a >>= 1
X */
X
PRIVATE ULONG
sr(N a)
{
X	ULONG c = 0, i = NSIZE;
X	a += NSIZE;
X	while (i--) {
X		c |= *--a;
X		*a = c >> 1;
X		c = (c & 1) << UNIT_BITS;
X	}
X	return c;
}
X
/*
X * carry = sl(N a) -- shift left, a <<= 1
X */
X
PRIVATE ULONG
sl(N a)
{
X	ULONG c = 0, i = NSIZE;
X	while (i--) {
X		c |= (ULONG) * a << 1;
X		*a++ = c;
X		c = (c >> UNIT_BITS) & 1;
X	}
X	return c;
}
X
/*
X * dm(N a, N b, N c) -- divide-modulo unsigned, a = a / b, c = a % b
X */
X
PRIVATE void
dm(N a, N b, N c)
{
X	ULONG i = NSIZE * UNIT_BITS;
X	cl(c);
X	while (i--) {
X		sl(c);
X		*c |= sl(a);
X		if (sb(c, b)) {
X			ad(c, b);
X		} else {
X			*a |= 1;
X		}
X	}
}
X
/*
X * mu(N a, N b) -- multiply unsigned, a *= b
X */
X
PRIVATE void
mu(N a, N b)
{
X	ULONG i = NSIZE * UNIT_BITS;
X	NN c;
X	cl(c);
X	while (i--) {
X		sl(c);
X		if (sl(a))
X			ad(c, b);
X	}
X	cp(a, c);
}
X
/*
X * mm(N a, N b, N m) -- modular multiply, a = a * b mod m 
X */
X
PRIVATE void
mm(N a, N b, N m)
{
X	ULONG i = NSIZE * UNIT_BITS;
X	NN c;
X	cl(c);
X	while (i--) {
X		sl(c);
X		if (sb(c, m))
X			ad(c, m);
X		if (sl(a))
X			ad(c, b);
X		if (sb(c, m))
X			ad(c, m);
X	}
X	cp(a, c);
}
X
/*
X * pmm(N a, N b, N m, ULONG p) -- internal modmul w/precision for modexp
X */
X
#ifndef AMIGA
X
static void
pmm(N aa, N b, N m, ULONG p)
{
X	ULONG k, c, j = UNIT_BITS, i;
X	NN v;
X	N a;
X	i = p;
X	cl(v);
X	a = aa + p;
X	while (!*--a
X	       && i)
X		i--;
X	if (i) {
X		while (!(*a & (1 << j)) && j)
X			j--;
X		cp(v, b);
X	} while (i--) {
X		while (j--) {
X			for (k = 0, c = 0; k < p; k++) {
X				c |= (ULONG) v[k] << 1;
X				v
X					[k] = c;
X				c >>= UNIT_BITS;
X			} for (k = 0, c = 0; k < p; k++) {
X				c = v[k] - m[k] - c;
X				v[k] = c;
X				c = (c >> UNIT_BITS) & 1;
X			} if (c)
X				for (k = 0, c = 0; k < p; k++) {
X					c = v[k] + m[k] + c;
X					v[k] = c;
X					c >>= UNIT_BITS;
X			} if (*a & (1 << j)) {
X				for (k = 0, c = 0; k < p; k++) {
X					c = v[k] + b[k] + c;
X					v[k] = c;
X					c >>= UNIT_BITS;
X				} for (k = 0, c = 0; k < p; k++) {
X					c = v[k] - m[k] - c;
X					v[k] = c;
X					c = (c >> UNIT_BITS) & 1;
X				} if (c)
X					for (k = 0, c = 0; k < p; k++) {
X						c = v[k] + m[k] + c;
X						v[k] = c;
X						c >>= UNIT_BITS;
X					}
X			}
X		}
X		a--;
X		j = UNIT_BITS;
X	}
X	cp(aa, v);
}
X
#endif
X
/*
X * em(N a, N b, N m) -- modular exponentation, a = a^b mod n
X */
X
PRIVATE void
em(N a, N e, N m)
{
X	ULONG i = NSIZE, j = UNIT_BITS, p = NSIZE;
X	NN c;
X	N mp;
X	cl(c);
X	*c = 1;
X	e += NSIZE;
X	while (!*--e && i)
X		i--;
X	if (i) {
X		while (!(*e & (1 << j)))
X			j--;
X		cp(c, a);
X	}
X	mp = m + NSIZE;
X	while (!*--mp && p)
X		p--;
X	if (*mp & SIGN_BIT && p < NSIZE)
X		p++;
X	while (i--) {
X		while (j--) {
X			pmm(c, c, m, p);
X			if (*e & (1 << j))
X				pmm(c, a, m, p);
X		}
X		e--;
X		j = UNIT_BITS;
X	}
X	cp(a, c);
}
X
/*
X * gd(N a, N b) -- a = greatest common divisor(a,b)
X */
X
PRIVATE void
gd(N a, N bb)
{
X	NN r, b;
X	cp(b, bb);
X	while (ts(b)) {
X		dm(a, b, r);
X		cp(a, b);
X		cp(b, r);
X	}
}
X
/*
X * iv(N a, N b) -- multiplicative inverse, a = a^{-1} mod b 
X */
X
PRIVATE void
iv(N a, N b)
{
X	NN c, d, e, f, g, y;
X	cp(c, b);
X	cl(e);
X	cl(f);
X	*f = 1;
X	while (ts(a)) {
X		cp(y, c);
X		dm(y, a, d);
X		if (f[NSIZE - 1] & SIGN_BIT) {
X			ng(f);
X			mu(y, f);
X			ng(f);
X			ng(y);
X		} else
X			mu(y, f);
X		cp(g, e);
X		sb(g, y);
X		cp(c, a);
X		cp(a, d);
X		cp(e, f);
X		cp(f, g);
X	}
X	if (e[NSIZE - 1] & SIGN_BIT)
X		ad(e, b);
X	cp(a, e);
}
X
/*
X * nh(char *a, N b) -- convert value to a hex string (use for debugging)
X */
X
PRIVATE void
nh(char *a, N b)
{
X	char *d = "0123456789abcdef";
X	NN c;
X	ULONG i = NSIZE * sizeof(UWORD) * 2; /* 2 digits/byte! */
X	cp(c, b);
X	a += NSIZE * sizeof(UWORD) * 2;
X	*a = 0;
X	while (i--) {
X		*--a = d[*c & 15];
X		sr(c);
X		sr(c);
X		sr(c);
X		sr(c);
X	}
}
X
/*
X * hn(N a, char *b) -- lower-case hex string to value (use for constants)
X */
X
PRIVATE void
hn(N a, char *b)
{
X	cl(a);
X	while (*b) {
X		sl(a);
X		sl(a);
X		sl(a);
X		sl(a);
X		*a += *b < 'a' ? *b - '0' : *b - ('a' - 10);
X		b++;
X	}
}
X
/*
X * integer = ri() -- generate weak pseudorandom integer (range 0-65535)
X */
X
PRIVATE ULONG
ri(void)
{
X	static ULONG s = 27182;
X	s = (s * 31421 + 6927) & 0xffff;
X	return s;
}
X
/*
X * prime generation and RSA stuff
X */
X
/*
X * randomize(N n, ULONG bits) -- pseudorandomize n, limit to value to
X * range 2^bits to 2^(bits+1)-1. This function eors n with weak random
X * generator, n should be initialized with at least bits-bit strong random
X * value before call.
X * (XXX check portablility when converting to >16-bit UWORD machine)
X */
X
void
randomize(N a, ULONG b)
{
X	ULONG i;
X	NN c;
X	N d = c;
X	cp(c,a);
X	cl(a);
X	if(b>UNIT_BITS*NSIZE-2) {
X		i = NSIZE-1;
X		b = UNIT_BITS-2;
X	} else {
X		i = b / UNIT_BITS;
X		b = b % UNIT_BITS;
X	}
X	while (i--)
X		*a++ = *d++ ^ ri();
X	*a = ((*d ^ ri()) & ((1 << b) - 1)) | (1 << b);
}
X
/*
X * const sp[PRIMES], PRIMES -- table of small primes, number of primes in table
X */
X
static unsigned char sp[PRIMES] = {
X  2,  3,  5,  7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
X 59, 61, 67, 71, 73, 79, 83, 89, 97,101,103,107,109,113,127,131,
137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,
227,229,233,239,241,251
};
X
/*
X * divisor = sieve_prime(N n) -- try to find divisor of n by sieving
X * with small integers returns divisor or 0 if no divisor found.
X */
X
ULONG
sieve_prime(N n)
{
X	ULONG i;
X	NN a,b,c;
X	cl(b);
X	for(i=0;i<PRIMES;i++) {
X		cp(a,n);
X		*b = sp[i];
X		dm(a,b,c);
X		if(!ts(c)) return ((ULONG)sp[i]);
X	}
X	return (0);
}
X
/*
X * flag = prob_prime(N n) -- test if 2^(n-1) mod n == 1. Returns 0 if
X * test failed, !0 if success. Large n which passes this test is a
X * probable prime. This test does not work well with small value of n. 
X * Because this test is slow, you should first try sieving n.
X */
X
int
prob_prime(N m)
{
X	ULONG i = NSIZE, j = UNIT_BITS, p = NSIZE;
X	NN c, ee;
X	N mp, e = ee + NSIZE;
X	cl(c);
X	*c = 1;
X	cp(ee, m);
X	sb(ee, c);
X	while (!*--e && i)
X		i--;
X	if (i) {
X		while (!(*e & (1 << j)))
X			j--;
X		*c = 2;
X	}
X	mp = m + NSIZE;
X	while (!*--mp && p)
X		p--;
X	if (*mp & SIGN_BIT && p < NSIZE)
X		p++;
X	while (i--) {
X		while (j--) {
X			pmm(c, c, m, p);
X			if (*e & (1 << j)) {
X				sl(c);
X				if(sb(c, m)) ad(c, m);
X			}
X		}
X		e--;
X		j = UNIT_BITS;
X	}
X	cl(ee);
X	*ee = 1;
X	return (!cu(c,ee));
}
X
#ifndef THINK_SILENTLY
X
/*
X * tw() -- indicate working when checking/generating primes
X */
X
#include <stdio.h>		/* only for tw() */
X
static void
tw(void)
{
X	static ULONG j = 0;
X	putchar("/-\\|"[j & 3]);
X	j++;
X	putchar('\b');
X	fflush(stdout);
}
X
#endif /* THINK_SILENTLY */
X
/*
X * next_prime(N a) -- find next probable prime >= a
X */
X
void
next_prime(N a)
{
X	NN b;
X	*a |= 1;
X	cl(b); *b = 2;
X	for (;;) {
#ifndef THINK_SILENTLY
X		tw();
#endif /* THINK_SILENTLY */
X		if (!sieve_prime(a)) {
X			if (prob_prime(a))
X				return;
X		}
X		ad(a, b);
X	}
}
X
/*
X * bits rsa_gen(rsa_key *key) -- generate a RSA key from key->p and key->q
X * Initialize key->p and key->q either with primes or strong random
X * integers of apporopriate size. Returns number of bits in modulus key->pq
X * or 0 if key generation failed.
X */
X
ULONG
rsa_gen(rsa_key *k)
{
X	NN p1, q1, pq1, f, g, t;
X	next_prime(k->p);
X	next_prime(k->q);
X	if (cu(k->p, k->q) < 0) {
X		cp(t, k->p);
X		cp(k->p, k->q);
X		cp(k->q, t);
X	}
X	hn(t, "1");
X	cp(p1, k->p);
X	sb(p1, t);
X	cp(q1, k->q);
X	sb(q1, t);
X	cp(g, p1);
X	gd(g, q1);
X	hn(t, "ff");
X	if (cu(t, g) < 0)
X		return 0;
X	cp(k->pq, k->p);
X	mu(k->pq, k->q);
X	cp(pq1, p1);
X	mu(pq1, q1);
X	cp(f, pq1);
X	dm(f, g, t);
X	hn(k->e, "3");
X	hn(k->qp, "1");
X	cp(t, pq1);
X	gd(t, k->e);
X	if (cu(t, k->qp)) {
X		hn(k->e, "10001");
X		cp(t, pq1);
X		gd(t, k->e);
X		if (cu(t, k->qp))
X			return 0;
X	}
X	cp(k->d, k->e);
X	iv(k->d, f);
X	cp(t, k->d);
X	dm(t, p1, k->dp);
X	cp(t, k->d);
X	dm(t, q1, k->dq);
X	cp(k->qp, k->q);
X	iv(k->qp, k->p);
X	cp(t, k->pq);
X	for(k->b = 0; ts(t); sr(t), k->b++)
X		; /* VOID */
X	return (k->b);
}
X
/*
X * rsa_dec(N m, rsa_key *key) -- low level rsa decryption. Result undefined
X * (ie. wrong) if key is not private rsa key.
X */
X
void
rsa_dec(N m, rsa_key * k)
{
X	NN mp, mq, t;
X	cp(t, m);
X	dm(t, k->p, mp);
X	cp(t, m);
X	dm(t, k->q,
X	   mq);
X	em(mp, k->dp, k->p);
X	em(mq, k->dq, k->q);
X	if (sb(mp, mq))
X		ad(mp, k->p);
X	mm(mp, k->qp,
X	   k->p);
X	mu(mp, k->q);
X	ad(mp, mq);
X	cp(m, mp);
}
X
/*
X * rsa_enc(N m, rsa_key *k) -- low level rsa encryption
X */
X
void
rsa_enc(N m, rsa_key * k)
{
X	em(m, k->e, k->pq);
}
X
/*
X * len = n_to_b(unsigned char *buf, N a) -- convert a to bytes, most
X * significant byte first. Returns number of bytes written to buf. buf
X * should be large enough to hold sizeof(NN) bytes. (Note that number
X * is handled as unsigned, negative value converts to sizeof(NN) bytes.)
X * (XXX check portablility when converting to not-16/32 bit machine)
X */
X
ULONG
n_to_b(unsigned char *b, N a)
{
X	ULONG i = NSIZE - 1, l = 1;
X	a += NSIZE;
X	while (!*--a && i)
X		i--;
X	if (*a > 255) {
X		*b++ = *a >> 8;
X		l++;
X	}
X	*b++ = *a;
X	while (i--) {
X		*b++ = *--a >> 8;
X		*b++ = *a;
X		l += 2;
X	} 
X	return (l);
}
X
/*
X * b_to_n(N a, unsigned char *buf,ULONG len) -- convert len bytes from
X * buf to value a. Conversion is unsigned, most significant byte first.
X * (XXX check portablility when converting to not-16/32 bit machine)
X */
X
void
b_to_n(N a, unsigned char *b, ULONG l)
{
X	ULONG i;
X	if (l > NSIZE * sizeof(UWORD))
X		l = NSIZE * sizeof(UWORD);
X	b += l;
X	cl(a);
X	i = l / 2;
X	while (i--) {
X		*a = *--b;
X		*a++ |= (ULONG) *--b << 8;
X	}
X	if (l & 1)
X		*a = *b;
}
X
SHAR_EOF
chmod 0644 mrsa.c ||
echo 'restore of mrsa.c failed'
Wc_c="`wc -c < 'mrsa.c'`"
test 10725 -eq "$Wc_c" ||
	echo 'mrsa.c: original size 10725, current size' "$Wc_c"
fi
# ============= mrsa.h ==============
if test -f 'mrsa.h' -a X"$1" != X"-c"; then
	echo 'x - skipping mrsa.h (File already exists)'
else
echo 'x - extracting mrsa.h (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'mrsa.h' &&
/*
X * mrsa.h -- Multiprecision math and  RSA public key crypt library
X * 1993 Risto Paasivirta, paasivir@jyu.fi
X * Public Domain, no warranty. Use as you wish.
X */
X
#ifndef	MRSA_H
#define MRSA_H 1
#ifndef NSIZE
/* #define NSIZE 32 for max 512 (actually 511) bit modulus */
#define NSIZE 16
#endif
X
/*
X * unsigned 16 and 32 -bit types (other sizes may need some porting)
X */
X
#ifndef	UWORD	
typedef unsigned short UWORD;
#endif
#ifndef	ULONG
typedef unsigned long ULONG;
#endif
X
typedef UWORD *N, NN[NSIZE];
X
typedef struct rsa_key {
X	ULONG b;
X	NN pq,e,d,p,q,dp,dq,qp;
} rsa_key;
X
#if !defined(RSA_ONLY) || defined(MRSA_C)
X
#define UNIT_BITS 16		/* unit bits */
#define SIGN_BIT (1<<15)	/* top bit of unit */
#define PRIMES 54		/* number of primes in prime table */
X
#ifdef	RSA_ONLY	/* define RSA_ONLY if math stuff not needed */
#define PRIVATE static
#else
#define PRIVATE
#endif
X
X
int	ts(N a);		/* test signed, returns -1, 0 or 1 */
ULONG	ng(N a);		/* negate, return carry*/
void	cl(N a);		/* clear */
void	cp(N a,N b);		/* copy, a = b */
int	cu(N a,N b);		/* compare unsigned, returns, -1 0 or 1 */
ULONG	ad(N a,N b);		/* add, a += b */
ULONG	sb(N a,N b);		/* substract, a -= b */
ULONG	sr(N a);		/* shift right, a >>= 1, return carry */
ULONG	sl(N a);		/* shift left, a <<= 1, return carry */
void	dm(N a,N b,N c);	/* div-mod unsigned, a /= b, c = a % b */
void	mu(N a,N b);		/* multiply unsigned, a *= b */
void	mm(N a,N b,N m);	/* modular multiply, a = a * b mod m */
void	em(N a,N e,N m);	/* modular exponentiation, a = a^e mod m */
void	gd(N a,N b);		/* greatst common divisor, a = gcd(a,b) */
void	iv(N a,N b);		/* multiplicative inverse, a = a^{-1} mod p */
void	nh(char *a,N b);	/* convert number to hex string */
void	hn(N a,char *b);	/* convert lowercase hex string to number */
ULONG	ri();			/* weak pseudorandom integer */
X
#if	defined(AMIGA) && defined(MRSA_C)
__stdargs pmm(N aa, N b, N m, ULONG p); /* assembler modmul (SAS 6.2) */
#endif
X
#endif /* RSA_C */
X
void	randomize(N a, ULONG bits);
ULONG	sieve_prime(N);
int	prob_prime(N);
void	next_prime(N);
ULONG 	rsa_gen(rsa_key *);
void 	rsa_enc(N,rsa_key *);
void 	rsa_dec(N,rsa_key *);
ULONG 	n_to_b(unsigned char *,N);
void 	b_to_n(N,unsigned char *,ULONG);
X
#endif /* MRSA_H */
X
SHAR_EOF
chmod 0644 mrsa.h ||
echo 'restore of mrsa.h failed'
Wc_c="`wc -c < 'mrsa.h'`"
test 2250 -eq "$Wc_c" ||
	echo 'mrsa.h: original size 2250, current size' "$Wc_c"
fi
# ============= pmm68k.s ==============
if test -f 'pmm68k.s' -a X"$1" != X"-c"; then
	echo 'x - skipping pmm68k.s (File already exists)'
else
echo 'x - extracting pmm68k.s (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'pmm68k.s' &&
;-------------------------------------------------------------------------
; pmm68k.s
;
; (68000 assembler modmul, from my Amiga library, (max NSIZE = 65)
;
; 1993 Risto Paasivirta, paasivir@jyu.fi.
; Public Domain, NO WARRANTY.
;-------------------------------------------------------------------------
X
X		section	__MERGED,data
X
X		ds.l	33
a		ds.l	33
b		ds.l	33
m		ds.l	33
r
X
X		section	text,code
X
X		xdef	_pmm
X
;-------------------------------------------------------------------------
; Kludge glue
;
; __stdargs pmm(N aa, N b, N m, ULONG p)
;
X
glue		macro
X		movea.l	\1,a0		; load source
X		movea.l	\2,a1		; load dst
X		move.w	d0,d1
X		bra.b	gl.2\@
gl.1\@		move.w	(a0)+,-(a1)	; flip word endianess
gl.2\@		dbf	d1,gl.1\@
X		endm
X
_pmm		move.l	4*4(sp),d0
X		glue	1*4(sp),#a
X		clr.w	-(a1)		; clear last word
X		glue	2*4(sp),#b
X		clr.w	-(a1)		; clear last word
X		glue	3*4(sp),#m
X		clr.w	-(a1)		; clear last word
X		
X		addq.l	#1,d0		; long size
X		lsr.l	#1,d0
X		move.w	d0,d1		; offset address
X		lsl.w	#2,d1
X		lea	r,a0
X		suba.w	d1,a0		; address of r
X		move.l	a0,-(sp)	; to stack
X		move.w	#33*4,d1
X		suba.w	d1,a0		; addr of m
X		move.l	a0,-(sp)	; to stack
X		suba.w	d1,a0		; b
X		move.l	a0,-(sp)	; to stack
X		suba.w	d1,a0		; a
X		move.l	a0,-(sp)	; to stack
X		move.l	d0,-(sp)	; long len
X		bsr.b	__m_modmul
X		lea	5*4(sp),sp	; drop params
X
X		move.l	4*4(sp),d0	; copy result
X		lea	r,a0
X		movea.l	1*4(sp),a1
X		bra.b	pmm.2
pmm.1		move.w	-(a0),(a1)+	; flip word endianess
pmm.2		dbf	d0,pmm.1
X		rts
X
;-------------------------------------------------------------------------
; Macros
;-------------------------------------------------------------------------
; clra
X
clra		macro
X		suba.l	\1,\1
X		endm
X
;-------------------------------------------------------------------------
; clx
X
clx		macro
X		andi.w	#~(1<<4),ccr
X		endm
X
;-------------------------------------------------------------------------
; sex
X
sex		macro
X		ori.w	#(1<<4),ccr
X		endm
X
;-------------------------------------------------------------------------
; sez
X
sez		macro
X		ori.w	#(1<<2),ccr
X		endm
X
;-------------------------------------------------------------------------
; _m_modmul(w16 len, w32 *a, w32 *b, w32 *m, w32 *r)
;
; r = a * b mod m unsigned, b < m
;
X
;register usage:
; d0 = long counter:bit counter, d1 = counter, d2 = dividend unit
; d6 = long length, d7 = byte length
; a0 = multiplier, a1 = multiplier, a2 = divisor, a3 = remainder
; a5/a6 = work (for compare)
X
__m_modmul	movem.l	a2-a3/a5-a6/d2/d6-d7,-(sp)
X		movem.l	32(sp),d0/a0-a3
X		move.l	d0,d6		; save long length (.l)
X		move.l	d0,d7
X		lsl.l	#2,d7		; save byte length (.l)
X		adda.l	d7,a3		; point past remainder
X
X		bra.b	2$
X
1$		clr.l	-(a3)		; clear remainder
2$		dbf	d0,1$
X
X		move.w	d6,d0		; long counter
X		sez			; Z for compare
X		bra.b	4$
X
3$		move.l	(a0)+,d2	; find 1st nonzero unit of mcand
4$		dbne	d0,3$
X
X		beq	modmul_zero	; 0/X
X
X		swap	d0		; use both halves of d0
X		move.w	#31,d0		; bit counter
X
5$		btst	d0,d2		; find 1st bit set
X		dbne	d0,5$
X
X		move.l	d6,d1
X		adda.l	d7,a1		; point past multiplier
X		adda.l	d7,a3		; point past remainder
X		bra.b	7$
X
6$		move.l	-(a1),-(a3)	; copy multiplier to remainder
7$		dbf	d1,6$
X
X		bra	modmul_continue
X
modmul_longloop	move.l	(a0)+,d2
X		swap	d0
X		move.w	#31,d0
X
modmul_bitloop	move.w	d6,d1
X		add.w	d6,d1		; word size (x clear)
X		adda.l	d7,a3		; point past remainder
X		bra.b	2$
X
1$		roxl.w	-(a3)		; shift remainder left
2$		dbf	d1,1$
X
modmul_1stsub	move.w	d6,d1		; get long len (clear carry)
X		movea.l	a2,a5		; divisor
X		movea.l	a3,a6		; remainder
X		sez			; Z flag (for dbne)
X		bra.b	2$
X
1$		cmpm.l	(a5)+,(a6)+	; compare divisor and remainder
2$		dbne	d1,1$
X
X		bcs.b	modmul_add	; divisor greater
X
X		move.w	d6,d1		; long len
X		adda.l	d7,a2		; point past divisor
X		adda.l	d7,a3		; point past remainder
X		clx
X		bra.b	4$
X
3$		subx.l	-(a2),-(a3)	; substract divisor from remainder
4$		dbf	d1,3$
X
modmul_add	btst	d0,d2		; check next multiplicand bit
X		beq.b	modmul_continue ; zero, next iteration
X
X		move.w	d6,d1		; long len
X		adda.l	d7,a1		; point past multiplier
X		adda.l	d7,a3		; point past remainder
X		clx
X		bra.b	4$
X
3$		addx.l	-(a1),-(a3)	; add multiplier to remainder
4$		dbf	d1,3$
X
modmul_2ndsub	move.w	d6,d1		; get long len (clear carry)
X		movea.l	a2,a5		; divisor
X		movea.l	a3,a6		; remainder
X		sez			; Z flag (for dbne)
X		bra.b	2$
X
1$		cmpm.l	(a5)+,(a6)+	; compare divisor and remainder
2$		dbne	d1,1$
X
X		bcs.b	modmul_continue	; divisor greater
X
X		move.w	d6,d1		; long len
X		adda.l	d7,a2		; point past divisor
X		adda.l	d7,a3		; point past remainder
X		clx
X		bra.b	4$
X
3$		subx.l	-(a2),-(a3)	; substract divisor from remainder
4$		dbf	d1,3$
X
modmul_continue	dbf	d0,modmul_bitloop
X		swap	d0
X		dbf	d0,modmul_longloop
X
modmul_zero	movem.l	(sp)+,a2-a3/a5-a6/d2/d6-d7
X		rts
X
;-------------------------------------------------------------------------
X		end
;-------------------------------------------------------------------------
SHAR_EOF
chmod 0644 pmm68k.s ||
echo 'restore of pmm68k.s failed'
Wc_c="`wc -c < 'pmm68k.s'`"
test 4890 -eq "$Wc_c" ||
	echo 'pmm68k.s: original size 4890, current size' "$Wc_c"
fi
# ============= rsatest.c ==============
if test -f 'rsatest.c' -a X"$1" != X"-c"; then
	echo 'x - skipping rsatest.c (File already exists)'
else
echo 'x - extracting rsatest.c (Text)'
sed 's/^X//' << 'SHAR_EOF' > 'rsatest.c' &&
/*
X * rsatest.c
X */
X
#include <stdio.h>
#include <stdlib.h>
#include "mrsa.h"
X
void
main(int i, char **v) {
X	NN m,c,d;
X	rsa_key key;
X	char s[NSIZE*4+2];
X
X	if(i!=3) {
X		puts("Usage: mtest [hex_pseed] [hex_qseed]");
X		exit(10);
X	}
X
X	hn(key.p,v[1]);
X	hn(key.q,v[2]);
X
X	puts("Generating key...");
X	if(!rsa_gen(&key)) {
X		puts("Failed, try other seed values.");
X		exit(10);
X	}
X	puts("Done, key components: pq,e,d,p,q,dp,dq,qp");		
X	printf("bits = %u\n",key.b);
X	nh(s,key.pq); puts(s);
X	nh(s,key.e); puts(s);
X	nh(s,key.d); puts(s); 
X	nh(s,key.p); puts(s);
X	nh(s,key.q); puts(s);
X	nh(s,key.dp); puts(s);
X	nh(s,key.dq); puts(s);
X	nh(s,key.qp); puts(s);
X	puts("testing, msg,cip,dec1,dec2");
X	cl(m);
X	randomize(m, key.b-2);
X	nh(s,m); puts(s);
X	cp(c,m); rsa_enc(c,&key);
X	nh(s,c); puts(s);
X	cp(d,c); em(d,key.d,key.pq); /* slow way */
X	nh(s,d); puts(s);
X	cp(d,c); rsa_dec(d,&key); /* faster way */
X	nh(s,d); puts(s);
X	exit(0);
}
X
SHAR_EOF
chmod 0644 rsatest.c ||
echo 'restore of rsatest.c failed'
Wc_c="`wc -c < 'rsatest.c'`"
test 919 -eq "$Wc_c" ||
	echo 'rsatest.c: original size 919, current size' "$Wc_c"
fi
exit 0
-- 
/*paasivir@jyu.fi*/int a[3302],b=3301,*c=a,d,e,f;main(){for(e=b;--e;*c++=1);*c
=2;for(d=2001;d--;printf("%05d",f))for(c=a,e=b;e;f/=e--){f+=*c*1e5;*c++=f%e;}}
