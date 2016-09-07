#
# my simple makefile act as something like a user interface
#

MODULES = $(shell ls -d */ | cut -f1 -d'/')
BUILD = libhelper libttenv ttenv_simple ttenv_gpio_simple ttenv_gpio ttenv_swt_led

all::
	@echo "  +----------------------------------------------------------+"
	@echo " /                                                          /|"
	@echo "+----------------------------------------------------------+ |"
	@echo "| Usage:                                                   | |"
	@echo "| make build     -> build everthing                        | |"
	@echo "|                                                          | |"
	@echo "| make install   -> build_all + install (driver + examples)| |"
	@echo "| make uninstall -> uninstall all                          | |"
	@echo "|                                                          | |"
	@echo "| make set_caps  -> set file capabilities (run as non-root)| |"
	@echo "|                                                          | |"
	@echo "| make get_external_repos -> get ALL git repos from below  | |"
	@echo "| make schedtool          -> schedtool                     | |"
	@echo "| make rt-app             -> rt-app                        | |"
	@echo "| make rt-tests           -> rt-tests                      | |"
	@echo "|                                                          | |"
	@echo "| make clean     -> clean all dir/subdirs                  | +"
	@echo "| make distclean -> complete cleanup                       |/ "
	@echo "+----------------------------------------------------------+  "

build::
	for dir in $(BUILD); do (cd $$dir && $(MAKE)); done

clean::
	rm -f *~ .*~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

distclean::
	rm -f *~ .*~
	for dir in $(MODULES); do (cd $$dir && $(MAKE) $@); done

install::
	for dir in $(BUILD); do (cd $$dir && $(MAKE) $@); done
	(./set_file_caps.sh)

uninstall::
	for dir in $(BUILD); do (cd $$dir && $(MAKE) $@); done

set_caps::
	@echo "Set caps to all binary"
	(./set_file_caps.sh)

get_external_repos::
	(./scripts/get_external_git_repos.sh -p "https")

get_schedtool:
	(./scripts/get_external_git_repos.sh -r "schedtool" -p "https")

get_rt-tests::
	(./scripts/get_external_git_repos.sh -r "rt-tests" -p "https")

get_rt-app::
	(./scripts/get_external_git_repos.sh -r "rt-app" -p "https")
