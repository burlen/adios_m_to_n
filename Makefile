MPI_FLAGS=-fPIC -Wl,-z,noexecstack -I/usr/include/mpich-x86_64 -L/usr/lib64/mpich/lib -Wl,-rpath -Wl,/usr/lib64/mpich/lib -Wl,--enable-new-dtags -lmpi

ADIOS_13_FLAGS=-I/home/sensei/sc17/software/adios/1.13.0/include -I/home/sensei/sc17/software/dataspaces/1.6.2/include -I/home/sensei/sc17/software/chaos/stable-2017.10/include -I/home/sensei/sc17/software/chaos/stable-2017.10/include -I/home/sensei/sc17/software/chaos/stable-2017.10/include -I/home/sensei/sc17/software/chaos/stable-2017.10/include -L/home/sensei/sc17/software/adios/1.13.0/lib -ladios -L/home/sensei/sc17/software/dataspaces/1.6.2/lib -ldspaces -ldscommon -ldart -lpthread -lm -fPIC /home/sensei/sc17/software/chaos/stable-2017.10/lib/libevpath.a /home/sensei/sc17/software/chaos/stable-2017.10/lib/libffs.a /home/sensei/sc17/software/chaos/stable-2017.10/lib/libatl.a /home/sensei/sc17/software/chaos/stable-2017.10/lib/libdill.a /home/sensei/sc17/software/chaos/stable-2017.10/lib/libcercs_env.a -ldl -fPIC -lpthread -lm

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
