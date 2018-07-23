FROM dockcross/linux-x64

ENV DEFAULT_DOCKCROSS_IMAGE rtest-amd64

RUN apt-get install -yq libboost-serialization-dev qtdeclarative5-dev qt5-default libsqlite3-dev swig2.0

# Install Standard Python Dev Stuff
RUN apt-get install -yq libpython3-dev python3-numpy python3 libpython3.4

ENV LD_LIBRARY_PATH /opt/swig/usr/lib

# Add swig from debian unstable, so the newest parser features are available.
RUN echo "deb http://ftp.at.debian.org/debian unstable main" >> /etc/apt/sources.list

RUN apt-get update && apt-get -t sid download -y \
  swig3.0 && \
  mkdir -p /opt/swig && \
  mv ./*swig*.deb /opt/swig && \
  cd /opt/swig && \
  dpkg -x swig*.deb ./

# Link swig directory to expected path
RUN ln -s /opt/swig/usr/share/swig3.0/ /usr/share/swig3.0
RUN ln -s /opt/swig/usr/bin/swig3.0 /usr/bin/swig3.0
