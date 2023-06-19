# SSP
Speech siginal process alg.
This is repo for some ssp alg demo.

#### 启动
qemu-riscv64 -cpu c906fdv main /iothome/zhaoxy/code/asr_sdk/build/guanbichuanglian.wav

qemu-riscv64 -cpu c906fdv main guanbichuanglian.wav 
qemu-riscv64 -cpu c906fdv example -i ../input.raw ../input2.raw -r ../ref.raw ../ref2.raw -o ../out.pcm

```
#### 调试
```
qemu-riscv64 -cpu c906fdv -g 1024 main guanbichuanglian.wav 
riscv64-unknown-linux-gnu-gdb main
target remote localhost:1024