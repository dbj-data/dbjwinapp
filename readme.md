# DBJWINAPP&trade;

&copy; 2020 by dbj.org -- [LICENSE-DBJ](https://dbj.org/license-dbj)

This is to be built as VStudio 2019 solution

NOTE: as ever we do only 
- x64 builds
- static runtime library using, apps  and lib's'

Depends on `dbj-fwk`:

-  dbj-fwk-lib, static library
	- https://github.com/dbj-data/dbj-fwk
	- to use `dbj-fwk` users need to provide `extern "C" int program (int argc, char ** argv)`
	  as done in program.cpp in here
	- must use static runtime library '/MT' or /MTd' for debug builds

Purpose of `DBJ-FWK` is SEH top level implementation. SEH is always there. It just has to be used.

This is WIN desktop App. Thus it has no console and stdio (C or C++) does not work. Thus 
it has to use logging. No logging no output. Thus we log into the file.