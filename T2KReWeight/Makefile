# Authors:
# Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#  STFC - Rutherford Appleton Laboratory, UK
#
# Jim Dobson <j.dobson07 \at imperial.ac.uk>
#  Imperial College London, UK

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(T2KREWEIGHT)/make/Make.include

BUILD_TARGETS = make-bin-lib-dir \
		print \
		autogen-headers \
		app 
                
all: $(BUILD_TARGETS)

make-bin-lib-dir: FORCE
	@echo " "
	@echo "** Creating T2KReWeight lib and bin directories..."
	cd ${T2KREWEIGHT}; \
	[ -d bin ] || mkdir bin; chmod 755 bin;\
	[ -d lib ] || mkdir lib; chmod 755 lib;

print: FORCE
	@echo " "
	@echo " "
	@echo "***** Building T2KReWeight from source tree at: $(T2KREWEIGHT)"
	@echo "***** The source tree corresponds to T2KReWeight version $(T2KREWEIGHT_VERSION)"
	@echo " "

autogen-headers: FORCE
	@echo " "
	@echo "** Auto-generating T2KBuild.h and T2KVersion.h headers..."
	cd ${T2KREWEIGHT}/make;\
	perl write-version-header; \
	perl write-build-header;   \
	cd ${T2KREWEIGHT}


src:	FORCE
	@echo " "
	@echo "** Building all in src directory..."
	cd ${T2KREWEIGHT}/src;\
	make; \
	cd ${T2KREWEIGHT}

app: src
	@echo " "
	@echo "** Building all in app directory..."
	cd ${T2KREWEIGHT}/app;\
	make; \
	cd ${T2KREWEIGHT}

purge: FORCE
	@echo " "
	@echo "** Purging..."
	cd ${T2KREWEIGHT}/src;\
	make purge;\
	cd ${T2KREWEIGHT}

clean: clean-files clean-dir clean-etc

clean-files: FORCE
	@echo " "
	@echo "** Cleaning..."
	cd ${T2KREWEIGHT}/src; \
	make clean; cd ..; \
	cd ${T2KREWEIGHT}/app; \
	make clean; cd ..; \

clean-dir: FORCE
	@echo "Deleting the lib and bin directories...";\
	cd $(T2KREWEIGHT);\
	[ ! -d ./bin ] || rmdir ./bin;\
	[ ! -d ./lib ] || rmdir ./lib 

clean-etc: FORCE
	cd $(T2KREWEIGHT);\
	rm -f ./*log

FORCE:

