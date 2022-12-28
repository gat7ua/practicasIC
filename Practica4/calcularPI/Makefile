EXECS=calcularPI
MPICC?=mpicc

all: ${EXECS}

comPar: calcularPI.cc
	${MPICC} -o calcularPI calcularPI.cc -lstdc++

compSec: calcularPI_sec.cc
	g++ calcularPI_sec.cc -o calcularPI -lstdc++

sec: compSec
	./calcularPI
clean:
	rm ./calcularPI ./calcularPI_sec

par: comPar
	mpirun -mca plm_rsh_no_tree_spawn 1 -hostfile host_files -n 4 ./${EXECS}
