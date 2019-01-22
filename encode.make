include common.make

SOURCES = $(notdir $(wildcard FileEncoder/*.cpp))
OBJECTS = $(SOURCES:%.cpp=build/encode/%.o)

DEPS = $(OBJECTS:.o=.d)

all: bin/encode 

-include $(DEPS)

bin/encode: build/haffmanLib.a $(OBJECTS)
	@echo "Linking $@"
	$(CXX) $(OBJECTS) build/haffmanLib.a -o $@

build/encode/%.o: FileEncoder/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

build/haffmanLib.a:
	@echo "... Building haffmanLib.a"
	$(MAKE) -f haffmanLib.make build/haffmanLib.a
.PHONY : build/haffmanLib.a

clean:
	@echo "... Clean encode"
	@$(RM) bin/encode
	@$(RM) $(OBJECTS) $(DEPS)

.PHONY: clean
