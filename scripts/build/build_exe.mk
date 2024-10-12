#-------------------------------------------------------------------------------
#support c/c++ compile.
#build excutatble
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#							Build Config Region 
#-------------------------------------------------------------------------------

###buildout file name
buildout ?=

###compile tools
C_COMPILE ?= $(APP_C_COMPILE)
CC_COMPILE ?= $(APP_CC_COMPILE)

CFLAGS  := -O3 -std=c++11
CCFLAGS  := -O3 -std=c++11

###include Path
INCLUDES ?= 

###include library
library ?= 

###c/cpp objects for build
object ?=
c_objects ?=
cpp_objects ?=

objects+=${c_objects}
objects+=${cpp_objects}

# exe destination
run_dist ?=

# build information
$(info build excutable file)
$(info used c compiler:$(C_COMPILE), c++ compiler:$(CC_COMPILE))

#-------------------------------------------------------------------------------
#						End of Build Config Region 
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#						Build Rules Region 
#-------------------------------------------------------------------------------
%.o : %.cpp
	$(CC_COMPILE) $(CCFLAGS) -c $< -o $@ $(addprefix -I , $(INCLUDES))

%.o : %.c
	$(C_COMPILE) $(CFLAGS) -c $< -o $@ $(addprefix -I , $(INCLUDES))

all : $(buildout)

#build excutable file
$(buildout): $(objects)
ifeq ("${cpp_objects}", "")
	$(C_COMPILE) -o $(buildout) $(objects) $(library) $(CFLAGS)
else
	$(CC_COMPILE) -o $(buildout) $(objects) $(library) $(CCFLAGS) 
endif
	rm -rf $(objects)
	mkdir -m 755 -p ${run_dist}/
	mv $(buildout) $(run_dist)/

clean:
	rm -f $(objects)
tags :
	ctags -R *
#-------------------------------------------------------------------------------
#						End of Build Rules Region 
#-------------------------------------------------------------------------------