
MPI_FLAGS=-I/usr/include/openmpi-x86_64 -pthread -Wl,-rpath -Wl,/usr/lib64/openmpi/lib -Wl,--enable-new-dtags -L/usr/lib64/openmpi/lib -lmpi_cxx -lmpi

ADIOS_13_FLAGS=-I/work/apps/adios/1.13.0-g/include -I/work/apps/dataspaces/1.6.2/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -L/work/apps/adios/1.13.0-g/lib -ladios -L/work/apps/dataspaces/1.6.2/lib -ldspaces -ldscommon -ldart -lpthread -lm -fPIC /work/apps/chaos/stable/lib/libevpath.a /work/apps/chaos/stable/lib/libffs.a /work/apps/chaos/stable/lib/libatl.a /work/apps/chaos/stable/lib/libdill.a /work/apps/chaos/stable/lib/libcercs_env.a -ldl -libverbs -fPIC -lpthread -lm

ADIOS_12_FLAGS=-I/work/apps/adios/1.12.0-g/include -I/work/apps/dataspaces/1.6.2/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -L/work/apps/adios/1.12.0-g/lib -ladios -L/work/apps/dataspaces/1.6.2/lib -ldspaces -ldscommon -ldart -lpthread -lm -fPIC /work/apps/chaos/stable/lib/libevpath.a /work/apps/chaos/stable/lib/libffs.a /work/apps/chaos/stable/lib/libatl.a /work/apps/chaos/stable/lib/libdill.a /work/apps/chaos/stable/lib/libcercs_env.a -ldl -libverbs -fPIC -lpthread -lm

ADIOS_11_FLAGS=-I/work/apps/adios/1.11.0-g/include -I/work/apps/dataspaces/1.6.2/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -I/work/apps/chaos/stable/include -L/work/apps/adios/1.11.0-g/lib -ladios -L/work/apps/dataspaces/1.6.2/lib -L/work/apps/chaos/stable/lib -L/work/apps/chaos/stable/lib -L/work/apps/chaos/stable/lib -L/work/apps/chaos/stable/lib -L/work/apps/chaos/stable/lib -ldspaces -ldscommon -ldart -levpath -lffs -latl -ldill -lcercs_env -libverbs -fPIC -lm

ADIOS_FLAGS=$(ADIOS_13_FLAGS)

.PHONY:clean
.PHONY:exercise
.PHONY:solution

exercise: clean
	ln -s exercise/put.cpp put.cpp
	ln -s exercise/get.cpp get.cpp
	g++ -O0 -g3 -std=c++11 put.cpp $(MPI_FLAGS) $(ADIOS_FLAGS) -o put
	g++ -O0 -g3 -std=c++11 get.cpp $(MPI_FLAGS) $(ADIOS_FLAGS) -o get

solution: clean
	ln -s solution/put.cpp put.cpp
	ln -s solution/get.cpp get.cpp
	g++ -O0 -g3 -std=c++11 put.cpp $(MPI_FLAGS) $(ADIOS_FLAGS) -o put
	g++ -O0 -g3 -std=c++11 get.cpp $(MPI_FLAGS) $(ADIOS_FLAGS) -o get

clean:
	rm -f put.cpp get.cpp put get conf *.bp
