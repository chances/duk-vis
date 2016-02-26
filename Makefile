# DukDukVis default make config

all: build

build: build/Makefile
	cd build; make

build/Makefile:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..

cmake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..

run: build
	cd build; ./duk_vis

# Configure project (setup submoduale dependencies)
configure:
	git submodule init
	git submodule update
	cd vendor/OOGL; git fetch; git checkout osx; make

clean:
	rm -rf build

.PHONY: build cmake run configure clean
