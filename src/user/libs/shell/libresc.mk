ICON_FOLDER := ../../../icons

ICONS_FS := folder folder_open file computer hdd device
ICONS_APPS := default
ICONS_ACTIONS_32 := error question
ICONS_ACTIONS_16 := error back goto new_folder

EXTRA_RESC_FILES := icons.inf

ICON_DST_template = $(addsuffix _$(2).png, $(addprefix libresc/icons/, $(1)))
ICON_SRC_template = $(addsuffix .png, $(addprefix $(ICON_FOLDER)/$(2)/$(3)/, $(1)))

define ICON_template=
$(call ICON_DST_template,$(1),$(2),$(3)): $(call ICON_SRC_template,$(1),$(2),$(3))
	mkdir -p libresc/icons/
	cp $$< $$@

$(LIBRESC-TAR-NAME): $(call ICON_DST_template,$(1),$(2),$(3))
endef

$(foreach ICON, $(ICONS_FS), $(eval $(call ICON_template,$(ICON),32,fs)))
$(foreach ICON, $(ICONS_FS), $(eval $(call ICON_template,$(ICON),16,fs)))

$(foreach ICON, $(ICONS_APPS), $(eval $(call ICON_template,$(ICON),32,apps)))
$(foreach ICON, $(ICONS_APPS), $(eval $(call ICON_template,$(ICON),16,apps)))

$(foreach ICON, $(ICONS_ACTIONS_32), $(eval $(call ICON_template,$(ICON),32,actions)))
$(foreach ICON, $(ICONS_ACTIONS_16), $(eval $(call ICON_template,$(ICON),16,actions)))

EXTRA_DST_template = $(addprefix libresc/, $(1))

define EXTRA_template=
$(call EXTRA_DST_template,$(1)): $(1)
	cp $$< $$@

$(RESC-TAR-NAME): $(call EXTRA_DST_template,$(1))
endef

$(foreach EXTRA, $(EXTRA_RESC_FILES), $(eval $(call EXTRA_template,$(EXTRA))))

$(LIBRESC-TAR-NAME): libresc.mk
