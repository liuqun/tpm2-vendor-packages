# Change log

## Unreleased
### Added
- 导入开源项目ibmswtpm2的源码包(编号974和1119)
- 添加CHANGELOG日志

### Changed

### Fixed

# Some notes and tips
## How to fetch code from upstream
Project [ibmswtpm2](https://sourceforge.net/projects/ibmswtpm2/)
and [ibmtpm20tss](https://sourceforge.net/projects/ibmtpm20tss/)
are both maintained by Ken Goldman on sourceforge.net.

## How to build and install
Project depends on openssl 1.0.2g and the standard gcc toolchain.

    sudo apt-get install -y libssl1.0-dev
    mkdir -p build
    pushd build
    tar axf ../ibmtpm974.tar.gz
    make --directory src
    export PREFIX=/usr/local
    sudo mkdir -p ${PREFIX}/bin
    sudo cp src/tpm_server ${PREFIX}/bin/
    popd

