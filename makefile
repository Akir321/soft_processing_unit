CXX = g++
CXX_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline\
 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op\
 -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual\
 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG\
 -D_EJUDGE_CLIENT_SIDE

SRC_DIR  = source/
OBJ_DIR  = object/
DMP_DIR  = gr_dump/
LOG_DIR  = log/

INCLUDES  = $(SRC_DIR)cmd_enum.h     \
			$(SRC_DIR)commands.h     \
			$(SRC_DIR)errors.h       \
			$(SRC_DIR)html_logfile.h 

INC_SPU   = $(SRC_DIR)spu.h   \
			$(SRC_DIR)stack.h

INC_ASSM  = $(SRC_DIR)io.h       \
			$(SRC_DIR)assembler.h 

INC_DASSM = $(SRC_DIR)disassembler.h

OBJ_SPU   = $(OBJ_DIR)spu.o         \
			$(OBJ_DIR)main_spu.o    \
			$(OBJ_DIR)stack.o       \
			$(OBJ_DIR)html_logfile.o

OBJ_ASSM  = $(OBJ_DIR)assembler.o      \
			$(OBJ_DIR)main_assembler.o \
			$(OBJ_DIR)io.o             \
			$(OBJ_DIR)html_logfile.o

OBJ_DASSM = $(OBJ_DIR)disassembler.o      \
			$(OBJ_DIR)main_disassembler.o \
			$(OBJ_DIR)html_logfile.o
			



all: spu

spu: $(OBJ_SPU) 
	$(CXX) $(OBJ_SPU) -o $@ $(CXX_FLAGS)

assm: $(OBJ_ASSM)
	$(CXX) $(OBJ_ASSM) -o assembler $(CXX_FLAGS)

disassm: $(OBJ_DASSM)
	$(CXX) $(OBJ_DASSM) -o disassembler $(CXX_FLAGS)


$(OBJ_DIR)main_spu.o:          $(SRC_DIR)main_spu.cpp              $(INCLUDES)       $(INC_SPU)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)main_assembler.o:    $(SRC_DIR)main_assembler.cpp        $(INCLUDES)       $(INC_ASSM)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)main_disassembler.o: $(SRC_DIR)main_disassembler.cpp     $(INCLUDES)       $(INC_DASSM)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)spu.o:               $(SRC_DIR)spu.cpp                   $(INCLUDES)       $(INC_SPU)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)assembler.o:         $(SRC_DIR)assembler.cpp             $(INCLUDES)       $(INC_ASSM)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)disassembler.o:      $(SRC_DIR)disassembler.cpp          $(INCLUDES)       $(INC_DASSM)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)stack.o:             $(SRC_DIR)stack.cpp                 $(SRC_DIR)stack.h $(SRC_DIR)html_logfile.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)io.o:                $(SRC_DIR)io.cpp                    $(SRC_DIR)io.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(OBJ_DIR)html_logfile.o:      $(SRC_DIR)html_logfile.cpp          $(SRC_DIR)html_logfile.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)




.PHONY: clean clean_dumps clean_logs

clean:
	rm $(OBJECTS) *.exe 
	
clean_dumps:
	rm $(DUMPS)

clean_logs:
	rm $(LOG_DIR)*.html
