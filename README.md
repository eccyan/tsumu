# tsumu

## Build

```bash
$ docker build --no-cache --pull -f build/Dockerfile -t tsumu .
$ ./build/dbuild cmake .
$ ./build/dbuild make
```

## Running

```bash
$ ./build/dbuild bin/tsumu
```
