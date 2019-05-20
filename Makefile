#
#	make file
#

# === CHOSE CROSS COMPILATION (or not) ======
CROSS_CC = 0
# ===========================================

.SUFFIXES : .c .cpp .o 

# 	GNU C++ compiler
CC = g++
#CC = arm-none-eabi-g++

TARGET      = $(notdir $(shell pwd))	## current foldername is target name

BUILD_PATH 	= ./build

BIN_PATH 	= $(BUILD_PATH)/bin
OBJ_PATH 	= $(BUILD_PATH)/obj

SRC_PATH 	= .

SRCS		= $(wildcard $(SRC_PATH)/*.cpp)
OBJS 		= $(SRCS:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)

INCS	+= -I./include
INCS    += -I./lib/include
# 	Link Options
LIBS	+= -lpthread 
LIBS	+= -ludev
LIBS	+= -lv4l2  # video4linux2

LIBS	+= -lopencv_core
LIBS	+= -lopencv_highgui
LIBS	+= -lopencv_imgproc
LIBS	+= -lopencv_imgcodecs
LIBS 	+= -lopencv_aruco


ifeq ($(CROSS_CC), 0)
  CC    =   g++
  INCS	+= -I/usr/local/include/opencv4/
  LIBS	+= -L/usr/local/lib
  LIBS	+= -L/usr/lib/x86_64-linux-gnu 
endif
ifeq ($(CROSS_CC),1)
  CC    =   arm-linux-gnueabihf-g++
  LIBS	+= -L/lib/arm-linux-gnueabihf
  LIBS  += -L/home/jens/opencv/opencv/build-arm/lib
  LIBS	+= -L/usr/local/armhf/lib
  INCS	+= -I/usr/local/armhf/include/opencv4/
endif
	
# OpenCV default
OPENCV_CFLAGS  = #$(shell pkg-config --cflags opencv)
OPENCV_LIBS	   =# $(shell pkg-config --libs opencv)

#	set LDFLAGS
LDFLAGS     = $(LIBS) $(OPENCV_LIBS)

# 	set CFLAGS
#CXXFLAGS = $(INCS) $(OPENCV_CFLAGS) -c -std=c++11 -O3 -W -Wfatal-errors # -Wall
CXXFLAGS = $(INCS) $(OPENCV_CFLAGS) -c -std=c++11 -static -g  -fPIC -W -Wfatal-errors #-Wall

#	rm options
RM 			= @rm -rfv

# 	mkdir options
MKDIR 		= @mkdir -p

$(BIN_PATH)/$(TARGET): $(OBJS)
	$(MKDIR) $(BIN_PATH)
	$(CC) -o $(BIN_PATH)/$(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(MKDIR) $(OBJ_PATH)
	$(CC) $(CXXFLAGS) $< -o $@


all : $(BIN_PATH)/$(TARGET)

#	dependency
dep :
	$(MKDIR) $(BUILD_PATH)
	$(CC) -M $(INCS) $(SRCS) > $(BUILD_PATH)/.depend

#	clean
clean:
	$(RM) $(BUILD_PATH)
	$(RM) $(TARGET)
	@echo "Done."

#	include dependency
ifeq (.depend,$(wildcard .depend))
include .depend
endif
