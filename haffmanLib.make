include common.make

all: build/haffmanLib.a

SOURCES = $(notdir $(wildcard HaffmanLib/*.cpp))
OBJECTS = $(SOURCES:%.cpp=build/%.o)
DEPS = $(OBJECTS:.o=.d)

-include $(DEPS)

build/%.o: HaffmanLib/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/haffmanLib.a: $(OBJECTS)
	@echo "Linking $@"
	$(LNK) $(LNK_FLAGS) $@ $(OBJECTS)

clean:
	@echo "... Clean haffmanLib.a"
	@$(RM) build/haffmanLib.a $(OBJECTS)
