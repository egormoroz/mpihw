## Компиляция

```
mkdir build
cd build
cmake ../src
cmake --build . --config Release
```

## 1. Потоки

### Описание
Запуск указанного числа потоков и вывод номера каждого.

Аргументы: `1.exe <кол-во потоков>`.

Запуск:`.\Release\1.exe 16`.

### Вывод

```
hello from thread 2 / 16
hello from thread 4 / 16
hello from thread 5 / 16
hello from thread 1 / 16
hello from thread 6 / 16
hello from thread 8 / 16
hello from thread 3 / 16
hello from thread 9 / 16
hello from thread 11 / 16
hello from thread 12 / 16
hello from thread 13 / 16
hello from thread 14 / 16
hello from thread 15 / 16
hello from thread 16 / 16
hello from thread 7 / 16
hello from thread 10 / 16
all threads finished working
```

## 2. Сумма чисел

### Описание

Создание массива размера `final_size` и замер времени исполнения для суммирования чисел подмассивов
длины `len_i = min(mult_step^i * init_size, final_size), i = 1, 2, ...`.

Аргументы: `2.exe <init_size> <final_size> <mult_step>`. 

Запуск:`.\Release\2.exe 100 1000000000 10`.

### Вывод

Время растёт линейно с количеством чисел.

```
generating random array with 1000000000 elements...
size 100 time 0.000ms
size 1000 time 0.004ms
size 10000 time 0.035ms
size 100000 time 0.037ms
size 1000000 time 0.344ms
size 10000000 time 4.951ms
size 100000000 time 36.287ms
size 1000000000 time 339.579ms
```

## 3. Частная производная функции, заданной на сетке

### Описание

Для функции `f(x, y)`, заданной на сетке `n_i x n_i`, вычислить частные производные `df/dy` методом центральных разностей: `df(x_i, y_i)/dy ~= (df(x_i, y_{i+1) - df(x_i, y_{i-1})) / 2`.

Аргументы: `3.exe <init_size> <final_size> <mult_step>`.

Запуск: `.\Release\3.exe 10 20000 2`

### Вывод

Время зависит квадратично от размера таблицы (или линейно с кол-вом элементов).

```
size 10 time 0.000ms
size 20 time 0.000ms
size 40 time 0.001ms
size 80 time 0.007ms
size 160 time 0.493ms
size 320 time 1.041ms
size 640 time 2.179ms
size 1280 time 7.901ms
size 2560 time 5.950ms
size 5120 time 19.773ms
size 10240 time 73.206ms
size 20000 time 307.234ms
```

## 4. Произведение двух матриц


### Описание

Создание двух квадратных матриц размера `final_size` и замер времени исполнения 
для умножения подматриц размера `n_i = min(mult_step^i * init_size, final_size), i = 1, 2, ...`.

Аргументы: `4.exe <init_size> <final_size> <mult_step>`.

Запуск: `.\Release\4.exe 10 2560 2`.

Примечание: мой процессор имеет суммарно ~25мб кэша, в который вмещаются полностью две квадратные матрицы типа float размера 1280: 1280^2 * 4 * 2 байт равно 12.5 мегабайт. Две матрицы размера 2560 уже занимают вместе 50МБ и потому не помещаются полностью в кэше (учитывая, что надо где-то хранить результат произведения C).

### Вывод

Матрица B записана по столбцам (column major order), либо заранее транспонирована. Примерно кубическая зависимость времени от размера (размер x2, время x8).

```
size 10 time 0.001ms
size 20 time 0.005ms
size 40 time 0.045ms
size 80 time 0.123ms
size 160 time 1.895ms
size 320 time 11.840ms
size 640 time 116.349ms
size 1280 time 913.791ms
size 2560 time 8142.543ms
```

Матрица B записана по строкам (row major order). Примерно кубическая зависимость времени от размера (размер x2, время x8) для size <= 1280. На последнем шаге время увеличилось в ~39 раз.

```
size 10 time 0.001ms
size 20 time 0.018ms
size 40 time 0.016ms
size 80 time 0.764ms
size 160 time 1.260ms
size 320 time 16.018ms
size 640 time 143.516ms
size 1280 time 2090.220ms
size 2560 time 82031.648ms
```

## 5. Метод Гаусса для СЛАУ

Создание квадратной матрицы A размера `final_size` с диагональным преобладанием и вектора-столбца b. 
Замер времени исполнения метода гаусса для решения СЛАУ `A_i x = b_i` с `n_i = min(mult_step^i * init_size, final_size), i = 1, 2, ...` переменных.

### Описание

Аргументы: `5.exe <init_size> <final_size> <mult_step>`.

Запуск: `.\Release\5.exe 10 10000 2`.

Примечание: аналогичная ситуация как в умножении матриц, только здесь матрица одна вместо трёх, поэтому можно вместить больший размер.

### Вывод

```
size 10 time 0.000ms
size 20 time 0.010ms 
size 40 time 0.060ms 
size 80 time 0.439ms 
size 160 time 0.600ms
size 320 time 4.678ms
size 640 time 38.912ms
size 1280 time 310.091ms
size 2560 time 2634.607ms
size 5120 time 23998.877ms
size 10000 time 203329.188ms
```

