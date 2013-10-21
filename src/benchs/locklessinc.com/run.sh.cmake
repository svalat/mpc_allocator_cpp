#!/bin/bash
######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#set -x
set -e

######################################################
#config
TOTAL_ACTIONS='100'
#I_MAX='10000'
I_MAX='500'
LOCKLESS_SIZES='64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152'
ALLOCATORS="std: MPCAllocator:@MPC_ALLOC_PATH_FOR_BENCH@"
THREADS='1 2 4 8'

######################################################
if which likwid-bin 1>/dev/null 2>/dev/null; then
	PINTHREADS="likwid-pin -q"
else
	PINTHREADS=''
fi

######################################################
function print_header()
{
	echo "######################################################"
	echo "# Benchmark : $1"
	echo "# Date      : $(date)"
	echo "# Hostname  : $(hostname)"
	echo "# CPU       : $(uname -p)"
	echo "# Kernel    : $(uname -s -r -v -m)"
	echo "# GCC       : $(gcc --version | head -n 1)"
	echo "# Repos     : $(git rev-parse HEAD)"
	echo "######################################################"
}

######################################################
#run_bench {ALLOC} {NB_THREAD} {BLOC SIZE} [-usemem]
function run_bench()
{
	alloc_name=$(echo "$1" | cut -f 1 -d ':')
	alloc_lib=$(echo "$1" | cut -f 2 -d ':')
	#total=10000 threads=4 i_max=10000 size=10000 bins=1677
	LD_PRELOAD="${alloc_lib}:${LD_PRELOAD}" /usr/bin/time --format="DATA	${alloc_name}	$2	$3	%e	%U	%S %M" ${PINTHREADS} ./t-test1${4} ${TOTAL_ACTIONS} "$2" "${I_MAX}" "$3" 2>&1
}

######################################################
function run_all()
{
	for alloc in ${ALLOCATORS}
	do
		for thr in ${THREADS}
		do
			for size in ${LOCKLESS_SIZES}
			do
				run_bench "$alloc" "$thr" "$size" "$1" | grep '^DATA' | sed -e 's/^DATA	//'
			done
			sleep 1
		done
	done
}

######################################################
function run()
{
	fname="$1"
	print_header "locklessinc.com"
	echo ""
	echo "#fname          : $fname"
	echo "#I_MAX          : $I_MAX"
	echo "#TOTAL_ACTIONS  : $TOTAL_ACTIONS"
	echo "#PINTHREADS     : $PINTHREADS"
	echo "#ALLOCATORS     : $ALLOCATORS"
	echo "#THREADS        : $THREADS"
	echo ""
	echo "#Benchmark	threads	bloc_size	runtime	usertime	systime	max_mm"
	echo ""
	run_all "$2"
}

######################################################
function gen_gnuplot_per_bloc_size()
{
	echo '####################### GLOBAL #####################'
	echo 'reset'
	echo 'set xlabel "Bloc size (byte)"'
	echo 'set ylabel "Time (s)"'
	echo 'set grid'
	echo 'set logscale x'
	echo "$2"

	for thr in ${THREADS}
	do
		echo "####################### $thr THREAD ###################"
		echo "set title 't-test1 ${thr} thread'"

		sep=''
		printf "plot "
		for alloc in ${ALLOCATORS}
		do
			name=$(echo $alloc | cut -f 1 -d ':')
			printf " ${sep} \"< awk '{if (\$2 == $thr && \$1 ~ /${name}/) print}' $1\" u 3:4 w lp title \"${name}\" "
			sep=','
		done
		echo
	done
}

######################################################
function gen_gnuplot_per_alloc()
{
	echo '####################### GLOBAL #####################'
	echo 'reset'
	echo 'set xlabel "Bloc size (byte)"'
	echo 'set ylabel "Time (s)"'
	echo 'set grid'
	echo 'set logscale x'
	echo "$2"

	
	for alloc in ${ALLOCATORS}
	do
		name=$(echo $alloc | cut -f 1 -d ':')
		echo "####################### $name #####################"
		echo "set title 't-test1 $name'"
		sep=''
		printf "plot "
		for thr in ${THREADS}
		do
			printf " ${sep} \"< awk '{if (\$2 == $thr && \$1 ~ /${name}/) print}' $1\" u 3:4 w lp title \"${thr} threads\" "
			sep=','
		done
		echo
	done
}

######################################################
function gen_gnuplot_scala()
{
	echo '####################### GLOBAL #####################'
	echo 'reset'
	echo 'set xlabel "Threads"'
	echo 'set ylabel "Time (s)"'
	echo 'set grid'
	echo 'set logscale y'
# 	echo 'set xrange [0:20]'

	
	for s in ${LOCKLESS_SIZES}
	do
		echo "####################### $name #####################"
		echo "set title '$s'"
		sep=''
		printf "plot "
		for alloc in ${ALLOCATORS}
		do
			name=$(echo $alloc | cut -f 1 -d ':')
			printf " ${sep} \"< awk '{if (\$3 == $s && \$1 ~ /${name}/) print}' $1\" u 2:4 w lp title \"${name}\" "
			sep=','
		done
		echo
	done
}

######################################################
function plot_all()
{
	gen_gnuplot_per_bloc_size "results-locklessinc${1}.txt" > "results_per_bloc_size${1}.gnuplot"
	gnuplot -e "set term pdf; set output 'results_per_bloc_size${1}.pdf';" "results_per_bloc_size${1}.gnuplot" || echo "Failed to generate PDF"
	
	gen_gnuplot_per_bloc_size "results-locklessinc${1}.txt" 'set xrange [:10000]' > "results_per_bloc_size_max_10000${1}.gnuplot"
	gnuplot -e "set term pdf; set output 'results_per_bloc_size_max_10000${1}.pdf';" "results_per_bloc_size_max_10000${1}.gnuplot" || echo "Failed to generate PDF"
	
	gen_gnuplot_per_alloc "results-locklessinc${1}.txt" > "results_per_alloc${1}.gnuplot"
	gnuplot -e "set term pdf; set output 'results_per_alloc${1}.pdf';" "results_per_alloc${1}.gnuplot" || echo "Failed to generate PDF"
	
	gen_gnuplot_scala "results-locklessinc${1}.txt" > "results_scala${1}.gnuplot"
	gnuplot -e "set term pdf; set output 'results_scala${1}.pdf';" "results_scala${1}.gnuplot" || echo "Failed to generate PDF"
}

######################################################
#main
run 'results-locklessinc.txt' '-no-use-mem' | tee 'results-locklessinc.txt'
run 'results-locklessinc-usemem.txt' '-use-mem' | tee 'results-locklessinc-usemem.txt'
plot_all '-no-use-mem'
plot_all '-use-mem'
