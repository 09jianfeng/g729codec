#!/bin/bash

for f in *
do
nf=`echo $f | tr 'A-Z' 'a-z'`
echo $nf
[ "$f" != "$nf" ] && mv ./"$f" ./"$nf"
done
