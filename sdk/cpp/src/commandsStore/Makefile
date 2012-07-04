MESSAGE_BUILDER_H := $(IN_DIR)/commandsStore/MessageBuilder.h
MESSAGE_BUILDER_CPP := $(IN_DIR)/commandsStore/MessageBuilder.cpp

CLEANS += clean_message_builder
PREPARE += generate_message_builder

clean_message_builder:
	@rm -rf $(MESSAGE_BUILDER_H) $(MESSAGE_BUILDER_CPP)

generate_message_builder:
	python ./../python/generate.py $(MESSAGE_BUILDER_H) $(MESSAGE_BUILDER_CPP)
