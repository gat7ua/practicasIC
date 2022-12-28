EXECS=mpi_hello_world
MPICC?=mpicc

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	${MPICC} -o mpi_hello_world mpi_hello_world.c

clean:
	rm ${EXECS}

exec: mpi_hello_world
	mpirun -mca plm_rsh_no_tree_spawn 1 -hostfile host_files -n 3 ./${EXECS}
