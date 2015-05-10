all: test

parser_base.py: broadway.ebnf
	grako -m Broadway -o parser_base.py broadway.ebnf

test: parser_base.py

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

.PHONY: run
run : parser_base.py
	python broadway_parser.py $(RUN_ARGS) --trace

