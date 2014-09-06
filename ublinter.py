# linter for undefined behaviour

import cppcheckdata
import sys

# division by zero
def checkdivision(data):
  for token in data.tokenlist:
    # not division
    if token.str!='/' and token.str!='%':
      continue

    # get denominator
    op2 = token.astOperand2

    # cast
    if op2.str == "(" and op2.astOperand2 is None:
      op2 = op2.astOperand1

    # If we know that denominator is non-zero => continue
    if op2.str=='(' and op2.astOperand1.str=='sizeof':
      continue
    if op2.isInt and op2.str != '0':
      continue
    if op2.isFloat:
      continue

    # Denominator is not known to be non-zero..
    print('[' + token.file + ':' + str(token.linenr) + '] Division by zero')

def isIntMax(v):
    if v == 0x7fff:
        return 16
    if v == 0x7fffffff:
        return 32
    if v == 0x7fffffffffffffff:
        return 64
    return None

def checkintegeroverflow(data):
    for token in data.tokenlist:
        # INT_MAX - svar
        if token.str=='-' and token.astOperand2 and token.astOperand1.isInt and token.astOperand2.isName:
            bits = isIntMax(int(token.astOperand1.str))
            if bits:
                print('[' + token.file + ':' + str(token.linenr) + '] Signed integer overflow if ' + token.astOperand2.str + ' is negative (' + str(bits) + ' bit int)')

def checkuninit(data):
    # TODO
    return

for arg in sys.argv[1:]:
  print('Checking ' + arg + '...')
  data = cppcheckdata.parsedump(arg)
  checkdivision(data)         # division by zero
  checkintegeroverflow(data)  # integer overflow
  checkuninit(data)           # uninitialized variable
