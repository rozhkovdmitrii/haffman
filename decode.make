include common.make

SOURCES = $(notdir $(wildcard FileDecoder/*.cpp))
OBJECTS = $(SOURCES:%.cpp=build/decode/%.o)

DEPS = $(OBJECTS:.o=.d)

all: bin/decode

-include $(DEPS)

bin/decode: build/haffmanLib.a $(OBJECTS)
	@echo "Linking $@"
	$(CXX) $(OBJECTS) build/haffmanLib.a -o $@

build/decode/%.o: FileDecoder/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

build/haffmanLib.a:
	@echo "... Building haffmanLib.a"
	$(MAKE) -f haffmanLib.make build/haffmanLib.a
.PHONY : build/haffmanLib.a

clean:
	@echo "... Clean decode"
	@$(RM) bin/decode
	@$(RM) $(OBJECTS)
.PHONY : clean
