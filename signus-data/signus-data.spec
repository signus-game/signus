# This file is part of Signus: The Artefact Wars
# https://github.com/signus-game
#
# Copyright (C) 2021 Martin Doucha <doucha@swarmtech.cz>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

%define        ver_app    1.96.0
%define        ver_data   1.96

Name:          signus-data
Version:       %{ver_data}
Release:       0
BuildArch:     noarch
Source0:       %{name}-%{ver_data}.tar.bz2

License:       GPL-2.0-only
Summary:       Data files for Signus
Group:         Games/Strategy
URL:           https://github.com/signus-game

BuildRequires: libpng-devel

Requires:      signus >= %{ver_app}

%description
Common data files for Signus: The Artefact Wars


%package l10n-cs
Summary:       Czech data for Signus
Group:         Games/Strategy
Requires:      signus-data = %{ver_data}
Provides:      locale(signus-data:cs_CZ)

%description l10n-cs
Czech data files for Signus: The Artefact Wars


%prep
%setup -q

%build
%configure
%make_build

%install
%make_install

%files
%dir %{_datadir}/signus
%{_datadir}/signus/%{ver_data}/nolang
%{_datadir}/signus/%{ver_data}/en

%files l10n-cs
%dir %{_datadir}/signus/%{ver_data}/cs
%{_datadir}/signus/%{ver_data}/cs
