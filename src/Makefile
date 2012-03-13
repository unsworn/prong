SCRIPT_DIR=/Applications/Adobe\ Illustrator\ CS5/Presets.localized/en_GB/Scripts
INSTALL_DIR=/Users/nim/swlab/bin

DEPEND=tools export

all: $(DEPEND)

install: all
	@echo "Installing tools."
	@cp deskew/deskew $(INSTALL_DIR)
	@chmod u+x $(INSTALL_DIR)/deskew
	@echo "Installing scripts"
	@cp ExportTemplate.js $(SCRIPT_DIR)
	@echo "Done"

tools:
	@$(MAKE) -C deskew

export:
	@touch ExportTemplate.js

clean:
	@$(MAKE) -C deskew clean
