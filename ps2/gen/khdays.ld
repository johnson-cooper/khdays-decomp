/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# Linkfile script for ee-ld
*/

ENTRY(__start);

PHDRS {
  text PT_LOAD;
}

SECTIONS {
	.text 0x00100000: {
		_ftext = . ;
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t*)
		KEEP(*(.init))
		KEEP(*(.fini))
		QUAD(0)
	} :text

	PROVIDE(_etext = .);
	PROVIDE(etext = .);

	/* Global/static constructors and deconstructors. This build never
	   links gcc's own crtbegin.o/crtend.o (-nostartfiles), so there's
	   no -1 start sentinel or 0 end terminator for crt0.c's own
	   __do_global_ctors-style walk to rely on - __CTOR_LIST__/
	   __CTOR_END__ give it real, safe bounds instead. */
	.ctors ALIGN(16): {
		PROVIDE(__CTOR_LIST__ = .);
		KEEP(*crtbegin*.o(.ctors))
		KEEP(*(EXCLUDE_FILE(*crtend*.o) .ctors))
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__CTOR_END__ = .);
	}
	.dtors ALIGN(16): {
		PROVIDE(__DTOR_LIST__ = .);
		KEEP(*crtbegin*.o(.dtors))
		KEEP(*(EXCLUDE_FILE(*crtend*.o) .dtors))
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__DTOR_END__ = .);
	}

	.reginfo : { *(.reginfo) }

	.data ALIGN(128): {
		_fdata = . ;
		. = ALIGN(32) + 0x1c; __kh_dsdata_main = .;
		. = __kh_dsdata_main + 0x0; KEEP(*libkh_*.a:main_exception_index_0203d1bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x54; KEEP(*libkh_*.a:fx_sincos_table.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4054; KEEP(*libkh_*.a:fx_atan_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x425c; KEEP(*libkh_*.a:gx_load3d_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x42cc; KEEP(*libkh_*.a:snd_decibel_table.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x43cc; KEEP(*libkh_*.a:snd_volume_table.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x46a0; KEEP(*libkh_*.a:main_pointers_0204185c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x46c4; KEEP(*libkh_*.a:fs_overlay_digest_key.o(.rodata .rodata.*))
		. = __kh_dsdata_main + 0x4704; KEEP(*libkh_*.a:main_hmac_alloc_020418c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4768; KEEP(*libkh_*.a:main_pointers_02041924.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4808; KEEP(*libkh_*.a:g2d_oam_size_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4838; KEEP(*libkh_*.a:g2d_screen_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x48d0; KEEP(*libkh_*.a:g2d_charcanvas_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4904; KEEP(*libkh_*.a:g2d_bg_cnt_table.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4924; KEEP(*libkh_*.a:g3d_nsbca_pivot_table.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4948; KEEP(*libkh_*.a:sndarc_stream_adpcm_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4a0c; KEEP(*libkh_*.a:msl_ctype_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4c0c; KEEP(*libkh_*.a:main_tables_02041dc8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4c70; KEEP(*libkh_*.a:main_pointers_02041e2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4cb0; KEEP(*libkh_*.a:main_tables_02041e6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4cf0; KEEP(*libkh_*.a:main_pointers_02041eac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4d10; KEEP(*libkh_*.a:main_tables_02041ecc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4e18; KEEP(*libkh_*.a:main_pointers_02041fd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4ed0; KEEP(*libkh_*.a:main_tables_0204208c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x4f68; KEEP(*libkh_*.a:main_tables_02042124.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x50e4; KEEP(*libkh_*.a:os_irq_callback_bits.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x50f4; KEEP(*libkh_*.a:os_console_type_cache.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x50f8; KEEP(*libkh_*.a:gx_disp_on_flag.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x50fc; KEEP(*libkh_*.a:gx_dma_id.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5100; KEEP(*libkh_*.a:fs_archive_names.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5108; KEEP(*libkh_*.a:fs_overlay_digest_key.o(.data .data.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5110; KEEP(*libkh_*.a:dgt_sha1_test_vectors.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x522c; KEEP(*libkh_*.a:card_rom_header_addr.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5230; KEEP(*libkh_*.a:main_pointers_020423ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x525c; KEEP(*libkh_*.a:main_party_roster_02042418.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x52d4; KEEP(*libkh_*.a:g3d_anmobj_init_func_num.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x52d8; KEEP(*libkh_*.a:main_pointers_02042494.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x52f8; KEEP(*libkh_*.a:g3d_anmobj_init_funcs.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5348; KEEP(*libkh_*.a:main_pointers_02042504.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5370; KEEP(*libkh_*.a:main_scene_table_0204252c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5430; KEEP(*libkh_*.a:main_pointers_020425ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5574; KEEP(*libkh_*.a:main_pause_menu_02042730.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5618; KEEP(*libkh_*.a:main_pointers_020427d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5634; KEEP(*libkh_*.a:main_transform_defaults_020427f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5754; KEEP(*libkh_*.a:main_pointers_02042910.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x579c; KEEP(*libkh_*.a:main_worlds_02042958.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x58b4; KEEP(*libkh_*.a:main_pointers_02042a70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_main + 0x5904; KEEP(*libkh_*.a:main_overlay_digests_02042ac0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_main + 0x70e4);
		
		__kh_data_start_ov000 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov000 = .;
		. = __kh_dsdata_ov000 + 0x0; KEEP(*libkh_*.a:ov000_tables_0205a6b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x20; KEEP(*libkh_*.a:ov000_rodata_0205a6d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x44; KEEP(*libkh_*.a:ov000_tables_0205a6f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x60; KEEP(*libkh_*.a:ov000_pointers_0205a710.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x84; KEEP(*libkh_*.a:ov000_tables_0205a734.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x1a8; KEEP(*libkh_*.a:ov000_tracker_0205a858.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x1bc; KEEP(*libkh_*.a:ov000_pointers_0205a86c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x1d4; KEEP(*libkh_*.a:ov000_tables_0205a884.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x2ac; KEEP(*libkh_*.a:ov000_tracker_0205a95c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x2c0; KEEP(*libkh_*.a:ov000_tables_0205a970.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x310; KEEP(*libkh_*.a:ov000_class_0205a9c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x324; KEEP(*libkh_*.a:ov000_tables_0205a9d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x33c; KEEP(*libkh_*.a:ov000_strings_0205a9ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x384; KEEP(*libkh_*.a:ov000_class_0205aa34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x398; KEEP(*libkh_*.a:ov000_strings_0205aa48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x428; KEEP(*libkh_*.a:ov000_tables_0205aad8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x434; KEEP(*libkh_*.a:ov000_strings_0205aae4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x45c; KEEP(*libkh_*.a:ov000_class_0205ab0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x470; KEEP(*libkh_*.a:ov000_strings_0205ab20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x4d0; KEEP(*libkh_*.a:ov000_pointers_0205ab80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x4d8; KEEP(*libkh_*.a:ov000_strings_0205ab88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x4e4; KEEP(*libkh_*.a:ov000_class_0205ab94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov000 + 0x4f8; KEEP(*libkh_*.a:ov000_strings_0205aba8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov000 + 0x570);
		*libkh_*_ov000*.a:*(.data .data.*) __kh_data_end_ov000 = .;
		__kh_data_start_ov001 = .; *libkh_*_ov001*.a:*(.data .data.*) __kh_data_end_ov001 = .;
		__kh_data_start_ov002 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov002 = .;
		. = __kh_dsdata_ov002 + 0x0; KEEP(*libkh_*.a:ov002_tables_0207db54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x10c; KEEP(*libkh_*.a:ov002_pointers_0207dc60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x13c; KEEP(*libkh_*.a:ov002_tables_0207dc90.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x16c; KEEP(*libkh_*.a:ov002_panel_scales.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1c0; KEEP(*libkh_*.a:ov002_help_pages.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x228; KEEP(*libkh_*.a:ov002_gauge_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x364; KEEP(*libkh_*.a:ov002_panel_groups.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x630; KEEP(*libkh_*.a:ov002_tables_0207e184.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x6c0; KEEP(*libkh_*.a:ov002_panel_row_fx.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x804; KEEP(*libkh_*.a:ov002_tables_0207e358.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x824; KEEP(*libkh_*.a:ov002_pointers_0207e378.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x834; KEEP(*libkh_*.a:ov002_tables_0207e388.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x94c; KEEP(*libkh_*.a:ov002_map_page_ids.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x9a4; KEEP(*libkh_*.a:ov002_hud_records.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xaec; KEEP(*libkh_*.a:ov002_surface_tags.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xb24; KEEP(*libkh_*.a:ov002_tier_values.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xb26; KEEP(*libkh_*.a:ov002_peer_limit.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xb28; KEEP(*libkh_*.a:ov002_spawn_rows.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xba8; KEEP(*libkh_*.a:ov002_tables_0207e6fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xbcc; KEEP(*libkh_*.a:ov002_screens_0207e720.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xbe0; KEEP(*libkh_*.a:ov002_tables_0207e734.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xc10; KEEP(*libkh_*.a:ov002_camera_shots.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xcdc; KEEP(*libkh_*.a:ov002_screens_0207e830.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xcf0; KEEP(*libkh_*.a:ov002_strings_0207e844.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xd2c; KEEP(*libkh_*.a:ov002_screens_0207e880.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xd54; KEEP(*libkh_*.a:ov002_strings_0207e8a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xd60; KEEP(*libkh_*.a:ov002_screens_0207e8b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xd88; KEEP(*libkh_*.a:ov002_strings_0207e8dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xe34; KEEP(*libkh_*.a:ov002_tables_0207e988.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xe38; KEEP(*libkh_*.a:ov002_help_entries.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xe70; KEEP(*libkh_*.a:ov002_tables_0207e9c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xe78; KEEP(*libkh_*.a:ov002_screens_0207e9cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xea0; KEEP(*libkh_*.a:ov002_tables_0207e9f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xeac; KEEP(*libkh_*.a:ov002_screens_0207ea00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xec0; KEEP(*libkh_*.a:ov002_tile_rows.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xf88; KEEP(*libkh_*.a:ov002_strings_0207eadc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfb4; KEEP(*libkh_*.a:ov002_empty_text_0207eb08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfb8; KEEP(*libkh_*.a:ov002_tables_0207eb0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfbc; KEEP(*libkh_*.a:ov002_strings_0207eb10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfd0; KEEP(*libkh_*.a:ov002_tables_0207eb24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfd4; KEEP(*libkh_*.a:ov002_pointers_0207eb28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfd8; KEEP(*libkh_*.a:ov002_screens_0207eb2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0xfec; KEEP(*libkh_*.a:ov002_strings_0207eb40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x105c; KEEP(*libkh_*.a:ov002_screens_0207ebb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1070; KEEP(*libkh_*.a:ov002_strings_0207ebc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x10a0; KEEP(*libkh_*.a:ov002_pointers_0207ebf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x10ac; KEEP(*libkh_*.a:ov002_tables_0207ec00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x114c; KEEP(*libkh_*.a:ov002_screens_0207eca0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1160; KEEP(*libkh_*.a:ov002_strings_0207ecb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x11c8; KEEP(*libkh_*.a:ov002_tables_0207ed1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x11cc; KEEP(*libkh_*.a:ov002_strings_0207ed20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1220; KEEP(*libkh_*.a:ov002_screens_0207ed74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1234; KEEP(*libkh_*.a:ov002_strings_0207ed88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x12a0; KEEP(*libkh_*.a:ov002_screens_0207edf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x12b4; KEEP(*libkh_*.a:ov002_tables_0207ee08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x12e4; KEEP(*libkh_*.a:ov002_strings_0207ee38.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x131c; KEEP(*libkh_*.a:ov002_screens_0207ee70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1344; KEEP(*libkh_*.a:ov002_strings_0207ee98.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x135c; KEEP(*libkh_*.a:ov002_screens_0207eeb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1370; KEEP(*libkh_*.a:ov002_strings_0207eec4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1384; KEEP(*libkh_*.a:ov002_pointers_0207eed8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1394; KEEP(*libkh_*.a:ov002_screens_0207eee8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x13a8; KEEP(*libkh_*.a:ov002_strings_0207eefc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1400; KEEP(*libkh_*.a:ov002_screens_0207ef54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1414; KEEP(*libkh_*.a:ov002_tables_0207ef68.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x145c; KEEP(*libkh_*.a:ov002_strings_0207efb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x14b8; KEEP(*libkh_*.a:ov002_tables_0207f00c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x14bc; KEEP(*libkh_*.a:ov002_screens_0207f010.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x14d0; KEEP(*libkh_*.a:ov002_tables_0207f024.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x14d4; KEEP(*libkh_*.a:ov002_screens_0207f028.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x14e8; KEEP(*libkh_*.a:ov002_strings_0207f03c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1550; KEEP(*libkh_*.a:ov002_pointers_0207f0a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1580; KEEP(*libkh_*.a:ov002_strings_0207f0d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x15c0; KEEP(*libkh_*.a:ov002_tables_0207f114.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x15c4; KEEP(*libkh_*.a:ov002_empty_text_0207f118.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x15c8; KEEP(*libkh_*.a:ov002_strings_0207f11c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x15e0; KEEP(*libkh_*.a:ov002_pointers_0207f134.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x18b0; KEEP(*libkh_*.a:ov002_strings_0207f404.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x18b4; KEEP(*libkh_*.a:ov002_tables_0207f408.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x18b8; KEEP(*libkh_*.a:ov002_screens_0207f40c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x18cc; KEEP(*libkh_*.a:ov002_strings_0207f420.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1900; KEEP(*libkh_*.a:ov002_screens_0207f454.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1914; KEEP(*libkh_*.a:ov002_strings_0207f468.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1930; KEEP(*libkh_*.a:ov002_pointers_0207f484.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1a38; KEEP(*libkh_*.a:ov002_tables_0207f58c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov002 + 0x1a3c; KEEP(*libkh_*.a:ov002_panel_overlays.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov002 + 0x1aac);
		*libkh_*_ov002*.a:*(.data .data.*) __kh_data_end_ov002 = .;
		__kh_data_start_ov003 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov003 = .;
		. = __kh_dsdata_ov003 + 0x0; KEEP(*libkh_*.a:ov003_tables_0204f724.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov003 + 0x1c; KEEP(*libkh_*.a:ov003_strings_0204f740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov003 + 0x198; KEEP(*libkh_*.a:ov003_tables_0204f8bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov003 + 0x1c0; KEEP(*libkh_*.a:ov003_class_0204f8e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov003 + 0x1d4; KEEP(*libkh_*.a:ov003_pointers_0204f8f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov003 + 0x220; KEEP(*libkh_*.a:ov003_strings_0204f944.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov003 + 0x27c);
		*libkh_*_ov003*.a:*(.data .data.*) __kh_data_end_ov003 = .;
		__kh_data_start_ov004 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov004 = .;
		. = __kh_dsdata_ov004 + 0x0; KEEP(*libkh_*.a:ov004_tables_0205104c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x58; KEEP(*libkh_*.a:ov004_pointers_020510a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x64; KEEP(*libkh_*.a:ov004_tables_020510b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x6c; KEEP(*libkh_*.a:ov004_pointers_020510b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x80; KEEP(*libkh_*.a:ov004_tables_020510cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x194; KEEP(*libkh_*.a:ov004_strings_020511e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x1c4; KEEP(*libkh_*.a:ov004_class_02051210.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x1d8; KEEP(*libkh_*.a:ov004_strings_02051224.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x2a0; KEEP(*libkh_*.a:ov004_class_020512ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x2b4; KEEP(*libkh_*.a:ov004_pointers_02051300.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x2dc; KEEP(*libkh_*.a:ov004_strings_02051328.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov004 + 0x320; KEEP(*libkh_*.a:ov004_tables_0205136c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov004 + 0x334);
		*libkh_*_ov004*.a:*(.data .data.*) __kh_data_end_ov004 = .;
		__kh_data_start_ov005 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov005 = .;
		. = __kh_dsdata_ov005 + 0x0; KEEP(*libkh_*.a:ov005_tables_0205b2dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x8c; KEEP(*libkh_*.a:ov005_pointers_0205b368.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0xb0; KEEP(*libkh_*.a:ov005_tables_0205b38c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0xc0; KEEP(*libkh_*.a:ov005_pointers_0205b39c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0xe0; KEEP(*libkh_*.a:ov005_msglist_0205b3bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0xec; KEEP(*libkh_*.a:ov005_tables_0205b3c8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x1e4; KEEP(*libkh_*.a:ov005_strings_0205b4c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x200; KEEP(*libkh_*.a:ov005_class_0205b4dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x214; KEEP(*libkh_*.a:ov005_class_0205b4f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x228; KEEP(*libkh_*.a:ov005_tables_0205b504.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x230; KEEP(*libkh_*.a:ov005_strings_0205b50c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x2ac; KEEP(*libkh_*.a:ov005_tables_0205b588.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x2b4; KEEP(*libkh_*.a:ov005_class_0205b590.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x2c8; KEEP(*libkh_*.a:ov005_strings_0205b5a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x344; KEEP(*libkh_*.a:ov005_tables_0205b620.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x4ac; KEEP(*libkh_*.a:ov005_strings_0205b788.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x4c0; KEEP(*libkh_*.a:ov005_pointers_0205b79c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x4cc; KEEP(*libkh_*.a:ov005_tables_0205b7a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x4f0; KEEP(*libkh_*.a:ov005_class_0205b7cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov005 + 0x504; KEEP(*libkh_*.a:ov005_tables_0205b7e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov005 + 0x524);
		*libkh_*_ov005*.a:*(.data .data.*) __kh_data_end_ov005 = .;
		__kh_data_start_ov006 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov006 = .;
		. = __kh_dsdata_ov006 + 0x0; KEEP(*libkh_*.a:ov006_tables_020561c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0xcc; KEEP(*libkh_*.a:ov006_mission_tables.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x200; KEEP(*libkh_*.a:ov006_class_020563c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x214; KEEP(*libkh_*.a:ov006_tables_020563d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x224; KEEP(*libkh_*.a:ov006_class_020563e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x238; KEEP(*libkh_*.a:ov006_tables_020563f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x348; KEEP(*libkh_*.a:ov006_strings_02056508.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x35c; KEEP(*libkh_*.a:ov006_tables_0205651c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x36c; KEEP(*libkh_*.a:ov006_class_0205652c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x380; KEEP(*libkh_*.a:ov006_strings_02056540.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov006 + 0x400; KEEP(*libkh_*.a:ov006_class_020565c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov006 + 0x420);
		*libkh_*_ov006*.a:*(.data .data.*) __kh_data_end_ov006 = .;
		__kh_data_start_ov007 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov007 = .;
		. = __kh_dsdata_ov007 + 0x0; KEEP(*libkh_*.a:ov007_tables_0204d3ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov007 + 0x14; KEEP(*libkh_*.a:ov007_const_0204d3c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov007 + 0x18; KEEP(*libkh_*.a:ov007_class_0204d3c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov007 + 0x2c; KEEP(*libkh_*.a:ov007_strings_0204d3d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov007 + 0x74);
		*libkh_*_ov007*.a:*(.data .data.*) __kh_data_end_ov007 = .;
		__kh_data_start_ov008 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov008 = .;
		. = __kh_dsdata_ov008 + 0x0; KEEP(*libkh_*.a:ov008_header_limits.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x4; KEEP(*libkh_*.a:ov008_surface_0208e84c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x18; KEEP(*libkh_*.a:ov008_tables_0208e860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x34; KEEP(*libkh_*.a:ov008_pointers_0208e87c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x5c; KEEP(*libkh_*.a:ov008_tables_0208e8a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x110; KEEP(*libkh_*.a:ov008_tables_0208e958.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x17c; KEEP(*libkh_*.a:ov008_scene_params.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x58c; KEEP(*libkh_*.a:ov008_pointers_0208edd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x598; KEEP(*libkh_*.a:ov008_missionres_0208ede0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x5a4; KEEP(*libkh_*.a:ov008_tables_0208edec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x63c; KEEP(*libkh_*.a:ov008_tables_0208ee84.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x7fc; KEEP(*libkh_*.a:ov008_tables_0208f044.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x838; KEEP(*libkh_*.a:ov008_strings_0208f080.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x8c8; KEEP(*libkh_*.a:ov008_tables_0208f110.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x904; KEEP(*libkh_*.a:ov008_pointers_0208f14c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x914; KEEP(*libkh_*.a:ov008_tables_0208f15c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1088; KEEP(*libkh_*.a:ov008_missionres_0208f8d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1094; KEEP(*libkh_*.a:ov008_missionres_0208f8dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x10a0; KEEP(*libkh_*.a:ov008_tables_0208f8e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x11c8; KEEP(*libkh_*.a:ov008_missionres_0208fa10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x11d4; KEEP(*libkh_*.a:ov008_missionres_0208fa1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x11e0; KEEP(*libkh_*.a:ov008_tables_0208fa28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1510; KEEP(*libkh_*.a:ov008_surface_0208fd58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1524; KEEP(*libkh_*.a:ov008_tables_0208fd6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1544; KEEP(*libkh_*.a:ov008_pointers_0208fd8c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1580; KEEP(*libkh_*.a:ov008_tables_0208fdc8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1680; KEEP(*libkh_*.a:ov008_surface_0208fec8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1694; KEEP(*libkh_*.a:ov008_tables_0208fedc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1724; KEEP(*libkh_*.a:ov008_missionres_0208ff6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1730; KEEP(*libkh_*.a:ov008_tables_0208ff78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1758; KEEP(*libkh_*.a:ov008_class_0208ffa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x176c; KEEP(*libkh_*.a:ov008_strings_0208ffb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1808; KEEP(*libkh_*.a:ov008_tables_02090050.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x181c; KEEP(*libkh_*.a:ov008_hookslots_02090064.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1854; KEEP(*libkh_*.a:ov008_hookslots_0209009c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1890; KEEP(*libkh_*.a:ov008_class_020900d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x18a4; KEEP(*libkh_*.a:ov008_strings_020900ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x18c0; KEEP(*libkh_*.a:ov008_tables_02090108.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1900; KEEP(*libkh_*.a:ov008_screen_02090148.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1944; KEEP(*libkh_*.a:ov008_tables_0209018c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x19b4; KEEP(*libkh_*.a:ov008_strings_020901fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1aa0; KEEP(*libkh_*.a:ov008_screen_020902e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1ae4; KEEP(*libkh_*.a:ov008_strings_0209032c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1afc; KEEP(*libkh_*.a:ov008_empty_text_02090344.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1b00; KEEP(*libkh_*.a:ov008_strings_02090348.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1b18; KEEP(*libkh_*.a:ov008_tables_02090360.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1b38; KEEP(*libkh_*.a:ov008_screen_02090380.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1b7c; KEEP(*libkh_*.a:ov008_tables_020903c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1ba8; KEEP(*libkh_*.a:ov008_strings_020903f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1bd4; KEEP(*libkh_*.a:ov008_screen_0209041c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1c18; KEEP(*libkh_*.a:ov008_screen_02090460.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1c5c; KEEP(*libkh_*.a:ov008_strings_020904a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1cb0; KEEP(*libkh_*.a:ov008_tables_020904f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1d0c; KEEP(*libkh_*.a:ov008_screen_02090554.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1d50; KEEP(*libkh_*.a:ov008_menu_entries_02090598.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1f44; KEEP(*libkh_*.a:ov008_strings_0209078c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1f58; KEEP(*libkh_*.a:ov008_page_020907a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1f98; KEEP(*libkh_*.a:ov008_tables_020907e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1fa8; KEEP(*libkh_*.a:ov008_strings_020907f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1fc0; KEEP(*libkh_*.a:ov008_tables_02090808.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1fd0; KEEP(*libkh_*.a:ov008_strings_02090818.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x1fe4; KEEP(*libkh_*.a:ov008_page_0209082c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2024; KEEP(*libkh_*.a:ov008_strings_0209086c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2030; KEEP(*libkh_*.a:ov008_tables_02090878.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2040; KEEP(*libkh_*.a:ov008_empty_text_02090888.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2044; KEEP(*libkh_*.a:ov008_tables_0209088c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2054; KEEP(*libkh_*.a:ov008_strings_0209089c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2098; KEEP(*libkh_*.a:ov008_page_020908e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x20d8; KEEP(*libkh_*.a:ov008_strings_02090920.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x20e4; KEEP(*libkh_*.a:ov008_tables_0209092c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2148; KEEP(*libkh_*.a:ov008_empty_text_02090990.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x214c; KEEP(*libkh_*.a:ov008_tables_02090994.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x215c; KEEP(*libkh_*.a:ov008_strings_020909a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x218c; KEEP(*libkh_*.a:ov008_page_020909d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x21cc; KEEP(*libkh_*.a:ov008_page_02090a14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x220c; KEEP(*libkh_*.a:ov008_tables_02090a54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x22f8; KEEP(*libkh_*.a:ov008_strings_02090b40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2368; KEEP(*libkh_*.a:ov008_class_02090bb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x237c; KEEP(*libkh_*.a:ov008_tables_02090bc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x238c; KEEP(*libkh_*.a:ov008_class_02090bd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x23a0; KEEP(*libkh_*.a:ov008_tables_02090be8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x24b0; KEEP(*libkh_*.a:ov008_strings_02090cf8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x24c4; KEEP(*libkh_*.a:ov008_tables_02090d0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x24d4; KEEP(*libkh_*.a:ov008_class_02090d1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x24e8; KEEP(*libkh_*.a:ov008_strings_02090d30.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2568; KEEP(*libkh_*.a:ov008_class_02090db0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x257c; KEEP(*libkh_*.a:ov008_class_02090dc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2590; KEEP(*libkh_*.a:ov008_strings_02090dd8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x2658; KEEP(*libkh_*.a:ov008_tables_02090ea0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov008 + 0x268c; KEEP(*libkh_*.a:ov008_strings_02090ed4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov008 + 0x26b8);
		*libkh_*_ov008*.a:*(.data .data.*) __kh_data_end_ov008 = .;
		__kh_data_start_ov009 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov009 = .;
		. = __kh_dsdata_ov009 + 0x0; KEEP(*libkh_*.a:ov009_rodata_02055f54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x18; KEEP(*libkh_*.a:ov009_tables_02055f6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x34; KEEP(*libkh_*.a:ov009_pointers_02055f88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x5c; KEEP(*libkh_*.a:ov009_tables_02055fb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x1b4; KEEP(*libkh_*.a:ov009_msglist_02056108.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x1c0; KEEP(*libkh_*.a:ov009_tables_02056114.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x2cc; KEEP(*libkh_*.a:ov009_class_02056220.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x2e0; KEEP(*libkh_*.a:ov009_strings_02056234.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x36c; KEEP(*libkh_*.a:ov009_tables_020562c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x388; KEEP(*libkh_*.a:ov009_pointers_020562dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x38c; KEEP(*libkh_*.a:ov009_class_020562e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x3a0; KEEP(*libkh_*.a:ov009_handlers_020562f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x3e4; KEEP(*libkh_*.a:ov009_strings_02056338.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x438; KEEP(*libkh_*.a:ov009_tables_0205638c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov009 + 0x444; KEEP(*libkh_*.a:ov009_strings_02056398.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov009 + 0x48c);
		*libkh_*_ov009*.a:*(.data .data.*) __kh_data_end_ov009 = .;
		__kh_data_start_ov010 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov010 = .;
		. = __kh_dsdata_ov010 + 0x0; KEEP(*libkh_*.a:ov010_const_0204cf88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov010 + 0x18; KEEP(*libkh_*.a:ov010_class_0204cfa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov010 + 0x2c; KEEP(*libkh_*.a:ov010_strings_0204cfb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov010 + 0x58);
		*libkh_*_ov010*.a:*(.data .data.*) __kh_data_end_ov010 = .;
		__kh_data_start_ov011 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov011 = .;
		. = __kh_dsdata_ov011 + 0x0; KEEP(*libkh_*.a:ov011_tables_0205dc7c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0x14; KEEP(*libkh_*.a:ov011_pointers_0205dc90.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0x24; KEEP(*libkh_*.a:ov011_tables_0205dca0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0x4b4; KEEP(*libkh_*.a:ov011_anim_0205e130.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0xc24; KEEP(*libkh_*.a:ov011_class_0205e8a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0xc38; KEEP(*libkh_*.a:ov011_pointers_0205e8b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov011 + 0xc68; KEEP(*libkh_*.a:ov011_strings_0205e8e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov011 + 0xce4);
		*libkh_*_ov011*.a:*(.data .data.*) __kh_data_end_ov011 = .;
		__kh_data_start_ov012 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov012 = .;
		. = __kh_dsdata_ov012 + 0x0; KEEP(*libkh_*.a:ov012_opening_clip_0205c2a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov012 + 0x4; KEEP(*libkh_*.a:ov012_pointers_0205c2a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov012 + 0x1c; KEEP(*libkh_*.a:ov012_opening_data_0205c2bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov012 + 0x820; KEEP(*libkh_*.a:ov012_strings_0205cac0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov012 + 0x854; KEEP(*libkh_*.a:ov012_pointers_0205caf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov012 + 0x880);
		*libkh_*_ov012*.a:*(.data .data.*) __kh_data_end_ov012 = .;
		__kh_data_start_ov013 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov013 = .;
		. = __kh_dsdata_ov013 + 0x0; KEEP(*libkh_*.a:ov013_strings_0207fec0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov013 + 0x8; KEEP(*libkh_*.a:ov013_pointers_0207fec8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov013 + 0x20);
		*libkh_*_ov013*.a:*(.data .data.*) __kh_data_end_ov013 = .;
		__kh_data_start_ov014 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov014 = .;
		. = __kh_dsdata_ov014 + 0x0; KEEP(*libkh_*.a:ov014_pointers_020808e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov014 + 0x40);
		*libkh_*_ov014*.a:*(.data .data.*) __kh_data_end_ov014 = .;
		__kh_data_start_ov015 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov015 = .;
		. = __kh_dsdata_ov015 + 0x0; KEEP(*libkh_*.a:ov015_pickup_kinds_020828d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov015 + 0x2c; KEEP(*libkh_*.a:ov015_command_02082900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov015 + 0x30; KEEP(*libkh_*.a:ov015_pointers_02082904.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov015 + 0x8c);
		*libkh_*_ov015*.a:*(.data .data.*) __kh_data_end_ov015 = .;
		__kh_data_start_ov016 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov016 = .;
		. = __kh_dsdata_ov016 + 0x0; KEEP(*libkh_*.a:ov016_pointers_020826e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov016 + 0x60; KEEP(*libkh_*.a:ov016_tables_02082740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov016 + 0x68; KEEP(*libkh_*.a:ov016_pointers_02082748.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov016 + 0x6c; KEEP(*libkh_*.a:ov016_tables_0208274c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov016 + 0x78; KEEP(*libkh_*.a:ov016_kickable_rows_02082758.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov016 + 0xe0);
		*libkh_*_ov016*.a:*(.data .data.*) __kh_data_end_ov016 = .;
		__kh_data_start_ov017 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov017 = .;
		. = __kh_dsdata_ov017 + 0x0; KEEP(*libkh_*.a:ov017_pointers_02080e40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov017 + 0x60);
		*libkh_*_ov017*.a:*(.data .data.*) __kh_data_end_ov017 = .;
		__kh_data_start_ov018 = .; *libkh_*_ov018*.a:*(.data .data.*) __kh_data_end_ov018 = .;
		__kh_data_start_ov019 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov019 = .;
		. = __kh_dsdata_ov019 + 0x0; KEEP(*libkh_*.a:ov019_tables_0207fd40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov019 + 0x20; KEEP(*libkh_*.a:ov019_pointers_0207fd60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov019 + 0x38; KEEP(*libkh_*.a:ov019_strings_0207fd78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov019 + 0x40);
		*libkh_*_ov019*.a:*(.data .data.*) __kh_data_end_ov019 = .;
		__kh_data_start_ov020 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov020 = .;
		. = __kh_dsdata_ov020 + 0x0; KEEP(*libkh_*.a:ov020_pointers_020800c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov020 + 0x18; KEEP(*libkh_*.a:ov020_strings_020800d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov020 + 0x60);
		*libkh_*_ov020*.a:*(.data .data.*) __kh_data_end_ov020 = .;
		__kh_data_start_ov021 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov021 = .;
		. = __kh_dsdata_ov021 + 0x0; KEEP(*libkh_*.a:ov021_const_02080f18.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov021 + 0x8; KEEP(*libkh_*.a:ov021_pointers_02080f20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov021 + 0x28);
		*libkh_*_ov021*.a:*(.data .data.*) __kh_data_end_ov021 = .;
		__kh_data_start_ov022 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov022 = .;
		. = __kh_dsdata_ov022 + 0x0; KEEP(*libkh_*.a:ov022_tables_020b226c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x128; KEEP(*libkh_*.a:ov022_pointers_020b2394.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x278; KEEP(*libkh_*.a:ov022_strings_020b24e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x290; KEEP(*libkh_*.a:ov022_ctorargs_020b24fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x2a4; KEEP(*libkh_*.a:ov022_pointers_020b2510.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x2cc; KEEP(*libkh_*.a:ov022_tuning_020b2538.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x33c; KEEP(*libkh_*.a:ov022_pointers_020b25a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x348; KEEP(*libkh_*.a:ov022_strings_020b25b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x358; KEEP(*libkh_*.a:ov022_tables_020b25c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x398; KEEP(*libkh_*.a:ov022_ctorargs_020b2604.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x3ac; KEEP(*libkh_*.a:ov022_ctorargs_020b2618.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x3c0; KEEP(*libkh_*.a:ov022_ctorargs_020b262c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x3d4; KEEP(*libkh_*.a:ov022_ctorargs_020b2640.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x3e8; KEEP(*libkh_*.a:ov022_tables_020b2654.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x5e4; KEEP(*libkh_*.a:ov022_pointers_020b2850.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x614; KEEP(*libkh_*.a:ov022_pointers_020b2880.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x628; KEEP(*libkh_*.a:ov022_class_020b2894.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x63c; KEEP(*libkh_*.a:ov022_class_020b28a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x650; KEEP(*libkh_*.a:ov022_strings_020b28bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x668; KEEP(*libkh_*.a:ov022_class_020b28d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x67c; KEEP(*libkh_*.a:ov022_strings_020b28e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x68c; KEEP(*libkh_*.a:ov022_class_020b28f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x6a0; KEEP(*libkh_*.a:ov022_strings_020b290c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x6b0; KEEP(*libkh_*.a:ov022_class_020b291c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x6c4; KEEP(*libkh_*.a:ov022_class_020b2930.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x6d8; KEEP(*libkh_*.a:ov022_class_020b2944.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x6ec; KEEP(*libkh_*.a:ov022_strings_020b2958.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x750; KEEP(*libkh_*.a:ov022_pointers_020b29bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x8b0; KEEP(*libkh_*.a:ov022_strings_020b2b1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x8dc; KEEP(*libkh_*.a:ov022_pointers_020b2b48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0x900; KEEP(*libkh_*.a:ov022_strings_020b2b6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0xb44; KEEP(*libkh_*.a:ov022_class_020b2db0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov022 + 0xb58; KEEP(*libkh_*.a:ov022_strings_020b2dc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov022 + 0xbf4);
		*libkh_*_ov022*.a:*(.data .data.*) __kh_data_end_ov022 = .;
		__kh_data_start_ov023 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov023 = .;
		. = __kh_dsdata_ov023 + 0x0; KEEP(*libkh_*.a:ov023_tables_02089cfc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x18; KEEP(*libkh_*.a:ov023_pointers_02089d14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x78; KEEP(*libkh_*.a:ov023_tables_02089d74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x114; KEEP(*libkh_*.a:ov023_pointers_02089e10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x1b4; KEEP(*libkh_*.a:ov023_tables_02089eb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x304; KEEP(*libkh_*.a:ov023_const_0208a000.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x308; KEEP(*libkh_*.a:ov023_class_0208a004.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x31c; KEEP(*libkh_*.a:ov023_strings_0208a018.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x33c; KEEP(*libkh_*.a:ov023_class_0208a038.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x350; KEEP(*libkh_*.a:ov023_strings_0208a04c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x3b0; KEEP(*libkh_*.a:ov023_const_0208a0ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x3b4; KEEP(*libkh_*.a:ov023_strings_0208a0b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x638; KEEP(*libkh_*.a:ov023_pointers_0208a334.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x8d0; KEEP(*libkh_*.a:ov023_strings_0208a5cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x970; KEEP(*libkh_*.a:ov023_pointers_0208a66c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0x9a0; KEEP(*libkh_*.a:ov023_strings_0208a69c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0xa10; KEEP(*libkh_*.a:ov023_pointers_0208a70c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0xa34; KEEP(*libkh_*.a:ov023_strings_0208a730.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0xa48; KEEP(*libkh_*.a:ov023_const_0208a744.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0xa4c; KEEP(*libkh_*.a:ov023_class_0208a748.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov023 + 0xa60; KEEP(*libkh_*.a:ov023_strings_0208a75c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov023 + 0xa84);
		*libkh_*_ov023*.a:*(.data .data.*) __kh_data_end_ov023 = .;
		__kh_data_start_ov024 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov024 = .;
		. = __kh_dsdata_ov024 + 0x0; KEEP(*libkh_*.a:mobiclip_fastaudio.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0x13ac; KEEP(*libkh_*.a:ov024_vlc_variant1_020886c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0x34ac; KEEP(*libkh_*.a:ov024_vlc_variant0_0208a7c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0x55ac; KEEP(*libkh_*.a:mobiclip_payload.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xbb48; KEEP(*libkh_*.a:mobiclip_deblock.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc094; KEEP(*libkh_*.a:ov024_pair_saturation_020933ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc214; KEEP(*libkh_*.a:ov024_saturation_0209352c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc514; KEEP(*libkh_*.a:ov024_adpcm_0209382c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc5e8; KEEP(*libkh_*.a:ov024_player_state_02093900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc640; KEEP(*libkh_*.a:ov024_strings_02093958.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc65c; KEEP(*libkh_*.a:ov024_pointers_02093974.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc694; KEEP(*libkh_*.a:ov024_strings_020939ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov024 + 0xc6ac; KEEP(*libkh_*.a:ov024_pointers_020939c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov024 + 0xc708);
		*libkh_*_ov024*.a:*(.data .data.*) __kh_data_end_ov024 = .;
		__kh_data_start_ov025 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov025 = .;
		. = __kh_dsdata_ov025 + 0x0; KEEP(*libkh_*.a:ov025_limits_020b37ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x4; KEEP(*libkh_*.a:ov025_surface_020b37b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x18; KEEP(*libkh_*.a:ov025_tables_020b37c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x34; KEEP(*libkh_*.a:ov025_pointers_020b37e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x5c; KEEP(*libkh_*.a:ov025_tables_020b3808.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xc4; KEEP(*libkh_*.a:ov025_missionres_020b3870.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xd0; KEEP(*libkh_*.a:ov025_missionres_020b387c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xdc; KEEP(*libkh_*.a:ov025_tables_020b3888.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x178; KEEP(*libkh_*.a:ov025_strings_020b3924.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x188; KEEP(*libkh_*.a:ov025_pointers_020b3934.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x194; KEEP(*libkh_*.a:ov025_missionres_020b3940.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1a0; KEEP(*libkh_*.a:ov025_tables_020b394c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x434; KEEP(*libkh_*.a:ov025_strings_020b3be0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x4c4; KEEP(*libkh_*.a:ov025_tables_020b3c70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x500; KEEP(*libkh_*.a:ov025_pointers_020b3cac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x510; KEEP(*libkh_*.a:ov025_tables_020b3cbc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xa74; KEEP(*libkh_*.a:ov025_pointers_020b4220.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xa7c; KEEP(*libkh_*.a:ov025_reportsets_020b4228.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xa88; KEEP(*libkh_*.a:ov025_missionres_020b4234.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xa94; KEEP(*libkh_*.a:ov025_tables_020b4240.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xdd4; KEEP(*libkh_*.a:ov025_missionres_020b4580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xde0; KEEP(*libkh_*.a:ov025_missionres_020b458c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xdec; KEEP(*libkh_*.a:ov025_tables_020b4598.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xf14; KEEP(*libkh_*.a:ov025_missionres_020b46c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xf20; KEEP(*libkh_*.a:ov025_missionres_020b46cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0xf2c; KEEP(*libkh_*.a:ov025_tables_020b46d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x11bc; KEEP(*libkh_*.a:ov025_tables_020b4968.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1214; KEEP(*libkh_*.a:ov025_handle_020b49c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1218; KEEP(*libkh_*.a:ov025_class_020b49c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x122c; KEEP(*libkh_*.a:ov025_strings_020b49d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x12b8; KEEP(*libkh_*.a:ov025_tables_020b4a64.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x12cc; KEEP(*libkh_*.a:ov025_hookslots_020b4a78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1304; KEEP(*libkh_*.a:ov025_hookslots_020b4ab0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1340; KEEP(*libkh_*.a:ov025_class_020b4aec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1354; KEEP(*libkh_*.a:ov025_strings_020b4b00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1384; KEEP(*libkh_*.a:ov025_tables_020b4b30.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x13c4; KEEP(*libkh_*.a:ov025_screen_020b4b70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1408; KEEP(*libkh_*.a:ov025_tables_020b4bb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1478; KEEP(*libkh_*.a:ov025_strings_020b4c24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1508; KEEP(*libkh_*.a:ov025_screen_020b4cb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x154c; KEEP(*libkh_*.a:ov025_strings_020b4cf8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1564; KEEP(*libkh_*.a:ov025_zero_020b4d10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1568; KEEP(*libkh_*.a:ov025_strings_020b4d14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1580; KEEP(*libkh_*.a:ov025_tables_020b4d2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x15a0; KEEP(*libkh_*.a:ov025_screen_020b4d4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x15e4; KEEP(*libkh_*.a:ov025_tables_020b4d90.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1610; KEEP(*libkh_*.a:ov025_strings_020b4dbc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x163c; KEEP(*libkh_*.a:ov025_screen_020b4de8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1680; KEEP(*libkh_*.a:ov025_screen_020b4e2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x16c4; KEEP(*libkh_*.a:ov025_strings_020b4e70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1718; KEEP(*libkh_*.a:ov025_tables_020b4ec4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1774; KEEP(*libkh_*.a:ov025_screen_020b4f20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x17b8; KEEP(*libkh_*.a:ov025_menu_entries_020b4f64.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x19ac; KEEP(*libkh_*.a:ov025_strings_020b5158.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x19c0; KEEP(*libkh_*.a:ov025_screen_020b516c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1a04; KEEP(*libkh_*.a:ov025_strings_020b51b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1a4c; KEEP(*libkh_*.a:ov025_screen_020b51f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1a90; KEEP(*libkh_*.a:ov025_strings_020b523c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ac4; KEEP(*libkh_*.a:ov025_tables_020b5270.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ad8; KEEP(*libkh_*.a:ov025_page_020b5284.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1b18; KEEP(*libkh_*.a:ov025_tables_020b52c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1b28; KEEP(*libkh_*.a:ov025_strings_020b52d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1b40; KEEP(*libkh_*.a:ov025_tables_020b52ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1b50; KEEP(*libkh_*.a:ov025_strings_020b52fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1b64; KEEP(*libkh_*.a:ov025_page_020b5310.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ba4; KEEP(*libkh_*.a:ov025_strings_020b5350.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1bb0; KEEP(*libkh_*.a:ov025_tables_020b535c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1bc0; KEEP(*libkh_*.a:ov025_zero_020b536c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1bc4; KEEP(*libkh_*.a:ov025_tables_020b5370.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1bd4; KEEP(*libkh_*.a:ov025_strings_020b5380.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1c18; KEEP(*libkh_*.a:ov025_page_020b53c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1c58; KEEP(*libkh_*.a:ov025_strings_020b5404.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1c64; KEEP(*libkh_*.a:ov025_tables_020b5410.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1cc8; KEEP(*libkh_*.a:ov025_zero_020b5474.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ccc; KEEP(*libkh_*.a:ov025_tables_020b5478.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1cdc; KEEP(*libkh_*.a:ov025_strings_020b5488.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1d0c; KEEP(*libkh_*.a:ov025_page_020b54b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1d4c; KEEP(*libkh_*.a:ov025_strings_020b54f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1d5c; KEEP(*libkh_*.a:ov025_page_020b5508.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1d9c; KEEP(*libkh_*.a:ov025_page_020b5548.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ddc; KEEP(*libkh_*.a:ov025_tables_020b5588.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1ec8; KEEP(*libkh_*.a:ov025_strings_020b5674.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1f38; KEEP(*libkh_*.a:ov025_page_020b56e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov025 + 0x1f78; KEEP(*libkh_*.a:ov025_tables_020b5724.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov025 + 0x1f94);
		*libkh_*_ov025*.a:*(.data .data.*) __kh_data_end_ov025 = .;
		__kh_data_start_ov026 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov026 = .;
		. = __kh_dsdata_ov026 + 0x0; KEEP(*libkh_*.a:ov026_tables_020910b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x70; KEEP(*libkh_*.a:ov026_tracker_02091120.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x84; KEEP(*libkh_*.a:ov026_tables_02091134.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x114; KEEP(*libkh_*.a:ov026_msglist_020911c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x120; KEEP(*libkh_*.a:ov026_tables_020911d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x150; KEEP(*libkh_*.a:ov026_class_02091200.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x164; KEEP(*libkh_*.a:ov026_strings_02091214.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x180; KEEP(*libkh_*.a:ov026_class_02091230.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x194; KEEP(*libkh_*.a:ov026_strings_02091244.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x25c; KEEP(*libkh_*.a:ov026_tables_0209130c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov026 + 0x290; KEEP(*libkh_*.a:ov026_strings_02091340.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov026 + 0x2b0);
		*libkh_*_ov026*.a:*(.data .data.*) __kh_data_end_ov026 = .;
		__kh_data_start_ov027 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov027 = .;
		. = __kh_dsdata_ov027 + 0x0; KEEP(*libkh_*.a:ov027_tables_02083e04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0xdc; KEEP(*libkh_*.a:ov027_tables_02083ee0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0xf4; KEEP(*libkh_*.a:ov027_class_02083ef8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x108; KEEP(*libkh_*.a:ov027_pointers_02083f0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x120; KEEP(*libkh_*.a:ov027_tables_02083f24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x150; KEEP(*libkh_*.a:ov027_strings_02083f54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x198; KEEP(*libkh_*.a:ov027_tables_02083f9c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x1a0; KEEP(*libkh_*.a:ov027_strings_02083fa4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x2ec; KEEP(*libkh_*.a:ov027_tables_020840f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x344; KEEP(*libkh_*.a:ov027_strings_02084148.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x38c; KEEP(*libkh_*.a:ov027_pointers_02084190.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x3dc; KEEP(*libkh_*.a:ov027_strings_020841e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x434; KEEP(*libkh_*.a:ov027_tables_02084238.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov027 + 0x4f0; KEEP(*libkh_*.a:ov027_strings_020842f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov027 + 0x55c);
		*libkh_*_ov027*.a:*(.data .data.*) __kh_data_end_ov027 = .;
		__kh_data_start_ov028 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov028 = .;
		. = __kh_dsdata_ov028 + 0x0; KEEP(*libkh_*.a:dsprot_mac_owner.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov028 + 0x120; KEEP(*libkh_*.a:dsprot_rom_util.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov028 + 0x360; KEEP(*libkh_*.a:dsprot_rom_test.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov028 + 0x470; KEEP(*libkh_*.a:dsprot_main.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov028 + 0x990; KEEP(*libkh_*.a:ov028_dsprot_bad_mac_0208b560.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov028 + 0x998);
		*libkh_*_ov028*.a:*(.data .data.*) __kh_data_end_ov028 = .;
		__kh_data_start_ov029 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov029 = .;
		. = __kh_dsdata_ov029 + 0x0; KEEP(*libkh_*.a:ov029_pointers_020b2f70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov029 + 0x140; KEEP(*libkh_*.a:ov029_tables_020b30b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov029 + 0x290; KEEP(*libkh_*.a:ov029_tables_020b3200.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov029 + 0x2b0);
		*libkh_*_ov029*.a:*(.data .data.*) __kh_data_end_ov029 = .;
		__kh_data_start_ov030 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov030 = .;
		. = __kh_dsdata_ov030 + 0x0; KEEP(*libkh_*.a:ov030_strings_020b5860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov030 + 0x50; KEEP(*libkh_*.a:ov030_ctorargs_020b58b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov030 + 0x64; KEEP(*libkh_*.a:ov030_ctorargs_020b58c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov030 + 0x80; KEEP(*libkh_*.a:ov030_strings_020b58e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov030 + 0x94; KEEP(*libkh_*.a:ov030_class_020b58f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov030 + 0xa8; KEEP(*libkh_*.a:ov030_strings_020b5908.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov030 + 0x1a0);
		*libkh_*_ov030*.a:*(.data .data.*) __kh_data_end_ov030 = .;
		__kh_data_start_ov031 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov031 = .;
		. = __kh_dsdata_ov031 + 0x0; KEEP(*libkh_*.a:ov031_const_020b4c44.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov031 + 0x4; KEEP(*libkh_*.a:ov031_tables_020b4c48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov031 + 0x1c; KEEP(*libkh_*.a:ov031_strings_020b4c60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov031 + 0x7c; KEEP(*libkh_*.a:ov031_class_020b4cc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov031 + 0x90; KEEP(*libkh_*.a:ov031_pointers_020b4cd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov031 + 0xb8; KEEP(*libkh_*.a:ov031_strings_020b4cfc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov031 + 0x17c);
		*libkh_*_ov031*.a:*(.data .data.*) __kh_data_end_ov031 = .;
		__kh_data_start_ov032 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov032 = .;
		. = __kh_dsdata_ov032 + 0x0; KEEP(*libkh_*.a:ov032_tables_020b56e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov032 + 0x3c; KEEP(*libkh_*.a:ov032_ctorargs_020b5724.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov032 + 0x50; KEEP(*libkh_*.a:ov032_strings_020b5738.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov032 + 0xf8; KEEP(*libkh_*.a:ov032_strings_020b57e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov032 + 0x10c; KEEP(*libkh_*.a:ov032_class_020b57f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov032 + 0x120; KEEP(*libkh_*.a:ov032_strings_020b5808.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov032 + 0x1d8);
		*libkh_*_ov032*.a:*(.data .data.*) __kh_data_end_ov032 = .;
		__kh_data_start_ov033 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov033 = .;
		. = __kh_dsdata_ov033 + 0x0; KEEP(*libkh_*.a:ov033_strings_020b4aa8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov033 + 0x58; KEEP(*libkh_*.a:ov033_class_020b4b00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov033 + 0x6c; KEEP(*libkh_*.a:ov033_strings_020b4b14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov033 + 0xd8);
		*libkh_*_ov033*.a:*(.data .data.*) __kh_data_end_ov033 = .;
		__kh_data_start_ov034 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov034 = .;
		. = __kh_dsdata_ov034 + 0x0; KEEP(*libkh_*.a:ov034_tables_020b5570.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov034 + 0xc; KEEP(*libkh_*.a:ov034_strings_020b557c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov034 + 0x50; KEEP(*libkh_*.a:ov034_class_020b55c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov034 + 0x64; KEEP(*libkh_*.a:ov034_strings_020b55d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov034 + 0xf0);
		*libkh_*_ov034*.a:*(.data .data.*) __kh_data_end_ov034 = .;
		__kh_data_start_ov035 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov035 = .;
		. = __kh_dsdata_ov035 + 0x0; KEEP(*libkh_*.a:ov035_tables_020b4b60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0x18; KEEP(*libkh_*.a:ov035_ctorargs_020b4b78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0x2c; KEEP(*libkh_*.a:ov035_strings_020b4b8c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0x80; KEEP(*libkh_*.a:ov035_strings_020b4be0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0x94; KEEP(*libkh_*.a:ov035_class_020b4bf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0xa8; KEEP(*libkh_*.a:ov035_pointers_020b4c08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov035 + 0xbc; KEEP(*libkh_*.a:ov035_strings_020b4c1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov035 + 0x140);
		*libkh_*_ov035*.a:*(.data .data.*) __kh_data_end_ov035 = .;
		__kh_data_start_ov036 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov036 = .;
		. = __kh_dsdata_ov036 + 0x0; KEEP(*libkh_*.a:ov036_strings_020b4e08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov036 + 0x38; KEEP(*libkh_*.a:ov036_class_020b4e40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov036 + 0x4c; KEEP(*libkh_*.a:ov036_pointers_020b4e54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov036 + 0x60; KEEP(*libkh_*.a:ov036_strings_020b4e68.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov036 + 0x138);
		*libkh_*_ov036*.a:*(.data .data.*) __kh_data_end_ov036 = .;
		__kh_data_start_ov037 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov037 = .;
		. = __kh_dsdata_ov037 + 0x0; KEEP(*libkh_*.a:ov037_tables_020b4d04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov037 + 0xc; KEEP(*libkh_*.a:ov037_ctorargs_020b4d10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov037 + 0x20; KEEP(*libkh_*.a:ov037_strings_020b4d24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov037 + 0x7c; KEEP(*libkh_*.a:ov037_strings_020b4d80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov037 + 0x90; KEEP(*libkh_*.a:ov037_class_020b4d94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov037 + 0xa4; KEEP(*libkh_*.a:ov037_strings_020b4da8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov037 + 0x11c);
		*libkh_*_ov037*.a:*(.data .data.*) __kh_data_end_ov037 = .;
		__kh_data_start_ov038 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov038 = .;
		. = __kh_dsdata_ov038 + 0x0; KEEP(*libkh_*.a:ov038_strings_020b4b4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov038 + 0x40; KEEP(*libkh_*.a:ov038_ctorargs_020b4b8c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov038 + 0x74; KEEP(*libkh_*.a:ov038_strings_020b4bc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov038 + 0x88; KEEP(*libkh_*.a:ov038_class_020b4bd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov038 + 0x9c; KEEP(*libkh_*.a:ov038_strings_020b4be8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov038 + 0x154);
		*libkh_*_ov038*.a:*(.data .data.*) __kh_data_end_ov038 = .;
		__kh_data_start_ov039 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov039 = .;
		. = __kh_dsdata_ov039 + 0x0; KEEP(*libkh_*.a:ov039_tables_020b5390.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0xc; KEEP(*libkh_*.a:ov039_ctorargs_020b539c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x20; KEEP(*libkh_*.a:ov039_tables_020b53b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x74; KEEP(*libkh_*.a:ov039_strings_020b5404.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x1b0; KEEP(*libkh_*.a:ov039_strings_020b5540.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x1c4; KEEP(*libkh_*.a:ov039_class_020b5554.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x1d8; KEEP(*libkh_*.a:ov039_pointers_020b5568.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov039 + 0x1ec; KEEP(*libkh_*.a:ov039_strings_020b557c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov039 + 0x270);
		*libkh_*_ov039*.a:*(.data .data.*) __kh_data_end_ov039 = .;
		__kh_data_start_ov040 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov040 = .;
		. = __kh_dsdata_ov040 + 0x0; KEEP(*libkh_*.a:ov040_strings_020b4a04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov040 + 0x40; KEEP(*libkh_*.a:ov040_ctorargs_020b4a44.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov040 + 0x5c; KEEP(*libkh_*.a:ov040_strings_020b4a60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov040 + 0x70; KEEP(*libkh_*.a:ov040_class_020b4a74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov040 + 0x84; KEEP(*libkh_*.a:ov040_strings_020b4a88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov040 + 0x11c);
		*libkh_*_ov040*.a:*(.data .data.*) __kh_data_end_ov040 = .;
		__kh_data_start_ov041 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov041 = .;
		. = __kh_dsdata_ov041 + 0x0; KEEP(*libkh_*.a:ov041_strings_020b4c0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov041 + 0x54; KEEP(*libkh_*.a:ov041_class_020b4c60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov041 + 0x68; KEEP(*libkh_*.a:ov041_strings_020b4c74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov041 + 0xf4);
		*libkh_*_ov041*.a:*(.data .data.*) __kh_data_end_ov041 = .;
		__kh_data_start_ov042 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov042 = .;
		. = __kh_dsdata_ov042 + 0x0; KEEP(*libkh_*.a:ov042_tables_020b4690.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov042 + 0x24; KEEP(*libkh_*.a:ov042_strings_020b46b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov042 + 0x64; KEEP(*libkh_*.a:ov042_tables_020b46f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov042 + 0xb0; KEEP(*libkh_*.a:ov042_class_020b4740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov042 + 0xc4; KEEP(*libkh_*.a:ov042_pointers_020b4754.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov042 + 0xd8; KEEP(*libkh_*.a:ov042_strings_020b4768.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov042 + 0x170);
		*libkh_*_ov042*.a:*(.data .data.*) __kh_data_end_ov042 = .;
		__kh_data_start_ov043 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov043 = .;
		. = __kh_dsdata_ov043 + 0x0; KEEP(*libkh_*.a:ov043_strings_020b573c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov043 + 0x30; KEEP(*libkh_*.a:ov043_tables_020b576c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov043 + 0x98; KEEP(*libkh_*.a:ov043_strings_020b57d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov043 + 0xc4; KEEP(*libkh_*.a:ov043_class_020b5800.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov043 + 0xd8; KEEP(*libkh_*.a:ov043_pointers_020b5814.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov043 + 0xec; KEEP(*libkh_*.a:ov043_strings_020b5828.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov043 + 0x1a4);
		*libkh_*_ov043*.a:*(.data .data.*) __kh_data_end_ov043 = .;
		__kh_data_start_ov044 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov044 = .;
		. = __kh_dsdata_ov044 + 0x0; KEEP(*libkh_*.a:ov044_strings_020b5494.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov044 + 0x20; KEEP(*libkh_*.a:ov044_const_020b54b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov044 + 0x24; KEEP(*libkh_*.a:ov044_strings_020b54b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov044 + 0x40; KEEP(*libkh_*.a:ov044_ctorargs_020b54d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov044 + 0x6c; KEEP(*libkh_*.a:ov044_class_020b5500.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov044 + 0x80; KEEP(*libkh_*.a:ov044_strings_020b5514.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov044 + 0x18c);
		*libkh_*_ov044*.a:*(.data .data.*) __kh_data_end_ov044 = .;
		__kh_data_start_ov045 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov045 = .;
		. = __kh_dsdata_ov045 + 0x0; KEEP(*libkh_*.a:ov045_strings_020b4a7c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov045 + 0x30; KEEP(*libkh_*.a:ov045_tables_020b4aac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov045 + 0x104; KEEP(*libkh_*.a:ov045_class_020b4b80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov045 + 0x118; KEEP(*libkh_*.a:ov045_pointers_020b4b94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov045 + 0x12c; KEEP(*libkh_*.a:ov045_strings_020b4ba8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov045 + 0x1a4);
		*libkh_*_ov045*.a:*(.data .data.*) __kh_data_end_ov045 = .;
		__kh_data_start_ov046 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov046 = .;
		. = __kh_dsdata_ov046 + 0x0; KEEP(*libkh_*.a:ov046_strings_020b4a58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov046 + 0x48; KEEP(*libkh_*.a:ov046_class_020b4aa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov046 + 0x5c; KEEP(*libkh_*.a:ov046_pointers_020b4ab4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov046 + 0x70; KEEP(*libkh_*.a:ov046_strings_020b4ac8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov046 + 0xe8);
		*libkh_*_ov046*.a:*(.data .data.*) __kh_data_end_ov046 = .;
		__kh_data_start_ov047 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov047 = .;
		. = __kh_dsdata_ov047 + 0x0; KEEP(*libkh_*.a:ov047_strings_020b420c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov047 + 0x40; KEEP(*libkh_*.a:ov047_tables_020b424c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov047 + 0xd4; KEEP(*libkh_*.a:ov047_pointers_020b42e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov047 + 0xe8; KEEP(*libkh_*.a:ov047_class_020b42f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov047 + 0xfc; KEEP(*libkh_*.a:ov047_strings_020b4308.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov047 + 0x174);
		*libkh_*_ov047*.a:*(.data .data.*) __kh_data_end_ov047 = .;
		__kh_data_start_ov048 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov048 = .;
		. = __kh_dsdata_ov048 + 0x0; KEEP(*libkh_*.a:ov048_strings_020b4a4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov048 + 0x40; KEEP(*libkh_*.a:ov048_ctorargs_020b4a8c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov048 + 0x74; KEEP(*libkh_*.a:ov048_strings_020b4ac0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov048 + 0x88; KEEP(*libkh_*.a:ov048_class_020b4ad4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov048 + 0x9c; KEEP(*libkh_*.a:ov048_strings_020b4ae8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov048 + 0x134);
		*libkh_*_ov048*.a:*(.data .data.*) __kh_data_end_ov048 = .;
		__kh_data_start_ov049 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov049 = .;
		. = __kh_dsdata_ov049 + 0x0; KEEP(*libkh_*.a:ov049_strings_020b4af4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x50; KEEP(*libkh_*.a:ov049_ctorargs_020b4b44.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x64; KEEP(*libkh_*.a:ov049_tables_020b4b58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x12c; KEEP(*libkh_*.a:ov049_strings_020b4c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x140; KEEP(*libkh_*.a:ov049_class_020b4c34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x154; KEEP(*libkh_*.a:ov049_pointers_020b4c48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov049 + 0x168; KEEP(*libkh_*.a:ov049_strings_020b4c5c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov049 + 0x20c);
		*libkh_*_ov049*.a:*(.data .data.*) __kh_data_end_ov049 = .;
		__kh_data_start_ov050 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov050 = .;
		. = __kh_dsdata_ov050 + 0x0; KEEP(*libkh_*.a:ov050_const_020b7444.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov050 + 0x4; KEEP(*libkh_*.a:ov050_tables_020b7448.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov050 + 0x1c; KEEP(*libkh_*.a:ov050_strings_020b7460.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov050 + 0x7c; KEEP(*libkh_*.a:ov050_class_020b74c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov050 + 0x90; KEEP(*libkh_*.a:ov050_pointers_020b74d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov050 + 0xb8; KEEP(*libkh_*.a:ov050_strings_020b74fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov050 + 0x17c);
		*libkh_*_ov050*.a:*(.data .data.*) __kh_data_end_ov050 = .;
		__kh_data_start_ov051 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov051 = .;
		. = __kh_dsdata_ov051 + 0x0; KEEP(*libkh_*.a:ov051_strings_020b72a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov051 + 0x58; KEEP(*libkh_*.a:ov051_class_020b7300.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov051 + 0x6c; KEEP(*libkh_*.a:ov051_strings_020b7314.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov051 + 0xd8);
		*libkh_*_ov051*.a:*(.data .data.*) __kh_data_end_ov051 = .;
		__kh_data_start_ov052 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov052 = .;
		. = __kh_dsdata_ov052 + 0x0; KEEP(*libkh_*.a:ov052_tables_020b7ee8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov052 + 0x3c; KEEP(*libkh_*.a:ov052_ctorargs_020b7f24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov052 + 0x50; KEEP(*libkh_*.a:ov052_strings_020b7f38.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov052 + 0xf8; KEEP(*libkh_*.a:ov052_strings_020b7fe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov052 + 0x10c; KEEP(*libkh_*.a:ov052_class_020b7ff4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov052 + 0x120; KEEP(*libkh_*.a:ov052_strings_020b8008.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov052 + 0x1d8);
		*libkh_*_ov052*.a:*(.data .data.*) __kh_data_end_ov052 = .;
		__kh_data_start_ov053 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov053 = .;
		. = __kh_dsdata_ov053 + 0x0; KEEP(*libkh_*.a:ov053_tables_020b7d70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov053 + 0xc; KEEP(*libkh_*.a:ov053_strings_020b7d7c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov053 + 0x50; KEEP(*libkh_*.a:ov053_class_020b7dc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov053 + 0x64; KEEP(*libkh_*.a:ov053_strings_020b7dd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov053 + 0xf0);
		*libkh_*_ov053*.a:*(.data .data.*) __kh_data_end_ov053 = .;
		__kh_data_start_ov054 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov054 = .;
		. = __kh_dsdata_ov054 + 0x0; KEEP(*libkh_*.a:ov054_tables_020b7360.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0x18; KEEP(*libkh_*.a:ov054_ctorargs_020b7378.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0x2c; KEEP(*libkh_*.a:ov054_strings_020b738c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0x80; KEEP(*libkh_*.a:ov054_strings_020b73e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0x94; KEEP(*libkh_*.a:ov054_class_020b73f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0xa8; KEEP(*libkh_*.a:ov054_pointers_020b7408.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov054 + 0xbc; KEEP(*libkh_*.a:ov054_strings_020b741c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov054 + 0x140);
		*libkh_*_ov054*.a:*(.data .data.*) __kh_data_end_ov054 = .;
		__kh_data_start_ov055 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov055 = .;
		. = __kh_dsdata_ov055 + 0x0; KEEP(*libkh_*.a:ov055_strings_020b7608.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov055 + 0x38; KEEP(*libkh_*.a:ov055_class_020b7640.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov055 + 0x4c; KEEP(*libkh_*.a:ov055_pointers_020b7654.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov055 + 0x60; KEEP(*libkh_*.a:ov055_strings_020b7668.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov055 + 0x138);
		*libkh_*_ov055*.a:*(.data .data.*) __kh_data_end_ov055 = .;
		__kh_data_start_ov056 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov056 = .;
		. = __kh_dsdata_ov056 + 0x0; KEEP(*libkh_*.a:ov056_tables_020b7504.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov056 + 0xc; KEEP(*libkh_*.a:ov056_ctorargs_020b7510.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov056 + 0x20; KEEP(*libkh_*.a:ov056_strings_020b7524.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov056 + 0x7c; KEEP(*libkh_*.a:ov056_strings_020b7580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov056 + 0x90; KEEP(*libkh_*.a:ov056_class_020b7594.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov056 + 0xa4; KEEP(*libkh_*.a:ov056_strings_020b75a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov056 + 0x11c);
		*libkh_*_ov056*.a:*(.data .data.*) __kh_data_end_ov056 = .;
		__kh_data_start_ov057 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov057 = .;
		. = __kh_dsdata_ov057 + 0x0; KEEP(*libkh_*.a:ov057_strings_020b734c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov057 + 0x40; KEEP(*libkh_*.a:ov057_descriptors.o(.rodata .rodata.*))
		. = __kh_dsdata_ov057 + 0x74; KEEP(*libkh_*.a:ov057_strings_020b73c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov057 + 0x88; KEEP(*libkh_*.a:ov057_descriptors.o(.data .data.* .bss .bss.*))
		. = __kh_dsdata_ov057 + 0x9c; KEEP(*libkh_*.a:ov057_strings_020b73e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov057 + 0x154);
		*libkh_*_ov057*.a:*(.data .data.*) __kh_data_end_ov057 = .;
		__kh_data_start_ov058 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov058 = .;
		. = __kh_dsdata_ov058 + 0x0; KEEP(*libkh_*.a:ov058_tables_020b7b90.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0xc; KEEP(*libkh_*.a:ov058_ctorargs_020b7b9c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x20; KEEP(*libkh_*.a:ov058_tables_020b7bb0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x74; KEEP(*libkh_*.a:ov058_strings_020b7c04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x1b0; KEEP(*libkh_*.a:ov058_strings_020b7d40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x1c4; KEEP(*libkh_*.a:ov058_class_020b7d54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x1d8; KEEP(*libkh_*.a:ov058_pointers_020b7d68.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov058 + 0x1ec; KEEP(*libkh_*.a:ov058_strings_020b7d7c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov058 + 0x270);
		*libkh_*_ov058*.a:*(.data .data.*) __kh_data_end_ov058 = .;
		__kh_data_start_ov059 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov059 = .;
		. = __kh_dsdata_ov059 + 0x0; KEEP(*libkh_*.a:ov059_strings_020b7204.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov059 + 0x40; KEEP(*libkh_*.a:ov059_ctorargs_020b7244.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov059 + 0x5c; KEEP(*libkh_*.a:ov059_strings_020b7260.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov059 + 0x70; KEEP(*libkh_*.a:ov059_class_020b7274.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov059 + 0x84; KEEP(*libkh_*.a:ov059_strings_020b7288.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov059 + 0x11c);
		*libkh_*_ov059*.a:*(.data .data.*) __kh_data_end_ov059 = .;
		__kh_data_start_ov060 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov060 = .;
		. = __kh_dsdata_ov060 + 0x0; KEEP(*libkh_*.a:ov060_strings_020b740c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov060 + 0x54; KEEP(*libkh_*.a:ov060_class_020b7460.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov060 + 0x68; KEEP(*libkh_*.a:ov060_strings_020b7474.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov060 + 0xf4);
		*libkh_*_ov060*.a:*(.data .data.*) __kh_data_end_ov060 = .;
		__kh_data_start_ov061 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov061 = .;
		. = __kh_dsdata_ov061 + 0x0; KEEP(*libkh_*.a:ov061_tables_020b6e90.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov061 + 0x24; KEEP(*libkh_*.a:ov061_strings_020b6eb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov061 + 0x64; KEEP(*libkh_*.a:ov061_tables_020b6ef4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov061 + 0xb0; KEEP(*libkh_*.a:ov061_class_020b6f40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov061 + 0xc4; KEEP(*libkh_*.a:ov061_pointers_020b6f54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov061 + 0xd8; KEEP(*libkh_*.a:ov061_strings_020b6f68.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov061 + 0x170);
		*libkh_*_ov061*.a:*(.data .data.*) __kh_data_end_ov061 = .;
		__kh_data_start_ov062 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov062 = .;
		. = __kh_dsdata_ov062 + 0x0; KEEP(*libkh_*.a:ov062_strings_020b7f3c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov062 + 0x30; KEEP(*libkh_*.a:ov062_tables_020b7f6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov062 + 0x98; KEEP(*libkh_*.a:ov062_strings_020b7fd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov062 + 0xc4; KEEP(*libkh_*.a:ov062_class_020b8000.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov062 + 0xd8; KEEP(*libkh_*.a:ov062_pointers_020b8014.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov062 + 0xec; KEEP(*libkh_*.a:ov062_strings_020b8028.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov062 + 0x1a4);
		*libkh_*_ov062*.a:*(.data .data.*) __kh_data_end_ov062 = .;
		__kh_data_start_ov063 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov063 = .;
		. = __kh_dsdata_ov063 + 0x0; KEEP(*libkh_*.a:ov063_strings_020b7c94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov063 + 0x40; KEEP(*libkh_*.a:ov063_ctorargs_020b7cd4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov063 + 0x6c; KEEP(*libkh_*.a:ov063_class_020b7d00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov063 + 0x80; KEEP(*libkh_*.a:ov063_strings_020b7d14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov063 + 0x18c);
		*libkh_*_ov063*.a:*(.data .data.*) __kh_data_end_ov063 = .;
		__kh_data_start_ov064 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov064 = .;
		. = __kh_dsdata_ov064 + 0x0; KEEP(*libkh_*.a:ov064_strings_020b727c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov064 + 0x30; KEEP(*libkh_*.a:ov064_tables_020b72ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov064 + 0x104; KEEP(*libkh_*.a:ov064_class_020b7380.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov064 + 0x118; KEEP(*libkh_*.a:ov064_pointers_020b7394.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov064 + 0x12c; KEEP(*libkh_*.a:ov064_strings_020b73a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov064 + 0x1a4);
		*libkh_*_ov064*.a:*(.data .data.*) __kh_data_end_ov064 = .;
		__kh_data_start_ov065 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov065 = .;
		. = __kh_dsdata_ov065 + 0x0; KEEP(*libkh_*.a:ov065_strings_020b7258.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov065 + 0x48; KEEP(*libkh_*.a:ov065_class_020b72a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov065 + 0x5c; KEEP(*libkh_*.a:ov065_pointers_020b72b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov065 + 0x70; KEEP(*libkh_*.a:ov065_strings_020b72c8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov065 + 0xe8);
		*libkh_*_ov065*.a:*(.data .data.*) __kh_data_end_ov065 = .;
		__kh_data_start_ov066 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov066 = .;
		. = __kh_dsdata_ov066 + 0x0; KEEP(*libkh_*.a:ov066_strings_020b6a0c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov066 + 0x40; KEEP(*libkh_*.a:ov066_tables_020b6a4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov066 + 0xd4; KEEP(*libkh_*.a:ov066_pointers_020b6ae0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov066 + 0xe8; KEEP(*libkh_*.a:ov066_class_020b6af4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov066 + 0xfc; KEEP(*libkh_*.a:ov066_strings_020b6b08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov066 + 0x174);
		*libkh_*_ov066*.a:*(.data .data.*) __kh_data_end_ov066 = .;
		__kh_data_start_ov067 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov067 = .;
		. = __kh_dsdata_ov067 + 0x0; KEEP(*libkh_*.a:ov067_strings_020b724c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov067 + 0x40; KEEP(*libkh_*.a:ov067_ctorargs_020b728c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov067 + 0x74; KEEP(*libkh_*.a:ov067_strings_020b72c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov067 + 0x88; KEEP(*libkh_*.a:ov067_class_020b72d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov067 + 0x9c; KEEP(*libkh_*.a:ov067_strings_020b72e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov067 + 0x134);
		*libkh_*_ov067*.a:*(.data .data.*) __kh_data_end_ov067 = .;
		__kh_data_start_ov068 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov068 = .;
		. = __kh_dsdata_ov068 + 0x0; KEEP(*libkh_*.a:ov068_strings_020b72f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x50; KEEP(*libkh_*.a:ov068_ctorargs_020b7344.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x64; KEEP(*libkh_*.a:ov068_tables_020b7358.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x12c; KEEP(*libkh_*.a:ov068_strings_020b7420.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x140; KEEP(*libkh_*.a:ov068_class_020b7434.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x154; KEEP(*libkh_*.a:ov068_pointers_020b7448.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov068 + 0x168; KEEP(*libkh_*.a:ov068_strings_020b745c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov068 + 0x20c);
		*libkh_*_ov068*.a:*(.data .data.*) __kh_data_end_ov068 = .;
		__kh_data_start_ov069 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov069 = .;
		. = __kh_dsdata_ov069 + 0x0; KEEP(*libkh_*.a:ov069_tables_020ba7d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x1c; KEEP(*libkh_*.a:ov069_msglist_020ba7ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x28; KEEP(*libkh_*.a:ov069_tables_020ba7f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x1e8; KEEP(*libkh_*.a:ov069_tables_020ba9b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x250; KEEP(*libkh_*.a:ov069_pointers_020baa20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x2a0; KEEP(*libkh_*.a:ov069_strings_020baa70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x2b4; KEEP(*libkh_*.a:ov069_pointers_020baa84.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov069 + 0x2e4; KEEP(*libkh_*.a:ov069_strings_020baab4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov069 + 0x310);
		*libkh_*_ov069*.a:*(.data .data.*) __kh_data_end_ov069 = .;
		__kh_data_start_ov070 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov070 = .;
		. = __kh_dsdata_ov070 + 0x0; KEEP(*libkh_*.a:ov070_const_020b9b24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov070 + 0x4; KEEP(*libkh_*.a:ov070_tables_020b9b28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov070 + 0x1c; KEEP(*libkh_*.a:ov070_strings_020b9b40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov070 + 0x7c; KEEP(*libkh_*.a:ov070_class_020b9ba0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov070 + 0x90; KEEP(*libkh_*.a:ov070_pointers_020b9bb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov070 + 0xb8; KEEP(*libkh_*.a:ov070_strings_020b9bdc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov070 + 0x17c);
		*libkh_*_ov070*.a:*(.data .data.*) __kh_data_end_ov070 = .;
		__kh_data_start_ov071 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov071 = .;
		. = __kh_dsdata_ov071 + 0x0; KEEP(*libkh_*.a:ov071_strings_020b9988.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov071 + 0x58; KEEP(*libkh_*.a:ov071_class_020b99e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov071 + 0x6c; KEEP(*libkh_*.a:ov071_strings_020b99f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov071 + 0xd8);
		*libkh_*_ov071*.a:*(.data .data.*) __kh_data_end_ov071 = .;
		__kh_data_start_ov072 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov072 = .;
		. = __kh_dsdata_ov072 + 0x0; KEEP(*libkh_*.a:ov072_tables_020ba5c8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov072 + 0x3c; KEEP(*libkh_*.a:ov072_ctorargs_020ba604.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov072 + 0x50; KEEP(*libkh_*.a:ov072_strings_020ba618.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov072 + 0xf8; KEEP(*libkh_*.a:ov072_strings_020ba6c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov072 + 0x10c; KEEP(*libkh_*.a:ov072_class_020ba6d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov072 + 0x120; KEEP(*libkh_*.a:ov072_strings_020ba6e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov072 + 0x1d8);
		*libkh_*_ov072*.a:*(.data .data.*) __kh_data_end_ov072 = .;
		__kh_data_start_ov073 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov073 = .;
		. = __kh_dsdata_ov073 + 0x0; KEEP(*libkh_*.a:ov073_tables_020ba450.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov073 + 0xc; KEEP(*libkh_*.a:ov073_strings_020ba45c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov073 + 0x50; KEEP(*libkh_*.a:ov073_class_020ba4a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov073 + 0x64; KEEP(*libkh_*.a:ov073_strings_020ba4b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov073 + 0xf0);
		*libkh_*_ov073*.a:*(.data .data.*) __kh_data_end_ov073 = .;
		__kh_data_start_ov074 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov074 = .;
		. = __kh_dsdata_ov074 + 0x0; KEEP(*libkh_*.a:ov074_tables_020b9a40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0x18; KEEP(*libkh_*.a:ov074_ctorargs_020b9a58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0x2c; KEEP(*libkh_*.a:ov074_strings_020b9a6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0x80; KEEP(*libkh_*.a:ov074_strings_020b9ac0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0x94; KEEP(*libkh_*.a:ov074_class_020b9ad4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0xa8; KEEP(*libkh_*.a:ov074_pointers_020b9ae8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov074 + 0xbc; KEEP(*libkh_*.a:ov074_strings_020b9afc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov074 + 0x140);
		*libkh_*_ov074*.a:*(.data .data.*) __kh_data_end_ov074 = .;
		__kh_data_start_ov075 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov075 = .;
		. = __kh_dsdata_ov075 + 0x0; KEEP(*libkh_*.a:ov075_strings_020b9ce8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov075 + 0x38; KEEP(*libkh_*.a:ov075_class_020b9d20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov075 + 0x4c; KEEP(*libkh_*.a:ov075_pointers_020b9d34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov075 + 0x60; KEEP(*libkh_*.a:ov075_strings_020b9d48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov075 + 0x138);
		*libkh_*_ov075*.a:*(.data .data.*) __kh_data_end_ov075 = .;
		__kh_data_start_ov076 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov076 = .;
		. = __kh_dsdata_ov076 + 0x0; KEEP(*libkh_*.a:ov076_tables_020b9be4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov076 + 0xc; KEEP(*libkh_*.a:ov076_ctorargs_020b9bf0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov076 + 0x20; KEEP(*libkh_*.a:ov076_strings_020b9c04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov076 + 0x7c; KEEP(*libkh_*.a:ov076_strings_020b9c60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov076 + 0x90; KEEP(*libkh_*.a:ov076_class_020b9c74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov076 + 0xa4; KEEP(*libkh_*.a:ov076_strings_020b9c88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov076 + 0x11c);
		*libkh_*_ov076*.a:*(.data .data.*) __kh_data_end_ov076 = .;
		__kh_data_start_ov077 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov077 = .;
		. = __kh_dsdata_ov077 + 0x0; KEEP(*libkh_*.a:ov077_strings_020b9a2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov077 + 0x40; KEEP(*libkh_*.a:ov077_ctorargs_020b9a6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov077 + 0x74; KEEP(*libkh_*.a:ov077_strings_020b9aa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov077 + 0x88; KEEP(*libkh_*.a:ov077_class_020b9ab4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov077 + 0x9c; KEEP(*libkh_*.a:ov077_strings_020b9ac8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov077 + 0x154);
		*libkh_*_ov077*.a:*(.data .data.*) __kh_data_end_ov077 = .;
		__kh_data_start_ov078 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov078 = .;
		. = __kh_dsdata_ov078 + 0x0; KEEP(*libkh_*.a:ov078_tables_020ba270.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0xc; KEEP(*libkh_*.a:ov078_ctorargs_020ba27c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x20; KEEP(*libkh_*.a:ov078_tables_020ba290.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x74; KEEP(*libkh_*.a:ov078_strings_020ba2e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x1b0; KEEP(*libkh_*.a:ov078_strings_020ba420.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x1c4; KEEP(*libkh_*.a:ov078_class_020ba434.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x1d8; KEEP(*libkh_*.a:ov078_pointers_020ba448.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov078 + 0x1ec; KEEP(*libkh_*.a:ov078_strings_020ba45c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov078 + 0x270);
		*libkh_*_ov078*.a:*(.data .data.*) __kh_data_end_ov078 = .;
		__kh_data_start_ov079 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov079 = .;
		. = __kh_dsdata_ov079 + 0x0; KEEP(*libkh_*.a:ov079_strings_020b98e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov079 + 0x40; KEEP(*libkh_*.a:ov079_ctorargs_020b9924.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov079 + 0x5c; KEEP(*libkh_*.a:ov079_strings_020b9940.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov079 + 0x70; KEEP(*libkh_*.a:ov079_class_020b9954.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov079 + 0x84; KEEP(*libkh_*.a:ov079_strings_020b9968.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov079 + 0x11c);
		*libkh_*_ov079*.a:*(.data .data.*) __kh_data_end_ov079 = .;
		__kh_data_start_ov080 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov080 = .;
		. = __kh_dsdata_ov080 + 0x0; KEEP(*libkh_*.a:ov080_strings_020b9aec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov080 + 0x54; KEEP(*libkh_*.a:ov080_class_020b9b40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov080 + 0x68; KEEP(*libkh_*.a:ov080_strings_020b9b54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov080 + 0xf4);
		*libkh_*_ov080*.a:*(.data .data.*) __kh_data_end_ov080 = .;
		__kh_data_start_ov081 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov081 = .;
		. = __kh_dsdata_ov081 + 0x0; KEEP(*libkh_*.a:ov081_tables_020b9570.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov081 + 0x24; KEEP(*libkh_*.a:ov081_strings_020b9594.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov081 + 0x64; KEEP(*libkh_*.a:ov081_tables_020b95d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov081 + 0xb0; KEEP(*libkh_*.a:ov081_class_020b9620.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov081 + 0xc4; KEEP(*libkh_*.a:ov081_pointers_020b9634.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov081 + 0xd8; KEEP(*libkh_*.a:ov081_strings_020b9648.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov081 + 0x170);
		*libkh_*_ov081*.a:*(.data .data.*) __kh_data_end_ov081 = .;
		__kh_data_start_ov082 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov082 = .;
		. = __kh_dsdata_ov082 + 0x0; KEEP(*libkh_*.a:ov082_strings_020ba374.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov082 + 0x40; KEEP(*libkh_*.a:ov082_ctorargs_020ba3b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov082 + 0x6c; KEEP(*libkh_*.a:ov082_class_020ba3e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov082 + 0x80; KEEP(*libkh_*.a:ov082_strings_020ba3f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov082 + 0x18c);
		*libkh_*_ov082*.a:*(.data .data.*) __kh_data_end_ov082 = .;
		__kh_data_start_ov083 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov083 = .;
		. = __kh_dsdata_ov083 + 0x0; KEEP(*libkh_*.a:ov083_strings_020b995c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov083 + 0x30; KEEP(*libkh_*.a:ov083_tables_020b998c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov083 + 0x104; KEEP(*libkh_*.a:ov083_class_020b9a60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov083 + 0x118; KEEP(*libkh_*.a:ov083_pointers_020b9a74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov083 + 0x12c; KEEP(*libkh_*.a:ov083_strings_020b9a88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov083 + 0x1a4);
		*libkh_*_ov083*.a:*(.data .data.*) __kh_data_end_ov083 = .;
		__kh_data_start_ov084 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov084 = .;
		. = __kh_dsdata_ov084 + 0x0; KEEP(*libkh_*.a:ov084_strings_020b9938.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov084 + 0x48; KEEP(*libkh_*.a:ov084_class_020b9980.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov084 + 0x5c; KEEP(*libkh_*.a:ov084_pointers_020b9994.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov084 + 0x70; KEEP(*libkh_*.a:ov084_strings_020b99a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov084 + 0xe8);
		*libkh_*_ov084*.a:*(.data .data.*) __kh_data_end_ov084 = .;
		__kh_data_start_ov085 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov085 = .;
		. = __kh_dsdata_ov085 + 0x0; KEEP(*libkh_*.a:ov085_strings_020b90ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov085 + 0x40; KEEP(*libkh_*.a:ov085_tables_020b912c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov085 + 0xd4; KEEP(*libkh_*.a:ov085_pointers_020b91c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov085 + 0xe8; KEEP(*libkh_*.a:ov085_class_020b91d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov085 + 0xfc; KEEP(*libkh_*.a:ov085_strings_020b91e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov085 + 0x174);
		*libkh_*_ov085*.a:*(.data .data.*) __kh_data_end_ov085 = .;
		__kh_data_start_ov086 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov086 = .;
		. = __kh_dsdata_ov086 + 0x0; KEEP(*libkh_*.a:ov086_strings_020b992c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov086 + 0x40; KEEP(*libkh_*.a:ov086_ctorargs_020b996c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov086 + 0x74; KEEP(*libkh_*.a:ov086_strings_020b99a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov086 + 0x88; KEEP(*libkh_*.a:ov086_class_020b99b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov086 + 0x9c; KEEP(*libkh_*.a:ov086_strings_020b99c8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov086 + 0x134);
		*libkh_*_ov086*.a:*(.data .data.*) __kh_data_end_ov086 = .;
		__kh_data_start_ov087 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov087 = .;
		. = __kh_dsdata_ov087 + 0x0; KEEP(*libkh_*.a:ov087_strings_020b99d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x50; KEEP(*libkh_*.a:ov087_ctorargs_020b9a24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x64; KEEP(*libkh_*.a:ov087_tables_020b9a38.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x12c; KEEP(*libkh_*.a:ov087_strings_020b9b00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x140; KEEP(*libkh_*.a:ov087_class_020b9b14.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x154; KEEP(*libkh_*.a:ov087_pointers_020b9b28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov087 + 0x168; KEEP(*libkh_*.a:ov087_strings_020b9b3c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov087 + 0x20c);
		*libkh_*_ov087*.a:*(.data .data.*) __kh_data_end_ov087 = .;
		__kh_data_start_ov088 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov088 = .;
		. = __kh_dsdata_ov088 + 0x0; KEEP(*libkh_*.a:ov088_const_020bc1e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov088 + 0x4; KEEP(*libkh_*.a:ov088_tables_020bc1e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov088 + 0x1c; KEEP(*libkh_*.a:ov088_strings_020bc200.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov088 + 0x7c; KEEP(*libkh_*.a:ov088_class_020bc260.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov088 + 0x90; KEEP(*libkh_*.a:ov088_pointers_020bc274.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov088 + 0xb8; KEEP(*libkh_*.a:ov088_strings_020bc29c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov088 + 0x17c);
		*libkh_*_ov088*.a:*(.data .data.*) __kh_data_end_ov088 = .;
		__kh_data_start_ov089 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov089 = .;
		. = __kh_dsdata_ov089 + 0x0; KEEP(*libkh_*.a:ov089_strings_020bc048.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov089 + 0x58; KEEP(*libkh_*.a:ov089_class_020bc0a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov089 + 0x6c; KEEP(*libkh_*.a:ov089_strings_020bc0b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov089 + 0xd8);
		*libkh_*_ov089*.a:*(.data .data.*) __kh_data_end_ov089 = .;
		__kh_data_start_ov090 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov090 = .;
		. = __kh_dsdata_ov090 + 0x0; KEEP(*libkh_*.a:ov090_tables_020bcb10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov090 + 0xc; KEEP(*libkh_*.a:ov090_strings_020bcb1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov090 + 0x50; KEEP(*libkh_*.a:ov090_class_020bcb60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov090 + 0x64; KEEP(*libkh_*.a:ov090_strings_020bcb74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov090 + 0xf0);
		*libkh_*_ov090*.a:*(.data .data.*) __kh_data_end_ov090 = .;
		__kh_data_start_ov091 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov091 = .;
		. = __kh_dsdata_ov091 + 0x0; KEEP(*libkh_*.a:ov091_tables_020bc100.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0x18; KEEP(*libkh_*.a:ov091_ctorargs_020bc118.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0x2c; KEEP(*libkh_*.a:ov091_strings_020bc12c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0x80; KEEP(*libkh_*.a:ov091_strings_020bc180.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0x94; KEEP(*libkh_*.a:ov091_class_020bc194.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0xa8; KEEP(*libkh_*.a:ov091_pointers_020bc1a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov091 + 0xbc; KEEP(*libkh_*.a:ov091_strings_020bc1bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov091 + 0x140);
		*libkh_*_ov091*.a:*(.data .data.*) __kh_data_end_ov091 = .;
		__kh_data_start_ov092 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov092 = .;
		. = __kh_dsdata_ov092 + 0x0; KEEP(*libkh_*.a:ov092_strings_020bc3a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov092 + 0x38; KEEP(*libkh_*.a:ov092_class_020bc3e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov092 + 0x4c; KEEP(*libkh_*.a:ov092_pointers_020bc3f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov092 + 0x60; KEEP(*libkh_*.a:ov092_strings_020bc408.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov092 + 0x138);
		*libkh_*_ov092*.a:*(.data .data.*) __kh_data_end_ov092 = .;
		__kh_data_start_ov093 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov093 = .;
		. = __kh_dsdata_ov093 + 0x0; KEEP(*libkh_*.a:ov093_tables_020bc2a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov093 + 0xc; KEEP(*libkh_*.a:ov093_ctorargs_020bc2b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov093 + 0x20; KEEP(*libkh_*.a:ov093_strings_020bc2c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov093 + 0x7c; KEEP(*libkh_*.a:ov093_strings_020bc320.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov093 + 0x90; KEEP(*libkh_*.a:ov093_class_020bc334.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov093 + 0xa4; KEEP(*libkh_*.a:ov093_strings_020bc348.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov093 + 0x11c);
		*libkh_*_ov093*.a:*(.data .data.*) __kh_data_end_ov093 = .;
		__kh_data_start_ov094 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov094 = .;
		. = __kh_dsdata_ov094 + 0x0; KEEP(*libkh_*.a:ov094_strings_020bc0ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov094 + 0x40; KEEP(*libkh_*.a:ov094_ctorargs_020bc12c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov094 + 0x74; KEEP(*libkh_*.a:ov094_strings_020bc160.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov094 + 0x88; KEEP(*libkh_*.a:ov094_class_020bc174.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov094 + 0x9c; KEEP(*libkh_*.a:ov094_strings_020bc188.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov094 + 0x154);
		*libkh_*_ov094*.a:*(.data .data.*) __kh_data_end_ov094 = .;
		__kh_data_start_ov095 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov095 = .;
		. = __kh_dsdata_ov095 + 0x0; KEEP(*libkh_*.a:ov095_tables_020bc930.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0xc; KEEP(*libkh_*.a:ov095_ctorargs_020bc93c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x20; KEEP(*libkh_*.a:ov095_tables_020bc950.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x74; KEEP(*libkh_*.a:ov095_strings_020bc9a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x1b0; KEEP(*libkh_*.a:ov095_strings_020bcae0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x1c4; KEEP(*libkh_*.a:ov095_class_020bcaf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x1d8; KEEP(*libkh_*.a:ov095_pointers_020bcb08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov095 + 0x1ec; KEEP(*libkh_*.a:ov095_strings_020bcb1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov095 + 0x270);
		*libkh_*_ov095*.a:*(.data .data.*) __kh_data_end_ov095 = .;
		__kh_data_start_ov096 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov096 = .;
		. = __kh_dsdata_ov096 + 0x0; KEEP(*libkh_*.a:ov096_strings_020bbfa4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov096 + 0x40; KEEP(*libkh_*.a:ov096_ctorargs_020bbfe4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov096 + 0x5c; KEEP(*libkh_*.a:ov096_strings_020bc000.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov096 + 0x70; KEEP(*libkh_*.a:ov096_class_020bc014.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov096 + 0x84; KEEP(*libkh_*.a:ov096_strings_020bc028.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov096 + 0x11c);
		*libkh_*_ov096*.a:*(.data .data.*) __kh_data_end_ov096 = .;
		__kh_data_start_ov097 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov097 = .;
		. = __kh_dsdata_ov097 + 0x0; KEEP(*libkh_*.a:ov097_strings_020bc1ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov097 + 0x54; KEEP(*libkh_*.a:ov097_class_020bc200.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov097 + 0x68; KEEP(*libkh_*.a:ov097_strings_020bc214.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov097 + 0xf4);
		*libkh_*_ov097*.a:*(.data .data.*) __kh_data_end_ov097 = .;
		__kh_data_start_ov098 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov098 = .;
		. = __kh_dsdata_ov098 + 0x0; KEEP(*libkh_*.a:ov098_tables_020bbc30.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov098 + 0x24; KEEP(*libkh_*.a:ov098_strings_020bbc54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov098 + 0x64; KEEP(*libkh_*.a:ov098_tables_020bbc94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov098 + 0xb0; KEEP(*libkh_*.a:ov098_class_020bbce0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov098 + 0xc4; KEEP(*libkh_*.a:ov098_pointers_020bbcf4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov098 + 0xd8; KEEP(*libkh_*.a:ov098_strings_020bbd08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov098 + 0x170);
		*libkh_*_ov098*.a:*(.data .data.*) __kh_data_end_ov098 = .;
		__kh_data_start_ov099 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov099 = .;
		. = __kh_dsdata_ov099 + 0x0; KEEP(*libkh_*.a:ov099_strings_020bca34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov099 + 0x40; KEEP(*libkh_*.a:ov099_ctorargs_020bca74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov099 + 0x6c; KEEP(*libkh_*.a:ov099_class_020bcaa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov099 + 0x80; KEEP(*libkh_*.a:ov099_strings_020bcab4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov099 + 0x18c);
		*libkh_*_ov099*.a:*(.data .data.*) __kh_data_end_ov099 = .;
		__kh_data_start_ov100 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov100 = .;
		. = __kh_dsdata_ov100 + 0x0; KEEP(*libkh_*.a:ov100_strings_020bc01c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov100 + 0x30; KEEP(*libkh_*.a:ov100_tables_020bc04c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov100 + 0x104; KEEP(*libkh_*.a:ov100_class_020bc120.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov100 + 0x118; KEEP(*libkh_*.a:ov100_pointers_020bc134.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov100 + 0x12c; KEEP(*libkh_*.a:ov100_strings_020bc148.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov100 + 0x1a4);
		*libkh_*_ov100*.a:*(.data .data.*) __kh_data_end_ov100 = .;
		__kh_data_start_ov101 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov101 = .;
		. = __kh_dsdata_ov101 + 0x0; KEEP(*libkh_*.a:ov101_strings_020bbff8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov101 + 0x48; KEEP(*libkh_*.a:ov101_class_020bc040.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov101 + 0x5c; KEEP(*libkh_*.a:ov101_pointers_020bc054.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov101 + 0x70; KEEP(*libkh_*.a:ov101_strings_020bc068.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov101 + 0xe8);
		*libkh_*_ov101*.a:*(.data .data.*) __kh_data_end_ov101 = .;
		__kh_data_start_ov102 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov102 = .;
		. = __kh_dsdata_ov102 + 0x0; KEEP(*libkh_*.a:ov102_strings_020bb7ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov102 + 0x40; KEEP(*libkh_*.a:ov102_tables_020bb7ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov102 + 0xd4; KEEP(*libkh_*.a:ov102_pointers_020bb880.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov102 + 0xe8; KEEP(*libkh_*.a:ov102_class_020bb894.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov102 + 0xfc; KEEP(*libkh_*.a:ov102_strings_020bb8a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov102 + 0x174);
		*libkh_*_ov102*.a:*(.data .data.*) __kh_data_end_ov102 = .;
		__kh_data_start_ov103 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov103 = .;
		. = __kh_dsdata_ov103 + 0x0; KEEP(*libkh_*.a:ov103_strings_020bbfec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov103 + 0x40; KEEP(*libkh_*.a:ov103_ctorargs_020bc02c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov103 + 0x74; KEEP(*libkh_*.a:ov103_strings_020bc060.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov103 + 0x88; KEEP(*libkh_*.a:ov103_class_020bc074.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov103 + 0x9c; KEEP(*libkh_*.a:ov103_strings_020bc088.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov103 + 0x134);
		*libkh_*_ov103*.a:*(.data .data.*) __kh_data_end_ov103 = .;
		__kh_data_start_ov104 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov104 = .;
		. = __kh_dsdata_ov104 + 0x0; KEEP(*libkh_*.a:ov104_strings_020bc094.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x50; KEEP(*libkh_*.a:ov104_ctorargs_020bc0e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x64; KEEP(*libkh_*.a:ov104_tables_020bc0f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x12c; KEEP(*libkh_*.a:ov104_strings_020bc1c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x140; KEEP(*libkh_*.a:ov104_class_020bc1d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x154; KEEP(*libkh_*.a:ov104_pointers_020bc1e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov104 + 0x168; KEEP(*libkh_*.a:ov104_strings_020bc1fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov104 + 0x20c);
		*libkh_*_ov104*.a:*(.data .data.*) __kh_data_end_ov104 = .;
		__kh_data_start_ov105 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov105 = .;
		. = __kh_dsdata_ov105 + 0x0; KEEP(*libkh_*.a:ov105_tables_020bfa00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov105 + 0x20);
		*libkh_*_ov105*.a:*(.data .data.*) __kh_data_end_ov105 = .;
		__kh_data_start_ov106 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov106 = .;
		. = __kh_dsdata_ov106 + 0x0; KEEP(*libkh_*.a:ov106_tables_020b8a6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov106 + 0x34; KEEP(*libkh_*.a:ov106_class_020b8aa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov106 + 0x48; KEEP(*libkh_*.a:ov106_strings_020b8ab4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov106 + 0x84; KEEP(*libkh_*.a:ov106_pointers_020b8af0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov106 + 0xb4; KEEP(*libkh_*.a:ov106_class_020b8b20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov106 + 0xc8; KEEP(*libkh_*.a:ov106_strings_020b8b34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov106 + 0xf4);
		*libkh_*_ov106*.a:*(.data .data.*) __kh_data_end_ov106 = .;
		__kh_data_start_ov107 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov107 = .;
		. = __kh_dsdata_ov107 + 0x0; KEEP(*libkh_*.a:ov107_pointers_020cb5f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x18; KEEP(*libkh_*.a:ov107_tables_020cb610.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x38; KEEP(*libkh_*.a:ov107_tables_020cb630.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x40; KEEP(*libkh_*.a:ov107_strings_020cb638.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x54; KEEP(*libkh_*.a:ov107_tables_020cb64c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0xa8; KEEP(*libkh_*.a:ov107_tables_020cb6a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x370; KEEP(*libkh_*.a:ov107_strings_020cb968.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x3ac; KEEP(*libkh_*.a:ov107_slotkinds_020cb9a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x4ac; KEEP(*libkh_*.a:ov107_class_020cbaa4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov107 + 0x4c0; KEEP(*libkh_*.a:ov107_strings_020cbab8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov107 + 0x508);
		*libkh_*_ov107*.a:*(.data .data.*) __kh_data_end_ov107 = .;
		__kh_data_start_ov108 = .; *libkh_*_ov108*.a:*(.data .data.*) __kh_data_end_ov108 = .;
		__kh_data_start_ov109 = .; *libkh_*_ov109*.a:*(.data .data.*) __kh_data_end_ov109 = .;
		__kh_data_start_ov110 = .; *libkh_*_ov110*.a:*(.data .data.*) __kh_data_end_ov110 = .;
		__kh_data_start_ov111 = .; *libkh_*_ov111*.a:*(.data .data.*) __kh_data_end_ov111 = .;
		__kh_data_start_ov112 = .; *libkh_*_ov112*.a:*(.data .data.*) __kh_data_end_ov112 = .;
		__kh_data_start_ov113 = .; *libkh_*_ov113*.a:*(.data .data.*) __kh_data_end_ov113 = .;
		__kh_data_start_ov114 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov114 = .;
		. = __kh_dsdata_ov114 + 0x0; KEEP(*libkh_*.a:ov114_tables_020cdfb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov114 + 0x2c; KEEP(*libkh_*.a:ov114_strings_020cdfe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov114 + 0x4c);
		*libkh_*_ov114*.a:*(.data .data.*) __kh_data_end_ov114 = .;
		__kh_data_start_ov115 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov115 = .;
		. = __kh_dsdata_ov115 + 0x0; KEEP(*libkh_*.a:ov115_tables_020ceb38.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov115 + 0x48; KEEP(*libkh_*.a:ov115_strings_020ceb80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov115 + 0x68);
		*libkh_*_ov115*.a:*(.data .data.*) __kh_data_end_ov115 = .;
		__kh_data_start_ov116 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov116 = .;
		. = __kh_dsdata_ov116 + 0x0; KEEP(*libkh_*.a:ov116_tables_020d2778.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov116 + 0x48; KEEP(*libkh_*.a:ov116_strings_020d27c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov116 + 0x68);
		*libkh_*_ov116*.a:*(.data .data.*) __kh_data_end_ov116 = .;
		__kh_data_start_ov117 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov117 = .;
		. = __kh_dsdata_ov117 + 0x0; KEEP(*libkh_*.a:ov117_tables_020cdc28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov117 + 0x38; KEEP(*libkh_*.a:ov117_const_020cdc60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov117 + 0x3c; KEEP(*libkh_*.a:ov117_strings_020cdc64.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov117 + 0x58);
		*libkh_*_ov117*.a:*(.data .data.*) __kh_data_end_ov117 = .;
		__kh_data_start_ov118 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov118 = .;
		. = __kh_dsdata_ov118 + 0x0; KEEP(*libkh_*.a:ov118_tables_020d1868.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov118 + 0x38; KEEP(*libkh_*.a:ov118_const_020d18a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov118 + 0x3c; KEEP(*libkh_*.a:ov118_strings_020d18a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov118 + 0x58);
		*libkh_*_ov118*.a:*(.data .data.*) __kh_data_end_ov118 = .;
		__kh_data_start_ov119 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov119 = .;
		. = __kh_dsdata_ov119 + 0x0; KEEP(*libkh_*.a:ov119_tables_020cf93c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov119 + 0x44; KEEP(*libkh_*.a:ov119_strings_020cf980.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov119 + 0xa4);
		*libkh_*_ov119*.a:*(.data .data.*) __kh_data_end_ov119 = .;
		__kh_data_start_ov120 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov120 = .;
		. = __kh_dsdata_ov120 + 0x0; KEEP(*libkh_*.a:ov120_tables_020cdee4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov120 + 0x3c; KEEP(*libkh_*.a:ov120_strings_020cdf20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov120 + 0x9c);
		*libkh_*_ov120*.a:*(.data .data.*) __kh_data_end_ov120 = .;
		__kh_data_start_ov121 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov121 = .;
		. = __kh_dsdata_ov121 + 0x0; KEEP(*libkh_*.a:ov121_tables_020cfd04.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov121 + 0x3c; KEEP(*libkh_*.a:ov121_strings_020cfd40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov121 + 0x9c);
		*libkh_*_ov121*.a:*(.data .data.*) __kh_data_end_ov121 = .;
		__kh_data_start_ov122 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov122 = .;
		. = __kh_dsdata_ov122 + 0x0; KEEP(*libkh_*.a:ov122_tables_020d1b24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov122 + 0x3c; KEEP(*libkh_*.a:ov122_strings_020d1b60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov122 + 0x9c);
		*libkh_*_ov122*.a:*(.data .data.*) __kh_data_end_ov122 = .;
		__kh_data_start_ov123 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov123 = .;
		. = __kh_dsdata_ov123 + 0x0; KEEP(*libkh_*.a:ov123_tables_020ce298.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov123 + 0x68; KEEP(*libkh_*.a:ov123_strings_020ce300.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov123 + 0x88);
		*libkh_*_ov123*.a:*(.data .data.*) __kh_data_end_ov123 = .;
		__kh_data_start_ov124 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov124 = .;
		. = __kh_dsdata_ov124 + 0x0; KEEP(*libkh_*.a:ov124_tables_020d1ed8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov124 + 0x68; KEEP(*libkh_*.a:ov124_strings_020d1f40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov124 + 0x88);
		*libkh_*_ov124*.a:*(.data .data.*) __kh_data_end_ov124 = .;
		__kh_data_start_ov125 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov125 = .;
		. = __kh_dsdata_ov125 + 0x0; KEEP(*libkh_*.a:ov125_tables_020d03a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov125 + 0x98; KEEP(*libkh_*.a:ov125_strings_020d0440.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov125 + 0xb8);
		*libkh_*_ov125*.a:*(.data .data.*) __kh_data_end_ov125 = .;
		__kh_data_start_ov126 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov126 = .;
		. = __kh_dsdata_ov126 + 0x0; KEEP(*libkh_*.a:ov126_tables_020d3fe8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov126 + 0x98; KEEP(*libkh_*.a:ov126_strings_020d4080.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov126 + 0xb8);
		*libkh_*_ov126*.a:*(.data .data.*) __kh_data_end_ov126 = .;
		__kh_data_start_ov127 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov127 = .;
		. = __kh_dsdata_ov127 + 0x0; KEEP(*libkh_*.a:ov127_const_020cd744.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov127 + 0x1c; KEEP(*libkh_*.a:ov127_strings_020cd760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov127 + 0x3c);
		*libkh_*_ov127*.a:*(.data .data.*) __kh_data_end_ov127 = .;
		__kh_data_start_ov128 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov128 = .;
		. = __kh_dsdata_ov128 + 0x0; KEEP(*libkh_*.a:ov128_const_020d4fc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov128 + 0x1c; KEEP(*libkh_*.a:ov128_strings_020d4fe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov128 + 0x3c);
		*libkh_*_ov128*.a:*(.data .data.*) __kh_data_end_ov128 = .;
		__kh_data_start_ov129 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov129 = .;
		. = __kh_dsdata_ov129 + 0x0; KEEP(*libkh_*.a:ov129_const_020d6de4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov129 + 0x1c; KEEP(*libkh_*.a:ov129_strings_020d6e00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov129 + 0x3c);
		*libkh_*_ov129*.a:*(.data .data.*) __kh_data_end_ov129 = .;
		__kh_data_start_ov130 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov130 = .;
		. = __kh_dsdata_ov130 + 0x0; KEEP(*libkh_*.a:ov130_const_020cd740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov130 + 0x20; KEEP(*libkh_*.a:ov130_strings_020cd760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov130 + 0x40);
		*libkh_*_ov130*.a:*(.data .data.*) __kh_data_end_ov130 = .;
		__kh_data_start_ov131 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov131 = .;
		. = __kh_dsdata_ov131 + 0x0; KEEP(*libkh_*.a:ov131_tables_020cef58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov131 + 0x88; KEEP(*libkh_*.a:ov131_strings_020cefe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov131 + 0xc8);
		*libkh_*_ov131*.a:*(.data .data.*) __kh_data_end_ov131 = .;
		__kh_data_start_ov132 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov132 = .;
		. = __kh_dsdata_ov132 + 0x0; KEEP(*libkh_*.a:ov132_tables_020d0d78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov132 + 0x88; KEEP(*libkh_*.a:ov132_strings_020d0e00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov132 + 0xc8);
		*libkh_*_ov132*.a:*(.data .data.*) __kh_data_end_ov132 = .;
		__kh_data_start_ov133 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov133 = .;
		. = __kh_dsdata_ov133 + 0x0; KEEP(*libkh_*.a:ov133_tables_020d49b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov133 + 0x88; KEEP(*libkh_*.a:ov133_strings_020d4a40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov133 + 0xc8);
		*libkh_*_ov133*.a:*(.data .data.*) __kh_data_end_ov133 = .;
		__kh_data_start_ov134 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov134 = .;
		. = __kh_dsdata_ov134 + 0x0; KEEP(*libkh_*.a:ov134_tables_020cdf78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov134 + 0x68; KEEP(*libkh_*.a:ov134_strings_020cdfe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov134 + 0x88; KEEP(*libkh_*.a:ov134_const_020ce000.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov134 + 0x8c; KEEP(*libkh_*.a:ov134_strings_020ce004.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov134 + 0xa8);
		*libkh_*_ov134*.a:*(.data .data.*) __kh_data_end_ov134 = .;
		__kh_data_start_ov135 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov135 = .;
		. = __kh_dsdata_ov135 + 0x0; KEEP(*libkh_*.a:ov135_tables_020d1bb8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov135 + 0x68; KEEP(*libkh_*.a:ov135_strings_020d1c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov135 + 0x88; KEEP(*libkh_*.a:ov135_const_020d1c40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov135 + 0x8c; KEEP(*libkh_*.a:ov135_strings_020d1c44.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov135 + 0xa8);
		*libkh_*_ov135*.a:*(.data .data.*) __kh_data_end_ov135 = .;
		__kh_data_start_ov136 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov136 = .;
		. = __kh_dsdata_ov136 + 0x0; KEEP(*libkh_*.a:ov136_tables_020d39d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov136 + 0x68; KEEP(*libkh_*.a:ov136_strings_020d3a40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov136 + 0x88; KEEP(*libkh_*.a:ov136_const_020d3a60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov136 + 0x8c; KEEP(*libkh_*.a:ov136_strings_020d3a64.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov136 + 0xa8);
		*libkh_*_ov136*.a:*(.data .data.*) __kh_data_end_ov136 = .;
		__kh_data_start_ov137 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov137 = .;
		. = __kh_dsdata_ov137 + 0x0; KEEP(*libkh_*.a:ov137_tables_020cf35c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov137 + 0x64; KEEP(*libkh_*.a:ov137_strings_020cf3c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov137 + 0x84);
		*libkh_*_ov137*.a:*(.data .data.*) __kh_data_end_ov137 = .;
		__kh_data_start_ov138 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov138 = .;
		. = __kh_dsdata_ov138 + 0x0; KEEP(*libkh_*.a:ov138_tables_020d2f9c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov138 + 0x64; KEEP(*libkh_*.a:ov138_strings_020d3000.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov138 + 0x84);
		*libkh_*_ov138*.a:*(.data .data.*) __kh_data_end_ov138 = .;
		__kh_data_start_ov139 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov139 = .;
		. = __kh_dsdata_ov139 + 0x0; KEEP(*libkh_*.a:ov139_tables_020cebfc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov139 + 0x84; KEEP(*libkh_*.a:ov139_strings_020cec80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov139 + 0xa4);
		*libkh_*_ov139*.a:*(.data .data.*) __kh_data_end_ov139 = .;
		__kh_data_start_ov140 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov140 = .;
		. = __kh_dsdata_ov140 + 0x0; KEEP(*libkh_*.a:ov140_tables_020d283c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov140 + 0x84; KEEP(*libkh_*.a:ov140_strings_020d28c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov140 + 0xa4);
		*libkh_*_ov140*.a:*(.data .data.*) __kh_data_end_ov140 = .;
		__kh_data_start_ov141 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov141 = .;
		. = __kh_dsdata_ov141 + 0x0; KEEP(*libkh_*.a:ov141_tables_020ce9bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov141 + 0x84; KEEP(*libkh_*.a:ov141_strings_020cea40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov141 + 0x90; KEEP(*libkh_*.a:ov141_const_020cea4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov141 + 0x94; KEEP(*libkh_*.a:ov141_strings_020cea50.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov141 + 0xc4);
		*libkh_*_ov141*.a:*(.data .data.*) __kh_data_end_ov141 = .;
		__kh_data_start_ov142 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov142 = .;
		. = __kh_dsdata_ov142 + 0x0; KEEP(*libkh_*.a:ov142_tables_020d25fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov142 + 0x84; KEEP(*libkh_*.a:ov142_strings_020d2680.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov142 + 0x90; KEEP(*libkh_*.a:ov142_const_020d268c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov142 + 0x94; KEEP(*libkh_*.a:ov142_strings_020d2690.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov142 + 0xc4);
		*libkh_*_ov142*.a:*(.data .data.*) __kh_data_end_ov142 = .;
		__kh_data_start_ov143 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov143 = .;
		. = __kh_dsdata_ov143 + 0x0; KEEP(*libkh_*.a:ov143_tables_020d623c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov143 + 0x84; KEEP(*libkh_*.a:ov143_strings_020d62c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov143 + 0x90; KEEP(*libkh_*.a:ov143_const_020d62cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov143 + 0x94; KEEP(*libkh_*.a:ov143_strings_020d62d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov143 + 0xc4);
		*libkh_*_ov143*.a:*(.data .data.*) __kh_data_end_ov143 = .;
		__kh_data_start_ov144 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov144 = .;
		. = __kh_dsdata_ov144 + 0x0; KEEP(*libkh_*.a:ov144_strings_020cdce0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov144 + 0x20);
		*libkh_*_ov144*.a:*(.data .data.*) __kh_data_end_ov144 = .;
		__kh_data_start_ov145 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov145 = .;
		. = __kh_dsdata_ov145 + 0x0; KEEP(*libkh_*.a:ov145_strings_020cfae0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov145 + 0x20);
		*libkh_*_ov145*.a:*(.data .data.*) __kh_data_end_ov145 = .;
		__kh_data_start_ov146 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov146 = .;
		. = __kh_dsdata_ov146 + 0x0; KEEP(*libkh_*.a:ov146_tables_020cf4f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov146 + 0x28; KEEP(*libkh_*.a:ov146_strings_020cf520.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov146 + 0x3c; KEEP(*libkh_*.a:ov146_tables_020cf534.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov146 + 0x48);
		*libkh_*_ov146*.a:*(.data .data.*) __kh_data_end_ov146 = .;
		__kh_data_start_ov147 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov147 = .;
		. = __kh_dsdata_ov147 + 0x0; KEEP(*libkh_*.a:ov147_tables_020ce8a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov147 + 0x20; KEEP(*libkh_*.a:ov147_strings_020ce8c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov147 + 0x40);
		*libkh_*_ov147*.a:*(.data .data.*) __kh_data_end_ov147 = .;
		__kh_data_start_ov148 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov148 = .;
		. = __kh_dsdata_ov148 + 0x0; KEEP(*libkh_*.a:ov148_tables_020d24e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov148 + 0x20; KEEP(*libkh_*.a:ov148_strings_020d2500.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov148 + 0x40);
		*libkh_*_ov148*.a:*(.data .data.*) __kh_data_end_ov148 = .;
		__kh_data_start_ov149 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov149 = .;
		. = __kh_dsdata_ov149 + 0x0; KEEP(*libkh_*.a:ov149_tables_020d0738.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov149 + 0x68; KEEP(*libkh_*.a:ov149_strings_020d07a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov149 + 0x74; KEEP(*libkh_*.a:ov149_const_020d07ac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov149 + 0x78; KEEP(*libkh_*.a:ov149_strings_020d07b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov149 + 0xa8);
		*libkh_*_ov149*.a:*(.data .data.*) __kh_data_end_ov149 = .;
		__kh_data_start_ov150 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov150 = .;
		. = __kh_dsdata_ov150 + 0x0; KEEP(*libkh_*.a:ov150_tables_020d2558.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov150 + 0x68; KEEP(*libkh_*.a:ov150_strings_020d25c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov150 + 0x74; KEEP(*libkh_*.a:ov150_const_020d25cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov150 + 0x78; KEEP(*libkh_*.a:ov150_strings_020d25d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov150 + 0xa8);
		*libkh_*_ov150*.a:*(.data .data.*) __kh_data_end_ov150 = .;
		__kh_data_start_ov151 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov151 = .;
		. = __kh_dsdata_ov151 + 0x0; KEEP(*libkh_*.a:ov151_tables_020cebe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov151 + 0x80; KEEP(*libkh_*.a:ov151_strings_020cec60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov151 + 0x8c; KEEP(*libkh_*.a:ov151_const_020cec6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov151 + 0x90; KEEP(*libkh_*.a:ov151_strings_020cec70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov151 + 0xc0);
		*libkh_*_ov151*.a:*(.data .data.*) __kh_data_end_ov151 = .;
		__kh_data_start_ov152 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov152 = .;
		. = __kh_dsdata_ov152 + 0x0; KEEP(*libkh_*.a:ov152_tables_020d6460.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov152 + 0x80; KEEP(*libkh_*.a:ov152_strings_020d64e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov152 + 0x8c; KEEP(*libkh_*.a:ov152_const_020d64ec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov152 + 0x90; KEEP(*libkh_*.a:ov152_strings_020d64f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov152 + 0xc0);
		*libkh_*_ov152*.a:*(.data .data.*) __kh_data_end_ov152 = .;
		__kh_data_start_ov153 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov153 = .;
		. = __kh_dsdata_ov153 + 0x0; KEEP(*libkh_*.a:ov153_tables_020ce00c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov153 + 0x74; KEEP(*libkh_*.a:ov153_strings_020ce080.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov153 + 0x94);
		*libkh_*_ov153*.a:*(.data .data.*) __kh_data_end_ov153 = .;
		__kh_data_start_ov154 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov154 = .;
		. = __kh_dsdata_ov154 + 0x0; KEEP(*libkh_*.a:ov154_tables_020d1c4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov154 + 0x74; KEEP(*libkh_*.a:ov154_strings_020d1cc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov154 + 0x94);
		*libkh_*_ov154*.a:*(.data .data.*) __kh_data_end_ov154 = .;
		__kh_data_start_ov155 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov155 = .;
		. = __kh_dsdata_ov155 + 0x0; KEEP(*libkh_*.a:ov155_tables_020d588c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov155 + 0x74; KEEP(*libkh_*.a:ov155_strings_020d5900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov155 + 0x94);
		*libkh_*_ov155*.a:*(.data .data.*) __kh_data_end_ov155 = .;
		__kh_data_start_ov156 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov156 = .;
		. = __kh_dsdata_ov156 + 0x0; KEEP(*libkh_*.a:ov156_tables_020ced74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov156 + 0x6c; KEEP(*libkh_*.a:ov156_strings_020cede0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov156 + 0x8c);
		*libkh_*_ov156*.a:*(.data .data.*) __kh_data_end_ov156 = .;
		__kh_data_start_ov157 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov157 = .;
		. = __kh_dsdata_ov157 + 0x0; KEEP(*libkh_*.a:ov157_tables_020d0b94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov157 + 0x6c; KEEP(*libkh_*.a:ov157_strings_020d0c00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov157 + 0x8c);
		*libkh_*_ov157*.a:*(.data .data.*) __kh_data_end_ov157 = .;
		__kh_data_start_ov158 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov158 = .;
		. = __kh_dsdata_ov158 + 0x0; KEEP(*libkh_*.a:ov158_tables_020cf520.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov158 + 0x60; KEEP(*libkh_*.a:ov158_strings_020cf580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov158 + 0x80);
		*libkh_*_ov158*.a:*(.data .data.*) __kh_data_end_ov158 = .;
		__kh_data_start_ov159 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov159 = .;
		. = __kh_dsdata_ov159 + 0x0; KEEP(*libkh_*.a:ov159_tables_020d4f80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov159 + 0x60; KEEP(*libkh_*.a:ov159_strings_020d4fe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov159 + 0x80);
		*libkh_*_ov159*.a:*(.data .data.*) __kh_data_end_ov159 = .;
		__kh_data_start_ov160 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov160 = .;
		. = __kh_dsdata_ov160 + 0x0; KEEP(*libkh_*.a:ov160_tables_020cf770.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov160 + 0x90; KEEP(*libkh_*.a:ov160_strings_020cf800.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov160 + 0xb0);
		*libkh_*_ov160*.a:*(.data .data.*) __kh_data_end_ov160 = .;
		__kh_data_start_ov161 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov161 = .;
		. = __kh_dsdata_ov161 + 0x0; KEEP(*libkh_*.a:ov161_tables_020cf260.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov161 + 0x80; KEEP(*libkh_*.a:ov161_strings_020cf2e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov161 + 0xc0);
		*libkh_*_ov161*.a:*(.data .data.*) __kh_data_end_ov161 = .;
		__kh_data_start_ov162 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov162 = .;
		. = __kh_dsdata_ov162 + 0x0; KEEP(*libkh_*.a:ov162_tables_020d1080.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov162 + 0x80; KEEP(*libkh_*.a:ov162_strings_020d1100.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov162 + 0xc0);
		*libkh_*_ov162*.a:*(.data .data.*) __kh_data_end_ov162 = .;
		__kh_data_start_ov163 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov163 = .;
		. = __kh_dsdata_ov163 + 0x0; KEEP(*libkh_*.a:ov163_tables_020d0e54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov163 + 0x8c; KEEP(*libkh_*.a:ov163_strings_020d0ee0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov163 + 0xcc);
		*libkh_*_ov163*.a:*(.data .data.*) __kh_data_end_ov163 = .;
		__kh_data_start_ov164 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov164 = .;
		. = __kh_dsdata_ov164 + 0x0; KEEP(*libkh_*.a:ov164_tables_020d2c74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov164 + 0x8c; KEEP(*libkh_*.a:ov164_strings_020d2d00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov164 + 0xcc);
		*libkh_*_ov164*.a:*(.data .data.*) __kh_data_end_ov164 = .;
		__kh_data_start_ov165 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov165 = .;
		. = __kh_dsdata_ov165 + 0x0; KEEP(*libkh_*.a:ov165_tables_020d4a94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov165 + 0x8c; KEEP(*libkh_*.a:ov165_strings_020d4b20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov165 + 0xcc);
		*libkh_*_ov165*.a:*(.data .data.*) __kh_data_end_ov165 = .;
		__kh_data_start_ov166 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov166 = .;
		. = __kh_dsdata_ov166 + 0x0; KEEP(*libkh_*.a:ov166_tables_020cead0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov166 + 0x30; KEEP(*libkh_*.a:ov166_strings_020ceb00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov166 + 0x50);
		*libkh_*_ov166*.a:*(.data .data.*) __kh_data_end_ov166 = .;
		__kh_data_start_ov167 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov167 = .;
		. = __kh_dsdata_ov167 + 0x0; KEEP(*libkh_*.a:ov167_tables_020d2710.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov167 + 0x30; KEEP(*libkh_*.a:ov167_strings_020d2740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov167 + 0x50);
		*libkh_*_ov167*.a:*(.data .data.*) __kh_data_end_ov167 = .;
		__kh_data_start_ov168 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov168 = .;
		. = __kh_dsdata_ov168 + 0x0; KEEP(*libkh_*.a:ov168_tables_020d6350.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov168 + 0x30; KEEP(*libkh_*.a:ov168_strings_020d6380.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov168 + 0x50);
		*libkh_*_ov168*.a:*(.data .data.*) __kh_data_end_ov168 = .;
		__kh_data_start_ov169 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov169 = .;
		. = __kh_dsdata_ov169 + 0x0; KEEP(*libkh_*.a:ov169_tables_020cee44.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov169 + 0x1c; KEEP(*libkh_*.a:ov169_strings_020cee60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov169 + 0x3c);
		*libkh_*_ov169*.a:*(.data .data.*) __kh_data_end_ov169 = .;
		__kh_data_start_ov170 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov170 = .;
		. = __kh_dsdata_ov170 + 0x0; KEEP(*libkh_*.a:ov170_tables_020d0c64.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov170 + 0x1c; KEEP(*libkh_*.a:ov170_strings_020d0c80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov170 + 0x3c);
		*libkh_*_ov170*.a:*(.data .data.*) __kh_data_end_ov170 = .;
		__kh_data_start_ov171 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov171 = .;
		. = __kh_dsdata_ov171 + 0x0; KEEP(*libkh_*.a:ov171_tables_020ceeac.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov171 + 0x14; KEEP(*libkh_*.a:ov171_strings_020ceec0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov171 + 0x34);
		*libkh_*_ov171*.a:*(.data .data.*) __kh_data_end_ov171 = .;
		__kh_data_start_ov172 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov172 = .;
		. = __kh_dsdata_ov172 + 0x0; KEEP(*libkh_*.a:ov172_tables_020d2aec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov172 + 0x14; KEEP(*libkh_*.a:ov172_strings_020d2b00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov172 + 0x34);
		*libkh_*_ov172*.a:*(.data .data.*) __kh_data_end_ov172 = .;
		__kh_data_start_ov173 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov173 = .;
		. = __kh_dsdata_ov173 + 0x0; KEEP(*libkh_*.a:ov173_tables_020cec98.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov173 + 0x28; KEEP(*libkh_*.a:ov173_strings_020cecc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov173 + 0x48);
		*libkh_*_ov173*.a:*(.data .data.*) __kh_data_end_ov173 = .;
		__kh_data_start_ov174 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov174 = .;
		. = __kh_dsdata_ov174 + 0x0; KEEP(*libkh_*.a:ov174_tables_020d28d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov174 + 0x28; KEEP(*libkh_*.a:ov174_strings_020d2900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov174 + 0x48);
		*libkh_*_ov174*.a:*(.data .data.*) __kh_data_end_ov174 = .;
		__kh_data_start_ov175 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov175 = .;
		. = __kh_dsdata_ov175 + 0x0; KEEP(*libkh_*.a:ov175_tables_020cea78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov175 + 0x28; KEEP(*libkh_*.a:ov175_strings_020ceaa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov175 + 0x48);
		*libkh_*_ov175*.a:*(.data .data.*) __kh_data_end_ov175 = .;
		__kh_data_start_ov176 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov176 = .;
		. = __kh_dsdata_ov176 + 0x0; KEEP(*libkh_*.a:ov176_tables_020d26b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov176 + 0x28; KEEP(*libkh_*.a:ov176_strings_020d26e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov176 + 0x48);
		*libkh_*_ov176*.a:*(.data .data.*) __kh_data_end_ov176 = .;
		__kh_data_start_ov177 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov177 = .;
		. = __kh_dsdata_ov177 + 0x0; KEEP(*libkh_*.a:ov177_tables_020d62f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov177 + 0x28; KEEP(*libkh_*.a:ov177_strings_020d6320.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov177 + 0x48);
		*libkh_*_ov177*.a:*(.data .data.*) __kh_data_end_ov177 = .;
		__kh_data_start_ov178 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov178 = .;
		. = __kh_dsdata_ov178 + 0x0; KEEP(*libkh_*.a:ov178_tables_020cee70.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov178 + 0x30; KEEP(*libkh_*.a:ov178_strings_020ceea0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov178 + 0x50);
		*libkh_*_ov178*.a:*(.data .data.*) __kh_data_end_ov178 = .;
		__kh_data_start_ov179 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov179 = .;
		. = __kh_dsdata_ov179 + 0x0; KEEP(*libkh_*.a:ov179_tables_020d48d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov179 + 0x30; KEEP(*libkh_*.a:ov179_strings_020d4900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov179 + 0x50);
		*libkh_*_ov179*.a:*(.data .data.*) __kh_data_end_ov179 = .;
		__kh_data_start_ov180 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov180 = .;
		. = __kh_dsdata_ov180 + 0x0; KEEP(*libkh_*.a:ov180_tables_020d66f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov180 + 0x30; KEEP(*libkh_*.a:ov180_strings_020d6720.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov180 + 0x50);
		*libkh_*_ov180*.a:*(.data .data.*) __kh_data_end_ov180 = .;
		__kh_data_start_ov181 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov181 = .;
		. = __kh_dsdata_ov181 + 0x0; KEEP(*libkh_*.a:ov181_tables_020ce9f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov181 + 0x48; KEEP(*libkh_*.a:ov181_strings_020cea40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov181 + 0x68);
		*libkh_*_ov181*.a:*(.data .data.*) __kh_data_end_ov181 = .;
		__kh_data_start_ov182 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov182 = .;
		. = __kh_dsdata_ov182 + 0x0; KEEP(*libkh_*.a:ov182_tables_020d0818.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov182 + 0x48; KEEP(*libkh_*.a:ov182_strings_020d0860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov182 + 0x68);
		*libkh_*_ov182*.a:*(.data .data.*) __kh_data_end_ov182 = .;
		__kh_data_start_ov183 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov183 = .;
		. = __kh_dsdata_ov183 + 0x0; KEEP(*libkh_*.a:ov183_tables_020d2638.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov183 + 0x48; KEEP(*libkh_*.a:ov183_strings_020d2680.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov183 + 0x68);
		*libkh_*_ov183*.a:*(.data .data.*) __kh_data_end_ov183 = .;
		__kh_data_start_ov184 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov184 = .;
		. = __kh_dsdata_ov184 + 0x0; KEEP(*libkh_*.a:ov184_tables_020d4458.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov184 + 0x48; KEEP(*libkh_*.a:ov184_strings_020d44a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov184 + 0x68);
		*libkh_*_ov184*.a:*(.data .data.*) __kh_data_end_ov184 = .;
		__kh_data_start_ov185 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov185 = .;
		. = __kh_dsdata_ov185 + 0x0; KEEP(*libkh_*.a:ov185_tables_020d1690.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov185 + 0x30; KEEP(*libkh_*.a:ov185_const_020d16c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov185 + 0x34; KEEP(*libkh_*.a:ov185_strings_020d16c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov185 + 0x40; KEEP(*libkh_*.a:ov185_const_020d16d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov185 + 0x44; KEEP(*libkh_*.a:ov185_strings_020d16d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov185 + 0x50);
		*libkh_*_ov185*.a:*(.data .data.*) __kh_data_end_ov185 = .;
		__kh_data_start_ov186 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov186 = .;
		. = __kh_dsdata_ov186 + 0x0; KEEP(*libkh_*.a:ov186_tables_020d34b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov186 + 0x30; KEEP(*libkh_*.a:ov186_const_020d34e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov186 + 0x34; KEEP(*libkh_*.a:ov186_strings_020d34e4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov186 + 0x40; KEEP(*libkh_*.a:ov186_const_020d34f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov186 + 0x44; KEEP(*libkh_*.a:ov186_strings_020d34f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov186 + 0x50);
		*libkh_*_ov186*.a:*(.data .data.*) __kh_data_end_ov186 = .;
		__kh_data_start_ov187 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov187 = .;
		. = __kh_dsdata_ov187 + 0x0; KEEP(*libkh_*.a:ov187_tables_020d70f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov187 + 0x30; KEEP(*libkh_*.a:ov187_const_020d7120.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov187 + 0x34; KEEP(*libkh_*.a:ov187_strings_020d7124.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov187 + 0x40; KEEP(*libkh_*.a:ov187_const_020d7130.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov187 + 0x44; KEEP(*libkh_*.a:ov187_strings_020d7134.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov187 + 0x50);
		*libkh_*_ov187*.a:*(.data .data.*) __kh_data_end_ov187 = .;
		__kh_data_start_ov188 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov188 = .;
		. = __kh_dsdata_ov188 + 0x0; KEEP(*libkh_*.a:ov188_const_020d0278.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov188 + 0x8; KEEP(*libkh_*.a:ov188_strings_020d0280.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov188 + 0xc8);
		*libkh_*_ov188*.a:*(.data .data.*) __kh_data_end_ov188 = .;
		__kh_data_start_ov189 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov189 = .;
		. = __kh_dsdata_ov189 + 0x0; KEEP(*libkh_*.a:ov189_const_020d2098.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov189 + 0x8; KEEP(*libkh_*.a:ov189_strings_020d20a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov189 + 0xc8);
		*libkh_*_ov189*.a:*(.data .data.*) __kh_data_end_ov189 = .;
		__kh_data_start_ov190 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov190 = .;
		. = __kh_dsdata_ov190 + 0x0; KEEP(*libkh_*.a:ov190_const_020d5cd8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov190 + 0x8; KEEP(*libkh_*.a:ov190_strings_020d5ce0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov190 + 0xc8);
		*libkh_*_ov190*.a:*(.data .data.*) __kh_data_end_ov190 = .;
		__kh_data_start_ov191 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov191 = .;
		. = __kh_dsdata_ov191 + 0x0; KEEP(*libkh_*.a:ov191_tables_020d2d54.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov191 + 0x8c; KEEP(*libkh_*.a:ov191_strings_020d2de0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov191 + 0xac);
		*libkh_*_ov191*.a:*(.data .data.*) __kh_data_end_ov191 = .;
		__kh_data_start_ov192 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov192 = .;
		. = __kh_dsdata_ov192 + 0x0; KEEP(*libkh_*.a:ov192_tables_020d4b74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov192 + 0x8c; KEEP(*libkh_*.a:ov192_strings_020d4c00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov192 + 0xac);
		*libkh_*_ov192*.a:*(.data .data.*) __kh_data_end_ov192 = .;
		__kh_data_start_ov193 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov193 = .;
		. = __kh_dsdata_ov193 + 0x0; KEEP(*libkh_*.a:ov193_tables_020d6994.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov193 + 0x8c; KEEP(*libkh_*.a:ov193_strings_020d6a20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov193 + 0xac);
		*libkh_*_ov193*.a:*(.data .data.*) __kh_data_end_ov193 = .;
		__kh_data_start_ov194 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov194 = .;
		. = __kh_dsdata_ov194 + 0x0; KEEP(*libkh_*.a:ov194_tables_020cef4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov194 + 0x94; KEEP(*libkh_*.a:ov194_strings_020cefe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov194 + 0xbc; KEEP(*libkh_*.a:ov194_const_020cf008.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov194 + 0xc0; KEEP(*libkh_*.a:ov194_strings_020cf00c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov194 + 0xd4);
		*libkh_*_ov194*.a:*(.data .data.*) __kh_data_end_ov194 = .;
		__kh_data_start_ov195 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov195 = .;
		. = __kh_dsdata_ov195 + 0x0; KEEP(*libkh_*.a:ov195_tables_020d2b8c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov195 + 0x94; KEEP(*libkh_*.a:ov195_strings_020d2c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov195 + 0xbc; KEEP(*libkh_*.a:ov195_const_020d2c48.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov195 + 0xc0; KEEP(*libkh_*.a:ov195_strings_020d2c4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov195 + 0xd4);
		*libkh_*_ov195*.a:*(.data .data.*) __kh_data_end_ov195 = .;
		__kh_data_start_ov196 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov196 = .;
		. = __kh_dsdata_ov196 + 0x0; KEEP(*libkh_*.a:ov196_tables_020d67cc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov196 + 0x94; KEEP(*libkh_*.a:ov196_strings_020d6860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov196 + 0xbc; KEEP(*libkh_*.a:ov196_const_020d6888.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov196 + 0xc0; KEEP(*libkh_*.a:ov196_strings_020d688c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov196 + 0xd4);
		*libkh_*_ov196*.a:*(.data .data.*) __kh_data_end_ov196 = .;
		__kh_data_start_ov197 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov197 = .;
		. = __kh_dsdata_ov197 + 0x0; KEEP(*libkh_*.a:ov197_tables_020ce894.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov197 + 0x2c; KEEP(*libkh_*.a:ov197_strings_020ce8c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov197 + 0x4c);
		*libkh_*_ov197*.a:*(.data .data.*) __kh_data_end_ov197 = .;
		__kh_data_start_ov198 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov198 = .;
		. = __kh_dsdata_ov198 + 0x0; KEEP(*libkh_*.a:ov198_tables_020d24d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov198 + 0x2c; KEEP(*libkh_*.a:ov198_strings_020d2500.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov198 + 0x4c);
		*libkh_*_ov198*.a:*(.data .data.*) __kh_data_end_ov198 = .;
		__kh_data_start_ov199 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov199 = .;
		. = __kh_dsdata_ov199 + 0x0; KEEP(*libkh_*.a:ov199_tables_020d6114.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov199 + 0x2c; KEEP(*libkh_*.a:ov199_strings_020d6140.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov199 + 0x4c);
		*libkh_*_ov199*.a:*(.data .data.*) __kh_data_end_ov199 = .;
		__kh_data_start_ov200 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov200 = .;
		. = __kh_dsdata_ov200 + 0x0; KEEP(*libkh_*.a:ov200_tables_020d1804.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov200 + 0x5c; KEEP(*libkh_*.a:ov200_strings_020d1860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov200 + 0x7c);
		*libkh_*_ov200*.a:*(.data .data.*) __kh_data_end_ov200 = .;
		__kh_data_start_ov201 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov201 = .;
		. = __kh_dsdata_ov201 + 0x0; KEEP(*libkh_*.a:ov201_tables_020d5444.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov201 + 0x5c; KEEP(*libkh_*.a:ov201_strings_020d54a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov201 + 0x7c);
		*libkh_*_ov201*.a:*(.data .data.*) __kh_data_end_ov201 = .;
		__kh_data_start_ov202 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov202 = .;
		. = __kh_dsdata_ov202 + 0x0; KEEP(*libkh_*.a:ov202_tables_020cef50.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov202 + 0x90; KEEP(*libkh_*.a:ov202_strings_020cefe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov202 + 0xd0);
		*libkh_*_ov202*.a:*(.data .data.*) __kh_data_end_ov202 = .;
		__kh_data_start_ov203 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov203 = .;
		. = __kh_dsdata_ov203 + 0x0; KEEP(*libkh_*.a:ov203_tables_020d67d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov203 + 0x90; KEEP(*libkh_*.a:ov203_strings_020d6860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov203 + 0xd0);
		*libkh_*_ov203*.a:*(.data .data.*) __kh_data_end_ov203 = .;
		__kh_data_start_ov204 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov204 = .;
		. = __kh_dsdata_ov204 + 0x0; KEEP(*libkh_*.a:ov204_tables_020d35c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov204 + 0xdc; KEEP(*libkh_*.a:ov204_strings_020d36a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov204 + 0xfc);
		*libkh_*_ov204*.a:*(.data .data.*) __kh_data_end_ov204 = .;
		__kh_data_start_ov205 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov205 = .;
		. = __kh_dsdata_ov205 + 0x0; KEEP(*libkh_*.a:ov205_tables_020d7204.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov205 + 0xdc; KEEP(*libkh_*.a:ov205_strings_020d72e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov205 + 0xfc);
		*libkh_*_ov205*.a:*(.data .data.*) __kh_data_end_ov205 = .;
		__kh_data_start_ov206 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov206 = .;
		. = __kh_dsdata_ov206 + 0x0; KEEP(*libkh_*.a:ov206_tables_020d0550.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov206 + 0x110; KEEP(*libkh_*.a:ov206_strings_020d0660.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov206 + 0x190);
		*libkh_*_ov206*.a:*(.data .data.*) __kh_data_end_ov206 = .;
		__kh_data_start_ov207 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov207 = .;
		. = __kh_dsdata_ov207 + 0x0; KEEP(*libkh_*.a:ov207_tables_020d4190.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov207 + 0x110; KEEP(*libkh_*.a:ov207_strings_020d42a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov207 + 0x190);
		*libkh_*_ov207*.a:*(.data .data.*) __kh_data_end_ov207 = .;
		__kh_data_start_ov208 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov208 = .;
		. = __kh_dsdata_ov208 + 0x0; KEEP(*libkh_*.a:ov208_tables_020d4784.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov208 + 0x58; KEEP(*libkh_*.a:ov208_tables_020d47dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov208 + 0x9c; KEEP(*libkh_*.a:ov208_strings_020d4820.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov208 + 0xf8; KEEP(*libkh_*.a:ov208_tables_020d487c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov208 + 0x11c);
		*libkh_*_ov208*.a:*(.data .data.*) __kh_data_end_ov208 = .;
		__kh_data_start_ov209 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov209 = .;
		. = __kh_dsdata_ov209 + 0x0; KEEP(*libkh_*.a:ov209_tables_020d65a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov209 + 0x58; KEEP(*libkh_*.a:ov209_tables_020d65fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov209 + 0x9c; KEEP(*libkh_*.a:ov209_strings_020d6640.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov209 + 0xf8; KEEP(*libkh_*.a:ov209_tables_020d669c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov209 + 0x11c);
		*libkh_*_ov209*.a:*(.data .data.*) __kh_data_end_ov209 = .;
		__kh_data_start_ov210 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov210 = .;
		. = __kh_dsdata_ov210 + 0x0; KEEP(*libkh_*.a:ov210_tables_020d4664.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov210 + 0x11c; KEEP(*libkh_*.a:ov210_strings_020d4780.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov210 + 0x17c);
		*libkh_*_ov210*.a:*(.data .data.*) __kh_data_end_ov210 = .;
		__kh_data_start_ov211 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov211 = .;
		. = __kh_dsdata_ov211 + 0x0; KEEP(*libkh_*.a:ov211_tables_020d6484.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov211 + 0x11c; KEEP(*libkh_*.a:ov211_strings_020d65a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov211 + 0x17c);
		*libkh_*_ov211*.a:*(.data .data.*) __kh_data_end_ov211 = .;
		__kh_data_start_ov212 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov212 = .;
		. = __kh_dsdata_ov212 + 0x0; KEEP(*libkh_*.a:ov212_const_020d2128.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov212 + 0x4; KEEP(*libkh_*.a:ov212_tables_020d212c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov212 + 0x58; KEEP(*libkh_*.a:ov212_strings_020d2180.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov212 + 0xd8);
		*libkh_*_ov212*.a:*(.data .data.*) __kh_data_end_ov212 = .;
		__kh_data_start_ov213 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov213 = .;
		. = __kh_dsdata_ov213 + 0x0; KEEP(*libkh_*.a:ov213_tables_020d2d74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov213 + 0x22c; KEEP(*libkh_*.a:ov213_strings_020d2fa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov213 + 0x26c);
		*libkh_*_ov213*.a:*(.data .data.*) __kh_data_end_ov213 = .;
		__kh_data_start_ov214 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov214 = .;
		. = __kh_dsdata_ov214 + 0x0; KEEP(*libkh_*.a:ov214_tables_020cebe4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov214 + 0x5c; KEEP(*libkh_*.a:ov214_strings_020cec40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov214 + 0x7c);
		*libkh_*_ov214*.a:*(.data .data.*) __kh_data_end_ov214 = .;
		__kh_data_start_ov215 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov215 = .;
		. = __kh_dsdata_ov215 + 0x0; KEEP(*libkh_*.a:ov215_tables_020d2824.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov215 + 0x5c; KEEP(*libkh_*.a:ov215_strings_020d2880.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov215 + 0x7c);
		*libkh_*_ov215*.a:*(.data .data.*) __kh_data_end_ov215 = .;
		__kh_data_start_ov216 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov216 = .;
		. = __kh_dsdata_ov216 + 0x0; KEEP(*libkh_*.a:ov216_tables_020cebe4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov216 + 0x5c; KEEP(*libkh_*.a:ov216_strings_020cec40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov216 + 0x7c);
		*libkh_*_ov216*.a:*(.data .data.*) __kh_data_end_ov216 = .;
		__kh_data_start_ov217 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov217 = .;
		. = __kh_dsdata_ov217 + 0x0; KEEP(*libkh_*.a:ov217_tables_020d2824.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov217 + 0x5c; KEEP(*libkh_*.a:ov217_strings_020d2880.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov217 + 0x7c);
		*libkh_*_ov217*.a:*(.data .data.*) __kh_data_end_ov217 = .;
		__kh_data_start_ov218 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov218 = .;
		. = __kh_dsdata_ov218 + 0x0; KEEP(*libkh_*.a:ov218_tables_020cf30c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov218 + 0x14; KEEP(*libkh_*.a:ov218_strings_020cf320.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov218 + 0x34);
		*libkh_*_ov218*.a:*(.data .data.*) __kh_data_end_ov218 = .;
		__kh_data_start_ov219 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov219 = .;
		. = __kh_dsdata_ov219 + 0x0; KEEP(*libkh_*.a:ov219_tables_020d18a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov219 + 0x20; KEEP(*libkh_*.a:ov219_strings_020d18c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov219 + 0x40);
		*libkh_*_ov219*.a:*(.data .data.*) __kh_data_end_ov219 = .;
		__kh_data_start_ov220 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov220 = .;
		. = __kh_dsdata_ov220 + 0x0; KEEP(*libkh_*.a:ov220_tables_020d3b94.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov220 + 0xc; KEEP(*libkh_*.a:ov220_strings_020d3ba0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov220 + 0x2c);
		*libkh_*_ov220*.a:*(.data .data.*) __kh_data_end_ov220 = .;
		__kh_data_start_ov221 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov221 = .;
		. = __kh_dsdata_ov221 + 0x0; KEEP(*libkh_*.a:ov221_tables_020d4da8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov221 + 0x58; KEEP(*libkh_*.a:ov221_strings_020d4e00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov221 + 0xb0; KEEP(*libkh_*.a:ov221_zero_020d4e58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov221 + 0xb8);
		*libkh_*_ov221*.a:*(.data .data.*) __kh_data_end_ov221 = .;
		__kh_data_start_ov222 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov222 = .;
		. = __kh_dsdata_ov222 + 0x0; KEEP(*libkh_*.a:ov222_tables_020d6bc8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov222 + 0x58; KEEP(*libkh_*.a:ov222_strings_020d6c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov222 + 0xb0; KEEP(*libkh_*.a:ov222_zero_020d6c78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov222 + 0xb8);
		*libkh_*_ov222*.a:*(.data .data.*) __kh_data_end_ov222 = .;
		__kh_data_start_ov223 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov223 = .;
		. = __kh_dsdata_ov223 + 0x0; KEEP(*libkh_*.a:ov223_tables_020d50c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov223 + 0x3c; KEEP(*libkh_*.a:ov223_strings_020d5100.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov223 + 0x9c);
		*libkh_*_ov223*.a:*(.data .data.*) __kh_data_end_ov223 = .;
		__kh_data_start_ov224 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov224 = .;
		. = __kh_dsdata_ov224 + 0x0; KEEP(*libkh_*.a:ov224_tables_020d4de0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov224 + 0x40; KEEP(*libkh_*.a:ov224_strings_020d4e20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov224 + 0x98; KEEP(*libkh_*.a:ov224_zero_020d4e78.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov224 + 0xa0);
		*libkh_*_ov224*.a:*(.data .data.*) __kh_data_end_ov224 = .;
		__kh_data_start_ov225 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov225 = .;
		. = __kh_dsdata_ov225 + 0x0; KEEP(*libkh_*.a:ov225_tables_020d5094.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov225 + 0x4c; KEEP(*libkh_*.a:ov225_strings_020d50e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov225 + 0xa4; KEEP(*libkh_*.a:ov225_zero_020d5138.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov225 + 0xac);
		*libkh_*_ov225*.a:*(.data .data.*) __kh_data_end_ov225 = .;
		__kh_data_start_ov226 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov226 = .;
		. = __kh_dsdata_ov226 + 0x0; KEEP(*libkh_*.a:ov226_tables_020d4b20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov226 + 0x40; KEEP(*libkh_*.a:ov226_strings_020d4b60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov226 + 0xa0);
		*libkh_*_ov226*.a:*(.data .data.*) __kh_data_end_ov226 = .;
		__kh_data_start_ov227 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov227 = .;
		. = __kh_dsdata_ov227 + 0x0; KEEP(*libkh_*.a:ov227_tables_020d4b34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov227 + 0x4c; KEEP(*libkh_*.a:ov227_strings_020d4b80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov227 + 0xac);
		*libkh_*_ov227*.a:*(.data .data.*) __kh_data_end_ov227 = .;
		__kh_data_start_ov228 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov228 = .;
		. = __kh_dsdata_ov228 + 0x0; KEEP(*libkh_*.a:ov228_tables_020d2c2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov228 + 0xb4; KEEP(*libkh_*.a:ov228_strings_020d2ce0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov228 + 0xf4);
		*libkh_*_ov228*.a:*(.data .data.*) __kh_data_end_ov228 = .;
		__kh_data_start_ov229 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov229 = .;
		. = __kh_dsdata_ov229 + 0x0; KEEP(*libkh_*.a:ov229_tables_020d686c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov229 + 0xb4; KEEP(*libkh_*.a:ov229_strings_020d6920.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov229 + 0xf4);
		*libkh_*_ov229*.a:*(.data .data.*) __kh_data_end_ov229 = .;
		__kh_data_start_ov230 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov230 = .;
		. = __kh_dsdata_ov230 + 0x0; KEEP(*libkh_*.a:ov230_tables_020d63b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov230 + 0xc8; KEEP(*libkh_*.a:ov230_strings_020d6480.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov230 + 0x108);
		*libkh_*_ov230*.a:*(.data .data.*) __kh_data_end_ov230 = .;
		__kh_data_start_ov231 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov231 = .;
		. = __kh_dsdata_ov231 + 0x0; KEEP(*libkh_*.a:ov231_tables_020cfa10.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov231 + 0x24; KEEP(*libkh_*.a:ov231_tables_020cfa34.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov231 + 0x50; KEEP(*libkh_*.a:ov231_strings_020cfa60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov231 + 0xb0);
		*libkh_*_ov231*.a:*(.data .data.*) __kh_data_end_ov231 = .;
		__kh_data_start_ov232 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov232 = .;
		. = __kh_dsdata_ov232 + 0x0; KEEP(*libkh_*.a:ov232_tables_020d3650.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov232 + 0x24; KEEP(*libkh_*.a:ov232_tables_020d3674.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov232 + 0x50; KEEP(*libkh_*.a:ov232_strings_020d36a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov232 + 0xb0);
		*libkh_*_ov232*.a:*(.data .data.*) __kh_data_end_ov232 = .;
		__kh_data_start_ov233 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov233 = .;
		. = __kh_dsdata_ov233 + 0x0; KEEP(*libkh_*.a:ov233_tables_020d0d74.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov233 + 0xcc; KEEP(*libkh_*.a:ov233_strings_020d0e40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov233 + 0x10c);
		*libkh_*_ov233*.a:*(.data .data.*) __kh_data_end_ov233 = .;
		__kh_data_start_ov234 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov234 = .;
		. = __kh_dsdata_ov234 + 0x0; KEEP(*libkh_*.a:ov234_tables_020cd100.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov234 + 0x20; KEEP(*libkh_*.a:ov234_strings_020cd120.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov234 + 0x40);
		*libkh_*_ov234*.a:*(.data .data.*) __kh_data_end_ov234 = .;
		__kh_data_start_ov235 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov235 = .;
		. = __kh_dsdata_ov235 + 0x0; KEEP(*libkh_*.a:ov235_tables_020d22d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov235 + 0x2b0; KEEP(*libkh_*.a:ov235_strings_020d2580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov235 + 0x310);
		*libkh_*_ov235*.a:*(.data .data.*) __kh_data_end_ov235 = .;
		__kh_data_start_ov236 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov236 = .;
		. = __kh_dsdata_ov236 + 0x0; KEEP(*libkh_*.a:ov236_tables_020d627c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov236 + 0x224; KEEP(*libkh_*.a:ov236_strings_020d64a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov236 + 0x2a4);
		*libkh_*_ov236*.a:*(.data .data.*) __kh_data_end_ov236 = .;
		__kh_data_start_ov237 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov237 = .;
		. = __kh_dsdata_ov237 + 0x0; KEEP(*libkh_*.a:ov237_const_020d19d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov237 + 0x24; KEEP(*libkh_*.a:ov237_pointers_020d19f8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov237 + 0x4c; KEEP(*libkh_*.a:ov237_tables_020d1a20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov237 + 0x24c; KEEP(*libkh_*.a:ov237_strings_020d1c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov237 + 0x30c);
		*libkh_*_ov237*.a:*(.data .data.*) __kh_data_end_ov237 = .;
		__kh_data_start_ov238 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov238 = .;
		. = __kh_dsdata_ov238 + 0x0; KEEP(*libkh_*.a:ov238_tables_020d3668.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov238 + 0x98; KEEP(*libkh_*.a:ov238_strings_020d3700.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov238 + 0xd8);
		*libkh_*_ov238*.a:*(.data .data.*) __kh_data_end_ov238 = .;
		__kh_data_start_ov239 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov239 = .;
		. = __kh_dsdata_ov239 + 0x0; KEEP(*libkh_*.a:ov239_const_020cdc20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov239 + 0x20; KEEP(*libkh_*.a:ov239_strings_020cdc40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov239 + 0x60);
		*libkh_*_ov239*.a:*(.data .data.*) __kh_data_end_ov239 = .;
		__kh_data_start_ov240 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov240 = .;
		. = __kh_dsdata_ov240 + 0x0; KEEP(*libkh_*.a:ov240_const_020cfb60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov240 + 0x20; KEEP(*libkh_*.a:ov240_strings_020cfb80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov240 + 0x60);
		*libkh_*_ov240*.a:*(.data .data.*) __kh_data_end_ov240 = .;
		__kh_data_start_ov241 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov241 = .;
		. = __kh_dsdata_ov241 + 0x0; KEEP(*libkh_*.a:ov241_pointers_020d0c84.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov241 + 0x1c; KEEP(*libkh_*.a:ov241_strings_020d0ca0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov241 + 0x5c);
		*libkh_*_ov241*.a:*(.data .data.*) __kh_data_end_ov241 = .;
		__kh_data_start_ov242 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov242 = .;
		. = __kh_dsdata_ov242 + 0x0; KEEP(*libkh_*.a:ov242_pointers_020d48c4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov242 + 0x1c; KEEP(*libkh_*.a:ov242_strings_020d48e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov242 + 0x5c);
		*libkh_*_ov242*.a:*(.data .data.*) __kh_data_end_ov242 = .;
		__kh_data_start_ov243 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov243 = .;
		. = __kh_dsdata_ov243 + 0x0; KEEP(*libkh_*.a:ov243_pointers_020d4748.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov243 + 0x18; KEEP(*libkh_*.a:ov243_strings_020d4760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov243 + 0x38);
		*libkh_*_ov243*.a:*(.data .data.*) __kh_data_end_ov243 = .;
		__kh_data_start_ov244 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov244 = .;
		. = __kh_dsdata_ov244 + 0x0; KEEP(*libkh_*.a:ov244_tables_020d3628.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov244 + 0x198; KEEP(*libkh_*.a:ov244_strings_020d37c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov244 + 0x298);
		*libkh_*_ov244*.a:*(.data .data.*) __kh_data_end_ov244 = .;
		__kh_data_start_ov245 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov245 = .;
		. = __kh_dsdata_ov245 + 0x0; KEEP(*libkh_*.a:ov245_tables_020d7198.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov245 + 0x48; KEEP(*libkh_*.a:ov245_strings_020d71e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov245 + 0x128);
		*libkh_*_ov245*.a:*(.data .data.*) __kh_data_end_ov245 = .;
		__kh_data_start_ov246 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov246 = .;
		. = __kh_dsdata_ov246 + 0x0; KEEP(*libkh_*.a:ov246_tables_020d30c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov246 + 0x60; KEEP(*libkh_*.a:ov246_strings_020d3120.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov246 + 0x80);
		*libkh_*_ov246*.a:*(.data .data.*) __kh_data_end_ov246 = .;
		__kh_data_start_ov247 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov247 = .;
		. = __kh_dsdata_ov247 + 0x0; KEEP(*libkh_*.a:ov247_tables_020d4ee0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov247 + 0x60; KEEP(*libkh_*.a:ov247_strings_020d4f40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov247 + 0x80);
		*libkh_*_ov247*.a:*(.data .data.*) __kh_data_end_ov247 = .;
		__kh_data_start_ov248 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov248 = .;
		. = __kh_dsdata_ov248 + 0x0; KEEP(*libkh_*.a:ov248_tables_020d0b58.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov248 + 0xc8; KEEP(*libkh_*.a:ov248_strings_020d0c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov248 + 0x108);
		*libkh_*_ov248*.a:*(.data .data.*) __kh_data_end_ov248 = .;
		__kh_data_start_ov249 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov249 = .;
		. = __kh_dsdata_ov249 + 0x0; KEEP(*libkh_*.a:ov249_tables_020d48dc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov249 + 0xc4; KEEP(*libkh_*.a:ov249_strings_020d49a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov249 + 0x104);
		*libkh_*_ov249*.a:*(.data .data.*) __kh_data_end_ov249 = .;
		__kh_data_start_ov250 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov250 = .;
		. = __kh_dsdata_ov250 + 0x0; KEEP(*libkh_*.a:ov250_tables_020d2890.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov250 + 0x70; KEEP(*libkh_*.a:ov250_strings_020d2900.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov250 + 0xb0);
		*libkh_*_ov250*.a:*(.data .data.*) __kh_data_end_ov250 = .;
		__kh_data_start_ov251 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov251 = .;
		. = __kh_dsdata_ov251 + 0x0; KEEP(*libkh_*.a:ov251_tables_020d64d0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov251 + 0x70; KEEP(*libkh_*.a:ov251_strings_020d6540.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov251 + 0xb0);
		*libkh_*_ov251*.a:*(.data .data.*) __kh_data_end_ov251 = .;
		__kh_data_start_ov252 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov252 = .;
		. = __kh_dsdata_ov252 + 0x0; KEEP(*libkh_*.a:ov252_rodata_020d4258.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov252 + 0xdc; KEEP(*libkh_*.a:ov252_templates_020d4334.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov252 + 0x2c8; KEEP(*libkh_*.a:ov252_strings_020d4520.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov252 + 0x388);
		*libkh_*_ov252*.a:*(.data .data.*) __kh_data_end_ov252 = .;
		__kh_data_start_ov253 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov253 = .;
		. = __kh_dsdata_ov253 + 0x0; KEEP(*libkh_*.a:ov253_rodata_020d482c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x18; KEEP(*libkh_*.a:ov253_rodata_020d4844.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x68; KEEP(*libkh_*.a:ov253_rodata_020d4894.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x114; KEEP(*libkh_*.a:ov253_rodata_020d4940.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x124; KEEP(*libkh_*.a:ov253_rodata_020d4950.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x138; KEEP(*libkh_*.a:ov253_rodata_020d4964.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x17c; KEEP(*libkh_*.a:ov253_rodata_020d49a8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x1d4; KEEP(*libkh_*.a:ov253_rodata_020d4a00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x1dc; KEEP(*libkh_*.a:ov253_rodata_020d4a08.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov253 + 0x214; KEEP(*libkh_*.a:ov253_strings_020d4a40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov253 + 0x3b4);
		*libkh_*_ov253*.a:*(.data .data.*) __kh_data_end_ov253 = .;
		__kh_data_start_ov254 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov254 = .;
		. = __kh_dsdata_ov254 + 0x0; KEEP(*libkh_*.a:ov254_const_020d5910.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov254 + 0x4; KEEP(*libkh_*.a:ov254_tables_020d5914.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov254 + 0x70; KEEP(*libkh_*.a:ov254_strings_020d5980.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov254 + 0xf0);
		*libkh_*_ov254*.a:*(.data .data.*) __kh_data_end_ov254 = .;
		__kh_data_start_ov255 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov255 = .;
		. = __kh_dsdata_ov255 + 0x0; KEEP(*libkh_*.a:ov255_tables_020d29d8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov255 + 0x208; KEEP(*libkh_*.a:ov255_strings_020d2be0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov255 + 0x24c; KEEP(*libkh_*.a:ov255_const_020d2c24.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov255 + 0x250; KEEP(*libkh_*.a:ov255_strings_020d2c28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov255 + 0x268);
		*libkh_*_ov255*.a:*(.data .data.*) __kh_data_end_ov255 = .;
		__kh_data_start_ov256 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov256 = .;
		. = __kh_dsdata_ov256 + 0x0; KEEP(*libkh_*.a:ov256_rodata_020d2444.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov256 + 0x150; KEEP(*libkh_*.a:ov256_templates_020d2594.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov256 + 0x23c; KEEP(*libkh_*.a:ov256_strings_020d2680.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov256 + 0x2bc);
		*libkh_*_ov256*.a:*(.data .data.*) __kh_data_end_ov256 = .;
		__kh_data_start_ov257 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov257 = .;
		. = __kh_dsdata_ov257 + 0x0; KEEP(*libkh_*.a:ov257_rodata_020d3064.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov257 + 0x2fc; KEEP(*libkh_*.a:ov257_strings_020d3360.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov257 + 0x39c);
		*libkh_*_ov257*.a:*(.data .data.*) __kh_data_end_ov257 = .;
		__kh_data_start_ov258 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov258 = .;
		. = __kh_dsdata_ov258 + 0x0; KEEP(*libkh_*.a:ov258_rodata_020d16f4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov258 + 0x16c; KEEP(*libkh_*.a:ov258_strings_020d1860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov258 + 0x20c);
		*libkh_*_ov258*.a:*(.data .data.*) __kh_data_end_ov258 = .;
		__kh_data_start_ov259 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov259 = .;
		. = __kh_dsdata_ov259 + 0x0; KEEP(*libkh_*.a:ov259_rodata_020d2f2c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov259 + 0x94; KEEP(*libkh_*.a:ov259_strings_020d2fc0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov259 + 0xd4);
		*libkh_*_ov259*.a:*(.data .data.*) __kh_data_end_ov259 = .;
		__kh_data_start_ov260 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov260 = .;
		. = __kh_dsdata_ov260 + 0x0; KEEP(*libkh_*.a:ov260_tables_020d2a3c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov260 + 0x64; KEEP(*libkh_*.a:ov260_strings_020d2aa0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov260 + 0xa4);
		*libkh_*_ov260*.a:*(.data .data.*) __kh_data_end_ov260 = .;
		__kh_data_start_ov261 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov261 = .;
		. = __kh_dsdata_ov261 + 0x0; KEEP(*libkh_*.a:ov261_strings_020d1580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov261 + 0x40);
		*libkh_*_ov261*.a:*(.data .data.*) __kh_data_end_ov261 = .;
		__kh_data_start_ov262 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov262 = .;
		. = __kh_dsdata_ov262 + 0x0; KEEP(*libkh_*.a:ov262_strings_020d51c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov262 + 0x40);
		*libkh_*_ov262*.a:*(.data .data.*) __kh_data_end_ov262 = .;
		__kh_data_start_ov263 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov263 = .;
		. = __kh_dsdata_ov263 + 0x0; KEEP(*libkh_*.a:ov263_tables_020d3674.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov263 + 0x24; KEEP(*libkh_*.a:ov263_tables_020d3698.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov263 + 0x4c; KEEP(*libkh_*.a:ov263_strings_020d36c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov263 + 0xac);
		*libkh_*_ov263*.a:*(.data .data.*) __kh_data_end_ov263 = .;
		__kh_data_start_ov264 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov264 = .;
		. = __kh_dsdata_ov264 + 0x0; KEEP(*libkh_*.a:ov264_tables_020cebc4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov264 + 0x5c; KEEP(*libkh_*.a:ov264_strings_020cec20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov264 + 0x7c);
		*libkh_*_ov264*.a:*(.data .data.*) __kh_data_end_ov264 = .;
		__kh_data_start_ov265 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov265 = .;
		. = __kh_dsdata_ov265 + 0x0; KEEP(*libkh_*.a:ov265_tables_020d3668.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov265 + 0x24; KEEP(*libkh_*.a:ov265_tables_020d368c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov265 + 0x58; KEEP(*libkh_*.a:ov265_strings_020d36c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov265 + 0xb8);
		*libkh_*_ov265*.a:*(.data .data.*) __kh_data_end_ov265 = .;
		__kh_data_start_ov266 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov266 = .;
		. = __kh_dsdata_ov266 + 0x0; KEEP(*libkh_*.a:ov266_const_020d3f1c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov266 + 0x4; KEEP(*libkh_*.a:ov266_tables_020d3f20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov266 + 0x30; KEEP(*libkh_*.a:ov266_tables_020d3f4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov266 + 0x44; KEEP(*libkh_*.a:ov266_strings_020d3f60.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov266 + 0xc4);
		*libkh_*_ov266*.a:*(.data .data.*) __kh_data_end_ov266 = .;
		__kh_data_start_ov267 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov267 = .;
		. = __kh_dsdata_ov267 + 0x0; KEEP(*libkh_*.a:ov267_const_020d5d3c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov267 + 0x4; KEEP(*libkh_*.a:ov267_tables_020d5d40.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov267 + 0x30; KEEP(*libkh_*.a:ov267_tables_020d5d6c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov267 + 0x44; KEEP(*libkh_*.a:ov267_strings_020d5d80.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov267 + 0xc4);
		*libkh_*_ov267*.a:*(.data .data.*) __kh_data_end_ov267 = .;
		__kh_data_start_ov268 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov268 = .;
		. = __kh_dsdata_ov268 + 0x0; KEEP(*libkh_*.a:ov268_tables_020d47b0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov268 + 0x58; KEEP(*libkh_*.a:ov268_tables_020d4808.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov268 + 0xb0; KEEP(*libkh_*.a:ov268_strings_020d4860.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov268 + 0x10c; KEEP(*libkh_*.a:ov268_tables_020d48bc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov268 + 0x130);
		*libkh_*_ov268*.a:*(.data .data.*) __kh_data_end_ov268 = .;
		__kh_data_start_ov269 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov269 = .;
		. = __kh_dsdata_ov269 + 0x0; KEEP(*libkh_*.a:ov269_tables_020d4a28.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov269 + 0x98; KEEP(*libkh_*.a:ov269_strings_020d4ac0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov269 + 0xc0; KEEP(*libkh_*.a:ov269_const_020d4ae8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov269 + 0xc4; KEEP(*libkh_*.a:ov269_strings_020d4aec.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov269 + 0xd8);
		*libkh_*_ov269*.a:*(.data .data.*) __kh_data_end_ov269 = .;
		__kh_data_start_ov270 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov270 = .;
		. = __kh_dsdata_ov270 + 0x0; KEEP(*libkh_*.a:ov270_tables_020d6848.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov270 + 0x98; KEEP(*libkh_*.a:ov270_strings_020d68e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov270 + 0xc0; KEEP(*libkh_*.a:ov270_const_020d6908.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov270 + 0xc4; KEEP(*libkh_*.a:ov270_strings_020d690c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov270 + 0xd8);
		*libkh_*_ov270*.a:*(.data .data.*) __kh_data_end_ov270 = .;
		__kh_data_start_ov271 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov271 = .;
		. = __kh_dsdata_ov271 + 0x0; KEEP(*libkh_*.a:ov271_tables_020d3678.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov271 + 0x68; KEEP(*libkh_*.a:ov271_strings_020d36e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov271 + 0x88);
		*libkh_*_ov271*.a:*(.data .data.*) __kh_data_end_ov271 = .;
		__kh_data_start_ov272 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov272 = .;
		. = __kh_dsdata_ov272 + 0x0; KEEP(*libkh_*.a:ov272_tables_020d3568.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov272 + 0x38; KEEP(*libkh_*.a:ov272_strings_020d35a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov272 + 0x98);
		*libkh_*_ov272*.a:*(.data .data.*) __kh_data_end_ov272 = .;
		__kh_data_start_ov273 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov273 = .;
		. = __kh_dsdata_ov273 + 0x0; KEEP(*libkh_*.a:ov273_tables_020d69b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov273 + 0x22c; KEEP(*libkh_*.a:ov273_strings_020d6be0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov273 + 0x26c);
		*libkh_*_ov273*.a:*(.data .data.*) __kh_data_end_ov273 = .;
		__kh_data_start_ov274 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov274 = .;
		. = __kh_dsdata_ov274 + 0x0; KEEP(*libkh_*.a:ov274_tables_020d4194.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov274 + 0x12c; KEEP(*libkh_*.a:ov274_strings_020d42c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov274 + 0x1ac);
		*libkh_*_ov274*.a:*(.data .data.*) __kh_data_end_ov274 = .;
		__kh_data_start_ov275 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov275 = .;
		. = __kh_dsdata_ov275 + 0x0; KEEP(*libkh_*.a:ov275_tables_020d5fb4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov275 + 0x12c; KEEP(*libkh_*.a:ov275_strings_020d60e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov275 + 0x1ac);
		*libkh_*_ov275*.a:*(.data .data.*) __kh_data_end_ov275 = .;
		__kh_data_start_ov276 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov276 = .;
		. = __kh_dsdata_ov276 + 0x0; KEEP(*libkh_*.a:ov276_tables_020d2b88.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov276 + 0x98; KEEP(*libkh_*.a:ov276_strings_020d2c20.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov276 + 0xd8);
		*libkh_*_ov276*.a:*(.data .data.*) __kh_data_end_ov276 = .;
		__kh_data_start_ov277 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov277 = .;
		. = __kh_dsdata_ov277 + 0x0; KEEP(*libkh_*.a:ov277_tables_020d35c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov277 + 0x1a0; KEEP(*libkh_*.a:ov277_strings_020d3760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov277 + 0x1f8; KEEP(*libkh_*.a:ov277_tables_020d37b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov277 + 0x200; KEEP(*libkh_*.a:ov277_strings_020d37c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov277 + 0x2a0);
		*libkh_*_ov277*.a:*(.data .data.*) __kh_data_end_ov277 = .;
		__kh_data_start_ov278 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov278 = .;
		. = __kh_dsdata_ov278 + 0x0; KEEP(*libkh_*.a:ov278_tables_020d6230.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov278 + 0x230; KEEP(*libkh_*.a:ov278_strings_020d6460.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov278 + 0x2b0);
		*libkh_*_ov278*.a:*(.data .data.*) __kh_data_end_ov278 = .;
		__kh_data_start_ov279 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov279 = .;
		. = __kh_dsdata_ov279 + 0x0; KEEP(*libkh_*.a:ov279_tables_020d357c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov279 + 0x44; KEEP(*libkh_*.a:ov279_strings_020d35c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov279 + 0xa4);
		*libkh_*_ov279*.a:*(.data .data.*) __kh_data_end_ov279 = .;
		__kh_data_start_ov280 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov280 = .;
		. = __kh_dsdata_ov280 + 0x0; KEEP(*libkh_*.a:ov280_tables_020d3674.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov280 + 0x24; KEEP(*libkh_*.a:ov280_tables_020d3698.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov280 + 0x4c; KEEP(*libkh_*.a:ov280_strings_020d36c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov280 + 0xac);
		*libkh_*_ov280*.a:*(.data .data.*) __kh_data_end_ov280 = .;
		__kh_data_start_ov281 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov281 = .;
		. = __kh_dsdata_ov281 + 0x0; KEEP(*libkh_*.a:ov281_const_020ce468.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov281 + 0x18; KEEP(*libkh_*.a:ov281_strings_020ce480.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov281 + 0xd8);
		*libkh_*_ov281*.a:*(.data .data.*) __kh_data_end_ov281 = .;
		__kh_data_start_ov282 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov282 = .;
		. = __kh_dsdata_ov282 + 0x0; KEEP(*libkh_*.a:ov282_tables_020d4678.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov282 + 0x128; KEEP(*libkh_*.a:ov282_strings_020d47a0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov282 + 0x188);
		*libkh_*_ov282*.a:*(.data .data.*) __kh_data_end_ov282 = .;
		__kh_data_start_ov283 = .; . = ALIGN(32) + 0xc; __kh_dsdata_ov283 = .;
		. = __kh_dsdata_ov283 + 0x0; KEEP(*libkh_*.a:ov283_tables_020cfb4c.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov283 + 0x94; KEEP(*libkh_*.a:ov283_strings_020cfbe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov283 + 0xb4);
		*libkh_*_ov283*.a:*(.data .data.*) __kh_data_end_ov283 = .;
		__kh_data_start_ov284 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov284 = .;
		. = __kh_dsdata_ov284 + 0x0; KEEP(*libkh_*.a:ov284_tables_020cd594.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov284 + 0x10; KEEP(*libkh_*.a:ov284_pointers_020cd5a4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov284 + 0x20; KEEP(*libkh_*.a:ov284_tables_020cd5b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov284 + 0x4c; KEEP(*libkh_*.a:ov284_strings_020cd5e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov284 + 0x8c);
		*libkh_*_ov284*.a:*(.data .data.*) __kh_data_end_ov284 = .;
		__kh_data_start_ov285 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov285 = .;
		. = __kh_dsdata_ov285 + 0x0; KEEP(*libkh_*.a:ov285_strings_020d2760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov285 + 0x20);
		*libkh_*_ov285*.a:*(.data .data.*) __kh_data_end_ov285 = .;
		__kh_data_start_ov286 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov286 = .;
		. = __kh_dsdata_ov286 + 0x0; KEEP(*libkh_*.a:ov286_strings_020d4580.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov286 + 0x20);
		*libkh_*_ov286*.a:*(.data .data.*) __kh_data_end_ov286 = .;
		__kh_data_start_ov287 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov287 = .;
		. = __kh_dsdata_ov287 + 0x0; KEEP(*libkh_*.a:ov287_tables_020d16d4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov287 + 0xc; KEEP(*libkh_*.a:ov287_strings_020d16e0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov287 + 0x2c);
		*libkh_*_ov287*.a:*(.data .data.*) __kh_data_end_ov287 = .;
		__kh_data_start_ov288 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov288 = .;
		. = __kh_dsdata_ov288 + 0x0; KEEP(*libkh_*.a:ov288_tables_020d5314.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov288 + 0xc; KEEP(*libkh_*.a:ov288_strings_020d5320.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov288 + 0x2c);
		*libkh_*_ov288*.a:*(.data .data.*) __kh_data_end_ov288 = .;
		__kh_data_start_ov289 = .; . = ALIGN(32) + 0x14; __kh_dsdata_ov289 = .;
		. = __kh_dsdata_ov289 + 0x0; KEEP(*libkh_*.a:ov289_tables_020d7134.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov289 + 0xc; KEEP(*libkh_*.a:ov289_strings_020d7140.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov289 + 0x2c);
		*libkh_*_ov289*.a:*(.data .data.*) __kh_data_end_ov289 = .;
		__kh_data_start_ov290 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov290 = .;
		. = __kh_dsdata_ov290 + 0x0; KEEP(*libkh_*.a:ov290_strings_020cc020.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov290 + 0x20);
		*libkh_*_ov290*.a:*(.data .data.*) __kh_data_end_ov290 = .;
		__kh_data_start_ov291 = .; . = ALIGN(32) + 0x4; __kh_dsdata_ov291 = .;
		. = __kh_dsdata_ov291 + 0x0; KEEP(*libkh_*.a:ov291_tables_020cd604.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov291 + 0x1c; KEEP(*libkh_*.a:ov291_strings_020cd620.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov291 + 0x3c);
		*libkh_*_ov291*.a:*(.data .data.*) __kh_data_end_ov291 = .;
		__kh_data_start_ov292 = .; . = ALIGN(32) + 0x18; __kh_dsdata_ov292 = .;
		. = __kh_dsdata_ov292 + 0x0; KEEP(*libkh_*.a:ov292_const_020d48b8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov292 + 0x8; KEEP(*libkh_*.a:ov292_strings_020d48c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov292 + 0x28);
		*libkh_*_ov292*.a:*(.data .data.*) __kh_data_end_ov292 = .;
		__kh_data_start_ov293 = .; . = ALIGN(32) + 0x1c; __kh_dsdata_ov293 = .;
		. = __kh_dsdata_ov293 + 0x0; KEEP(*libkh_*.a:ov293_tables_020d35fc.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov293 + 0x24; KEEP(*libkh_*.a:ov293_strings_020d3620.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov293 + 0x64);
		*libkh_*_ov293*.a:*(.data .data.*) __kh_data_end_ov293 = .;
		__kh_data_start_ov294 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov294 = .;
		. = __kh_dsdata_ov294 + 0x0; KEEP(*libkh_*.a:ov294_strings_020d2520.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov294 + 0x20);
		*libkh_*_ov294*.a:*(.data .data.*) __kh_data_end_ov294 = .;
		__kh_data_start_ov295 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov295 = .;
		. = __kh_dsdata_ov295 + 0x0; KEEP(*libkh_*.a:ov295_strings_020d4340.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov295 + 0x20);
		*libkh_*_ov295*.a:*(.data .data.*) __kh_data_end_ov295 = .;
		__kh_data_start_ov296 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov296 = .;
		. = __kh_dsdata_ov296 + 0x0; KEEP(*libkh_*.a:ov296_strings_020d6160.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov296 + 0x20);
		*libkh_*_ov296*.a:*(.data .data.*) __kh_data_end_ov296 = .;
		__kh_data_start_ov297 = .; . = ALIGN(32) + 0x10; __kh_dsdata_ov297 = .;
		. = __kh_dsdata_ov297 + 0x0; KEEP(*libkh_*.a:ov297_tables_020d5690.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov297 + 0x24; KEEP(*libkh_*.a:ov297_tables_020d56b4.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov297 + 0x130; KEEP(*libkh_*.a:ov297_strings_020d57c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov297 + 0x150);
		*libkh_*_ov297*.a:*(.data .data.*) __kh_data_end_ov297 = .;
		__kh_data_start_ov298 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov298 = .;
		. = __kh_dsdata_ov298 + 0x0; KEEP(*libkh_*.a:ov298_tables_020d54e8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov298 + 0x8; KEEP(*libkh_*.a:ov298_tables_020d54f0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov298 + 0xd8; KEEP(*libkh_*.a:ov298_strings_020d55c0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov298 + 0xf8);
		*libkh_*_ov298*.a:*(.data .data.*) __kh_data_end_ov298 = .;
		__kh_data_start_ov299 = .; . = ALIGN(32) + 0x8; __kh_dsdata_ov299 = .;
		. = __kh_dsdata_ov299 + 0x0; KEEP(*libkh_*.a:ov299_const_020d4de8.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = __kh_dsdata_ov299 + 0x18; KEEP(*libkh_*.a:ov299_strings_020d4e00.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov299 + 0x38);
		*libkh_*_ov299*.a:*(.data .data.*) __kh_data_end_ov299 = .;
		__kh_data_start_ov300 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov300 = .;
		. = __kh_dsdata_ov300 + 0x0; KEEP(*libkh_*.a:ov300_strings_020cbfe0.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov300 + 0x20);
		*libkh_*_ov300*.a:*(.data .data.*) __kh_data_end_ov300 = .;
		__kh_data_start_ov301 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov301 = .;
		. = __kh_dsdata_ov301 + 0x0; KEEP(*libkh_*.a:ov301_strings_020cc760.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov301 + 0x20);
		*libkh_*_ov301*.a:*(.data .data.*) __kh_data_end_ov301 = .;
		__kh_data_start_ov302 = .; . = ALIGN(32) + 0x0; __kh_dsdata_ov302 = .;
		. = __kh_dsdata_ov302 + 0x0; KEEP(*libkh_*.a:ov302_strings_020cc740.o(.data .data.* .rodata .rodata.* .bss .bss.*))
		. = MAX(., __kh_dsdata_ov302 + 0x20);
		*libkh_*_ov302*.a:*(.data .data.*) __kh_data_end_ov302 = .;
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		SORT(CONSTRUCTORS)
	}

	/* Static data.  */
	.rodata ALIGN(128): {
		KEEP(*(.rodata.kh.overlay_retain))
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	}

	.rdata ALIGN(128): { *(.rdata) }
	.gcc_except_table ALIGN(128): { *(.gcc_except_table) }

	_gp = ALIGN(128) + 0x7ff0;
	.lit4 ALIGN(128): { *(.lit4) }
	.lit8 ALIGN(128): { *(.lit8) }

	.sdata ALIGN(128): {
		*(.sdata)
		*(.sdata.*)
		*(.gnu.linkonce.s*)
	}

	_edata = .;
	PROVIDE(edata = .);

	/* Uninitialized data.  */
	.sbss ALIGN(128) : {
		_fbss = . ;
		*(.sbss)
		*(.sbss.*)
		*(.gnu.linkonce.sb*)
		*(.scommon)
	}

	.bss ALIGN(128) : {
		*(SORT(.bss.kh.*))
		__kh_cbss_start_ov000 = .; *libkh_*_ov000*.a:*(.bss .bss.*) *libkh_*_ov000*.a:(COMMON) __kh_cbss_end_ov000 = .;
		__kh_cbss_start_ov001 = .; *libkh_*_ov001*.a:*(.bss .bss.*) *libkh_*_ov001*.a:(COMMON) __kh_cbss_end_ov001 = .;
		__kh_cbss_start_ov002 = .; *libkh_*_ov002*.a:*(.bss .bss.*) *libkh_*_ov002*.a:(COMMON) __kh_cbss_end_ov002 = .;
		__kh_cbss_start_ov003 = .; *libkh_*_ov003*.a:*(.bss .bss.*) *libkh_*_ov003*.a:(COMMON) __kh_cbss_end_ov003 = .;
		__kh_cbss_start_ov004 = .; *libkh_*_ov004*.a:*(.bss .bss.*) *libkh_*_ov004*.a:(COMMON) __kh_cbss_end_ov004 = .;
		__kh_cbss_start_ov005 = .; *libkh_*_ov005*.a:*(.bss .bss.*) *libkh_*_ov005*.a:(COMMON) __kh_cbss_end_ov005 = .;
		__kh_cbss_start_ov006 = .; *libkh_*_ov006*.a:*(.bss .bss.*) *libkh_*_ov006*.a:(COMMON) __kh_cbss_end_ov006 = .;
		__kh_cbss_start_ov007 = .; *libkh_*_ov007*.a:*(.bss .bss.*) *libkh_*_ov007*.a:(COMMON) __kh_cbss_end_ov007 = .;
		__kh_cbss_start_ov008 = .; *libkh_*_ov008*.a:*(.bss .bss.*) *libkh_*_ov008*.a:(COMMON) __kh_cbss_end_ov008 = .;
		__kh_cbss_start_ov009 = .; *libkh_*_ov009*.a:*(.bss .bss.*) *libkh_*_ov009*.a:(COMMON) __kh_cbss_end_ov009 = .;
		__kh_cbss_start_ov010 = .; *libkh_*_ov010*.a:*(.bss .bss.*) *libkh_*_ov010*.a:(COMMON) __kh_cbss_end_ov010 = .;
		__kh_cbss_start_ov011 = .; *libkh_*_ov011*.a:*(.bss .bss.*) *libkh_*_ov011*.a:(COMMON) __kh_cbss_end_ov011 = .;
		__kh_cbss_start_ov012 = .; *libkh_*_ov012*.a:*(.bss .bss.*) *libkh_*_ov012*.a:(COMMON) __kh_cbss_end_ov012 = .;
		__kh_cbss_start_ov013 = .; *libkh_*_ov013*.a:*(.bss .bss.*) *libkh_*_ov013*.a:(COMMON) __kh_cbss_end_ov013 = .;
		__kh_cbss_start_ov014 = .; *libkh_*_ov014*.a:*(.bss .bss.*) *libkh_*_ov014*.a:(COMMON) __kh_cbss_end_ov014 = .;
		__kh_cbss_start_ov015 = .; *libkh_*_ov015*.a:*(.bss .bss.*) *libkh_*_ov015*.a:(COMMON) __kh_cbss_end_ov015 = .;
		__kh_cbss_start_ov016 = .; *libkh_*_ov016*.a:*(.bss .bss.*) *libkh_*_ov016*.a:(COMMON) __kh_cbss_end_ov016 = .;
		__kh_cbss_start_ov017 = .; *libkh_*_ov017*.a:*(.bss .bss.*) *libkh_*_ov017*.a:(COMMON) __kh_cbss_end_ov017 = .;
		__kh_cbss_start_ov018 = .; *libkh_*_ov018*.a:*(.bss .bss.*) *libkh_*_ov018*.a:(COMMON) __kh_cbss_end_ov018 = .;
		__kh_cbss_start_ov019 = .; *libkh_*_ov019*.a:*(.bss .bss.*) *libkh_*_ov019*.a:(COMMON) __kh_cbss_end_ov019 = .;
		__kh_cbss_start_ov020 = .; *libkh_*_ov020*.a:*(.bss .bss.*) *libkh_*_ov020*.a:(COMMON) __kh_cbss_end_ov020 = .;
		__kh_cbss_start_ov021 = .; *libkh_*_ov021*.a:*(.bss .bss.*) *libkh_*_ov021*.a:(COMMON) __kh_cbss_end_ov021 = .;
		__kh_cbss_start_ov022 = .; *libkh_*_ov022*.a:*(.bss .bss.*) *libkh_*_ov022*.a:(COMMON) __kh_cbss_end_ov022 = .;
		__kh_cbss_start_ov023 = .; *libkh_*_ov023*.a:*(.bss .bss.*) *libkh_*_ov023*.a:(COMMON) __kh_cbss_end_ov023 = .;
		__kh_cbss_start_ov024 = .; *libkh_*_ov024*.a:*(.bss .bss.*) *libkh_*_ov024*.a:(COMMON) __kh_cbss_end_ov024 = .;
		__kh_cbss_start_ov025 = .; *libkh_*_ov025*.a:*(.bss .bss.*) *libkh_*_ov025*.a:(COMMON) __kh_cbss_end_ov025 = .;
		__kh_cbss_start_ov026 = .; *libkh_*_ov026*.a:*(.bss .bss.*) *libkh_*_ov026*.a:(COMMON) __kh_cbss_end_ov026 = .;
		__kh_cbss_start_ov027 = .; *libkh_*_ov027*.a:*(.bss .bss.*) *libkh_*_ov027*.a:(COMMON) __kh_cbss_end_ov027 = .;
		__kh_cbss_start_ov028 = .; *libkh_*_ov028*.a:*(.bss .bss.*) *libkh_*_ov028*.a:(COMMON) __kh_cbss_end_ov028 = .;
		__kh_cbss_start_ov029 = .; *libkh_*_ov029*.a:*(.bss .bss.*) *libkh_*_ov029*.a:(COMMON) __kh_cbss_end_ov029 = .;
		__kh_cbss_start_ov030 = .; *libkh_*_ov030*.a:*(.bss .bss.*) *libkh_*_ov030*.a:(COMMON) __kh_cbss_end_ov030 = .;
		__kh_cbss_start_ov031 = .; *libkh_*_ov031*.a:*(.bss .bss.*) *libkh_*_ov031*.a:(COMMON) __kh_cbss_end_ov031 = .;
		__kh_cbss_start_ov032 = .; *libkh_*_ov032*.a:*(.bss .bss.*) *libkh_*_ov032*.a:(COMMON) __kh_cbss_end_ov032 = .;
		__kh_cbss_start_ov033 = .; *libkh_*_ov033*.a:*(.bss .bss.*) *libkh_*_ov033*.a:(COMMON) __kh_cbss_end_ov033 = .;
		__kh_cbss_start_ov034 = .; *libkh_*_ov034*.a:*(.bss .bss.*) *libkh_*_ov034*.a:(COMMON) __kh_cbss_end_ov034 = .;
		__kh_cbss_start_ov035 = .; *libkh_*_ov035*.a:*(.bss .bss.*) *libkh_*_ov035*.a:(COMMON) __kh_cbss_end_ov035 = .;
		__kh_cbss_start_ov036 = .; *libkh_*_ov036*.a:*(.bss .bss.*) *libkh_*_ov036*.a:(COMMON) __kh_cbss_end_ov036 = .;
		__kh_cbss_start_ov037 = .; *libkh_*_ov037*.a:*(.bss .bss.*) *libkh_*_ov037*.a:(COMMON) __kh_cbss_end_ov037 = .;
		__kh_cbss_start_ov038 = .; *libkh_*_ov038*.a:*(.bss .bss.*) *libkh_*_ov038*.a:(COMMON) __kh_cbss_end_ov038 = .;
		__kh_cbss_start_ov039 = .; *libkh_*_ov039*.a:*(.bss .bss.*) *libkh_*_ov039*.a:(COMMON) __kh_cbss_end_ov039 = .;
		__kh_cbss_start_ov040 = .; *libkh_*_ov040*.a:*(.bss .bss.*) *libkh_*_ov040*.a:(COMMON) __kh_cbss_end_ov040 = .;
		__kh_cbss_start_ov041 = .; *libkh_*_ov041*.a:*(.bss .bss.*) *libkh_*_ov041*.a:(COMMON) __kh_cbss_end_ov041 = .;
		__kh_cbss_start_ov042 = .; *libkh_*_ov042*.a:*(.bss .bss.*) *libkh_*_ov042*.a:(COMMON) __kh_cbss_end_ov042 = .;
		__kh_cbss_start_ov043 = .; *libkh_*_ov043*.a:*(.bss .bss.*) *libkh_*_ov043*.a:(COMMON) __kh_cbss_end_ov043 = .;
		__kh_cbss_start_ov044 = .; *libkh_*_ov044*.a:*(.bss .bss.*) *libkh_*_ov044*.a:(COMMON) __kh_cbss_end_ov044 = .;
		__kh_cbss_start_ov045 = .; *libkh_*_ov045*.a:*(.bss .bss.*) *libkh_*_ov045*.a:(COMMON) __kh_cbss_end_ov045 = .;
		__kh_cbss_start_ov046 = .; *libkh_*_ov046*.a:*(.bss .bss.*) *libkh_*_ov046*.a:(COMMON) __kh_cbss_end_ov046 = .;
		__kh_cbss_start_ov047 = .; *libkh_*_ov047*.a:*(.bss .bss.*) *libkh_*_ov047*.a:(COMMON) __kh_cbss_end_ov047 = .;
		__kh_cbss_start_ov048 = .; *libkh_*_ov048*.a:*(.bss .bss.*) *libkh_*_ov048*.a:(COMMON) __kh_cbss_end_ov048 = .;
		__kh_cbss_start_ov049 = .; *libkh_*_ov049*.a:*(.bss .bss.*) *libkh_*_ov049*.a:(COMMON) __kh_cbss_end_ov049 = .;
		__kh_cbss_start_ov050 = .; *libkh_*_ov050*.a:*(.bss .bss.*) *libkh_*_ov050*.a:(COMMON) __kh_cbss_end_ov050 = .;
		__kh_cbss_start_ov051 = .; *libkh_*_ov051*.a:*(.bss .bss.*) *libkh_*_ov051*.a:(COMMON) __kh_cbss_end_ov051 = .;
		__kh_cbss_start_ov052 = .; *libkh_*_ov052*.a:*(.bss .bss.*) *libkh_*_ov052*.a:(COMMON) __kh_cbss_end_ov052 = .;
		__kh_cbss_start_ov053 = .; *libkh_*_ov053*.a:*(.bss .bss.*) *libkh_*_ov053*.a:(COMMON) __kh_cbss_end_ov053 = .;
		__kh_cbss_start_ov054 = .; *libkh_*_ov054*.a:*(.bss .bss.*) *libkh_*_ov054*.a:(COMMON) __kh_cbss_end_ov054 = .;
		__kh_cbss_start_ov055 = .; *libkh_*_ov055*.a:*(.bss .bss.*) *libkh_*_ov055*.a:(COMMON) __kh_cbss_end_ov055 = .;
		__kh_cbss_start_ov056 = .; *libkh_*_ov056*.a:*(.bss .bss.*) *libkh_*_ov056*.a:(COMMON) __kh_cbss_end_ov056 = .;
		__kh_cbss_start_ov057 = .; *libkh_*_ov057*.a:*(.bss .bss.*) *libkh_*_ov057*.a:(COMMON) __kh_cbss_end_ov057 = .;
		__kh_cbss_start_ov058 = .; *libkh_*_ov058*.a:*(.bss .bss.*) *libkh_*_ov058*.a:(COMMON) __kh_cbss_end_ov058 = .;
		__kh_cbss_start_ov059 = .; *libkh_*_ov059*.a:*(.bss .bss.*) *libkh_*_ov059*.a:(COMMON) __kh_cbss_end_ov059 = .;
		__kh_cbss_start_ov060 = .; *libkh_*_ov060*.a:*(.bss .bss.*) *libkh_*_ov060*.a:(COMMON) __kh_cbss_end_ov060 = .;
		__kh_cbss_start_ov061 = .; *libkh_*_ov061*.a:*(.bss .bss.*) *libkh_*_ov061*.a:(COMMON) __kh_cbss_end_ov061 = .;
		__kh_cbss_start_ov062 = .; *libkh_*_ov062*.a:*(.bss .bss.*) *libkh_*_ov062*.a:(COMMON) __kh_cbss_end_ov062 = .;
		__kh_cbss_start_ov063 = .; *libkh_*_ov063*.a:*(.bss .bss.*) *libkh_*_ov063*.a:(COMMON) __kh_cbss_end_ov063 = .;
		__kh_cbss_start_ov064 = .; *libkh_*_ov064*.a:*(.bss .bss.*) *libkh_*_ov064*.a:(COMMON) __kh_cbss_end_ov064 = .;
		__kh_cbss_start_ov065 = .; *libkh_*_ov065*.a:*(.bss .bss.*) *libkh_*_ov065*.a:(COMMON) __kh_cbss_end_ov065 = .;
		__kh_cbss_start_ov066 = .; *libkh_*_ov066*.a:*(.bss .bss.*) *libkh_*_ov066*.a:(COMMON) __kh_cbss_end_ov066 = .;
		__kh_cbss_start_ov067 = .; *libkh_*_ov067*.a:*(.bss .bss.*) *libkh_*_ov067*.a:(COMMON) __kh_cbss_end_ov067 = .;
		__kh_cbss_start_ov068 = .; *libkh_*_ov068*.a:*(.bss .bss.*) *libkh_*_ov068*.a:(COMMON) __kh_cbss_end_ov068 = .;
		__kh_cbss_start_ov069 = .; *libkh_*_ov069*.a:*(.bss .bss.*) *libkh_*_ov069*.a:(COMMON) __kh_cbss_end_ov069 = .;
		__kh_cbss_start_ov070 = .; *libkh_*_ov070*.a:*(.bss .bss.*) *libkh_*_ov070*.a:(COMMON) __kh_cbss_end_ov070 = .;
		__kh_cbss_start_ov071 = .; *libkh_*_ov071*.a:*(.bss .bss.*) *libkh_*_ov071*.a:(COMMON) __kh_cbss_end_ov071 = .;
		__kh_cbss_start_ov072 = .; *libkh_*_ov072*.a:*(.bss .bss.*) *libkh_*_ov072*.a:(COMMON) __kh_cbss_end_ov072 = .;
		__kh_cbss_start_ov073 = .; *libkh_*_ov073*.a:*(.bss .bss.*) *libkh_*_ov073*.a:(COMMON) __kh_cbss_end_ov073 = .;
		__kh_cbss_start_ov074 = .; *libkh_*_ov074*.a:*(.bss .bss.*) *libkh_*_ov074*.a:(COMMON) __kh_cbss_end_ov074 = .;
		__kh_cbss_start_ov075 = .; *libkh_*_ov075*.a:*(.bss .bss.*) *libkh_*_ov075*.a:(COMMON) __kh_cbss_end_ov075 = .;
		__kh_cbss_start_ov076 = .; *libkh_*_ov076*.a:*(.bss .bss.*) *libkh_*_ov076*.a:(COMMON) __kh_cbss_end_ov076 = .;
		__kh_cbss_start_ov077 = .; *libkh_*_ov077*.a:*(.bss .bss.*) *libkh_*_ov077*.a:(COMMON) __kh_cbss_end_ov077 = .;
		__kh_cbss_start_ov078 = .; *libkh_*_ov078*.a:*(.bss .bss.*) *libkh_*_ov078*.a:(COMMON) __kh_cbss_end_ov078 = .;
		__kh_cbss_start_ov079 = .; *libkh_*_ov079*.a:*(.bss .bss.*) *libkh_*_ov079*.a:(COMMON) __kh_cbss_end_ov079 = .;
		__kh_cbss_start_ov080 = .; *libkh_*_ov080*.a:*(.bss .bss.*) *libkh_*_ov080*.a:(COMMON) __kh_cbss_end_ov080 = .;
		__kh_cbss_start_ov081 = .; *libkh_*_ov081*.a:*(.bss .bss.*) *libkh_*_ov081*.a:(COMMON) __kh_cbss_end_ov081 = .;
		__kh_cbss_start_ov082 = .; *libkh_*_ov082*.a:*(.bss .bss.*) *libkh_*_ov082*.a:(COMMON) __kh_cbss_end_ov082 = .;
		__kh_cbss_start_ov083 = .; *libkh_*_ov083*.a:*(.bss .bss.*) *libkh_*_ov083*.a:(COMMON) __kh_cbss_end_ov083 = .;
		__kh_cbss_start_ov084 = .; *libkh_*_ov084*.a:*(.bss .bss.*) *libkh_*_ov084*.a:(COMMON) __kh_cbss_end_ov084 = .;
		__kh_cbss_start_ov085 = .; *libkh_*_ov085*.a:*(.bss .bss.*) *libkh_*_ov085*.a:(COMMON) __kh_cbss_end_ov085 = .;
		__kh_cbss_start_ov086 = .; *libkh_*_ov086*.a:*(.bss .bss.*) *libkh_*_ov086*.a:(COMMON) __kh_cbss_end_ov086 = .;
		__kh_cbss_start_ov087 = .; *libkh_*_ov087*.a:*(.bss .bss.*) *libkh_*_ov087*.a:(COMMON) __kh_cbss_end_ov087 = .;
		__kh_cbss_start_ov088 = .; *libkh_*_ov088*.a:*(.bss .bss.*) *libkh_*_ov088*.a:(COMMON) __kh_cbss_end_ov088 = .;
		__kh_cbss_start_ov089 = .; *libkh_*_ov089*.a:*(.bss .bss.*) *libkh_*_ov089*.a:(COMMON) __kh_cbss_end_ov089 = .;
		__kh_cbss_start_ov090 = .; *libkh_*_ov090*.a:*(.bss .bss.*) *libkh_*_ov090*.a:(COMMON) __kh_cbss_end_ov090 = .;
		__kh_cbss_start_ov091 = .; *libkh_*_ov091*.a:*(.bss .bss.*) *libkh_*_ov091*.a:(COMMON) __kh_cbss_end_ov091 = .;
		__kh_cbss_start_ov092 = .; *libkh_*_ov092*.a:*(.bss .bss.*) *libkh_*_ov092*.a:(COMMON) __kh_cbss_end_ov092 = .;
		__kh_cbss_start_ov093 = .; *libkh_*_ov093*.a:*(.bss .bss.*) *libkh_*_ov093*.a:(COMMON) __kh_cbss_end_ov093 = .;
		__kh_cbss_start_ov094 = .; *libkh_*_ov094*.a:*(.bss .bss.*) *libkh_*_ov094*.a:(COMMON) __kh_cbss_end_ov094 = .;
		__kh_cbss_start_ov095 = .; *libkh_*_ov095*.a:*(.bss .bss.*) *libkh_*_ov095*.a:(COMMON) __kh_cbss_end_ov095 = .;
		__kh_cbss_start_ov096 = .; *libkh_*_ov096*.a:*(.bss .bss.*) *libkh_*_ov096*.a:(COMMON) __kh_cbss_end_ov096 = .;
		__kh_cbss_start_ov097 = .; *libkh_*_ov097*.a:*(.bss .bss.*) *libkh_*_ov097*.a:(COMMON) __kh_cbss_end_ov097 = .;
		__kh_cbss_start_ov098 = .; *libkh_*_ov098*.a:*(.bss .bss.*) *libkh_*_ov098*.a:(COMMON) __kh_cbss_end_ov098 = .;
		__kh_cbss_start_ov099 = .; *libkh_*_ov099*.a:*(.bss .bss.*) *libkh_*_ov099*.a:(COMMON) __kh_cbss_end_ov099 = .;
		__kh_cbss_start_ov100 = .; *libkh_*_ov100*.a:*(.bss .bss.*) *libkh_*_ov100*.a:(COMMON) __kh_cbss_end_ov100 = .;
		__kh_cbss_start_ov101 = .; *libkh_*_ov101*.a:*(.bss .bss.*) *libkh_*_ov101*.a:(COMMON) __kh_cbss_end_ov101 = .;
		__kh_cbss_start_ov102 = .; *libkh_*_ov102*.a:*(.bss .bss.*) *libkh_*_ov102*.a:(COMMON) __kh_cbss_end_ov102 = .;
		__kh_cbss_start_ov103 = .; *libkh_*_ov103*.a:*(.bss .bss.*) *libkh_*_ov103*.a:(COMMON) __kh_cbss_end_ov103 = .;
		__kh_cbss_start_ov104 = .; *libkh_*_ov104*.a:*(.bss .bss.*) *libkh_*_ov104*.a:(COMMON) __kh_cbss_end_ov104 = .;
		__kh_cbss_start_ov105 = .; *libkh_*_ov105*.a:*(.bss .bss.*) *libkh_*_ov105*.a:(COMMON) __kh_cbss_end_ov105 = .;
		__kh_cbss_start_ov106 = .; *libkh_*_ov106*.a:*(.bss .bss.*) *libkh_*_ov106*.a:(COMMON) __kh_cbss_end_ov106 = .;
		__kh_cbss_start_ov107 = .; *libkh_*_ov107*.a:*(.bss .bss.*) *libkh_*_ov107*.a:(COMMON) __kh_cbss_end_ov107 = .;
		__kh_cbss_start_ov108 = .; *libkh_*_ov108*.a:*(.bss .bss.*) *libkh_*_ov108*.a:(COMMON) __kh_cbss_end_ov108 = .;
		__kh_cbss_start_ov109 = .; *libkh_*_ov109*.a:*(.bss .bss.*) *libkh_*_ov109*.a:(COMMON) __kh_cbss_end_ov109 = .;
		__kh_cbss_start_ov110 = .; *libkh_*_ov110*.a:*(.bss .bss.*) *libkh_*_ov110*.a:(COMMON) __kh_cbss_end_ov110 = .;
		__kh_cbss_start_ov111 = .; *libkh_*_ov111*.a:*(.bss .bss.*) *libkh_*_ov111*.a:(COMMON) __kh_cbss_end_ov111 = .;
		__kh_cbss_start_ov112 = .; *libkh_*_ov112*.a:*(.bss .bss.*) *libkh_*_ov112*.a:(COMMON) __kh_cbss_end_ov112 = .;
		__kh_cbss_start_ov113 = .; *libkh_*_ov113*.a:*(.bss .bss.*) *libkh_*_ov113*.a:(COMMON) __kh_cbss_end_ov113 = .;
		__kh_cbss_start_ov114 = .; *libkh_*_ov114*.a:*(.bss .bss.*) *libkh_*_ov114*.a:(COMMON) __kh_cbss_end_ov114 = .;
		__kh_cbss_start_ov115 = .; *libkh_*_ov115*.a:*(.bss .bss.*) *libkh_*_ov115*.a:(COMMON) __kh_cbss_end_ov115 = .;
		__kh_cbss_start_ov116 = .; *libkh_*_ov116*.a:*(.bss .bss.*) *libkh_*_ov116*.a:(COMMON) __kh_cbss_end_ov116 = .;
		__kh_cbss_start_ov117 = .; *libkh_*_ov117*.a:*(.bss .bss.*) *libkh_*_ov117*.a:(COMMON) __kh_cbss_end_ov117 = .;
		__kh_cbss_start_ov118 = .; *libkh_*_ov118*.a:*(.bss .bss.*) *libkh_*_ov118*.a:(COMMON) __kh_cbss_end_ov118 = .;
		__kh_cbss_start_ov119 = .; *libkh_*_ov119*.a:*(.bss .bss.*) *libkh_*_ov119*.a:(COMMON) __kh_cbss_end_ov119 = .;
		__kh_cbss_start_ov120 = .; *libkh_*_ov120*.a:*(.bss .bss.*) *libkh_*_ov120*.a:(COMMON) __kh_cbss_end_ov120 = .;
		__kh_cbss_start_ov121 = .; *libkh_*_ov121*.a:*(.bss .bss.*) *libkh_*_ov121*.a:(COMMON) __kh_cbss_end_ov121 = .;
		__kh_cbss_start_ov122 = .; *libkh_*_ov122*.a:*(.bss .bss.*) *libkh_*_ov122*.a:(COMMON) __kh_cbss_end_ov122 = .;
		__kh_cbss_start_ov123 = .; *libkh_*_ov123*.a:*(.bss .bss.*) *libkh_*_ov123*.a:(COMMON) __kh_cbss_end_ov123 = .;
		__kh_cbss_start_ov124 = .; *libkh_*_ov124*.a:*(.bss .bss.*) *libkh_*_ov124*.a:(COMMON) __kh_cbss_end_ov124 = .;
		__kh_cbss_start_ov125 = .; *libkh_*_ov125*.a:*(.bss .bss.*) *libkh_*_ov125*.a:(COMMON) __kh_cbss_end_ov125 = .;
		__kh_cbss_start_ov126 = .; *libkh_*_ov126*.a:*(.bss .bss.*) *libkh_*_ov126*.a:(COMMON) __kh_cbss_end_ov126 = .;
		__kh_cbss_start_ov127 = .; *libkh_*_ov127*.a:*(.bss .bss.*) *libkh_*_ov127*.a:(COMMON) __kh_cbss_end_ov127 = .;
		__kh_cbss_start_ov128 = .; *libkh_*_ov128*.a:*(.bss .bss.*) *libkh_*_ov128*.a:(COMMON) __kh_cbss_end_ov128 = .;
		__kh_cbss_start_ov129 = .; *libkh_*_ov129*.a:*(.bss .bss.*) *libkh_*_ov129*.a:(COMMON) __kh_cbss_end_ov129 = .;
		__kh_cbss_start_ov130 = .; *libkh_*_ov130*.a:*(.bss .bss.*) *libkh_*_ov130*.a:(COMMON) __kh_cbss_end_ov130 = .;
		__kh_cbss_start_ov131 = .; *libkh_*_ov131*.a:*(.bss .bss.*) *libkh_*_ov131*.a:(COMMON) __kh_cbss_end_ov131 = .;
		__kh_cbss_start_ov132 = .; *libkh_*_ov132*.a:*(.bss .bss.*) *libkh_*_ov132*.a:(COMMON) __kh_cbss_end_ov132 = .;
		__kh_cbss_start_ov133 = .; *libkh_*_ov133*.a:*(.bss .bss.*) *libkh_*_ov133*.a:(COMMON) __kh_cbss_end_ov133 = .;
		__kh_cbss_start_ov134 = .; *libkh_*_ov134*.a:*(.bss .bss.*) *libkh_*_ov134*.a:(COMMON) __kh_cbss_end_ov134 = .;
		__kh_cbss_start_ov135 = .; *libkh_*_ov135*.a:*(.bss .bss.*) *libkh_*_ov135*.a:(COMMON) __kh_cbss_end_ov135 = .;
		__kh_cbss_start_ov136 = .; *libkh_*_ov136*.a:*(.bss .bss.*) *libkh_*_ov136*.a:(COMMON) __kh_cbss_end_ov136 = .;
		__kh_cbss_start_ov137 = .; *libkh_*_ov137*.a:*(.bss .bss.*) *libkh_*_ov137*.a:(COMMON) __kh_cbss_end_ov137 = .;
		__kh_cbss_start_ov138 = .; *libkh_*_ov138*.a:*(.bss .bss.*) *libkh_*_ov138*.a:(COMMON) __kh_cbss_end_ov138 = .;
		__kh_cbss_start_ov139 = .; *libkh_*_ov139*.a:*(.bss .bss.*) *libkh_*_ov139*.a:(COMMON) __kh_cbss_end_ov139 = .;
		__kh_cbss_start_ov140 = .; *libkh_*_ov140*.a:*(.bss .bss.*) *libkh_*_ov140*.a:(COMMON) __kh_cbss_end_ov140 = .;
		__kh_cbss_start_ov141 = .; *libkh_*_ov141*.a:*(.bss .bss.*) *libkh_*_ov141*.a:(COMMON) __kh_cbss_end_ov141 = .;
		__kh_cbss_start_ov142 = .; *libkh_*_ov142*.a:*(.bss .bss.*) *libkh_*_ov142*.a:(COMMON) __kh_cbss_end_ov142 = .;
		__kh_cbss_start_ov143 = .; *libkh_*_ov143*.a:*(.bss .bss.*) *libkh_*_ov143*.a:(COMMON) __kh_cbss_end_ov143 = .;
		__kh_cbss_start_ov144 = .; *libkh_*_ov144*.a:*(.bss .bss.*) *libkh_*_ov144*.a:(COMMON) __kh_cbss_end_ov144 = .;
		__kh_cbss_start_ov145 = .; *libkh_*_ov145*.a:*(.bss .bss.*) *libkh_*_ov145*.a:(COMMON) __kh_cbss_end_ov145 = .;
		__kh_cbss_start_ov146 = .; *libkh_*_ov146*.a:*(.bss .bss.*) *libkh_*_ov146*.a:(COMMON) __kh_cbss_end_ov146 = .;
		__kh_cbss_start_ov147 = .; *libkh_*_ov147*.a:*(.bss .bss.*) *libkh_*_ov147*.a:(COMMON) __kh_cbss_end_ov147 = .;
		__kh_cbss_start_ov148 = .; *libkh_*_ov148*.a:*(.bss .bss.*) *libkh_*_ov148*.a:(COMMON) __kh_cbss_end_ov148 = .;
		__kh_cbss_start_ov149 = .; *libkh_*_ov149*.a:*(.bss .bss.*) *libkh_*_ov149*.a:(COMMON) __kh_cbss_end_ov149 = .;
		__kh_cbss_start_ov150 = .; *libkh_*_ov150*.a:*(.bss .bss.*) *libkh_*_ov150*.a:(COMMON) __kh_cbss_end_ov150 = .;
		__kh_cbss_start_ov151 = .; *libkh_*_ov151*.a:*(.bss .bss.*) *libkh_*_ov151*.a:(COMMON) __kh_cbss_end_ov151 = .;
		__kh_cbss_start_ov152 = .; *libkh_*_ov152*.a:*(.bss .bss.*) *libkh_*_ov152*.a:(COMMON) __kh_cbss_end_ov152 = .;
		__kh_cbss_start_ov153 = .; *libkh_*_ov153*.a:*(.bss .bss.*) *libkh_*_ov153*.a:(COMMON) __kh_cbss_end_ov153 = .;
		__kh_cbss_start_ov154 = .; *libkh_*_ov154*.a:*(.bss .bss.*) *libkh_*_ov154*.a:(COMMON) __kh_cbss_end_ov154 = .;
		__kh_cbss_start_ov155 = .; *libkh_*_ov155*.a:*(.bss .bss.*) *libkh_*_ov155*.a:(COMMON) __kh_cbss_end_ov155 = .;
		__kh_cbss_start_ov156 = .; *libkh_*_ov156*.a:*(.bss .bss.*) *libkh_*_ov156*.a:(COMMON) __kh_cbss_end_ov156 = .;
		__kh_cbss_start_ov157 = .; *libkh_*_ov157*.a:*(.bss .bss.*) *libkh_*_ov157*.a:(COMMON) __kh_cbss_end_ov157 = .;
		__kh_cbss_start_ov158 = .; *libkh_*_ov158*.a:*(.bss .bss.*) *libkh_*_ov158*.a:(COMMON) __kh_cbss_end_ov158 = .;
		__kh_cbss_start_ov159 = .; *libkh_*_ov159*.a:*(.bss .bss.*) *libkh_*_ov159*.a:(COMMON) __kh_cbss_end_ov159 = .;
		__kh_cbss_start_ov160 = .; *libkh_*_ov160*.a:*(.bss .bss.*) *libkh_*_ov160*.a:(COMMON) __kh_cbss_end_ov160 = .;
		__kh_cbss_start_ov161 = .; *libkh_*_ov161*.a:*(.bss .bss.*) *libkh_*_ov161*.a:(COMMON) __kh_cbss_end_ov161 = .;
		__kh_cbss_start_ov162 = .; *libkh_*_ov162*.a:*(.bss .bss.*) *libkh_*_ov162*.a:(COMMON) __kh_cbss_end_ov162 = .;
		__kh_cbss_start_ov163 = .; *libkh_*_ov163*.a:*(.bss .bss.*) *libkh_*_ov163*.a:(COMMON) __kh_cbss_end_ov163 = .;
		__kh_cbss_start_ov164 = .; *libkh_*_ov164*.a:*(.bss .bss.*) *libkh_*_ov164*.a:(COMMON) __kh_cbss_end_ov164 = .;
		__kh_cbss_start_ov165 = .; *libkh_*_ov165*.a:*(.bss .bss.*) *libkh_*_ov165*.a:(COMMON) __kh_cbss_end_ov165 = .;
		__kh_cbss_start_ov166 = .; *libkh_*_ov166*.a:*(.bss .bss.*) *libkh_*_ov166*.a:(COMMON) __kh_cbss_end_ov166 = .;
		__kh_cbss_start_ov167 = .; *libkh_*_ov167*.a:*(.bss .bss.*) *libkh_*_ov167*.a:(COMMON) __kh_cbss_end_ov167 = .;
		__kh_cbss_start_ov168 = .; *libkh_*_ov168*.a:*(.bss .bss.*) *libkh_*_ov168*.a:(COMMON) __kh_cbss_end_ov168 = .;
		__kh_cbss_start_ov169 = .; *libkh_*_ov169*.a:*(.bss .bss.*) *libkh_*_ov169*.a:(COMMON) __kh_cbss_end_ov169 = .;
		__kh_cbss_start_ov170 = .; *libkh_*_ov170*.a:*(.bss .bss.*) *libkh_*_ov170*.a:(COMMON) __kh_cbss_end_ov170 = .;
		__kh_cbss_start_ov171 = .; *libkh_*_ov171*.a:*(.bss .bss.*) *libkh_*_ov171*.a:(COMMON) __kh_cbss_end_ov171 = .;
		__kh_cbss_start_ov172 = .; *libkh_*_ov172*.a:*(.bss .bss.*) *libkh_*_ov172*.a:(COMMON) __kh_cbss_end_ov172 = .;
		__kh_cbss_start_ov173 = .; *libkh_*_ov173*.a:*(.bss .bss.*) *libkh_*_ov173*.a:(COMMON) __kh_cbss_end_ov173 = .;
		__kh_cbss_start_ov174 = .; *libkh_*_ov174*.a:*(.bss .bss.*) *libkh_*_ov174*.a:(COMMON) __kh_cbss_end_ov174 = .;
		__kh_cbss_start_ov175 = .; *libkh_*_ov175*.a:*(.bss .bss.*) *libkh_*_ov175*.a:(COMMON) __kh_cbss_end_ov175 = .;
		__kh_cbss_start_ov176 = .; *libkh_*_ov176*.a:*(.bss .bss.*) *libkh_*_ov176*.a:(COMMON) __kh_cbss_end_ov176 = .;
		__kh_cbss_start_ov177 = .; *libkh_*_ov177*.a:*(.bss .bss.*) *libkh_*_ov177*.a:(COMMON) __kh_cbss_end_ov177 = .;
		__kh_cbss_start_ov178 = .; *libkh_*_ov178*.a:*(.bss .bss.*) *libkh_*_ov178*.a:(COMMON) __kh_cbss_end_ov178 = .;
		__kh_cbss_start_ov179 = .; *libkh_*_ov179*.a:*(.bss .bss.*) *libkh_*_ov179*.a:(COMMON) __kh_cbss_end_ov179 = .;
		__kh_cbss_start_ov180 = .; *libkh_*_ov180*.a:*(.bss .bss.*) *libkh_*_ov180*.a:(COMMON) __kh_cbss_end_ov180 = .;
		__kh_cbss_start_ov181 = .; *libkh_*_ov181*.a:*(.bss .bss.*) *libkh_*_ov181*.a:(COMMON) __kh_cbss_end_ov181 = .;
		__kh_cbss_start_ov182 = .; *libkh_*_ov182*.a:*(.bss .bss.*) *libkh_*_ov182*.a:(COMMON) __kh_cbss_end_ov182 = .;
		__kh_cbss_start_ov183 = .; *libkh_*_ov183*.a:*(.bss .bss.*) *libkh_*_ov183*.a:(COMMON) __kh_cbss_end_ov183 = .;
		__kh_cbss_start_ov184 = .; *libkh_*_ov184*.a:*(.bss .bss.*) *libkh_*_ov184*.a:(COMMON) __kh_cbss_end_ov184 = .;
		__kh_cbss_start_ov185 = .; *libkh_*_ov185*.a:*(.bss .bss.*) *libkh_*_ov185*.a:(COMMON) __kh_cbss_end_ov185 = .;
		__kh_cbss_start_ov186 = .; *libkh_*_ov186*.a:*(.bss .bss.*) *libkh_*_ov186*.a:(COMMON) __kh_cbss_end_ov186 = .;
		__kh_cbss_start_ov187 = .; *libkh_*_ov187*.a:*(.bss .bss.*) *libkh_*_ov187*.a:(COMMON) __kh_cbss_end_ov187 = .;
		__kh_cbss_start_ov188 = .; *libkh_*_ov188*.a:*(.bss .bss.*) *libkh_*_ov188*.a:(COMMON) __kh_cbss_end_ov188 = .;
		__kh_cbss_start_ov189 = .; *libkh_*_ov189*.a:*(.bss .bss.*) *libkh_*_ov189*.a:(COMMON) __kh_cbss_end_ov189 = .;
		__kh_cbss_start_ov190 = .; *libkh_*_ov190*.a:*(.bss .bss.*) *libkh_*_ov190*.a:(COMMON) __kh_cbss_end_ov190 = .;
		__kh_cbss_start_ov191 = .; *libkh_*_ov191*.a:*(.bss .bss.*) *libkh_*_ov191*.a:(COMMON) __kh_cbss_end_ov191 = .;
		__kh_cbss_start_ov192 = .; *libkh_*_ov192*.a:*(.bss .bss.*) *libkh_*_ov192*.a:(COMMON) __kh_cbss_end_ov192 = .;
		__kh_cbss_start_ov193 = .; *libkh_*_ov193*.a:*(.bss .bss.*) *libkh_*_ov193*.a:(COMMON) __kh_cbss_end_ov193 = .;
		__kh_cbss_start_ov194 = .; *libkh_*_ov194*.a:*(.bss .bss.*) *libkh_*_ov194*.a:(COMMON) __kh_cbss_end_ov194 = .;
		__kh_cbss_start_ov195 = .; *libkh_*_ov195*.a:*(.bss .bss.*) *libkh_*_ov195*.a:(COMMON) __kh_cbss_end_ov195 = .;
		__kh_cbss_start_ov196 = .; *libkh_*_ov196*.a:*(.bss .bss.*) *libkh_*_ov196*.a:(COMMON) __kh_cbss_end_ov196 = .;
		__kh_cbss_start_ov197 = .; *libkh_*_ov197*.a:*(.bss .bss.*) *libkh_*_ov197*.a:(COMMON) __kh_cbss_end_ov197 = .;
		__kh_cbss_start_ov198 = .; *libkh_*_ov198*.a:*(.bss .bss.*) *libkh_*_ov198*.a:(COMMON) __kh_cbss_end_ov198 = .;
		__kh_cbss_start_ov199 = .; *libkh_*_ov199*.a:*(.bss .bss.*) *libkh_*_ov199*.a:(COMMON) __kh_cbss_end_ov199 = .;
		__kh_cbss_start_ov200 = .; *libkh_*_ov200*.a:*(.bss .bss.*) *libkh_*_ov200*.a:(COMMON) __kh_cbss_end_ov200 = .;
		__kh_cbss_start_ov201 = .; *libkh_*_ov201*.a:*(.bss .bss.*) *libkh_*_ov201*.a:(COMMON) __kh_cbss_end_ov201 = .;
		__kh_cbss_start_ov202 = .; *libkh_*_ov202*.a:*(.bss .bss.*) *libkh_*_ov202*.a:(COMMON) __kh_cbss_end_ov202 = .;
		__kh_cbss_start_ov203 = .; *libkh_*_ov203*.a:*(.bss .bss.*) *libkh_*_ov203*.a:(COMMON) __kh_cbss_end_ov203 = .;
		__kh_cbss_start_ov204 = .; *libkh_*_ov204*.a:*(.bss .bss.*) *libkh_*_ov204*.a:(COMMON) __kh_cbss_end_ov204 = .;
		__kh_cbss_start_ov205 = .; *libkh_*_ov205*.a:*(.bss .bss.*) *libkh_*_ov205*.a:(COMMON) __kh_cbss_end_ov205 = .;
		__kh_cbss_start_ov206 = .; *libkh_*_ov206*.a:*(.bss .bss.*) *libkh_*_ov206*.a:(COMMON) __kh_cbss_end_ov206 = .;
		__kh_cbss_start_ov207 = .; *libkh_*_ov207*.a:*(.bss .bss.*) *libkh_*_ov207*.a:(COMMON) __kh_cbss_end_ov207 = .;
		__kh_cbss_start_ov208 = .; *libkh_*_ov208*.a:*(.bss .bss.*) *libkh_*_ov208*.a:(COMMON) __kh_cbss_end_ov208 = .;
		__kh_cbss_start_ov209 = .; *libkh_*_ov209*.a:*(.bss .bss.*) *libkh_*_ov209*.a:(COMMON) __kh_cbss_end_ov209 = .;
		__kh_cbss_start_ov210 = .; *libkh_*_ov210*.a:*(.bss .bss.*) *libkh_*_ov210*.a:(COMMON) __kh_cbss_end_ov210 = .;
		__kh_cbss_start_ov211 = .; *libkh_*_ov211*.a:*(.bss .bss.*) *libkh_*_ov211*.a:(COMMON) __kh_cbss_end_ov211 = .;
		__kh_cbss_start_ov212 = .; *libkh_*_ov212*.a:*(.bss .bss.*) *libkh_*_ov212*.a:(COMMON) __kh_cbss_end_ov212 = .;
		__kh_cbss_start_ov213 = .; *libkh_*_ov213*.a:*(.bss .bss.*) *libkh_*_ov213*.a:(COMMON) __kh_cbss_end_ov213 = .;
		__kh_cbss_start_ov214 = .; *libkh_*_ov214*.a:*(.bss .bss.*) *libkh_*_ov214*.a:(COMMON) __kh_cbss_end_ov214 = .;
		__kh_cbss_start_ov215 = .; *libkh_*_ov215*.a:*(.bss .bss.*) *libkh_*_ov215*.a:(COMMON) __kh_cbss_end_ov215 = .;
		__kh_cbss_start_ov216 = .; *libkh_*_ov216*.a:*(.bss .bss.*) *libkh_*_ov216*.a:(COMMON) __kh_cbss_end_ov216 = .;
		__kh_cbss_start_ov217 = .; *libkh_*_ov217*.a:*(.bss .bss.*) *libkh_*_ov217*.a:(COMMON) __kh_cbss_end_ov217 = .;
		__kh_cbss_start_ov218 = .; *libkh_*_ov218*.a:*(.bss .bss.*) *libkh_*_ov218*.a:(COMMON) __kh_cbss_end_ov218 = .;
		__kh_cbss_start_ov219 = .; *libkh_*_ov219*.a:*(.bss .bss.*) *libkh_*_ov219*.a:(COMMON) __kh_cbss_end_ov219 = .;
		__kh_cbss_start_ov220 = .; *libkh_*_ov220*.a:*(.bss .bss.*) *libkh_*_ov220*.a:(COMMON) __kh_cbss_end_ov220 = .;
		__kh_cbss_start_ov221 = .; *libkh_*_ov221*.a:*(.bss .bss.*) *libkh_*_ov221*.a:(COMMON) __kh_cbss_end_ov221 = .;
		__kh_cbss_start_ov222 = .; *libkh_*_ov222*.a:*(.bss .bss.*) *libkh_*_ov222*.a:(COMMON) __kh_cbss_end_ov222 = .;
		__kh_cbss_start_ov223 = .; *libkh_*_ov223*.a:*(.bss .bss.*) *libkh_*_ov223*.a:(COMMON) __kh_cbss_end_ov223 = .;
		__kh_cbss_start_ov224 = .; *libkh_*_ov224*.a:*(.bss .bss.*) *libkh_*_ov224*.a:(COMMON) __kh_cbss_end_ov224 = .;
		__kh_cbss_start_ov225 = .; *libkh_*_ov225*.a:*(.bss .bss.*) *libkh_*_ov225*.a:(COMMON) __kh_cbss_end_ov225 = .;
		__kh_cbss_start_ov226 = .; *libkh_*_ov226*.a:*(.bss .bss.*) *libkh_*_ov226*.a:(COMMON) __kh_cbss_end_ov226 = .;
		__kh_cbss_start_ov227 = .; *libkh_*_ov227*.a:*(.bss .bss.*) *libkh_*_ov227*.a:(COMMON) __kh_cbss_end_ov227 = .;
		__kh_cbss_start_ov228 = .; *libkh_*_ov228*.a:*(.bss .bss.*) *libkh_*_ov228*.a:(COMMON) __kh_cbss_end_ov228 = .;
		__kh_cbss_start_ov229 = .; *libkh_*_ov229*.a:*(.bss .bss.*) *libkh_*_ov229*.a:(COMMON) __kh_cbss_end_ov229 = .;
		__kh_cbss_start_ov230 = .; *libkh_*_ov230*.a:*(.bss .bss.*) *libkh_*_ov230*.a:(COMMON) __kh_cbss_end_ov230 = .;
		__kh_cbss_start_ov231 = .; *libkh_*_ov231*.a:*(.bss .bss.*) *libkh_*_ov231*.a:(COMMON) __kh_cbss_end_ov231 = .;
		__kh_cbss_start_ov232 = .; *libkh_*_ov232*.a:*(.bss .bss.*) *libkh_*_ov232*.a:(COMMON) __kh_cbss_end_ov232 = .;
		__kh_cbss_start_ov233 = .; *libkh_*_ov233*.a:*(.bss .bss.*) *libkh_*_ov233*.a:(COMMON) __kh_cbss_end_ov233 = .;
		__kh_cbss_start_ov234 = .; *libkh_*_ov234*.a:*(.bss .bss.*) *libkh_*_ov234*.a:(COMMON) __kh_cbss_end_ov234 = .;
		__kh_cbss_start_ov235 = .; *libkh_*_ov235*.a:*(.bss .bss.*) *libkh_*_ov235*.a:(COMMON) __kh_cbss_end_ov235 = .;
		__kh_cbss_start_ov236 = .; *libkh_*_ov236*.a:*(.bss .bss.*) *libkh_*_ov236*.a:(COMMON) __kh_cbss_end_ov236 = .;
		__kh_cbss_start_ov237 = .; *libkh_*_ov237*.a:*(.bss .bss.*) *libkh_*_ov237*.a:(COMMON) __kh_cbss_end_ov237 = .;
		__kh_cbss_start_ov238 = .; *libkh_*_ov238*.a:*(.bss .bss.*) *libkh_*_ov238*.a:(COMMON) __kh_cbss_end_ov238 = .;
		__kh_cbss_start_ov239 = .; *libkh_*_ov239*.a:*(.bss .bss.*) *libkh_*_ov239*.a:(COMMON) __kh_cbss_end_ov239 = .;
		__kh_cbss_start_ov240 = .; *libkh_*_ov240*.a:*(.bss .bss.*) *libkh_*_ov240*.a:(COMMON) __kh_cbss_end_ov240 = .;
		__kh_cbss_start_ov241 = .; *libkh_*_ov241*.a:*(.bss .bss.*) *libkh_*_ov241*.a:(COMMON) __kh_cbss_end_ov241 = .;
		__kh_cbss_start_ov242 = .; *libkh_*_ov242*.a:*(.bss .bss.*) *libkh_*_ov242*.a:(COMMON) __kh_cbss_end_ov242 = .;
		__kh_cbss_start_ov243 = .; *libkh_*_ov243*.a:*(.bss .bss.*) *libkh_*_ov243*.a:(COMMON) __kh_cbss_end_ov243 = .;
		__kh_cbss_start_ov244 = .; *libkh_*_ov244*.a:*(.bss .bss.*) *libkh_*_ov244*.a:(COMMON) __kh_cbss_end_ov244 = .;
		__kh_cbss_start_ov245 = .; *libkh_*_ov245*.a:*(.bss .bss.*) *libkh_*_ov245*.a:(COMMON) __kh_cbss_end_ov245 = .;
		__kh_cbss_start_ov246 = .; *libkh_*_ov246*.a:*(.bss .bss.*) *libkh_*_ov246*.a:(COMMON) __kh_cbss_end_ov246 = .;
		__kh_cbss_start_ov247 = .; *libkh_*_ov247*.a:*(.bss .bss.*) *libkh_*_ov247*.a:(COMMON) __kh_cbss_end_ov247 = .;
		__kh_cbss_start_ov248 = .; *libkh_*_ov248*.a:*(.bss .bss.*) *libkh_*_ov248*.a:(COMMON) __kh_cbss_end_ov248 = .;
		__kh_cbss_start_ov249 = .; *libkh_*_ov249*.a:*(.bss .bss.*) *libkh_*_ov249*.a:(COMMON) __kh_cbss_end_ov249 = .;
		__kh_cbss_start_ov250 = .; *libkh_*_ov250*.a:*(.bss .bss.*) *libkh_*_ov250*.a:(COMMON) __kh_cbss_end_ov250 = .;
		__kh_cbss_start_ov251 = .; *libkh_*_ov251*.a:*(.bss .bss.*) *libkh_*_ov251*.a:(COMMON) __kh_cbss_end_ov251 = .;
		__kh_cbss_start_ov252 = .; *libkh_*_ov252*.a:*(.bss .bss.*) *libkh_*_ov252*.a:(COMMON) __kh_cbss_end_ov252 = .;
		__kh_cbss_start_ov253 = .; *libkh_*_ov253*.a:*(.bss .bss.*) *libkh_*_ov253*.a:(COMMON) __kh_cbss_end_ov253 = .;
		__kh_cbss_start_ov254 = .; *libkh_*_ov254*.a:*(.bss .bss.*) *libkh_*_ov254*.a:(COMMON) __kh_cbss_end_ov254 = .;
		__kh_cbss_start_ov255 = .; *libkh_*_ov255*.a:*(.bss .bss.*) *libkh_*_ov255*.a:(COMMON) __kh_cbss_end_ov255 = .;
		__kh_cbss_start_ov256 = .; *libkh_*_ov256*.a:*(.bss .bss.*) *libkh_*_ov256*.a:(COMMON) __kh_cbss_end_ov256 = .;
		__kh_cbss_start_ov257 = .; *libkh_*_ov257*.a:*(.bss .bss.*) *libkh_*_ov257*.a:(COMMON) __kh_cbss_end_ov257 = .;
		__kh_cbss_start_ov258 = .; *libkh_*_ov258*.a:*(.bss .bss.*) *libkh_*_ov258*.a:(COMMON) __kh_cbss_end_ov258 = .;
		__kh_cbss_start_ov259 = .; *libkh_*_ov259*.a:*(.bss .bss.*) *libkh_*_ov259*.a:(COMMON) __kh_cbss_end_ov259 = .;
		__kh_cbss_start_ov260 = .; *libkh_*_ov260*.a:*(.bss .bss.*) *libkh_*_ov260*.a:(COMMON) __kh_cbss_end_ov260 = .;
		__kh_cbss_start_ov261 = .; *libkh_*_ov261*.a:*(.bss .bss.*) *libkh_*_ov261*.a:(COMMON) __kh_cbss_end_ov261 = .;
		__kh_cbss_start_ov262 = .; *libkh_*_ov262*.a:*(.bss .bss.*) *libkh_*_ov262*.a:(COMMON) __kh_cbss_end_ov262 = .;
		__kh_cbss_start_ov263 = .; *libkh_*_ov263*.a:*(.bss .bss.*) *libkh_*_ov263*.a:(COMMON) __kh_cbss_end_ov263 = .;
		__kh_cbss_start_ov264 = .; *libkh_*_ov264*.a:*(.bss .bss.*) *libkh_*_ov264*.a:(COMMON) __kh_cbss_end_ov264 = .;
		__kh_cbss_start_ov265 = .; *libkh_*_ov265*.a:*(.bss .bss.*) *libkh_*_ov265*.a:(COMMON) __kh_cbss_end_ov265 = .;
		__kh_cbss_start_ov266 = .; *libkh_*_ov266*.a:*(.bss .bss.*) *libkh_*_ov266*.a:(COMMON) __kh_cbss_end_ov266 = .;
		__kh_cbss_start_ov267 = .; *libkh_*_ov267*.a:*(.bss .bss.*) *libkh_*_ov267*.a:(COMMON) __kh_cbss_end_ov267 = .;
		__kh_cbss_start_ov268 = .; *libkh_*_ov268*.a:*(.bss .bss.*) *libkh_*_ov268*.a:(COMMON) __kh_cbss_end_ov268 = .;
		__kh_cbss_start_ov269 = .; *libkh_*_ov269*.a:*(.bss .bss.*) *libkh_*_ov269*.a:(COMMON) __kh_cbss_end_ov269 = .;
		__kh_cbss_start_ov270 = .; *libkh_*_ov270*.a:*(.bss .bss.*) *libkh_*_ov270*.a:(COMMON) __kh_cbss_end_ov270 = .;
		__kh_cbss_start_ov271 = .; *libkh_*_ov271*.a:*(.bss .bss.*) *libkh_*_ov271*.a:(COMMON) __kh_cbss_end_ov271 = .;
		__kh_cbss_start_ov272 = .; *libkh_*_ov272*.a:*(.bss .bss.*) *libkh_*_ov272*.a:(COMMON) __kh_cbss_end_ov272 = .;
		__kh_cbss_start_ov273 = .; *libkh_*_ov273*.a:*(.bss .bss.*) *libkh_*_ov273*.a:(COMMON) __kh_cbss_end_ov273 = .;
		__kh_cbss_start_ov274 = .; *libkh_*_ov274*.a:*(.bss .bss.*) *libkh_*_ov274*.a:(COMMON) __kh_cbss_end_ov274 = .;
		__kh_cbss_start_ov275 = .; *libkh_*_ov275*.a:*(.bss .bss.*) *libkh_*_ov275*.a:(COMMON) __kh_cbss_end_ov275 = .;
		__kh_cbss_start_ov276 = .; *libkh_*_ov276*.a:*(.bss .bss.*) *libkh_*_ov276*.a:(COMMON) __kh_cbss_end_ov276 = .;
		__kh_cbss_start_ov277 = .; *libkh_*_ov277*.a:*(.bss .bss.*) *libkh_*_ov277*.a:(COMMON) __kh_cbss_end_ov277 = .;
		__kh_cbss_start_ov278 = .; *libkh_*_ov278*.a:*(.bss .bss.*) *libkh_*_ov278*.a:(COMMON) __kh_cbss_end_ov278 = .;
		__kh_cbss_start_ov279 = .; *libkh_*_ov279*.a:*(.bss .bss.*) *libkh_*_ov279*.a:(COMMON) __kh_cbss_end_ov279 = .;
		__kh_cbss_start_ov280 = .; *libkh_*_ov280*.a:*(.bss .bss.*) *libkh_*_ov280*.a:(COMMON) __kh_cbss_end_ov280 = .;
		__kh_cbss_start_ov281 = .; *libkh_*_ov281*.a:*(.bss .bss.*) *libkh_*_ov281*.a:(COMMON) __kh_cbss_end_ov281 = .;
		__kh_cbss_start_ov282 = .; *libkh_*_ov282*.a:*(.bss .bss.*) *libkh_*_ov282*.a:(COMMON) __kh_cbss_end_ov282 = .;
		__kh_cbss_start_ov283 = .; *libkh_*_ov283*.a:*(.bss .bss.*) *libkh_*_ov283*.a:(COMMON) __kh_cbss_end_ov283 = .;
		__kh_cbss_start_ov284 = .; *libkh_*_ov284*.a:*(.bss .bss.*) *libkh_*_ov284*.a:(COMMON) __kh_cbss_end_ov284 = .;
		__kh_cbss_start_ov285 = .; *libkh_*_ov285*.a:*(.bss .bss.*) *libkh_*_ov285*.a:(COMMON) __kh_cbss_end_ov285 = .;
		__kh_cbss_start_ov286 = .; *libkh_*_ov286*.a:*(.bss .bss.*) *libkh_*_ov286*.a:(COMMON) __kh_cbss_end_ov286 = .;
		__kh_cbss_start_ov287 = .; *libkh_*_ov287*.a:*(.bss .bss.*) *libkh_*_ov287*.a:(COMMON) __kh_cbss_end_ov287 = .;
		__kh_cbss_start_ov288 = .; *libkh_*_ov288*.a:*(.bss .bss.*) *libkh_*_ov288*.a:(COMMON) __kh_cbss_end_ov288 = .;
		__kh_cbss_start_ov289 = .; *libkh_*_ov289*.a:*(.bss .bss.*) *libkh_*_ov289*.a:(COMMON) __kh_cbss_end_ov289 = .;
		__kh_cbss_start_ov290 = .; *libkh_*_ov290*.a:*(.bss .bss.*) *libkh_*_ov290*.a:(COMMON) __kh_cbss_end_ov290 = .;
		__kh_cbss_start_ov291 = .; *libkh_*_ov291*.a:*(.bss .bss.*) *libkh_*_ov291*.a:(COMMON) __kh_cbss_end_ov291 = .;
		__kh_cbss_start_ov292 = .; *libkh_*_ov292*.a:*(.bss .bss.*) *libkh_*_ov292*.a:(COMMON) __kh_cbss_end_ov292 = .;
		__kh_cbss_start_ov293 = .; *libkh_*_ov293*.a:*(.bss .bss.*) *libkh_*_ov293*.a:(COMMON) __kh_cbss_end_ov293 = .;
		__kh_cbss_start_ov294 = .; *libkh_*_ov294*.a:*(.bss .bss.*) *libkh_*_ov294*.a:(COMMON) __kh_cbss_end_ov294 = .;
		__kh_cbss_start_ov295 = .; *libkh_*_ov295*.a:*(.bss .bss.*) *libkh_*_ov295*.a:(COMMON) __kh_cbss_end_ov295 = .;
		__kh_cbss_start_ov296 = .; *libkh_*_ov296*.a:*(.bss .bss.*) *libkh_*_ov296*.a:(COMMON) __kh_cbss_end_ov296 = .;
		__kh_cbss_start_ov297 = .; *libkh_*_ov297*.a:*(.bss .bss.*) *libkh_*_ov297*.a:(COMMON) __kh_cbss_end_ov297 = .;
		__kh_cbss_start_ov298 = .; *libkh_*_ov298*.a:*(.bss .bss.*) *libkh_*_ov298*.a:(COMMON) __kh_cbss_end_ov298 = .;
		__kh_cbss_start_ov299 = .; *libkh_*_ov299*.a:*(.bss .bss.*) *libkh_*_ov299*.a:(COMMON) __kh_cbss_end_ov299 = .;
		__kh_cbss_start_ov300 = .; *libkh_*_ov300*.a:*(.bss .bss.*) *libkh_*_ov300*.a:(COMMON) __kh_cbss_end_ov300 = .;
		__kh_cbss_start_ov301 = .; *libkh_*_ov301*.a:*(.bss .bss.*) *libkh_*_ov301*.a:(COMMON) __kh_cbss_end_ov301 = .;
		__kh_cbss_start_ov302 = .; *libkh_*_ov302*.a:*(.bss .bss.*) *libkh_*_ov302*.a:(COMMON) __kh_cbss_end_ov302 = .;
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b*)
		*(COMMON)
	}
	_end_bss = .;

	_end = . ;
	PROVIDE(end = .);

	.spad 0x70000000: {
		*(.spad)
	}

	/* Symbols needed by crt0.s.  */
	PROVIDE(_heap_size = -1);
	PROVIDE(_stack = -1);
	PROVIDE(_stack_size = 128 * 1024);
}

/* ---- GENERATED by ps2/tools/gen_link.py ---- */
/* overlay ids: FS_OVERLAY_ID(x) is the address of this absolute symbol */
OVERLAY_0_ID = 0;
OVERLAY_1_ID = 1;
OVERLAY_2_ID = 2;
OVERLAY_3_ID = 3;
OVERLAY_4_ID = 4;
OVERLAY_5_ID = 5;
OVERLAY_6_ID = 6;
OVERLAY_7_ID = 7;
OVERLAY_8_ID = 8;
OVERLAY_9_ID = 9;
OVERLAY_10_ID = 10;
OVERLAY_11_ID = 11;
OVERLAY_12_ID = 12;
OVERLAY_13_ID = 13;
OVERLAY_14_ID = 14;
OVERLAY_15_ID = 15;
OVERLAY_16_ID = 16;
OVERLAY_17_ID = 17;
OVERLAY_18_ID = 18;
OVERLAY_19_ID = 19;
OVERLAY_20_ID = 20;
OVERLAY_21_ID = 21;
OVERLAY_22_ID = 22;
OVERLAY_23_ID = 23;
OVERLAY_24_ID = 24;
OVERLAY_25_ID = 25;
OVERLAY_26_ID = 26;
OVERLAY_27_ID = 27;
OVERLAY_28_ID = 28;
OVERLAY_29_ID = 29;
OVERLAY_30_ID = 30;
OVERLAY_31_ID = 31;
OVERLAY_32_ID = 32;
OVERLAY_33_ID = 33;
OVERLAY_34_ID = 34;
OVERLAY_35_ID = 35;
OVERLAY_36_ID = 36;
OVERLAY_37_ID = 37;
OVERLAY_38_ID = 38;
OVERLAY_39_ID = 39;
OVERLAY_40_ID = 40;
OVERLAY_41_ID = 41;
OVERLAY_42_ID = 42;
OVERLAY_43_ID = 43;
OVERLAY_44_ID = 44;
OVERLAY_45_ID = 45;
OVERLAY_46_ID = 46;
OVERLAY_47_ID = 47;
OVERLAY_48_ID = 48;
OVERLAY_49_ID = 49;
OVERLAY_50_ID = 50;
OVERLAY_51_ID = 51;
OVERLAY_52_ID = 52;
OVERLAY_53_ID = 53;
OVERLAY_54_ID = 54;
OVERLAY_55_ID = 55;
OVERLAY_56_ID = 56;
OVERLAY_57_ID = 57;
OVERLAY_58_ID = 58;
OVERLAY_59_ID = 59;
OVERLAY_60_ID = 60;
OVERLAY_61_ID = 61;
OVERLAY_62_ID = 62;
OVERLAY_63_ID = 63;
OVERLAY_64_ID = 64;
OVERLAY_65_ID = 65;
OVERLAY_66_ID = 66;
OVERLAY_67_ID = 67;
OVERLAY_68_ID = 68;
OVERLAY_69_ID = 69;
OVERLAY_70_ID = 70;
OVERLAY_71_ID = 71;
OVERLAY_72_ID = 72;
OVERLAY_73_ID = 73;
OVERLAY_74_ID = 74;
OVERLAY_75_ID = 75;
OVERLAY_76_ID = 76;
OVERLAY_77_ID = 77;
OVERLAY_78_ID = 78;
OVERLAY_79_ID = 79;
OVERLAY_80_ID = 80;
OVERLAY_81_ID = 81;
OVERLAY_82_ID = 82;
OVERLAY_83_ID = 83;
OVERLAY_84_ID = 84;
OVERLAY_85_ID = 85;
OVERLAY_86_ID = 86;
OVERLAY_87_ID = 87;
OVERLAY_88_ID = 88;
OVERLAY_89_ID = 89;
OVERLAY_90_ID = 90;
OVERLAY_91_ID = 91;
OVERLAY_92_ID = 92;
OVERLAY_93_ID = 93;
OVERLAY_94_ID = 94;
OVERLAY_95_ID = 95;
OVERLAY_96_ID = 96;
OVERLAY_97_ID = 97;
OVERLAY_98_ID = 98;
OVERLAY_99_ID = 99;
OVERLAY_100_ID = 100;
OVERLAY_101_ID = 101;
OVERLAY_102_ID = 102;
OVERLAY_103_ID = 103;
OVERLAY_104_ID = 104;
OVERLAY_105_ID = 105;
OVERLAY_106_ID = 106;
OVERLAY_107_ID = 107;
OVERLAY_108_ID = 108;
OVERLAY_109_ID = 109;
OVERLAY_110_ID = 110;
OVERLAY_111_ID = 111;
OVERLAY_112_ID = 112;
OVERLAY_113_ID = 113;
OVERLAY_114_ID = 114;
OVERLAY_115_ID = 115;
OVERLAY_116_ID = 116;
OVERLAY_117_ID = 117;
OVERLAY_118_ID = 118;
OVERLAY_119_ID = 119;
OVERLAY_120_ID = 120;
OVERLAY_121_ID = 121;
OVERLAY_122_ID = 122;
OVERLAY_123_ID = 123;
OVERLAY_124_ID = 124;
OVERLAY_125_ID = 125;
OVERLAY_126_ID = 126;
OVERLAY_127_ID = 127;
OVERLAY_128_ID = 128;
OVERLAY_129_ID = 129;
OVERLAY_130_ID = 130;
OVERLAY_131_ID = 131;
OVERLAY_132_ID = 132;
OVERLAY_133_ID = 133;
OVERLAY_134_ID = 134;
OVERLAY_135_ID = 135;
OVERLAY_136_ID = 136;
OVERLAY_137_ID = 137;
OVERLAY_138_ID = 138;
OVERLAY_139_ID = 139;
OVERLAY_140_ID = 140;
OVERLAY_141_ID = 141;
OVERLAY_142_ID = 142;
OVERLAY_143_ID = 143;
OVERLAY_144_ID = 144;
OVERLAY_145_ID = 145;
OVERLAY_146_ID = 146;
OVERLAY_147_ID = 147;
OVERLAY_148_ID = 148;
OVERLAY_149_ID = 149;
OVERLAY_150_ID = 150;
OVERLAY_151_ID = 151;
OVERLAY_152_ID = 152;
OVERLAY_153_ID = 153;
OVERLAY_154_ID = 154;
OVERLAY_155_ID = 155;
OVERLAY_156_ID = 156;
OVERLAY_157_ID = 157;
OVERLAY_158_ID = 158;
OVERLAY_159_ID = 159;
OVERLAY_160_ID = 160;
OVERLAY_161_ID = 161;
OVERLAY_162_ID = 162;
OVERLAY_163_ID = 163;
OVERLAY_164_ID = 164;
OVERLAY_165_ID = 165;
OVERLAY_166_ID = 166;
OVERLAY_167_ID = 167;
OVERLAY_168_ID = 168;
OVERLAY_169_ID = 169;
OVERLAY_170_ID = 170;
OVERLAY_171_ID = 171;
OVERLAY_172_ID = 172;
OVERLAY_173_ID = 173;
OVERLAY_174_ID = 174;
OVERLAY_175_ID = 175;
OVERLAY_176_ID = 176;
OVERLAY_177_ID = 177;
OVERLAY_178_ID = 178;
OVERLAY_179_ID = 179;
OVERLAY_180_ID = 180;
OVERLAY_181_ID = 181;
OVERLAY_182_ID = 182;
OVERLAY_183_ID = 183;
OVERLAY_184_ID = 184;
OVERLAY_185_ID = 185;
OVERLAY_186_ID = 186;
OVERLAY_187_ID = 187;
OVERLAY_188_ID = 188;
OVERLAY_189_ID = 189;
OVERLAY_190_ID = 190;
OVERLAY_191_ID = 191;
OVERLAY_192_ID = 192;
OVERLAY_193_ID = 193;
OVERLAY_194_ID = 194;
OVERLAY_195_ID = 195;
OVERLAY_196_ID = 196;
OVERLAY_197_ID = 197;
OVERLAY_198_ID = 198;
OVERLAY_199_ID = 199;
OVERLAY_200_ID = 200;
OVERLAY_201_ID = 201;
OVERLAY_202_ID = 202;
OVERLAY_203_ID = 203;
OVERLAY_204_ID = 204;
OVERLAY_205_ID = 205;
OVERLAY_206_ID = 206;
OVERLAY_207_ID = 207;
OVERLAY_208_ID = 208;
OVERLAY_209_ID = 209;
OVERLAY_210_ID = 210;
OVERLAY_211_ID = 211;
OVERLAY_212_ID = 212;
OVERLAY_213_ID = 213;
OVERLAY_214_ID = 214;
OVERLAY_215_ID = 215;
OVERLAY_216_ID = 216;
OVERLAY_217_ID = 217;
OVERLAY_218_ID = 218;
OVERLAY_219_ID = 219;
OVERLAY_220_ID = 220;
OVERLAY_221_ID = 221;
OVERLAY_222_ID = 222;
OVERLAY_223_ID = 223;
OVERLAY_224_ID = 224;
OVERLAY_225_ID = 225;
OVERLAY_226_ID = 226;
OVERLAY_227_ID = 227;
OVERLAY_228_ID = 228;
OVERLAY_229_ID = 229;
OVERLAY_230_ID = 230;
OVERLAY_231_ID = 231;
OVERLAY_232_ID = 232;
OVERLAY_233_ID = 233;
OVERLAY_234_ID = 234;
OVERLAY_235_ID = 235;
OVERLAY_236_ID = 236;
OVERLAY_237_ID = 237;
OVERLAY_238_ID = 238;
OVERLAY_239_ID = 239;
OVERLAY_240_ID = 240;
OVERLAY_241_ID = 241;
OVERLAY_242_ID = 242;
OVERLAY_243_ID = 243;
OVERLAY_244_ID = 244;
OVERLAY_245_ID = 245;
OVERLAY_246_ID = 246;
OVERLAY_247_ID = 247;
OVERLAY_248_ID = 248;
OVERLAY_249_ID = 249;
OVERLAY_250_ID = 250;
OVERLAY_251_ID = 251;
OVERLAY_252_ID = 252;
OVERLAY_253_ID = 253;
OVERLAY_254_ID = 254;
OVERLAY_255_ID = 255;
OVERLAY_256_ID = 256;
OVERLAY_257_ID = 257;
OVERLAY_258_ID = 258;
OVERLAY_259_ID = 259;
OVERLAY_260_ID = 260;
OVERLAY_261_ID = 261;
OVERLAY_262_ID = 262;
OVERLAY_263_ID = 263;
OVERLAY_264_ID = 264;
OVERLAY_265_ID = 265;
OVERLAY_266_ID = 266;
OVERLAY_267_ID = 267;
OVERLAY_268_ID = 268;
OVERLAY_269_ID = 269;
OVERLAY_270_ID = 270;
OVERLAY_271_ID = 271;
OVERLAY_272_ID = 272;
OVERLAY_273_ID = 273;
OVERLAY_274_ID = 274;
OVERLAY_275_ID = 275;
OVERLAY_276_ID = 276;
OVERLAY_277_ID = 277;
OVERLAY_278_ID = 278;
OVERLAY_279_ID = 279;
OVERLAY_280_ID = 280;
OVERLAY_281_ID = 281;
OVERLAY_282_ID = 282;
OVERLAY_283_ID = 283;
OVERLAY_284_ID = 284;
OVERLAY_285_ID = 285;
OVERLAY_286_ID = 286;
OVERLAY_287_ID = 287;
OVERLAY_288_ID = 288;
OVERLAY_289_ID = 289;
OVERLAY_290_ID = 290;
OVERLAY_291_ID = 291;
OVERLAY_292_ID = 292;
OVERLAY_293_ID = 293;
OVERLAY_294_ID = 294;
OVERLAY_295_ID = 295;
OVERLAY_296_ID = 296;
OVERLAY_297_ID = 297;
OVERLAY_298_ID = 298;
OVERLAY_299_ID = 299;
OVERLAY_300_ID = 300;
OVERLAY_301_ID = 301;
OVERLAY_302_ID = 302;
/* data labels that point inside a larger C-defined object */
PROVIDE(data_ov002_0207e9f4_default = data_ov002_0207e9f4 + 0x0);
PROVIDE(data_ov002_0207ef80_offsets = data_ov002_0207ef80 + 0x0);
