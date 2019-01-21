MKDIR = mkdir -p
DIRS = build/encode build/decode bin

all: dirs bin/decode bin/encode

bin/encode:
	$(MAKE) -f encode.make bin/encode

bin/decode:
	$(MAKE) -f decode.make bin/decode

clean:
	$(MAKE) -f haffmanLib.make clean
	$(MAKE) -f encode.make clean
	$(MAKE) -f decode.make clean


dirs:
	@echo "... Creating directories"
	$(MKDIR) $(DIRS)
