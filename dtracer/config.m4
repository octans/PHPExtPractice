PHP_ARG_ENABLE(dtracer, whether to enable dtracer support,
dnl Make sure that the comment is aligned:
[  --enable-dtracer           Enable dtracer support])

  PHP_NEW_EXTENSION(dtracer, dtracer.c, $ext_shared)
