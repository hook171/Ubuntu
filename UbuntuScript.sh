git pull origin main

if [ ! -d "build_ubuntu" ]; then
  mkdir build_ubuntu
fi

cd build_ubuntu

cmake ..

cmake --build .

./Ubuntu