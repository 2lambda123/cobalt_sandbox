// Copyright 2019 The Cobalt Authors. All Rights Reserved.
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

#ifndef COBALT_UI_NAVIGATION_NAV_ITEM_H_
#define COBALT_UI_NAVIGATION_NAV_ITEM_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "cobalt/ui_navigation/interface.h"
#include "starboard/ui_navigation.h"

namespace cobalt {
namespace ui_navigation {

// This wraps a SbUiNavItem to make it refcounted.
class NavItem : public base::RefCountedThreadSafe<NavItem> {
 public:
  NavItem(SbUiNavItemType type,
          const base::Closure& onblur_callback,
          const base::Closure& onfocus_callback,
          const base::Closure& onscroll_callback);

  SbUiNavItemType GetType() const {
    return nav_item_type_;
  }

  bool IsContainer() const {
    return nav_item_type_ == kSbUiNavItemTypeContainer;
  }

  void RegisterRootContainerWithWindow(SbWindow window) {
    GetInterface().register_root_container_with_window(nav_item_, window);
  }

  void Focus() {
    GetInterface().set_focus(nav_item_);
  }

  void SetEnabled(bool enabled) {
    GetInterface().set_item_enabled(nav_item_, enabled);
  }

  void SetSize(float width, float height) {
    GetInterface().set_item_size(nav_item_, width, height);
  }

  void SetPosition(float x, float y) {
    GetInterface().set_item_position(nav_item_, x, y);
  }

  bool GetLocalTransform(SbUiNavTransform* out_transform) {
    return GetInterface().get_item_local_transform(nav_item_, out_transform);
  }

  bool RegisterContent(const scoped_refptr<NavItem>& content) {
    return GetInterface().register_item_content(nav_item_,
                                                  content->nav_item_);
  }

  void UnregisterAsContent() {
    GetInterface().unregister_item_as_content(nav_item_);
  }

  void SetContentOffset(float x, float y) {
    GetInterface().set_item_content_offset(nav_item_, x, y);
  }

  void GetContentOffset(float* out_x, float* out_y) {
    GetInterface().get_item_content_offset(nav_item_, out_x, out_y);
  }

 private:
  friend class base::RefCountedThreadSafe<NavItem>;
  ~NavItem();

  static void OnBlur(SbUiNavItem item, void* callback_context);
  static void OnFocus(SbUiNavItem item, void* callback_context);
  static void OnScroll(SbUiNavItem item, void* callback_context);

  base::Closure onblur_callback_;
  base::Closure onfocus_callback_;
  base::Closure onscroll_callback_;

  SbUiNavItemType nav_item_type_;
  SbUiNavItem nav_item_;

  static SbUiNavCallbacks s_callbacks_;
};

}  // namespace ui_navigation
}  // namespace cobalt

#endif  // COBALT_UI_NAVIGATION_NAV_ITEM_H_
