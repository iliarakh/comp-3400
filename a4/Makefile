
CXXFLAGS=-std=c++23 -Wall -Wextra -Werror

all: a04-soln.exe

clean:
	rm -f *.exe

sample_inputs:
	cat input.dat
	cat input2.dat
	cat input3.dat
	cat input4.dat
	cat input5.dat
	cat input6.dat

run: a04-soln.exe
	./$< <input.dat
	@echo ""
	./$< <input2.dat
	@echo ""
	./$< <input3.dat
	@echo ""
	./$< <input4.dat || true
	@echo ""
	./$< <input5.dat
	@echo ""
	./$< <input6.dat || true

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
