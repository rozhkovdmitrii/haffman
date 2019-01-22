## Haffman algorithm based encoder and decoder project

### Building

It's possible to build encoder and decoder executable in two ways: by simple Makefile or cmake engine.

#### make

One thing you need is to type `make` inside project root - then encoder and decoder will be placed into **bin** directory.

#### cmake

Create create **build** directory where it convinient for you and change current directory to this one. Type `cmake <project-root-path>` and then if all rigth `cmake --build .`

### Use haffman encode

If you built **encode** with make run  it as following:

```
> bin/encode file-to-encode encoded-file-name
```

**file-to-encode** will be encoded by 5Mb blocks into **encoded-file-name**

### Use haffman decode

If you built **decode** with make run  it as following:

```
> bin/decode encoded-file-name decoded-file-name
```

**encoded-file-name** will be decoded block by block as you encoded it previously. If you try to decode wrong file it won't be decoded with error message.

###### That's all

