MKDIR = mkdir -p
DIRS = build/encode build/decode bin

all: dirs bin/decode bin/encode


bin/encode:
	$(MAKE) -f encode.make bin/encode
.PHONY : bin/encode

bin/decode:
	$(MAKE) -f decode.make bin/decode
.PHONY : bin/decode

clean:
	$(MAKE) -f haffmanLib.make clean
	$(MAKE) -f encode.make clean
	$(MAKE) -f decode.make clean
.PHONY : clean

dirs:
	@echo "... Creating directories"
	$(MKDIR) $(DIRS)
