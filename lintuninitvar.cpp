/*
 * LINTUNINITVAR - LINT tool made by Daniel Marjamäki
 * Copyright (C) 2007-2013 Daniel Marjamäki
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//---------------------------------------------------------------------------
#include "mathlib.h"
#include "symboldatabase.h"
#include "checknullpointer.h"
#include "config.h"
#include "check.h"
#include "settings.h"
#include <cassert>

//---------------------------------------------------------------------------

class Token;
class Scope;
class Variable;

class CPPCHECKLIB LintUninitVar : public Check {
public:
    /** @brief This constructor is used when registering the LintUninitVar */
    LintUninitVar() : Check(myName())
    { }

    /** @brief This constructor is used when running checks. */
    LintUninitVar(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger)
    { }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        LintUninitVar lintUninitVar(tokenizer, settings, errorLogger);
        lintUninitVar.check();
    }

    /** Check for uninitialized variables */
    void check();
    void checkScope(const Scope* scope);
    bool checkScopeForVariable(const Scope* scope, const Token *tok, const Variable& var, const std::string &membervar);
    bool checkIfForWhileHead(const Token *startparentheses, const Variable& var, const std::string &membervar);
    bool checkLoopBody(const Token *tok, const Variable& var, const std::string &membervar);
    static bool isVariableUsage(const Token *vartok, bool ispointer, bool cpp);
    bool isMemberVariableAssignment(const Token *tok, const std::string &membervar) const;
    bool isMemberVariableUsage(const Token *tok, bool isPointer, const std::string &membervar) const;

    void uninitstringError(const Token *tok, const std::string &varname, bool strncpy_);
    void uninitdataError(const Token *tok, const std::string &varname);
    void uninitvarError(const Token *tok, const std::string &varname);
    void uninitStructMemberError(const Token *tok, const std::string &membername);

private:
    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        LintUninitVar c(0, settings, errorLogger);

        // error
        c.uninitstringError(0, "varname", true);
        c.uninitdataError(0, "varname");
        c.uninitvarError(0, "varname");
        c.uninitStructMemberError(0, "a.b");
    }

    static std::string myName() {
        return "LINT: Uninitialized variables";
    }

    std::string classInfo() const {
        return "Lint for uninitialized variables\n"
               "* using uninitialized variables and data\n";
    }
};

//---------------------------------------------------------------------------

// Register this check class (by creating a static instance of it)
namespace {
    LintUninitVar instance;
}

//---------------------------------------------------------------------------

void LintUninitVar::check()
{
    const SymbolDatabase *symbolDatabase = _tokenizer->getSymbolDatabase();
    std::list<Scope>::const_iterator scope;

    // check every executable scope
    for (scope = symbolDatabase->scopeList.begin(); scope != symbolDatabase->scopeList.end(); ++scope) {
        if (scope->isExecutable()) {
            checkScope(&*scope);
        }
    }
}

void LintUninitVar::checkScope(const Scope* scope)
{
    for (std::list<Variable>::const_iterator i = scope->varlist.begin(); i != scope->varlist.end(); ++i) {
/*
        if ((_tokenizer->isCPP() && i->type() && !i->isPointer() && i->type()->needInitialization != Type::True) ||
            i->isStatic() || i->isExtern() || i->isConst() || i->isArray() || i->isReference())
            continue;
*/
        if (i->nameToken()->strAt(1) == "(")
            continue;

        const Token* tok = i->typeEndToken();

        while (tok && tok->str() != ";")
            tok = tok->next();

        checkScopeForVariable(scope, tok, *i, "");

        if (Token::Match(i->typeStartToken(), "struct %type% %var% ;")) {
            const std::string structname(i->typeStartToken()->next()->str());
            const SymbolDatabase * symbolDatabase = _tokenizer->getSymbolDatabase();
            for (std::size_t j = 0U; j < symbolDatabase->classAndStructScopes.size(); ++j) {
                const Scope *scope2 = symbolDatabase->classAndStructScopes[j];
                if (scope2->className == structname && scope2->numConstructors == 0U) {
                    for (std::list<Variable>::const_iterator it = scope2->varlist.begin(); it != scope2->varlist.end(); ++it) {
                        const Variable &var = *it;
                        if (!var.isArray())
                            checkScopeForVariable(scope, tok, *i, var.name());
                    }
                }
            }
        }
    }
}

bool LintUninitVar::checkScopeForVariable(const Scope* scope, const Token *tok, const Variable& var, const std::string &membervar)
{
    for (; tok; tok = tok->next()) {
        // End of scope..
        if (tok->str() == "}") {
            break;
        }

        // Unconditional inner scope..
        if (tok->str() == "{" && Token::Match(tok->previous(), "[;{}=]")) {
            if (checkScopeForVariable(scope, tok->next(), var, membervar))
                return true;
            tok = tok->link();
            continue;
        }

        // Inner scope..
        if (Token::simpleMatch(tok, "if (")) {
            // initialization / usage in condition..
            if (checkIfForWhileHead(tok->next(), var, membervar))
                return true;

            // goto the {
            tok = tok->next()->link()->next();

            if (!tok)
                break;
            if (tok->str() == "{") {
                const bool initif = checkScopeForVariable(scope, tok->next(), var, membervar);

                // goto the }
                tok = tok->link();

                if (Token::simpleMatch(tok, "} else {")) {
                    // goto the {
                    tok = tok->tokAt(2);

                    const bool initelse = checkScopeForVariable(scope, tok->next(), var, membervar);

                    // goto the }
                    tok = tok->link();

                    if (initif && initelse)
                        return true;
                }
            }
        }

        // skip sizeof / offsetof
        if (Token::Match(tok, "sizeof|typeof|offsetof|decltype ("))
            tok = tok->next()->link();

        // for/while..
        if (Token::Match(tok, "for|while (")) {
            // is variable initialized in for-head (don't report errors yet)?
            if (checkIfForWhileHead(tok->next(), var, membervar))
                return true;

            // goto the {
            tok = tok->next()->link()->next();

            checkLoopBody(tok, var, membervar);

            // goto the }
            tok = tok->link();
        }

        if (Token::Match(tok, "return|break|continue|throw|goto")) {
            while (tok && tok->str() != ";") {
                // variable is seen..
                if (tok->varId() == var.varId()) {
                    if (!membervar.empty()) {
                        if (Token::Match(tok, "%var% . %var% ;|%cop%") && tok->strAt(2) == membervar)
                            uninitStructMemberError(tok, tok->str() + "." + membervar);
                        else
                            return true;
                    }

                    // Use variable
                    else if (isVariableUsage(tok, var.isPointer(), _tokenizer->isCPP()))
                        uninitvarError(tok, tok->str());

                    else
                        // assume that variable is assigned
                        return true;
                }

                else if (Token::Match(tok, "sizeof|typeof|offsetof|decltype ("))
                    tok = tok->linkAt(1);

                tok = tok->next();
            }

            return true;
        }

        // variable is seen..
        if (tok->varId() == var.varId()) {
            if (!membervar.empty()) {
                if (isMemberVariableAssignment(tok, membervar))
                    return true;

                if (isMemberVariableUsage(tok, var.isPointer(), membervar))
                    uninitStructMemberError(tok, tok->str() + "." + membervar);

                else if (Token::Match(tok->previous(), "[(,] %var% [,)]"))
                    return true;

            } else {
                // Use variable
                if (isVariableUsage(tok, var.isPointer(), _tokenizer->isCPP()))
                    uninitvarError(tok, tok->str());

                else
                    // assume that variable is assigned
                    return true;
            }
        }
    }

    return false;
}

bool LintUninitVar::checkIfForWhileHead(const Token *startparentheses, const Variable& var, const std::string &membervar)
{
    const Token * const endpar = startparentheses->link();
    for (const Token *tok = startparentheses->next(); tok && tok != endpar; tok = tok->next()) {
        if (tok->varId() == var.varId()) {
            if (Token::Match(tok, "%var% . %var%")) {
                if (tok->strAt(2) == membervar) {
                    if (isMemberVariableAssignment(tok, membervar))
                        return true;

                    if (isMemberVariableUsage(tok, var.isPointer(), membervar))
                        uninitStructMemberError(tok, tok->str() + "." + membervar);
                }
                continue;
            }

            if (isVariableUsage(tok, var.isPointer(), _tokenizer->isCPP())) {
                uninitvarError(tok, tok->str());
            }
            return true;
        }
        if (Token::Match(tok, "sizeof|decltype|offsetof ("))
            tok = tok->next()->link();
    }
    return false;
}

bool LintUninitVar::checkLoopBody(const Token *tok, const Variable& var, const std::string &membervar)
{
    const Token *usetok = NULL;

    assert(tok->str() == "{");

    for (const Token * const end = tok->link(); tok != end; tok = tok->next()) {
        if (tok->varId() == var.varId()) {
            if (!membervar.empty()) {
                if (isMemberVariableAssignment(tok, membervar))
                    return true;

                if (isMemberVariableUsage(tok, var.isPointer(), membervar))
                    usetok = tok;
                else if (Token::Match(tok->previous(), "[(,] %var% [,)]"))
                    return true;
            } else {
                if (isVariableUsage(tok, var.isPointer(), _tokenizer->isCPP()))
                    usetok = tok;
                else
                    return true;
            }
        }

        if (Token::Match(tok, "sizeof|typeof ("))
            tok = tok->next()->link();
    }

    if (usetok) {
        if (membervar.empty())
            uninitvarError(usetok, usetok->str());
        else
            uninitStructMemberError(usetok, usetok->str() + "." + membervar);
        return true;
    }

    return false;
}

bool LintUninitVar::isVariableUsage(const Token *vartok, bool pointer, bool cpp)
{
    if (vartok->previous()->str() == "return")
        return true;

    // Passing variable to function..
    if (Token::Match(vartok->previous(), "[(,] %var% [,)]") || Token::Match(vartok->tokAt(-2), "[(,] & %var% [,)]")) {
        const bool address(vartok->previous()->str() == "&");

        // locate start parentheses in function call..
        int argumentNumber = 0;
        const Token *start = vartok;
        while (start && !Token::Match(start, "[;{}(]")) {
            if (start->str() == ")")
                start = start->link();
            else if (start->str() == ",")
                ++argumentNumber;
            start = start->previous();
        }

        // is this a function call?
        if (start && Token::Match(start->previous(), "%var% (")) {
            // check how function handle uninitialized data arguments..
            const Function *func = start->previous()->function();
            if (func) {
                const Variable *arg = func->getArgumentVar(argumentNumber);
                if (arg) {
                    const Token *argStart = arg->typeStartToken();
                    while (argStart->previous() && argStart->previous()->isName())
                        argStart = argStart->previous();
                    if (!address && Token::Match(argStart, "const| struct| %type% %var% [,)]"))
                        return true;
                    if (Token::Match(argStart, "const %type% & %var% [,)]"))
                        return true;
                    if (pointer && !address && Token::Match(argStart, "%type% * %var% [,)]"))
                        return true;
                    if ((pointer || address) && Token::Match(argStart, "const %type% * %var% [,)]"))
                        return true;
                }

            } else if (Token::Match(start->previous(), "if|while|for")) {
                // control-flow statement reading the variable "by value"
                return true;
            }
        }
    }

    if (Token::Match(vartok->previous(), "++|--|%cop%")) {
        if (cpp && vartok->previous()->str() == ">>") {
            // assume that variable is initialized
            return false;
        }

        // is there something like: ; "*((&var ..expr.. ="  => the variable is assigned
        if (vartok->previous()->str() == "&") {
            const Token *tok2 = vartok->tokAt(-2);
            if (tok2 && (tok2->isConstOp() || tok2->str() == "("))
                return false; // address of
            if (tok2 && tok2->str() == ")")
                tok2 = tok2->link()->previous();
            if (Token::Match(tok2,"[()] ( %type% *| ) &") && tok2->tokAt(2)->varId() == 0)
                return false; // cast
            while (tok2 && tok2->str() == "(")
                tok2 = tok2->previous();
            while (tok2 && tok2->str() == "*")
                tok2 = tok2->previous();
            if (Token::Match(tok2, "[;{}] *")) {
                // there is some such code before vartok: "[*]+ [(]* &"
                // determine if there is a = after vartok
                for (tok2 = vartok; tok2; tok2 = tok2->next()) {
                    if (Token::Match(tok2, "[;{}]"))
                        break;
                    if (tok2->str() == "=")
                        return false;
                }
            }
        }

        if (vartok->previous()->str() != "&" || !Token::Match(vartok->tokAt(-2), "[(,=?:]")) {
            return true;
        }
    }

    bool unknown = false;
    if (pointer && CheckNullPointer::isPointerDeRef(vartok, unknown)) {
        // function parameter?
        bool functionParameter = false;
        if (Token::Match(vartok->tokAt(-2), "%var% (") || vartok->previous()->str() == ",")
            functionParameter = true;

        // if this is not a function parameter report this dereference as variable usage
        if (!functionParameter)
            return true;
    }

    if (cpp && Token::Match(vartok->next(), "<<|>>")) {
        // Is this calculation done in rhs?
        const Token *tok = vartok;
        while (tok && Token::Match(tok, "%var%|.|::"))
            tok = tok->previous();
        if (Token::Match(tok, "[;{}]"))
            return false;

        // Is variable a known POD type then this is a variable usage,
        // otherwise we assume it's not.
        const Variable *var = vartok->variable();
        return (var && var->typeStartToken()->isStandardType());
    }

    if (vartok->next() && vartok->next()->isOp() && !vartok->next()->isAssignmentOp())
        return true;

    if (vartok->strAt(1) == "]")
        return true;

    return false;
}

bool LintUninitVar::isMemberVariableAssignment(const Token *tok, const std::string &membervar) const
{
    if (Token::Match(tok, "%var% . %var%") && tok->strAt(2) == membervar) {
        if (Token::Match(tok->tokAt(3), "[=.[]"))
            return true;
        else if (Token::Match(tok->tokAt(-2), "[(,=] &"))
            return true;
        else if ((tok->previous() && tok->previous()->isConstOp()) || Token::Match(tok->previous(), "[|="))
            ; // member variable usage
        else if (tok->tokAt(3)->isConstOp())
            ; // member variable usage
        else
            return true;
    } else if (tok->strAt(1) == "=")
        return true;
    else if (tok->strAt(-1) == "&")
        return true;
    return false;
}

bool LintUninitVar::isMemberVariableUsage(const Token *tok, bool isPointer, const std::string &membervar) const
{
    if (isMemberVariableAssignment(tok, membervar))
        return false;

    if (Token::Match(tok, "%var% . %var%") && tok->strAt(2) == membervar)
        return true;
    else if (Token::Match(tok->previous(), "[(,] %var% [,)]") && isVariableUsage(tok, isPointer, _tokenizer->isCPP()))
        return true;

    return false;
}

void LintUninitVar::uninitstringError(const Token *tok, const std::string &varname, bool strncpy_)
{
    reportError(tok, Severity::error, "uninitstring", "Dangerous usage of '" + varname + "'" + (strncpy_ ? " (strncpy doesn't always null-terminate it)." : " (not null-terminated)."));
}

void LintUninitVar::uninitdataError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "uninitdata", "Memory is allocated but not initialized: " + varname);
}

void LintUninitVar::uninitvarError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "uninitvar", "Uninitialized variable: " + varname);
}

void LintUninitVar::uninitStructMemberError(const Token *tok, const std::string &membername)
{
    reportError(tok,
                Severity::error,
                "uninitStructMember",
                "Uninitialized struct member: " + membername);
}
