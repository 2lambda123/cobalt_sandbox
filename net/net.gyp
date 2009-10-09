# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'net',
      'type': '<(library)',
      'dependencies': [
        '../base/base.gyp:base',
        '../build/temp_gyp/googleurl.gyp:googleurl',
        '../sdch/sdch.gyp:sdch',
        '../third_party/bzip2/bzip2.gyp:bzip2',
        '../third_party/icu/icu.gyp:icui18n',
        '../third_party/icu/icu.gyp:icuuc',
        '../third_party/modp_b64/modp_b64.gyp:modp_b64',
        '../third_party/zlib/zlib.gyp:zlib',
        'net_resources',
      ],
      'msvs_guid': '326E9795-E760-410A-B69A-3F79DB3F5243',
      'sources': [
        'base/address_list.cc',
        'base/address_list.h',
        'base/auth.h',
        'base/base64.cc',
        'base/base64.h',
        'base/bzip2_filter.cc',
        'base/bzip2_filter.h',
        'base/cache_type.h',
        'base/cert_status_flags.cc',
        'base/cert_status_flags.h',
        'base/cert_verifier.cc',
        'base/cert_verifier.h',
        'base/cert_verify_result.h',
        'base/completion_callback.h',
        'base/connection_type_histograms.cc',
        'base/connection_type_histograms.h',
        'base/cookie_monster.cc',
        'base/cookie_monster.h',
        'base/cookie_options.h',
        'base/cookie_policy.cc',
        'base/cookie_policy.h',
        'base/cookie_store.h',
        'base/data_url.cc',
        'base/data_url.h',
        'base/directory_lister.cc',
        'base/directory_lister.h',
        'base/dns_util.cc',
        'base/dns_util.h',
        'base/effective_tld_names.cc',
        'base/effective_tld_names.dat',
        'base/escape.cc',
        'base/escape.h',
        'base/ev_root_ca_metadata.cc',
        'base/ev_root_ca_metadata.h',
        'base/file_stream.h',
        'base/file_stream_posix.cc',
        'base/file_stream_win.cc',
        'base/filter.cc',
        'base/filter.h',
        'base/gzip_filter.cc',
        'base/gzip_filter.h',
        'base/gzip_header.cc',
        'base/gzip_header.h',
        'base/host_cache.cc',
        'base/host_cache.h',
        'base/host_resolver.cc',
        'base/host_resolver.h',
        'base/host_resolver_impl.cc',
        'base/host_resolver_impl.h',
        'base/host_resolver_proc.cc',
        'base/host_resolver_proc.h',
        'base/io_buffer.cc',
        'base/io_buffer.h',
        'base/listen_socket.cc',
        'base/listen_socket.h',
        'base/load_flags.h',
        'base/load_log.h',
        'base/load_log.cc',
        'base/load_log_event_type_list.h',
        'base/load_log_util.cc',
        'base/load_log_util.h',
        'base/load_states.h',
        'base/mime_sniffer.cc',
        'base/mime_sniffer.h',
        'base/mime_util.cc',
        'base/mime_util.h',
        # TODO(eroman): move this into its own test-support target.
        'base/mock_host_resolver.cc',
        'base/mock_host_resolver.h',
        'base/net_error_list.h',
        'base/net_errors.cc',
        'base/net_errors.h',
        'base/net_module.cc',
        'base/net_module.h',
        'base/net_util.cc',
        'base/net_util.h',
        'base/net_util_posix.cc',
        'base/net_util_win.cc',
        'base/nss_memio.c',
        'base/nss_memio.h',
        'base/platform_mime_util.h',
        # TODO(tc): gnome-vfs? xdgmime? /etc/mime.types?
        'base/platform_mime_util_linux.cc',
        'base/platform_mime_util_mac.cc',
        'base/platform_mime_util_win.cc',
        'base/registry_controlled_domain.cc',
        'base/registry_controlled_domain.h',
        'base/scoped_cert_chain_context.h',
        'base/sdch_filter.cc',
        'base/sdch_filter.h',
        'base/sdch_manager.cc',
        'base/sdch_manager.h',
        'base/ssl_cert_request_info.h',
        'base/ssl_client_auth_cache.cc',
        'base/ssl_client_auth_cache.h',
        'base/ssl_config_service.cc',
        'base/ssl_config_service.h',
        'base/ssl_config_service_defaults.h',
        'base/ssl_config_service_mac.cc',
        'base/ssl_config_service_mac.h',
        'base/ssl_config_service_win.cc',
        'base/ssl_config_service_win.h',
        'base/ssl_info.h',
        'base/strict_transport_security_state.cc',
        'base/strict_transport_security_state.h',
        'base/telnet_server.cc',
        'base/telnet_server.h',
        'base/test_completion_callback.h',
        'base/upload_data.cc',
        'base/upload_data.h',
        'base/upload_data_stream.cc',
        'base/upload_data_stream.h',
        'base/wininet_util.cc',
        'base/wininet_util.h',
        'base/winsock_init.cc',
        'base/winsock_init.h',
        'base/x509_certificate.cc',
        'base/x509_certificate.h',
        'base/x509_certificate_mac.cc',
        'base/x509_certificate_nss.cc',
        'base/x509_certificate_win.cc',
        'disk_cache/addr.cc',
        'disk_cache/addr.h',
        'disk_cache/backend_impl.cc',
        'disk_cache/backend_impl.h',
        'disk_cache/bitmap.cc',
        'disk_cache/bitmap.h',
        'disk_cache/block_files.cc',
        'disk_cache/block_files.h',
        'disk_cache/cache_util.h',
        'disk_cache/cache_util_posix.cc',
        'disk_cache/cache_util_win.cc',
        'disk_cache/disk_cache.h',
        'disk_cache/disk_format.h',
        'disk_cache/entry_impl.cc',
        'disk_cache/entry_impl.h',
        'disk_cache/errors.h',
        'disk_cache/eviction.cc',
        'disk_cache/eviction.h',
        'disk_cache/file.h',
        'disk_cache/file_block.h',
        'disk_cache/file_lock.cc',
        'disk_cache/file_lock.h',
        'disk_cache/file_posix.cc',
        'disk_cache/file_win.cc',
        'disk_cache/hash.cc',
        'disk_cache/hash.h',
        'disk_cache/histogram_macros.h',
        'disk_cache/mapped_file.h',
        'disk_cache/mapped_file_posix.cc',
        'disk_cache/mapped_file_win.cc',
        'disk_cache/mem_backend_impl.cc',
        'disk_cache/mem_backend_impl.h',
        'disk_cache/mem_entry_impl.cc',
        'disk_cache/mem_entry_impl.h',
        'disk_cache/mem_rankings.cc',
        'disk_cache/mem_rankings.h',
        'disk_cache/rankings.cc',
        'disk_cache/rankings.h',
        'disk_cache/sparse_control.cc',
        'disk_cache/sparse_control.h',
        'disk_cache/stats.cc',
        'disk_cache/stats.h',
        'disk_cache/stats_histogram.cc',
        'disk_cache/stats_histogram.h',
        'disk_cache/storage_block-inl.h',
        'disk_cache/storage_block.h',
        'disk_cache/trace.cc',
        'disk_cache/trace.h',
        'ftp/ftp_auth_cache.cc',
        'ftp/ftp_auth_cache.h',
        'ftp/ftp_ctrl_response_buffer.cc',
        'ftp/ftp_ctrl_response_buffer.h',
        'ftp/ftp_network_layer.cc',
        'ftp/ftp_network_layer.h',
        'ftp/ftp_network_session.h',
        'ftp/ftp_network_transaction.cc',
        'ftp/ftp_network_transaction.h',
        'ftp/ftp_request_info.h',
        'ftp/ftp_response_info.h',
        'ftp/ftp_server_type_histograms.cc',
        'ftp/ftp_server_type_histograms.h',
        'ftp/ftp_transaction.h',
        'ftp/ftp_transaction_factory.h',
        'ftp/ftp_util.cc',
        'ftp/ftp_util.h',
        'http/des.cc',
        'http/des.h',
        'http/http_atom_list.h',
        'http/http_auth.cc',
        'http/http_auth.h',
        'http/http_auth_cache.cc',
        'http/http_auth_cache.h',
        'http/http_auth_handler.h',
        'http/http_auth_handler.cc',
        'http/http_auth_handler_basic.cc',
        'http/http_auth_handler_basic.h',
        'http/http_auth_handler_digest.cc',
        'http/http_auth_handler_digest.h',
        'http/http_auth_handler_ntlm.cc',
        'http/http_auth_handler_ntlm.h',
        'http/http_auth_handler_ntlm_portable.cc',
        'http/http_auth_handler_ntlm_win.cc',
        'http/http_basic_stream.h',
        'http/http_byte_range.cc',
        'http/http_byte_range.h',
        'http/http_cache.cc',
        'http/http_cache.h',
        'http/http_chunked_decoder.cc',
        'http/http_chunked_decoder.h',
        'http/http_network_layer.cc',
        'http/http_network_layer.h',
        'http/http_network_session.cc',
        'http/http_network_session.h',
        'http/http_network_transaction.cc',
        'http/http_network_transaction.h',
        'http/http_request_info.h',
        'http/http_response_headers.cc',
        'http/http_response_headers.h',
        'http/http_response_info.cc',
        'http/http_response_info.h',
        'http/http_stream.h',
        'http/http_transaction.h',
        'http/http_transaction_factory.h',
        'http/http_util.cc',
        'http/http_util_icu.cc',
        'http/http_util.h',
        'http/http_vary_data.cc',
        'http/http_vary_data.h',
        'http/http_version.h',
        'http/md4.cc',
        'http/md4.h',
        'http/partial_data.cc',
        'http/partial_data.h',
        'ocsp/nss_ocsp.cc',
        'ocsp/nss_ocsp.h',
        'proxy/init_proxy_resolver.cc',
        'proxy/init_proxy_resolver.h',
        'proxy/proxy_config.cc',
        'proxy/proxy_config.h',
        'proxy/proxy_config_service.h',
        'proxy/proxy_config_service_fixed.h',
        'proxy/proxy_config_service_linux.cc',
        'proxy/proxy_config_service_linux.h',
        'proxy/proxy_config_service_win.cc',
        'proxy/proxy_config_service_win.h',
        'proxy/proxy_info.cc',
        'proxy/proxy_info.h',
        'proxy/proxy_list.cc',
        'proxy/proxy_list.h',
        'proxy/proxy_resolver.h',
        'proxy/proxy_resolver_js_bindings.cc',
        'proxy/proxy_resolver_js_bindings.h',
        'proxy/proxy_resolver_mac.cc',
        'proxy/proxy_resolver_mac.h',
        'proxy/proxy_resolver_script.h',
        'proxy/proxy_resolver_v8.cc',
        'proxy/proxy_resolver_v8.h',
        'proxy/proxy_resolver_winhttp.cc',
        'proxy/proxy_resolver_winhttp.h',
        'proxy/proxy_retry_info.h',
        'proxy/proxy_script_fetcher.cc',
        'proxy/proxy_script_fetcher.h',
        'proxy/proxy_server.cc',
        'proxy/proxy_server.h',
        'proxy/proxy_service.cc',
        'proxy/proxy_service.h',
        'proxy/single_threaded_proxy_resolver.cc',
        'proxy/single_threaded_proxy_resolver.h',
        'socket/client_socket.cc',
        'socket/client_socket.h',
        'socket/client_socket_factory.cc',
        'socket/client_socket_factory.h',
        'socket/client_socket_handle.cc',
        'socket/client_socket_handle.h',
        'socket/client_socket_pool.h',
        'socket/client_socket_pool_base.cc',
        'socket/client_socket_pool_base.h',
        'socket/socket.h',
        'socket/socks5_client_socket.cc',
        'socket/socks5_client_socket.h',
        'socket/socks_client_socket.cc',
        'socket/socks_client_socket.h',
        'socket/ssl_client_socket.h',
        'socket/ssl_client_socket_mac.cc',
        'socket/ssl_client_socket_mac.h',
        'socket/ssl_client_socket_nss.cc',
        'socket/ssl_client_socket_nss.h',
        'socket/ssl_client_socket_win.cc',
        'socket/ssl_client_socket_win.h',
        'socket/ssl_test_util.cc',
        'socket/ssl_test_util.h',
        'socket/tcp_client_socket.h',
        'socket/tcp_client_socket_libevent.cc',
        'socket/tcp_client_socket_libevent.h',
        'socket/tcp_client_socket_pool.cc',
        'socket/tcp_client_socket_pool.h',
        'socket/tcp_client_socket_win.cc',
        'socket/tcp_client_socket_win.h',
        'socket/tcp_pinger.h',
        'socket_stream/socket_stream.cc',
        'socket_stream/socket_stream.h',
        'third_party/parseftp/ParseFTPList.cpp',
        'third_party/parseftp/ParseFTPList.h',
        'url_request/url_request.cc',
        'url_request/url_request.h',
        'url_request/url_request_about_job.cc',
        'url_request/url_request_about_job.h',
        'url_request/url_request_context.h',
        'url_request/url_request_error_job.cc',
        'url_request/url_request_error_job.h',
        'url_request/url_request_file_dir_job.cc',
        'url_request/url_request_file_dir_job.h',
        'url_request/url_request_file_job.cc',
        'url_request/url_request_file_job.h',
        'url_request/url_request_filter.cc',
        'url_request/url_request_filter.h',
        'url_request/url_request_ftp_job.cc',
        'url_request/url_request_ftp_job.h',
        'url_request/url_request_http_job.cc',
        'url_request/url_request_http_job.h',
        'url_request/url_request_inet_job.cc',
        'url_request/url_request_inet_job.h',
        'url_request/url_request_job.cc',
        'url_request/url_request_job.h',
        'url_request/url_request_job_manager.cc',
        'url_request/url_request_job_manager.h',
        'url_request/url_request_job_metrics.cc',
        'url_request/url_request_job_metrics.h',
        'url_request/url_request_job_tracker.cc',
        'url_request/url_request_job_tracker.h',
        'url_request/url_request_new_ftp_job.cc',
        'url_request/url_request_new_ftp_job.h',
        'url_request/url_request_redirect_job.cc',
        'url_request/url_request_redirect_job.h',
        'url_request/url_request_simple_job.cc',
        'url_request/url_request_simple_job.h',
        'url_request/url_request_status.h',
        'url_request/url_request_test_job.cc',
        'url_request/url_request_test_job.h',
        'url_request/url_request_view_net_internals_job.cc',
        'url_request/url_request_view_net_internals_job.h',
        'url_request/view_cache_helper.cc',
        'url_request/view_cache_helper.h',
      ],
      'export_dependent_settings': [
        '../base/base.gyp:base',
      ],
      'conditions': [
        ['javascript_engine=="v8"', {
          'dependencies': [
            '../v8/tools/gyp/v8.gyp:v8',
          ],
        }],
        [ 'OS == "linux"', {
          'dependencies': [
            '../build/linux/system.gyp:gconf',
            '../build/linux/system.gyp:gdk',
            '../build/linux/system.gyp:nss',
          ],
        }],
        [ 'OS == "win"', {
            'sources/': [ ['exclude', '_(mac|linux|posix)\\.cc$'] ],
            'sources!': [
              'http/http_auth_handler_ntlm_portable.cc',
              'socket/tcp_client_socket_libevent.cc',
            ],
            'dependencies': [
              'tld_cleanup',
            ],
          },
          {  # else: OS != "win"
            'sources!': [
              'base/wininet_util.cc',
              'base/winsock_init.cc',
              'proxy/proxy_resolver_winhttp.cc',
              'url_request/url_request_ftp_job.cc',
              'url_request/url_request_inet_job.cc',
            ],
          },
        ],
        [ 'OS == "linux"', {
            'sources/': [ ['exclude', '_(mac|win)\\.cc$'] ],
          },
          {  # else: OS != "linux"
            'sources!': [
              'base/nss_memio.c',
              'base/nss_memio.h',
              'base/x509_certificate_nss.cc',
              'ocsp/nss_ocsp.cc',
              'ocsp/nss_ocsp.h',
              'socket/ssl_client_socket_nss.cc',
              'socket/ssl_client_socket_nss.h',
            ],
            # Get U_STATIC_IMPLEMENTATION and -I directories on Linux.
            'dependencies': [
              '../third_party/icu/icu.gyp:icui18n',
              '../third_party/icu/icu.gyp:icuuc',
            ],
          },
        ],
        [ 'OS == "mac"', {
            'sources/': [ ['exclude', '_(linux|win)\\.cc$'] ],
            'link_settings': {
              'libraries': [
                '$(SDKROOT)/System/Library/Frameworks/Security.framework',
                '$(SDKROOT)/System/Library/Frameworks/SystemConfiguration.framework',
              ]
            },
          },
        ],
      ],
    },
    {
      'target_name': 'net_unittests',
      'type': 'executable',
      'dependencies': [
        'net',
        'net_test_support',
        '../base/base.gyp:base',
        '../testing/gtest.gyp:gtest',
        '../third_party/zlib/zlib.gyp:zlib',
        '../third_party/bzip2/bzip2.gyp:bzip2',
      ],
      'msvs_guid': 'E99DA267-BE90-4F45-88A1-6919DB2C7567',
      'sources': [
        'base/address_list_unittest.cc',
        'base/base64_unittest.cc',
        'base/bzip2_filter_unittest.cc',
        'base/cookie_monster_unittest.cc',
        'base/cookie_policy_unittest.cc',
        'base/data_url_unittest.cc',
        'base/directory_lister_unittest.cc',
        'base/dns_util_unittest.cc',
        'base/escape_unittest.cc',
        'base/file_stream_unittest.cc',
        'base/filter_unittest.cc',
        'base/filter_unittest.h',
        'base/gzip_filter_unittest.cc',
        'base/host_cache_unittest.cc',
        'base/host_resolver_impl_unittest.cc',
        'base/load_log_unittest.cc',
        'base/load_log_unittest.h',
        'base/load_log_util_unittest.cc',
        'base/listen_socket_unittest.cc',
        'base/listen_socket_unittest.h',
        'base/mime_sniffer_unittest.cc',
        'base/mime_util_unittest.cc',
        'base/net_util_unittest.cc',
        'base/registry_controlled_domain_unittest.cc',
        'base/run_all_unittests.cc',
        'base/sdch_filter_unittest.cc',
        'base/ssl_client_auth_cache_unittest.cc',
        'base/ssl_config_service_mac_unittest.cc',
        'base/ssl_config_service_win_unittest.cc',
        'base/strict_transport_security_state_unittest.cc',
        'base/telnet_server_unittest.cc',
        'base/test_completion_callback_unittest.cc',
        'base/wininet_util_unittest.cc',
        'base/x509_certificate_unittest.cc',
        'disk_cache/addr_unittest.cc',
        'disk_cache/backend_unittest.cc',
        'disk_cache/bitmap_unittest.cc',
        'disk_cache/block_files_unittest.cc',
        'disk_cache/disk_cache_test_base.cc',
        'disk_cache/disk_cache_test_base.h',
        'disk_cache/entry_unittest.cc',
        'disk_cache/mapped_file_unittest.cc',
        'disk_cache/storage_block_unittest.cc',
        'ftp/ftp_auth_cache_unittest.cc',
        'ftp/ftp_ctrl_response_buffer_unittest.cc',
        'ftp/ftp_network_transaction_unittest.cc',
        'ftp/ftp_util_unittest.cc',
        'http/des_unittest.cc',
        'http/http_auth_cache_unittest.cc',
        'http/http_auth_handler_basic_unittest.cc',
        'http/http_auth_handler_digest_unittest.cc',
        'http/http_auth_unittest.cc',
        'http/http_byte_range_unittest.cc',
        'http/http_cache_unittest.cc',
        'http/http_chunked_decoder_unittest.cc',
        'http/http_network_layer_unittest.cc',
        'http/http_network_transaction_unittest.cc',
        'http/http_response_headers_unittest.cc',
        'http/http_transaction_unittest.cc',
        'http/http_transaction_unittest.h',
        'http/http_util_unittest.cc',
        'http/http_vary_data_unittest.cc',
        'proxy/init_proxy_resolver_unittest.cc',
        'proxy/mock_proxy_resolver.h',
        'proxy/proxy_config_service_linux_unittest.cc',
        'proxy/proxy_config_service_win_unittest.cc',
        'proxy/proxy_config_unittest.cc',
        'proxy/proxy_list_unittest.cc',
        'proxy/proxy_resolver_js_bindings_unittest.cc',
        'proxy/proxy_resolver_v8_unittest.cc',
        'proxy/proxy_script_fetcher_unittest.cc',
        'proxy/proxy_server_unittest.cc',
        'proxy/proxy_service_unittest.cc',
        'proxy/single_threaded_proxy_resolver_unittest.cc',
        'socket/client_socket_pool_base_unittest.cc',
        'socket/socks5_client_socket_unittest.cc',
        'socket/socks_client_socket_unittest.cc',
        'socket/ssl_client_socket_unittest.cc',
        'socket/tcp_client_socket_pool_unittest.cc',
        'socket/tcp_client_socket_unittest.cc',
        'socket/tcp_pinger_unittest.cc',
        'url_request/url_request_unittest.cc',
        'url_request/url_request_unittest.h',
      ],
      'conditions': [
        [ 'OS == "win"', {
            'sources/': [ ['exclude', '_(mac|linux|posix)_unittest\\.cc$'] ],
          },
        ],
        [ 'OS != "win"', {
            'sources!': [
              'base/wininet_util_unittest.cc',
            ],
          },
        ],
        [ 'OS == "linux"', {
            'sources/': [ ['exclude', '_(mac|win)_unittest\\.cc$'] ],
            'dependencies': [
              '../build/linux/system.gyp:gtk',
            ],
            'sources!': [
              'base/sdch_filter_unittest.cc',
            ],
          },
        ],
        [ 'OS == "mac"', {
            'sources/': [ ['exclude', '_(linux|win)_unittest\\.cc$'] ],
          },
        ],
        # This is needed to trigger the dll copy step on windows.
        # TODO(mark): Specifying this here shouldn't be necessary.
        [ 'OS == "win"', {
            'dependencies': [
              '../third_party/icu/icu.gyp:icudata',
            ],
          },
        ],
      ],
    },
    {
      'target_name': 'net_perftests',
      'type': 'executable',
      'dependencies': [
        'net',
        'net_test_support',
        '../base/base.gyp:base',
        '../base/base.gyp:test_support_base',
        '../base/base.gyp:test_support_perf',
        '../testing/gtest.gyp:gtest',
      ],
      'msvs_guid': 'AAC78796-B9A2-4CD9-BF89-09B03E92BF73',
      'sources': [
        'base/cookie_monster_perftest.cc',
        'disk_cache/disk_cache_perftest.cc',
        'proxy/proxy_resolver_perftest.cc',
      ],
      'conditions': [
        # This is needed to trigger the dll copy step on windows.
        # TODO(mark): Specifying this here shouldn't be necessary.
        [ 'OS == "win"', {
            'dependencies': [
              '../third_party/icu/icu.gyp:icudata',
            ],
          },
        ],
      ],
    },
    {
      'target_name': 'stress_cache',
      'type': 'executable',
      'dependencies': [
        'net',
        'net_test_support',
        '../base/base.gyp:base',
      ],
      'sources': [
        'disk_cache/stress_cache.cc',
      ],
    },
    {
      'target_name': 'tld_cleanup',
      'type': 'executable',
      'dependencies': [
        '../base/base.gyp:base',
        '../build/temp_gyp/googleurl.gyp:googleurl',
      ],
      'msvs_guid': 'E13045CD-7E1F-4A41-9B18-8D288B2E7B41',
      'sources': [
        'tools/tld_cleanup/tld_cleanup.cc',
      ],
    },
    {
      'target_name': 'crash_cache',
      'type': 'executable',
      'dependencies': [
        'net',
        'net_test_support',
        '../base/base.gyp:base',
      ],
      'msvs_guid': 'B0EE0599-2913-46A0-A847-A3EC813658D3',
      'sources': [
        'tools/crash_cache/crash_cache.cc',
      ],
    },
    {
      'target_name': 'net_test_support',
      'type': '<(library)',
      'dependencies': [
        'net',
        '../base/base.gyp:base',
        '../testing/gtest.gyp:gtest',
      ],
      'sources': [
        'disk_cache/disk_cache_test_util.cc',
        'disk_cache/disk_cache_test_util.h',
        'proxy/proxy_config_service_common_unittest.cc',
        'proxy/proxy_config_service_common_unittest.h',
        'socket/socket_test_util.cc',
        'socket/socket_test_util.h',
      ],
    },
    {
      'target_name': 'net_resources',
      'type': 'none',
      'msvs_guid': '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942',
      'rules': [
        {
          'rule_name': 'grit',
          'extension': 'grd',
          'inputs': [
            '../tools/grit/grit.py',
          ],
          'outputs': [
            '<(SHARED_INTERMEDIATE_DIR)/net/grit/<(RULE_INPUT_ROOT).h',
            '<(SHARED_INTERMEDIATE_DIR)/net/<(RULE_INPUT_ROOT).rc',
            '<(SHARED_INTERMEDIATE_DIR)/net/<(RULE_INPUT_ROOT).pak',
          ],
          'action':
            ['python', '<@(_inputs)', '-i', '<(RULE_INPUT_PATH)', 'build', '-o', '<(SHARED_INTERMEDIATE_DIR)/net'],
          'message': 'Generating resources from <(RULE_INPUT_PATH)',
        },
      ],
      'sources': [
        'base/net_resources.grd',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)/net',
        ],
      },
      'conditions': [
        ['OS=="win"', {
          'dependencies': ['../build/win/system.gyp:cygwin'],
        }],
      ],
    },
    {
      'target_name': 'fetch_client',
      'type': 'executable',
      'dependencies': [
        'net',
        '../base/base.gyp:base',
        '../base/base.gyp:test_support_perf',
        '../testing/gtest.gyp:gtest',
      ],
      'msvs_guid': 'DABB8796-B9A2-4CD9-BF89-09B03E92B123',
      'sources': [
        'tools/fetch/fetch_client.cc',
      ],
    },
    {
      'target_name': 'fetch_server',
      'type': 'executable',
      'dependencies': [
        'net',
        '../base/base.gyp:base',
        '../base/base.gyp:test_support_perf',
        '../testing/gtest.gyp:gtest',
      ],
      'msvs_guid': 'DABB8796-B9A2-4CD9-BF89-09B03E92B124',
      'sources': [
        'tools/fetch/fetch_server.cc',
        'tools/fetch/http_listen_socket.cc',
        'tools/fetch/http_listen_socket.h',
        'tools/fetch/http_server.cc',
        'tools/fetch/http_server.h',
        'tools/fetch/http_server_request_info.h',
        'tools/fetch/http_server_response_info.h',
        'tools/fetch/http_session.cc',
        'tools/fetch/http_session.h',
      ],
    },
  ],
  'conditions': [
    ['OS=="win"', {
      'targets': [
        {
          # TODO(port): dump_cache is still Windows-specific.
          'target_name': 'dump_cache',
          'type': 'executable',
          'dependencies': [
            'net',
            '../base/base.gyp:base',
          ],
          'sources': [
            'tools/dump_cache/cache_dumper.cc',
            'tools/dump_cache/cache_dumper.h',
            'tools/dump_cache/dump_cache.cc',
            'tools/dump_cache/dump_files.cc',
            'tools/dump_cache/upgrade.cc',
            'tools/dump_cache/url_to_filename_encoder.h',
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
