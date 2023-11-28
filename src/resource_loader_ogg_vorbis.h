#pragma once

#include <godot_cpp/classes/audio_stream_ogg_vorbis.hpp>

class ResourceLoaderOggVorbis : public Object {
	GDCLASS(ResourceLoaderOggVorbis, Object);

	enum {
		OGG_SYNC_BUFFER_SIZE = 8192,
	};

protected:
	static void _bind_methods();

public:
	static Ref<AudioStreamOggVorbis> load(const String &p_path);
	static Ref<AudioStreamOggVorbis> load_buffer(const PackedByteArray &buffer);
};

