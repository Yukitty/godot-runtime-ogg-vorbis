# Runtime Ogg Vorbis
This GDExtension implements libogg at runtime for Godot 4.1+.

## Usage
### Static methods:
```
ResourceLoaderOggVorbis.load(path : String) -> AudioStreamOggVorbis
ResourceLoaderOggVorbis.load_buffer(buffer : PackedByteArray) -> AudioStreamOggVorbis
```

Call one of the load functions on any loose ogg file or ogg buffer in memory to decode it into an AudioStream.

Returns null on error.

You are responsible for setting up loop points and cacheing the AudioStream yourself.

