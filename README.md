# Rin's s6 colorizer

A program which wraps s6-rc and colorizes its output for some eye-candy during system startup and shutdown.

With colors you can also more easily tell what's going on at a glance.

Usage:
- copy or link the built executable to /bin or /usr/bin as "s6-rc-color"
- use "s6-rc-color" in place of "s6-rc" in scripts (on Artix /etc/s6/current/scripts/{runlevel,rc.shutdown})
- alternatively pass the output of "s6-rc" with the -v2 argument through the colorizer program (preferably with execline)

TODO:
[x] separate functionality into smaller functions
[x] add wrapper functionality if run as "s6-rc-color"
