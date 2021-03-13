CC = clang++
LLDB = -g
BOOST = -lboost_serialization
PYTHON = -lboost_python3
INCLUDE = -I/usr/local/lib `python3.6-config --includes`
LDFLAGS = `python3.6-config --ldflags`

hpylm:
	$(CC) -O3 -DPIC -shared -fPIC -o model.so src/model.cpp $(INCLUDE) $(LDFLAGS) $(PYTHON) $(BOOST)

test:
	$(CC) -O3 -DPIC -shared -fPIC -o test src/test.cpp $(LLDB) $(INCLUDE) $(LDFLAGS) $(PYTHON) $(BOOST)

clean:
	rm -f model.so test

.PHONY: clean
