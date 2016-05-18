#############################################################################
#                                                                           #
#  Software    : TwitterTetris - GameEngine                                 #
#                                                                           #
#  Authors     : Gilles Steiner <gilles.steiner@gmail.com>                  #
#  Web site    : https://github.com/yrochat/tetris                          #
#                                                                           #
#############################################################################
.SUFFIXES: .h .cpp .o

.cpp.o:
	@echo ""
	@echo "-------------- $< ---------------"
	$(CXX) -c $(CXXFLAGS) $(VARFLAG) $< -o $@

sou_cpp = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#############################################################################
$(EXE): $(sou_cpp)
	$(LD) $(LDFLAGS) $(VARFLAG) -o $(EXE) $(sou_cpp) $(POSTLDFLAG)

$(sou_cpp): *.h

.PHONY: doc clean cleandoc

doc:
	doxygen Doxyfile > ../Doxygen.log

clean:
	rm  -f $(sou_cpp) *~ $(EXE)

cleandoc:
	rm -rf ../doc/*

all: $(EXE)
	cd .. ; make all
