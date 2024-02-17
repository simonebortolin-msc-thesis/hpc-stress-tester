mpicc -O3 -march=haswell  basic_add_avx2.c -o avx
mpicc -O3 -march=haswell  basic_maddubs_epi.c -o sse

Based on https://github.com/VictorRodriguez/AVX-SG/

