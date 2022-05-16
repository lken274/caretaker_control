all: clean ct4ui/lib/libcaretaker.so
	@bash build_ct4ui.sh
	@echo
	@echo "`tput bold`Change directory to Software/apps/linux/ct4ui and run \"sudo ./run_ct4ui.sh\" to execute app.`tput sgr0`"
	@echo
ct4ui/lib/libcaretaker.so: ct4ui/lib/caretaker.h

ct4ui/lib/caretaker.h:
	@echo "Building libcaretaker dependencies..."
	@make -C ../../../libs/libcaretaker all

clean:
	@rm -rf build-*

