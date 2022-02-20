DESTDIR="${DESTDIR:-/usr}"

redo-ifchange rins-s6-colorizer
install -m0755 "${DESTDIR}/bin"
install -m0755 ./rins-s6-colorizer "${DESTDIR}/bin/s6-rc-color"
