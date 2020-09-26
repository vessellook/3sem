#pragma once

#include <memory>
#include <utility>

#include "token.h"
#include "../linked_list.h"

class TreeNode {
private:
    Token token_;
    std::shared_ptr<TreeNode> parent_;
    std::shared_ptr<art::SinglyLinkedList<std::shared_ptr<TreeNode>>> children_;
public:
    explicit TreeNode(Token token): token_(std::move(token)), parent_(nullptr), children_(new art::SinglyLinkedList<std::shared_ptr<TreeNode>>()) {}
    Token getToken() const { return token_; }
    void setToken(Token token) { token_ = std::move(token); }
    std::shared_ptr<TreeNode> getParent() const { return parent_; }
    void setParent(const std::shared_ptr<TreeNode>& parent) { parent_ = parent; }
    std::shared_ptr<art::SinglyLinkedList<std::shared_ptr<TreeNode>>> getChildren() { return children_; }
    void addChild(std::shared_ptr<TreeNode> child) { children_->append(std::move(child)); }
    TreeNode* removeLastChild() { children_->remove(0); return this; }
    std::shared_ptr<TreeNode> getLastChild() { return children_->get(0); }
};