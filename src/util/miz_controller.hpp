#pragma once

#include <memory>
#include <vector>

namespace mizcore {

class ASTBlock;
class ASTToken;
class SymbolTable;
class TokenTable;
class ErrorTable;

class MizController
{
  public:
    MizController() = default;
    virtual ~MizController() = default;
    MizController(MizController const&) = delete;
    MizController(MizController&&) = delete;
    MizController& operator=(MizController const&) = delete;
    MizController& operator=(MizController&&) = delete;

    void ExecImpl(std::istream& ifs_miz, const char* vctpath);
    void ExecFile(const char* mizpath, const char* vctpath);
    void ExecBuffer(const char* buffer, const char* vctpath);
    std::shared_ptr<TokenTable> GetTokenTable() const { return token_table_; }
    std::shared_ptr<ASTBlock> GetASTRoot() const { return ast_root_; }
    std::shared_ptr<ErrorTable> GetErrorTable() const { return error_table_; }
    bool IsABSMode() const { return is_abs_mode_; }
    void SetABSMode(bool is_abs_mode) { is_abs_mode_ = is_abs_mode; }

    bool CheckIsSeparableTokens(const std::vector<ASTToken*>& tokens) const;

  private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::shared_ptr<TokenTable> token_table_;
    std::shared_ptr<ASTBlock> ast_root_;
    std::shared_ptr<ErrorTable> error_table_;
    bool is_abs_mode_ = false;
};

} // namespace mizcore
