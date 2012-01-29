#! /bin/sh

if `test -e ./openanno`
then
	if [ "$1" == "--debugger" ]
	then
		gdb ./openanno
	else
		if [ "$1" == "--backtrace" ]; then
			echo "run" > backtrace.gdb
			echo "backtrace" >> backtrace.gdb
			echo "quit" >> backtrace.gdb
			gdb ./openanno < backtrace.gdb | tee backtrace.log
		else
			./openanno
		fi
	fi
else
	if `test -e Makefile`
	then
		echo "Couldn't find openanno. Will try to run make"
		CFLAGS="-g"
		CXXFLAGS="-g"
		make
	else
		echo "Couldn't find openanno. Will try to run cmake . && make"
		CFLAGS="-g"
		CXXFLAGS="-g"
		cmake . && make
	fi
	if `test -e ./openanno`
	then
		if [ "$1" == "--debugger" ]
		then
			gdb ./openanno
		else
			if [ "$1" == "--backtrace" ]; then
				echo "run" > backtrace.gdb
				echo "backtrace" >> backtrace.gdb
				echo "quit" >> backtrace.gdb
				gdb ./openanno < backtrace.gdb | tee backtrace.log
			else
				./openanno
			fi
		fi
	else
		echo "Couldn't make openanno"
	fi
	
fi
