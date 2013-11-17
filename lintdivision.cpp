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

#include "check.h"
#include "mathlib.h"

class LintDivision : public Check {
public:
    /** @brief This constructor is used when registering the LintDivision */
    LintDivision() : Check(myName()) {
    }

    /** @brief This constructor is used when running checks. */
    LintDivision(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        LintDivision lintDivision(tokenizer, settings, errorLogger);
        lintDivision.check();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) { }

    /** Check for division where second operator might be zero */
    void check();
    void divisionError(const Token *tok);

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
    LintDivision instance;
}

//---------------------------------------------------------------------------

void LintDivision::check()
{
    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next()) {
        // Division?
        if (!Token::Match(tok, "[/%]"))
            continue;

        // ok if second operator is non-zero
        const std::string second = tok->next() ? tok->strAt(1) : "";
        if (MathLib::isInt(second) && second != "0")
            continue;
        if (MathLib::isFloat(second)) {
            bool notzero = false;
            for (std::size_t i = 0; i < second.size(); ++i)
                notzero |= (second[i]>='1' && second[i]<='9');
            if (notzero)
                continue;
        }

        // second operator for division might be zero..
        divisionError(tok);
    }
}

void LintDivision::divisionError(const Token *tok)
{
    reportError(tok,
                Severity::error,
                "division",
                "Division, failed to determine that second operator is non-zero");
}
