#include <filesystem>
#include <fstream>
#include <iostream>

#include "ast_block.hpp"
#include "ast_token.hpp"
#include "doctest/doctest.h"
#include "file_handling_tools.hpp"
#include "miz_controller.hpp"
#include "token_table.hpp"


using mizcore::MizController;
using mizcore::ASTToken;
namespace fs = std::filesystem;

namespace {

const fs::path&
TEST_DIR()
{
    static fs::path test_dir = fs::path(__FILE__).parent_path();
    return test_dir;
}

} // namespace

void test_miz_controller(MizController& miz_controller)
{
  if (!fs::exists(TEST_DIR() / "result")) {
        fs::create_directory(TEST_DIR() / "result");
    }
    fs::path result_file_path = TEST_DIR() / "result" / "numerals_blocks.json";
    nlohmann::json json;
    auto ast_root = miz_controller.GetASTRoot();
    ast_root->ToJson(json);
    mizcore::write_json_file(json, result_file_path);
    fs::path expected_file_path =
      TEST_DIR() / "expected" / "numerals_blocks.json";

    auto json_diff =
      mizcore::json_file_diff(result_file_path, expected_file_path);

    CHECK(json_diff.empty());
    if (!json_diff.empty()) {
        fs::path diff_file_path =
          TEST_DIR() / "result" / "numerals_blocks_diff.json";
        mizcore::write_json_file(json_diff, diff_file_path);
    } else {
        remove(result_file_path.string().c_str());
    }

}

TEST_CASE("test miz_controller ExecFile")
{
    mizcore::MizController miz_controller;
    auto mizpath = TEST_DIR() / "data" / "numerals.miz";
    auto vctpath = TEST_DIR().parent_path() / "parser" / "data" / "mml.vct";
    miz_controller.ExecFile(mizpath.string().c_str(), vctpath.string().c_str());
    test_miz_controller(miz_controller);
}

TEST_CASE("test miz_controller ExecBuffer")
{
    auto mizpath = TEST_DIR() / "data" / "numerals.miz";
    auto vctpath = TEST_DIR().parent_path() / "parser" / "data" / "mml.vct";
    mizcore::MizController miz_controller;
    std::ifstream ifs_miz(mizpath);
    CHECK(ifs_miz.good());
    // Convert path to std::string
    std::string str_text = std::string((std::istreambuf_iterator<char>(ifs_miz)),
                            std::istreambuf_iterator<char>());
    miz_controller.ExecBuffer(str_text.c_str(), vctpath.string().c_str());
    test_miz_controller(miz_controller);
}

TEST_CASE("test miz_controller CheckIsSeparableTokens")
{
    auto mizpath = TEST_DIR() / "data" / "numerals.miz";
    auto vctpath = TEST_DIR().parent_path() / "parser" / "data" / "mml.vct";
    mizcore::MizController miz_controller;
    miz_controller.ExecFile(mizpath.string().c_str(), vctpath.string().c_str());
    auto token_table = miz_controller.GetTokenTable();

    auto* t1 = token_table->GetToken(50); // {}
    auto* t2 = token_table->GetToken(51); // is
    auto* t3 = token_table->GetToken(52); // Element

    std::vector<ASTToken*> v1 = {t1, t2};
    CHECK(miz_controller.CheckIsSeparableTokens(v1) == true);

    std::vector<ASTToken*> v2 = {t1, t2, t3};
    CHECK(miz_controller.CheckIsSeparableTokens(v2) == false);
}
