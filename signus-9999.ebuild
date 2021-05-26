# Copyright 2021 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=7

inherit git-r3

DESCRIPTION="Turn-based sci-fi strategy wargame"
HOMEPAGE="https://github.com/signus-game/signus"
EGIT_REPO_URI="https://github.com/signus-game/signus"
EGIT_SUBMODULES=()

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="debug"
RESTRICT="debug? ( strip )"

RDEPEND="
	media-libs/libsdl2:=
	media-libs/sdl2-image:=[jpeg]
	media-libs/sdl2-mixer:=[midi,vorbis]
	media-libs/sdl2-ttf:="
DEPEND="${RDEPEND}
	media-libs/libpng"

src_configure() {
	cd signus
	./bootstrap
	econf $(use_enable debug)
	cd ../signus-data
	./bootstrap
	econf
	cd ..
}

src_compile() {
	cd signus
	emake
	cd ../signus-data
	emake
	cd ..
}

src_install() {
	cd signus
	emake DESTDIR="${D}" install
	dodoc AUTHORS
	cd ../signus-data
	emake DESTDIR="${D}" install
	cd ..
}
