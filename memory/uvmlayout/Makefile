VERSION=0
RELEASE=1

CC=gcc

TOP_DIR=$(PWD)

DOCS=docs
SOURCE=source
MAKEFILES=Makefile
TOOLS=tools
BIN=bin

PRODUCT=uvmlayout
TARGET_RELEASE=uvmlayout-$(VERSION).$(RELEASE)

TOOLS_DIR=tools
BIN_DIR=bin
SHIP_DIR=ship
SRC_DIR=$(SOURCE)/src
INC_DIR=$(SOURCE)/include

SRC_FILES=vmlayout_main.c

VPATH :=$(SRC_DIR) $(INC_DIR)

BUNDLE_FILES := $(SOURCE) $(DOCS) $(MAKEFILES) $(TOOLS)

all: $(BIN_DIR)/$(PRODUCT);

$(BIN_DIR)/$(PRODUCT): $(SRC_FILES)
  $(CC) -o $@ -I$(INC_DIR) $<

clean:
  rm -rf $(BIN_DIR)/$(PRODUCT)
  rm -rf $(SHIP_DIR)/$(TARGET_RELEASE).tar

ship_bundle:
  rm -rf $(SHIP_DIR)/$(TARGET_RELEASE).tar;
  rm -rf $(SHIP_DIR)/$(TARGET_RELEASE).tar.gz;
  tar -cvf $(SHIP_DIR)/$(TARGET_RELEASE).tar ../$(TARGET_RELEASE)
  gzip $(SHIP_DIR)/$(TARGET_RELEASE).tar
