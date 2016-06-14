# Copyright 2016 Google Inc. All Rights Reserved.
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
  'targets': [
    {
      'target_name': 'starboard_platform',
      'type': 'static_library',
      'sources': [
        '<(DEPTH)/starboard/shared/starboard/file_mode_string_to_flags.cc',
        '<(DEPTH)/starboard/shared/starboard/log_message.cc',
        '<(DEPTH)/starboard/stub/atomic_public.h',
        '<(DEPTH)/starboard/stub/byte_swap.cc',
        '<(DEPTH)/starboard/stub/character_is_alphanumeric.cc',
        '<(DEPTH)/starboard/stub/character_is_digit.cc',
        '<(DEPTH)/starboard/stub/character_is_hex_digit.cc',
        '<(DEPTH)/starboard/stub/character_is_space.cc',
        '<(DEPTH)/starboard/stub/character_is_upper.cc',
        '<(DEPTH)/starboard/stub/character_to_lower.cc',
        '<(DEPTH)/starboard/stub/character_to_upper.cc',
        '<(DEPTH)/starboard/stub/condition_variable_broadcast.cc',
        '<(DEPTH)/starboard/stub/condition_variable_create.cc',
        '<(DEPTH)/starboard/stub/condition_variable_destroy.cc',
        '<(DEPTH)/starboard/stub/condition_variable_signal.cc',
        '<(DEPTH)/starboard/stub/condition_variable_wait.cc',
        '<(DEPTH)/starboard/stub/condition_variable_wait_timed.cc',
        '<(DEPTH)/starboard/stub/directory_can_open.cc',
        '<(DEPTH)/starboard/stub/directory_close.cc',
        '<(DEPTH)/starboard/stub/directory_create.cc',
        '<(DEPTH)/starboard/stub/directory_get_next.cc',
        '<(DEPTH)/starboard/stub/directory_open.cc',
        '<(DEPTH)/starboard/stub/double_absolute.cc',
        '<(DEPTH)/starboard/stub/double_exponent.cc',
        '<(DEPTH)/starboard/stub/double_floor.cc',
        '<(DEPTH)/starboard/stub/double_is_finite.cc',
        '<(DEPTH)/starboard/stub/double_is_nan.cc',
        '<(DEPTH)/starboard/stub/file_can_open.cc',
        '<(DEPTH)/starboard/stub/file_close.cc',
        '<(DEPTH)/starboard/stub/file_delete.cc',
        '<(DEPTH)/starboard/stub/file_exists.cc',
        '<(DEPTH)/starboard/stub/file_flush.cc',
        '<(DEPTH)/starboard/stub/file_get_info.cc',
        '<(DEPTH)/starboard/stub/file_get_path_info.cc',
        '<(DEPTH)/starboard/stub/file_open.cc',
        '<(DEPTH)/starboard/stub/file_read.cc',
        '<(DEPTH)/starboard/stub/file_seek.cc',
        '<(DEPTH)/starboard/stub/file_truncate.cc',
        '<(DEPTH)/starboard/stub/file_write.cc',
        '<(DEPTH)/starboard/stub/log.cc',
        '<(DEPTH)/starboard/stub/log_flush.cc',
        '<(DEPTH)/starboard/stub/log_format.cc',
        '<(DEPTH)/starboard/stub/log_is_tty.cc',
        '<(DEPTH)/starboard/stub/log_raw.cc',
        '<(DEPTH)/starboard/stub/log_raw_dump_stack.cc',
        '<(DEPTH)/starboard/stub/log_raw_format.cc',
        '<(DEPTH)/starboard/stub/memory_allocate_aligned_unchecked.cc',
        '<(DEPTH)/starboard/stub/memory_allocate_unchecked.cc',
        '<(DEPTH)/starboard/stub/memory_compare.cc',
        '<(DEPTH)/starboard/stub/memory_copy.cc',
        '<(DEPTH)/starboard/stub/memory_find_byte.cc',
        '<(DEPTH)/starboard/stub/memory_free.cc',
        '<(DEPTH)/starboard/stub/memory_free_aligned.cc',
        '<(DEPTH)/starboard/stub/memory_get_stack_bounds.cc',
        '<(DEPTH)/starboard/stub/memory_map.cc',
        '<(DEPTH)/starboard/stub/memory_move.cc',
        '<(DEPTH)/starboard/stub/memory_reallocate_unchecked.cc',
        '<(DEPTH)/starboard/stub/memory_set.cc',
        '<(DEPTH)/starboard/stub/memory_unmap.cc',
        '<(DEPTH)/starboard/stub/mutex_acquire.cc',
        '<(DEPTH)/starboard/stub/mutex_acquire_try.cc',
        '<(DEPTH)/starboard/stub/mutex_create.cc',
        '<(DEPTH)/starboard/stub/mutex_destroy.cc',
        '<(DEPTH)/starboard/stub/mutex_release.cc',
        '<(DEPTH)/starboard/stub/once.cc',
        '<(DEPTH)/starboard/stub/socket_accept.cc',
        '<(DEPTH)/starboard/stub/socket_bind.cc',
        '<(DEPTH)/starboard/stub/socket_clear_last_error.cc',
        '<(DEPTH)/starboard/stub/socket_connect.cc',
        '<(DEPTH)/starboard/stub/socket_create.cc',
        '<(DEPTH)/starboard/stub/socket_destroy.cc',
        '<(DEPTH)/starboard/stub/socket_free_resolution.cc',
        '<(DEPTH)/starboard/stub/socket_get_last_error.cc',
        '<(DEPTH)/starboard/stub/socket_get_local_address.cc',
        '<(DEPTH)/starboard/stub/socket_get_local_interface_address.cc',
        '<(DEPTH)/starboard/stub/socket_is_connected.cc',
        '<(DEPTH)/starboard/stub/socket_is_connected_and_idle.cc',
        '<(DEPTH)/starboard/stub/socket_join_multicast_group.cc',
        '<(DEPTH)/starboard/stub/socket_listen.cc',
        '<(DEPTH)/starboard/stub/socket_receive_from.cc',
        '<(DEPTH)/starboard/stub/socket_resolve.cc',
        '<(DEPTH)/starboard/stub/socket_send_to.cc',
        '<(DEPTH)/starboard/stub/socket_set_broadcast.cc',
        '<(DEPTH)/starboard/stub/socket_set_receive_buffer_size.cc',
        '<(DEPTH)/starboard/stub/socket_set_reuse_address.cc',
        '<(DEPTH)/starboard/stub/socket_set_send_buffer_size.cc',
        '<(DEPTH)/starboard/stub/socket_set_tcp_keep_alive.cc',
        '<(DEPTH)/starboard/stub/socket_set_tcp_no_delay.cc',
        '<(DEPTH)/starboard/stub/socket_set_tcp_window_scaling.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_add.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_create.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_destroy.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_remove.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_wait.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_wait_timed.cc',
        '<(DEPTH)/starboard/stub/socket_waiter_wake_up.cc',
        '<(DEPTH)/starboard/stub/storage_close_record.cc',
        '<(DEPTH)/starboard/stub/storage_delete_record.cc',
        '<(DEPTH)/starboard/stub/storage_get_record_size.cc',
        '<(DEPTH)/starboard/stub/storage_open_record.cc',
        '<(DEPTH)/starboard/stub/storage_read_record.cc',
        '<(DEPTH)/starboard/stub/storage_write_record.cc',
        '<(DEPTH)/starboard/stub/string_compare.cc',
        '<(DEPTH)/starboard/stub/string_compare_all.cc',
        '<(DEPTH)/starboard/stub/string_compare_no_case.cc',
        '<(DEPTH)/starboard/stub/string_compare_no_case_n.cc',
        '<(DEPTH)/starboard/stub/string_compare_wide.cc',
        '<(DEPTH)/starboard/stub/string_concat.cc',
        '<(DEPTH)/starboard/stub/string_concat_wide.cc',
        '<(DEPTH)/starboard/stub/string_copy.cc',
        '<(DEPTH)/starboard/stub/string_copy_wide.cc',
        '<(DEPTH)/starboard/stub/string_duplicate.cc',
        '<(DEPTH)/starboard/stub/string_find_character.cc',
        '<(DEPTH)/starboard/stub/string_find_last_character.cc',
        '<(DEPTH)/starboard/stub/string_find_string.cc',
        '<(DEPTH)/starboard/stub/string_format.cc',
        '<(DEPTH)/starboard/stub/string_format_wide.cc',
        '<(DEPTH)/starboard/stub/string_get_length.cc',
        '<(DEPTH)/starboard/stub/string_get_length_wide.cc',
        '<(DEPTH)/starboard/stub/string_parse_signed_integer.cc',
        '<(DEPTH)/starboard/stub/string_parse_uint64.cc',
        '<(DEPTH)/starboard/stub/string_parse_unsigned_integer.cc',
        '<(DEPTH)/starboard/stub/string_scan.cc',
        '<(DEPTH)/starboard/stub/system_binary_search.cc',
        '<(DEPTH)/starboard/stub/system_break_into_debugger.cc',
        '<(DEPTH)/starboard/stub/system_clear_last_error.cc',
        '<(DEPTH)/starboard/stub/system_get_error_string.cc',
        '<(DEPTH)/starboard/stub/system_get_last_error.cc',
        '<(DEPTH)/starboard/stub/system_get_locale_id.cc',
        '<(DEPTH)/starboard/stub/system_get_number_of_processors.cc',
        '<(DEPTH)/starboard/stub/system_get_path.cc',
        '<(DEPTH)/starboard/stub/system_get_property.cc',
        '<(DEPTH)/starboard/stub/system_get_random_data.cc',
        '<(DEPTH)/starboard/stub/system_get_random_uint64.cc',
        '<(DEPTH)/starboard/stub/system_get_stack.cc',
        '<(DEPTH)/starboard/stub/system_get_total_memory.cc',
        '<(DEPTH)/starboard/stub/system_has_capability.cc',
        '<(DEPTH)/starboard/stub/system_is_debugger_attached.cc',
        '<(DEPTH)/starboard/stub/system_request_stop.cc',
        '<(DEPTH)/starboard/stub/system_sort.cc',
        '<(DEPTH)/starboard/stub/system_symbolize.cc',
        '<(DEPTH)/starboard/stub/thread_create.cc',
        '<(DEPTH)/starboard/stub/thread_create_local_key.cc',
        '<(DEPTH)/starboard/stub/thread_destroy_local_key.cc',
        '<(DEPTH)/starboard/stub/thread_detach.cc',
        '<(DEPTH)/starboard/stub/thread_get_current.cc',
        '<(DEPTH)/starboard/stub/thread_get_id.cc',
        '<(DEPTH)/starboard/stub/thread_get_local_value.cc',
        '<(DEPTH)/starboard/stub/thread_get_name.cc',
        '<(DEPTH)/starboard/stub/thread_is_equal.cc',
        '<(DEPTH)/starboard/stub/thread_join.cc',
        '<(DEPTH)/starboard/stub/thread_set_local_value.cc',
        '<(DEPTH)/starboard/stub/thread_set_name.cc',
        '<(DEPTH)/starboard/stub/thread_sleep.cc',
        '<(DEPTH)/starboard/stub/thread_yield.cc',
        '<(DEPTH)/starboard/stub/time_get_monotonic_now.cc',
        '<(DEPTH)/starboard/stub/time_get_now.cc',
        '<(DEPTH)/starboard/stub/time_zone_get_current.cc',
        '<(DEPTH)/starboard/stub/time_zone_get_dst_name.cc',
        '<(DEPTH)/starboard/stub/time_zone_get_name.cc',
        '<(DEPTH)/starboard/stub/user_get_current.cc',
        '<(DEPTH)/starboard/stub/user_get_property.cc',
        '<(DEPTH)/starboard/stub/user_get_signed_in.cc',
        '<(DEPTH)/starboard/stub/user_is_age_restricted.cc',
        '<(DEPTH)/starboard/stub/user_start_sign_in.cc',
        '<(DEPTH)/starboard/stub/window_create.cc',
        '<(DEPTH)/starboard/stub/window_destroy.cc',
        '<(DEPTH)/starboard/stub/window_get_platform_handle.cc',
        '<(DEPTH)/starboard/stub/window_get_size.cc',
        '<(DEPTH)/starboard/stub/window_set_default_options.cc',
      ],
      'defines': [
        # This must be defined when building Starboard, and must not when
        # building Starboard client code.
        'STARBOARD_IMPLEMENTATION',
      ],
    },
  ],
}
