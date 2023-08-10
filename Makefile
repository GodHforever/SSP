all: example_riscv_linux


example_x86:
	mkdir -p example_x86_build;
	cd example_x86_build; cmake -D SET_DEBUG=1 -DCMAKE_BUILD_TYPE=Debug ..; make -j8
example_riscv_linux:
	mkdir -p example_riscv_linux_build;
	cd example_riscv_linux_build; cmake -D C906=1 -D LINUX=1 -D CONFIG_KWS=1 -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_kws.cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j8
example_riscv_rtos:
	mkdir -p example_riscv_rtos_build;
	cd example_riscv_rtos_build; cmake -D E907=1 -D RTOS=1 -D CONFIG_KWS=1 -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_kws.cmake -DCMAKE_BUILD_TYPE=Release ..; make -j8
example_x86_exe:
	mkdir -p example_x86_build;
	cd example_x86_build; cmake -D CONFIG_AEC_EXE=1 -D SET_DEBUG=1 -DCMAKE_BUILD_TYPE=Debug ..; make -j8
clean:
	rm -rf example_riscv_linux_build;
