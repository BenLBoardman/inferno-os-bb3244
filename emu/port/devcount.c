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
	"data",	{Qdata},	0,	0666,
};

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
countread(Chan *c, void *va, long count, vlong offset)
{
	static long counter = 0;
	USED(offset);

	if(c->qid.type & QTDIR)
		return devdirread(c, va, count, counttab, nelem(counttab), devgen);


	counter++;
	return counter;
}

static long
countwrite(Chan *c, void *va, long count, vlong offset)
{
	USED(c);
	USED(va);
	USED(offset);

	return count;
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
