# Create an auto-incrementing build number.

BUILD_NUMBER_LDFLAGS  = -defsym __BUILD_DATE=$$(date +'%Y%m%d')
BUILD_NUMBER_LDFLAGS += -defsym __BUILD_NUMBER=$$(cat $(BUILD_NUMBER_FILE))

build_number.target:
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)