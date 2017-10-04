#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := gatt_server_
mypath := $(shell pwd )
COMPONENT_ADD_INCLUDEDIRS := components/include 

include  $(mypath)/project_gatts.mk
#include $(IDF_PATH)/make/project_gatts.mk
