## Задача 1. Потоки (процессы)

### Лог

```
$ mpirun -n 4 p1_hello
Hello from processor ubuntu, rank 3 out of 4
Hello from processor ubuntu, rank 2 out of 4
Hello from processor ubuntu, rank 0 out of 4
Hello from processor ubuntu, rank 1 out of 4
```

## Задача 2. Сумма чисел

### Лог

```
$ mpirun -n 1 p2_sum 10
result      354 took 0.000006s
$ mpirun -n 2 p2_sum 10
result      354 took 0.000016s

$ mpirun -n 1 p2_sum 1000
result    -9666 took 0.000006s
$ mpirun -n 2 p2_sum 1000
result    -9666 took 0.000075s

$ mpirun -n 1 p2_sum 10000000
result -1149040 took 0.005656s
$ mpirun -n 2 p2_sum 10000000
result -1149040 took 0.005722s

$ mpirun -n 1 p2_sum 1000000000
result  12037951 took 0.561324s
$ mpirun -n 2 p2_sum 1000000000
result  12037951 took 0.547573s

$ mpirun -n 1 p2_sum 10000000000
result  10221467 took 1.192339s
$ mpirun -n 2 p2_sum 10000000000
result  10221467 took 0.765406s
```

### В виде таблицы:

| N           | Процессы | Время (сек) |
| ----------- | -------- | ----------- |
| 10          | 1        | 0.000006    | 
| 1000        | 1        | 0.000006    | 
| 10000000    | 1        | 0.005656    | 
| 1000000000  | 1        | 0.561324    | 
| 10000000000 | 1        | 1.192339    | 
| 10          | 2        | 0.000016    | 
| 1000        | 2        | 0.000075    | 
| 10000000    | 2        | 0.005722    | 
| 1000000000  | 2        | 0.547573    | 
| 10000000000 | 2        | 0.765406    | 


## Задача 3. Частная производная функции, заданной на сетке

### Лог

```
$ mpirun -n 1 p3_deriv 1000
took 0.000781s
$ mpirun -n 2 p3_deriv 1000
took 0.000924s

$ mpirun -n 1 p3_deriv 10000
took 0.112738s
$ mpirun -n 2 p3_deriv 10000
took 0.126219s

$ mpirun -n 1 p3_deriv 20000
took 0.463885s
$ mpirun -n 2 p3_deriv 20000
took 0.472845s
```

### В виде таблицы:

| N       | Процессы | Время (сек) |
| ------- | -------- | ----------- |
| 1000    | 1        | 0.000781    | 
| 10000   | 1        | 0.112738    | 
| 20000   | 1        | 0.463885    | 
| 1000    | 2        | 0.000924    | 
| 10000   | 2        | 0.126219    | 
| 20000   | 2        | 0.472845    | 



## Задача 4. Произведение двух матриц

### Лог

```
$ mpirun -n 1 p4_matmul 512
trace -229.1460 took 0.007862s
$ mpirun -n 2 p4_matmul 512
trace -229.1460 took 0.003709s
$ mpirun -n 4 p4_matmul 512
trace -229.1460 took 0.004140s

$ mpirun -n 1 p4_matmul 1024
trace -297.4893 took 0.070104s
$ mpirun -n 2 p4_matmul 1024
trace -297.4893 took 0.036013s
$ mpirun -n 4 p4_matmul 1024
trace -297.4893 took 0.031262s

$ mpirun -n 1 p4_matmul 2048
trace  121.0394 took 0.591009s
$ mpirun -n 2 p4_matmul 2048
trace  121.0394 took 0.315137s
$ mpirun -n 4 p4_matmul 2048
trace  121.0394 took 0.191180s

$ mpirun -n 1 p4_matmul 4096
trace -1705.1299 took 5.292815s
$ mpirun -n 2 p4_matmul 4096
trace -1705.1299 took 2.617982s
$ mpirun -n 4 p4_matmul 4096
trace -1705.1299 took 1.827324s
```

### В виде таблицы:

| N       | Процессы | Время (сек) |
| ------- | -------- | ----------- |
| 512     | 1        | 0.007862    | 
| 1024    | 1        | 0.070104    | 
| 2048    | 1        | 0.591009    | 
| 4096    | 1        | 5.292815    | 
| 512     | 2        | 0.003709    | 
| 1024    | 2        | 0.036013    | 
| 2048    | 2        | 0.315137    | 
| 4096    | 2        | 2.617982    | 
| 512     | 4        | 0.004140    | 
| 1024    | 4        | 0.031262    | 
| 2048    | 4        | 0.191180    | 
| 4096    | 4        | 1.827324    | 



## Задача 5. Метод Гаусса

### Лог
```
$ mpirun -n 1 p5_gauss 512
sum  639.874695 took 0.006413s
$ mpirun -n 1 p5_gauss 1024
sum  1279.875366 took 0.059284s
$ mpirun -n 1 p5_gauss 2048
sum  2559.874756 took 0.467746s
$ mpirun -n 1 p5_gauss 4096
sum  5119.882324 took 5.012211s

$ mpirun -n 2 p5_gauss 512
sum  639.874695 took 0.002791s
$ mpirun -n 2 p5_gauss 1024
sum  1279.875366 took 0.039916s
$ mpirun -n 2 p5_gauss 2048
sum  2559.874756 took 0.323846s
$ mpirun -n 2 p5_gauss 4096
sum  5119.882324 took 4.172616s

$ mpirun -n 4 p5_gauss 512
sum  639.874695 took 0.002981s
$ mpirun -n 4 p5_gauss 1024
sum  1279.875366 took 0.021491s
$ mpirun -n 4 p5_gauss 2048
sum  2559.874756 took 0.213488s
$ mpirun -n 4 p5_gauss 4096
sum  5119.882324 took 3.594501s
```

### В виде таблицы:

| N       | Процессы | Время (сек) |
| ------- | -------- | ----------- |
| 512     | 1        | 0.006413    | 
| 1024    | 1        | 0.059284    | 
| 2048    | 1        | 0.467746    | 
| 4096    | 1        | 5.012211    | 
| 512     | 2        | 0.002791    | 
| 1024    | 2        | 0.039916    | 
| 2048    | 2        | 0.323846    | 
| 4096    | 2        | 4.172616    | 
| 512     | 4        | 0.002981    | 
| 1024    | 4        | 0.021491    | 
| 2048    | 4        | 0.213488    | 
| 4096    | 4        | 3.594501    | 
