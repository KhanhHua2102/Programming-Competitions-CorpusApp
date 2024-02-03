# Tools for South Pacific Problem Setting

Useful tools for problem setting.

## Polygon export instructions.

Get packages from polygon, if download all packages need to run wine doall.sh
otherwise if get linux packages it’s already generated.

Extract each problem into polygon/ in directories named A, B, C, ... - this is
necessary if you want to use run_polygon2problemtools.sh to make converting the
problems easier. If not, you can directly invoke polygon2problemtools.sh.

Run the run_polygon2problemtools.sh script to generate the problems. Make sure
to check problems that have custom checkers as these will require manual
modification.

After that, copy the latex files from a previous contest, e.g. these files:

xxDivisionals.tex
\*.png
problemset.cls

and update the tex file and problemset.cls file to refer to the new problems.

Directory structure should look like this in the end:
polygon/<POLYGON PROBLEMS>
domjudge/{A, B, C, ...}
domjudge/xx{Divisionals,Regionals}.tex + other latex support files.
