#!/bin/bash
# Runs polygon2problemtools.py over all directories in $ARG/polygon/
# If no $ARG is specified, used current directory.
# Additionally runs some utilities like dos2unix on the output.

echo "looking in $1/polygon/"
for d in $1/polygon/*/; do
  NAME=$(basename $d)
  COLOUR=$(echo $RANDOM | md5sum | head -c 6)
  OUTPUT=$1/domjudge/$NAME/
  echo "converting $d and placing in $OUTPUT, ok? (ctrl-c to stop)"
  read
  python3 ./polygon2problemtools.py --shortcode $NAME --colour $COLOUR \
      --domjudge -o $OUTPUT $d
  find $OUTPUT -type f -print0 | xargs -0 dos2unix -q
  echo
  echo
done
