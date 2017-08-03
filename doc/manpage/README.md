Documentation
-------------

To generate the man pages for srcml:

1. Install `ronn` via:

```
gem install ronn
```

2. The file `srcml_input.md` contains all of the information for the
man page, but it has some variables in it (like `LANGUAGE_FLAG_SHORT` or
`LANGUAGE_FLAG_LONG`), which we want to replace (`l` and `language`,
repsectively). So we execute a bash script to find all of these variables
and replace them with the desired name, via:

```
./replace_variables.sh srcml_input.md srcml.md
```

3. Now we can generate the man page based on `srcml.md`. Execute `ronn`
from this directory via:

```
ronn srcml.md --manual=srcml
```

