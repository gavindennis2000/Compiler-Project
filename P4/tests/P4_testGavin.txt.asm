	 sum?0
	 sum?0
	 sum?0
LOAD 0
STORE P10
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 0
STORE P20
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 510
ADD -10
STORE P10
	 sum?1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD P10
ADD 0
STORE ARG1
	 sum?1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD P20
ADD 499
STORE ARG2
LOAD ARG1
SUB ARG2
BRZNEG SKIP1
	 sum?1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD P10
ADD -495
STORE ITER1
BRZNEG SKIP1
LOOP1: SUB 1
STORE ITER1
	 sum?1
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 1
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 2
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	Pushing110 300 to add nest. size: 3
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	Pushing110 -200 to add nest. size: 4
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	Pushing110 P10 to add nest. size: 5
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD -496
ADD 4226
ADD P10
	Popping384 P10 from add nest. size: 4
ADD -200
	Popping384 -200 from add nest. size: 3
ADD 300
	Popping384 300 from add nest. size: 2
	Popping399 & from add nest. size: 1
	Sum calc was true
	 sum?0
	 sum?0
	Popping375 & from add nest. size: 0
ADD -50
STORE TEMP1
WRITE TEMP1
LOAD ITER1
BRPOS LOOP1
SKIP1: NOOP
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 1
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 2
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 3
	 sum?0
	 sum?0
	Pushing104 & to add nest. size: 4
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 0
ADD 0
	Sum calc was true
	 sum?0
	 sum?0
	Popping375 & from add nest. size: 3
ADD 1
	 sum?0
	 sum?0
	Popping375 & from add nest. size: 2
ADD 2
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
	 sum?0
LOAD 3
ADD 4
	Sum calc was true
	 sum?0
	 sum?0
	Popping375 & from add nest. size: 1
ADD -999
	 sum?0
	 sum?0

STOP
P10 0
P20 0
ARG1 0
ARG2 0
ITER1 0
TEMP1 0
