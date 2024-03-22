MPICXX=mpicxx

EXECS=p1_hello p2_sum p3_deriv p4_matmul p5_gauss

CCFLAGS=-std=c++11 -Ofast -march=native


all: $(EXECS)


$(EXECS): %: %.cpp
	$(MPICXX) $(CCFLAGS) -o $@ $^

clean:
	rm $(EXECS)
