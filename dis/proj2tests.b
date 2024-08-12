implement Proj2Tests;

include "sys.m";
	sys: Sys;
include "draw.m";

Proj2Tests: module
{
	init:		fn(ctxt: ref Draw->Context, argv: list of string);
	
	reusetest: fn();
	consumetest: fn();	


	emptyprog:	fn();
	longloopprog:	fn();
};

init(ctxt: ref Draw->Context, argv: list of string)
{
	sys = load Sys Sys->PATH;	
	consumetest();
}

reusetest()
{
	n: int;
	
	n = 0;
	for(; n < 100; n++)
	{
		spawn emptyprog();
		sys->sleep(100);
	} 

}

consumetest()
{
	n: int;
	n = 0;
	for(; n < 5; n++)
	{
		spawn longloopprog();
		sys->sleep(100);
	}
	for(n = 0; n < 100; n++)
	{
		spawn emptyprog();
		sys->sleep(100);
	}
	
}

emptyprog()
{
	sys->print("");
}

longloopprog()
{
	i, n: int;

	i = 0;
	for(;i < 30000000; i++)
	{
		n++;	
	}
	sys->print("longloop exiting\n");
}
