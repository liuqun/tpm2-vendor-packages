# Change log and develop memo

## Unreleased
### Added
- 使用"git subtree add"命令取回libtpms项目的最新版本
- 添加CHANGELOG日志

### Changed

### Fixed

# Some notes and tips
## How to fetch code from upstream
Update libtpms source tree using "git subtree pull":

    LIBTPMS_BRANCH=tpm2-preview.rev146
    git subtree pull --prefix=libtpms https://github.com/stefanberger/libtpms.git ${LIBTPMS_BRANCH}

## How to compile libtpms and swtpm
Project libtpms depends on libssl-dev and the standard gcc toolchain.

    sudo apt-get install -y libssl-dev
    pushd libtpms
    ./bootstrap.sh
    ./configure --with-openssl --prefix=/usr/local --with-tpm2
    make -j(nproc)
    make check
    sudo make install
    popd # libtpms

Project swtpm suggests the following development packages:

    sudo apt-get install -y gnutls-bin libgnutls28-dev libfuse-dev libglib2.0-dev libgmp-dev expect libtasn1-dev socat findutils trousers tpm-tools

Project [swtpm](https://github.com/stefanberger/swtpm)
and [libtpms](https://github.com/stefanberger/libtpms)
are both maintained by Stefan Berger on GitHub.
