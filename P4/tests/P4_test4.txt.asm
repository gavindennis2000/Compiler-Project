	 sum?0
	 sum?0
	 sum?0
LOAD 0
STORE P91
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 100
STORE ARG1
	 sum?0
	 sum?0
LOAD P91
STORE ARG2
LOAD ARG1
SUB ARG2
BRZNEG SKIP1
	 sum?0
	 sum?0
LOAD 4
STORE ITER1
BRZNEG SKIP1
LOOP1: SUB 1
STORE ITER1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 100
ADD P91
	 sum?1
LOAD ITER1
BRPOS LOOP1
SKIP1: NOOP
STORE TEMP1
WRITE TEMP1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 100
STORE P91
	 sum?0
	 sum?0

STOP
P91 0
ARG1 0
ARG2 0
ITER1 0
TEMP1 0
