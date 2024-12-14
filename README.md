Collection of stuff I've made either as uni work, or just while pondering.

Some older files would be including a header file stdc++.h, which includes lots of pre-made libraries,
as well as a few functions & constants I've made for my own convenience.
Going forward: fewer wildcard includes (less bloating).

Most work I add here would have ANSI escape codes (for positioning cursor, clearing screen,
or coloring output) used in strings, which work as intended on Mac and Linux terminals,
but do not display correctly on most IDEs' consoles, and might not work on Windows at all.

If that is an issue, try removing the ANSI escape code
  (starts with \33[ or \033[ or \e[ and ends with the first letter after)
e.g if this is a problematic string: "\e[32Huncolored\33[(WHATEVER IS IN BETWEEN)mColored"
  the new string would be: "uncoloredColored"
