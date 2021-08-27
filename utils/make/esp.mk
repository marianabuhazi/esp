# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

ESP_DEFCONFIG ?= $(ESP_ROOT)/socs/defconfig/esp_$(BOARD)_defconfig

$(ESP_CFG_BUILD):
	$(QUIET_MKDIR)mkdir -p $(ESP_CFG_BUILD)

$(ESP_CFG_BUILD)/.esp_config:
	$(QUIET_MKDIR)mkdir -p $(ESP_CFG_BUILD)
	$(QUIET_CP)cp $(ESP_DEFCONFIG) $@

esp-defconfig: $(ESP_DEFCONFIG) $(ESP_CFG_BUILD)
	$(QUIET_CP) \
	cd $(ESP_CFG_BUILD); \
	cp $< .esp_config
	$(QUIET_MAKE)$(MAKE) esp-config

$(ESP_CFG_BUILD)/socmap.vhd: $(ESP_CFG_BUILD)/.esp_config $(GRLIB_CFG_BUILD)/grlib_config.vhd top.vhd Makefile
	$(QUIET_DIFF)echo "checking .esp_config..."
	@cd $(ESP_CFG_BUILD); \
	/usr/bin/diff .esp_config $(ESP_DEFCONFIG) -q > /dev/null; \
	if test $$? = "0"; then \
		echo $(SPACES)"INFO Using default configuration file for ESP"; \
	else \
		echo $(SPACES)"INFO Using custom configuration found in \".esp_config\" for ESP"; \
	fi; \
	echo ""; \
	echo "Generating ESP configuration..."; \
	LD_LIBRARY_PATH="" xvfb-run -a python3 $(ESP_ROOT)/tools/socgen/esp_creator_batch.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)

$(ESP_CFG_BUILD)/socmap.h: $(ESP_CFG_BUILD)/socmap.vhd

ESPLINK_SRCS = $(wildcard $(ESP_ROOT)/tools/esplink/src/*.c)
ESPLINK_HDRS = $(wildcard $(ESP_ROOT)/tools/esplink/src/*.h)
esplink: $(ESP_CFG_BUILD)/socmap.h $(ESPLINK_HDRS) $(ESPLINK_SRCS)
	$(QUIET_CC) \
	cd $(ESP_CFG_BUILD); \
	gcc -O3 -Wall -Werror -fmax-errors=5 \
		-DESPLINK_IP=\"$(ESPLINK_IP)\" -DPORT=$(ESPLINK_PORT) \
		-I$(ESP_ROOT)/tools/esplink/src/ -I$(DESIGN_PATH)/$(ESP_CFG_BUILD) \
		$(ESPLINK_SRCS) -o $@

NEWSOCMAP_DEPS  = $(ESP_ROOT)/tools/newsocgen/main.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/tile.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/tile.h
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/espcreator.ui
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/espcreator.h
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/address_map.h
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/power_info.h
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/socmap_utils.h
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/espcreator.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/socmap_utils.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/address_map.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/power_info.cpp
NEWSOCMAP_DEPS += $(ESP_ROOT)/tools/newsocgen/constants.h

esp_constants.h: $(ESP_ROOT)/utils/esp_constants.inc .grlib_config
	$(QUIET_BUILD)
	@echo "#ifndef ESP_CONSTANTS_H" > $@
	@echo "" >> $@
	@echo "#define ESP_CONSTANTS_H" >> $@
	@cat $< | sed 's/#/\/\//g' | sed 's/\./#define /g' | sed 's/=/ /g' | sed 's/\(#define.*\)$$/\1ULL/g' >> $@
	@echo "" >> $@
	@echo "// GRLIB" >> $@
	@cat .grlib_config | grep 'CONFIG_SVGA_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_SVGA_ENABLE 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_GRETH_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_GRETH_ENABLE 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_DSU_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_DSU_ENABLE 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_DSU_ETH' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_DSU_ETH 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_DSU_JTAG' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_DSU_JTAG 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_UART1_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_UART_ENABLE 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_GPT_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_GPT_ENABLE 1' >> $@; \
	fi;
	@cat .grlib_config | grep 'CONFIG_IRQ3_ENABLE' | grep -q -v '#'; \
	if [ $$? -eq 0 ]; then \
	    echo '#define CFG_GRLIB_IRQ3_ENABLE 1' >> $@; \
	fi;
	@echo "" >> $@
	@echo "#endif /* ESP_CONSTANTS_H */" >> $@

#POWER_ESTIMATION_RTPS=$(shell find $(ESP_ROOT)/tech/$(POW_TECHLIB)/acc -name "power*.rpt")

#esp_power_db.cpp: $(ESP_ROOT)/utils/scripts/update_power_db.py $(POWER_ESTIMATION_RTPS)
#	$(QUIET_BUILD) $< $(ESP_ROOT)/tech/$(POW_TECHLIB) > $@


newsocmap.mk: $(ESP_ROOT)/tools/newsocgen/socmap.pro
	DESIGN_PATH=$(DESIGN_PATH) 			\
	TECH_PATH=$(ESP_ROOT)/tech/$(TECHLIB) 		\
	POW_TECH_PATH=$(ESP_ROOT)/tech/$(POW_TECHLIB) 	\
	TECH=$(TECHLIB) 				\
	BOARD=$(BOARD) 					\
	CPU_ARCH=$(CPU_ARCH) 				\
	DMA_WIDTH=$(NOC_WIDTH)				\
	qmake-qt5 -o $@ $<

#esp_power_db.cpp
newsocmap: newsocmap.mk $(NEWSOCMAP_DEPS) esp_constants.h
	$(QUIET_MAKE) make --quiet -f $<

newsocmap-run: newsocmap $(GRLIB_CFG_BUILD)/grlib_config.vhd
	$(QUIET_RUN)./$< $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC)

newsocmap-clean:
	$(QUIET_CLEAN) $(RM) 		\
		espcreator.o		\
		main.o			\
		tile.o			\
		address_map.o		\
		socmap_utils.o		\
		newsocmap.mk		\
		moc_*			\
		ui_espcreator.h

newsocmap-distclean: newsocmap-clean
	$(QUIET_CLEAN) $(RM) newsocmap

.PHONY: newsocmap-clean newsocmap-distclean newsocmap-run

esp-config: $(ESP_CFG_BUILD)/socmap.vhd

esp-xconfig: $(ESP_CFG_BUILD) $(GRLIB_CFG_BUILD)/grlib_config.vhd
	@echo ""
	@echo "Running interactive ESP configuration..."
	@cd $(ESP_CFG_BUILD); \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/esp_creator.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_power.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_power.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_mmi64.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_mmi64.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_cache_config_svh.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_cache_config_svh.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_esp_global_vhd.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_esp_global_vhd.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_riscv_dts.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_riscv_dts.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_S64esp.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_S64esp.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_socmap_h.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_socmap_h.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK); \
	echo "Running generate_socmap_vhd.py..."; \
	LD_LIBRARY_PATH="" python3 $(ESP_ROOT)/tools/socgen/generate_socmap_vhd.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)



esp-config-clean:
	$(QUIET_CLEAN)$(RM) \
		$(ESP_CFG_BUILD)/.esp_config.bak

ifneq ("$(CPU_ARCH)", "leon3")
$(ESP_CFG_BUILD)/riscv.dts: $(ESP_CFG_BUILD)/.esp_config $(GRLIB_CFG_BUILD)/grlib_config.vhd top.vhd
	$(QUIET_MAKE)$(MAKE) $(ESP_CFG_BUILD)/socmap.vhd

ARIANE_RV_PLIC_REGMAP_GEN = $(ESP_ROOT)/rtl/cores/ariane/ariane/src/rv_plic/rtl/gen_plic_addrmap.py

$(ESP_CFG_BUILD)/plic_regmap.sv: $(ARIANE_RV_PLIC_REGMAP_GEN) $(ESP_CFG_BUILD)/.esp_config
	$(QUIET_MAKE)$< -t $$(($(NCPU_TILE)*2)) > $@
endif

ifeq ("$(CPU_ARCH)", "leon3")
$(ESP_CFG_BUILD)/plic_regmap.sv:

endif

esp-config-distclean:
	$(QUIET_CLEAN)$(RM) $(ESP_CFG_BUILD)

config-distclean:
	$(QUIET_CLEAN)$(RM) $(CFG_BUILD)

.PHONY: esplink newsocmap-run esp-xconfig esp-defconfig esp-config-clean esp-config-distclean config-distclean
