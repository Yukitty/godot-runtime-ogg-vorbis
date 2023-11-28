# Runtime Ogg Vorbis
This GDExtension implements libogg at runtime for Godot 4.1.3

## Usage
### Static methods:
```
ResourceLoaderOggVorbis.load(path : String) -> AudioStreamOggVorbis
ResourceLoaderOggVorbis.load_buffer(buffer : PackedByteArray) -> AudioStreamOggVorbis
```

Call one of the load functions on any path to a loose ogg file or ogg memory buffer to decode it into a usable AudioStream.

Returns `null` on error.

Your app is responsible for cacheing the returned AudioStream and enabling looping as necessary.

## License
This GDExtension is released under a permissive MIT license (see LICENSE.md),
but makes use of the OggVorbis software codec source code which is
Copyright ©️ 1994-2019 by the Xiph.Org Foundation https://www.xiph.org/
