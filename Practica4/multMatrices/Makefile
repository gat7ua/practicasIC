MPICC?=mpicc

all: ${EXECS}

comPar: mult_par.cc
	${MPICC} -o mult_par mult_par.cc -lstdc++

compSec: mult_sec.cc
	${MPICC}  mult_sec.cc -o mult_sec -lstdc++

sec: compSec
	./mult_sec
clean:
	rm ./mult_sec ./mult_par

par: comPar
	mpirun -mca plm_rsh_no_tree_spawn 1 -hostfile host_files -n 4 ./mult_par
