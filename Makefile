all: experiment
experiment: experiment.o agent.o environment.o box.o
	$(CXX)  experiment.o agent.o environment.o box.o  $(LDFLAGS) -lm -lrlglue -lrlutils -o experiment -lBox2D -g
experiment.o: experiment.cc
	$(CXX)  $(CFLAGS) -c experiment.cc -o experiment.o
agent.o: agent.cc
	$(CXX)  $(CFLAGS) -c agent.cc -o agent.o
environment.o: environment.cc
	$(CXX)  $(CFLAGS) -c environment.cc -o environment.o
box.o: box.cc
	$(CXX)	$(CFLAGS) -c box.cc -o box.o
clean:
	rm -rf experiment experiment.exe *.o result.csv results.dat valuefuncPath
