# linter for undefined behaviour

import cppcheckdata
import sys

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

def checkuninit(data):
  return

for arg in sys.argv[1:]:
  print('Checking ' + arg + '...')
  data = cppcheckdata.parsedump(arg)
  checkdivision(data)
  checkuninit(data)
