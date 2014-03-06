#!/bin/sh

# indent some code, requires python, astyle and autopep8:
# cd /tmp && git clone https://github.com/hhatto/autopep8.git && cd autopep8 && ./setup.py install --user

# cxx
# cxx_files=`find lib -name *.?xx`
# for cxx_file in $cxx_files
# do
#   echo "-- $cxx_file"
#   astyle --style=ansi --indent=spaces=2 --indent-switches --pad-oper --break-closing-brackets $cxx_file
# done
astyle --style=ansi --recursive --indent=spaces=2 --indent-switches --pad-oper --break-closing-brackets lib/*.?xx

# python
/usr/share/doc/python2.7/examples/Tools/scripts/reindent.py --verbose --recurse python/
~/.local/bin/autopep8 --verbose --in-place --recursive python/
