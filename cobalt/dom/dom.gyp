# Copyright 2014 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'variables': {
    'sb_pedantic_warnings': 1,
  },
  'targets': [
    {
      'target_name': 'dom',
      'type': 'static_library',
      'sources': [
        'animation_event.h',
        'animation_frame_request_callback_list.cc',
        'animation_frame_request_callback_list.h',
        'attr.cc',
        'attr.h',
        'audio_track.h',
        'audio_track_list.h',
        'base64.cc',
        'base64.h',
        'benchmark_stat_names.cc',
        'benchmark_stat_names.h',
        'blob.cc',
        'blob.h',
        'blob_property_bag.h',
        'buffer_source.cc',
        'buffer_source.h',
        'captions/system_caption_settings.cc',
        'captions/system_caption_settings.h',
        'camera_3d.cc',
        'camera_3d.h',
        'cdata_section.cc',
        'cdata_section.h',
        'character_data.cc',
        'character_data.h',
        'comment.cc',
        'comment.h',
        'console.cc',
        'console.h',
        'crypto.cc',
        'crypto.h',
        'csp_delegate.cc',
        'csp_delegate.h',
        'csp_delegate_enum.h',
        'csp_delegate_factory.cc',
        'csp_delegate_factory.h',
        'csp_violation_reporter.cc',
        'csp_violation_reporter.h',
        'css_animations_adapter.cc',
        'css_animations_adapter.h',
        'css_transitions_adapter.cc',
        'css_transitions_adapter.h',
        'custom_event.h',
        'device_orientation_event.cc',
        'device_orientation_event.h',
        'device_orientation_event_init.h',
        'document.cc',
        'document.h',
        'document_timeline.cc',
        'document_timeline.h',
        'document_type.cc',
        'document_type.h',
        'dom_animatable.cc',
        'dom_animatable.h',
        'dom_implementation.cc',
        'dom_implementation.h',
        'dom_parser.cc',
        'dom_parser.h',
        'dom_rect.h',
        'dom_rect_list.cc',
        'dom_rect_list.h',
        'dom_rect_read_only.h',
        'dom_settings.cc',
        'dom_settings.h',
        'dom_stat_tracker.cc',
        'dom_stat_tracker.h',
        'dom_string_map.cc',
        'dom_string_map.h',
        'dom_token_list.cc',
        'dom_token_list.h',
        'element.cc',
        'element.h',
        'error_event.h',
        'event.cc',
        'event.h',
        'event_init.h',
        'event_listener.h',
        'event_queue.cc',
        'event_queue.h',
        'event_target.cc',
        'event_target.h',
        'focus_event.cc',
        'focus_event.h',
        'focus_event_init.h',
        'font_cache.cc',
        'font_cache.h',
        'font_face.cc',
        'font_face.h',
        'font_face_updater.cc',
        'font_face_updater.h',
        'font_list.cc',
        'font_list.h',
        'generic_event_handler_reference.cc',
        'generic_event_handler_reference.h',
        'global_stats.cc',
        'global_stats.h',
        'history.cc',
        'history.h',
        'html_anchor_element.cc',
        'html_anchor_element.h',
        'html_body_element.cc',
        'html_body_element.h',
        'html_br_element.cc',
        'html_br_element.h',
        'html_collection.cc',
        'html_collection.h',
        'html_div_element.cc',
        'html_div_element.h',
        'html_element.cc',
        'html_element.h',
        'html_element_context.cc',
        'html_element_context.h',
        'html_element_factory.cc',
        'html_element_factory.h',
        'html_head_element.cc',
        'html_head_element.h',
        'html_heading_element.cc',
        'html_heading_element.h',
        'html_html_element.cc',
        'html_html_element.h',
        'html_image_element.cc',
        'html_image_element.h',
        'html_link_element.cc',
        'html_link_element.h',
        'html_media_element.cc',
        'html_media_element.h',
        'html_meta_element.cc',
        'html_meta_element.h',
        'html_paragraph_element.cc',
        'html_paragraph_element.h',
        'html_script_element.cc',
        'html_script_element.h',
        'html_span_element.cc',
        'html_span_element.h',
        'html_style_element.cc',
        'html_style_element.h',
        'html_title_element.cc',
        'html_title_element.h',
        'html_unknown_element.h',
        'html_video_element.cc',
        'html_video_element.h',
        'initial_computed_style.cc',
        'initial_computed_style.h',
        'input_event.cc',
        'input_event.h',
        'input_event_init.h',
        'keyboard_event.cc',
        'keyboard_event.h',
        'keyboard_event_init.h',
        'keycode.h',
        'keyframes_map_updater.cc',
        'keyframes_map_updater.h',
        'layout_boxes.h',
        'local_storage_database.cc',
        'local_storage_database.h',
        'location.cc',
        'location.h',
        'media_query_list.cc',
        'media_query_list.h',
        'memory_info.cc',
        'memory_info.h',
        'message_event.cc',
        'message_event.h',
        'mime_type_array.cc',
        'mime_type_array.h',
        'mouse_event.cc',
        'mouse_event.h',
        'mouse_event_init.h',
        'mutation_observer.cc',
        'mutation_observer.h',
        'mutation_observer_init.h',
        'mutation_observer_task_manager.cc',
        'mutation_observer_task_manager.h',
        'mutation_record.cc',
        'mutation_record.h',
        'mutation_reporter.cc',
        'mutation_reporter.h',
        'named_node_map.cc',
        'named_node_map.h',
        'navigator.cc',
        'navigator.h',
        'node.cc',
        'node.h',
        'node_children_iterator.h',
        'node_collection.h',
        'node_descendants_iterator.h',
        'node_list.cc',
        'node_list.h',
        'node_list_live.cc',
        'node_list_live.h',
        'on_error_event_listener.cc',
        'on_error_event_listener.h',
        'on_screen_keyboard.cc',
        'on_screen_keyboard.h',
        'on_screen_keyboard_bridge.h',
        'performance.cc',
        'performance.h',
        'performance_timing.cc',
        'performance_timing.h',
        'plugin_array.cc',
        'plugin_array.h',
        'pointer_event.cc',
        'pointer_event.h',
        'pointer_event_init.h',
        'pointer_state.cc',
        'pointer_state.h',
        'progress_event.cc',
        'progress_event.h',
        'pseudo_element.cc',
        'pseudo_element.h',
        'registered_observer.h',
        'registered_observer_list.cc',
        'registered_observer_list.h',
        'rule_matching.cc',
        'rule_matching.h',
        'screen.h',
        'screenshot.cc',
        'screenshot.h',
        'screenshot_manager.cc',
        'screenshot_manager.h',
        'script_event_log.cc',
        'script_event_log.h',
        'security_policy_violation_event.cc',
        'security_policy_violation_event.h',
        'serializer.cc',
        'serializer.h',
        'storage.cc',
        'storage.h',
        'storage_area.cc',
        'storage_area.h',
        'storage_event.cc',
        'storage_event.h',
        'test_runner.cc',
        'test_runner.h',
        'text.cc',
        'text.h',
        'time_ranges.cc',
        'time_ranges.h',
        'track_base.h',
        'track_default.h',
        'track_default_list.h',
        'track_event.h',
        'transition_event.h',
        'ui_event.cc',
        'ui_event.h',
        'ui_event_init.h',
        'ui_event_with_key_state.cc',
        'ui_event_with_key_state.h',
        'url.cc',
        'url.h',
        'url_registry.h',
        'url_utils.cc',
        'url_utils.h',
        'video_track.h',
        'video_track_list.h',
        'wheel_event.cc',
        'wheel_event.h',
        'wheel_event_init.h',
        'window.cc',
        'window.h',
        'window_timers.cc',
        'window_timers.h',
        'xml_document.h',
        'xml_serializer.cc',
        'xml_serializer.h',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/browser/browser_bindings_gen.gyp:generated_types',
        '<(DEPTH)/cobalt/csp/csp.gyp:csp',
        '<(DEPTH)/cobalt/cssom/cssom.gyp:cssom',
        '<(DEPTH)/cobalt/dom/dom_exception.gyp:dom_exception',
        '<(DEPTH)/cobalt/loader/loader.gyp:loader',
        '<(DEPTH)/cobalt/media_capture/media_capture.gyp:media_capture',
        '<(DEPTH)/cobalt/media_session/media_session.gyp:media_session',
        # Interface layer to avoid directly depending on network.
        '<(DEPTH)/cobalt/network_bridge/network_bridge.gyp:network_bridge',
        '<(DEPTH)/cobalt/page_visibility/page_visibility.gyp:page_visibility',
        '<(DEPTH)/cobalt/script/script.gyp:script',
        '<(DEPTH)/cobalt/speech/speech.gyp:speech',
        '<(DEPTH)/cobalt/storage/storage.gyp:storage',
        '<(DEPTH)/cobalt/system_window/system_window.gyp:system_window',
        '<(DEPTH)/cobalt/web_animations/web_animations.gyp:web_animations',
        '<(DEPTH)/googleurl/googleurl.gyp:googleurl',
        '<(DEPTH)/nb/nb.gyp:nb',
      ],
      'conditions': [
        ['cobalt_media_source_2016==1', {
          'sources': [
            'eme/media_encrypted_event.cc',
            'eme/media_encrypted_event.h',
            'eme/media_key_message_event.cc',
            'eme/media_key_message_event.h',
            'eme/media_key_session.cc',
            'eme/media_key_session.h',
            'eme/media_key_status_map.cc',
            'eme/media_key_status_map.h',
            'eme/media_key_system_access.cc',
            'eme/media_key_system_access.h',
            'eme/media_keys.cc',
            'eme/media_keys.h',
            'media_source/media_source.cc',
            'media_source/media_source.h',
            'media_source/source_buffer.cc',
            'media_source/source_buffer.h',
            'media_source/source_buffer_list.cc',
            'media_source/source_buffer_list.h',
          ],
          'dependencies': [
            '<(DEPTH)/cobalt/media/media2.gyp:media2',
          ],
        }, {
          'sources': [
            'media_error.h',
            'media_key_complete_event.cc',
            'media_key_complete_event.h',
            'media_key_error.h',
            'media_key_error_event.cc',
            'media_key_error_event.h',
            'media_key_message_event.cc',
            'media_key_message_event.h',
            'media_key_needed_event.cc',
            'media_key_needed_event.h',
            'media_source.cc',
            'media_source.h',
            'source_buffer.cc',
            'source_buffer.h',
            'source_buffer_list.cc',
            'source_buffer_list.h',
          ],
          'dependencies': [
            '<(DEPTH)/cobalt/media/media.gyp:media',
          ],
        }],
        ['enable_map_to_mesh == 1', {
          'defines' : ['ENABLE_MAP_TO_MESH'],
        }],
      ],
      # This target doesn't generate any headers, but it exposes generated
      # header files (for idl dictionaries) through this module's public header
      # files. So mark this target as a hard dependency to ensure that any
      # dependent targets wait until this target (and its hard dependencies) are
      # built.
      'hard_dependency': 1,
      'export_dependent_settings': [
        # Additionally, ensure that the include directories for generated
        # headers are put on the include directories for targets that depend on
        # this one.
        '<(DEPTH)/cobalt/browser/browser_bindings_gen.gyp:generated_types',
      ],
    },
  ],
}
