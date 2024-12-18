2:
	gcc ./2-system-calls.c -o ./main.out -lm

3:
	gcc ./3-matrix-operations.c -o ./main.out -lm

4:
	gcc ./4-rw-mutex.c -o ./main.out -lm

5:
	gcc ./5-pc-mutex.c -o ./main.out -lm

6: 
	gcc ./6-rw-semaphore.c -o ./main.out -lm

7:
	gcc ./7-pc-semaphore.c -o ./main.out -lm

8:
	gcc ./8-diningPh-sem.c -o ./main.out -lm

9:
	gcc ./9-fcfs.c -o ./main.out -lm 

10:
	gcc ./10-sjf-both.c -o ./main.out -lm 

11:
	gcc ./11-priority-both.c -o ./main.out -lm

12:
	gcc ./12-round-robin.c -o ./main.out -lm

13:
	gcc ./13-deadlock-avoid-bankers.c -o ./main.out -lm

15:
	gcc ./15-fifo-page-fault.c -o ./main.out -lm 

16:
	gcc ./16-lru-page-fault.c -o ./main.out -lm 

17: 
	gcc ./17-optimal-page-fault.c -o ./main.out -lm

18: 
	gcc ./18-diskSch-fcfs.c -o ./main.out -lm

19: 
	gcc ./19-diskSch-sstf.c -o ./main.out -lm

20: 
	gcc ./20-diskSch-scan.c -o ./main.out -lm

21: 
	gcc ./21-diskSch-cscan.c -o ./main.out -lm

24:
	gcc ./24-copy-contents.c -o ./main.out -lm

25:
	gcc ./25-alp-load.c -o ./main.out -lm

26:
	gcc ./26-opcode-error.c -o ./main.out -lm

27:
	gcc ./27-operand-error.c -o ./main.out -lm
	

clean:
	rm -rf ./main.out 

run:
	./main.out