# unit tests

## requirements (debian/ubuntu)

```
apt-get install python-nose cmake
```

## howto (debian/ubuntu)

```
cd ~ 
git clone https://github.com/BeaEngine/beaengine.git
cmake -DoptHAS_OPTIMIZED=TRUE -DoptHAS_SYMBOLS=TRUE -DoptBUILD_64BIT=FALSE -DoptBUILD_DLL=TRUE -DoptBUILD_LITE=FALSE beaengine
make
cp beaengine/lib/Linux.gnu.relwithdebinfo/libBeaEngine.so beaengine/headers
cd beaengine & nosetests tests/*.py
```


