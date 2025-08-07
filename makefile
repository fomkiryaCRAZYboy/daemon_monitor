CC = /home/fomchik/x-tools/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-gcc

monitor:
	gcc audit_analysis.c check\&updt_daemons.c init.c main.c -o monitor

monitor-mips:
	$(CC) -static audit_analysis.c check\&updt_daemons.c init.c main.c -o monitor-mips

clean:
	rm monitor monitor-mips 2 >> /dev/null

getpid:
	pstree --show-pids | grep "monitor"

qemu-run:
	qemu-mipsel ./monitor-mips