/* CS370 PROJECT 4
 * FILE SERVER, SERVING A SINGLE FILE THAT INCREMENTS A COUNTER ON EVERY READ
 */

#include	"dat.h"
#include	"fns.h"
#include	"error.h"

enum {
	Qdir,
	Qdata,
};

Dirtab counttab[] =
{
	".",		{Qdir, 0, QTDIR},	0,	0555,
	"count",	{Qdata},	0,	0666,
};

int itoa(char*, int);

uint counter = 0;

static Chan *
countattach(char *spec)
{
	return devattach('n', spec);
}

static Walkqid *
countwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, counttab, nelem(counttab), devgen);
}

static int
countstat(Chan *c, uchar *db, int n)
{
	return devstat(c, db, n, counttab, nelem(counttab), devgen);
}

static Chan *
countopen(Chan *c, int omode)
{
	return devopen(c, omode, counttab, nelem(counttab), devgen);
}

static void
countclose(Chan *c)
{
	USED(c);
}

static long
countread(Chan *c, void *buf, long numbytes, vlong offset)
{
	int len;
	USED(c);
	USED(numbytes);

	if(c->qid.type & QTDIR) {
			return devdirread(c, buf, numbytes, counttab, nelem(counttab), devgen);
	}
	if(offset == 0) {
		memset(buf, 0, numbytes);
		counter++;
		len = itoa(buf, counter);

		return len;
	}

	return 0;
	
}

static long
countwrite(Chan *c, void *buf, long numbytes, vlong offset)
{
	USED(c);
	USED(numbytes);
	USED(offset);

	counter = atoi((char *) buf);

	return counter;
}

Dev countdevtab = {
	'n',
	"count",

	devinit,
	countattach,
	countwalk,
	countstat,
	countopen,
	devcreate,
	countclose,
	countread,
	devbread,
	countwrite,
	devbwrite,
	devremove,
	devwstat,
};


//itoa copied from "lib/lego/styx.c"
int
itoa(char *buf, int value)
{
	char *bp = buf;
	int divisor;
	if (value < 0) {
		*bp++ = '-';
		value = -value;
	}
	if (value == 0)
		*bp++ = '0';
	else {
		divisor = 10000;
		while (divisor > value)
			divisor /= 10;
		while (divisor) {
			*bp++ = '0' + value / divisor;
			value %= divisor;
			divisor /= 10;
		}
	}
	return bp - buf;
}