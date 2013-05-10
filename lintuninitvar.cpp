/*
 * ublinter - lint tool made by Daniel Marjamäki
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
#include "config.h"
#include "check.h"
#include "settings.h"

//---------------------------------------------------------------------------

class Token;
class Scope;
class Variable;

class LintUninitVar : public Check {
public:
    /** @brief This constructor is used when registering the LintUninitVar */
    LintUninitVar() : Check(myName())
    { }

    /** @brief This constructor is used when running checks. */
    LintUninitVar(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger)
    { }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        LintUninitVar lintUninitVar(tokenizer, settings, errorLogger);
        lintUninitVar.check();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) { }

    /** Check for uninitialized variables */
    void check();
    void checkScope(const Scope* scope);
    bool checkScopeForVariable(const Scope* scope, const Token *tok, const Variable& var, std::set<unsigned int> * const aliases, const std::string &membervar);
    bool checkIfForWhileHead(const Token *startparentheses, const Variable& var, const std::set<unsigned int> &aliases, const std::string &membervar);
    static bool isVariableAssignment(const Token *vartok, bool pointer, bool alias, bool full);
    static bool isVariableUsage(const Token *vartok, bool ispointer);
    bool isMemberVariableAssignment(const Token *tok, const std::string &membervar) const;
    bool isMemberVariableUsage(const Token *tok, bool isPointer, const std::string &membervar) const;

    void uninitdataError(const Token *tok, const std::string &varname);
    void uninitvarError(const Token *tok, const std::string &varname);
    void uninitStructMemberError(const Token *tok, const std::string &membername);

private:

    void getErrorMessages(ErrorLogger*, const Settings*) const {}

    static std::string myName() {
        return "";
    }

    std::string classInfo() const {
        return "";
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
        if (i->isStatic())
            continue;

        if (i->nameToken()->strAt(1) == "(")
            continue;

        const Token* tok = Token::findmatch(i->typeEndToken(), "[=;]");
        if (tok && tok->str() == "=")
            continue;

        std::set<unsigned int> aliases;
        checkScopeForVariable(scope, tok, *i, &aliases, "");

        if (Token::Match(i->typeStartToken(), "struct %type% %var% ;")) {
            const std::string structname(i->typeStartToken()->next()->str());
            const SymbolDatabase * symbolDatabase = _tokenizer->getSymbolDatabase();
            for (std::size_t j = 0U; j < symbolDatabase->classAndStructScopes.size(); ++j) {
                const Scope *scope2 = symbolDatabase->classAndStructScopes[j];
                if (scope2->className == structname && scope2->numConstructors == 0U) {
                    for (std::list<Variable>::const_iterator it = scope2->varlist.begin(); it != scope2->varlist.end(); ++it) {
                        const Variable &var = *it;
                        if (!var.isArray()) {
                            aliases.clear();
                            checkScopeForVariable(scope, tok, *i, &aliases, var.name());
                        }
                    }
                }
            }
        }
    }
}

bool LintUninitVar::checkScopeForVariable(const Scope* scope, const Token *tok, const Variable& var, std::set<unsigned int> * const aliases, const std::string &membervar)
{
    const bool startsWithCase(tok->str() == "case");

    for (; tok; tok = tok->next()) {
        // End of scope..
        if (tok->str() == "}") {
            break;
        }

        // Unconditional inner scope..
        if (tok->str() == "{" && Token::Match(tok->previous(), "[;{}=]")) {
            if (checkScopeForVariable(scope, tok->next(), var, aliases, membervar))
                return true;
            tok = tok->link();
            continue;
        }

        // Inner scope..
        if (Token::simpleMatch(tok, "if (")) {
            // initialization / usage in condition..
            if (checkIfForWhileHead(tok->next(), var, *aliases, membervar))
                return true;

            // goto the {
            tok = tok->next()->link()->next();

            if (!tok)
                break;
            if (tok->str() == "{") {
                std::set<unsigned int> aliasesif(*aliases);
                std::set<unsigned int> aliaseselse(*aliases);

                const bool initif = checkScopeForVariable(scope, tok->next(), var, &aliasesif, membervar);
                if (initif)
                    aliasesif.clear();

                // goto the }
                tok = tok->link();

                if (Token::simpleMatch(tok, "} else {")) {
                    // goto the {
                    tok = tok->tokAt(2);

                    const bool initelse = checkScopeForVariable(scope, tok->next(), var, &aliaseselse, membervar);
                    if (initelse)
                        aliaseselse.clear();

                    // goto the }
                    tok = tok->link();

                    if (initif && initelse)
                        return true;
                }

                aliases->insert(aliasesif.begin(), aliasesif.end());
                aliases->insert(aliaseselse.begin(), aliaseselse.end());
            }
        }

        // skip sizeof / offsetof
        if (Token::Match(tok, "sizeof|typeof|offsetof|decltype ("))
            tok = tok->next()->link();

        // for/while..
        if (Token::Match(tok, "for|while (")) {
            if (membervar.empty() && Token::Match(tok, "for ( %varid% =", var.varId()))
                return true;

            // is variable initialized in for-head (don't report errors yet)?
            if (checkIfForWhileHead(tok->next(), var, *aliases, membervar))
                return true;

            // goto the {
            tok = tok->next()->link()->next();

            checkScopeForVariable(scope, tok->next(), var, aliases, membervar);

            // goto the }
            tok = tok->link();
        }

        // switch
        if (Token::simpleMatch(tok, "switch (")) {
            if (checkIfForWhileHead(tok->next(), var, *aliases, membervar))
                return true;

            // goto the {
            tok = tok->next()->link()->next();

            if (!tok)
                break;
            if (tok->str() == "{") {
                const Token *endTok = tok->link();
                bool init = true;
                bool def = false;
                for (const Token *tok2 = tok->next(); tok2 != endTok; tok2 = tok2->next()) {
                    if (Token::Match(tok2, "case %any% :") &&
                        !checkScopeForVariable(scope, tok2, var, aliases, membervar)) {
                        init = false;
                    }
                    if (Token::simpleMatch(tok2, "default :")) {
                        def = true;
                        if (!checkScopeForVariable(scope, tok2, var, aliases, membervar))
                            init = false;
                    }
                }

                // goto the }
                tok = endTok;

                if (def && init)
                    return true;
            }
        }

        if (Token::Match(tok, "return|break|continue|throw")) {
            while (tok && tok->str() != ";") {
                // variable is seen..
                if (tok->varId() == var.varId() || (tok->varId() > 0U && aliases->find(tok->varId()) != aliases->end())) {
                    if (!membervar.empty()) {
                        if (Token::Match(tok, "%var% . %var% ;|%cop%") && tok->strAt(2) == membervar)
                            uninitStructMemberError(tok, tok->str() + "." + membervar);
                    }

                    // Use variable
                    else if (isVariableUsage(tok, var.isPointer()))
                        uninitvarError(tok, tok->str());

                    else if (!Token::Match(tok->previous(), "[;{}] %var% [|."))
                        return true;
                }

                else if (Token::Match(tok, "sizeof|typeof|offsetof|decltype ("))
                    tok = tok->linkAt(1);

                tok = tok->next();
            }

            return !startsWithCase;
        }

        if (Token::Match(tok, "goto %var% ;")) {
            // locate end of variable scope
            const Token *tok2 = var.typeStartToken();
            while (tok2 && tok2->str() != "{") {
                if (tok2->str() == "}")
                    tok2 = tok2->link();
                tok2 = tok2->previous();
            }
            const Token * const endToken = tok2->link();

            // Warn if variable is used anywhere in the scope. Assumes that execution
            // runs wildly.
            for (tok2 = Token::findsimplematch(var.typeEndToken(), ";"); tok2 != endToken; tok2 = tok2->next()) {
                if (tok2->varId() == var.varId() || (tok2->varId() > 0U && aliases->find(tok->varId()) != aliases->end())) {
                    bool assign = false;

                    if (!membervar.empty()) {
                        if (isMemberVariableAssignment(tok2, membervar))
                            assign = true;
                        else if (isMemberVariableUsage(tok2, var.isPointer(), membervar))
                            uninitStructMemberError(tok2, tok2->str() + "." + membervar);
                    } else {
                        if (isVariableAssignment(tok2, var.isPointer(), tok2->varId() != var.varId(), true))
                            assign = true;

                        // Use variable
                        else if (isVariableUsage(tok2, var.isPointer()))
                            uninitvarError(tok2, tok2->str());
                    }

                    if (assign) {
                        while (tok2 && tok2->str() != "}" && !Token::Match(tok2, "%var% :")) {
                            if (tok2->str() == "{")
                                tok2 = tok2->link();
                            tok2 = tok2->next();
                        }
                        if (!tok2 || tok2 == endToken)
                            break;
                    }
                }
            }

            return true;
        }

        // variable is seen..
        if (tok->varId() == var.varId() || (tok->varId() > 0U && aliases->find(tok->varId()) != aliases->end())) {
            if (!membervar.empty()) {
                if (isMemberVariableAssignment(tok, membervar))
                    return true;

                if (isMemberVariableUsage(tok, var.isPointer(), membervar))
                    uninitStructMemberError(tok, tok->str() + "." + membervar);
            } else {
                if (isVariableAssignment(tok, var.isPointer(), tok->varId() != var.varId(), true))
                    return true;

                if (Token::Match(tok->tokAt(-4), "[;{}] %var% = & %var% ;")) {
                    const Variable *var1 = _tokenizer->getSymbolDatabase()->getVariableFromVarId(tok->tokAt(-3)->varId());
                    if (var1 && var1->isPointer()) {
                        aliases->insert(var1->varId());
                        continue;
                    }
                }

                else if (var.isArray() && Token::Match(tok->tokAt(-3), "[;{}] %var% = %var% ;")) {
                    const Variable *var1 = _tokenizer->getSymbolDatabase()->getVariableFromVarId(tok->tokAt(-2)->varId());
                    if (var1 && var1->isPointer()) {
                        aliases->insert(var1->varId());
                        continue;
                    }
                }

                // Use variable
                if (isVariableUsage(tok, var.isPointer()))
                    uninitvarError(tok, tok->str());
            }
        }
    }

    return false;
}

bool LintUninitVar::checkIfForWhileHead(const Token *startparentheses, const Variable& var, const std::set<unsigned int> &aliases, const std::string &membervar)
{
    const Token * const endpar = startparentheses->link();
    for (const Token *tok = startparentheses->next(); tok && tok != endpar; tok = tok->next()) {
        if (tok->varId() == var.varId() || (tok->varId() > 0U && aliases.find(tok->varId()) != aliases.end())) {
            if (Token::Match(tok, "%var% . %var%")) {
                if (tok->strAt(2) == membervar) {
                    if (isMemberVariableAssignment(tok, membervar))
                        return true;

                    if (isMemberVariableUsage(tok, var.isPointer(), membervar))
                        uninitStructMemberError(tok, tok->str() + "." + membervar);
                } else if (membervar.empty()) {
                    uninitvarError(tok, tok->str());
                }
                continue;
            }

            if (isVariableUsage(tok, var.isPointer()))
                uninitvarError(tok, tok->str());
        }
        if (Token::Match(tok, "sizeof|decltype|offsetof ("))
            tok = tok->next()->link();
    }
    return false;
}

bool LintUninitVar::isVariableAssignment(const Token *vartok, bool pointer, bool alias, bool full)
{
    // assign _whole_ var/array/struct
    if (!alias && Token::Match(vartok->previous(), "[;{}=] %var% ="))
        return true;
    else if (alias && Token::Match(vartok->tokAt(-2), "[;{}=] * %var% ="))
        return true;
    else if (Token::Match(vartok->tokAt(-2), "va_start ( %var% ,"))
        return true;
    else if (Token::Match(vartok->tokAt(-3), "memset ( & %var% , 0 , sizeof ( %varid% ) )", vartok->varId()))
        return true;

    // assigning part of array/struct
    if (!full && !pointer && Token::Match(vartok->previous(), "[;{}=] %var% . %var% ="))
        return true;
    if (!full && Token::Match(vartok->previous(), "[;{}] %var% [")) {
        const Token *tok2 = vartok->next();
        while (tok2 && tok2->str() == "[")
            tok2 = tok2->link()->next();
        if (tok2 && tok2->str() == "=")
            return true;
    }

    return false;
}

bool LintUninitVar::isVariableUsage(const Token *vartok, bool pointer)
{
    if (isVariableAssignment(vartok, pointer, false, false))
        return false;
    return true;
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
    else if (Token::Match(tok->previous(), "[(,] %var% [,)]") && isVariableUsage(tok, isPointer))
        return true;

    return false;
}

void LintUninitVar::uninitdataError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "uninitdata", "Memory is allocated but not initialized: " + varname);
}

void LintUninitVar::uninitvarError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "lintuninitvar", "Uninitialized variable: " + varname);
}

void LintUninitVar::uninitStructMemberError(const Token *tok, const std::string &membername)
{
    reportError(tok,
                Severity::error,
                "uninitStructMember",
                "Uninitialized struct member: " + membername);
}
