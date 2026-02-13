PICO_PLATFORM=rp2040

BUILD_DIR=build/$(PICO_PLATFORM)

.PHONY: build
build:
	cmake -B $(BUILD_DIR) -G Ninja
	cmake --build $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: distclean
distclean:
	rm -rf build
	rm -f tags

.PHONY: tags
tags:
	ctags --exclude=build/* --exclude=tmp/* -R .
