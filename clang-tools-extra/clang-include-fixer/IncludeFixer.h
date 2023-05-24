//===-- IncludeFixer.h - Include inserter -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INCLUDEFIXER_H
#define LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INCLUDEFIXER_H

#include "IncludeFixerContext.h"
#include "SymbolIndexManager.h"
#include "clang/Format/Format.h"
#include "clang/Sema/ExternalSemaSource.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <vector>

namespace clang {

class CompilerInvocation;
class DiagnosticConsumer;
class FileManager;
class PCHContainerOperations;

namespace include_fixer {

class IncludeFixerActionFactory : public clang::tooling::ToolAction {
public:
  /// \param SymbolIndexMgr A source for matching symbols to header files.
  /// \param Contexts The contexts for the symbols being queried.
  /// \param StyleName Fallback style for reformatting.
  /// \param MinimizeIncludePaths whether inserted include paths are optimized.
  IncludeFixerActionFactory(SymbolIndexManager &SymbolIndexMgr,
                            std::vector<IncludeFixerContext> &Contexts,
                            StringRef StyleName,
                            bool MinimizeIncludePaths = true);

  ~IncludeFixerActionFactory() override;

  bool
  runInvocation(std::shared_ptr<clang::CompilerInvocation> Invocation,
                clang::FileManager *Files,
                std::shared_ptr<clang::PCHContainerOperations> PCHContainerOps,
                clang::DiagnosticConsumer *Diagnostics) override;

private:
  /// The client to use to find cross-references.
  SymbolIndexManager &SymbolIndexMgr;

  /// Multiple contexts for files being processed.
  std::vector<IncludeFixerContext> &Contexts;

  /// Whether inserted include paths should be optimized.
  bool MinimizeIncludePaths;

  /// The fallback format style for formatting after insertion if no
  /// clang-format config file was found.
  std::string FallbackStyle;
};

/// Create replacements, which are generated by clang-format, for the
/// missing header and mising qualifiers insertions. The function uses the
/// first header for insertion.
///
/// \param Code The source code.
/// \param Context The context which contains all information for creating
/// clang-include-fixer replacements.
/// \param Style clang-format style being used.
/// \param AddQualifiers  Whether we should add qualifiers to all instances of
/// an unidentified symbol.
///
/// \return Formatted replacements for inserting, sorting headers and adding
/// qualifiers on success; otherwise, an llvm::Error carrying llvm::StringError
/// is returned.
llvm::Expected<tooling::Replacements> createIncludeFixerReplacements(
    StringRef Code, const IncludeFixerContext &Context,
    const format::FormatStyle &Style = format::getLLVMStyle(),
    bool AddQualifiers = true);

/// Handles callbacks from sema, does the include lookup and turns it into an
/// IncludeFixerContext.
class IncludeFixerSemaSource : public clang::ExternalSemaSource {
public:
  explicit IncludeFixerSemaSource(SymbolIndexManager &SymbolIndexMgr,
                                  bool MinimizeIncludePaths,
                                  bool GenerateDiagnostics)
      : SymbolIndexMgr(SymbolIndexMgr),
        MinimizeIncludePaths(MinimizeIncludePaths),
        GenerateDiagnostics(GenerateDiagnostics) {}

  void setCompilerInstance(CompilerInstance *CI) { this->CI = CI; }
  void setFilePath(StringRef FilePath) { this->FilePath = FilePath; }

  /// Callback for incomplete types. If we encounter a forward declaration we
  /// have the fully qualified name ready. Just query that.
  bool MaybeDiagnoseMissingCompleteType(clang::SourceLocation Loc,
                                        clang::QualType T) override;

  /// Callback for unknown identifiers. Try to piece together as much
  /// qualification as we can get and do a query.
  clang::TypoCorrection CorrectTypo(const DeclarationNameInfo &Typo,
                                    int LookupKind, Scope *S, CXXScopeSpec *SS,
                                    CorrectionCandidateCallback &CCC,
                                    DeclContext *MemberContext,
                                    bool EnteringContext,
                                    const ObjCObjectPointerType *OPT) override;

  /// Get the minimal include for a given path.
  std::string minimizeInclude(StringRef Include,
                              const clang::SourceManager &SourceManager,
                              clang::HeaderSearch &HeaderSearch) const;

  /// Get the include fixer context for the queried symbol.
  IncludeFixerContext getIncludeFixerContext(
      const clang::SourceManager &SourceManager,
      clang::HeaderSearch &HeaderSearch,
      ArrayRef<find_all_symbols::SymbolInfo> MatchedSymbols) const;

  /// Get the global matched symbols.
  ArrayRef<find_all_symbols::SymbolInfo> getMatchedSymbols() const {
    return MatchedSymbols;
  }

private:
  /// Query the database for a given identifier.
  std::vector<find_all_symbols::SymbolInfo>
  query(StringRef Query, StringRef ScopedQualifiers, tooling::Range Range);

  CompilerInstance *CI;

  /// The client to use to find cross-references.
  SymbolIndexManager &SymbolIndexMgr;

  /// The information of the symbols being queried.
  std::vector<IncludeFixerContext::QuerySymbolInfo> QuerySymbolInfos;

  /// All symbol candidates which match QuerySymbol. We only include the first
  /// discovered identifier to avoid getting caught in results from error
  /// recovery.
  std::vector<find_all_symbols::SymbolInfo> MatchedSymbols;

  /// The file path to the file being processed.
  std::string FilePath;

  /// Whether we should use the smallest possible include path.
  bool MinimizeIncludePaths = true;

  /// Whether we should generate diagnostics with fixits for missing symbols.
  bool GenerateDiagnostics = false;
};
} // namespace include_fixer
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_INCLUDE_FIXER_INCLUDEFIXER_H
