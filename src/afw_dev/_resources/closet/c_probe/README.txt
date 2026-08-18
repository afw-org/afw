Prime a leaf (preferred):

  afwdev prime-test-c-probe tests/hole
  afwdev prime-test-c-probe src/myext/tests/advanced/hole

Path is relative to the current directory or absolute. It must be a
new or empty leaf under tests/ or tests-extra/.

Then:

  afwdev test --test-pattern 'hole'
  afwdev test -T src/afw/tests-extra/some-hole

These two files are the closet source the subcommand copies. You can
still copy them by hand if you need to.

Do not add a cmake test target. The helper compiles against installed
libafw at test time.

Handbook: Developer Guide → Writing Tests → C Probes.
Pad: designs/c-probes.md
