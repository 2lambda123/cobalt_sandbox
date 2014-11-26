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

#include "cobalt/render_tree/node_visitor.h"

#include "cobalt/render_tree/composition_node.h"
#include "cobalt/render_tree/image_node.h"
#include "cobalt/render_tree/rect_node.h"
#include "cobalt/render_tree/text_node.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using cobalt::render_tree::CompositionNode;
using cobalt::render_tree::Font;
using cobalt::render_tree::Image;
using cobalt::render_tree::ImageNode;
using cobalt::render_tree::CompositionNodeMutable;
using cobalt::render_tree::NodeVisitor;
using cobalt::render_tree::RectNode;
using cobalt::render_tree::TextNode;
using cobalt::render_tree::Brush;

class MockNodeVisitor : public NodeVisitor {
 public:
  MOCK_METHOD1(Visit, void(CompositionNode* composition));
  MOCK_METHOD1(Visit, void(ImageNode* image));
  MOCK_METHOD1(Visit, void(RectNode* rect));
  MOCK_METHOD1(Visit, void(TextNode* text));
};

TEST(NodeVisitorTest, VisitsComposition) {
  scoped_refptr<CompositionNode> composition(
      new CompositionNode(make_scoped_ptr(new CompositionNodeMutable())));
  MockNodeVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(composition.get()));
  composition->Accept(&mock_visitor);
}

namespace {

class DummyImage : public Image {
  int GetWidth() const OVERRIDE { return 0; }
  int GetHeight() const OVERRIDE { return 0; }
};

}  // namespace

TEST(NodeVisitorTest, VisitsImage) {
  scoped_refptr<DummyImage> image = make_scoped_refptr(new DummyImage());
  scoped_refptr<ImageNode> image_node(new ImageNode(image));
  MockNodeVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(image_node.get()));
  image_node->Accept(&mock_visitor);
}

TEST(NodeVisitorTest, VisitsRect) {
  scoped_refptr<RectNode> rect(
      new RectNode(cobalt::math::SizeF(), scoped_ptr<Brush>()));
  MockNodeVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(rect.get()));
  rect->Accept(&mock_visitor);
}

namespace {

class DummyFont : public Font {
 public:
  cobalt::math::SizeF GetBounds(const std::string& text) const OVERRIDE {
    return cobalt::math::SizeF();
  }
};

}  // namespace

TEST(NodeVisitorTest, VisitsText) {
  scoped_refptr<TextNode> text(
      new TextNode("foobar", make_scoped_refptr(new DummyFont())));
  MockNodeVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, Visit(text.get()));
  text->Accept(&mock_visitor);
}
