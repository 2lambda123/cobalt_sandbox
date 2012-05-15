# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'includes': [
    'ui_resources.gypi',
  ],
  'targets': [
    {
      'target_name': 'ui',
      'type': '<(component)',
      'variables': { 'enable_wexit_time_destructors': 1, },
      'includes': [
        'base/ime/ime.gypi',
      ],
      'dependencies': [
        '../base/base.gyp:base',
        '../base/base.gyp:base_i18n',
        '../base/base.gyp:base_static',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../build/temp_gyp/googleurl.gyp:googleurl',
        '../net/net.gyp:net',
        '../skia/skia.gyp:skia',
        '../third_party/icu/icu.gyp:icui18n',
        '../third_party/icu/icu.gyp:icuuc',
        '../third_party/libpng/libpng.gyp:libpng',
        '../third_party/zlib/zlib.gyp:zlib',
        'base/strings/ui_strings.gyp:ui_strings',
        'gfx_resources',
        '<(libjpeg_gyp_path):libjpeg',
      ],
      'defines': [
        'UI_IMPLEMENTATION',
      ],
      # Export these dependencies since text_elider.h includes ICU headers.
      'export_dependent_settings': [
        '../net/net.gyp:net',
        '../third_party/icu/icu.gyp:icui18n',
        '../third_party/icu/icu.gyp:icuuc',
      ],
      'sources': [
        'base/accelerators/accelerator.cc',
        'base/accelerators/accelerator.h',
        'base/accelerators/accelerator_cocoa.h',
        'base/accelerators/accelerator_cocoa.mm',
        'base/accelerators/accelerator_gtk.cc',
        'base/accelerators/accelerator_gtk.h',
        'base/accelerators/accelerator_manager.cc',
        'base/accelerators/accelerator_manager.h',
        'base/accessibility/accessibility_types.h',
        'base/accessibility/accessible_text_utils.cc',
        'base/accessibility/accessible_text_utils.h',
        'base/accessibility/accessible_view_state.cc',
        'base/accessibility/accessible_view_state.h',
        'base/animation/animation.cc',
        'base/animation/animation.h',
        'base/animation/animation_container.cc',
        'base/animation/animation_container.h',
        'base/animation/animation_container_element.h',
        'base/animation/animation_container_observer.h',
        'base/animation/animation_delegate.h',
        'base/animation/linear_animation.cc',
        'base/animation/linear_animation.h',
        'base/animation/multi_animation.cc',
        'base/animation/multi_animation.h',
        'base/animation/slide_animation.cc',
        'base/animation/slide_animation.h',
        'base/animation/throb_animation.cc',
        'base/animation/throb_animation.h',
        'base/animation/tween.cc',
        'base/animation/tween.h',
        'base/clipboard/clipboard.cc',
        'base/clipboard/clipboard.h',
        'base/clipboard/clipboard_android.cc',
        'base/clipboard/clipboard_aurax11.cc',
        'base/clipboard/clipboard_gtk.cc',
        'base/clipboard/clipboard_mac.mm',
        'base/clipboard/clipboard_util_win.cc',
        'base/clipboard/clipboard_util_win.h',
        'base/clipboard/clipboard_win.cc',
        'base/clipboard/custom_data_helper.cc',
        'base/clipboard/custom_data_helper.h',
        'base/clipboard/custom_data_helper_mac.mm',
        'base/clipboard/custom_data_helper_x.cc',
        'base/clipboard/scoped_clipboard_writer.cc',
        'base/clipboard/scoped_clipboard_writer.h',
        'base/cocoa/base_view.h',
        'base/cocoa/base_view.mm',
        'base/cocoa/events_mac.mm',
        'base/cocoa/find_pasteboard.h',
        'base/cocoa/find_pasteboard.mm',
        'base/cocoa/focus_tracker.h',
        'base/cocoa/focus_tracker.mm',
        'base/cocoa/fullscreen_window_manager.h',
        'base/cocoa/fullscreen_window_manager.mm',
        'base/cocoa/nib_loading.h',
        'base/cocoa/nib_loading.mm',
        'base/cocoa/underlay_opengl_hosting_window.h',
        'base/cocoa/underlay_opengl_hosting_window.mm',
        'base/cocoa/window_size_constants.h',
        'base/cocoa/window_size_constants.mm',
        'base/cursor/cursor.cc',
        'base/cursor/cursor.h',
        'base/cursor/cursor_win.cc',
        'base/cursor/cursor_x11.cc',
        'base/dragdrop/cocoa_dnd_util.h',
        'base/dragdrop/cocoa_dnd_util.mm',
        'base/dragdrop/drag_drop_types_gtk.cc',
        'base/dragdrop/drag_drop_types.h',
        'base/dragdrop/drag_drop_types_win.cc',
        'base/dragdrop/drag_source.cc',
        'base/dragdrop/drag_source.h',
        'base/dragdrop/drop_target.cc',
        'base/dragdrop/drop_target.h',
        'base/dragdrop/drag_utils.cc',
        'base/dragdrop/drag_utils.h',
        'base/dragdrop/drag_utils_aura.cc',
        'base/dragdrop/drag_utils_gtk.cc',
        'base/dragdrop/drag_utils_win.cc',
        'base/dragdrop/gtk_dnd_util.cc',
        'base/dragdrop/gtk_dnd_util.h',
        'base/dragdrop/os_exchange_data.cc',
        'base/dragdrop/os_exchange_data.h',
        'base/dragdrop/os_exchange_data_provider_aura.cc',
        'base/dragdrop/os_exchange_data_provider_aura.h',
        'base/dragdrop/os_exchange_data_provider_gtk.cc',
        'base/dragdrop/os_exchange_data_provider_gtk.h',
        'base/dragdrop/os_exchange_data_provider_win.cc',
        'base/dragdrop/os_exchange_data_provider_win.h',
        'base/events.h',
        'base/gestures/gesture_configuration.cc',
        'base/gestures/gesture_configuration.h',
        'base/gestures/gesture_point.cc',
        'base/gestures/gesture_point.h',
        'base/gestures/gesture_recognizer.h',
        'base/gestures/gesture_recognizer_impl.cc',
        'base/gestures/gesture_recognizer_impl.h',
        'base/gestures/gesture_sequence.cc',
        'base/gestures/gesture_sequence.h',
        'base/gestures/velocity_calculator.cc',
        'base/gestures/velocity_calculator.h',
        'base/gtk/event_synthesis_gtk.cc',
        'base/gtk/event_synthesis_gtk.h',
        'base/gtk/focus_store_gtk.cc',
        'base/gtk/focus_store_gtk.h',
        'base/gtk/g_object_destructor_filo.cc',
        'base/gtk/g_object_destructor_filo.h',
        'base/gtk/gtk_expanded_container.cc',
        'base/gtk/gtk_expanded_container.h',
        'base/gtk/gtk_floating_container.cc',
        'base/gtk/gtk_floating_container.h',
        'base/gtk/gtk_im_context_util.cc',
        'base/gtk/gtk_im_context_util.h',
        'base/gtk/gtk_hig_constants.h',
        'base/gtk/gtk_screen_util.cc',
        'base/gtk/gtk_screen_util.h',
        'base/gtk/gtk_signal.h',
        'base/gtk/gtk_signal_registrar.cc',
        'base/gtk/gtk_signal_registrar.h',
        'base/gtk/gtk_windowing.cc',
        'base/gtk/gtk_windowing.h',
        'base/gtk/menu_label_accelerator_util.cc',
        'base/gtk/menu_label_accelerator_util.h',
        'base/gtk/owned_widget_gtk.cc',
        'base/gtk/owned_widget_gtk.h',
        'base/gtk/scoped_gobject.h',
        'base/gtk/scoped_region.cc',
        'base/gtk/scoped_region.h',
        'base/hit_test.h',
        'base/javascript_message_type.h',
        'base/keycodes/keyboard_code_conversion.cc',
        'base/keycodes/keyboard_code_conversion.h',
        'base/keycodes/keyboard_code_conversion_gtk.cc',
        'base/keycodes/keyboard_code_conversion_gtk.h',
        'base/keycodes/keyboard_code_conversion_mac.h',
        'base/keycodes/keyboard_code_conversion_mac.mm',
        'base/keycodes/keyboard_code_conversion_win.cc',
        'base/keycodes/keyboard_code_conversion_win.h',
        'base/keycodes/keyboard_code_conversion_x.cc',
        'base/keycodes/keyboard_code_conversion_x.h',
        'base/keycodes/keyboard_codes.h',
        'base/l10n/l10n_font_util.cc',
        'base/l10n/l10n_font_util.h',
        'base/l10n/l10n_util.cc',
        'base/l10n/l10n_util.h',
        'base/l10n/l10n_util_collator.h',
        'base/l10n/l10n_util_mac.h',
        'base/l10n/l10n_util_mac.mm',
        'base/l10n/l10n_util_posix.cc',
        'base/l10n/l10n_util_win.cc',
        'base/l10n/l10n_util_win.h',
        'base/layout.cc',
        'base/layout.h',
        'base/models/button_menu_item_model.cc',
        'base/models/button_menu_item_model.h',
        'base/models/combobox_model.h',
        'base/models/list_model.h',
        'base/models/list_model_observer.h',
        'base/models/menu_model.cc',
        'base/models/menu_model.h',
        'base/models/menu_model_delegate.h',
        'base/models/simple_menu_model.cc',
        'base/models/simple_menu_model.h',
        'base/models/table_model.cc',
        'base/models/table_model.h',
        'base/models/table_model_observer.h',
        'base/models/tree_model.cc',
        'base/models/tree_model.h',
        'base/models/tree_node_iterator.h',
        'base/models/tree_node_model.h',
        'base/native_theme/native_theme.cc',
        'base/native_theme/native_theme.h',
        'base/native_theme/native_theme_android.cc',
        'base/native_theme/native_theme_android.h',
        'base/native_theme/native_theme_aura.cc',
        'base/native_theme/native_theme_aura.h',
        'base/native_theme/native_theme_base.cc',
        'base/native_theme/native_theme_base.h',
        'base/native_theme/native_theme_gtk.cc',
        'base/native_theme/native_theme_gtk.h',
        'base/native_theme/native_theme_win.cc',
        'base/native_theme/native_theme_win.h',
        'base/range/range.cc',
        'base/range/range.h',
        'base/range/range_mac.mm',
        'base/range/range_win.cc',
        'base/resource/data_pack.cc',
        'base/resource/data_pack.h',
        'base/resource/resource_bundle.cc',
        'base/resource/resource_bundle.h',
        'base/resource/resource_bundle_android.cc',
        'base/resource/resource_bundle_aurax11.cc',
        'base/resource/resource_bundle_gtk.cc',
        'base/resource/resource_bundle_mac.mm',
        'base/resource/resource_bundle_win.cc',
        'base/resource/resource_bundle_win.h',
        'base/resource/resource_data_dll_win.cc',
        'base/resource/resource_data_dll_win.h',
        'base/resource/resource_handle.cc',
        'base/resource/resource_handle.h',
        'base/text/bytes_formatting.cc',
        'base/text/bytes_formatting.h',
        'base/text/text_elider.cc',
        'base/text/text_elider.h',
        'base/text/utf16_indexing.cc',
        'base/text/utf16_indexing.h',
        'base/theme_provider.cc',
        'base/theme_provider.h',
        'base/touch/touch_factory.cc',
        'base/touch/touch_factory.h',
        'base/ui_base_exports.cc',
        'base/ui_base_paths.cc',
        'base/ui_base_paths.h',
        'base/ui_base_switches.cc',
        'base/ui_base_switches.h',
        'base/ui_base_types.h',
        'base/ui_export.h',
        'base/view_prop.cc',
        'base/view_prop.h',
        'base/win/atl_module.h',
        'base/win/dpi.cc',
        'base/win/dpi.h',
        'base/win/events_win.cc',
        'base/win/foreground_helper.cc',
        'base/win/foreground_helper.h',
        'base/win/hwnd_util.cc',
        'base/win/hwnd_util.h',
        'base/win/hwnd_subclass.cc',
        'base/win/hwnd_subclass.h',
        'base/win/ime_input.cc',
        'base/win/ime_input.h',
        'base/win/message_box_win.cc',
        'base/win/message_box_win.h',
        'base/win/mouse_wheel_util.cc',
        'base/win/mouse_wheel_util.h',
        'base/win/scoped_ole_initializer.cc',
        'base/win/scoped_ole_initializer.h',
        'base/win/scoped_set_map_mode.h',
        'base/win/shell.cc',
        'base/win/shell.h',
        'base/win/singleton_hwnd.cc',
        'base/win/singleton_hwnd.h',
        'base/win/window_impl.cc',
        'base/win/window_impl.h',
        'base/work_area_watcher_observer.h',
        'base/x/active_window_watcher_x.cc',
        'base/x/active_window_watcher_x.h',
        'base/x/active_window_watcher_x_observer.h',
        'base/x/events_x.cc',
        'base/x/root_window_property_watcher_x.cc',
        'base/x/root_window_property_watcher_x.h',
        'base/x/work_area_watcher_x.cc',
        'base/x/work_area_watcher_x.h',
        'base/x/x11_util.cc',
        'base/x/x11_util.h',
        'base/x/x11_util_internal.h',
        'gfx/blit.cc',
        'gfx/blit.h',
        'gfx/canvas.cc',
        'gfx/canvas.h',
        'gfx/canvas_android.cc',
        'gfx/canvas_linux.cc',
        'gfx/canvas_mac.mm',
        'gfx/canvas_paint.h',
        'gfx/canvas_paint_win.cc',
        'gfx/canvas_skia.cc',
        'gfx/canvas_skia_paint.h',
        'gfx/canvas_win.cc',
        'gfx/codec/jpeg_codec.cc',
        'gfx/codec/jpeg_codec.h',
        'gfx/codec/png_codec.cc',
        'gfx/codec/png_codec.h',
        'gfx/color_analysis.cc',
        'gfx/color_analysis.h',
        'gfx/color_utils.cc',
        'gfx/color_utils.h',
        'gfx/favicon_size.cc',
        'gfx/favicon_size.h',
        'gfx/font.h',
        'gfx/font.cc',
        'gfx/font_list.h',
        'gfx/font_list.cc',
        'gfx/font_smoothing_win.cc',
        'gfx/font_smoothing_win.h',
        'gfx/gfx_paths.cc',
        'gfx/gfx_paths.h',
        'gfx/image/image.cc',
        'gfx/image/image.h',
        'gfx/image/image_mac.mm',
        'gfx/image/image_skia.cc',
        'gfx/image/image_skia.h',
        'gfx/image/image_util.cc',
        'gfx/image/image_util.h',
        'gfx/insets.cc',
        'gfx/insets.h',
        'gfx/interpolated_transform.h',
        'gfx/interpolated_transform.cc',
        'gfx/mac/nsimage_cache.h',
        'gfx/mac/nsimage_cache.mm',
        'gfx/mac/scoped_ns_disable_screen_updates.h',
        'gfx/monitor.cc',
        'gfx/monitor.h',
        'gfx/native_widget_types.h',
        'gfx/pango_util.h',
        'gfx/pango_util.cc',
        'gfx/path.cc',
        'gfx/path.h',
        'gfx/path_aura.cc',
        'gfx/path_gtk.cc',
        'gfx/path_win.cc',
        'gfx/platform_font.h',
        'gfx/platform_font_android.cc',
        'gfx/platform_font_pango.h',
        'gfx/platform_font_pango.cc',
        'gfx/platform_font_mac.h',
        'gfx/platform_font_mac.mm',
        'gfx/platform_font_win.h',
        'gfx/platform_font_win.cc',
        'gfx/point.cc',
        'gfx/point.h',
        'gfx/point_base.h',
        'gfx/rect.cc',
        'gfx/rect.h',
        'gfx/rect_base.h',
        'gfx/rect_base_impl.h',
        'gfx/render_text.cc',
        'gfx/render_text.h',
        'gfx/render_text_linux.cc',
        'gfx/render_text_linux.h',
        'gfx/render_text_win.cc',
        'gfx/render_text_win.h',
        'gfx/screen.h',
        'gfx/screen_android.cc',
        'gfx/screen_aura.cc',
        'gfx/screen_gtk.cc',
        'gfx/screen_impl.h',
        'gfx/screen_mac.mm',
        'gfx/screen_win.cc',
        'gfx/scoped_cg_context_save_gstate_mac.h',
        'gfx/scoped_ns_graphics_context_save_gstate_mac.h',
        'gfx/scoped_ns_graphics_context_save_gstate_mac.mm',
        'gfx/scrollbar_size.cc',
        'gfx/scrollbar_size.h',
        'gfx/selection_model.cc',
        'gfx/selection_model.h',
        'gfx/shadow_value.cc',
        'gfx/shadow_value.h',
        'gfx/size.cc',
        'gfx/size.h',
        'gfx/size_base.h',
        'gfx/size_base_impl.h',
        'gfx/skbitmap_operations.cc',
        'gfx/skbitmap_operations.h',
        'gfx/skia_util.cc',
        'gfx/skia_util.h',
        'gfx/skia_utils_gtk.cc',
        'gfx/skia_utils_gtk.h',
        'gfx/sys_color_change_listener.cc',
        'gfx/sys_color_change_listener.h',
        'gfx/transform.cc',
        'gfx/transform.h',
        'gfx/transform_util.cc',
        'gfx/transform_util.h',
        'gfx/video_decode_acceleration_support_mac.h',
        'gfx/video_decode_acceleration_support_mac.mm',
        'ui_controls/ui_controls.h',
        'ui_controls/ui_controls_aura.cc',
        'ui_controls/ui_controls_internal_win.h',
        'ui_controls/ui_controls_internal_win.cc',
        'ui_controls/ui_controls_gtk.cc',
        'ui_controls/ui_controls_mac.mm',
        'ui_controls/ui_controls_win.cc',
      ],
      'conditions': [
        # TODO(asvitkine): Switch all platforms to use canvas_skia.cc.
        #                  http://crbug.com/105550
        ['use_canvas_skia==1', {
          'sources!': [
            'gfx/canvas_android.cc',
            'gfx/canvas_linux.cc',
            'gfx/canvas_mac.mm',
            'gfx/canvas_win.cc',
          ],
        }, {  # use_canvas_skia!=1
          'sources!': [
            'gfx/canvas_skia.cc',
          ],
        }],
        ['use_aura==1', {
          'sources/': [
            ['exclude', 'gfx/gtk_'],
            ['exclude', 'gfx/gtk_util.cc'],
            ['exclude', 'gfx/gtk_util.h'],
            ['exclude', 'gfx/screen_gtk.cc'],
            ['exclude', 'gfx/screen_win.cc'],
            ['exclude', 'base/dragdrop/drag_utils_win.cc'],
            ['exclude', 'base/win/mouse_wheel_util.cc'],
            ['exclude', 'base/win/mouse_wheel_util.h'],
            ['exclude', 'base/work_area_watcher_observer.h'],
            ['exclude', 'base/x/active_window_watcher_x.cc'],
            ['exclude', 'base/x/active_window_watcher_x.h'],
            ['exclude', 'base/x/active_window_watcher_x_observer.h'],
            ['exclude', 'base/x/root_window_property_watcher_x.cc'],
            ['exclude', 'base/x/root_window_property_watcher_x.h'],
            ['exclude', 'base/x/work_area_watcher_x.cc'],
            ['exclude', 'base/x/work_area_watcher_x.h'],
            ['exclude', 'ui_controls_win.cc'],
           ],
        }, {  # use_aura!=1
          'sources!': [
            'base/cursor/cursor.cc',
            'base/cursor/cursor.h',
            'base/cursor/cursor_win.cc',
            'base/cursor/cursor_x11.cc',
            'base/native_theme/native_theme_aura.cc',
            'base/native_theme/native_theme_aura.h',
          ]
        }],
        ['use_aura==1 and OS=="win"', {
          'sources/': [
            ['exclude', 'base/dragdrop/os_exchange_data_provider_win.cc'],
            ['exclude', 'base/dragdrop/os_exchange_data_provider_win.h'],
            ['exclude', 'base/native_theme/native_theme_win.cc'],
            ['exclude', 'base/native_theme/native_theme_win.h'],
            ['exclude', 'gfx/path_win.cc'],
          ],
        }],
        ['use_aura==0 and toolkit_views==0', {
          'sources/': [
            ['exclude', '^base/gestures/*'],
          ]
        }],
        ['use_ibus==1', {
          'dependencies': [
            '../build/linux/system.gyp:ibus',
          ],
        }],
        ['use_glib == 1', {
          'dependencies': [
            # font_gtk.cc uses fontconfig.
            '../build/linux/system.gyp:fontconfig',
            '../build/linux/system.gyp:glib',
            '../build/linux/system.gyp:pangocairo',
            '../build/linux/system.gyp:x11',
            '../build/linux/system.gyp:xext',
            '../build/linux/system.gyp:xfixes',
          ],
          'link_settings': {
            'libraries': [
              '-lXcursor',  # For XCursor* function calls in x11_util.cc. 
              '-lXrender',  # For XRender* function calls in x11_util.cc.
            ],
          },
          'conditions': [
            ['toolkit_views==0', {
              # Note: because of gyp predence rules this has to be defined as
              # 'sources/' rather than 'sources!'.
              'sources/': [
                ['exclude', '^base/dragdrop/drag_drop_types_gtk.cc'],
                ['exclude', '^base/dragdrop/drag_utils_gtk.cc'],
                ['exclude', '^base/dragdrop/drag_utils.cc'],
                ['exclude', '^base/dragdrop/drag_utils.h'],
                ['exclude', '^base/dragdrop/os_exchange_data.cc'],
                ['exclude', '^base/dragdrop/os_exchange_data.h'],
                ['exclude', '^base/dragdrop/os_exchange_data_provider_gtk.cc'],
                ['exclude', '^base/dragdrop/os_exchange_data_provider_gtk.h'],
              ],
            }, {
              # Note: because of gyp predence rules this has to be defined as
              # 'sources/' rather than 'sources!'.
              'sources/': [
                ['include', '^base/dragdrop/os_exchange_data.cc'],
              ],
            }],
          ],
        }],
        ['toolkit_uses_gtk == 1', {
          'dependencies': [
            '../build/linux/system.gyp:gtk',
          ],
          'sources': [
            'gfx/gtk_native_view_id_manager.cc',
            'gfx/gtk_native_view_id_manager.h',
            'gfx/gtk_preserve_window.cc',
            'gfx/gtk_preserve_window.h',
            'gfx/gtk_util.cc',
            'gfx/gtk_util.h',
            'gfx/image/cairo_cached_surface.cc',
            'gfx/image/cairo_cached_surface.h',
          ],
        }, {  # toolkit_uses_gtk != 1
          'sources!': [
            'base/native_theme/native_theme_gtk.cc',
            'base/native_theme/native_theme_gtk.h',
          ]
        }],
        ['OS=="win"', {
          'sources': [
            'gfx/gdi_util.cc',
            'gfx/gdi_util.h',
            'gfx/icon_util.cc',
            'gfx/icon_util.h',
            'base/native_theme/native_theme_win.cc',
            'base/native_theme/native_theme_win.h',
          ],
          'sources!': [
            'base/touch/touch_factory.cc',
            'base/touch/touch_factory.h',
            'gfx/pango_util.h',
            'gfx/pango_util.cc',
            'gfx/platform_font_pango.cc',
            'gfx/platform_font_pango.h',
          ],
          'include_dirs': [
            '../',
            '../third_party/wtl/include',
          ],
          'msvs_settings': {
            'VCLinkerTool': {
              'DelayLoadDLLs': [
                'd2d1.dll',
                'd3d10_1.dll',
              ],
              'AdditionalDependencies': [
                'd2d1.lib',
                'd3d10_1.lib',
              ],
            },
          },
          'link_settings': {
            'libraries': [
              '-limm32.lib',
              '-ld2d1.lib',
              '-loleacc.lib',
            ],
          },
        },{  # OS!="win"
          'conditions': [
            ['use_aura==0', {
              'sources!': [
                'base/view_prop.cc',
                'base/view_prop.h',
              ],
            }],
          ],
          'sources!': [
            'base/dragdrop/drag_source.cc',
            'base/dragdrop/drag_source.h',
            'base/dragdrop/drag_drop_types.h',
            'base/dragdrop/drop_target.cc',
            'base/dragdrop/drop_target.h',
            'base/dragdrop/os_exchange_data.cc',
            'base/native_theme/native_theme_win.cc',
            'base/native_theme/native_theme_win.h',
          ],
          'sources/': [
            ['exclude', '^base/win/*'],
          ],
        }],
        ['OS=="mac"', {
          'sources!': [
            'base/dragdrop/drag_utils.cc',
            'base/dragdrop/drag_utils.h',
            'base/touch/touch_factory.cc',
            'base/touch/touch_factory.h',
            'gfx/pango_util.h',
            'gfx/pango_util.cc',
            'gfx/platform_font_pango.h',
            'gfx/platform_font_pango.cc',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/Accelerate.framework',
              '$(SDKROOT)/System/Library/Frameworks/AudioUnit.framework',
              '$(SDKROOT)/System/Library/Frameworks/CoreVideo.framework',
            ],
          },
        }],
        ['OS=="android"', {
          'sources!': [
            'gfx/pango_util.h',
            'gfx/pango_util.cc',
            'gfx/platform_font_pango.h',
            'gfx/platform_font_pango.cc',
          ],
        }],
        ['use_x11==1', {
          'all_dependent_settings': {
            'ldflags': [
              '-L<(PRODUCT_DIR)',
            ],
            'link_settings': {
              'libraries': [
                '-lX11 -lXcursor',
              ],
            },
          },
        }, {  # use_x11==0
          'sources!': [
            'base/keycodes/keyboard_code_conversion_x.cc',
            'base/keycodes/keyboard_code_conversion_x.h',
            'base/x/',
          ],
        }],
        ['toolkit_views==0', {
          'sources!': [
            'base/x/events_x.cc',
          ],
        }],
        ['toolkit_views==0 and use_canvas_skia==0', {
          'sources!': [
            'gfx/render_text.cc',
            'gfx/render_text.h',
            'gfx/render_text_linux.cc',
            'gfx/render_text_linux.h',
            'gfx/render_text_win.cc',
            'gfx/render_text_win.h',
          ],
        }],
        ['OS=="android"', {
          'sources!': [
            'base/touch/touch_factory.cc',
            'base/touch/touch_factory.h',
            'gfx/pango_util.h',
            'gfx/pango_util.cc',
            'gfx/platform_font_pango.cc',
            'gfx/platform_font_pango.h',
          ],
        }],
        ['OS=="linux"', {
          'libraries': [
            '-ldl',
          ],
        }],
        ['os_bsd==1 and use_system_libjpeg==1', {
          'include_dirs': [
            '/usr/local/include',
          ],
        }],
      ],
    },
    {
      'target_name': 'gfx_resources',
      'type': 'none',
      'variables': {
        'grit_out_dir': '<(SHARED_INTERMEDIATE_DIR)/ui/gfx',
      },
      'actions': [
        {
          'action_name': 'gfx_resources',
          'variables': {
            'grit_grd_file': 'gfx/gfx_resources.grd',
          },
          'includes': [ '../build/grit_action.gypi' ],
        },
      ],
      'includes': [ '../build/grit_target.gypi' ],
    },
  ],
  'conditions': [
    ['inside_chromium_build==1', {
      'includes': [
        'ui_unittests.gypi',
      ],
      'targets': [
        {
          # TODO(rsesek): Remove this target once ui_unittests is run on the
          # waterfall instead of gfx_unittests.
          'target_name': 'gfx_unittests',
          'type': 'none',
          'dependencies': [
            'ui_unittests',
          ],
          'actions': [
            {
              'message': 'TEMPORARY: Copying ui_unittests to gfx_unittests',
              'variables': {
                'ui_copy_target': '<(PRODUCT_DIR)/ui_unittests<(EXECUTABLE_SUFFIX)',
                'ui_copy_dest': '<(PRODUCT_DIR)/gfx_unittests<(EXECUTABLE_SUFFIX)',
              },
              'inputs': ['<(ui_copy_target)'],
              'outputs': ['<(ui_copy_dest)'],
              'action_name': 'TEMP_copy_ui_unittests',
              'action': [
                'python', '-c',
                'import os, shutil; ' \
                'shutil.copyfile(\'<(ui_copy_target)\', \'<(ui_copy_dest)\'); ' \
                'os.chmod(\'<(ui_copy_dest)\', 0700)'
              ]
            }
          ],
        },
      ],
    }],
  ],
}
