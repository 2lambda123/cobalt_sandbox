# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'base',
      'type': '<(library)',
      'msvs_guid': '1832A374-8A74-4F9E-B536-69A699B3E165',
      'sources': [
        '../build/build_config.h',
        'crypto/cssm_init.cc',
        'crypto/cssm_init.h',
        'crypto/rsa_private_key.h',
        'crypto/rsa_private_key.cc',
        'crypto/rsa_private_key_mac.cc',
        'crypto/rsa_private_key_nss.cc',
        'crypto/rsa_private_key_win.cc',
        'crypto/signature_creator.h',
        'crypto/signature_creator_mac.cc',
        'crypto/signature_creator_nss.cc',
        'crypto/signature_creator_win.cc',
        'crypto/signature_verifier.h',
        'crypto/signature_verifier_mac.cc',
        'crypto/signature_verifier_nss.cc',
        'crypto/signature_verifier_win.cc',
        'third_party/dmg_fp/dmg_fp.h',
        'third_party/dmg_fp/dtoa.cc',
        'third_party/dmg_fp/g_fmt.cc',
        'third_party/icu/icu_utf.cc',
        'third_party/icu/icu_utf.h',
        'third_party/nspr/prcpucfg.h',
        'third_party/nspr/prcpucfg_win.h',
        'third_party/nspr/prtime.cc',
        'third_party/nspr/prtime.h',
        'third_party/nspr/prtypes.h',
        'third_party/nss/blapi.h',
        'third_party/nss/blapit.h',
        'third_party/nss/sha256.h',
        'third_party/nss/sha512.cc',
        'third_party/purify/pure.h',
        'third_party/purify/pure_api.c',
        'third_party/xdg_mime/xdgmime.c',
        'third_party/xdg_mime/xdgmime.h',
        'third_party/xdg_mime/xdgmimealias.c',
        'third_party/xdg_mime/xdgmimealias.h',
        'third_party/xdg_mime/xdgmimecache.c',
        'third_party/xdg_mime/xdgmimecache.h',
        'third_party/xdg_mime/xdgmimeglob.c',
        'third_party/xdg_mime/xdgmimeglob.h',
        'third_party/xdg_mime/xdgmimeicon.c',
        'third_party/xdg_mime/xdgmimeicon.h',
        'third_party/xdg_mime/xdgmimeint.c',
        'third_party/xdg_mime/xdgmimeint.h',
        'third_party/xdg_mime/xdgmimemagic.c',
        'third_party/xdg_mime/xdgmimemagic.h',
        'third_party/xdg_mime/xdgmimeparent.c',
        'third_party/xdg_mime/xdgmimeparent.h',
        'atomicops_internals_x86_gcc.cc',
        'at_exit.cc',
        'at_exit.h',
        'atomic_flag.h',
        'atomic_flag.cc',
        'atomic_ref_count.h',
        'atomic_sequence_num.h',
        'atomicops.h',
        'atomicops_internals_x86_msvc.h',
        'base_drag_source.cc',
        'base_drag_source.h',
        'base_drop_target.cc',
        'base_drop_target.h',
        'base_paths.cc',
        'base_paths.h',
        'base_paths_linux.h',
        'base_paths_linux.cc',
        'base_paths_mac.h',
        'base_paths_mac.mm',
        'base_paths_win.cc',
        'base_paths_win.h',
        'base_switches.cc',
        'base_switches.h',
        'basictypes.h',
        'bzip2_error_handler.cc',
        'chrome_application_mac.h',
        'chrome_application_mac.mm',
        'cocoa_protocols_mac.h',
        'command_line.cc',
        'command_line.h',
        'compiler_specific.h',
        'condition_variable.h',
        'condition_variable_posix.cc',
        'condition_variable_win.cc',
        'cpu.cc',
        'cpu.h',
        'data_pack.cc',
        'debug_on_start.cc',
        'debug_on_start.h',
        'debug_util.cc',
        'debug_util.h',
        'debug_util_mac.cc',
        'debug_util_posix.cc',
        'debug_util_win.cc',
        'directory_watcher.h',
        'directory_watcher_inotify.cc',
        'directory_watcher_mac.cc',
        'directory_watcher_win.cc',
        'dynamic_annotations.h',
        'dynamic_annotations.cc',
        'event_recorder.cc',
        'event_recorder.h',
        'event_recorder_stubs.cc',
        'field_trial.cc',
        'field_trial.h',
        'file_descriptor_shuffle.cc',
        'file_descriptor_shuffle.h',
        'file_path.cc',
        'file_path.h',
        'file_util.cc',
        'file_util.h',
        'file_util_linux.cc',
        'file_util_mac.mm',
        'file_util_posix.cc',
        'file_util_win.cc',
        'file_version_info.cc',
        'file_version_info.h',
        'file_version_info_mac.mm',
        'fix_wp64.h',
        'float_util.h',
        'foundation_utils_mac.h',
        'gfx/point.cc',
        'gfx/point.h',
        'gfx/rect.cc',
        'gfx/rect.h',
        'gfx/size.cc',
        'gfx/size.h',
        'global_descriptors_posix.h',
        'global_descriptors_posix.cc',
        'hash_tables.h',
        'histogram.cc',
        'histogram.h',
        'hmac.h',
        'hmac_mac.cc',
        'hmac_nss.cc',
        'hmac_win.cc',
        'iat_patch.cc',
        'iat_patch.h',
        'id_map.h',
        'image_util.cc',
        'image_util.h',
        'json/json_reader.cc',
        'json/json_reader.h',
        'json/json_writer.cc',
        'json/json_writer.h',
        'json/string_escape.cc',
        'json/string_escape.h',
        'keyboard_code_conversion_gtk.cc',
        'keyboard_code_conversion_gtk.h',
        'keyboard_codes.h',
        'keyboard_codes_win.h',
        'keyboard_codes_posix.h',
        'lazy_instance.cc',
        'lazy_instance.h',
        'leak_annotations.h',
        'leak_tracker.h',
        'linked_list.h',
        'linked_ptr.h',
        'linux_util.cc',
        'linux_util.h',
        'lock.cc',
        'lock.h',
        'lock_impl.h',
        'lock_impl_posix.cc',
        'lock_impl_win.cc',
        'logging.cc',
        'logging.h',
        'mac_util.h',
        'mac_util.mm',
        'md5.cc',
        'md5.h',
        'memory_debug.cc',
        'memory_debug.h',
        'message_loop.cc',
        'message_loop.h',
        'message_pump.h',
        'message_pump_default.cc',
        'message_pump_default.h',
        'message_pump_glib.cc',
        'message_pump_glib.h',
        'message_pump_libevent.cc',
        'message_pump_libevent.h',
        'message_pump_mac.h',
        'message_pump_mac.mm',
        'message_pump_win.cc',
        'message_pump_win.h',
        'mime_util.h',
        'mime_util_linux.cc',
        'native_library.h',
        'native_library_linux.cc',
        'native_library_mac.mm',
        'native_library_win.cc',
        'non_thread_safe.cc',
        'non_thread_safe.h',
        'nsimage_cache_mac.h',
        'nsimage_cache_mac.mm',
        'nss_init.cc',
        'nss_init.h',
        'nullable_string16.h',
        'object_watcher.cc',
        'object_watcher.h',
        'observer_list.h',
        'observer_list_threadsafe.h',
        'path_service.cc',
        'path_service.h',
        'pe_image.cc',
        'pe_image.h',
        'pickle.cc',
        'pickle.h',
        'platform_file.h',
        'platform_file_win.cc',
        'platform_file_posix.cc',
        'platform_thread.h',
        'platform_thread_mac.mm',
        'platform_thread_posix.cc',
        'platform_thread_win.cc',
        'port.h',
        'profiler.cc',
        'profiler.h',
        'process.h',
        'process_linux.cc',
        'process_posix.cc',
        'process_util.h',
        'process_util_linux.cc',
        'process_util_mac.mm',
        'process_util_posix.cc',
        'process_util_win.cc',
        'process_win.cc',
        'rand_util.cc',
        'rand_util.h',
        'rand_util_posix.cc',
        'rand_util_win.cc',
        'ref_counted.cc',
        'ref_counted.h',
        'ref_counted_memory.h',
        'registry.cc',
        'registry.h',
        'resource_util.cc',
        'resource_util.h',
        'safe_strerror_posix.cc',
        'safe_strerror_posix.h',
        'scoped_bool.h',
        'scoped_bstr_win.cc',
        'scoped_bstr_win.h',
        'scoped_cftyperef.h',
        'scoped_comptr_win.h',
        'scoped_handle.h',
        'scoped_handle_gtk.h',
        'scoped_handle_win.h',
        'scoped_nsautorelease_pool.h',
        'scoped_nsautorelease_pool.mm',
        'scoped_nsdisable_screen_updates.h',
        'scoped_nsobject.h',
        'scoped_ptr.h',
        'scoped_temp_dir.cc',
        'scoped_temp_dir.h',
        'scoped_variant_win.cc',
        'scoped_variant_win.h',
        'scoped_vector.h',
        'setproctitle_linux.c',
        'setproctitle_linux.h',
        'sha1.cc',
        'sha1.h',
        'sha2.cc',
        'sha2.h',
        'shared_memory.h',
        'shared_memory_posix.cc',
        'shared_memory_win.cc',
        'simple_thread.cc',
        'simple_thread.h',
        'singleton.h',
        'spin_wait.h',
        'stack_container.h',
        'stats_counters.h',
        'stats_table.cc',
        'stats_table.h',
        'stl_util-inl.h',
        'string16.cc',
        'string16.h',
        'string_piece.cc',
        'string_piece.h',
        'string_tokenizer.h',
        'string_util.cc',
        'string_util.h',
        'string_util_win.h',
        'sys_info.h',
        'sys_info_chromeos.cc',
        'sys_info_mac.cc',
        'sys_info_posix.cc',
        'sys_info_win.cc',
        'sys_string_conversions.h',
        'sys_string_conversions_linux.cc',
        'sys_string_conversions_mac.mm',
        'sys_string_conversions_win.cc',
        'system_monitor.cc',
        'system_monitor.h',
        'system_monitor_posix.cc',
        'system_monitor_win.cc',
        'task.h',
        'thread.cc',
        'thread.h',
        'thread_collision_warner.cc',
        'thread_collision_warner.h',
        'thread_local.h',
        'thread_local_posix.cc',
        'thread_local_storage.h',
        'thread_local_storage_posix.cc',
        'thread_local_storage_win.cc',
        'thread_local_win.cc',
        'time.cc',
        'time.h',
        'time_mac.cc',
        'time_posix.cc',
        'time_win.cc',
        'timer.cc',
        'timer.h',
        'trace_event.cc',
        'trace_event.h',
        'tracked.cc',
        'tracked.h',
        'tracked_objects.cc',
        'tracked_objects.h',
        'tuple.h',
        'utf_offset_string_conversions.cc',
        'utf_offset_string_conversions.h',
        'utf_string_conversion_utils.cc',
        'utf_string_conversion_utils.h',
        'utf_string_conversions.cc',
        'utf_string_conversions.h',
        'unix_domain_socket_posix.cc',
        'values.cc',
        'values.h',
        'version.cc',
        'version.h',
        'waitable_event.h',
        'waitable_event_posix.cc',
        'waitable_event_watcher.h',
        'waitable_event_watcher_posix.cc',
        'waitable_event_watcher_win.cc',
        'waitable_event_win.cc',
        'watchdog.cc',
        'watchdog.h',
        'weak_ptr.h',
        'win_util.cc',
        'win_util.h',
        'windows_message_list.h',
        'wmi_util.cc',
        'wmi_util.h',
        'worker_pool.h',
        'worker_pool_linux.cc',
        'worker_pool_linux.h',
        'worker_pool_mac.h',
        'worker_pool_mac.mm',
        'worker_pool_win.cc',
      ],
      'include_dirs': [
        '..',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '..',
        ],
      },
      # These warnings are needed for the files in third_party\dmg_fp.
      'msvs_disabled_warnings': [
        4244, 4554, 4018, 4102,
      ],
      'mac_framework_dirs': [
        '$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework/Frameworks',
      ],
      'conditions': [
        [ 'OS == "linux" or OS == "freebsd"', {
            'sources/': [ ['exclude', '_(mac|win|chromeos)\\.cc$'],
                          ['exclude', '\\.mm?$' ] ],
            'conditions': [
              [ 'chromeos==1 or toolkit_views==1', {
                  'sources/': [ ['include', '_chromeos\\.cc$'] ]
                },
              ],
              [ 'linux_use_tcmalloc==1', {
                  'defines': [
                    'LINUX_USE_TCMALLOC',
                  ],
                  'direct_dependent_settings': {
                    'defines': [
                      'LINUX_USE_TCMALLOC',
                    ],
                  },
                },
              ],
            ],
            'dependencies': [
              '../build/util/build_util.gyp:lastchange',
              '../build/linux/system.gyp:gtk',
              '../build/linux/system.gyp:nss',
            ],
            'cflags': [
              '-Wno-write-strings',
            ],
            'link_settings': {
              'libraries': [
                # We need rt for clock_gettime().
                '-lrt',
              ],
            },
            'export_dependent_settings': [
              '../build/linux/system.gyp:gtk',
            ],
          },
          {  # else: OS != "linux" && OS != "freebsd"
            'sources/': [
              ['exclude', '/xdg_mime/'],
            ],
            'sources!': [
              'crypto/rsa_private_key_nss.cc',
              'crypto/signature_creator_nss.cc',
              'crypto/signature_verifier_nss.cc',
              'atomicops_internals_x86_gcc.cc',
              'directory_watcher_inotify.cc',
              'hmac_nss.cc',
              'linux_util.cc',
              'message_pump_glib.cc',
              'nss_init.cc',
              'nss_init.h',
              'time_posix.cc',
            ],
          },
        ],
        [ 'OS != "linux"', {
            'sources!': [
              # Not automatically excluded by the *linux.cc rules.
              'setproctitle_linux.c',
              'setproctitle_linux.h',
            ],
          },
        ],
        [ 'GENERATOR == "quentin"', {
            # Quentin builds don't have a recent enough glibc to include the
            # inotify headers
            'sources!': [
              'directory_watcher_inotify.cc',
            ],
            'sources': [
              'directory_watcher_stub.cc',
            ],
          },
        ],
        [ 'OS == "freebsd"', {
            'sources!': [
              'directory_watcher_inotify.cc',
            ],
            'sources': [
              'directory_watcher_stub.cc',
            ],
          },
        ],
        [ 'OS == "mac"', {
            'sources/': [ ['exclude', '_(linux|gtk|win|chromeos)\\.cc$'] ],
            'sources!': [
            ],
            'link_settings': {
              'libraries': [
                '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
                '$(SDKROOT)/System/Library/Frameworks/Carbon.framework',
                '$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
                '$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
                '$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
                '$(SDKROOT)/System/Library/Frameworks/Security.framework',
              ],
            },
          },
          {  # else: OS != "mac"
            'sources!': [
              'crypto/cssm_init.cc',
              'crypto/cssm_init.h',
            ],
          }
        ],
        [ 'OS == "win"', {
            'include_dirs': [
              '../chrome/third_party/wtl/include',
            ],
            'sources/': [ ['exclude', '_(linux|gtk|mac|posix|chromeos)\\.cc$'],
                          ['exclude', '\\.mm?$' ] ],
            'sources!': [
              'data_pack.cc',
              'event_recorder_stubs.cc',
              'file_descriptor_shuffle.cc',
              'message_pump_libevent.cc',
              'string16.cc',
            ],
          },
          {  # else: OS != "win"
            'dependencies': ['../third_party/libevent/libevent.gyp:libevent'],
            'sources!': [
              'third_party/purify/pure_api.c',
              'base_drag_source.cc',
              'base_drop_target.cc',
              'cpu.cc',
              'debug_on_start.cc',
              'event_recorder.cc',
              'file_version_info.cc',
              'iat_patch.cc',
              'image_util.cc',
              'object_watcher.cc',
              'pe_image.cc',
              'registry.cc',
              'resource_util.cc',
              'win_util.cc',
              'wmi_util.cc',
            ],
          },
        ],
      ],
    },
    {
      'target_name': 'base_i18n',
      'type': '<(library)',
      'msvs_guid': '968F3222-9798-4D21-BE08-15ECB5EF2994',
      'dependencies': [
        'base',
        '../third_party/icu/icu.gyp:icui18n',
        '../third_party/icu/icu.gyp:icuuc',
      ],
      'export_dependent_settings': [
        'base',
      ],
      'sources': [
        'i18n/file_util_icu.cc',
        'i18n/file_util_icu.h',
        'i18n/icu_string_conversions.cc',
        'i18n/icu_string_conversions.h',
        'i18n/icu_util.cc',
        'i18n/icu_util.h',
        'i18n/number_formatting.cc',
        'i18n/number_formatting.h',
        'i18n/time_formatting.cc',
        'i18n/time_formatting.h',
        'i18n/word_iterator.cc',
        'i18n/word_iterator.h',
      ],
      'conditions': [
        ['OS == "linux" or OS == "freebsd"', {
          'dependencies': [
            '../build/linux/system.gyp:gtk',
          ],
        }],
      ],
    },
    {
      'target_name': 'base_unittests',
      'type': 'executable',
      'msvs_guid': '27A30967-4BBA-48D1-8522-CDE95F7B1CEC',
      'sources': [
        # Infrastructure files.
        'multiprocess_test.h',
        'test/run_all_unittests.cc',
        'test/test_suite.h',

        # Tests.
        'at_exit_unittest.cc',
        'atomic_flag_unittest.cc',
        'atomicops_unittest.cc',
        'command_line_unittest.cc',
        'condition_variable_unittest.cc',
        'crypto/rsa_private_key_unittest.cc',
        'crypto/signature_creator_unittest.cc',
        'crypto/signature_verifier_unittest.cc',
        'data_pack_unittest.cc',
        'debug_util_unittest.cc',
        'directory_watcher_unittest.cc',
        'field_trial_unittest.cc',
        'file_descriptor_shuffle_unittest.cc',
        'file_path_unittest.cc',
        'file_util_unittest.cc',
        'file_version_info_unittest.cc',
        'gfx/rect_unittest.cc',
        'gmock_unittest.cc',
        'histogram_unittest.cc',
        'hmac_unittest.cc',
        'id_map_unittest.cc',
        'i18n/file_util_icu_unittest.cc',
        'i18n/icu_string_conversions_unittest.cc',
        'i18n/word_iterator_unittest.cc',
        'json/json_reader_unittest.cc',
        'json/json_writer_unittest.cc',
        'json/string_escape_unittest.cc',
        'lazy_instance_unittest.cc',
        'leak_tracker_unittest.cc',
        'linked_list_unittest.cc',
        'linked_ptr_unittest.cc',
        'mac_util_unittest.mm',
        'message_loop_unittest.cc',
        'message_pump_glib_unittest.cc',
        'object_watcher_unittest.cc',
        'observer_list_unittest.cc',
        'path_service_unittest.cc',
        'pe_image_unittest.cc',
        'pickle_unittest.cc',
        'pr_time_unittest.cc',
        'process_util_unittest.cc',
        'rand_util_unittest.cc',
        'ref_counted_unittest.cc',
        'scoped_bstr_win_unittest.cc',
        'scoped_comptr_win_unittest.cc',
        'scoped_ptr_unittest.cc',
        'scoped_temp_dir_unittest.cc',
        'scoped_variant_win_unittest.cc',
        'sha1_unittest.cc',
        'sha2_unittest.cc',
        'shared_memory_unittest.cc',
        'simple_thread_unittest.cc',
        'singleton_unittest.cc',
        'stack_container_unittest.cc',
        'stats_table_unittest.cc',
        'string_piece_unittest.cc',
        'string_tokenizer_unittest.cc',
        'string_util_unittest.cc',
        'sys_info_unittest.cc',
        'sys_string_conversions_mac_unittest.mm',
        'sys_string_conversions_unittest.cc',
        'system_monitor_unittest.cc',
        'task_unittest.cc',
        'thread_collision_warner_unittest.cc',
        'thread_local_storage_unittest.cc',
        'thread_local_unittest.cc',
        'thread_unittest.cc',
        'time_unittest.cc',
        'time_win_unittest.cc',
        'timer_unittest.cc',
        'tracked_objects_unittest.cc',
        'tuple_unittest.cc',
        'utf_offset_string_conversions_unittest.cc',
        'utf_string_conversions_unittest.cc',
        'values_unittest.cc',
        'version_unittest.cc',
        'waitable_event_unittest.cc',
        'waitable_event_watcher_unittest.cc',
        'watchdog_unittest.cc',
        'weak_ptr_unittest.cc',
        'win_util_unittest.cc',
        'wmi_util_unittest.cc',
        'worker_pool_unittest.cc',
      ],
      'include_dirs': [
        # word_iterator.h (used by word_iterator_unittest.cc) leaks an ICU
        # #include for unicode/uchar.h.  This should probably be cleaned up.
        '../third_party/icu/public/common',
      ],
      'dependencies': [
        'base',
        'base_i18n',
        '../testing/gmock.gyp:gmock',
        '../testing/gtest.gyp:gtest',
      ],
      'conditions': [
        ['OS == "linux" or OS == "freebsd"', {
          'sources/': [ ['exclude', '\\.mm?$' ] ],
          'sources!': [
            'file_version_info_unittest.cc',
            'worker_pool_linux_unittest.cc',
          ],
          'conditions': [
            [ 'linux_use_tcmalloc==1', {
                'dependencies': [
                  '../third_party/tcmalloc/tcmalloc.gyp:tcmalloc',
                ],
              },
            ],
          ],
          'dependencies': [
            '../build/linux/system.gyp:gtk',
            '../build/linux/system.gyp:nss',
            '../tools/xdisplaycheck/xdisplaycheck.gyp:xdisplaycheck',
          ],
        }, {  # OS != "linux" and OS != "freebsd"
          'sources!': [
            'message_pump_glib_unittest.cc',
          ]
        }],
        # This is needed to trigger the dll copy step on windows.
        # TODO(mark): This should not be necessary.
        ['OS == "win"', {
          'dependencies': [
            '../third_party/icu/icu.gyp:icudata',
          ],
          'sources!': [
            'data_pack_unittest.cc',
            'file_descriptor_shuffle_unittest.cc',
          ],
        }, {  # OS != "win"
          'sources!': [
            'object_watcher_unittest.cc',
            'pe_image_unittest.cc',
            'scoped_bstr_win_unittest.cc',
            'scoped_comptr_win_unittest.cc',
            'scoped_variant_win_unittest.cc',
            'system_monitor_unittest.cc',
            'time_win_unittest.cc',
            'win_util_unittest.cc',
            'wmi_util_unittest.cc',
          ],
        }],
      ],
    },
    {
      'target_name': 'test_support_base',
      'type': '<(library)',
      'dependencies': [
        'base',
      ],
      'sources': [
        'test/test_file_util.h',
        'test/test_file_util_linux.cc',
        'test/test_file_util_mac.cc',
        'test/test_file_util_posix.cc',
        'test/test_file_util_win.cc',
      ],
      'conditions': [
        [ 'OS == "linux" or OS == "freebsd"', {
            'sources/': [ ['exclude', '_(mac|win|chromeos)\\.cc$'],
                          ['exclude', '\\.mm?$' ] ],
            'conditions': [
              [ 'chromeos==1 or toolkit_views==1', {
                  'sources/': [ ['include', '_chromeos\\.cc$'] ]
                },
              ],
            ],
          },
        ],
        [ 'OS == "mac"', {
            'sources/': [ ['exclude', '_(linux|win|chromeos)\\.cc$'] ],
          },
        ],
        [ 'OS == "win"', {
            'sources/': [ ['exclude', '_(linux|mac|posix|chromeos)\\.cc$'],
                          ['exclude', '\\.mm?$' ] ],
          },
        ],
      ],
    },
    {
      'target_name': 'test_support_perf',
      'type': '<(library)',
      'dependencies': [
        'base',
        '../testing/gtest.gyp:gtest',
      ],
      'sources': [
        'perftimer.cc',
        'test/run_all_perftests.cc',
      ],
      'direct_dependent_settings': {
        'defines': [
          'PERF_TEST',
        ],
      },
      'conditions': [
        ['OS == "linux" or OS == "freebsd"', {
          'dependencies': [
            # Needed to handle the #include chain:
            #   base/test/perf_test_suite.h
            #   base/test/test_suite.h
            #   gtk/gtk.h
            '../build/linux/system.gyp:gtk',
          ],
        }],
      ],
    },
  ],
  'conditions': [
    [ 'OS == "win"', {
      'targets': [
        {
          'target_name': 'debug_message',
          'type': 'executable',
          'sources': [
            'debug_message.cc',
          ],
          'msvs_settings': {
            'VCLinkerTool': {
              'SubSystem': '2',         # Set /SUBSYSTEM:WINDOWS
            },
          },
        },
      ],
    }],
    [ 'OS == "linux" or OS == "freebsd"', {
      'targets': [
        {
          'target_name': 'linux_versioninfo',
          'type': '<(library)',
          'sources': [
            'file_version_info_linux.cc',
          ],
          'include_dirs': [
            '..',
            '<(SHARED_INTERMEDIATE_DIR)',
          ],
          'actions': [
            {
              'action_name': 'linux_version',
              'variables': {
                'lastchange_path':
                  '<(SHARED_INTERMEDIATE_DIR)/build/LASTCHANGE',
                'version_py_path': '../chrome/tools/build/version.py',
                'version_path': '../chrome/VERSION',
                'template_input_path': 'file_version_info_linux.h.version',
              },
              'conditions': [
                [ 'branding == "Chrome"', {
                  'variables': {
                     'branding_path':
                       '../chrome/app/theme/google_chrome/BRANDING',
                  },
                }, { # else branding!="Chrome"
                  'variables': {
                     'branding_path':
                       '../chrome/app/theme/chromium/BRANDING',
                  },
                }],
              ],
              'inputs': [
                '<(template_input_path)',
                '<(version_path)',
                '<(branding_path)',
                '<(lastchange_path)',
              ],
              'outputs': [
                '<(SHARED_INTERMEDIATE_DIR)/base/file_version_info_linux.h',
              ],
              'action': [
                'python',
                '<(version_py_path)',
                '-f', '<(version_path)',
                '-f', '<(branding_path)',
                '-f', '<(lastchange_path)',
                '<(template_input_path)',
                '<@(_outputs)',
              ],
              'message': 'Generating version information',
            },
          ],
        },
      ],
    }],
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
