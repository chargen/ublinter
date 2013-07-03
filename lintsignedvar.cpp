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

#include "symboldatabase.h"
#include "tokenize.h"
#include "check.h"

//---------------------------------------------------------------------------

class LintSignedVar : public Check {
public:
    /** @brief This constructor is used when registering the LintSignedVar */
    LintSignedVar() : Check(myName())
    { }

    /** @brief This constructor is used when running checks. */
    LintSignedVar(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger)
    { }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        LintSignedVar lintSignedVar(tokenizer, settings, errorLogger);
        lintSignedVar.check();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) { }

    /** Check */
    void check();

    void signedCalcError(const Token *tok, const std::string &op, const std::string &varname);
    void signedArrayIndexError(const Token *tok, const std::string &varname);
    void signedSqrtfError(const Token *tok, const std::string &varname);

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
    LintSignedVar instance;
}

//---------------------------------------------------------------------------

bool badop(const Token *tok)
{
    return ((tok->str() == "&" && !tok->previous()->isOp()) ||
            tok->str() == "|" ||
            tok->str() == "^" ||
            tok->str() == "<<" ||
            tok->str() == ">>");
}

static bool isSigned(const Variable &var)
{
    bool s = false;
    for (const Token *decl = var.typeStartToken(); decl != var.nameToken(); decl = decl->next())
        s |= (Token::Match(decl,"char|short|int|long") && !decl->isUnsigned());
	return s;
}

void LintSignedVar::check()
{
    const SymbolDatabase *symbolDatabase = _tokenizer->getSymbolDatabase();
    std::list<Scope>::const_iterator scope;

    // check every executable scope
    std::set<unsigned int> signedVariables;
    for (scope = symbolDatabase->scopeList.begin(); scope != symbolDatabase->scopeList.end(); ++scope) {
        if (scope->type == Scope::eFunction) {
            const Function *function = NULL;
            for (std::list<Function>::const_iterator it = scope->nestedIn->functionList.begin(); it != scope->nestedIn->functionList.end(); ++it) {
                if (it->functionScope == &(*scope)) {
                    function = &(*it);
                    break;
                }
            }

            if (function) {
                for (unsigned int arg = 0; arg < function->argCount(); ++arg) {
                    const Variable *variable = function->getArgumentVar(arg);
                    if (isSigned(*variable))
                        signedVariables.insert(variable->varId());
                }
            }
        }

        // Get signed variables..
        for (std::list<Variable>::const_iterator var = scope->varlist.begin(); var != scope->varlist.end(); ++var) {
            if (isSigned(*var))
                signedVariables.insert(var->varId());
        }

        // Parse scope..
        for (const Token *tok = scope->classStart; tok != scope->classEnd; tok = tok->next()) {
            if (tok->varId() > 0U && signedVariables.find(tok->varId()) != signedVariables.end()) {
                if (badop(tok->previous()))
                    signedCalcError(tok, tok->previous()->str(), tok->str());
                if (badop(tok->next()))
                    signedCalcError(tok, tok->next()->str(), tok->str());
                if (tok->previous()->str() == "[" || tok->next()->str() == "]")
                    signedArrayIndexError(tok, tok->str());
                if (Token::Match(tok, "%var% )")) {
                    const Token *tok1 = tok->previous();
                    if (tok1->str() == ")")
                        tok1 = tok1->link()->previous();
                    if (tok1 && Token::simpleMatch(tok1->previous(), "sqrtf ("))
                        signedSqrtfError(tok, tok->str());
                }
            }
        }
    }
}

void LintSignedVar::signedCalcError(const Token *tok, const std::string &op, const std::string &varname)
{
    reportError(tok, Severity::error, "signedcalc", "Behaviour of '" + op + "' is not defined when a operand is negative. The operand '" + varname + "' is signed.");
}

void LintSignedVar::signedArrayIndexError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "signedarrayindex", "Negative array index is often out of bounds. The array index '" + varname + "' is signed.");
}

void LintSignedVar::signedSqrtfError(const Token *tok, const std::string &varname)
{
    reportError(tok, Severity::error, "signedsqrtf", "Negative parameter to sqrt is invalid. The variable '" + varname + "' is signed.");
}
