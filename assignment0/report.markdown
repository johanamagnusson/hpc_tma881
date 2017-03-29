# Assignment 0 TMA881
## Holger Lindström, 930916, holgerlindstrom93@gmail.com
## Andreas Magnusson, 900410, andreas.magnusson90@gmail.com
## Rickard Johansson

### 1. Installing

* We started by getting an account to ozzy and getting comfortable with ozzy and tmux.
* We found a PLASMA installer at [PLASMA](http://www.netlib.org/plasma/), simply the installer there.
* We created the directories HOME and plasma_local.
* We used the command "./setup.py --cc=gcc --fc=gfortran --blaslib="/usr/lib/libblas.a" --lapacklib="/usr/lib/lapack/liblapack.a"" on tmux to install PLASMA
* We realized that the tests did run directly after the install, but they take very long time. Therefore we interrupted it and then run the tests for themselfs.


### 2. Testing

When installing PLASMA, the test files run automatically as mentioned. We noticed however that the tests took a while to run, so we did them separatly from the install.

To do so we entered "/HOME/plasma_local/plasma-installer_2.8.0/build/plasma_2.8.0/testing" and entered "./plasma_testing.py -c 28". The -c flag let's us enter the number of cores we wanna use for the tests (figured out by using -h), and when using 28 it took about 15min. We think that they run the tests on 2 cores when running after the installation, and this takes time. 

### 3. Makefile

When we look in the Makefile in the testing directory, we can find the file for z-testing. Here we can also find the directly included makefiles Makefile.internal and Makefile.src. Makefile.internal also indirectly includes make.inc. 

Directly: Makefile.internal, Makefile.src
Indirectly: make.inc
