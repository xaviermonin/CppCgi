LDFLAGS=-lmysqlclient
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
VERSION=1

all: $(CGI_LIB)

$(CGI_LIB): $(OBJ)
	@$(CPP) -shared -Wl,-soname,lib$(CGI_LIB).so.$(VERSION) -o lib$(CGI_LIB).so.1 $^ $(LDFLAGS) && \
	ln -fs lib$(CGI_LIB).so.$(VERSION) lib$(CGI_LIB).so

%.o: %.cpp
	@$(CPP) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf lib$(CGI_LIB).so*

