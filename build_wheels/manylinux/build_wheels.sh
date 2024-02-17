cd /
mkdir /wheels

# build wheels for all c-python versions
for FILE in /opt/python/cp*;
do
	echo -e "\nbuilding with $($FILE/bin/pip -V)"
	$FILE/bin/pip wheel -w /wheels /PandaPrimes --no-deps
done

cd /wheels
# audit the wheels to manylinux wheels
for WHEEL in /wheels/*;
do
	echo -e "auditing $WHEEL";
	auditwheel repair $WHEEL
done
