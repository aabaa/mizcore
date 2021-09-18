#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "ast_component.hpp"
#include "ast_type.hpp"

namespace mizcore {

class Token;
class KeywordToken;

class ASTBlock : public ASTComponent
{
  public:
    ASTBlock(BLOCK_TYPE type)
      : block_type_(type)
    {}
    ~ASTBlock() override = default;
    ASTBlock(ASTBlock const&) = delete;
    ASTBlock(ASTBlock&&) = delete;
    ASTBlock& operator=(ASTBlock const&) = delete;
    ASTBlock& operator=(ASTBlock&&) = delete;

    ELEMENT_TYPE GetElementType() const override { return ELEMENT_TYPE::BLOCK; }

    BLOCK_TYPE GetBlockType() const { return block_type_; }
    void SetBlockType(BLOCK_TYPE block_type) { block_type_ = block_type; }

    Token* GetFirstToken() const { return first_token_; }
    void SetFirstToken(Token* token) { first_token_ = token; }
    Token* GetEndToken() const { return end_token_; }
    void SetEndToken(Token* token) { end_token_ = token; }
    Token* GetSemicolonToken() const { return semicolon_token_; }
    void SetSemicolonToken(Token* token) { semicolon_token_ = token; }

    Token* GetRangeStartToken() const override { return first_token_; }
    Token* GetRangeEndToken() const override
    {
        return semicolon_token_ == nullptr ? end_token_ : semicolon_token_;
    }

    size_t GetChildComponentNum() const { return child_components_.size(); }
    ASTComponent* GetChildComponent(size_t i) const
    {
        return child_components_[i].get();
    }
    void AddChildComponent(std::unique_ptr<ASTComponent>&& component)
    {
        component->SetParent(this);
        child_components_.push_back(std::move(component));
    }

  private:
    BLOCK_TYPE block_type_ = BLOCK_TYPE::UNKNOWN;
    Token* first_token_ = nullptr;
    Token* end_token_ = nullptr;
    Token* semicolon_token_ = nullptr;
    std::vector<std::unique_ptr<ASTComponent>> child_components_;
};

} // namespace mizcore
