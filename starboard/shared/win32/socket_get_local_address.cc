// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/socket.h"

#include <winsock2.h>

#include "starboard/log.h"
#include "starboard/shared/win32/socket_internal.h"

namespace sbwin32 = starboard::shared::win32;

bool SbSocketGetLocalAddress(SbSocket socket, SbSocketAddress* out_address) {
  if (!SbSocketIsValid(socket)) {
    return false;
  }

  SB_DCHECK(socket->socket_handle != INVALID_SOCKET);
  sbwin32::SockAddr sock_addr;
  int result = getsockname(socket->socket_handle, sock_addr.sockaddr(),
                           &sock_addr.length);
  if (result == SOCKET_ERROR) {
    int last_error = WSAGetLastError();
    SB_LOG(ERROR) << "getsockname() failed with last_error = " << last_error;
    socket->error = sbwin32::TranslateSocketErrorStatus(last_error);
    return false;
  }
  if (!sock_addr.ToSbSocketAddress(out_address)) {
    socket->error = kSbSocketErrorFailed;
    return false;
  }

  socket->error = kSbSocketOk;
  return true;
}
