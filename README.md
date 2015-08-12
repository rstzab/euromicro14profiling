# Evaluation code for EUROMICRO/SEAA 2014 Paper

The code in this repository was used to collect evaluation information for EUROMICRO/SEAA 2014 paper ["Profiling-Based Task Scheduling for Factory-Worker Applications in Infrastructure-as-a-Service Clouds"](http://www.infosys.tuwien.ac.at/staff/phdschool/rstzab/papers/SEAA2014_Profiling-based%20task%20scheduling.pdf).

In order to compile and run the project, you need:

* linux-based system (ubuntu 13.04 was used)
* gcc (clang) compiler (gcc version 4.8.1, clang version 3.2-7ubuntu1 were used)
* additional libraries:
	
    sudo apt-get install libpng-dev imagemagick libmagickcore-dev

The project can be compiled and run using `runme.sh` script located at the same folder.

The project should be compilable and runnable on Windows as well, 
the simplest approach is to use minGW or Cygwin in order to run gcc.
Additional dependencies can be satisfied with 
[Magick++](http://www.imagemagick.org/Magick++/) project.

Additionally, there is [CodeBlocks](http://www.codeblocks.org) project file (`ImageProcessing.cbp`),that should allow to build and run project on any platform.

If you don't want to compile/run evaluation again, raw results are available in "results" folder.
