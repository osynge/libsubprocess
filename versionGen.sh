if test "A" != A$SRC_VERSION; then
echo $SRC_VERSION
exit 0
fi
NUMBER=`head -n 1 ChangeLog | sed -e 's/^.*-//'`
echo ${NUMBER}rc`date '+20%y%m%d%H'`
