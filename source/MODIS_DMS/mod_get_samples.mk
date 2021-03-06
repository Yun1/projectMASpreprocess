#############################################################################
# !make
#
# makefile name: makefile 
#
##!END
#############################################################################
CC = gcc 
#CFLAGS = -ansi -Wall -v -da -Q
CFLAGS = -Wall

TARGET = mod_get_samples.exe

#INC = -I. -I$(TIFFINC) -I$(GEOTIFF_INC)
#LIB =     -L$(GEOTIFF_LIB) -lgeotiff \
          -L$(TIFFLIB) -ltiff -lm\

INC = -I. -I$(GEOTIFF_INC)
LIB = -lgeotiff -ltiff -lm 

OBJ = get_samples.o sensor.o utility.o

all: $(TARGET)

$(OBJ) : modis.h

# Make the process
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB)  -o $(TARGET)

#
# Rules
#
.c.o: $(INC_FILES)
	$(CC) $(CFLAGS) $(ADD_CFLAGS) $(INC) -c $< -o $@

#******************* End of make file *******************************


