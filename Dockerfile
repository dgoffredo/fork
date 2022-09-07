from ubuntu:20.04

env DEBIAN_FRONTEND=noninteractive
run apt-get update && apt-get install -y g++ procps
copy file /etc/file
run mkdir /src
copy program.cpp /src/program.cpp
run g++ -o /bin/program /src/program.cpp
copy init /bin/init
cmd ["/bin/init"]

