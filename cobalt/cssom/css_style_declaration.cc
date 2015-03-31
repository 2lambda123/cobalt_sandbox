/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/cssom/css_style_declaration.h"

#include "base/string_util.h"
#include "cobalt/base/source_location.h"
#include "cobalt/cssom/css_parser.h"
#include "cobalt/cssom/mutation_observer.h"
#include "cobalt/cssom/property_names.h"

namespace cobalt {
namespace cssom {

namespace {
const base::SourceLocation kSourceLocation =
    base::SourceLocation("[object CSSStyleDeclaration]", 1, 1);
}  // namespace

CSSStyleDeclaration::CSSStyleDeclaration(cssom::CSSParser* css_parser)
    : css_parser_(css_parser), mutation_observer_(NULL) {}

CSSStyleDeclaration::~CSSStyleDeclaration() {}

scoped_refptr<PropertyValue> CSSStyleDeclaration::GetPropertyValue(
    const std::string& property_name) {
  switch (property_name.size()) {
    case 5:
      if (LowerCaseEqualsASCII(property_name, kColorPropertyName)) {
        return color();
      }
      if (LowerCaseEqualsASCII(property_name, kWidthPropertyName)) {
        return width();
      }
      return NULL;

    case 6:
      if (LowerCaseEqualsASCII(property_name, kHeightPropertyName)) {
        return height();
      }
      return NULL;

    case 7:
      if (LowerCaseEqualsASCII(property_name, kDisplayPropertyName)) {
        return display();
      }
      if (LowerCaseEqualsASCII(property_name, kOpacityPropertyName)) {
        return opacity();
      }
      return NULL;

    case 8:
      if (LowerCaseEqualsASCII(property_name, kOverflowPropertyName)) {
        return overflow();
      }
      return NULL;

    case 9:
      if (LowerCaseEqualsASCII(property_name, kFontSizePropertyName)) {
        return font_size();
      }
      if (LowerCaseEqualsASCII(property_name, kTransformPropertyName)) {
        return transform();
      }
      return NULL;

    case 10:
      if (LowerCaseEqualsASCII(property_name, kBackgroundPropertyName)) {
        return background();
      }
      return NULL;

    case 11:
      if (LowerCaseEqualsASCII(property_name, kFontFamilyPropertyName)) {
        return font_family();
      }
      if (LowerCaseEqualsASCII(property_name, kFontWeightPropertyName)) {
        return font_weight();
      }
      return NULL;

    case 13:
      if (LowerCaseEqualsASCII(property_name, kBorderRadiusPropertyName)) {
        return border_radius();
      }
      return NULL;

    case 16:
      if (LowerCaseEqualsASCII(property_name, kBackgroundColorPropertyName)) {
        return background_color();
      }
      if (LowerCaseEqualsASCII(property_name, kBackgroundImagePropertyName)) {
        return background_image();
      }
      return NULL;

    default:
      return NULL;
  }
}

void CSSStyleDeclaration::SetPropertyValue(
    const std::string& property_name,
    const scoped_refptr<PropertyValue>& property_value) {
  switch (property_name.size()) {
    case 5:
      if (LowerCaseEqualsASCII(property_name, kColorPropertyName)) {
        set_color(property_value);
      } else if (LowerCaseEqualsASCII(property_name, kWidthPropertyName)) {
        set_width(property_value);
      }
      break;

    case 6:
      if (LowerCaseEqualsASCII(property_name, kHeightPropertyName)) {
        set_height(property_value);
      }
      break;

    case 7:
      if (LowerCaseEqualsASCII(property_name, kDisplayPropertyName)) {
        set_display(property_value);
      } else if (LowerCaseEqualsASCII(property_name, kOpacityPropertyName)) {
        set_opacity(property_value);
      }
      break;

    case 8:
      if (LowerCaseEqualsASCII(property_name, kOverflowPropertyName)) {
        set_overflow(property_value);
      }
      break;

    case 9:
      if (LowerCaseEqualsASCII(property_name, kFontSizePropertyName)) {
        set_font_size(property_value);
      } else if (LowerCaseEqualsASCII(property_name, kTransformPropertyName)) {
        set_transform(property_value);
      }
      break;

    case 10:
      if (LowerCaseEqualsASCII(property_name, kBackgroundPropertyName)) {
        set_background(property_value);
      }
      break;

    case 11:
      if (LowerCaseEqualsASCII(property_name, kFontFamilyPropertyName)) {
        set_font_family(property_value);
      } else if (LowerCaseEqualsASCII(property_name, kFontWeightPropertyName)) {
        set_font_weight(property_value);
      }
      break;

    case 13:
      if (LowerCaseEqualsASCII(property_name, kBorderRadiusPropertyName)) {
        set_border_radius(property_value);
      }
      break;

    case 16:
      if (LowerCaseEqualsASCII(property_name, kBackgroundColorPropertyName)) {
        set_background_color(property_value);
      } else if (LowerCaseEqualsASCII(property_name,
                                      kBackgroundImagePropertyName)) {
        set_background_image(property_value);
      }
      break;

    default:
      // 3. If property is not a case-sensitive match for a supported CSS
      // property, terminate this algorithm.
      //   http://dev.w3.org/csswg/cssom/#dom-cssstyledeclaration-setproperty
      break;
  }
}

// TODO(***REMOVED***): The getter of css_text returns the result of serializing the
// declarations, which is not required for Performance Spike. This should be
// handled propertly afterwards.
const std::string CSSStyleDeclaration::css_text() const {
  NOTREACHED();
  return NULL;
}

void CSSStyleDeclaration::set_css_text(const std::string& css_text) {
  scoped_refptr<CSSStyleDeclaration> declaration =
      css_parser_->ParseDeclarationList(css_text, kSourceLocation);
  if (declaration) {
    AssignFrom(*declaration.get());

    if (mutation_observer_) {
      // Trigger layout update.
      mutation_observer_->OnMutation();
    }
  }
}

void CSSStyleDeclaration::AssignFrom(const CSSStyleDeclaration& rhs) {
  set_background(rhs.background());
  set_background_color(rhs.background_color());
  set_background_image(rhs.background_image());
  set_border_radius(rhs.border_radius());
  set_color(rhs.color());
  set_display(rhs.display());
  set_font_family(rhs.font_family());
  set_font_size(rhs.font_size());
  set_font_weight(rhs.font_weight());
  set_height(rhs.height());
  set_opacity(rhs.opacity());
  set_overflow(rhs.overflow());
  set_transform(rhs.transform());
  set_width(rhs.width());
}

}  // namespace cssom
}  // namespace cobalt
