implement Proj4Tests;

include "sys.m";
	sys: Sys;
    rfd, wfd: ref Sys->FD;
include "draw.m";

Proj4Tests: module
{
	init:		fn(ctxt: ref Draw->Context, argv: list of string);
	
	read5test: fn(): int;
};

init(ctxt: ref Draw->Context, argv: list of string)
{
	sys = load Sys Sys->PATH;	
	read5test();
}

read5test(): int
{
    n: int;
    buf:= array [256] of byte;
    buf2:= array [256] of byte;
    str: string;
    rfd = sys->open("/dev/count", sys->OREAD);
    wfd = sys->open("/dev/count", sys->OWRITE);
    if(rfd == nil || wfd == nil)
    {
        sys->print("Failed to open file\n");
        return -1;
    }

    sys->print("File opened successfully\n");

    for(i := 0; i < 5; i++) 
    {
        n = sys->read(rfd, buf, len buf);
        sys->seek(rfd, big 0, sys->SEEKSTART);
        str = string buf;
        sys->print("Count file server has been read %s times\n", str);
    }
    
    buf = array of byte "500";
    n= sys->write(wfd, buf, 255);
    n = sys->read(rfd, buf2, len buf2);
    str = string buf2;
    sys->print("Count file server has been read %s times\n", str);

    rfd = wfd = nil;
    return 0;
}